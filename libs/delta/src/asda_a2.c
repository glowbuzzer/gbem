
/**
 ******************************************************************************
 * @file           :  asda_a2.c
 * @brief          :  delta ASDA-A2
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "asda_a2.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_asda_a2 = {
        .number_of_entries = 1,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_asda_a2 = {
        .number_of_entries = 1,
        .SM_assignment_index = 0x1c13};


/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_asda_a2[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1600,
};

/* This is used for the fixed PDO remapping */
uint16_t map_SM3_index_of_assigned_PDO_asda_a2[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a00,
};


gberror_t ec_pdo_map_asda_a2(const uint16_t slave) {
    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for ASDA_A2 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to ASDA_A2 slave [%u]",
                slave);
    }

    if (!ec_sdo_write_uint16(slave, map_SM2_asda_a2.SM_assignment_index, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_asda_a2.SM_assignment_index, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_asda_a2.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_asda_a2.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_asda_a2[i], true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_asda_a2.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_asda_a2.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_asda_a2[i], true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_asda_a2.SM_assignment_index, 0,
                             map_SM2_asda_a2.number_of_entries, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_asda_a2.SM_assignment_index, 0,
                             map_SM3_asda_a2.number_of_entries, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;


    //all applied correctly
    return E_SUCCESS;
}


gberror_t ec_initial_pdo_asda_a2(const uint16_t slave) {

    ec_pdo_set_output_int8(slave, ASDA_A2_MOOSET_PDO_INDEX, CIA_MOO_CSP);

    return E_SUCCESS;
}

gberror_t ec_standard_sdos_asda_a2(const uint16_t slave) {

    if (!ec_sdo_write_int32(slave, ASDA_A2_INTERPOLATION_TIME_UNITS_SDO_INDEX,
                            ASDA_A2_INTERPOLATION_TIME_UNITS_SDO_SUB_INDEX, MAP_CYCLE_TIME, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    if (!ec_sdo_write_int32(slave, ASDA_A2_MIN_LIMIT_SDO_INDEX,
                            ASDA_A2_MIN_LIMIT_SDO_SUB_INDEX, map_drive_neg_limit[slave], true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_int32(slave, ASDA_A2_MAX_LIMIT_SDO_INDEX,
                            ASDA_A2_MAX_LIMIT_SDO_SUB_INDEX, map_drive_pos_limit[slave], true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_int32(slave, ASDA_A2_DIRECTION_SDO_INDEX,
                            ASDA_A2_DIRECTION_SDO_SUB_INDEX, map_drive_direction[slave], true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint32(slave, ASDA_A2_POSITION_FACTOR_NUMERATOR_SDO_INDEX,
                             ASDA_A2_POSITION_FACTOR_NUMERATOR_SDO_SUB_INDEX,
                             ASDA_A2_POSITION_FACTOR_NUMERATOR_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    if (!ec_sdo_write_uint32(slave, ASDA_A2_DIRECTION_SDO_INDEX,
                             ASDA_A2_DIRECTION_SDO_SUB_INDEX, ASDA_A2_POSITION_FACTOR_NUMERATOR_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }


//    ASDA_A2_POSITION_FACTOR_FEEDCONSTANT_SDO_INDEX



    return E_SUCCESS;
}

int8_t ec_get_moo_pdo_asda_a2(const uint16_t drive) {
//    return CIA_MOO_CSP;
    return ec_pdo_get_input_int8(map_drive_to_slave[drive], ASDA_A2_MOOGET_PDO_INDEX);

}


bool ec_get_remote_asda_a2(const uint16_t drive) {
//    return true;
    uint16_t drive_stat_wrd;
    drive_stat_wrd = ec_get_stat_wrd_asda_a2(drive);
    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}


bool ec_get_follow_error_asda_a2(const uint16_t drive) {

    uint16_t drive_stat_wrd;
    drive_stat_wrd = ec_get_stat_wrd_asda_a2(drive);
    if (BIT_CHECK(drive_stat_wrd, CIA_FOLLOW_ERROR_BIT_NUM)) {
        return true;
    }
    return false;

}

uint8_t *ec_get_error_string_sdo_asda_a2(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint16_t drive_error_code = 0;


    if (!ec_sdo_read_uint16(map_drive_to_slave[drive], ASDA_A2_ERROR_CODE_SDO_INDEX, ASDA_A2_ERROR_CODE_SDO_SUB_INDEX,
                            &drive_error_code, false)) {

        if (drive_error_code == 0) {
            sprintf((char *) error_code_string, "ASDA_A2 no error on drive");
            return error_code_string;
        }

        for (int i = 0; i < NUM_ASDA_A2_ERROR_CODES; i++) {
            if (asda_a2_error_code_description[i].numeric_code_1 == drive_error_code) {
                sprintf((char *) error_code_string, "%s", asda_a2_error_code_description[i].description);
                return error_code_string;
            }
        }

        sprintf((char *) error_code_string,
                "ASDA_A2 error code returned by drive did not match any in the error table");
        return error_code_string;
    }
    sprintf((char *) error_code_string, "ASDA_A2 error code returned by drive did not match any in the error table");

    return error_code_string;
}

int32_t ec_get_actpos_wrd_asda_a2(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], ASDA_A2_ACTPOS_PDO_INDEX);
}

gberror_t ec_set_ctrl_wrd_asda_a2(const uint16_t drive, const uint16_t ctrlwrd) {
    ec_pdo_set_output_uint16(map_drive_to_slave[drive], ASDA_A2_CONTROLWORD_PDO_INDEX, ctrlwrd);
    return E_SUCCESS;
}

uint16_t ec_get_stat_wrd_asda_a2(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], ASDA_A2_STATUSWORD_PDO_INDEX);
}

gberror_t ec_set_setpos_wrd_asda_a2(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], ASDA_A2_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}


gberror_t ec_print_params_asda_a2(const uint16_t drive) {


    return E_SUCCESS;

}


asda_a2_error_code_description_t asda_a2_error_code_description[NUM_ASDA_A2_ERROR_CODES] = {
        {"AL001", "Overcurrent",                                                     0x2310, 0x0001},
        {"AL002", "Overvoltage",                                                     0x3110, 0x0002},
        {"AL003", "Undervoltage",                                                    0x3120, 0x0003},
        {"AL004", "Motor error",                                                     0x7122, 0x0004},
        {"AL005", "Regeneration error",                                              0x3210, 0x0005},
        {"AL006", "Overload",                                                        0x3230, 0x0006},
        {"AL007", "Overspeed",                                                       0x8400, 0x0007},
        {"AL008", "Abnormal pulse control command",                                  0x8600, 0x0008},
        {"AL009", "Excessive deviation",                                             0x8611, 0x0009},
        {"AL010", "Reserved",                                                        0x0000, 0x0010},
        {"AL011", "Encoder error",                                                   0x7305, 0x0011},
        {"AL012", "Adjustment error",                                                0x6320, 0x0012},
        {"AL013", "Emergency stop activated",                                        0x5441, 0x0013},
        {"AL014", "Reverse limit switch error",                                      0x5443, 0x0014},
        {"AL015", "Forward limit switch error",                                      0x5442, 0x0015},
        {"AL016", "IGBT temperature error",                                          0x4210, 0x0016},
        {"AL017", "Memory error",                                                    0x5330, 0x0017},
        {"AL018", "Encoder output error",                                            0x7306, 0x0018},
        {"AL019", "Serial communication error",                                      0x7510, 0x0019},
        {"AL020", "Serial communication time out",                                   0x7520, 0x0020},
        {"AL022", "Input power phase loss",                                          0x3130, 0x0022},
        {"AL023", "Early warning for overload",                                      0x3231, 0x0023},
        {"AL024", "Encoder initial magnetic field error",                            0x7305, 0x0024},
        {"AL025", "Encoder internal error",                                          0x7305, 0x0025},
        {"AL026", "Encoder internal error",                                          0x7305, 0x0026},
        {"AL027", "Encoder data error",                                              0x7305, 0x0027},
        {"AL030", "Motor protection error",                                          0x7121, 0x0030},
        {"AL031", "U,V,W wiring error",                                              0x3300, 0x0031},
        {"AL040", "Full-closed loop excessive deviation",                            0x8610, 0x0040},
        {"AL099", "DSP firmware upgrade",                                            0x5500, 0x0099},
        {"AL201", "CANopen Data Initial Error",                                      0x6310, 0x0201},
        {"AL283", "Forward software limit",                                          0x5444, 0x0283},
        {"AL285", "Reverse software limit",                                          0x5445, 0x0285},
        {"AL185", "EtherCAT connection error(Servo Off)",                            0x8120, 0x0185},
        {"AL180", "Node guarding or Heartbeat error (Servo Off)",                    0x8130, 0x0180},
        {"AL122", "Sub-index error occurs when accessing CANopen PDO object",        0x8200, 0x0122},
        {"AL123", "Data type (size) error occurs when accessing CANopen PDO object", 0x8200, 0x0123},
        {"AL124", "Data range error occurs when accessing CANopen PDO object.",      0x8200, 0x0124},
        {"AL125", "CANopen PDO object is read-only and write-protected",             0x8200, 0x0125},
        {"AL126", "CANopen PDO object does not support PDO",                         0x8200, 0x0126},
        {"AL127", "CANopen PDO object is write-protected when Servo On",             0x8200, 0x0127},
        {"AL128", "Error occurs when reading CANopen PDO object from EEPROM",        0x8200, 0x0128},
        {"AL129", "Error occurs when writing CANopen PDO object into EEPROM",        0x8200, 0x0129},
        {"AL130", "EEPROM invalid address range",                                    0x8200, 0x0130},
        {"AL131", "EEPROM checksum error",                                           0x8200, 0x0131},
        {"AL132", "EEPROM zone error",                                               0x8200, 0x0132},
        {"AL201", "CANopen load/save 1010/1011 error",                               0x6310, 0x0201},
        {"AL3E1", "CANopen SYNC failed (Servo Off)",                                 0x6200, 0x03E1},
        {"AL3E2", "CANopen SYNC signal error (Servo Off)",                           0x6200, 0x03E2},
        {"AL3E3", "CANopen SYNC time out (Servo Off)",                               0x6200, 0x03E3},
        {"AL3E4", "CANopen IP command failed (Servo Off)",                           0x6200, 0x03E4},
        {"AL3E5", "SYNC period error (Servo Off)",                                   0x6200, 0x03E5},
        {"AL500", "Safe torque off (Servo Off)",                                     0x9000, 0x0500},
        {"AL501", "STO_A lost (Servo Off)",                                          0x9000, 0x0501},
        {"AL502", "STO_B lost (Servo Off)",                                          0x9000, 0x0502},
        {"AL503", "STO error (Servo Off)",                                           0x9000, 0x0503},
};