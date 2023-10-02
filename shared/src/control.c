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
//#include "ethercatsetget.h"
#include "cia402.h"
#include "read_drive_error_code_into_ecm_status.h"
#include "print_slave_error_messages.h"
#include "status_control_word_bit_definitions.h"
#include "ec_functions.h"
#include "pos_vel_acc.h"
#include "p.h"
#include "user_message.h"

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
        [CONTROL_EVENT_GBC_MOVE_NOT_OP_END_REQUEST] = {.message="An error has been detected [Move when not in op en]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_GBC_INTERNAL_FAULT] = {.message="An error has been detected [GBC internal fault]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_HEARTBEAT_LOST] = {.message="An error has been detected [Heartbeat to GBC lost]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_LIMIT_REACHED] = {.message="An error has been detected [Drive reached limit]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_STATE_CHANGE_TIMEOUT] = {.message="An error has been detected [Drive state change timeout]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_FOLLOW_ERROR] = {.message="An error has been detected [Drive follow error too large]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_NO_REMOTE] = {.message="An error has been detected [Drive lost remote]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_ECAT_ERROR] = {.message="An error has been detected [EtherCAT network]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_ALARM] = {.message="An error has been detected [Drive alarm]", .type=CYCLIC_MSG_ERROR},
//gbc_to_plc con error is not used on GBEM it signals an error when gbc loses contact with GBEM
//        [CONTROL_EVENT_GBC_TO_GBEM_CON_ERROR] = {.message="An error has been detected [GBC-to-GBEM connection error]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_MOOERROR] = {.message="An error has been detected [modes of operation]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_ECAT_SLAVE_ERROR] = {.message="An error has been detected [EtherCAT slave]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_PLC_SIGNALLED_ERROR] = {.message="An error has been detected [PLC signalled an error]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_HOMING_ERROR] = {.message="\"An error has been detected [Homing error]", .type=CYCLIC_MSG_ERROR},
};

//@formatter:on



/**global variable to check how long drives take to response to state change request */
uint32_t ctrl_state_change_cycle_count = 1;
uint32_t ctrl_state_change_timeout = CTRL_DRIVE_CHANGE_STATE_TIMEOUT;

/* functions to copy between EC and DPM */
static void ctrl_copy_actpos(void);

static void ctrl_copy_actvel(void);

static void ctrl_copy_acttorq(void);


static void ctrl_copy_setpos(void);

static void ctrl_copy_values_to_drives(void);

static void ctrl_copy_drive_statuswords(void);

/* functions to force gbc and pdos */
void ctrl_process_forces_in(void);

void ctrl_process_forces_out(void);

static void ctrl_check_for_big_pos_jump(uint16_t drive, int32_t current_position);


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

/* Forward declaration of sm states so that they can be defined in an logical order: */
static struct state cia_not_ready_to_switch_on_state, cia_switch_on_disabled_state, cia_ready_to_switch_on_state, cia_switched_on_state, cia_operation_enabled_state, cia_quick_stop_active_state,
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
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                NULL,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 2,
        .data = (void *) CIA_NOT_READY_TO_SWITCH_ON,
        .entryAction = &cia_generic_entry_action,
        .exitAction =
        NULL,};

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
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn2_guard,
                                NULL,
                                &cia_ready_to_switch_on_state},},
        .numTransitions = 2,
        .data =  (void *) CIA_SWITCH_ON_DISABLED,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

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
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn3_guard,
                                NULL,
                                &cia_switched_on_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn7_guard,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 3,
        .data =  (void *) CIA_READY_TO_SWITCH_ON,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

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
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn6_guard,
                                NULL,
                                &cia_ready_to_switch_on_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn4_guard,
                                NULL,
                                &cia_operation_enabled_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn10_guard,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 4,
        .data =  (void *) CIA_SWITCHED_ON,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

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
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn5_guard,
                                NULL,
                                &cia_switched_on_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn8_guard,
                                NULL,
                                &cia_ready_to_switch_on_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn9_guard,
                                NULL,
                                &cia_switch_on_disabled_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn11_guard,
                                NULL,
                                &cia_quick_stop_active_state},},
        .numTransitions = 5,
        .data =  (void *) CIA_OPERATION_ENABLED,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

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
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn16_guard,
                                NULL,
                                &cia_operation_enabled_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn12_guard,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 3,
        .data = (void *) CIA_QUICK_STOP_ACTIVE,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

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
                                &cia_fault_state},
                        {
                                Event_cyclic,
                                NULL,
                                NULL,
                                &cia_set_current_fault_causes_action,
                                &cia_fault_reaction_active_state},},
        .numTransitions = 2,
        .data =  (void *) CIA_FAULT_REACTION_ACTIVE,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

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
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn15_guard,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 2,
        .data =  (void *) CIA_FAULT,
        .entryAction = &cia_generic_entry_action,
        .exitAction =
        NULL,};

__attribute__((unused)) static struct state errorState = {
        .entryAction = &sm_error_state_entry_action};


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
                     "sm: TRN5 Guard - changing drive states with a Disabled operation controlword (switch on controword is same)");
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
    if (((event_data_t *) event->data)->machine_move_not_op_enabled_fault_req == true) {
        LL_TRACE(GBEM_SM_LOG_EN,
                 "sm: Fault > machine word is requesting an error because a move has been attempted and we are not in operation enabled");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_MOVE_NOT_OP_EN_BIT_NUM);
        control_event[CONTROL_EVENT_GBC_MOVE_NOT_OP_END_REQUEST].active = true;
        have_fault = true;
    }
//    CTRL_GBC_INTERNAL_FAULT_REQ_BIT_NUM

    if (((event_data_t *) event->data)->gbc_internal_fault == true) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > machine word is signalling GBC had an internal fault");
        BIT_SET(((event_data_t *) event->data)->fault_cause, FAULT_CAUSE_GBC_INTERNAL_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_GBC_MOVE_NOT_OP_END_REQUEST].active = true;
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
    if (ctrl_state_change_cycle_count > ctrl_state_change_timeout) {
        LL_TRACE(GBEM_SM_LOG_EN, "sm: Fault > one or more drives took too long reponding to a state change request");
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

    switch (cia_ctrlwrd_to_command(dpm_out->machine_word)) {

        //    uint32_t x=(uint32_t) ((intptr_t) condition & 0xFFFFFFFF);

// machine is commanding SHUTDOWN, our state = READY_TO_SWITCH_ON
        case CIA_SHUTDOWN:
            if (((cia_state_t) ((void *) condition) != CIA_READY_TO_SWITCH_ON) &&
                (ctrl_state_change_cycle_count > ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
        case CIA_SWITCH_ON:
            if (((cia_state_t) ((uintptr_t *) condition) != CIA_SWITCHED_ON) &&
                (ctrl_state_change_cycle_count > ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
//        case CIA_SWITCH_ON_AND_ENABLE_OPERATION:
//            if ((((cia_state_t) ((intptr_t *) condition) != CIA_SWITCHED_ON) ||
//                 ((cia_state_t) ((intptr_t *) condition) != CIA_OPERATION_ENABLED))
//                && ctrl_state_change_cycle_count > ctrl_state_change_timeout) {
//                state_mismatch = true;
//            }
//            break;
        case CIA_DISABLE_VOLTAGE:
            if ((((cia_state_t) ((intptr_t *) condition) != CIA_SWITCHED_ON) ||
                 ((cia_state_t) ((intptr_t *) condition) != CIA_OPERATION_ENABLED))
                && ctrl_state_change_cycle_count > ctrl_state_change_timeout) {
                state_mismatch = true;
            }
            break;
        case CIA_QUICK_STOP:
            if ((((cia_state_t) ((intptr_t *) condition) != CIA_QUICK_STOP_ACTIVE) ||
                 ((cia_state_t) ((intptr_t *) condition) != CIA_SWITCH_ON_DISABLED)) &&
                ctrl_state_change_cycle_count > ctrl_state_change_timeout) {
                state_mismatch = true;
            }
            break;
        case CIA_DISABLE_OPERATION:
            if (((cia_state_t) ((intptr_t *) condition)) != CIA_SWITCHED_ON &&
                (ctrl_state_change_cycle_count > ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
        case CIA_ENABLE_OPERATION:
            if (((cia_state_t) ((intptr_t *) condition) == CIA_OPERATION_ENABLED) &&
                (ctrl_state_change_cycle_count > ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
        case CIA_FAULT_RESET:
            break;
    }
    if (state_mismatch) {
        LL_TRACE(GBEM_SM_LOG_EN,
                 "sm: Mismatch between drive state and the controlword and the current state & no transitions occurring");
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
                if (moo_disp > 0)
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

//#if CTRL_COPY_ACTVEL == 1
    ctrl_copy_actvel();
//#endif
//#if CTRL_COPY_ACTTORQ == 1
    ctrl_copy_acttorq();
//#endif

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

#if USE_ESTOP_RESET == 1 && DISABLE_ESTOP_CHECKING != 1

    if (ctrl_estop_din.slave_num < 1) {
        UM_FATAL("GBEM: no ctrl_estop_din is defined!");
    }

//    if (!BIT_CHECK(dpm_in->digital, CTRL_ESTOP_DIN)) {
    if (!ec_pdo_get_input_bit(ctrl_estop_din.slave_num, ctrl_estop_din.bit_num)) {
//        printf("estop=true");
        estop = true;
    } else {
//        if (BIT_CHECK(dpm_in->digital, CTRL_ESTOP_RESET_DIN)) {
        if (ec_pdo_get_input_bit(ctrl_estop_reset_din.slave_num, ctrl_estop_reset_din.bit_num)) {
            estop = false;
        }
    }
#endif
#if  USE_ESTOP_RESET == 0 && DISABLE_ESTOP_CHECKING != 1

    if (ctrl_estop_din_reset.slave_num < 1){
        UM_FATAL("GBEM: no ctrl_estop_din_reset is defined!");
    }
//    if (!BIT_CHECK(dpm_in->digital, CTRL_ESTOP_DIN)) {
        if (!ec_pdo_get_input_bit(ctrl_estop_din.slave_num, ctrl_estop_din.bit_num)){
        estop = true;
    }
    else {
        estop = false;
    }
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
    event_data.machine_move_not_op_enabled_fault_req = BIT_CHECK(dpm_out->machine_word,
                                                                 CTRL_MOVE_NOT_OP_ENABLED_FAULT_REQ_BIT_NUM);


    event_data.machine_request_error = BIT_CHECK(dpm_out->machine_word, CTRL_MACHINE_CTRL_WRD_REQUEST_FAULT_BIT_NUM);

    //gbc_internal_fault
    event_data.gbc_internal_fault = BIT_CHECK(dpm_out->machine_word, CTRL_GBC_INTERNAL_FAULT_REQ_BIT_NUM);

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
            if (moo_disp > 0)
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
                    (void *) (intptr_t) &event_data});
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
                "GBEM: State machine reached a final state - this should never happen unless something has gone inexplicably pear shaped");
    }

//read current state of state machine
    current_state = (cia_state_t) (stateM_currentState(m)->data);

    ecm_status.machine_state = current_state;
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

//copy setpos from DPM_OUT (read) to EC_OUT
//    ctrl_copy_setpos();
    ctrl_copy_values_to_drives();

#if CTRL_ENABLE_FORCING == 1
    ctrl_process_forces_out();
#endif


    if (current_state == CIA_OPERATION_ENABLED) {
        ctrl_process_iomap_out(false);

    } else {
        //todo crit do we want to zero?
        ctrl_process_iomap_out(false);
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
//            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
//                     "GBEM: Missing function pointer for map_drive_get_actvel_wrd on drive [%u]", i);
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
//            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
//                     "GBEM: Missing function pointer for map_drive_get_acttorq_wrd on drive [%u]", i);
        }
    }
}


#define CTRL_POS_JUMP_THRESHOLD 25000

__attribute__((unused)) static void ctrl_check_for_big_pos_jump(uint16_t drive, int32_t current_position) {
    static int32_t last_pos[MAP_NUM_DRIVES];
    if ((abs(current_position - last_pos[drive])) > CTRL_POS_JUMP_THRESHOLD) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Position jump too big for drive [%u] jump [%u]", drive,
                 abs(current_position - last_pos[drive]));
    }
    last_pos[drive] = current_position;
}


/**
 * Copies correct set values to the drives based on the MOO for the drive
 */
static void ctrl_copy_values_to_drives(void) {
    gberror_t grc;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {

        switch (map_drive_moo[i]) {

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

                break;

            case CIA_MOO_CSV:

                if (*map_drive_set_setvel_wrd_function_ptr[i] != NULL) {
                    //run control loop
                    int32_t adjusted_velocity = calculateVelocityCommand(
                            dpm_out->joint_set_position[i],
                            dpm_in->joint_actual_position[i]);

                    grc = map_drive_set_setvel_wrd_function_ptr[i](i,
                                                                   dpm_out->joint_set_velocity[i] + adjusted_velocity);
                    if (grc != E_SUCCESS) {
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: drive setvel function error [%s]", gb_strerror(grc));
                    }
                } else {
                    LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                             "GBEM: Missing function pointer for map_drive_set_setvel_wrd on drive [%u]", i);
                }


//                if (get_demand_velocity(0) > 0) {
//                    printf("pos %d\n", get_demand_position(0));
//                    printf("vel %d\n", get_demand_velocity(0));
//                    printf("torq %d\n", get_demand_acceleration(0));
//                }

                break;

            case CIA_MOO_CST:

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
                    iomap_set_gbc_digital_in_from_pdo(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                      map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                                                      map_iomap[i].gbc.ionum);
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
            }//switch on datatype
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
                        iomap_set_pdo_out_bool(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                               map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                                               BIT_CHECK(dpm_out->digital, map_iomap[i].gbc.ionum));

                    } else {
                        iomap_set_pdo_out_bool(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                               map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                                               false);
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
            }//switch on datatype

        } // if
    } //for loop
}

