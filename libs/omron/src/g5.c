
/**
 ******************************************************************************
 * @file           :  g5.c
 * @brief          :  OMROM G5 drive functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "g5.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


typedef struct {
    uint16_t main_error_num;
    uint16_t sub_error_num;
    char error_string[100];
} g5_error_main_and_sub_t;


typedef struct {
    uint16_t main_error_num;
    char error_string[100];
} g5_error_main_t;




//The lower word of FF00 to FFFF hex gives the main number of the error.

//0x603f
//0000 hex: No error
//FF01 hex: Error main number 1
//FF02 hex: Error main number 2
//: :
//FF99 hex: Error main number 99
//FFA0 hex: Warning A0 hex
//: :
//FFA9 hex: Warning A9 hex
//FFB0 hex: Warning B0 hex
//FFB1 hex: Warning B1 hex
//FFB2 hex: Warning B2 hex
//Others Reserved



g5_error_main_and_sub_t g5_error_table_main_and_sub[100] = {
        {0x11, 0, "Control Power Supply Undervoltage"},
        {0x12, 0, "Overvoltage"},
        {0x13, 0, "Main Power Supply Undervoltage (insufficient voltage between P and N)"},
        {0x13, 1, "Main Power Supply Undervoltage (AC cutoff detected)"},
        {0x14, 0, "Overcurrent"},
        {0x14, 1, "IPM Error"},
        {0x15, 0, "Servo Drive Overheat"},
        {0x16, 0, "Overload"},
        {0x18, 0, "Regeneration Overload"},
        {0x18, 1, "Regeneration Tr Error"},
        {0x21, 0, "Encoder Communications Disconnection Error"},
        {0x21, 1, "Encoder Communications Error"},
        {0x23, 0, "Encoder Communications Data Error"},
        {0x24, 0, "Error Counter Overflow"},
        {0x24, 1, "Excessive Speed Deviation Error"},
        {0x25, 0, "Excessive Hybrid Deviation Error"},
        {0x26, 0, "Overspeed"},
        {0x26, 1, "Overspeed 2"},
        {0x27, 1, "Absolute Value Cleared"},
        {0x27, 4, "Command Error"},
        {0x27, 5, "Command Generation Error"},
        {0x27, 6, "Operation Command Duplicated"},
        {0x27, 7, "Position Data Initialized"},
        {0x29, 1, "Error Counter Overflow 1"},
        {0x29, 2, "Error Counter Overflow 2"},
        {0x30, 0, "Safety Input Error"},
        {0x33, 0, "Interface Input Duplicate Allocation Error 1"},
        {0x33, 1, "Interface Input Duplicate Allocation Error 2"},
        {0x33, 2, "Interface Input Function Number Error 1"},
        {0x33, 3, "Interface Input Function Number Error 2"},
        {0x33, 4, "Interface Output Function Number Error 1"},
        {0x33, 5, "Interface Output Function Number Error 2"},
        {0x33, 8, "External Latch Input Allocation Error"},
        {0x34, 0, "Overrun Limit Error"},
        {0x36, 0, "Oject Error"},
        {0x36, 1, "Object Error"},
        {0x36, 2, "Object Error"},
        {0x37, 0, "Object Corrupted"},
        {0x37, 1, "Object Corrupted"},
        {0x37, 2, "Object Corrupted"},
        {0x38, 0, "Drive Prohibition Input Error 1"},
        {0x38, 1, "Drive Prohibition Input Error 2"},
        {0x40, 0, "Absolute Encoder System Down Error"},
        {0x41, 0, "Absolute Encoder Counter Overflow Error"},
        {0x42, 0, "Absolute Encoder Overspeed Error"},
        {0x43, 0, "Encoder Initialization Error"},
        {0x44, 0, "Absolute Encoder 1-rotation Counter Error"},
        {0x45, 0, "Absolute Encoder Multi-rotation Counter Error"},
        {0x47, 0, "Absolute Encoder Status Error"},
        {0x48, 0, "Encoder Phase-Z Error"},
        {0x49, 0, "Encoder CS Signal Error"},
        {0x50, 0, "External Encoder Connection Error"},
        {0x50, 1, "External Encoder Communications Data Error"},
        {0x51, 0, "External Encoder Status Error 0"},
        {0x51, 1, "External Encoder Status Error 1"},
        {0x51, 2, "External Encoder Status Error 2"},
        {0x51, 3, "External Encoder Status Error 3"},
        {0x51, 4, "External Encoder Status Error 4"},
        {0x51, 5, "External Encoder Status Error 5"},
        {0x55, 0, "Phase-A Connection Error"},
        {0x55, 1, "Phase-B Connection Error"},
        {0x55, 2, "Phase-Z Connection Error"},
        {0x83, 1, "EtherCAT State Change Error"},
        {0x83, 2, "EtherCAT Illegal State Change Error"},
        {0x83, 3, "Communications Synchronization Error"},
        {0x83, 4, "Synchronization Error"},
        {0x83, 5, "Sync Manager WDT Error"},
        {0x87, 0, "Immediate Stop Input Error"},
        {0x88, 0, "Node Address Setting Error"},
        {0x88, 1, "ESC Initialization Error"},
        {0x88, 2, "Interruptions Error"},
        {0x88, 3, "SII Verification Error"},
        {0x90, 0, "Communications Setting Error"},
        {0x91, 1, "Command Error"},
        {0x92, 0, "Encoder Data Restoration Error"},
        {0x92, 1, "External Encoder Data Restoration Error"},
        {0x93, 0, "Object Setting Error 1"},
        {0x93, 2, "Object Setting Error 2"},
        {0x93, 3, "External Encoder Connection Error"},
        {0x93, 4, "Function Setting Error"},
        {0x95, 0, "Motor Non-conformity"},
        {0x95, 1, "Motor Non-conformity"},
        {0x95, 2, "Motor Non-conformity"},
        {0x95, 3, "Motor Non-conformity"},
        {0x95, 4, "Motor Non-conformity"}
};

#define G5_NUM_MAIN_ERRORS 100

g5_error_main_t g5_error_table_main[G5_NUM_MAIN_ERRORS] = {
        {0x11, "Control Power Supply Undervoltage"},
        {0x12, "Overvoltage"},
        {0x13, "Main Power Supply Undervoltage"},
        {0x14, "Overcurrent or IPM Error"},
        {0x15, "Servo Drive Overheat"},
        {0x16, "Overload"},
        {0x18, "Regeneration error"},
        {0x21, "Encoder Communications Error"},
        {0x23, "Encoder Communications Data Error"},
        {0x24, "Error Counter Overflow or Excessive Speed Deviation"},
        {0x25, "Excessive Hybrid Deviation Error"},
        {0x26, "Overspeed"},
        {0x27, "Command Error"},
        {0x29, "Error Counter Overflow"},
        {0x30, "Safety Input Error"},
        {0x33, "Interface Input Error"},
        {0x34, "Overrun Limit Error"},
        {0x36, "Oject Error"},
        {0x37, "Object Corrupted"},
        {0x38, "Drive Prohibition Input Error"},
        {0x40, "Absolute Encoder System Down Error"},
        {0x41, "Absolute Encoder Counter Overflow Error"},
        {0x42, "Absolute Encoder Overspeed Error"},
        {0x43, "Encoder Initialization Error"},
        {0x44, "Absolute Encoder 1-rotation Counter Error"},
        {0x45, "Absolute Encoder Multi-rotation Counter Error"},
        {0x47, "Absolute Encoder Status Error"},
        {0x48, "Encoder Phase-Z Error"},
        {0x49, "Encoder CS Signal Error"},
        {0x50, "External Encoder Error"},
        {0x51, "External Encoder Status Erro"},
        {0x55, "Phase Connection Error"},
        {0x83, "EtherCAT Error"},
        {0x87, "Immediate Stop Input Error"},
        {0x88, "ESC error"},
        {0x90, "Communications Setting Error"},
        {0x91, "Command Error"},
        {0x92, "Encoder Data Restoration Error"},
        {0x93, "Object Setting Error"},
        {0x95, "Motor Non-conformity"},
};


gberror_t ec_pdo_map_g5(const uint16_t slave) {
    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for G5 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to G5 slave [%u]",
                slave);
    }

    //all applied correctly
    return E_SUCCESS;
}


gberror_t ec_standard_sdos_g5(const uint16_t slave) {
    if (!ec_sdo_write_int8(slave, G5_MOOSET_SDO_INDEX, G5_MOOSET_SDO_SUB_INDEX, CIA_MOO_CSP, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_int16(slave, G5_DRIVE_PROHIBITION_INPUT_SDO_INDEX, G5_DRIVE_PROHIBITION_INPUT_SDO_SUB_INDEX,
                            G5_DRIVE_PROHIBITION_INPUT_SDO_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_int32(slave, G5_SOFTWARE_POS_LIMIT_MIN_SDO_INDEX, G5_SOFTWARE_POS_LIMIT_MIN_SDO_SUB_INDEX,
                            map_drive_neg_limit[slave - 1], true)) {

        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_int32(slave, G5_SOFTWARE_POS_LIMIT_MAX_SDO_INDEX, G5_SOFTWARE_POS_LIMIT_MAX_SDO_SUB_INDEX,
                            map_drive_pos_limit[slave - 1], true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_int32(slave, G5_SHAFT_REVOLUTIONS_SDO_INDEX, G5_SHAFT_REVOLUTIONS_SDO_SUB_INDEX,
                            G5_SHAFT_REVOLUTIONS_SDO_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_int32(slave, G5_MOTOR_REVOLUTIONS_SDO_INDEX, G5_MOTOR_REVOLUTIONS_SDO_SUB_INDEX,
                            G5_MOTOR_REVOLUTIONS_SDO_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    return E_SUCCESS;
}

int8_t ec_get_moo_sdo_g5(const uint16_t drive) {
    int8_t moo;
    if (!ec_sdo_read_int8(map_drive_to_slave[drive], G5_MOOGET_SDO_INDEX, G5_MOOGET_SDO_SUB_INDEX, &moo, true)) {
        return 0;
    } else {
        printf("moo = [%d]\n", moo);
        return (CIA_MOO_CSP);
    }

}


bool ec_get_remote_g5(const uint16_t drive) {
    uint16_t drive_stat_wrd;
    drive_stat_wrd = ec_get_stat_wrd_g5(drive);
    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}

#define G5_FOLLOWERROR_TOLERANCE 200000

bool ec_get_follow_error_g5(const uint16_t drive) {
    int32_t follow_error = abs(ec_pdo_get_input_int32(map_drive_to_slave[drive], G5_ACTFOLLOWWERROR_PDO_INDEX));
    if (follow_error > G5_FOLLOWERROR_TOLERANCE) {
        return true;
    }
    return false;
}

uint8_t *ec_get_error_string_pdo_g5(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint16_t drive_error_code = 0;
    drive_error_code = ec_pdo_get_input_uint16(map_drive_to_slave[drive], G5_ERROR_CODE_PDO_INDEX);

    drive_error_code = drive_error_code & 0xFF;

    for (int i = 0; i < G5_NUM_MAIN_ERRORS; i++) {
        if (g5_error_table_main[i].main_error_num == drive_error_code) {
            strncpy((char *) &error_code_string[0], g5_error_table_main[i].error_string, sizeof(error_code_string) - 1);
            break;
        }
    }
    return error_code_string;
}

int32_t ec_get_actpos_wrd_g5(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], G5_ACTPOS_PDO_INDEX);
}

gberror_t ec_set_ctrl_wrd_g5(const uint16_t drive, const uint16_t ctrlwrd) {
    ec_pdo_set_output_uint16(map_drive_to_slave[drive], G5_CONTROLWORD_PDO_INDEX, ctrlwrd);
    return E_SUCCESS;
}

uint16_t ec_get_stat_wrd_g5(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], G5_STATUSWORD_PDO_INDEX);
}

gberror_t ec_set_setpos_wrd_g5(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], G5_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}


gberror_t ec_print_params_g5(const uint16_t drive) {


    return E_SUCCESS;

}

