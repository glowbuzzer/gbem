
/**
 ******************************************************************************
 * @file           :  n5.c
 * @brief          :  nanotec N5 drive functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "n5.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


#define N5_NUM_ERROR_NUM 29

n5_error_number_t n5_error_number[N5_NUM_ERROR_NUM] = {
        {0,  "Watchdog-Reset"},
        {1,  "Input voltage too high"},
        {2,  "Output current too high"},
        {3,  "Input voltage too low"},
        {4,  "Error at fieldbus"},
        {5,  "Motor turns – in spite of active block – in the wrong direction"},
        {6,  "CANopen only: NMT master takes too long to send nodeguarding request"},
        {7,  "Encoder error due to electrical fault or defective hardware"},
        {8,  "Encoder error; index not found during the auto setup"},
        {9,  "Error in the AB track"},
        {10, "Positive limit switch and tolerance zone exceeded"},
        {11, "Negative limit switch and tolerance zone exceeded"},
        {12, "Device temperature above 80°C"},
        {
         13,
             "The values of object 6065h (Following Error Window) and object 6066h (Following Error Time Out) were exceeded; a fault was triggered"
        },
        {14, "Nonvolatile memory full; controller must be restarted for cleanup work"},
        {15, "Motor blocked"},
        {16, "Nonvolatile memory damaged; controller must be restarted for cleanup work"},
        {17, "CANopen only: Slave took too long to send PDO messages"},
        {18, "Hall sensor faulty"},
        {19, "CANopen only: PDO not processed due to a length error"},
        {20, "CANopen only: PDO length exceeded"},
        {21, "Nonvolatile memory full; controller must be restarted for cleanup work"},
        {22, "Rated current must be set (203Bh:01h)"},
        {23, "Encoder resolution, number of pole pairs and some other values are incorrect"},
        {24, "Motor current is too high, adjust the PI parameters"},
        {25, "Internal software error, generic"},
        {26, "Current too high at digital output"},
        {27, "CANopen only: Unexpected sync length"},
        {
         28,
             "EtherCAT only: The motor was stopped because EtherCAT switched state from OP to either SafeOP or PreOP without first stopping the motor"
        },
};

#define N5_NUM_ERROR_CODE 18

n5_error_code_t n5_error_code[N5_NUM_ERROR_CODE] = {
        {0x1000, "General error"},
        {0x2300, "Current at the controller output too large"},
        {0x3100, "Overvoltage/undervoltage at controller input"},
        {0x4200, "Temperature error within the controller"},
        {0x6010, "Software reset (watchdog)"},
        {0x6100, "Internal software error, generic"},
        {0x6320, "Rated current must be set (203Bh:01h)"},
        {0x7121, "Motor blocked"},
        {0x7305, "Incremental encoder or Hall sensor faulty"},
        {0x7600, "Nonvolatile memory full or corrupt; restart the controller for cleanup work"},
        {0x8000, "Error during fieldbus monitoring"},
        {0x8130, "CANopen only: Life Guard error or Heartbeat error"},
        {0x8200, "CANopen only: Slave took too long to send PDO messages"},
        {0x8210, "CANopen only: PDO was not processed due to a length error"},
        {0x8220, "CANopen only: PDO length exceeded"},
        {0x8611, "Position monitoring error: Following error too large"},
        {0x8612, "Position monitoring error: Limit switch and tolerance zone exceeded"},
        {0x9000, "EtherCAT: Motor running while EtherCAT changes from OP -> SafeOp, PreOP, etc."},
};


gberror_t ec_pdo_map_n5(const uint16_t slave) {
    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for N5 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to N5 slave [%u]",
                slave);
    }

    //all applied correctly
    return E_SUCCESS;
}


gberror_t ec_initial_pdo_n5(const uint16_t slave) {
    ec_pdo_set_output_int8(slave, N5_MOOSET_PDO_INDEX, CIA_MOO_CSP);

    return E_SUCCESS;
}

gberror_t ec_standard_sdos_n5(const uint16_t slave) {
    //todo
    // if (!ec_sdo_write_int32(slave, N5_MIN_POS_LIMIT_SDO_INDEX, N5_MIN_POS_LIMIT_SDO_SUB_INDEX,
    //                         map_drive_neg_limit[slave - 1], true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    //
    //
    // if (!ec_sdo_write_int32(slave, N5_MAX_POS_LIMIT_SDO_INDEX, N5_MAX_POS_LIMIT_SDO_SUB_INDEX,
    //                         map_drive_pos_limit[slave - 1], true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }

//    if (!ec_sdo_write_uint8(slave, N5_DIRECTION_OF_ROTATION_SDO_INDEX, N5_DIRECTION_OF_ROTATION_SDO_SUB_INDEX,
//                            map_drive_direction[slave - 1], true)) {
//        return E_SDO_WRITE_FAILURE;
//    }
    if (!ec_sdo_write_int32(slave, N5_GEAR_MOTOR_REVOLUTIONS_SDO_INDEX, N5_GEAR_MOTOR_REVOLUTIONS_SDO_SUB_INDEX,
                            N5_GEAR_MOTOR_REVOLUTIONS_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_int32(slave, N5_GEAR_SHAFT_REVOLUTIONS_SDO_INDEX, N5_GEAR_SHAFT_REVOLUTIONS_SDO_SUB_INDEX,
                            N5_GEAR_SHAFT_REVOLUTIONS_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_int32(slave, N5_FEED_CONSTANT_SDO_INDEX, N5_FEED_CONSTANT_SDO_SUB_INDEX,
                            N5_FEED_CONSTANT_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_int32(slave, N5_FEED_SHAFT_REVOLUTIONS_SDO_INDEX, N5_FEED_SHAFT_REVOLUTIONS_SDO_SUB_INDEX,
                            N5_FEED_SHAFT_REVOLUTIONS_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, N5_INTERPOLATION_TIME_PERIOD_SDO_INDEX, N5_INTERPOLATION_TIME_PERIOD_SDO_SUB_INDEX,
                            4, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    return E_SUCCESS;
}

int8_t ec_get_moo_pdo_n5(const uint16_t drive) {
    return ec_pdo_get_input_int8(map_drive_to_slave[drive], N5_MOOGET_PDO_INDEX);
}


bool ec_get_remote_n5(const uint16_t drive) {
    uint16_t drive_stat_wrd;
    drive_stat_wrd = ec_get_stat_wrd_n5(drive);
    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}


bool ec_get_follow_error_n5(const uint16_t drive) {
    uint16_t drive_stat_wrd;
    drive_stat_wrd = ec_get_stat_wrd_n5(drive);
    if (BIT_CHECK(drive_stat_wrd, CIA_FOLLOW_ERROR_BIT_NUM)) {
        return true;
    }
    return false;
}

uint8_t *ec_get_error_string_sdo_n5(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint32_t drive_error_code = 0;
    uint8_t num_errors = 0;

    if (!ec_sdo_read_uint8(map_drive_to_slave[drive], N5_PREDEFINED_ERROR_NUM_OF_ERRORS_SDO_INDEX,
                           N5_PREDEFINED_ERROR_NUM_OF_ERRORS_SDO_SUB_INDEX, &num_errors, false)) {
        sprintf((char *) error_code_string, "Can't read error code from N5 drive, %u", drive);
        return error_code_string;
    }

    if (num_errors > 0) {
        if (!ec_sdo_read_uint32(map_drive_to_slave[drive], N5_PREDEFINED_ERROR_SDO_INDEX, 1, &drive_error_code,
                                false)) {
            sprintf((char *) error_code_string, "Can't read error code from N5 drive, %u", drive);
            return error_code_string;
        }
    }

    uint16_t error_code = LAST(drive_error_code, 16);
    uint8_t error_class = MID(drive_error_code, 16, 24);
    uint8_t error_number = MID(drive_error_code, 24, 32);

    bool have_error_class = false;
    strcat((char *) error_code_string, "N5: Error class: ");

    if (BIT_CHECK(error_class, N5_GENERAL_ERROR_BIT_NUM)) {
        strcat((char *) error_code_string, "GEN ");
        have_error_class = true;
    }

    if (BIT_CHECK(error_class, N5_CURRENT_ERROR_BIT_NUM)) {
        strcat((char *) error_code_string, "CURRENT ");
        have_error_class = true;
    }

    if (BIT_CHECK(error_class, N5_VOLTAGE_ERROR_BIT_NUM)) {
        strcat((char *) error_code_string, "VOLTAGE ");
        have_error_class = true;
    }


    if (BIT_CHECK(error_class, N5_TEMPERATURE_ERROR_BIT_NUM)) {
        strcat((char *) error_code_string, "TEMP ");
        have_error_class = true;
    }
    if (BIT_CHECK(error_class, N5_COMMUNICATION_ERROR_BIT_NUM)) {
        strcat((char *) error_code_string, "COMMS ");
        have_error_class = true;
    }
    if (BIT_CHECK(error_class, N5_PROFILE_ERROR_BIT_NUM)) {
        strcat((char *) error_code_string, "PROFILE ");
        have_error_class = true;
    }
    if (BIT_CHECK(error_class, N5_MOTOR_WRONG_DIR_ERROR_BIT_NUM)) {
        strcat((char *) error_code_string, "WRONG DIR ");
        have_error_class = true;
    }

    if (!have_error_class) {
        strcat((char *) error_code_string, "NONE ");
    }
    strcat((char *) error_code_string, "Error code: ");

    for (int i = 0; i < N5_NUM_ERROR_CODE; i++) {
        if (n5_error_code[i].code == error_code) {
            strcat((char *) error_code_string, n5_error_code[i].description);
        } else {
            strcat((char *) error_code_string, "None");
        }
    }

    strcat((char *) error_code_string, " Error number: ");

    for (int i = 0; i < N5_NUM_ERROR_NUM; i++) {
        if (n5_error_number[i].number == error_number) {
            strcat((char *) error_code_string, n5_error_number[i].description);
        } else {
            strcat((char *) error_code_string, "None");
        }
    }

    return error_code_string;
}

int32_t ec_get_actpos_wrd_n5(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], N5_ACTPOS_PDO_INDEX);
}

gberror_t ec_set_ctrl_wrd_n5(const uint16_t drive, const uint16_t ctrlwrd) {
    ec_pdo_set_output_uint16(map_drive_to_slave[drive], N5_CONTROLWORD_PDO_INDEX, ctrlwrd);
    return E_SUCCESS;
}

uint16_t ec_get_stat_wrd_n5(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], N5_STATUSWORD_PDO_INDEX);
}

gberror_t ec_set_setpos_wrd_n5(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], N5_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}


gberror_t ec_print_params_n5(const uint16_t drive) {
    return E_SUCCESS;
}




