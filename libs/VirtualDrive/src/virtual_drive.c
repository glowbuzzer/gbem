/**
 ******************************************************************************
 * @file           :  virtual.c
 * @brief          :  virtual drive functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "virtual_drive.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"
#include "map_config.h"
#include "map.h"

static void virtual_drive_process_state(uint16_t ctrlwrd, cia_state_t *state);


static int32_t virtual_drive_position[MAP_MAX_NUM_DRIVES];
static int32_t virtual_drive_velocity[MAP_MAX_NUM_DRIVES];
static int32_t virtual_drive_torque[MAP_MAX_NUM_DRIVES];

uint16_t virtual_drive_controlword[MAP_MAX_NUM_DRIVES];
uint16_t virtual_drive_statusword[MAP_MAX_NUM_DRIVES];

static cia_state_t virtual_drive_state[MAP_MAX_NUM_DRIVES] = {[0 ... MAP_MAX_NUM_DRIVES - 1]=CIA_SWITCH_ON_DISABLED};


#define VIRTUAL_DRIVE_MOMENT_OF_INERTIA 0.1

/**
 * @brief get the value of the remote bit from an virtual drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_virtual(const uint16_t drive) {
    //remote bit not implemented on virtual drives
    return true;
}

/**
 * @brief gets the mode of operation (MOO) for virtual drives
 * @param drive_number
 * @return  moo value (e.g. 8 for CSP)
 * @attention handles sub-drives
 */
int8_t ec_get_moo_pdo_virtual(const uint16_t drive) {

    return map_drive_moo[drive];


}

gberror_t ec_set_moo_pdo_virtual(const uint16_t drive, int8_t moo) {
    return E_SUCCESS;
}

int32_t ec_get_control_effort_wrd_virtual(uint16_t drive) {
    return 0;
}


gberror_t ec_set_setveloffset_wrd_virtual(const uint16_t drive, const int32_t setveloffset) {
    return E_SUCCESS;
}

gberror_t ec_set_settorqoffset_wrd_virtual(const uint16_t drive, const int32_t settorqoffset) {

    return E_SUCCESS;
}

uint8_t *ec_get_error_string_pdo_virtual(uint16_t drive, bool *error) {
    *error = false;
    return (uint8_t *) "No error";
}


bool ec_get_follow_error_virtual(const uint16_t drive) {
    return false;
}


/**
 * @brief get actpos for an Virtual drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actpos_wrd_virtual(const uint16_t drive) {

    return virtual_drive_position[drive];

}

/**
 * @brief get actvel for an Virtual drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actvel_wrd_virtual(const uint16_t drive) {

    return virtual_drive_velocity[drive];

}


/**
 * @brief get acttorq for an Virtual drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_acttorq_wrd_virtual(const uint16_t drive) {

    return virtual_drive_torque[drive];

}


/**
 * @brief set control word for Virtualdrive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_virtual(const uint16_t drive, const uint16_t ctrlwrd) {

    virtual_drive_controlword[drive] = ctrlwrd;

    virtual_drive_process_state(ctrlwrd, &virtual_drive_state[drive]);

    return E_SUCCESS;
}

/**
 * @brief set status word for Virtual drive
 * @param drive
 * @return status word

 */
uint16_t ec_get_stat_wrd_virtual(const uint16_t drive) {

    return cia_state_to_statwrd(virtual_drive_state[drive]);

}

/**
 * @brief set setpos for an virtual drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setpos_wrd_virtual(const uint16_t drive, const int32_t setpos) {

    virtual_drive_position[drive] = setpos;
    return E_SUCCESS;
}


/**
 * @brief set setvel for an virtual drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setvel_wrd_virtual(const uint16_t drive, const int32_t setvel) {

    virtual_drive_velocity[drive] = setvel;
    virtual_drive_position[drive] =
            virtual_drive_position[drive] + (int32_t) ((double) setvel * ((double) MAP_CYCLE_TIME / (double) 1000));
    return E_SUCCESS;
}


/**
 * @brief set settorq for an virtual drive
 * @param drive
 * @param settorq
 * @return gberror
 */
gberror_t ec_set_settorq_wrd_virtual(const uint16_t drive, const int32_t settorq) {

    double angular_acceleration = (double) settorq / (double) VIRTUAL_DRIVE_MOMENT_OF_INERTIA;

    virtual_drive_velocity[drive] =
            virtual_drive_velocity[drive] +
            (int32_t) (angular_acceleration * ((double) MAP_CYCLE_TIME / (double) 1000));

    virtual_drive_position[drive] =
            virtual_drive_position[drive] +
            (int32_t) ((double) virtual_drive_velocity[drive] * ((double) MAP_CYCLE_TIME / (double) 1000));

    virtual_drive_torque[drive] = settorq;
    return E_SUCCESS;


}

/**
 * @brief runs state machine for a single motor
 * @param ctrlwrd
 * @param state
 * @param status
 */
static void virtual_drive_process_state(uint16_t ctrlwrd, cia_state_t *state) {


    switch (*state) {
        case CIA_FAULT_REACTION_ACTIVE:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_FAULT_RESET) {
                *state = CIA_FAULT;
            } else {
                *state = CIA_FAULT_REACTION_ACTIVE;
            }
            break;
        case CIA_FAULT:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_FAULT_RESET) {

                *state = CIA_SWITCH_ON_DISABLED;
            } else {
                *state = CIA_FAULT;
            }
            break;
        case CIA_NOT_READY_TO_SWITCH_ON:
            *state = CIA_SWITCH_ON_DISABLED;
            break;
        case CIA_SWITCH_ON_DISABLED:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else {
                *state = CIA_SWITCH_ON_DISABLED;
            }

            break;
        case CIA_READY_TO_SWITCH_ON:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SWITCH_ON) {
                *state = CIA_SWITCHED_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else {
                *state = CIA_READY_TO_SWITCH_ON;
            }
            break;
        case CIA_SWITCHED_ON:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_ENABLE_OPERATION) {
                *state = CIA_OPERATION_ENABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else {
                *state = CIA_SWITCHED_ON;
            }

            break;
        case CIA_OPERATION_ENABLED:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_OPERATION) {
                *state = CIA_SWITCHED_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_QUICK_STOP) {
                *state = CIA_QUICK_STOP_ACTIVE;
            } else {
                *state = CIA_OPERATION_ENABLED;
            }


            break;
        case CIA_QUICK_STOP_ACTIVE:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_ENABLE_OPERATION) {
                *state = CIA_OPERATION_ENABLED;
            } else {
                *state = CIA_QUICK_STOP_ACTIVE;
            }
            break;
        default:
            break;
    }

}

