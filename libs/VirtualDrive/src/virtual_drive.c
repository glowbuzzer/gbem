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

static void virtual_drive_process_state(uint16_t ctrlwrd, cia_state_t *state);


//int32_t virtual_drive_actpos[MAP_MAX_NUM_DRIVES];
int32_t virtual_drive_position[MAP_MAX_NUM_DRIVES];
uint16_t virtual_drive_controlword[MAP_MAX_NUM_DRIVES];
uint16_t virtual_drive_statusword[MAP_MAX_NUM_DRIVES];

//todo
cia_state_t virtual_drive_state[MAP_MAX_NUM_DRIVES] = {CIA_SWITCH_ON_DISABLED, CIA_SWITCH_ON_DISABLED,
                                                       CIA_SWITCH_ON_DISABLED};


/**
 * @brief get the value of the remote bit from an virtual drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_virtual(const uint16_t drive) {
    //remote bit not implemented on AW-J-series drives
    return true;
}

/**
 * @brief gets the mode of operation (MOO) for virtual drives
 * @param drive_number
 * @return  moo value (8 for CSP)
 * @attention handles sub-drives
 */
int8_t ec_get_moo_pdo_virtual(const uint16_t drive) {

    return CIA_MOO_CSP;


}


bool ec_get_follow_error_virtual(const uint16_t drive) {
    return false;
}


/**
 * @brief get actpos for an AW-J-Series drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actpos_wrd_virtual(const uint16_t drive) {

    return virtual_drive_position[drive];

}


/**
 * @brief set control word for AW-J-series drive
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
 * @brief set status word for AW-J-series drive
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

