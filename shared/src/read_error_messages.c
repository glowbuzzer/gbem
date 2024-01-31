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
//This is now just drive error messages
_Noreturn void ec_read_error_messages(void) {
    while (1) {
        if (current_state == CIA_FAULT || current_state == CIA_FAULT_REACTION_ACTIVE) {
            for (int i = 0; i < MAP_NUM_DRIVES; i++) {
                read_drive_error_code_into_ecm_status(i);
            }
        }
        sleep(5);
    }
}
