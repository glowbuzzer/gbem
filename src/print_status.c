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

    printf("Number of int32 ins: %d\n", data->gbc_max_int32_in_count);

    printf("Number of int32 outs: %d\n", data->gbc_max_int32_out_count);

    printf("Number of float ins: %d\n", data->gbc_max_float_in_count);

    printf("Number of float outs: %d\n", data->gbc_max_float_out_count);

    printf("Number of digital ins: %d\n", data->gbc_max_digital_in_count);

    printf("Number of digital outs: %d\n", data->gbc_max_digital_out_count);


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

    printf("Integers in:\n");
    for (int i = 0; i < data->gbc_max_int32_in_count; i++) {
        printf("%d\n", dpm_in->integer32[i]);
    }

    printf("Integers out:\n");
    for (int i = 0; i < data->gbc_max_int32_out_count; i++) {
        printf("%d\n", dpm_out->integer32[i]);
    }

    printf("Floats in:\n");

    for (int i = 0; i < data->gbc_max_float_in_count; i++) {
        printf("%f\n", dpm_in->analog[i]);
    }

    printf("Floats out:\n");
    for (int i = 0; i < data->gbc_max_float_out_count; i++) {
        printf("%f\n", dpm_out->analog[i]);
    }

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


}

