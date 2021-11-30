/**
 ******************************************************************************
 * @file           :  status.c
 * @brief          :  functions to produce a JSON status file cyclically
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "status.h"
#include <sys/file.h>
#include "json_maker.h"
#include "dpm.h"
#include <unistd.h>
#include "log.h"
#include "main.h"
#include "ec_functions.h"
#include "control.h"


/**
 * @brief this is the thread for writing the status JSON file
 * @param argument
 */
void ec_emstat(void *argument) {
    /* this function never exits so no need for statics */
    bool first_run = true;
    bool start_output = false;
    int ascii_counter = 0;
    uint32_t trigger = 2000; // ms
    const int numDots = 50;
    const char prompt[] = "GBEM: running ";

    for (;;) {
        /* Here we have a short pause before outputting the JSON status and const files and spitting out the spinner*/
        if (ecm_status.cycle_count > (DELAY_TO_START_JSON_OUTPUT_SECS * (1000 / MAP_CYCLE_TIME))) {
            start_output = true;
        }
        if (start_output) {
            if (first_run) {
                write_const_json();
                first_run = false;
            }
            write_status_json();

//#define STARSPINNER
#ifdef STARSPINNER
            // Return and clear with spaces, then return and print prompt.
            printf("\r%*s\r%s", sizeof(prompt) - 1 + numDots, "", prompt);
            fflush(stdout);

            // Print numDots number of dots, one every trigger milliseconds.
            for (int i = 0; i < numDots; i++) {
                usleep(trigger * (uint32_t) 1000);
                fputc('#', stdout);
                fflush(stdout);
            }

#endif


#ifdef XSPINNER
            /* littler spinner to let us know that GBEM is still running */
            ascii_counter++;
            if (ascii_counter == 4) {
                ascii_counter = 0;
            }
            printf("\b%c", "|/-\\"[ascii_counter]);
            fflush(stdout);
#endif
        }
        /* this is the period at which the JSON status file is written to disk */
        sleep(JSON_STATUS_UPDATE_PERIOD_SECS);
    }
}

/**
 * @brief serialises the JSON status data
 * @param dest Destination memory block
 * @param data Source data structure
 * @return
 */
char *status_json_data(char *dest, const ecm_status_t *data) {
    dest = json_objOpen(dest, NULL);
    dest = json_uint(dest, "heartbeat_in", dpm_in->heartbeat);

    dest = json_uint(dest, "heartbeat_out", dpm_out->heartbeat);

    dest = json_str(dest, "machine_control_word", cia_command_names[cia_ctrlwrd_to_command(dpm_out->machine_word)]);

    dest = json_str(dest, "machine_status_word", cia_state_names[cia_statwrd_to_state(dpm_in->machine_word)]);

    if (sizeof(long)==4){
        //32 bit linux, well just a platform where a long is 4 bytes
        dest = json_uverylong(dest, "din", (unsigned long long) dpm_in->digital);
        dest = json_uverylong(dest, "dout", (unsigned long long) dpm_out->digital);
    } else{
        //64 bit linux, well just a platform where a long is 8 bytes
        dest = json_ulong(dest, "din", (long long) dpm_in->digital);
        dest = json_ulong(dest, "dout", (long long) dpm_out->digital);
    }

    dest = json_uint(dest, "machine_active_faults", dpm_in->active_fault_word);
    dest = json_uint(dest, "machine_historic_faults", dpm_in->fault_history_word);

    dest = json_arrOpen(dest, "int_in");


    for (int i = 0; i < data->gbc_max_int32_in_count; i++) {
        dest = json_int(dest, NULL, dpm_in->integer32[i]);
    }
    dest = json_arrClose(dest);

    dest = json_arrOpen(dest, "int_out");
    for (int i = 0; i < data->gbc_max_int32_out_count; i++) {
        dest = json_int(dest, NULL, dpm_out->integer32[i]);
    }
    dest = json_arrClose(dest);

    dest = json_arrOpen(dest, "float_in");
    for (int i = 0; i < data->gbc_max_float_in_count; i++) {
        dest = json_double(dest, NULL, dpm_in->analog[i]);
    }
    dest = json_arrClose(dest);

    dest = json_arrOpen(dest, "float_out");
    for (int i = 0; i < data->gbc_max_float_out_count; i++) {
        dest = json_double(dest, NULL, dpm_out->analog[i]);
    }
    dest = json_arrClose(dest);

//todo    ecm_status.ec_check_found_error?
    if (ecm_status.cyclic_state == ECM_ERROR) {
        dest = json_str(dest, "ecat_error", "EtherCAT error detected");
    } else {
        dest = json_str(dest, "ecat_error", "No error");
    }

    if (ecm_status.gbc_connected) {
        dest = json_str(dest, "gbc_connected", "Connection successful");
    } else {
        dest = json_str(dest, "gbc_connected", "Connection failure");
    }

    dest = json_arrOpen(dest, "slave_errors");

    int current_index = ecm_status.slave_error_messages.head;
    for (int i = 0; i < ecm_status.slave_error_messages.num_slots_full; i++) {
        dest = json_str(dest, NULL, (char *) ec_peek_circular_slave_error_message(&ecm_status.slave_error_messages, current_index));
        if (ecm_status.slave_error_messages.head == MAX_NUM_SLAVE_ERROR_MESSAGES) {
            current_index = 0;
        } else {
            current_index++;
        }
//        dest = json_str(dest, NULL, ecm_status.slave_error_messages.slave_error_message[i]);
    }
    dest = json_arrClose(dest);

    dest = json_arrOpen(dest, "slaves");
    for (int i = 1; i < data->slavecount + 1; i++) {
        dest = json_objOpen(dest, NULL);
        dest = json_str(dest, "state", ec_state_to_string[data->map[i].state]);
        dest = json_uint(dest, "ALstatuscode", data->map[i].ALstatuscode);
        dest = json_objClose(dest);
    }
    dest = json_arrClose(dest);

    dest = json_arrOpen(dest, "drives");
    for (int i = 0; i < data->drive_count; i++) {
        dest = json_objOpen(dest, NULL);

        cia_commands_t command = cia_ctrlwrd_to_command(data->drives[i].command);
        cia_state_t state = cia_statwrd_to_state(dpm_in->joint_statusword[i]);

        //test if we have an invalid command before we try and get its name from the array
        if (command > 6 ){
            dest = json_str(dest, "command", "unknown command");
        }else{
            dest = json_str(dest, "command", cia_command_names[command]);
        }

        //test if we have an invalid state before we try and get its name from the array
        if (state > 7){
            dest = json_str(dest, "status", "unknown state");
        } else {
            dest = json_str(dest, "status", cia_state_names[state]);
        }

        dest = json_int(dest, "actpos", dpm_in->joint_actual_position[i]);
        dest = json_int(dest, "setpos", dpm_out->joint_set_position[i]);
        dest = json_str(dest, "error_message", data->drives[i].error_message);
        dest = json_objClose(dest);
    }
    dest = json_arrClose(dest);


    dest = json_objClose(dest);
    return dest;
}

/**
 * @brief serialises the JSON const data
 * @param dest Destination memory block
 * @param name
 * @param data Source data structure
 * @return
 */
char *const_json_data(char *dest, const ecm_status_t *data) {
    dest = json_objOpen(dest, NULL);

    dest = json_int(dest, "number_of_slaves", data->slavecount);
    dest = json_int(dest, "number_of_drives", data->drive_count);


    dest = json_int(dest, "number_of_int32_ins", data->gbc_max_int32_in_count);

    dest = json_int(dest, "number_of_int32_outs", data->gbc_max_int32_out_count);

    dest = json_int(dest, "number_of_float_ins", data->gbc_max_float_in_count);
    dest = json_int(dest, "number_of_float_outs", data->gbc_max_float_out_count);


    dest = json_int(dest, "number_of_float_ins", data->gbc_max_float_in_count);
    dest = json_int(dest, "number_of_float_outs", data->gbc_max_float_out_count);

    dest = json_int(dest, "number_of_digital_ins", data->gbc_max_digital_in_count);
    dest = json_int(dest, "number_of_digital_outs", data->gbc_max_digital_out_count);




    dest = json_arrOpen(dest, "slaves");
    for (int i = 1; i < data->slavecount + 1; i++) {
        dest = json_objOpen(dest, NULL);
        dest = json_str(dest, "name", data->map[i].name);
        dest = json_uint(dest, "Obits", data->map[i].Obits);
        dest = json_uint(dest, "Ibits", data->map[i].Ibits);
        dest = json_uint(dest, "Obytes", data->map[i].Obytes);
        dest = json_uint(dest, "Ibytes", data->map[i].Ibytes);
        dest = json_uint(dest, "hasDC", data->map[i].hasdc);
        dest = json_uint(dest, "address", data->map[i].configadr);
        dest = json_uint(dest, "number", (uint32_t) i);
        dest = json_objClose(dest);
    }
    dest = json_arrClose(dest);

    dest = json_arrOpen(dest, "drives");
    for (int i = 0; i < data->drive_count; i++) {
        dest = json_objOpen(dest, NULL);
        dest = json_str(dest, "name", ecm_status.drives[i].name);
        dest = json_objClose(dest);
    }
    dest = json_arrClose(dest);
    dest = json_objClose(dest);
    return dest;
}


/** Convert a data structure to a root JSON object.
  * @param dest Destination memory block
  * @param data Source data structure
  * @return  The JSON string length. */
int status_data_to_json(char *dest, ecm_status_t *data) {
    char *p = status_json_data(dest, data);
    p = json_end(p);
    return p - dest;
}

/** Convert a data structure to a root JSON object
  * @param dest Destination memory block
  * @param data Source data structure
  * @return  The JSON string length */
int const_data_to_json(char *dest, ecm_status_t *data) {
    char *p = const_json_data(dest, data);
    p = json_end(p);
    return p - dest;
}


/**
 * @brief write status JSON to file
 * @return
 */
bool write_status_json() {
    FILE *fp1;
    //stack based buffer to write JSON in to
    char buff[8000];

    //so it is a bit later but...
    int len = status_data_to_json(buff, &ecm_status);
    if (len >= sizeof buff) {
        LL_FATAL("GBEM: %s%d%s%d\n", "Error. Len: ", len, " Max: ", (int) sizeof buff - 1);
        return false;
    }

    if (full_path_for_emstat_json_status[0] == '\0') {
        LL_FATAL("GBEM: empty path for status json");
    }

    fp1 = fopen(full_path_for_emstat_json_status, "w+");
//    int lock = flock(fp1, LOCK_SH);
    if (fp1) {
        fwrite(buff, sizeof(char), (size_t) len, fp1);
        fclose(fp1);
    } else {
        return false;
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: failed to open emstat status file for writing");
    }


//    int release = flock(fp1, LOCK_UN);
    return true;
}

/**
 * @brief write const JSON to file
 * @return
 */
bool write_const_json() {
    FILE *fp2;
    char buff[8000];
    int len = const_data_to_json(buff, &ecm_status);

    //so it is a bit later but...
    if (len >= sizeof buff) {
        LL_FATAL("GBEM: %s%d%s%d\n", "Error. Len: ", len, " Max: ", (int) sizeof buff - 1);
        return false;
    }
    if (full_path_for_emstat_json_const[0] == '\0') {
        LL_FATAL("GBEM: empty path for const json");
    }


    fp2 = fopen(full_path_for_emstat_json_const, "w+");
//    int lock = flock(fp2, LOCK_SH);
    if (fp2) {
        fwrite(buff, sizeof(char), (size_t) len, fp2);
        fclose(fp2);

    } else {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: failed to open emstat const file for writing");
        return false;
    }
    return true;


}
