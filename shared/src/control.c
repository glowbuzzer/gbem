/**
 ******************************************************************************
 * @file           : control.c
 * @brief          : main control functions - runs drive state machine
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "shared.h"
#include "stateMachine.h"
#include "log.h"
#include "dpm.h"
#include "fault_cause.h"
#include "control.h"
#include "user_message.h"
#include "cyclic_message.h"
#include "plc_core.h"
#include "iomap_processing.h"
#include "ecrxtx.h"
#include "ethercatsetget.h"
#include "cia402.h"
#include "read_drive_error_code_into_ecm_status.h"
#include "print_slave_error_messages.h"
#include "status_control_word_bit_definitions.h"
#include "ec_functions.h"
#include "user_message.h"
#include "shared_mem_types.h"
#include "print_status.h"

//todo review need for semaphores in control.c and ec_rxtx.c
#define DPM_IN_PROTECT_START
#define DPM_IN_PROTECT_END
#define DPM_OUT_PROTECT_START
#define DPM_OUT_PROTECT_END


/* Types of sm events */
enum eventType {
    Event_cyclic,
};

event_data_t event_data = {};
uint32_t gbem_heartbeat = 0;
bool estop = true;
cia_state_t current_state = CIA_NOT_READY_TO_SWITCH_ON;


//@formatter:off
cyclic_event_t control_event[NUM_CONTROL_EVENTS] = {
        [CONTROL_EVENT_ESTOP] = {.message="An error has been detected [ESTOP event]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_FAULT] = {.message="An error has been detected [Drive fault]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_GBC_FAULT_REQUEST] = {.message="An error has been detected [GBC requesting fault state]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_GBC_INTERNAL_FAULT] = {.message="An error has been detected [GBC internal fault]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_HEARTBEAT_LOST] = {.message="An error has been detected [Heartbeat to GBC lost]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_LIMIT_REACHED] = {.message="An error has been detected [Drive reached limit]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_STATE_CHANGE_TIMEOUT] = {.message="An error has been detected [Drive state change timeout]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_FOLLOW_ERROR] = {.message="An error has been detected [Drive follow error too large]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_NO_REMOTE] = {.message="An error has been detected [Drive lost remote]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_ECAT_ERROR] = {.message="An error has been detected [EtherCAT network]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_ALARM] = {.message="An error has been detected [Drive alarm]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_MOOERROR] = {.message="An error has been detected [modes of operation]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_ECAT_SLAVE_ERROR] = {.message="An error has been detected [EtherCAT slave]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_PLC_SIGNALLED_ERROR] = {.message="An error has been detected [PLC signalled an error]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_HOMING_ERROR] = {.message="\"An error has been detected [Homing error]", .type=CYCLIC_MSG_ERROR},
};

//@formatter:on


/**global variable to check how long drives take to response to state change request */
uint32_t ctrl_state_change_cycle_count = 1;
uint32_t ctrl_state_change_timeout = (CTRL_DRIVE_CHANGE_STATE_TIMEOUT * MAP_CYCLE_TIME);


/** FSoE functions */
void copy_fsoe_data(void);

gberror_t update_fsoe_ecm_status(void);


/* functions to copy between EC and DPM */
static void ctrl_copy_actpos(void);

static void ctrl_copy_actvel(void);

static void ctrl_copy_acttorq(void);

static void ctrl_copy_control_effort(void);

static void ctrl_copy_setpos(void);

static void ctrl_copy_values_to_drives(uint64_t cycle_count, cia_state_t current_state);

static void ctrl_copy_drive_statuswords(void);

/* functions to force gbc and pdos */
void ctrl_process_forces_in(void);

void ctrl_process_forces_out(void);

static void ctrl_check_for_big_pos_jump(uint16_t drive, int32_t current_position);

static void ctrl_set_moo_pdo(void);

/* sm guard functions */
static bool cia_trn13_guard(void *condition, struct event *event);

static bool cia_trn14_guard(void *condition, struct event *event);

static bool cia_trn2_guard(void *condition, struct event *event);

static bool cia_trn3_guard(void *condition, struct event *event);

static bool cia_trn4_guard(void *condition, struct event *event);

static bool cia_trn5_guard(void *condition, struct event *event);

static bool cia_trn6_guard(void *condition, struct event *event);

static bool cia_trn7_guard(void *condition, struct event *event);

static bool cia_trn8_guard(void *condition, struct event *event);

static bool cia_trn9_guard(void *condition, struct event *event);

static bool cia_trn10_guard(void *condition, struct event *event);

static bool cia_trn11_guard(void *condition, struct event *event);

static bool cia_trn12_guard(void *condition, struct event *event);

static bool cia_trn15_guard(void *condition, struct event *event);

static bool cia_trn16_guard(void *condition, struct event *event);

/* sm entry action functions */
static void cia_generic_entry_action(void *stateData, struct event *event);

static void sm_error_state_entry_action(void *stateData, struct event *event);

/* sm transition action functions */
static void cia_set_current_fault_causes_action(void *oldStateData, struct event *event, void *newStateData);

/* Forward declaration of sm states so that they can be defined in a logical order: */
static struct state cia_not_ready_to_switch_on_state, cia_switch_on_disabled_state, cia_ready_to_switch_on_state,
        cia_switched_on_state, cia_operation_enabled_state, cia_quick_stop_active_state,
        cia_fault_reaction_active_state, cia_fault_state, sm_error_state;

static struct state cia_not_ready_to_switch_on_state = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[])
    {
        {
            Event_cyclic,
            (void *) ((intptr_t) CIA_NOT_READY_TO_SWITCH_ON),
            &cia_trn13_guard,
            NULL,
            &cia_fault_reaction_active_state
        },
        {
            Event_cyclic,
            NULL,
            NULL,
            NULL,
            &cia_switch_on_disabled_state
        },
    },
    .numTransitions = 2,
    .data = (void *) CIA_NOT_READY_TO_SWITCH_ON,
    .entryAction = &cia_generic_entry_action,
    .exitAction =
    NULL,
};

static struct state cia_switch_on_disabled_state = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[])
    {
        {
            Event_cyclic,
            (void *) ((intptr_t) CIA_SWITCH_ON_DISABLED),
            &cia_trn13_guard,
            NULL,
            &cia_fault_reaction_active_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn2_guard,
            NULL,
            &cia_ready_to_switch_on_state
        },
    },
    .numTransitions = 2,
    .data = (void *) CIA_SWITCH_ON_DISABLED,
    .entryAction = &cia_generic_entry_action,
    .exitAction = NULL,
};

static struct state cia_ready_to_switch_on_state = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[])
    {
        {
            Event_cyclic,
            (void *) ((intptr_t) CIA_READY_TO_SWITCH_ON),
            &cia_trn13_guard,
            NULL,
            &cia_fault_reaction_active_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn3_guard,
            NULL,
            &cia_switched_on_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn7_guard,
            NULL,
            &cia_switch_on_disabled_state
        },
    },
    .numTransitions = 3,
    .data = (void *) CIA_READY_TO_SWITCH_ON,
    .entryAction = &cia_generic_entry_action,
    .exitAction = NULL,
};

static struct state cia_switched_on_state = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[])
    {
        {
            Event_cyclic,
            (void *) ((intptr_t) CIA_SWITCHED_ON),
            &cia_trn13_guard,
            NULL,
            &cia_fault_reaction_active_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn6_guard,
            NULL,
            &cia_ready_to_switch_on_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn4_guard,
            NULL,
            &cia_operation_enabled_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn10_guard,
            NULL,
            &cia_switch_on_disabled_state
        },
    },
    .numTransitions = 4,
    .data = (void *) CIA_SWITCHED_ON,
    .entryAction = &cia_generic_entry_action,
    .exitAction = NULL,
};

static struct state cia_operation_enabled_state = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[])
    {
        {
            Event_cyclic,
            (void *) ((intptr_t) CIA_OPERATION_ENABLED),
            &cia_trn13_guard,
            NULL,
            &cia_fault_reaction_active_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn5_guard,
            NULL,
            &cia_switched_on_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn8_guard,
            NULL,
            &cia_ready_to_switch_on_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn9_guard,
            NULL,
            &cia_switch_on_disabled_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn11_guard,
            NULL,
            &cia_quick_stop_active_state
        },
    },
    .numTransitions = 5,
    .data = (void *) CIA_OPERATION_ENABLED,
    .entryAction = &cia_generic_entry_action,
    .exitAction = NULL,
};

static struct state cia_quick_stop_active_state = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[])
    {
        {
            Event_cyclic,
            (void *) ((intptr_t) CIA_QUICK_STOP_ACTIVE),
            &cia_trn13_guard,
            NULL,
            &cia_fault_reaction_active_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn16_guard,
            NULL,
            &cia_operation_enabled_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn12_guard,
            NULL,
            &cia_switch_on_disabled_state
        },
    },
    .numTransitions = 3,
    .data = (void *) CIA_QUICK_STOP_ACTIVE,
    .entryAction = &cia_generic_entry_action,
    .exitAction = NULL,
};

//this state has a unguarded transition to itself with a transition action to set the current fault cause bits
//when a state transitions to itself, entry and exit actions are not called
static struct state cia_fault_reaction_active_state = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[])
    {
        {
            Event_cyclic,
            NULL,
            &cia_trn14_guard,
            NULL,
            &cia_fault_state
        },
        {
            Event_cyclic,
            NULL,
            NULL,
            &cia_set_current_fault_causes_action,
            &cia_fault_reaction_active_state
        },
    },
    .numTransitions = 2,
    .data = (void *) CIA_FAULT_REACTION_ACTIVE,
    .entryAction = &cia_generic_entry_action,
    .exitAction = NULL,
};

static struct state cia_fault_state = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[])
    {
        {
            Event_cyclic,
            (void *) ((intptr_t) CIA_FAULT),
            &cia_trn13_guard,
            NULL,
            &cia_fault_reaction_active_state
        },
        {
            Event_cyclic,
            NULL,
            &cia_trn15_guard,
            NULL,
            &cia_switch_on_disabled_state
        },
    },
    .numTransitions = 2,
    .data = (void *) CIA_FAULT,
    .entryAction = &cia_generic_entry_action,
    .exitAction =
    NULL,
};

__attribute__((unused)) static struct state errorState = {
    .entryAction = &sm_error_state_entry_action
};


// Function to check if the buffer is empty
int ec_is_empty_circular_slave_error_message(ec_circular_slave_error_message_t *c) {
    return (c->num_slots_full == 0);
}

// Function to check if the buffer is full
int ec_is_full__circular_slave_error_message(ec_circular_slave_error_message_t *c) {
    return (c->num_slots_full == MAX_NUM_SLAVE_ERROR_MESSAGES);
}

/**
 * @brief copies an error message into a circular buffer
 * @param c pointer to circular buffer
 * @param slave_error_message (message to add to buffer)
 */
void ec_push_circular_slave_error_message(ec_circular_slave_error_message_t *c, uint8_t *slave_error_message) {
    if (ec_is_full__circular_slave_error_message(c)) {
        // If the buffer is full, overwrite the oldest message
        c->head = (c->head + 1) % MAX_NUM_SLAVE_ERROR_MESSAGES;
    }

    c->tail = (c->tail + 1) % MAX_NUM_SLAVE_ERROR_MESSAGES;
    strncpy((char *) c->slave_error_message[c->tail], (char *) slave_error_message, EC_MAXERRORNAME - 1);
    c->slave_error_message[c->tail][EC_MAXERRORNAME - 1] = '\0'; // Ensure null-terminated string

    if (!ec_is_full__circular_slave_error_message(c)) {
        c->num_slots_full++;
    }
};

/**
 * @brief peek at the circular queue of error message
 * @param c pointer to circular buffer
 * @param slave_error_message_number number of message in queue
 * @return pointer to error message
 */
uint8_t *
ec_peek_circular_slave_error_message(ec_circular_slave_error_message_t *c, uint8_t slave_error_message_number) {
    return c->slave_error_message[slave_error_message_number];
}


/**
 * @brief Guards transition from Switch on disabled to Ready to switch on - expected machine controlword: SHUTDOWN. Commands drives with CIA_SHUTDOWN_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn2_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN2 Guard (Switch on disabled to Ready to switch on)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SHUTDOWN) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN2 Guard - machine command matches Shutdown");
        if (ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON)) {
            LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN2 Guard - All drives are ready to switch on - return true");
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_SHUTDOWN_CTRLWRD);
            LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN2 Guard - changing drive states with a shutdown controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN2 Guard - machine command is not Shutdown");
    return false;
}

/**
 * @brief Guards transition from Ready to switch on to Switched on - expected machine controlword: SWITCH_ON or SWITCH_ON_AND_ENABLE_OPERATION. Commands drives with CIA_SWITCH_ON_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn3_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN3 Guard (Ready to switch on to Switched on)");
    if ((cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SWITCH_ON)) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN3 Guard - machine command matches Switch on");
        if (ctrl_check_all_drives_state(CIA_SWITCHED_ON)) {
            LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN3 Guard - All drives are switched on - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_SWITCH_ON_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN3 Guard - changing drive states with a Switch on controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN3 Guard - machine command is not Switch on");
    return false;
}

/**
 * @brief Guards transition from Switched on to Operation enabled - expected machine controlword: ENABLE_OPERATION or SWITCH_ON_AND_ENABLE_OPERATION. Commands drives with CIA_ENABLE_OPERATION_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn4_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN4 Guard (Switched on to Operation enabled)");
    if ((cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_ENABLE_OPERATION)) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN4 Guard - machine command matches Enable operation");
        if (ctrl_check_all_drives_state(CIA_OPERATION_ENABLED)) {
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN4 Guard - All drives are Operation enabled - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_ENABLE_OPERATION_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN4 Guard - changing drive states with a Enabled operation controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN4 Guard - machine command is not Enable operation");
    return false;
}

/**
 * @brief Guards transition from Operation enabled to Switched on - expected machine controlword: DISABLE_OPERATION >>OR<< SWITCH_ON. Commands drives with CIA_DISABLE_OPERATION_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn5_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN5 Guard (Operation enabled to Switched on)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_DISABLE_OPERATION ||
        cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SWITCH_ON) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN5 Guard - machine command matches Disable operation");
        if (ctrl_check_all_drives_state(CIA_SWITCHED_ON)) {
            LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN5 Guard - All drives are Switched on - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_DISABLE_OPERATION_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN5 Guard - changing drive states with a Disabled operation controlword (switch on controword is same)")
            ;
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN5 Guard - Machine command is not Disable operation");
    return false;
}

/**
 * @brief Guards transition from Switched on to Ready to switch on - expected machine controlword: SHUTDOWN. Commands drives with CIA_SHUTDOWN_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn6_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN6 Guard (Switched on to Ready to switch on)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SHUTDOWN) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN6 Guard - machine command matches Shutdown");
        if (ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON)) {
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN6 Guard - All drives are Ready to switch on - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_SHUTDOWN_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN6 Guard - changing drive states with a Shutdown controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN6 Guard - Machine command is not Shutdown");
    return false;
}

/**
 * @brief Guards transition from Ready to switch on to Switch on disabled - expected machine controlword: DISABLE_VOLTAGE OR QUICK_STOP. Commands drives with CIA_DISABLE_VOLTAGE_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn7_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN7 Guard (Ready to switch on to Switch on disabled)");
    if ((cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_DISABLE_VOLTAGE) ||
        (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_QUICK_STOP)) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN7 Guard - Machine command matches Disable voltage");
        if (ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN7 Guard - All drives are Switch on disabled - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_DISABLE_VOLTAGE_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN7 Guard - changing drive states with a Disabled voltage controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN7 Guard - Machine command is not Disable voltage");
    return false;
}

/**
 * @brief Guards transition from Operation enabled to Ready to switch on - expected machine controlword: SHUTDOWN. Commands drives with CIA_SHUTDOWN_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn8_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN8 Guard (Operation enabled to Ready to switch on)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SHUTDOWN) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN8 Guard - Machine command matches Shutdown");
        if (ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON)) {
            ctrl_state_change_cycle_count = 0;
            LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN8 Guard - All drives are Ready to swithc on - return true");
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_SHUTDOWN_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN8 Guard - changing drive states with a Shutdown controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN8 Guard - Machine command is not Shutdown");
    return false;
}

/**
 * @brief Guards transition from Operation enabled to Switch on disabled - expected machine controlword: DISABLE_VOLTAGE. Commands drives with CIA_DISABLE_VOLTAGE_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn9_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN9 Guard (Operation enabled to Switch on disabled)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_DISABLE_VOLTAGE) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN9 Guard - Machine command matches Disable voltage");
        if (ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN9 Guard - All drives are Switch on disabled - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_DISABLE_VOLTAGE_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN9 Guard - changing drive states with a Disable voltage controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN9 Guard - Machine command is not Disable voltage");
    return false;
}

/**
 * @brief Guards transition from Switched on to Switch on disabled - expected machine controlword: DISABLE_VOLTAGE or QUICK_STOP. Commands drives with CIA_DISABLE_VOLTAGE_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn10_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN10 Guard (Switched on to Switch on disabled)");
    if ((cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_DISABLE_VOLTAGE) ||
        (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_QUICK_STOP)) {
        LL_TRACE(GBEM_SM_LOG_EN,
                 "sm: TRN10 Guard - Machine command matches Disable voltage OR Quick stop");
        if (ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN10 Guard - All drives are Switch on disabled - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_DISABLE_VOLTAGE_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN10 Guard - changing drive states with a Disable voltage controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN10 Guard - Machine command is not Disable voltage");
    return false;
}


/**
 * @brief Guards transition from Operation Enabled to Quick stop active - expected machine controlword: QUICK_STOP. Commands drives with CIA_QUICK_STOP_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */

static bool cia_trn11_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN11 Guard (Operation Enabled to Quick stop active)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_QUICK_STOP) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN11 Guard - Machine command matches Quick stop");
        if (ctrl_check_all_drives_state(CIA_QUICK_STOP_ACTIVE) || ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN11 Guard - All drives are Quick stop - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_QUICK_STOP_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN11 Guard - changing drive states with a Quick stop controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN11 Guard - Machine command is not Quick stop");
    return false;
}


/**
 * @brief Guards transition from Quick stop active to Switch on disabled - expected machine controlword: DISABLE_VOLTAGE. Commands drives with CIA_DISABLE_VOLTAGE_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn12_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN12 Guard (Quick stop active to Switch on disabled)");
    if (ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
        LL_TRACE(GBEM_SM_LOG_EN,
                 "sm: TRN12 Guard - All drives are Switch on disabled - return true");
        ctrl_state_change_cycle_count = 0;
        return true;
    } else {
        //			ctrl_change_all_drives_states(CIA_DISABLE_VOLTAGE_CTRLWRD);
        ctrl_state_change_cycle_count++;
        //			LL_TRACE(SM_LOG_EN, ecm_status.cycle_count, "sm: TRN12 Guard - changing drive states with a Disable voltage controlword");
        return false;
    }
    //	}
    //	LL_TRACE(SM_LOG_EN,  "sm: TRN12 Guard - Machine command is not Disable voltage");
    //	return false;
}


/**
 * @brief Guards transition from Fault to Switch on disabled - expected machine controlword: FAULT_RESET. Commands drives with CIA_FAULT_RESET_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn15_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN15 Guard (Fault to Switch on disabled)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_FAULT_RESET) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN15 Guard - Machine command matches Fault reset");
        if (ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN15 Guard - All drives are Switch on disabled - return true");
            ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_FAULT_RESET_CTRLWRD);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN15 Guard - changing drive states with a Fault reset controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN15 Guard - Machine command is not Fault reset");
    return false;
}

/**
 * @brief Guards transition from Quick stop active to Operation enabled - expected machine controlword: ENABLE_OPERATION. Commands drives with CIA_ENABLE_OPERATION
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn16_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN16 Guard (Quick stop active to Operation enabled)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_ENABLE_OPERATION) {
        LL_TRACE(GBEM_SM_LOG_EN,
                 "sm: TRN16 Guard - Machine command matches Enable operation");
        if (ctrl_check_all_drives_state(CIA_OPERATION_ENABLED)) {
            ctrl_state_change_cycle_count = 0;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN16 Guard - All drives are Operation enabled - return true");
            return true;
        } else {
            ctrl_change_all_drives_states(CIA_ENABLE_OPERATION);
            ctrl_state_change_cycle_count++;
            LL_TRACE(GBEM_SM_LOG_EN,
                     "sm: TRN15 Guard - changing drive states with a Enabled operation controlword");
            return false;
        }
    }
    LL_TRACE(GBEM_SM_LOG_EN, "sm: TRN16 Guard - Machine command is not Enable operation");
    return false;
}

/**
 * @brief Checks heartbeat stop, estop=true, follow_error=true, drives in fault or fault reaction active, machine controlword = fault trigger
 * @param event
 * @return true there is a fault condition, false no fault condition
 * @warning doesn't check for drive states being out of sync
 */
bool cia_is_fault_condition(struct event *event) {
    static uint64_t wrong_moo_count[MAP_NUM_DRIVES] = {0};

    ((event_data_t *) event->data)->fault_cause = 0;

    //reset all control events (errors)
    for (int i = 0; i < NUM_CONTROL_EVENTS; i++) {
        control_event[i].active = false;
    }


    bool have_fault = false;
    if (((event_data_t *) event->data)->heartbeat_lost == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > heart beat lost");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_HEARTBEAT_LOST_BIT_NUM);
        control_event[CONTROL_EVENT_HEARTBEAT_LOST].active = true;
        have_fault = true;
    }
    if (((event_data_t *) event->data)->follow_error == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > follow error");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_FOLLOW_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_FOLLOW_ERROR].active = true;
        have_fault = true;
    }
    if (((event_data_t *) event->data)->estop == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > estop");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_ESTOP_BIT_NUM);
        control_event[CONTROL_EVENT_ESTOP].active = true;
        have_fault = true;
    }

    if (((event_data_t *) event->data)->machine_request_error == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > machine word is requesting error state");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_GBC_FAULT_REQUEST_BIT_NUM);
        control_event[CONTROL_EVENT_GBC_FAULT_REQUEST].active = true;
        have_fault = true;
    }

    if (((event_data_t *) event->data)->gbc_internal_fault == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > machine word is signalling GBC had an operation errror");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_GBC_OPERATION_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_GBC_INTERNAL_FAULT].active = true;
        have_fault = true;
    }

    if (ctrl_check_any_drives_state(CIA_FAULT_REACTION_ACTIVE)) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > one or more drives are in FAULT REACTION ACTIVE");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_FAULT_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_FAULT].active = true;
        have_fault = true;
    }
    if (ctrl_check_any_drives_state(CIA_FAULT)) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > one or more drives are in FAULT");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_FAULT_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_FAULT].active = true;
        have_fault = true;
    }
    if ((ctrl_state_change_cycle_count * MAP_CYCLE_TIME) > ctrl_state_change_timeout) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > one or more drives took too long responding to a state change request");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_STATE_CHANGE_TIMEOUT_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_STATE_CHANGE_TIMEOUT].active = true;
        have_fault = true;
    }
    if (((event_data_t *) event->data)->remote_ok == false) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > drives not signalling remote ok");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_NO_REMOTE_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_NO_REMOTE].active = true;
        have_fault = true;
    }
    if (((event_data_t *) event->data)->ec_check_error == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > ecat check detected error");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_ECAT_BIT_NUM);
        control_event[CONTROL_EVENT_ECAT_ERROR].active = true;
        have_fault = true;
    }
    if (((event_data_t *) event->data)->any_drive_has_alarm == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > a drive is signalling an any_drive_has_alarm");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_ALARM_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_ALARM].active = true;
        have_fault = true;
    }

    if (((event_data_t *) event->data)->internal_limit == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > a drive has hit a limit");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_LIMIT_REACHED_BIT_NUM);
        control_event[CONTROL_EVENT_LIMIT_REACHED].active = true;
        have_fault = true;
    }

    if (((event_data_t *) event->data)->slave_reported_error == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > a slave reported an error");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_ECAT_SLAVE_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_ECAT_SLAVE_ERROR].active = true;
        have_fault = true;
    }

    if (((event_data_t *) event->data)->plc_signalled_error == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > the PLC signalled an error");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_PLC_SIGNALLED_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_ECAT_SLAVE_ERROR].active = true;
        have_fault = true;
    }

    if (((event_data_t *) event->data)->homing_failed == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > the homing operation failed for one or more drives");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_HOMING_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_HOMING_ERROR].active = true;
        have_fault = true;
    }


    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        //        printf("moodisp event data:%i\n", ((event_data_t *) event->data)->moo_disp[i]);
        if ((((event_data_t *) event->data)->moo_disp[i]) != map_drive_moo[i]) {
            wrong_moo_count[i]++;
        } else {
            wrong_moo_count[i] = 0;
        }

        if (wrong_moo_count[i] > 5) {
            LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > a drive is signalling a moo issue on drive:%d", i);
            BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_MOOERROR_BIT_NUM);
            control_event[CONTROL_EVENT_DRIVE_MOOERROR].active = true;
            have_fault = true;
        }
    }

    dpm_in->active_fault_word = ((event_data_t *) event->data)->fault_cause;

    return have_fault;
}

/**
 * @brief Guards transition from any state into fault reaction active - expected machine controlword drive this (apart from the trigger fault bit)
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn13_guard(void *condition, struct event *event) {
    if (cia_is_fault_condition(event)) {
        //a QUICK_STOP command should send all drives back to switch on disabled
        ctrl_change_all_drives_states(CIA_QUICK_STOP_CTRLWRD);
        return true;
    }
    // condition will be the state that we are currently in


    //todo crit remove this
    return false;

    cia_state_t drive_state = 0;
    bool found_drive_state = false;

    if (ctrl_check_all_drives_state(CIA_NOT_READY_TO_SWITCH_ON)) {
        drive_state = CIA_NOT_READY_TO_SWITCH_ON;
        printf("drive state is CIA_NOT_READY_TO_SWITCH_ON\n");
        found_drive_state = true;
    }
    if (ctrl_check_all_drives_state(CIA_OPERATION_ENABLED)) {
        drive_state = CIA_OPERATION_ENABLED;
        found_drive_state = true;
        printf("drive state is CIA_OPERATION_ENABLED\n");
    }
    if (ctrl_check_all_drives_state(CIA_SWITCHED_ON)) {
        drive_state = CIA_SWITCHED_ON;
        found_drive_state = true;
        printf("drive state is CIA_SWITCHED_ON\n");
    }
    if (ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON)) {
        drive_state = CIA_READY_TO_SWITCH_ON;
        found_drive_state = true;
        printf("drive state is CIA_READY_TO_SWITCH_ON\n");
    }
    if (ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
        drive_state = CIA_SWITCH_ON_DISABLED;
        found_drive_state = true;
        printf("drive state is CIA_SWITCH_ON_DISABLED\n");
    }
    if (ctrl_check_all_drives_state(CIA_QUICK_STOP_ACTIVE)) {
        drive_state = CIA_QUICK_STOP_ACTIVE;
        found_drive_state = true;
        printf("drive state is CIA_QUICK_STOP_ACTIVE\n");
    }
    if (ctrl_check_all_drives_state(CIA_FAULT)) {
        drive_state = CIA_FAULT;
        found_drive_state = true;
        printf("drive state is CIA_FAULT\n");
    }
    if (ctrl_check_all_drives_state(CIA_FAULT_REACTION_ACTIVE)) {
        drive_state = CIA_FAULT_REACTION_ACTIVE;
        found_drive_state = true;
        printf("drive state is CIA_FAULT_REACTION_ACTIVE\n");
    }

    if (!found_drive_state) {
        UM_FATAL("GBEM: Could not establish drive state");
    }

    /*
     * we are in op enabled
     * controlword is switch on disabled
     * drives are in quick stop
     * cycle change count will be ticking up
     * eventually if state does not match the controlword we will error (state change timeout)
     *
     * BUT
     * if we are in op enabled
     * controlword is enable op
     * drive drops to switch on disabled
     *
     *
     */
    bool state_mismatch = false;

    printf("machine word [%u] command [%u]\n", dpm_out->machine_word, cia_ctrlwrd_to_command(dpm_out->machine_word));

    /*fault if:
    op enabled and state is not op enabled
    */


    switch (drive_state) {
        //    uint32_t x=(uint32_t) ((intptr_t) condition & 0xFFFFFFFF);


        case CIA_READY_TO_SWITCH_ON:
            if (((cia_state_t) ((void *) condition) != CIA_READY_TO_SWITCH_ON) &&
                (ctrl_state_change_cycle_count * MAP_CYCLE_TIME > ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
        case CIA_SWITCHED_ON:
            if (((cia_state_t) ((uintptr_t *) condition) != CIA_SWITCHED_ON) &&
                (ctrl_state_change_cycle_count * MAP_CYCLE_TIME > ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
        //        case CIA_SWITCH_ON_AND_ENABLE_OPERATION:
        //            if ((((cia_state_t) ((intptr_t *) condition) != CIA_SWITCHED_ON) ||
        //                 ((cia_state_t) ((intptr_t *) condition) != CIA_OPERATION_ENABLED))
        //                && ctrl_state_change_cycle_count*MAP_CYCLE_TIME > ctrl_state_change_timeout) {
        //                state_mismatch = true;
        //            }
        //            break;
        case CIA_QUICK_STOP_ACTIVE:
            if (((cia_state_t) ((intptr_t *) condition) != CIA_QUICK_STOP_ACTIVE) &&
                ctrl_state_change_cycle_count * MAP_CYCLE_TIME > ctrl_state_change_timeout) {
                state_mismatch = true;
            }
            break;
        case CIA_OPERATION_ENABLED:
            printf("case CIA_OPERATION_ENABLED - %u\n", CIA_OPERATION_ENABLED);
            printf("%u condition\n", (((cia_state_t) ((intptr_t *) condition))));
            printf("state change timeout [%u]\n", ctrl_state_change_cycle_count * MAP_CYCLE_TIME);

            if (((cia_state_t) ((intptr_t *) condition)) != CIA_OPERATION_ENABLED) {
                state_mismatch = true;
            }
            break;
        case CIA_FAULT_RESET:
            break;
    }
    if (state_mismatch) {
        LL_TRACE(GBEM_SM_LOG_EN,
                 "sm: Mismatch between drive state and the controlword and the current state & no transitions occurring")
        ;
        ctrl_change_all_drives_states(CIA_QUICK_STOP_CTRLWRD);
        return false;
    }
    //	!! we transition to fault reaction active then loop in that state until drives have stopped
    return false;
}

//check that the drives are in sync with the statemachine state

//we have the problem here that drives might be one step ahead of the state machine or one step behind

/*
 * we are in state op enabled
 * control word is disabled voltage
 * we set drive control words to disable voltage
 * state remains op enabled until drives report switch on disabled
 *
 */


/**
 * @brief Guards the transition from fault reaction active to fault that occurs when a fault is cleared - doesn't depend on machine controlword
 * @param condition
 * @param event
 * @return
 */
static bool cia_trn14_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter

    static int reset_in_progress = 0;
    if (ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
        // all drives have transitioned to switch on so the state machine can go to fault state if no error
        if (!cia_is_fault_condition(event)) {
            return true;
        }
    } else {
        reset_in_progress++;
        if ((reset_in_progress % 5) == 0) {
            //ask all drives to fault reset which should send them to SWITCH ON DISABLED
            ctrl_change_all_drives_states(CIA_FAULT_RESET_CTRLWRD);
            //            printf("reset\n");
        } else {
            //provides a bit transition for the drives with the fault reset
            ctrl_change_all_drives_states(0b00000000);
            //            printf("reset 0\n");
        }
    }
    return false;
}

static void sm_error_state_entry_action(void *stateData, struct event *event) {
    (void) stateData; //unused parameter
    (void) event; //unused parameter

    LL_FATAL("GBEM: Reached error state of statemachine");
}

//GBEM sends the data from dpmIN over spi (this is the IN i.e. stuff read from fieldbus or created by state machine)
//GBEM receives data into DPMout over spi (this is the OUT i.e. stuff to be written to the fieldbus or be processed by the state machine)

/**
 * @brief checks all drives against a provided state
 * @param drive_status array of drive statuses
 * @param state to check
 * @return true=all drives are in the state, one or more drives are not in the state
 */
bool ctrl_check_all_drives_state(cia_state_t state) {
    int j = 0;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        uint16_t drive_stat_wrd;
        if (*map_drive_get_stat_wrd_function_ptr[i] != NULL) {
            drive_stat_wrd = map_drive_get_stat_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_get_stat_wrd on drive [%u]",
                     i);
            return false;
        }
        if (cia_statwrd_to_state(drive_stat_wrd) == state) {
            j++;
        }
    }
    if (j == MAP_NUM_DRIVES) {
        return true;
    } else {
        return false;
    }
}


bool ctrl_check_any_drives_state(cia_state_t state) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        uint16_t drive_stat_wrd;
        if (*map_drive_get_stat_wrd_function_ptr[i] != NULL) {
            drive_stat_wrd = map_drive_get_stat_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_get stat_wrd on drive [%u]",
                     i);
            continue;
        }
        if (cia_statwrd_to_state(drive_stat_wrd) == state) {
            //we found a drive in the given state
            return true;
        }
    }
    //we didn't find any drives in the given state
    return false;
}


/**
 * @brief sends a given controlword to all drives
 * @param controlword
 * @param event
 */
void ctrl_change_all_drives_states(uint16_t controlword) {
    LL_TRACE(GBEM_SM_LOG_EN, "sm: Change All drives control word: %s",
             cia_command_names[cia_ctrlwrd_to_command(controlword)]);
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        gberror_t grc;
        if (*map_drive_set_ctrl_wrd_function_ptr[i] != NULL) {
            grc = map_drive_set_ctrl_wrd_function_ptr[i](i, controlword);
            ecm_status.drives[i].command = cia_ctrlwrd_to_command(controlword);

            LL_TRACE(GBEM_SM_LOG_EN, "sm: Drive: %u, Command: %s", i,
                     cia_command_names[cia_ctrlwrd_to_command(controlword)]);
            if (grc != E_SUCCESS) {
                LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive set ctrl wrd function error [%s]", gb_strerror(grc));
            }
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_set_ctrl_wrd on drive [%u]",
                     i);
        }

        //dpm.out.joint_controlword gets overwritten by GBC whereas here we are using the statemachine to control teh drives - this is important
    }
}

/**
 * @brief action called by an unguarded transition on the fault reaction active state to set the current fault cause bits
 * @param oldStateData
 * @param event
 * @param event
 * @param newStateData
 * @attention this is set every cycle in fault reaction active - it is current fault cause
 */
static void cia_set_current_fault_causes_action(void *oldStateData, struct event *event, void *newStateData) {
    (void) oldStateData; //unused parameter
    (void) newStateData; //unused parameter

    /*this will record in the event struct the fault bits */
    cia_is_fault_condition(event);

    //        dpm_in->active_fault_word = ((event_data_t *) event->data)->fault_cause;
}

/**
 * @brief entry action called when the state machine enters a state for the first time
 * @param stateData
 * @param event
 */
static void cia_generic_entry_action(void *stateData, struct event *event) {
    //The drives have been commanded to change state and checked they have changed state in the transition
    LL_TRACE(GBEM_SM_LOG_EN, "sm: Entry action: current state = %s", cia_state_names[(int) ((cia_state_t) stateData)]);

    /* We need to:
     * a) set the ecm_status overall sm state (in the shared mem struct)
     * b) set the ecm_status drive statuses (in the shared mem struct)
     * c) set the machine status word (this is a write to the SPI DPM)
     * d)
     */
    //first set the overall current state of the gbem statemachine
    //it is ok to do this in the generic entry action as every time we change state we pass through here
    uint32_t bitmask_for_lower_16 = 0xFFFF0000;

    dpm_in->machine_word = dpm_in->machine_word & bitmask_for_lower_16;
    switch ((cia_state_t) stateData) {
        case CIA_NOT_READY_TO_SWITCH_ON:
            dpm_in->machine_word = dpm_in->machine_word | CIA_NOT_READY_TO_SWITCH_ON_STATWRD;
            break;
        case CIA_SWITCH_ON_DISABLED:
            dpm_in->machine_word = dpm_in->machine_word | CIA_SWITCH_ON_DISABLED_STATWRD;
            break;
        case CIA_READY_TO_SWITCH_ON:
            dpm_in->machine_word = dpm_in->machine_word | CIA_READY_TO_SWITCH_ON_STATWRD;
            break;
        case CIA_SWITCHED_ON:
            dpm_in->machine_word = dpm_in->machine_word | CIA_SWITCHED_ON_STATWRD;
            break;
        case CIA_OPERATION_ENABLED:
            dpm_in->machine_word = dpm_in->machine_word | CIA_OPERATION_ENABLED_STATWRD;
            break;
        case CIA_QUICK_STOP_ACTIVE:
            dpm_in->machine_word = dpm_in->machine_word | CIA_QUICK_STOP_ACTIVE_STATWRD;
            break;
        case CIA_FAULT_REACTION_ACTIVE:
            dpm_in->machine_word = dpm_in->machine_word | CIA_FAULT_REACTION_ACTIVE_STATWRD;
        /* trn13 should have called cia_is_fault_condition to set current faults in event struct*/
            dpm_in->fault_history_word = ((event_data_t *) event->data)->fault_cause;
            break;
        case CIA_FAULT:
            dpm_in->machine_word = dpm_in->machine_word | CIA_FAULT_STATWRD;
            break;
    }
}

/**
 * @brief malloc a state machine struct
 * @return ptr to stateMachine struct
 */
struct stateMachine *ctrl_statemachine_alloc(void) {
    struct stateMachine *smachine = malloc(sizeof(struct stateMachine));

    return smachine;
}

/**
 * @brief free a stateMachine struct
 * @param smachine
 */
void ctrl_statemachinefree(struct stateMachine *smachine) {
    free(smachine);
}


/**
 * @brief initialise the state machine so start in not ready to switch on
 * @param smachine
 */
void ctrl_init_statemachine(struct stateMachine *smachine) {
    stateM_init(smachine, &cia_not_ready_to_switch_on_state, &sm_error_state);
}

/**
 * @brief check heartbeat (incremented by GBEM and echoed by GBC)
 * @param gbem_heartbeat
 * @param gbc_heartbeat
 * @return true when heartbeat is within tolerance (CTRL_HEARTBEAT_TOLERANCE) return false when lost
 */
bool ctrl_check_heartbeat_ok(uint32_t gbem_heartbeat_to_check, uint32_t gbc_heartbeat_to_check) {
    if ((gbem_heartbeat_to_check - gbc_heartbeat_to_check) > CTRL_HEARTBEAT_TOLERANCE) {
        return false;
    }

    //    printf("check heartbeat (incremented by GBEM and echoed by GBC)\n");
    //    printf("gbem_heartbeat_to_check [%u]\n", gbem_heartbeat_to_check);
    //    printf("gbc_heartbeat_to_check [%u]\n", gbc_heartbeat_to_check);
    /*
     * cycle 1 - gbem send out 0 to gbc - gbc echos 0 back
     * cycle 2 - gbem send out 1 to gbc - gbc echos 0 back
     * cycle 2 - gbem send out 2 to gbc - gbc echos 1 back
     * cycle 3 - gbem send out 3 to gbc - gbc echos 2 back
     */
    return true;
}

//todo this function could do with moving out of this file - it is not part of control any more

/**
 * @brief if an EcatError is detected then copy the slave error message to the ecm_status struct
 * @warning this is called from the ec_check thread - every 500ms
 */
void ctrl_copy_slave_error_to_ecm_status(void) {
    uint8_t EcatError_read_count = 0;
    while (EcatError) {
        EcatError_read_count++;
        char *slave_error_msg = ec_elist2string();
        //        printf("slave_error_msg: %s\n", slave_error_msg);
        if (strlen(slave_error_msg) > 2) {
            ec_push_circular_slave_error_message(&ecm_status.slave_error_messages,
                                                 (uint8_t *) slave_error_msg);
        }
        osal_usleep(1000);
        if (EcatError_read_count > 30) {
            //todo crit
            //if an EtherCAT slave is just stuck in error we will be back here spewing out the same error message
            UM_ERROR(GBEM_UM_EN, "GBEM: EcatError_read_count > 30. We have too many slave error messages to read");
            break;
        }
    }
}

/**
 * @brief main control loop
 * @param m state machine object
 * @param first_run bool indicating that this is the first execution cycle
 * @param
 */
void ctrl_main(struct stateMachine *m, bool first_run) {
    /* in terms of the semaphores, a 32 bit read can't result in a torn read and a 32bit write cant result in a torn write
     * from arm manual:
     * All byte accesses
     * All halfword accesses to halfword aligned addresses
     * All word accesses to word aligned addresses
     * So do we really need these semaphores? We could also get half the data from one cycle and half teh data from a previous cycle - hard to analyses all impacts of this
     * But we are memcpying a packed struct but the struct lines up on 32 bit boundaries
     * P.S. any increments are not an atomic operation
     */

    //static - no thread safety or reentrency to think about here

    ecm_status.cycle_count++;

    if (first_run) {
        for (int slave = 1; slave < MAP_NUM_SLAVES + 1; slave++) {
            //            printf("In initial pdo set: slave %u\n", slave);
            if (*map_slave_initial_pdo_function_ptr[slave - 1] != NULL) {
                if ((*map_slave_initial_pdo_function_ptr[slave - 1])(slave) == E_SUCCESS) {
                    UM_INFO(GBEM_UM_EN, "GBEM: Initial PDO sets succeeded for slave [%u]", slave);
                } else {
                    UM_ERROR(GBEM_UM_EN, "GBEM: Initial PDO sets failed for slave [%u]", slave);
                }
            }
        }

        int8_t moo_disp = 0;

        for (int i = 0; i < MAP_NUM_DRIVES; i++) {
            if (*map_drive_get_moo_sdo_function_ptr[i] != NULL) {
                moo_disp = (*map_drive_get_moo_sdo_function_ptr[i])(i);
                //                printf("moo_disp: %d", moo_disp);
                ecm_status.drives[i].act_moo = moo_disp;

                event_data.moo_disp[i] = moo_disp;
            } else {
                LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                         "GBEM: Missing function pointer for map_drive_get_moo_sdo on drive [%u]", i);
            }
        }
    }

    DPM_IN_PROTECT_START
    //copy actpos from EC_IN to DPM_IN (write) (do this every cycle irrespective of state)
    ctrl_copy_actpos();


    ctrl_copy_actvel();

    ctrl_copy_acttorq();

    ctrl_copy_control_effort();

    //copy statuswords from EC_IN to DPM_IN (write)
    ctrl_copy_drive_statuswords();
    //copy digital ins from EC_IN to DPM_IN (write)

    ctrl_process_iomap_in();

#if CTRL_ENABLE_FORCING == 1
    ctrl_process_forces_in();
#endif


    //increment heartbeat (this is the value stored in a static var on gbem and sent to gbc and echoed back from there
    gbem_heartbeat++;
    //write heartbeat to DPM_IN (Write)
    dpm_in->heartbeat = gbem_heartbeat;

    //We check for estop against the DPM copy of the data from EC - we copied it in above
    /*
     * if estop digin is false then set estop=true
     * if estop digin is false AND estopreset is true estop=false
     */


#if  DISABLE_ESTOP_CHECKING == 0
    //using estop din but no reset
    //Assumes 0 on din forces quickstop


#if MAP_NUMBER_ESTOP_DIN == 0
    UM_FATAL("GBEM: MAP_NUMBER_ESTOP_DIN not defined");
#endif
#if MAP_NUMBER_ESTOP_DIN > 2
    UM_FATAL("GBEM: too many MAP_NUMBER_ESTOP_DIN are defined!");
#endif

#if MAP_NUMBER_ESTOP_DIN == 1

    if (ctrl_estop_din_1.slave_num < 1) {
        UM_FATAL("GBEM: no ctrl_estop_din is defined! (slave number less thhan 1)");
    }

    if (!ec_pdo_get_input_bit(ctrl_estop_din_1.slave_num, ctrl_estop_din_1.bit_num)) {
        estop = true;
    } else {
        estop = false;
    }
#endif

#if MAP_NUMBER_ESTOP_DIN == 2

    if (ctrl_estop_din_1.slave_num < 1) {
        UM_FATAL("GBEM: no ctrl_estop_din_1 is defined! (slave number less thhan 1)");
    }
    if (ctrl_estop_din_2.slave_num < 1) {
        UM_FATAL("GBEM: no ctrl_estop_din_2 is defined! (slave number less thhan 1)");
    }

    if (!ec_pdo_get_input_bit(ctrl_estop_din_1.slave_num, ctrl_estop_din_1.bit_num) ||
        !ec_pdo_get_input_bit(ctrl_estop_din_2.slave_num, ctrl_estop_din_2.bit_num)) {
        estop = true;
    } else {
        estop = false;
    }
#endif


#endif

    DPM_IN_PROTECT_END

    bool alarm_on_any_drive = ec_is_warning();
    DPM_OUT_PROTECT_START

    event_data.heartbeat_lost = !ctrl_check_heartbeat_ok(gbem_heartbeat, dpm_out->heartbeat);

#if DISABLE_HEARTBEAT_CHECKING == 1
    event_data.heartbeat_lost = false;
#endif


    event_data.any_drive_has_alarm = alarm_on_any_drive;


    event_data.estop = estop;

#if DISABLE_ESTOP_CHECKING == 1
    event_data.estop = false;
#endif
    gberror_t grc;


    event_data.follow_error = ec_check_for_follow_error(&grc);
    if (grc != E_SUCCESS) {
        UM_ERROR(GBEM_UM_EN, "GBEM: error checking drive for follow error [%s]", gb_strerror(grc));
    }

    event_data.internal_limit = ec_check_for_internal_limit(&grc);
    if (grc != E_SUCCESS) {
        UM_ERROR(GBEM_UM_EN, "GBEM: error checking drive for internal limit [%s]", gb_strerror(grc));
    }
    //    event_data.machine_move_not_op_enabled_fault_req = BIT_CHECK(dpm_out->machine_word,
    //                                                                 CONTROL_WORD_MOVE_NOT_OP_ENABLED_FAULT_REQ_BIT_NUM);


    event_data.machine_request_error = BIT_CHECK(dpm_out->machine_word, CONTROL_WORD_GBC_REQUEST_FAULT_BIT_NUM);

    //gbc_internal_fault
    event_data.gbc_internal_fault = BIT_CHECK(dpm_out->machine_word, CONTROL_WORD_GBC_REQUEST_FAULT_BIT_NUM);

    event_data.ec_check_error = (ecm_status.ec_check_found_error == true) ? true : false;

    event_data.slave_reported_error = EcatError;

    event_data.plc_signalled_error = plc_signalled_error;

    event_data.homing_failed = homing_failed;


    DPM_OUT_PROTECT_END

    volatile int8_t moo_disp;
    for (uint16_t i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_get_moo_pdo_function_ptr[i] != NULL) {
            moo_disp = (*map_drive_get_moo_pdo_function_ptr[i])(i);
            //            printf("moo disp pdo: %u (drive %u)\n", moo_disp, i);
            ecm_status.drives[i].act_moo = moo_disp;
            event_data.moo_disp[i] = moo_disp;
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                     "GBEM: Missing function pointer for map_drive_get_moo_pdo on drive [%u]. This may not be an error as they might be sdo set",
                     i);
        }
    }

    if (ec_check_remote()) {
        event_data.remote_ok = true;
    } else {
        event_data.remote_ok = false;
    }


    /* This prints out the drive status and control words */
    //    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
    //
    //        printf("drive:%u : status word:%u\n", i, dpm_in->joint_statusword[i]);
    //        printf("drive:%u : control word:%u\n", i, ecm_status.drives[i].command);
    //    }

    DPM_IN_PROTECT_START
    /*	 run the state machine */
    int ret = stateM_handleEvent(m, &(struct event)
                                 {
                                     Event_cyclic,
                                     (void *) (intptr_t) &event_data
                                 });
    DPM_IN_PROTECT_END
    if (ret == stateM_errArg) {
        LL_FATAL("GBEM: Erroneous arguments state machine");
    }
    if (ret == stateM_errorStateReached) {
        LL_FATAL("GBEM: State machine reached error state");
    }
    if (ret == stateM_noStateChange) {
    }
    /* state returned to itself */
    if (ret == stateM_stateLoopSelf) {
    }

    if (ret == stateM_finalStateReached) {
        LL_FATAL(
            "GBEM: State machine reached a final state - this should never happen unless something has gone inexplicably pear shaped")
        ;
    }

    //read current state of state machine
    current_state = (cia_state_t) (stateM_currentState(m)->data);

    ecm_status.machine_state = current_state;
    ecm_status.commanded_machine_state = cia_ctrlwrd_to_command(dpm_in->machine_word);
    LL_TRACE(GBEM_SM_LOG_EN, "sm: Current state of state machine - %s", cia_state_names[current_state]);

    //    printf("fault word:%u\n", dpm_in->active_fault_word);

    /* this protects the copy of DPMout */
    DPM_OUT_PROTECT_START

    //if we are in FAULT or FAULT reaction active then read error message
    //these are SDO reads and will bugger up our real timeliness
    //    if (current_state == CIA_FAULT || current_state == CIA_FAULT_REACTION_ACTIVE) {
    //        ctrl_copy_slave_error_to_ecm_status();
    //        if ((gbem_heartbeat % 1000) == 0) {
    ////            uint8_t *error_code_string;
    //            for (int i = 0; i < MAP_NUM_DRIVES; i++) {
    //                read_drive_error_code_into_ecm_status(i);
    //            }
    //        }
    //
    //        print_slave_error_messages();
    //    }

    //    printf("in out 0 [%d]\n", dpm_out->unsigned32[0]);


    //loops over all drives setting MOO
    ctrl_set_moo_pdo();


    //copy setpos from DPM_OUT (read) to EC_OUT
    //    ctrl_copy_setpos();
    ctrl_copy_values_to_drives(ecm_status.cycle_count, current_state);

#if CTRL_ENABLE_FORCING == 1
    ctrl_process_forces_out();
#endif


    if (current_state == CIA_OPERATION_ENABLED) {
        ctrl_process_iomap_out(false);
    } else {
        //todo crit do we want to zero?
        ctrl_process_iomap_out(false);
    }


    copy_fsoe_data();
    update_fsoe_ecm_status();
    // print_status(&ecm_status);

    //copy PDO error codes into ecm status
    for (uint16_t drive = 0; drive < MAP_NUM_DRIVES; drive++) {
        if (*map_drive_get_error_string_pdo_function_ptr[drive] != NULL) {
            uint8_t *error_code_string = map_drive_get_error_string_pdo_function_ptr[drive](drive);
            memset(&ecm_status.drives[drive].error_message[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        }
    }


    //RT-sensitive
    //output user messages for any faults that have occurred
#if ENABLE_CYCLIC_MESSAGES == 1
    print_cyclic_user_message(NUM_CONTROL_EVENTS, control_event);
#endif
    DPM_OUT_PROTECT_END
}

//copy joint actpos - from ec to dpm
/**
 * @brief actpos to be sent to GBC via DPM in is set from actpos received over EC from the drive (and update ecm_status for gui)
 * @param slave_ptrs
 * @param inout_ptrs
 */
static void ctrl_copy_actpos(void) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_get_actpos_wrd_function_ptr[i] != NULL) {
            dpm_in->joint_actual_position[i] = map_drive_get_actpos_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                     "GBEM: Missing function pointer for map_drive_get_actpos_wrd on drive [%u]", i);
        }
    }
}

/**
 * @brief actvel to be sent to GBC via DPM in is set from actpos received over EC from the drive (and update ecm_status for gui)
 * @param slave_ptrs
 * @param inout_ptrs
 */
static void ctrl_copy_actvel(void) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_get_actvel_wrd_function_ptr[i] != NULL) {
            dpm_in->joint_actual_velocity[i] = map_drive_get_actvel_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                     "GBEM: Missing function pointer for map_drive_get_actvel_wrd on drive [%u]", i);
        }
    }
}

/**
 * @brief acttorq to be sent to GBC via DPM in is set from acttorq received over EC from the drive (and update ecm_status for gui)
 * @param slave_ptrs
 * @param inout_ptrs
 */
static void ctrl_copy_acttorq(void) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_get_acttorq_wrd_function_ptr[i] != NULL) {
            dpm_in->joint_actual_torque[i] = map_drive_get_acttorq_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                     "GBEM: Missing function pointer for map_drive_get_acttorq_wrd on drive [%u]", i);
        }
    }
}


/**
 * @brief control effort to be sent to GBC via DPM in is set from control effort received over EC from the drive (and update ecm_status for gui)
 * @param
 */
static void ctrl_copy_control_effort(void) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_get_control_effort_wrd_function_ptr[i] != NULL) {
            dpm_in->joint_actual_control_effort[i] = map_drive_get_control_effort_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                     "GBEM: Missing function pointer for map_drive_get_control_effort_wrd_function_ptr on drive [%u]",
                     i);
        }
    }
}


#define CTRL_POS_JUMP_THRESHOLD 25000

__attribute__((unused)) static void ctrl_check_for_big_pos_jump(uint16_t drive, int32_t current_position) {
    static int32_t last_pos[MAP_NUM_DRIVES];
    if ((abs(current_position - last_pos[drive])) > CTRL_POS_JUMP_THRESHOLD) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Position jump too big for drive [%u] jump [%u]", drive,
                 abs(current_position - last_pos[drive]));
    }
    last_pos[drive] = current_position;
}


//mode = (control_word >> 1) & 0x0F;

/**
 * Copies correct set values to the drives based on the MOO for the drive
 */
static void ctrl_copy_values_to_drives(uint64_t cycle_count, cia_state_t current_state) {
    //    static uint64_t ms_counter = 0;
    //    ms_counter ++;


    gberror_t grc;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        //        bool pos_vel_control_active = false;


        switch (map_drive_moo[i]) {
            case CIA_MOO_OP_DISABLED:
                break;

            case CIA_MOO_CSP:
                if (*map_drive_set_setpos_wrd_function_ptr[i] != NULL) {
                    grc = map_drive_set_setpos_wrd_function_ptr[i](i, dpm_out->joint_set_position[i]);
                    /* This function can be used to log nasty jumps in position*/
                    //            ctrl_check_for_big_pos_jump(i,dpm_out->joint_set_position[i] );
                    if (grc != E_SUCCESS) {
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive setpos function error [%s]", gb_strerror(grc));
                    }
                } else {
                    LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                             "GBEM: Missing function pointer for map_drive_set_setpos_wrd on drive [%u]", i);
                }

                if (*map_drive_set_setveloffset_wrd_function_ptr[i] != NULL) {
                    //                    printf("setvel [%d]\n", dpm_out->joint_set_velocity[i]);
                    //                    printf("actvel [%d]\n", dpm_in->joint_actual_velocity[i]);

                    grc = map_drive_set_setveloffset_wrd_function_ptr[i](i, dpm_out->joint_set_velocity[i]);
                    if (grc != E_SUCCESS) {
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive setveloffset function error [%s]", gb_strerror(grc));
                    }
                } else {
                    /*
                     * this is not an error it just means you have decided to not add a function for setvel because the drive doesnt support it or you dont want to
                     *  LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_set_setvel_wrd on drive [%u]", i);
                     */
                }
                if (*map_drive_set_settorqoffset_wrd_function_ptr[i] != NULL) {
                    grc = map_drive_set_settorqoffset_wrd_function_ptr[i](i, dpm_out->joint_set_torque_offset[i] +
                                                                             dpm_out->joint_set_torque[i]);
                    //                    printf("torq offset [%d] on drive [%d]\n", dpm_out->joint_set_torque_offset[i], i);
                    if (grc != E_SUCCESS) {
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive settorqoffset function error [%s]", gb_strerror(grc));
                    }
                } else {
                    /*
                     * this is not an error it just means you have decided to not add a function for settorqoffset because the drive doesnt support it or you dont want to
                     *  LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_set_settorqoffset_wrd on drive [%u]", i);
                     */
                }


                break;

            case CIA_MOO_CSV:


                if (*map_drive_set_setvel_wrd_function_ptr[i] != NULL) {
                    grc = map_drive_set_setvel_wrd_function_ptr[i](i,
                                                                   dpm_out->joint_set_velocity[i]);
                    if (grc != E_SUCCESS) {
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive setvel function error [%s]", gb_strerror(grc));
                    }
                } else {
                    LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                             "GBEM: Missing function pointer for map_drive_set_setvel_wrd on drive [%u]", i);
                }


                break;

            case CIA_MOO_CST:


                if (*map_drive_set_settorq_wrd_function_ptr[i] != NULL) {
                    grc = map_drive_set_settorq_wrd_function_ptr[i](i, dpm_out->joint_set_torque[i]);
                    if (grc != E_SUCCESS) {
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive settorque function error [%s]", gb_strerror(grc));
                    }
                } else {
                    LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                             "GBEM: Missing function pointer for map_drive_set_settorque_wrd on drive [%u]", i);
                }

                if (*map_drive_set_settorqoffset_wrd_function_ptr[i] != NULL) {
                    grc = map_drive_set_settorqoffset_wrd_function_ptr[i](i, dpm_out->joint_set_torque_offset[i]);
                } else {
                    LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                             "GBEM: Missing function pointer for map_drive_set_settorqoffset_wrd on drive [%u]", i);
                }

            /* also write position to drive for transition back to CSP*/
                if (*map_drive_set_setpos_wrd_function_ptr[i] != NULL) {
                    grc = map_drive_set_setpos_wrd_function_ptr[i](i, dpm_out->joint_set_position[i]);
                    if (grc != E_SUCCESS) {
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive setpos function error [%s]", gb_strerror(grc));
                    }
                } else {
                    LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                             "GBEM: Missing function pointer for map_drive_set_setpos_wrd on drive [%u]", i);
                }


                break;
            default:
                UM_FATAL("GBEM: MOO not defined for drive [%u]", i);
        }
    }
}

/**
 * @brief setpos on drive is set to setpos from GBC (from DPM out) (and update ecm_status for gui)
 * @param slave_ptrs
 * @param inout_ptrs
 */
static void __attribute__((unused)) ctrl_copy_setpos(void) {
    gberror_t grc;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_set_setpos_wrd_function_ptr[i] != NULL) {
            grc = map_drive_set_setpos_wrd_function_ptr[i](i, dpm_out->joint_set_position[i]);
            /* This function can be used to log nasty jumps in position*/
            //            ctrl_check_for_big_pos_jump(i,dpm_out->joint_set_position[i] );
            if (grc != E_SUCCESS) {
                LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive setpos function error [%s]", gb_strerror(grc));
            }
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                     "GBEM: Missing function pointer for map_drive_set_setpos_wrd on drive [%u]", i);
        }
    }
}

/**
 * @brief copy drive status words from EC to DPM_IN
 */
static void ctrl_copy_drive_statuswords(void) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_get_stat_wrd_function_ptr[i] != NULL) {
            dpm_in->joint_statusword[i] = map_drive_get_stat_wrd_function_ptr[i](i);
            ecm_status.drives[i].state = cia_statwrd_to_state(dpm_in->joint_statusword[i]);
            LL_TRACE(GBEM_SM_LOG_EN, "sm: Drive: %u, State: %s", i,
                     cia_state_names[cia_statwrd_to_state(dpm_in->joint_statusword[i])]);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_get_stat_wrd on drive [%u]",
                     i);
        }
        //        printf("statusword:%d\n",dpm_in->joint_statusword[i]);
    }
}

/**
 * @brief copy drive controlwords from DPM_OUT to EC
 * @warn this should not be called as drive control words are set by the state machine
 */
void ctrl_copy_drive_controlwords(void) {
    LL_FATAL("GBEM: State machine is controlling drives not GBC - ctrl_copy_drive_controlwords was called");

    gberror_t grc;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_set_ctrl_wrd_function_ptr[i] != NULL) {
            grc = map_drive_set_ctrl_wrd_function_ptr[i](i, dpm_out->joint_controlword[i]);
            ecm_status.drives[i].command = cia_ctrlwrd_to_command(dpm_out->joint_controlword[i]);
            if (grc != E_SUCCESS) {
                LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive set ctrl wrd function error [%s]", gb_strerror(grc));
            }
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_set_ctrl_wrd on drive [%u]",
                     i);
        }
    }
}

/**
 * @brief checks all drives against a provided command
 * @param drive_status array of drive statuses
 * @param command to check
 * @return true=all drives are commanded with that command, false=one or more drives are not in the state
 *
 * @warning this is used for sim/test
 */
bool ctrl_check_all_drives_commands_sim(const cia_commands_t command) {
    int j = 0;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (cia_ctrlwrd_to_command(
                map_drive_get_ctrl_wrd_rev_function_ptr[i](i)) ==
            command) {
            j++;
        }
    }
    if (j == MAP_NUM_DRIVES) {
        return true;
    } else {
        return false;
    }
}


void ctrl_process_forces_in(void) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].gbc.inout == MAP_IN) {
            //we have a gbc entry
            if (map_iomap[i].force.active) {
                iomap_set_gbc_in_union(map_iomap[i].gbc.ionum, map_iomap[i].gbc.datatype, map_iomap[i].force.val);
            }
        }
    }
}


void ctrl_process_iomap_in(void) {
    //loop over whole iomap
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        /* look for rows matching
      * PDO | GBC | PLC
      * in  | in  | in
      * in  | in  | none
      */
        if ((map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_IN &&
             map_iomap[i].plc.inout == MAP_IN) ||
            (map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_IN &&
             map_iomap[i].plc.inout == MAP_UNDEFINED)) {
            switch (map_iomap[i].gbc.datatype) {
                case ECT_BOOLEAN:
                    iomap_set_gbc_digital_in_from_pdo(map_iomap[i].pdo.byte_slave, map_iomap[i].pdo.slave_num,
                                                      map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                                                      map_iomap[i].gbc.ionum, map_iomap[i].gbc.type);
                    break;
                case ECT_INTEGER32:
                    iomap_set_gbc_in_int32_from_pdo(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                    map_iomap[i].pdo.byte_num, map_iomap[i].gbc.ionum,
                                                    (float) map_iomap[i].pdo.max_val);
                    break;
                case ECT_UNSIGNED32:
                    iomap_set_gbc_uint32_in_from_pdo(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                     map_iomap[i].pdo.byte_num, map_iomap[i].gbc.ionum,
                                                     (float) map_iomap[i].pdo.max_val);
                    break;
                case ECT_REAL32:
                    iomap_set_gbc_float_in_from_pdo(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                    map_iomap[i].pdo.byte_num, map_iomap[i].gbc.ionum,
                                                    (float) map_iomap[i].pdo.max_val);
                    break;
                default:
                    break;
            } //switch on datatype
        } //if
    } //for loop
}


void ctrl_process_forces_out(void) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].pdo.inout == MAP_OUT) {
            //we have a pdo out entry
            if (map_iomap[i].force.active) {
                iomap_set_pdo_out_union(map_iomap[i].pdo.slave_num, map_iomap[i].pdo.byte_num,
                                        map_iomap[i].pdo.datatype, map_iomap[i].force.val);
            }
        }
    }
}

void ctrl_process_iomap_out(const bool zero) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        /* look for rows matching
          * PDO  | GBC  | PLC
          * out  | out  | out
          * out  | out  | none
          */
        if ((map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_OUT &&
             map_iomap[i].plc.inout == MAP_OUT) ||
            (map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_OUT &&
             map_iomap[i].plc.inout == MAP_UNDEFINED)) {
            //assume we have tested map for missing data so don't check
            switch (map_iomap[i].gbc.datatype) {
                //we have a gbc out
                case ECT_BOOLEAN:
                    if (!zero) {
                        if (map_iomap[i].gbc.type == GBC_IO_TYPE_NORMAL) {
                            iomap_set_pdo_out_bool(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                   map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                                                   BIT_CHECK(dpm_out->digital[0], map_iomap[i].gbc.ionum));
                        } else if (map_iomap[i].gbc.type == GBC_IO_TYPE_SAFETY) {
                            iomap_set_pdo_out_bool(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                   map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                                                   BIT_CHECK(dpm_out->safetyDigital[0], map_iomap[i].gbc.ionum));
                        } else {
                            UM_FATAL("GBEM: An external IO has been mapped to a digital in");
                        }
                    } else {
                        if (map_iomap[i].gbc.type == GBC_IO_TYPE_NORMAL) {
                            iomap_set_pdo_out_bool(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                   map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                                                   false);
                        } else if (map_iomap[i].gbc.type == GBC_IO_TYPE_SAFETY) {
                            iomap_set_pdo_out_bool(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                   map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                                                   false);
                        } else {
                            UM_FATAL("GBEM: An external IO has been mapped to a digital in");
                        }
                    }
                    break;
                case ECT_INTEGER32:
                    iomap_set_pdo_out_int32(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                            map_iomap[i].pdo.byte_num,
                                            dpm_out->integer32[map_iomap[i].gbc.ionum],
                                            (float) map_iomap[i].pdo.max_val);
                    break;
                case ECT_UNSIGNED32:
                    iomap_set_pdo_out_uint32(map_iomap[i].pdo.datatype,
                                             map_iomap[i].pdo.slave_num, map_iomap[i].pdo.byte_num,
                                             dpm_out->unsigned32[map_iomap[i].gbc.ionum],
                                             (float) map_iomap[i].pdo.max_val);

                    break;
                case ECT_REAL32:
                    iomap_set_pdo_out_float(map_iomap[i].pdo.datatype,
                                            map_iomap[i].pdo.slave_num, map_iomap[i].pdo.byte_num,
                                            dpm_out->analog[map_iomap[i].gbc.ionum], (float) map_iomap[i].pdo.max_val);
                    break;
                default:
                    break;
            } //switch on datatype
        } // if
    } //for loop
}

/**
 * @brief set moo on all drives based on whatever is set in map_drive_moo array
 * @attention some drives don't support setting moo with a PDO
 */
void ctrl_set_moo_pdo(void) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_set_moo_pdo_function_ptr[i] != NULL) {
            int8_t moo = 0;
            moo = (dpm_out->joint_controlword[i]) & 0x0F;
            map_drive_moo[i] = moo;


            //todo crit - enable to set moo from gbc controlled joint control word
            //            (*map_drive_set_moo_pdo_function_ptr[i])(i, moo);
            //            ecm_status.drives[i].cmd_moo = moo;
            //

            (*map_drive_set_moo_pdo_function_ptr[i])(i, map_drive_moo[i]);
            ecm_status.drives[i].cmd_moo = map_drive_moo[i];
        }
    }
}


/**
 * @brief get number of out bytes from slot
 *
 * @warning slave is zero indexed
 */
// uint32_t fsoe_get_num_bytes_out(uint16_t slave) {
//     switch (map_slave_mdp_slot_type[slave]) {
//         case MDP_SLOT_TYPE_NONE:
//             return 0;
//             break;
//         case MDP_SLOT_TYPE_MASTER_AGGREGATE:
//             return 0;
//         case MDP_SLOT_TYPE_BBH_32_12:
//             return 31;
//             break;
//         default:
//             LL_FATAL("GBEM: Invalid MDP_SLOT_TYPE [%u] on slave [%u]", map_slave_mdp_slot_type[slave], slave+1);
//             return 0;
//     }
// }


/**
 * @brief get number of out bytes from slot
 *
 * @warning slave is zero indexed
 */
// uint32_t fsoe_get_num_bytes_in(uint16_t slot) {
//     switch (map_slave_mdp_slot_type[slot]) {
//         case MDP_SLOT_TYPE_NONE:
//             return 0;
//             break;
//         case MDP_SLOT_TYPE_MASTER_AGGREGATE:
//             return 0;
//         case MDP_SLOT_TYPE_BBH_32_12:
//             return 11;
//             break;
//         default:
//             LL_FATAL("GBEM: Invalid MDP_SLOT_TYPE [%u] on slave [%u]", map_slave_mdp_slot_type[slot], slot);
//             return 0;
//     }
// }


/**
 * @brief copy fsoe data from slaves to master
 */
void copy_fsoe_data_slaves_to_master(uint16_t fsoe_master_no) {
    //copy from EL1904->EL2904
    //    ec_copy_between_slave_pdos(MAP_EL1904_1, MAP_EL6900_1, 0, 0, 6);
    //copy from EL2904->EL6900
    //    ec_copy_between_slave_pdos(MAP_EL2904_1, MAP_EL6900_1, 0, 6, 6);


    uint32_t base_master_byte_offset = map_slave_fsoe_offset_in[fsoe_master_no];
    uint32_t cumulative_master_byte_offset = 0;
    uint32_t slave_byte_offset = 0;
    // bool found_at_least_one_fsoe_slave = false;

    //todo maybe -1 for bytes here?
    for (int slot = 0; slot < MAP_NUM_FSOE_MASTER_SLOTS; slot++) {
        uint32_t slave_fsoe_out_bytes = map_fsoe_get_slot_size_out(slot);

        uint16_t slave = map_fsoe_master_slot_to_slave[slot];

        ec_copy_between_slave_pdos(slave, fsoe_master_no + 1, map_slave_fsoe_offset_out[slave - 1],
                                   base_master_byte_offset + cumulative_master_byte_offset,
                                   slave_fsoe_out_bytes);

        cumulative_master_byte_offset += slave_fsoe_out_bytes;
    }


    // if (found_fsoe_master) {
    //     for (int i = 0; i < MAP_NUM_SLAVES; i++) {
    //         if (map_slave_fsoe_function[i] == FSOE_SLAVE_FUNCTION_SLAVE) {
    //             //we have an FSoE slave
    //             found_at_least_one_fsoe_slave = true;
    //
    //             uint32_t slave_fsoe_out_bytes = map_fsoe_get_slot_size_slave_out(i + 1);
    //             // printf("slave fsoe out bytes [%u]\n", slave_fsoe_out_bytes);
    //
    //             if (slave_fsoe_out_bytes == 0) {
    //                 LL_FATAL("GBEM: Slave [%u] has no FSoE out bytes - check MDP slot config - this is a fatal error",
    //                          i + 1);
    //             }
    //
    //             if ( i != fsoe_master_no) {
    //                 ec_copy_between_slave_pdos(i + 1, fsoe_master_no + 1, map_slave_fsoe_offset_out[i],
    //                                            base_master_byte_offset + cumulative_master_byte_offset,
    //                                            slave_fsoe_out_bytes);
    //
    //                 // UM_INFO(GBEM_GEN_LOG_EN,
    //                 //         "GBEM: FSOE master copying [%u] bytes at [%u] offset from slave (slave no [%u]) to master    (slave no [%u]) at [%u] offset",
    //                 //         map_slave_fsoe_out_bytes[i], map_slave_fsoe_offset_out[i], i + 1, fsoe_master_no + 1,
    //                 //         base_master_byte_offset + cumulative_master_byte_offset);
    //
    //                 cumulative_master_byte_offset += slave_fsoe_out_bytes;
    //             }
    //         }
    //     }
    //     if (!found_at_least_one_fsoe_slave) {
    //         LL_FATAL("GBEM: FSOE master found but no FSOE slaves found - this is a fatal error");
    //     }
    // }
}

/**
 * @brief copy fsoe data from master to slaves
 */
void copy_fsoe_data_master_to_slaves(uint16_t fsoe_master_no) {
    //copy from EL6900->EL1904
    //    ec_copy_between_slave_pdos(MAP_EL6900_1, MAP_EL1904_1, 0, 0, 6);
    //copy from EL6900->EL2904
    //    ec_copy_between_slave_pdos(MAP_EL6900_1, MAP_EL2904_1, 6, 0, 6);


    uint32_t base_master_byte_offset = map_slave_fsoe_offset_out[fsoe_master_no];
    uint32_t cumulative_master_byte_offset = 0;
    uint32_t slave_byte_offset = 0;
    bool found_at_least_one_fsoe_slave = false;

    //todo maybe -1 for bytes here?
    for (int slot = 0; slot < MAP_NUM_FSOE_MASTER_SLOTS; slot++) {
        // uint32_t slave_fsoe_in_bytes = map_fsoe_get_slot_size_in(slot);
        uint32_t slave_fsoe_in_bytes = 11;

        uint16_t slave = map_fsoe_master_slot_to_slave[slot];
        slave_byte_offset = map_slave_fsoe_offset_in[slave - 1];

        ec_copy_between_slave_pdos(fsoe_master_no + 1, slave,
                                   base_master_byte_offset + cumulative_master_byte_offset,
                                   slave_byte_offset,
                                   slave_fsoe_in_bytes);


        cumulative_master_byte_offset += slave_fsoe_in_bytes;
    }


    // if (map_slave_fsoe_out_bytes[fsoe_master_no] > 0) {
    //     for (int i = 0; i < MAP_NUM_SLAVES; i++) {
    //         if (map_slave_fsoe_in_bytes[i] > 0 && i != fsoe_master_no) {
    //             found_at_least_one_fsoe_slave = true;
    //             slave_byte_offset = map_slave_fsoe_offset_in[i];
    //             ec_copy_between_slave_pdos(fsoe_master_no + 1, i + 1,
    //                                        base_master_byte_offset + cumulative_master_byte_offset,
    //                                        slave_byte_offset,
    //                                        map_slave_fsoe_out_bytes[fsoe_master_no]);
    //
    //
    //             if (map_slave_fsoe_out_bytes[fsoe_master_no] != map_slave_fsoe_in_bytes[i]) {
    //                 LL_FATAL(
    //                     "GBEM: FSOE master and slave have different number of bytes (master out to slave in) - this is a fatal error")
    //                 ;
    //             }
    //
    //             // UM_INFO(GBEM_GEN_LOG_EN,
    //             //         "GBEM: FSOE master copying [%u] bytes at [%u] offset from master (slave no [%u]) to slave (slave no [%u]) at [%u] offset",
    //             //         map_slave_fsoe_out_bytes[fsoe_master_no],
    //             //         base_master_byte_offset + cumulative_master_byte_offset,
    //             //         fsoe_master_no + 1, i + 1, slave_byte_offset);
    //
    //             cumulative_master_byte_offset += map_slave_fsoe_in_bytes[i];
    //         }
    //     }
    //     if (!found_at_least_one_fsoe_slave) {
    //         UM_FATAL("GBEM: FSOE master found but no FSOE slaves found - this is a fatal error");
    //     }
    // }
}


void fsoe_plc_program(void) {
    //ESM_From_PLC : Output 1
    //    FromPLC1 : BOOL;
    //ESM_From_PLC : Output 2
    //    ErrAck : BOOL;

    //ESM_To_PLC : Input 1
    //    ToPLC1 : BOOL;
    //ESM_To_PLC : Input 2
    //    FbErr : BOOL;
    //ESM_To_PLC : Input 3
    //    ComErr : BOOL;
    //ESM_To_PLC : Input 4
    //    OutErr : BOOL;


    //    printf("in 0 %s\n", BIT_CHECK(dpm_in->digital, 0) ? "true" : "false");
    //    printf("in 1 %s\n", BIT_CHECK(dpm_in->digital, 1) ? "true" : "false");
    //    printf("in 2 %s\n", BIT_CHECK(dpm_in->digital, 2) ? "true" : "false");
    //    printf("in 3 %s\n", BIT_CHECK(dpm_in->digital, 3) ? "true" : "false");
    //    printf("in 4 %s\n", BIT_CHECK(dpm_in->digital, 4) ? "true" : "false");
    //    printf("in 5 %s\n", BIT_CHECK(dpm_in->digital, 5) ? "true" : "false");
    //    printf("in 6 %s\n", BIT_CHECK(dpm_in->digital, 6) ? "true" : "false");
    //    printf("in 7 %s\n", BIT_CHECK(dpm_in->digital, 7) ? "true" : "false");
    //
    //    printf("out 0 %s\n", BIT_CHECK(dpm_out->digital, 0) ? "true" : "false");
    //    printf("out 1 %s\n", BIT_CHECK(dpm_out->digital, 1) ? "true" : "false");
    //    printf("out 2 %s\n", BIT_CHECK(dpm_out->digital, 2) ? "true" : "false");
    //    printf("out 3 %s\n", BIT_CHECK(dpm_out->digital, 3) ? "true" : "false");
    //
    //    static bool ErrAckProxy = false;
    //
    //    bool temp = iomap_get_pdo_in_bool(true, MAP_EL6900_1, 12, 2);
    //    bool temp2 = iomap_get_pdo_in_bool(true, MAP_EL6900_1, 12, 0);
    //    if (temp2) {
    //        printf("bit 0 in is true!\n");
    //    }
    //
    //    if (temp) {
    //        printf("bit 2 in is true\n");
    //    }
    //
    //    if (ErrAckProxy) {
    ////        BIT_CLEAR(dpm_out->digital, 1);
    //        printf("clearing out\n");
    //        ErrAckProxy = false;
    //        iomap_set_pdo_out_bool(true, MAP_EL6900_1, 12, 1, false);
    //        return;
    //    }
    //
    ////    if (BIT_CHECK(dpm_in->digital, 2)) {
    //    if (temp) {
    //        printf("setting out\n");
    ////        BIT_SET(dpm_out->digital, 1);
    //        ErrAckProxy = true;
    //        iomap_set_pdo_out_bool(true, MAP_EL6900_1, 12, 1, true);
    //    }
    //
}

typedef struct {
    uint8_t command;
    uint8_t data;
    uint16_t crc;
    uint16_t con_id;
} fsoe_t;


void print_fsoe_data(uint16_t slave_no, bool input, uint8_t byte_no, uint8_t num_bytes) {
    uint8_t *data_ptr;

    if (!input) {
        data_ptr = ec_slave[slave_no].inputs;
    } else {
        data_ptr = ec_slave[slave_no].outputs;
    }
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    if (input) {
        printf("Input ");
    } else {
        printf("Output ");
    }
    printf("Slave %" PRIu16 " ", slave_no);
    printf("Byte %" PRIu8 " ", byte_no);

    for (int i = 0; i < num_bytes; i++) {
        printf("[%" PRIu8 "] ", *data_ptr);
        data_ptr++;
    }
    printf("\n");
}


void copy_fsoe_data(void) {
    //    printf("EL6900\n");
    //    print_fsoe_data(MAP_EL6900_1, true, 0, 6);
    //    print_fsoe_data(MAP_EL6900_1, false, 0, 6);
    //    print_fsoe_data(MAP_EL6900_1, true, 6, 6);
    //    print_fsoe_data(MAP_EL6900_1, false, 6, 6);
    //
    //    printf("EL1904\n");
    //    print_fsoe_data(MAP_EL1904_1, true, 0, 6);
    //    print_fsoe_data(MAP_EL1904_1, false, 0, 6);
    //    printf("EL2904\n");
    //    print_fsoe_data(MAP_EL2904_1, true, 0, 6);
    //    print_fsoe_data(MAP_EL2904_1, false, 0, 6);


    // printf("BBH out offset 0 bytes 31\n");
    // print_fsoe_data(MAP_BBH_SCU_1_EC_1, false, 0, 11);
    // printf("BBH in offset 0 bytes 31\n");
    // print_fsoe_data(MAP_BBH_SCU_1_EC_1, true, 0, 31);
    //
    // printf("J25 out offset 43 bytes 31\n");
    // print_fsoe_data(MAP_AW_J25_FSOE_1, false, 43, 31);
    // printf("J25 in offset 43 bytes 31\n");
    // print_fsoe_data(MAP_AW_J25_FSOE_1, true, 35, 11);


    // printf(">>>BBH out offset 0 bytes 82\n");
    // print_fsoe_data(MAP_BBH_SCU_1_EC_1, false, 22, 1); //debug0
    // print_fsoe_data(MAP_BBH_SCU_1_EC_1, false, 27, 1);
    // print_fsoe_data(MAP_BBH_SCU_1_EC_1, false, 28, 1);
    // print_fsoe_data(MAP_BBH_SCU_1_EC_1, false, 29, 1);

    // print_fsoe_data(MAP_BBH_SCU_1_EC_1, true, 22, 1);


    static uint16_t fsoe_master_no = 0;
    static bool found_fsoe_master = false;
    if (!found_fsoe_master) {
        for (int i = 0; i < MAP_NUM_SLAVES; i++) {
            if (map_slave_fsoe_function[i] == FSOE_SLAVE_FUNCTION_MASTER) {
                if (found_fsoe_master == true) {
                    UM_FATAL(
                        "GBEM: More than one FSOE master found - this is a fatal error (MAP_SLAVE_FSOE_MASTER contains mutiple true values)")
                    ;
                }
                fsoe_master_no = i;
                UM_INFO(GBEM_GEN_LOG_EN, "GBEM: FSOE master found on slave [%u]", fsoe_master_no + 1);
                ecm_status.fsoe.master_slave_no = fsoe_master_no + 1;
                found_fsoe_master = true;
            }
        }
    }

    //only copy fsoe data if a master is found
    if (found_fsoe_master) {
        copy_fsoe_data_master_to_slaves(fsoe_master_no);
        copy_fsoe_data_slaves_to_master(fsoe_master_no);
    }
    //NO 31 32 33 39
    //FAULT 34?

    //todo crit this is quite key!
    uint32_t test_byte_no = 64;
    // iomap_set_pdo_out_bool(true, MAP_BBH_SCU_1_EC_1, test_byte_no, 1, true);
    // iomap_set_pdo_out_bool(true, MAP_BBH_SCU_1_EC_1, test_byte_no, 2, true);


    // for (int i = 0; i < 16; i++) {
    //     printf("in %u [%s]\n", i, BIT_CHECK(dpm_in->digital, i) ? "true" : "false");
    // }
    // bool temp = iomap_get_pdo_in_bool(true, MAP_BBH_SCU_1_EC_1, 27, 3);

    // printf("check 0 %u [%s]\n", 3, temp ? "true" : "false");

    // fsoe_plc_program();
}


gberror_t update_fsoe_ecm_status_slaves(void) {
    gberror_t grc = E_SUCCESS;

    for (int slave = 1; slave < MAP_NUM_SLAVES + 1; slave++) {
        if (*map_slave_fsoe_get_slave_state_function_ptr[slave - 1] != NULL) {
            if (map_slave_fsoe_function[slave - 1] == FSOE_SLAVE_FUNCTION_MASTER) {
                UM_FATAL(
                    "GBEM: FSoE master mapped on slave [%u] and we are trying to read slave slate - the mapping functions for reading FSoE slave state are incorrectly configured",
                    slave);
            }

            //todo crit is (slave - >>1<<) correct?
            uint32_t slave_state = 0;
            enum FSOE_SLAVE_HIGH_LEVEL_STATE high_level_state = 0;

            grc = map_slave_fsoe_get_slave_state_function_ptr[slave - 1](slave, &slave_state, &high_level_state);

            // printf("fsoe slave state [%u]\n", fsoe_slave_state);

            ecm_status.fsoe.slave_state[slave] = slave_state;
            ecm_status.fsoe.slave_high_level_state[slave] = high_level_state;
        } else {
            //some slaves that are not FSoE will not have this function
            // LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_slave_fsoe_get_slave_state on slave [%u]",
            //          slave);
        }


        if (*map_slave_fsoe_get_slave_con_id_function_ptr[slave - 1] != NULL) {
            uint16_t connection_id = 0;

            grc = map_slave_fsoe_get_slave_con_id_function_ptr[slave - 1](slave, &connection_id);

            ecm_status.fsoe.slave_connection_id[slave] = connection_id;
        }
    }

    return grc;
}

gberror_t update_fsoe_ecm_status_master(void) {
    gberror_t grc = E_SUCCESS;

    for (int slave = 1; slave < MAP_NUM_SLAVES + 1; slave++) {
        if (*map_slave_fsoe_get_master_state_function_ptr[slave - 1] != NULL) {
            if (map_slave_fsoe_function[slave - 1] != FSOE_SLAVE_FUNCTION_MASTER) {
                UM_FATAL(
                    "GBEM: FSoE master read state function mapped on slave [%u] but the slave is not tagged as the master - the mapping functions for reading FSoE master state are incorrectly configured",
                    slave);
            }
            uint32_t master_state = 0;

            uint32_t master_error_code = 0;


            enum FSOE_MASTER_HIGH_LEVEL_STATE high_level_state = 0;

            grc = map_slave_fsoe_get_master_state_function_ptr[slave - 1](
                slave, &master_state, &high_level_state, &master_error_code);

            // printf("fsoe slave state [%u]\n", fsoe_slave_state);

            ecm_status.fsoe.master_state = master_state;
            ecm_status.fsoe.master_high_level_state = high_level_state;
            ecm_status.fsoe.master_error_code = master_error_code;
        }

        if (grc != E_SUCCESS) {
            UM_ERROR(GBEM_UM_EN, "GBEM: update_fsoe_ecm_status_master error [%s]", gb_strerror(grc));
        }
    }

    return grc;
}

gberror_t update_fsoe_ecm_status(void) {
    gberror_t grc = E_SUCCESS;

    grc = update_fsoe_ecm_status_slaves();

    if (grc != E_SUCCESS) {
        UM_ERROR(GBEM_UM_EN, "GBEM: update_fsoe_ecm_status_slaves error [%s]", gb_strerror(grc));
    }

    grc = update_fsoe_ecm_status_master();

    if (grc != E_SUCCESS) {
        UM_ERROR(GBEM_UM_EN, "GBEM: update_fsoe_ecm_status_master error [%s]", gb_strerror(grc));
    }

    return grc;
}

