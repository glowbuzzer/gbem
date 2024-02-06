/**
 ******************************************************************************
 * @file           :  read_error_messages
 * @brief          :  thread (low priority) that reads error messages from the drives
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include <unistd.h>
#include "map.h"
#include "read_drive_error_code_into_ecm_status.h"
#include "control.h"
#include "print_slave_error_messages.h"
#include "stdio.h"

//This thread is only created after a successful boot

//Only reads drive error messages with an SDO read - if the machine map contains NULLs for the SDO drive read this is a NOP

_Noreturn void ec_read_error_messages(void) {
    while (1) {
        if (current_state == CIA_FAULT || current_state == CIA_FAULT_REACTION_ACTIVE) {
            for (int i = 0; i < MAP_NUM_DRIVES; i++) {
                read_drive_error_code_into_ecm_status(i, DRIVE_ERROR_MESSAGE_READ_TYPE_SDO);
            }
        }
        sleep(5);
    }
}
