/**
 ******************************************************************************
 * @file           :  print_status.c
 * @brief          :  function to the contents of ecm_status
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "print_status.h"
#include <sys/file.h>
#include "dpm.h"
#include <unistd.h>
#include "log.h"
#include "control.h"

#include <stdio.h>


void printBinaryUint64(uint64_t n) {
    // Number of bits in a uint64_t
    int numBits = sizeof(uint64_t) * 8;

    for (int i = numBits - 1; i >= 0; i--) {
        // Use bitwise shift and mask to extract each bit
        uint64_t bit = (n >> i) & 1;
        printf("%lu", bit); // Print the bit (0 or 1)
    }
}

void printBinaryUint32(uint64_t n) {
    // Number of bits in a uint32_t
    int numBits = sizeof(uint32_t) * 8;

    for (int i = numBits - 1; i >= 0; i--) {
        // Use bitwise shift and mask to extract each bit
        uint64_t bit = (n >> i) & 1;
        printf("%llu", bit); // Print the bit (0 or 1)
    }
}

/**
 * @brief printf status data
 * @param data Source data structure
 * @return
 */
void print_status(ecm_status_t *data) {
    printf("Number of slaves: %d\n", data->slavecount);
    printf("Number of drives: %d\n", data->drive_count);


    printf("Machine control word [%s]\n", cia_command_names[cia_ctrlwrd_to_command(dpm_out->machine_word)]);
    printf("machine status word [%s]\n", cia_state_names[cia_statwrd_to_state(dpm_in->machine_word)]);

    printf("DIN:\n");
    printBinaryUint64(dpm_in->digital);
    printf("\n");
    printf("DOUT:\n");
    printBinaryUint64(dpm_out->digital);
    printf("\n");

    printf("Machine active faults:\n");
    printBinaryUint32(dpm_in->active_fault_word);
    printf("\n");

    printf("Machine historic faults:\n");
    printBinaryUint32(dpm_in->fault_history_word);
    printf("\n");


    if (ecm_status.cyclic_state == ECM_ERROR) {
        printf("EtherCAT error detected\n");
    } else {
        printf("EtherCAT error not detected\n");
    }

    if (ecm_status.gbc_connected) {
        printf("GBC connected\n");
    } else {
        printf("GBC not connected\n");
    }

    printf("Slave error messages:\n");

    int current_index = ecm_status.slave_error_messages.head;
    if (ecm_status.slave_error_messages.num_slots_full == 0) {
        printf("No slave error messages\n");
    }

    for (int i = 0; i < ecm_status.slave_error_messages.num_slots_full; i++) {
        printf("%s\n", (char *) ec_peek_circular_slave_error_message(&ecm_status.slave_error_messages, current_index));
        if (ecm_status.slave_error_messages.head == MAX_NUM_SLAVE_ERROR_MESSAGES) {
            current_index = 0;
        } else {
            current_index++;
        }
    }

    printf("Slaves:\n");
    for (int i = 1; i < data->slavecount + 1; i++) {
        printf("Slave %d:\n", i);
        printf("Name: %s\n", data->map[i].name);
        printf("Obits: %d\n", data->map[i].Obits);
        printf("Ibits: %d\n", data->map[i].Ibits);
        printf("Obytes: %d\n", data->map[i].Obytes);
        printf("Ibytes: %d\n", data->map[i].Ibytes);
        printf("hasDC: %d\n", data->map[i].hasdc);
        printf("address: %d\n", data->map[i].configadr);
        printf("number: %d\n", i);
        printf("state: %s\n", ec_state_to_string[data->map[i].state]);
        printf("ALstatuscode: %d\n", data->map[i].ALstatuscode);
        //ec_ALstatuscode2string
        printf("ALstatuscode: %s\n", ec_ALstatuscode2string(data->map[i].ALstatuscode));
    }

    printf("Drives:\n");
    for (int i = 0; i < data->drive_count; i++) {
        cia_commands_t command = cia_ctrlwrd_to_command(data->drives[i].command);
        cia_state_t state = cia_statwrd_to_state(dpm_in->joint_statusword[i]);

        //test if we have an invalid command before we try and get its name from the array
        if (command > 6) {
            printf("command: unknown command\n");
        } else {
            printf("command: %s\n", cia_command_names[command]);
        }

        //test if we have an invalid state before we try and get its name from the array
        if (state > 7) {
            printf("status: unknown state\n");
        } else {
            printf("status: %s\n", cia_state_names[state]);
        }

        printf("name: %s\n", data->drives[i].name);
        printf("actpos: %d\n", dpm_in->joint_actual_position[i]);
        printf("setpos: %d\n", dpm_out->joint_set_position[i]);
        printf("actvel: %d\n", dpm_in->joint_actual_velocity[i]);
        printf("setvel: %d\n", dpm_out->joint_set_velocity[i]);
        printf("acttorq: %d\n", dpm_in->joint_actual_torque[i]);
        printf("settorq: %d\n", dpm_out->joint_set_torque[i]);

        printf("error_message: %s\n", data->drives[i].error_message);
    }


    // typedef struct {
    //     uint8_t master_slave_no; //set in control.c
    //     uint8_t slave_count; // set in ec_functions.c
    //     ecm_fsoe_slave_type_t slave_type[EC_MAXSLAVE]; // set in ec_functions.c
    //     uint32_t master_state;
    //     uint32_t master_error_code;
    //     uint32_t slave_state[EC_MAXSLAVE];
    //     uint16_t slave_connection_id[EC_MAXSLAVE];
    //     fsoe_slave_high_level_state_t slave_high_level_state[EC_MAXSLAVE];
    //     fsoe_master_high_level_state_t master_high_level_state;
    // } ecm_status_fsoe_t;


    printf("FSOE:\n");
    printf("master_slave_no: %d\n", ecm_status.fsoe.master_slave_no);
    printf("slave_count: %d\n", ecm_status.fsoe.slave_count);
    printf("master_error_code: %d\n", ecm_status.fsoe.master_error_code);
    printf("master_high_level_state: %d\n", ecm_status.fsoe.master_high_level_state);

    for (int i = 0; i < EC_MAXSLAVE; i++) {
        if (ecm_status.fsoe.slave_type[i] == FSOE_SLAVE_TYPE_NONE) {
            continue;
        }

        printf("Slave [%d] is an FSoE slave\n", i);


        // FSOE_SLAVE_TYPE_NONE,
        // FSOE_SLAVE_TYPE_SYNAPTICON,
        // FSOE_SLAVE_TYPE_EL1904,
        // FSOE_SLAVE_TYPE_EL2904,
        // FSOE_SLAVE_TYPE_SCU_1_EC,
        // FSOE_SLAVE_TYPE_EL6900,
        // FSOE_SLAVE_TYPE_EL6910

        switch (ecm_status.fsoe.slave_type[i]) {
            case FSOE_SLAVE_TYPE_SYNAPTICON:
                printf("slave_type: FSOE_SLAVE_TYPE_SYNAPTICON\n");
                break;
            case FSOE_SLAVE_TYPE_EL1904:
                printf("slave_type: FSOE_SLAVE_TYPE_EL1904\n");
                break;
            case FSOE_SLAVE_TYPE_EL2904:

                printf("slave_type: FSOE_SLAVE_TYPE_EL2904\n");
                break;
            case FSOE_SLAVE_TYPE_SCU_1_EC:
                printf("slave_type: FSOE_SLAVE_TYPE_SCU_1_EC\n");
                break;
            case FSOE_SLAVE_TYPE_EL6900:
                printf("slave_type: FSOE_SLAVE_TYPE_EL6900\n");
                break;
            case FSOE_SLAVE_TYPE_EL6910:
                printf("slave_type: FSOE_SLAVE_TYPE_EL6910\n");
                break;
            default:
                printf("slave_type: unknown\n");
        }
        printf("slave_state: %d\n", ecm_status.fsoe.slave_state[i]);
        printf("slave_connection_id: %d\n", ecm_status.fsoe.slave_connection_id[i]);
        printf("slave_high_level_state: %d\n", ecm_status.fsoe.slave_high_level_state[i]);
    }
}

