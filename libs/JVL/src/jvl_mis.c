
/**
 ******************************************************************************
 * @file           :  jvl_mis.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "jvl_mis.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"

//RxPDO 0x1c12 SM2
//TxPDO 0x1c13 SM3

map_PDO_configuration_object_t map_rxpdo_object_jvl_mis = {
        .number_of_entries = 3,
        .PDO_assignment_index = 0x1600
};

map_PDO_configuration_object_t map_txpdo_object_jvl_mis = {
        .number_of_entries = 7,
        .PDO_assignment_index = 0x1a00
};

uint32_t map_rxpdo_contents_jvl_mis[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x60400010,
        0x60600008,
        0x607A0020};

uint32_t map_txpdo_contents_jvl_mis[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x60410010,
        0x60610008,
        0x60640020,
        0x606c0020,
        0x60770010,
        0x60f40020,
        0x603f0010,
};

//0x6091:1
#define JVL_MIS_GEAR_RATIO_MOTOR_REV 100
//0x6092:1
#define JVL_MIS_FEED_CONSTANT_FEED  409600
//0x6092:2
#define JVL_MIS_FEED_CONSTANT_SHAFT_REV 1


gberror_t ec_pdo_map_jvl_mis(const uint16_t slave) {
    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for JVL MIS slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to JVL MIS slave [%u]",
                slave);
    }

    /* clear TxPdo 0x1a00 */
    if (!ec_sdo_write_uint8(slave, map_txpdo_object_jvl_mis.PDO_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* write contents of TxPdo */
    for (int i = 0; i < map_txpdo_object_jvl_mis.number_of_entries; i++) {
        if (!ec_sdo_write_uint32(slave, map_txpdo_object_jvl_mis.PDO_assignment_index, i + 1,
                                 map_txpdo_contents_jvl_mis[i])) {

            return E_SDO_WRITE_FAILURE;
        }
    }

    /* write number of entries in TxPdo 0x1a00 */
    if (!ec_sdo_write_uint8(slave, map_txpdo_object_jvl_mis.PDO_assignment_index, 0,
                            map_txpdo_object_jvl_mis.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* clear RxPdo 0x1600 */
    if (!ec_sdo_write_uint8(slave, map_rxpdo_object_jvl_mis.PDO_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* write contents of RxPdo */
    for (int i = 0; i < map_rxpdo_object_jvl_mis.number_of_entries; i++) {
        if (!ec_sdo_write_uint32(slave, map_rxpdo_object_jvl_mis.PDO_assignment_index, i + 1,
                                 map_rxpdo_contents_jvl_mis[i])) {

            return E_SDO_WRITE_FAILURE;
        }
    }

    /* write number of entries in RxPdo 0x1600 */
    if (!ec_sdo_write_uint8(slave, map_rxpdo_object_jvl_mis.PDO_assignment_index, 0,
                            map_rxpdo_object_jvl_mis.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

//all applied correctly
    return E_SUCCESS;
}


gberror_t ec_standard_sdos_jvl_mis(const uint16_t slave) {


    //INT32_MAX is multiturn encoder limit

    if (!ec_sdo_write_int32(slave, JVL_MIS_POS_POSLIMIT_SDO_INDEX, JVL_MIS_POS_POSLIMIT_SDO_SUB_INDEX,
                            map_drive_pos_limit[slave - 1])) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_int32(slave, JVL_MIS_NEG_POSLIMIT_SDO_INDEX, JVL_MIS_NEG_POSLIMIT_SDO_SUB_INDEX,
                            map_drive_neg_limit[slave - 1])) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, JVL_MIS_DIRECTION_SDO_INDEX,
                            JVL_MIS_DIRECTION_SDO_SUB_INDEX, map_drive_direction[slave - 1])) {
        return E_SDO_WRITE_FAILURE;
    }

//    VIP: The position factor is automatically calculated when the feed constant (Object 0x6092) and gear ratio (Object 0x6091) are set.

//0x6091:1
    if (!ec_sdo_write_uint32(slave, JVL_MIS_GEAR_MOTOR_REV_SDO_INDEX,
                             JVL_MIS_GEAR_MOTOR_REV_SDO_SUB_INDEX, JVL_MIS_GEAR_RATIO_MOTOR_REV)) {
        return E_SDO_WRITE_FAILURE;
    }

//0x6091:2
//    if (!ec_sdo_write_uint32(slave, JVL_MIS_GEAR_SHAFT_REV_SDO_INDEX,
//                             JVL_MIS_GEAR_SHAFT_REV_SDO_SUB_INDEX,1 )) {
//        return E_SDO_WRITE_FAILURE;
//    }

//feed 0x6092:1 defaults to 409600 & encoder increments (0x608f sub 1 defaults to 409,600)
//0x6092:1
    if (!ec_sdo_write_uint32(slave, JVL_MIS_FEED_FEED_SDO_INDEX,
                             JVL_MIS_FEED_FEED_SDO_SUB_INDEX, JVL_MIS_FEED_CONSTANT_FEED)) {
        return E_SDO_WRITE_FAILURE;
    }

//0x6092:2
    if (!ec_sdo_write_uint32(slave, JVL_MIS_FEED_SHAFT_REV_SDO_INDEX,
                             JVL_MIS_FEED_SHAFT_REV_SDO_SUB_INDEX, JVL_MIS_FEED_CONSTANT_SHAFT_REV)) {
        return E_SDO_WRITE_FAILURE;
    }



//quick stop option code

/* bit - function - supported
 0 - Disable drive function                                                 - Yes
 1 - Slow down on slow ramp and transit into Switch On Disabled             - Yes
 2 - Slow down on quick ramp and transit into Switch On Disabled            - Yes << DEFAULT
 3 - Slow down on current limit and transit into Switch On Disabled         - No/Same as 2
 4 - Slow down on voltage limit and transit into Switch On Disabled         - No/Same as 2
 5 - Slow down on slow ramp and stay in quick stop active                   - Yes
 6 - Slow down on quick stop ramp and stay in quick stop active             - Yes
 7 - Slow down on current limit and stay in quick stop active               - No/Same as 6
 8 - Slow down on voltage limit and stay in quick stop active               - No/Same as 6
*/

    int16_t quickstop_option_code = 0;
    BIT_SET(quickstop_option_code, 2);

    if (!ec_sdo_write_int16(slave, JVL_MIS_QUICKSTOP_OPTION_CODE_SDO_INDEX,
                            JVL_MIS_QUICKSTOP_OPTION_CODE_SDO_SUB_INDEX, quickstop_option_code)) {
        return E_SDO_WRITE_FAILURE;
    }


    return E_SUCCESS;
}


gberror_t ec_initial_pdo_jvl_mis(const uint16_t slave) {
    ec_pdo_set_output_int8(slave, JVL_MIS_MOOSET_PDO_INDEX, CIA_MOO_CSP);
    return E_SUCCESS;
}

int8_t ec_get_moo_pdo_jvl_mis(const uint16_t drive) {
    return ec_pdo_get_input_int8(map_drive_to_slave[drive], JVL_MIS_MOODISP_PDO_INDEX);
}

bool ec_get_remote_jvl_mis(const uint16_t drive) {
    uint16_t drive_stat_wrd;
    drive_stat_wrd = ec_get_stat_wrd_jvl_mis(drive);
    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}


#define JVL_MIS_FOLLOWERROR_TOLERANCE 200000

bool ec_get_follow_error_jvl_mis(const uint16_t drive) {
    int32_t follow_error = abs(ec_pdo_get_input_int32(map_drive_to_slave[drive], JVL_MIS_ACTFOLLOWWERROR_PDO_INDEX));

    if (follow_error > JVL_MIS_FOLLOWERROR_TOLERANCE) {
        return true;
    }
    return false;

}

#define NUM_OF_JVL_MIS_ERROR_STRINGS 17

const jvl_mis_error_string_t jvl_mis_alarm_code[NUM_OF_JVL_MIS_ERROR_STRINGS] = {
        {0,     "No error"},
        {16912, "Temperature too high"},
        {21376, "Initialization error"},
        {30081, "Initialization error"},
        {34321, "Follow erro"},
        {34432, "SW position limit exceeded"},
        {12832, "Undervoltage on bus<"},
        {29446, "Multiturn encoder lost position"},
        {29447, "Multiturn encoder reed error"},
        {29448, "Multiturn encoder communication error"},
        {30080, "SSI Encoder error"},
        {33921, "SSI Encoder error"},
        {21889, "Memory error"},
        {29445, "Singleturn encoder error"},
        {21377, "STO alarm error"},
        {34433, "Negative limit exceeded<"},
        {34434, "Positive limit exceeded"},
};

uint8_t *ec_get_error_string_pdo_jvl_mis(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint16_t drive_error_code = 0;
    drive_error_code = ec_pdo_get_input_uint16(map_drive_to_slave[drive], JVL_MIS_ERROR_CODE_PDO_INDEX);

    for (int i = 0; i < NUM_OF_JVL_MIS_ERROR_STRINGS; i++) {
        if (jvl_mis_alarm_code[i].error_id == drive_error_code) {
            memset(&error_code_string[0], 0, sizeof(error_code_string));
            strncpy((char *) &error_code_string[0], jvl_mis_alarm_code[i].text_string, sizeof(error_code_string) - 1);
            break;
        }
    }
    return error_code_string;
}

int32_t ec_get_actpos_wrd_jvl_mis(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], JVL_MIS_ACTPOS_PDO_INDEX);
}

gberror_t ec_set_ctrl_wrd_jvl_mis(const uint16_t drive, const uint16_t ctrlwrd) {
    ec_pdo_set_output_uint16(map_drive_to_slave[drive], JVL_MIS_CONTROLWORD_PDO_INDEX, ctrlwrd);
    return E_SUCCESS;
}

uint16_t ec_get_stat_wrd_jvl_mis(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], JVL_MIS_STATUSWORD_PDO_INDEX);
}

gberror_t ec_set_setpos_wrd_jvl_mis(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], JVL_MIS_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}


gberror_t ec_print_params_jvl_mis(const uint16_t drive) {

    //Encoder increments - this should not be changed
    uint32_t encoder_increments = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], 0x608f, 1, &encoder_increments);
    //Motor revolutions
    uint32_t motor_revolutions = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], 0x608f, 2, &motor_revolutions);

    //Gear motor: motor revolutions
    uint32_t gear_motor_motor_rev = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], 0x6091, 1, &gear_motor_motor_rev);

    //Feed constant: feed
    uint32_t feed_constat_feed = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], 0x6092, 1, &feed_constat_feed);

    //Feed constant: shaft revolutions
    uint32_t feed_constant_shaft_revolutions = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], 0x6092, 2, &feed_constant_shaft_revolutions);


    UM_INFO(GBEM_UM_EN, "GBEM: JVL scaling information for slave [%u]", map_drive_to_slave[drive]);
    UM_INFO(GBEM_UM_EN, "GBEM: Encoder increments [%u]", encoder_increments);
    UM_INFO(GBEM_UM_EN, "GBEM: Motor revolutions [%u]", motor_revolutions);
    UM_INFO(GBEM_UM_EN, "GBEM: Gear motor: motor revolutions [%u]", gear_motor_motor_rev);
    UM_INFO(GBEM_UM_EN, "GBEM: Feed constant; feed [%u]", feed_constat_feed);
    UM_INFO(GBEM_UM_EN, "GBEM: Feed constant: shaft revolutions [%u]", feed_constant_shaft_revolutions);

    float position_factor =
            ((float) gear_motor_motor_rev * (float) feed_constant_shaft_revolutions * (float) encoder_increments) /
            ((float) feed_constat_feed * (float) feed_constant_shaft_revolutions * (float) motor_revolutions);

    UM_INFO(GBEM_UM_EN, "GBEM: Position factor [%f]", position_factor);

    return E_SUCCESS;

}