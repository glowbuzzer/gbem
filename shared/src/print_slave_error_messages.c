/**
 ******************************************************************************
 * @file           :  print_slave_error_messages
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "control.h"
#include "stdio.h"
#include "ethercat.h"
#include "user_message.h"

void print_slave_error_messages(void) {


    for (int i = 1; i < ecm_status.slavecount + 1; i++) {
        if (ecm_status.map[i].state == EC_STATE_ERROR) {
            UM_INFO(GBEM_UM_EN, "GBEM: Slave [%d]", i);
            UM_INFO(GBEM_UM_EN, "GBEM: State [%s]", ec_state_to_string[ecm_status.map[i].state]);
            UM_INFO(GBEM_UM_EN, "GBEM: ALstatuscode [0x%#04x]", ecm_status.map[i].ALstatuscode);
            UM_INFO(GBEM_UM_EN, "GBEM: ALstatuscode [%s]", ec_ALstatuscode2string(ecm_status.map[i].ALstatuscode));
        }
    }
    int current_index = ecm_status.slave_error_messages.head - 1;

    if (current_index < 0) {
        current_index = MAX_NUM_SLAVE_ERROR_MESSAGES - 1; // Wrap around to the last slot
    }

    for (int i = 0; i < ecm_status.slave_error_messages.num_slots_full; i++) {
        UM_INFO(GBEM_UM_EN, "GBEM: Slave error message [%s] [%d]",
                (char *) ec_peek_circular_slave_error_message(&ecm_status.slave_error_messages, current_index));
        current_index = (current_index + 1) % MAX_NUM_SLAVE_ERROR_MESSAGES; // Use modulo to wrap around
    }
}