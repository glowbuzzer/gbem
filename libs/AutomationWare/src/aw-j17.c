/**
 ******************************************************************************
 * @file           :  aw-j17.c
 * @brief          :  AutomationWare J17 control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


/*
 * torque limit
 * cycle time - SDO
 * position limits
 * act toque act vel
 *
 */



#include "aw-j17.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


void read_detailled_error(uint16_t drive_number);
//LL_TRACE(GBEM_FUN_TRACE_LOG_EN ,
//         "GBEM: Linked ec_standard_sdo function: [%s] for slave [%u] (this is controlled by the MACHINE #define)",
//         __FUNCTION__ , slave);




///* This is used for the fixed POO remapping */
//map_SM_assignment_object_t map_SM2_aw_j17 = {
//        .number_of_entries = 1,
//        .SM_assignment_index = 0x1c12};
//
///* This is used for the fixed PDO remapping */
//map_SM_assignment_object_t map_SM3_w_j17d = {
//        .number_of_entries = 1,
//        .SM_assignment_index = 0x1c13};
//
//
///* This is used for the fixed PDO remapping */
//uint16_t map_SM2_index_of_assigned_PDO_aw_j17[ECM_MAX_PDO_MAPPING_ENTRIES] = {
//        0x1601,
//};
//
///* This is used for the fixed PDO remapping */
//uint16_t map_SM3_index_of_assigned_PDO_aw_j17[ECM_MAX_PDO_MAPPING_ENTRIES] = {
//        0x1a01,
//};

/**
 * @brief print key default SDOs for AW-J17 drive
 * @param slave
 */

gberror_t ec_print_key_sdos_aw_j17(const uint16_t slave) {

    int32_t max_pos = 0;
    ec_sdo_read_int32(slave, AW_J17_MAX_POSITION_LIMIT_SDO_INDEX, AW_J17_MAX_POSITION_LIMIT_SDO_SUB_INDEX, &max_pos);
    printf("default max pos: %d\n", max_pos);

    int32_t min_pos = 0;
    ec_sdo_read_int32(slave, AW_J17_MIN_POSITION_LIMIT_SDO_INDEX, AW_J17_MIN_POSITION_LIMIT_SDO_SUB_INDEX, &min_pos);
    printf("default min pos: %d\n", min_pos);

    uint16_t max_torque = 0;
    ec_sdo_read_uint16(slave, AW_J17_MAX_TORQUE_SDO_INDEX, AW_J17_MAX_TORQUE_SDO_SUB_INDEX, &max_torque);
    printf("default max torque: %d\n", max_torque);

    uint32_t motor_rated_torque = 0;
    ec_sdo_read_uint32(slave, AW_J17_MOTOR_RATED_TORQUE_SDO_INDEX, AW_J17_MOTOR_RATED_TORQUE_SDO_SUB_INDEX,
                       &motor_rated_torque);
    printf("default motor rated torque: %d\n", motor_rated_torque);

    uint32_t motor_revolutions = 0;
    ec_sdo_read_uint32(slave, AW_J17_MOTOR_REVOLUTIONS_SDO_INDEX, AW_J17_MOTOR_REVOLUTIONS_SDO_SUB_INDEX,
                       &motor_revolutions);
    printf("default motor revolutions: %d\n", motor_revolutions);

    uint32_t shaft_revolutions = 0;
    ec_sdo_read_uint32(slave, AW_J17_SHAFT_REVOLUTIONS_SDO_INDEX, AW_J17_SHAFT_REVOLUTIONS_SDO_SUB_INDEX,
                       &shaft_revolutions);
    printf("default shaft revolutions: %d\n", shaft_revolutions);



//    default max pos: 2147483647
//    default min pos: -2147483648
//    default max torque: 4016
//    default motor rated torque: 620
//    default motor revolutions: 51
//    default shaft revolutions: 1

    return E_SUCCESS;
}


/**
 * @brief perform SDOs writes during boot to configure a slave
 * @param slave
 * @return gberror_t
 * @retval E_SUCCESS all ok
 */
gberror_t ec_apply_standard_sdos_aw_j17(const uint16_t slave) {

//    ec_print_key_sdos_aw_j17(slave);
    //set bus cycle time
    //Communication cycle period	0x1006:0	DINT	32			100		readwrite
    if (!ec_sdo_write_int32(slave, AW_J17_COMMUNICATION_CYCLE_PERIOD_SDO_INDEX,
                            AW_J17_COMMUNICATION_CYCLE_PERIOD_SDO_SUB_INDEX, MAP_CYCLE_TIME * 100)) {
        return E_SDO_WRITE_FAILURE;
    }


    //Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
    //Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)



//    if (!ec_sdo_write_int32(slave, AW_J17_MAX_POSITION_LIMIT_SDO_INDEX, AW_J17_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
//                            2147483647)) {
//        return E_SDO_WRITE_FAILURE;
//    }
//
//    if (!ec_sdo_write_int32(slave, AW_J17_MIN_POSITION_LIMIT_SDO_INDEX, AW_J17_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
//                            -2147483647)) {
//        return E_SDO_WRITE_FAILURE;
//    }



    //todo

//    0x6060:0
    if (!ec_sdo_write_int32(slave, 0x6060, 0, 8)) {
        return E_SDO_WRITE_FAILURE;
    }



//Max torque	0x6072:0	UINT	16	0	32767	3000		readwrite

//Defines the absolute position limits for the 0x607A Target position and the 0x6064 Position actual value. Every new target and actual position are checked against these limits. The limit positions shall be always relative to the machine home position. Limits are disabled by setting them to default values, min. limit to -2147483648 and max. limit to 2147483647. The units are user-defined (increments of the position encoder by default; see 0x6092 Feed constant to change it).

//Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
//Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)

//Polarity	0x607E:0	USINT	8




    return E_SUCCESS;
}


///**
// * @brief sets fixed PDO mapping with a load of SDO writes for AW-J17 drive
// * @return
// * @warning must be called in pre-op states
// *
// * @attention covers all drives attached to a controller
// */
//gberror_t ec_pdo_map_aw_j17(const uint16_t slave) {
//
//    if (ec_printSDO) {
//        UM_INFO(GBEM_UM_EN, "SODs configured for PDO mapping for AW-J17 slave [%u] are:",
//                slave);
//    } else {
//        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to AW-J17 slave [%u]",
//                slave);
//    }
//
//    if (!ec_sdo_write_uint8(slave, map_SM2_aw_j17.SM_assignment_index, 0, 0)) {
//        return E_SDO_WRITE_FAILURE;
//    }
//
//    if (!ec_sdo_write_uint8(slave, map_SM3_aw_j17.SM_assignment_index, 0, 0)) {
//        return E_SDO_WRITE_FAILURE;
//    }
//
//    for (int i = 0; i < map_SM2_aw_j17.number_of_entries; i++) {
//        if (!ec_sdo_write_uint8(slave, map_SM2_aw_j17.SM_assignment_index, i + 1,
//                                map_SM2_index_of_assigned_PDO_aw_j17[i])) {
//            return E_SDO_WRITE_FAILURE;
//        }
//    }
//
//    for (int i = 0; i < map_SM3_aw_j17.number_of_entries; i++) {
//        if (!ec_sdo_write_uint8(slave, map_SM3_aw_j17.SM_assignment_index, i + 1,
//                                map_SM3_index_of_assigned_PDO_aw_j17[i])) {
//            return E_SDO_WRITE_FAILURE;
//        }
//    }
//
//    /*
//     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
//     */
//    if (!ec_sdo_write_uint16(slave, map_SM2_aw_j17.SM_assignment_index, 0,
//                             map_SM2_aw_j17.number_of_entries)) {
//        return E_SDO_WRITE_FAILURE;
//    }
//
//    if (!ec_sdo_write_uint16(slave, map_SM3_aw_j17.SM_assignment_index, 0,
//                             map_SM3_aw_j17.number_of_entries)) {
//        return E_SDO_WRITE_FAILURE;
//    }
//
//    //all applied correctly
//    return E_SUCCESS;
//
//
//}


/**
 * @brief get the value of the remote bit from an AW-J17 drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_aw_j17(const uint16_t drive) {

    return true;
}

/**
 * @brief gets the mode of operation (MOO) for AW-J17 drives
 * @param drive_number
 * @return  moo value (8 for CSP)
 * @attention handles sub-drives
 */
int8_t ec_get_moo_pdo_aw_j17(const uint16_t drive) {
//    LL_TRACE(GBEM_FUN_TRACE_LOG_EN,
//            "GBEM: Linked ec_get_modes_of_operation function: %s (this is controlled by the MACHINE #define)",
//            __FUNCTION__);

//printf("moo: %d\n",ec_pdo_get_input_int8(4, AW_J17_MOODISP_PDO_INDEX) );
//    return ec_pdo_get_input_int8(map_drive_to_slave[drive], AW_J17_MOODISP_PDO_INDEX);
////todo
    return ec_pdo_get_input_int8(4, AW_J17_MOODISP_PDO_INDEX);


}


bool ec_get_follow_error_aw_j17(const uint16_t drive) {
    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_aw_j17(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_FOLLOW_ERROR_BIT_NUM)) {
        return true;
    }
    return false;
}


/**
 * @brief Perform first cycle PDO writes for AW-J17 drives
 * @param slave - slave number to send PDO writes
 * @retval E_SUCCESS - Normal
 * @attention
 */
gberror_t ec_initial_pdo_aw_j17(const uint16_t slave) {

//    ec_pdo_set_output_int8(slave, AW_J17_MOOSET_PDO_INDEX, CIA_MOO_CSP);

//    LL_INFO(GBEM_GEN_LOG_EN,
//            "GBEM: Applying initial PDO writes for AW-J17 drive slave [%u], offset [%u], value [%u]", slave, AW_J17_MOOSET_PDO_INDEX, CIA_MOO_CSP);
//
//    if (ec_iserror()) {
//        LL_ERROR(GBEM_GEN_LOG_EN,
//                 "GBEM: EtherCAT error detected after initial PDO writes: %s", ec_elist2string());
//        return E_ETHERCAT_ERROR_DETECTED;
//    }

    return E_SUCCESS;
}



///**
// * @brief Writes an SDO that triggers a config write to NVRAM for an AZD drive
// * @param drive_number
// * @return gberror
// * @attention Needed after a write of reverses or directions
// * @warning Writes the config for all sub-drives
// * @warning This will wear out the NVRAM if left enabled every boot!
// */
//gberror_t ec_write_nvram_azd_ked(const uint16_t slave) {
//
//    //this writes stuff to drive NVRAM - DO NOT LEAVE ENABLED - maybe
//
//    if (!ec_sdo_write_int8(slave, AZD_KED_WRITE_CONFIG_SDO_INDEX, AZD_KED_WRITE_CONFIG_SDO_SUB_INDEX,
//                           AZD_KED_WRITE_CONFIG_SDO_VALUE)) {
//        return E_SDO_WRITE_FAILURE;
//    }
//
//    return E_SUCCESS;
//}
//

/**
 * @brief get actpos for an AW-J17 drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actpos_wrd_aw_j17(const uint16_t drive) {

    return ec_pdo_get_input_int32(map_drive_to_slave[drive], AW_J17_ACTPOS_PDO_INDEX);

}

/**
 * @brief get ctrlwrd for for an AW-J17 drive
 * @param drive
 * @return ctrlwrd
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
uint16_t ec_get_ctrl_wrd_rev_aw_j17(const uint16_t drive) {
    return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AW_J17_CONTROLWORD_PDO_INDEX);

}

/**
 * @brief set status word for an AW-J17 drive
 * @param drive
 * @param statwrd
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_stat_wrd_rev_aw_j17(const uint16_t drive, const uint16_t statwrd) {
    ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AW_J17_STATUSWORD_PDO_INDEX, statwrd);
    return E_SUCCESS;
}

/**
 * @brief set control word for AW-J17 drive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_aw_j17(const uint16_t drive, const uint16_t ctrlwrd) {

    ec_pdo_set_output_uint16(map_drive_to_slave[drive], AW_J17_CONTROLWORD_PDO_INDEX, ctrlwrd);

    return E_SUCCESS;
}

/**
 * @brief set status word for AW_J17 drive
 * @param drive
 * @return status word

 */
uint16_t ec_get_stat_wrd_aw_j17(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AW_J17_STATUSWORD_PDO_INDEX);

}

/**
 * @brief set setpos for an AW-J17 drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setpos_wrd_aw_j17(const uint16_t drive, const int32_t setpos) {

    ec_pdo_set_output_int32(map_drive_to_slave[drive], AW_J17_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}

/**
 * @brief set actpos for an AW-J17 drive
 * @param drive
 * @param actpos
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_actpos_wrd_rev_aw_j17(const uint16_t drive, const int32_t actpos) {
    ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AW_J17_ACTPOS_PDO_INDEX, actpos);
    return E_SUCCESS;
}

/**
 * @brief get setpos for an AW-J17 drive
 * @param drive
 * @return setpos
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
int32_t ec_get_setpos_word_rev_aw_j17(const uint16_t drive) {
    return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AW_J17_SETPOS_PDO_INDEX);

}


/**
 * @brief set moo for an AW-J17 drive
 * @param drive
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_moo_pdo_rev_aw_j17(const uint16_t drive) {
    ec_pdo_set_input_int8_rev(map_drive_to_slave[drive], AW_J17_MOODISP_PDO_INDEX, CIA_MOO_CSP);
    return E_SUCCESS;
}

///**
// * @brief set NVRAM SDOs for an AZD drive
// * @param slave
// * @return gberror
// * NVRAM sdos are things like limits and drive rotation direction that must be saved to NVRAM and the drive restarted before they take effect
// *
// */
//gberror_t ec_nvram_sdos_azd_ked(const uint16_t slave) {
//
//    if (ec_printSDO) {
//        UM_INFO(GBEM_UM_EN, "GBEM: NVRAM SDOs configured for AZD slave [%u] are:", slave);
//    } else {
//        UM_INFO(GBEM_UM_EN, "GBEM: Applying NVRAM SDOs to AZD slave [%u]", slave);
//    }
//
//    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
//        if (map_drive_to_slave[i] == slave) {
//
//            if (!ec_sdo_write_int32(slave, AZD_KED_MIN_LIMIT_SDO_INDEX,
//                                    AZD_KED_MIN_LIMIT_SDO_SUB_INDEX, map_drive_neg_limit[i])) {
//                return E_SDO_WRITE_FAILURE;
//            }
//
//            if (!ec_sdo_write_int32(slave, AZD_KED_MAX_LIMIT_SDO_INDEX,
//                                    AZD_KED_MAX_LIMIT_SDO_SUB_INDEX, map_drive_pos_limit[i])) {
//                return E_SDO_WRITE_FAILURE;
//            }
//            if (!ec_sdo_write_int32(slave, AZD_KED_DIRECTION_SDO_INDEX,
//                                    AZD_KED_DIRECTION_SDO_SUB_INDEX, map_drive_direction[i])) {
//                return E_SDO_WRITE_FAILURE;
//            }
//
//        }
//    }
//    gberror_t rc = ec_write_nvram_azd_ked(slave);
//    if (rc != E_SUCCESS) {
//
//        return E_NVRAM_WRITE_FAILURE;
//    } else {
//        return E_SUCCESS;
//    }
//}

uint8_t *ec_get_error_string_sdo_aw_j17(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint16_t drive_error_code = 0;

    if (!ec_sdo_read_uint16(map_drive_to_slave[drive], AW_J17_ERROR_CODE_SDO_INDEX, AW_J17_ERROR_CODE_SDO_SUB_INDEX,
                            &drive_error_code)) {

        if (drive_error_code == 0) {
            sprintf(error_code_string, "AW-J17 no error on drive");
            return error_code_string;
        }

        for (int i = 0; i < NUM_OF_AW_J17_ERROR_STRINGS; i++) {
            if (aw_j17_alarm_code[i].error_id == drive_error_code) {
                sprintf(error_code_string, "%s", aw_j17_alarm_code[i].text_string);
                return error_code_string;
            }
        }

        sprintf(error_code_string, "AW-J17 error code returned by drive did not match any in the error table");
        return error_code_string;
    }
    sprintf(error_code_string, "AW-J17 error code returned by drive did not match any in the error table");

    //todo //Description	0x203F:1	STRING(8)	64			0		readonly (default)

    printf("AW-J17 drive error [%s] [%d]\n", error_code_string, drive_error_code);

//    read_detailled_error(drive);
    return error_code_string;
}

void read_detailled_error(const uint16_t drive_number) {
    int os = 8;
    uint8_t octet_string[8];

    for (int i = 0; i < 8; i++) {
        int rc = ec_SDOread(map_drive_to_slave[drive_number], AW_J17_ERROR_DESCRIPTION_SDO_INDEX,
                            i + AW_J17_ERROR_DESCRIPTION_SDO_SUB_INDEX, false, &os,
                            &octet_string[i], EC_TIMEOUTRXM);
        if (rc <= 0) {
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Could not read SDO index:0x%04x - sub-index:0x%04x (on slave:%u",
                     AW_J17_ERROR_DESCRIPTION_SDO_INDEX, i + AW_J17_ERROR_DESCRIPTION_SDO_SUB_INDEX,
                     map_drive_to_slave[drive_number]);
        }
            printf("%s,", octet_string[i]);

    }
    printf("\n");
}
/*array mapping an AW-J17 drive error code to a text string */
    const aw_j17_error_string_t aw_j17_alarm_code[NUM_OF_AW_J17_ERROR_STRINGS] = {
            {0x2220, "Continuous over current (device internal)"},
            {0x2250, "Short circuit (device internal)"},
            {0x2350, "Load level fault (I2t, thermal state)"},
            {0x2351, "Load level warning (I2t, thermal state)"},
            {0x3130, "Phase failure"},
            {0x3131, "Phase failure L1"},
            {0x3132, "Phase failure L2"},
            {0x3133, "Phase failure L3"},
            {0x3210, "DC link over-voltage"},
            {0x3220, "DC link under-voltage"},
            {0x3331, "Field circuit interrupted"},
            {0x4210, "Excess temperature device"},
            {0x4310, "Excess temperature drive"},
            {0x5200, "Control"},
            {0x5300, "Operating unit"},
            {0x6010, "Software reset (watchdog)"},
            {0x6320, "Parameter error"},
            {0x7121, "Motor blocked"},
            {0x7300, "Sensor"},
            {0x7303, "Resolver 1 fault"},
            {0x7304, "Resolver 2 fault"},
            {0x7500, "Communication"},
            {0x8611, "Positioning controller (following error)"},
            {0x8612, "Positioning controller (reference limit)"},
            {0xF002, "Sub-synchronous run"},
            {0xFF00, "Manufacturer-specific"}
    };

//20992



/*
 * Internal limit active (bit 11) indicates if there is an element that doesn't allow the motion to follow the target value.
 *
 * Several limits exist in the firmware.
 *
 * Max. torque (6072h) limits Torque demand (6074h),
 * Max. motor speed (6080h) limits Velocity demand value (606Bh) and Profile velocity (6081h) in Profile position (PP) mode,
 * Software position limit (607Dh) limits Position demand value (6062h) and position or velocity controller integral limit limits integral part of the PID controller.
 * Additional limit, Position range limit (607Bh), exists for Position demand value (6062h) in Profile position (PP) mode since demand value can't wrap around the range limit, when profiler is being used.
 */