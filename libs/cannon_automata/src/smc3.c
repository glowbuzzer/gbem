
/**
 ******************************************************************************
 * @file           :  smc3.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "smc3.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"



/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_smc3 = {
        .number_of_entries = 1,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_smc3 = {
        .number_of_entries = 1,
        .SM_assignment_index = 0x1c13};


/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_smc3[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1602,
};

/* This is used for the fixed PDO remapping */
uint16_t map_SM3_index_of_assigned_PDO_smc3[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a02,
};



gberror_t ec_pdo_map_smc3(const uint16_t slave) {
    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "SODs configured for PDO mapping for SMC3 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to SCM3 slave [%u]",
                slave);
    }

    if (!ec_sdo_write_uint16(slave, map_SM2_smc3.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_smc3.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_smc3.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_smc3.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_smc3[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_smc3.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_smc3.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_smc3[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_smc3.SM_assignment_index, 0,
                             map_SM2_smc3.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_smc3.SM_assignment_index, 0,
                             map_SM3_smc3.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;


    //all applied correctly
    return E_SUCCESS;
}


gberror_t ec_initial_pdo_smc3(const uint16_t slave) {

    ec_pdo_set_output_int8(slave, SMC3_MOOSET_PDO_INDEX, CIA_MOO_CSP);

    return E_SUCCESS;
}

gberror_t ec_standard_sdos_smc3(const uint16_t slave) {

//    if (!ec_sdo_write_int32(slave, N5_FEED_CONSTANT_SDO_INDEX, N5_FEED_CONSTANT_SDO_SUB_INDEX, N5_FEED_CONSTANT_VALUE)) {
//        return E_SDO_WRITE_FAILURE;
//    }




    return E_SUCCESS;
}

int8_t ec_get_moo_pdo_smc3(const uint16_t drive) {
return CIA_MOO_CSP;
    return ec_pdo_get_input_int8(map_drive_to_slave[drive], SMC3_MOOGET_PDO_INDEX);

}


bool ec_get_remote_smc3(const uint16_t drive) {
    return true;
    uint16_t drive_stat_wrd;
    drive_stat_wrd = ec_get_stat_wrd_smc3(drive);
    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}


bool ec_get_follow_error_smc3(const uint16_t drive) {

    uint16_t drive_stat_wrd;
    drive_stat_wrd = ec_get_stat_wrd_smc3(drive);
    if (BIT_CHECK(drive_stat_wrd, CIA_FOLLOW_ERROR_BIT_NUM)) {
        return true;
    }
    return false;

}

uint8_t *ec_get_error_string_sdo_smc3(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint32_t drive_error_code = 0;
    uint8_t num_errors = 0;

//    if (!ec_sdo_read_uint8(map_drive_to_slave[drive], N5_PREDEFINED_ERROR_NUM_OF_ERRORS_SDO_INDEX, N5_PREDEFINED_ERROR_NUM_OF_ERRORS_SDO_SUB_INDEX, &num_errors)) {
//        sprintf(error_code_string, "Can't read error code from N5 drive, %u", drive);
//        return error_code_string;
//    }
//
//    if (num_errors > 0) {
//        if (!ec_sdo_read_uint32(map_drive_to_slave[drive], N5_PREDEFINED_ERROR_SDO_INDEX, 1, &drive_error_code)) {
//            sprintf(error_code_string, "Can't read error code from N5 drive, %u", drive);
//            return error_code_string;
//        }
//    }
//
//    uint16_t error_code = LAST(drive_error_code, 16);
//    uint8_t error_class = MID(drive_error_code, 16, 24);
//    uint8_t error_number = MID(drive_error_code, 24, 32);
//
//    bool have_error_class = false;
//    strcat(error_code_string,  "N5: Error class: ");
//
//    if (BIT_CHECK(error_class, N5_GENERAL_ERROR_BIT_NUM)) {
//        strcat(error_code_string, "GEN ");
//        have_error_class = true;
//    }
//
//    if (BIT_CHECK(error_class, N5_CURRENT_ERROR_BIT_NUM)) {
//        strcat(error_code_string, "CURRENT ");
//        have_error_class = true;
//    }
//
//    if (BIT_CHECK(error_class, N5_VOLTAGE_ERROR_BIT_NUM)) {
//        strcat(error_code_string, "VOLTAGE ");
//        have_error_class = true;
//    }
//
//
//    if (BIT_CHECK(error_class, N5_TEMPERATURE_ERROR_BIT_NUM)) {
//        strcat(error_code_string, "TEMP ");
//        have_error_class = true;
//    }
//    if (BIT_CHECK(error_class, N5_COMMUNICATION_ERROR_BIT_NUM)) {
//        strcat(error_code_string, "COMMS ");
//        have_error_class = true;
//    }
//    if (BIT_CHECK(error_class, N5_PROFILE_ERROR_BIT_NUM)) {
//        strcat(error_code_string, "PROFILE ");
//        have_error_class = true;
//    }
//    if (BIT_CHECK(error_class, N5_MOTOR_WRONG_DIR_ERROR_BIT_NUM)) {
//        strcat(error_code_string, "WRONG DIR ");
//        have_error_class = true;
//    }
//
//    if (!have_error_class){
//        strcat(error_code_string, "NONE ");
//    }
//    strcat(error_code_string, "Error code: ");
//
//    for (int i = 0; i < N5_NUM_ERROR_CODE; i++) {
//        if (n5_error_code[i].code == error_code) {
//            strcat(error_code_string, n5_error_code[i].description);
//        }else{
//            strcat(error_code_string, "None");
//        }
//    }
//
//    strcat(error_code_string, " Error number: ");
//
//    for (int i = 0; i < N5_NUM_ERROR_NUM; i++) {
//        if (n5_error_number[i].number == error_number) {
//            strcat(error_code_string, n5_error_number[i].description);
//        }else{
//            strcat(error_code_string, "None");
//        }
//    }

    return error_code_string;
}

int32_t ec_get_actpos_wrd_smc3(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], SMC3_ACTPOS_PDO_INDEX);
}

gberror_t ec_set_ctrl_wrd_smc3(const uint16_t drive, const uint16_t ctrlwrd) {
    ec_pdo_set_output_uint16(map_drive_to_slave[drive], SMC3_CONTROLWORD_PDO_INDEX, ctrlwrd);
    return E_SUCCESS;
}

uint16_t ec_get_stat_wrd_smc3(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], SMC3_STATUSWORD_PDO_INDEX);
}

gberror_t ec_set_setpos_wrd_smc3(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], SMC3_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}


gberror_t ec_print_params_smc3(const uint16_t drive) {


    return E_SUCCESS;

}




