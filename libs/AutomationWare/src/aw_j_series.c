/**
 ******************************************************************************
 * @file           :  aw_j_series.c
 * @brief          :  common functions for all AutomationWare j-series drives
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "aw_j_series.h"
#include "std_headers.h"
#include "map_config_custom_pdo.h"
#include "std_defs_and_macros.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"
#include <math.h>

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_aw_series_fsoe = {
    .number_of_entries = 4,
    .SM_assignment_index = 0x1c12
};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_aw_series_fsoe = {
    .number_of_entries = 4,
    .SM_assignment_index = 0x1c13
};

/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_aw_series_fsoe[ECM_MAX_PDO_MAPPING_ENTRIES] = {
    0x1600,
    0x1601,
    0x1602,
    0x1700,
    //        0x1701
};

/* This is used for the fixed PDO remapping */
uint16_t map_SM3_index_of_assigned_PDO_aw_series_fsoe[ECM_MAX_PDO_MAPPING_ENTRIES] = {
    0x1a00,
    0x1a01,
    0x1a03,
    0x1b00,
    //        0x1b01
};


map_custom_pdo_t aw_j_series_custom_pdo_map = {
    .sm2_assignment_object = 0x1C12,
    .num_sm2_assignments = 3,
    .rxpdo = {
        {
            .pdo_assignment_object = 0x1600,
            .num_assignments = 7,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = 0x6040}, //Controlword
                {.datatype = ECT_INTEGER8, .index = 0x6060}, //Modes of operation
                {.datatype = ECT_INTEGER16, .index = 0x6071}, //Target Torque
                {.datatype = ECT_INTEGER32, .index = 0x607A}, //Target position
                {.datatype = ECT_INTEGER32, .index = 0x60FF}, //Target velocity
                {.datatype = ECT_INTEGER16, .index = 0x60B2}, //Torque offset
                {.datatype = ECT_UNSIGNED32, .index = 0x2701}, //Tuning command
            }
        },
        {
            .pdo_assignment_object = 0x1601,
            .num_assignments = 2,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = 0x60FE, .offset = 0x1}, //Physical outputs
                {.datatype = ECT_UNSIGNED32, .index = 0x60FE, .offset = 0x2}, //Bit mask
            }
        },
        {
            .pdo_assignment_object = 0x1602,
            .num_assignments = 3,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = 0x2703}, //User MOSI
                {.datatype = ECT_INTEGER32, .index = 0x60B1}, //Velocity offset
                {.datatype = ECT_UNSIGNED32, .index = 0x2215, .offset = 0x1}, //LED colour
            }
        }
    },
    .sm3_assignment_object = 0x1C13,
    .num_sm3_assignments = 4,
    .txpdo = {
        {
            .pdo_assignment_object = 0x1A00,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = 0x6041}, //Statusword
                {.datatype = ECT_INTEGER8, .index = 0x6061}, //Modes of operation display
                {.datatype = ECT_INTEGER32, .index = 0x6064}, //Position actual value
                {.datatype = ECT_INTEGER32, .index = 0x606C}, //Velocity actual value
                {.datatype = ECT_INTEGER16, .index = 0x6077}, //Torque actual value
            }
        },
        {
            .pdo_assignment_object = 0x1A01,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = 0x2401}, //Analog input 1
                {.datatype = ECT_UNSIGNED16, .index = 0x2402}, // Analog input 2
                {.datatype = ECT_UNSIGNED16, .index = 0x2403}, //Analog input 3
                {.datatype = ECT_UNSIGNED16, .index = 0x2404}, //Analog input 4
                {.datatype = ECT_UNSIGNED32, .index = 0x2702}, //Tuning status
            }
        },
        {
            .pdo_assignment_object = 0x1A02,
            .num_assignments = 1,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = 0x60FD}, //Digital inputs
            }
        },
        {
            .pdo_assignment_object = 0x1A03,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = 0x2704}, //User MISO
                {.datatype = ECT_UNSIGNED32, .index = 0x20F0}, //Timestamp
                {.datatype = ECT_INTEGER32, .index = 0x60FC}, //Position demand internal value
                {.datatype = ECT_INTEGER32, .index = 0x606B}, //Velocity demand value
                {.datatype = ECT_INTEGER16, .index = 0x6074} //Torque demand
            }
        }
    }
};


map_custom_pdo_t aw_j_series_fsoe_custom_pdo_map = {
    .sm2_assignment_object = 0x1C12,
    .num_sm2_assignments = 3,
    .rxpdo = {
        {
            .pdo_assignment_object = 0x1600,
            .num_assignments = 7,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = 0x6040}, //Controlword
                {.datatype = ECT_INTEGER8, .index = 0x6060}, //Modes of operation
                {.datatype = ECT_INTEGER16, .index = 0x6071}, //Target Torque
                {.datatype = ECT_INTEGER32, .index = 0x607A}, //Target position
                {.datatype = ECT_INTEGER32, .index = 0x60FF}, //Target velocity
                {.datatype = ECT_INTEGER16, .index = 0x60B2}, //Torque offset
                {.datatype = ECT_UNSIGNED32, .index = 0x2701}, //Tuning command
            }
        },
        {
            .pdo_assignment_object = 0x1601,
            .num_assignments = 2,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = 0x60FE, .offset = 0x1}, //Physical outputs
                {.datatype = ECT_UNSIGNED32, .index = 0x60FE, .offset = 0x2}, //Bit mask
            }
        },
        {
            .pdo_assignment_object = 0x1602,
            .num_assignments = 3,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = 0x2703}, //User MOSI
                {.datatype = ECT_INTEGER32, .index = 0x60B1}, //Velocity offset
                {.datatype = ECT_UNSIGNED32, .index = 0x2215, .offset = 0x1}, //LED colour
            }
        },
        //                {.pdo_assignment_object=0x1700,
        //                        .num_assignments=1,
        //                        .assignments= {
        //                                {.datatype = ECT_UNSIGNED8, .index = 0x6770, .offset=0x1}, //FSoE Command
        //                                {.datatype = ECT_BOOLEAN, .index = 0x6640, .offset=0x0} // STO
        //
        //                        }
        //                }
    },
    .sm3_assignment_object = 0x1C13,
    .num_sm3_assignments = 4,
    .txpdo = {
        {
            .pdo_assignment_object = 0x1A00,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = 0x6041}, //Statusword
                {.datatype = ECT_INTEGER8, .index = 0x6061}, //Modes of operation display
                {.datatype = ECT_INTEGER32, .index = 0x6064}, //Position actual value
                {.datatype = ECT_INTEGER32, .index = 0x606C}, //Velocity actual value
                {.datatype = ECT_INTEGER16, .index = 0x6077}, //Torque actual value
            }
        },
        {
            .pdo_assignment_object = 0x1A01,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = 0x2401}, //Analog input 1
                {.datatype = ECT_UNSIGNED16, .index = 0x2402}, // Analog input 2
                {.datatype = ECT_UNSIGNED16, .index = 0x2403}, //Analog input 3
                {.datatype = ECT_UNSIGNED16, .index = 0x2404}, //Analog input 4
                {.datatype = ECT_UNSIGNED32, .index = 0x2702}, //Tuning status
            }
        },
        {
            .pdo_assignment_object = 0x1A02,
            .num_assignments = 1,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = 0x60FD}, //Digital inputs
            }
        },
        {
            .pdo_assignment_object = 0x1A03,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = 0x2704}, //User MISO
                {.datatype = ECT_UNSIGNED32, .index = 0x20F0}, //Timestamp
                {.datatype = ECT_INTEGER32, .index = 0x60FC}, //Position demand internal value
                {.datatype = ECT_INTEGER32, .index = 0x606B}, //Velocity demand value
                {.datatype = ECT_INTEGER16, .index = 0x6074} //Torque demand
            }
        }
    }
};

gberror_t ec_print_pdo_config_aw_series(const uint16_t slave) {
    uint8_t sm2_pdo_no = 0;
    uint8_t sm3_pdo_no = 0;

    UM_INFO(GBEM_UM_EN, "GBEM: PDO configuration for AW-J-Series drive slave [%u]", slave);


    if (!ec_sdo_read_uint8(slave, 0x1c12, 0, &sm2_pdo_no, false)) {
        return E_SDO_READ_FAILURE;
    }
    if (!ec_sdo_read_uint8(slave, 0x1c13, 0, &sm3_pdo_no, false)) {
        return E_SDO_READ_FAILURE;
    }

    UM_INFO(GBEM_UM_EN, "GBEM: SM2 PDOs [%u] SM3 PDOs [%u]", sm2_pdo_no, sm3_pdo_no);

    for (uint8_t i = 1; i <= sm2_pdo_no; i++) {
        uint32_t pdo_index = 0;
        if (!ec_sdo_read_uint32(slave, 0x1c12, i, &pdo_index, false)) {
            return E_SDO_READ_FAILURE;
        }
        UM_INFO(GBEM_UM_EN, "GBEM: SM2 PDO [%u] Index [0x%x]", i, pdo_index);
    }

    for (uint8_t i = 1; i <= sm3_pdo_no; i++) {
        uint32_t pdo_index = 0;
        if (!ec_sdo_read_uint32(slave, 0x1c13, i, &pdo_index, false)) {
            return E_SDO_READ_FAILURE;
        }
        UM_INFO(GBEM_UM_EN, "GBEM: SM3 PDO [%u] Index [0x%x]", i, pdo_index);
    }


    return E_SUCCESS;
}


bool ec_get_estop_state_aw_j_series(void) {

    bool estop = true;

    #if MAP_NUMBER_ESTOP_DIN == 0
    UM_FATAL("GBEM: MAP_NUMBER_ESTOP_DIN not defined");
#endif
#if MAP_NUMBER_ESTOP_DIN > 2
    UM_FATAL("GBEM: too many MAP_NUMBER_ESTOP_DIN are defined!");
#endif

#if MAP_NUMBER_ESTOP_DIN == 1

    if (ctrl_estop_din_1.slave_num < 1) {
        UM_FATAL("GBEM: no ctrl_estop_din is defined! (slave number less thhan 1)");
    }

    if (!ec_pdo_get_input_bit(ctrl_estop_din_1.slave_num, ctrl_estop_din_1.bit_num)) {
        estop = true;
    } else {
        estop = false;
    }
#endif

#if MAP_NUMBER_ESTOP_DIN == 2

    if (ctrl_estop_din_1.slave_num < 1) {
        UM_FATAL("GBEM: no ctrl_estop_din_1 is defined! (slave number less thhan 1)");
    }
    if (ctrl_estop_din_2.slave_num < 1) {
        UM_FATAL("GBEM: no ctrl_estop_din_2 is defined! (slave number less thhan 1)");
    }

    if (!ec_pdo_get_input_bit(ctrl_estop_din_1.slave_num, ctrl_estop_din_1.bit_num) ||
        !ec_pdo_get_input_bit(ctrl_estop_din_2.slave_num, ctrl_estop_din_2.bit_num)) {
        estop = true;
        } else {
            estop = false;
        }
#endif

    return estop;

}

gberror_t ec_custom_fmmu_sm_aw_j_series(const uint16_t slave) {
    ec_slave[slave].configindex = 998;

    ec_slave[slave].Ibits = 600;
    ec_slave[slave].Obits = 376;

    //todo crit
    // ec_slave[slave].FMMU[0].LogLength = 82;
    // ec_slave[slave].FMMU[1].LogLength = 54;


    ec_slave[slave].SM[2].SMlength = 47;
    ec_slave[slave].SM[3].SMlength = 75;

    return E_SUCCESS;
}


gberror_t ec_pdo_map_aw_j_series(const uint16_t slave) {
    ec_print_pdo_config_aw_series(slave);


    if (AW_FSOE == 1) {
        if (!ec_sdo_write_uint16(slave, map_SM2_aw_series_fsoe.SM_assignment_index, 0, 0, true)) {
            return E_SDO_WRITE_FAILURE;
        }

        if (!ec_sdo_write_uint16(slave, map_SM3_aw_series_fsoe.SM_assignment_index, 0, 0, true)) {
            return E_SDO_WRITE_FAILURE;
        }

        for (int i = 0; i < map_SM2_aw_series_fsoe.number_of_entries; i++) {
            if (!ec_sdo_write_uint16(slave, map_SM2_aw_series_fsoe.SM_assignment_index, i + 1,
                                     map_SM2_index_of_assigned_PDO_aw_series_fsoe[i], true)) {
                return E_SDO_WRITE_FAILURE;
            }
        }

        for (int i = 0; i < map_SM3_aw_series_fsoe.number_of_entries; i++) {
            if (!ec_sdo_write_uint16(slave, map_SM3_aw_series_fsoe.SM_assignment_index, i + 1,
                                     map_SM3_index_of_assigned_PDO_aw_series_fsoe[i], true)) {
                return E_SDO_WRITE_FAILURE;
            }
        }

        /*
         * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
         */
        if (!ec_sdo_write_uint16(slave, map_SM2_aw_series_fsoe.SM_assignment_index, 0,
                                 map_SM2_aw_series_fsoe.number_of_entries, true)) {
            return E_SDO_WRITE_FAILURE;
        }

        if (!ec_sdo_write_uint16(slave, map_SM3_aw_series_fsoe.SM_assignment_index, 0,
                                 map_SM3_aw_series_fsoe.number_of_entries, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    } else {
        return map_apply_custom_pdo_mapping(slave, aw_j_series_custom_pdo_map);
    }


    //    return map_apply_custom_pdo_mapping(slave, aw_j_series_custom_pdo_map);
}


gberror_t ec_apply_standard_sdos_aw_j_series(const uint16_t slave) {
#if AW_FSOE == 1
    ec_set_slots_aw_j_series_fsoe(slave);
#endif

#if AW_FSOE == 0
    //set bus cycle time
    //Communication cycle period	0x1006:0	DINT	32			100		readwrite
    if (!ec_sdo_write_int32(slave, AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_INDEX,
                            AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_SUB_INDEX, MAP_CYCLE_TIME * 100, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    switch (MAP_CYCLE_TIME) {
        case 1:
            if (!ec_sdo_write_uint8(slave, AW_J_SERIES_FIR_ORDER_SDO_INDEX, AW_J_SERIES_FIR_ORDER_SDO_SUB_INDEX, 3,
                                    true)) {
                return E_SDO_WRITE_FAILURE;
            }
            break;
        case 2:
            if (!ec_sdo_write_uint8(slave, AW_J_SERIES_FIR_ORDER_SDO_INDEX, AW_J_SERIES_FIR_ORDER_SDO_SUB_INDEX, 7,
                                    true)) {
                return E_SDO_WRITE_FAILURE;
            }
            break;
        case 4:
            if (!ec_sdo_write_uint8(slave, AW_J_SERIES_FIR_ORDER_SDO_INDEX, AW_J_SERIES_FIR_ORDER_SDO_SUB_INDEX, 15,
                                    true)) {
                return E_SDO_WRITE_FAILURE;
            }
        default:
            break;
    }
#endif

    //nolimits is a global variable set by running gbem with a command line option - x and is used when the drive is beyond the normal limits

    //Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
    //Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)

    //encoder is 2^20  1048576 counts per rev

#if MACHINE_AW_ROBOT_L2 == 1
    if (nolimits) {
        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                                2147483647, true)) {
            return E_SDO_WRITE_FAILURE;
        }

        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                -2147483647, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    } else {

        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                                (int32_t) (
                                        ((double) map_drive_scales[map_slave_to_drive(slave)].position_scale)
                                        *
                                        ((double) map_drive_pos_limit[map_slave_to_drive(slave)] * (M_PI /
                                                                                                    180.0))),
                                true)) {
            return E_SDO_WRITE_FAILURE;
        }
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Max position limit [%d] on drive [%d]",
                (int32_t) (((double) map_drive_scales[map_slave_to_drive(slave)].position_scale)
                           *
                           ((double) map_drive_pos_limit[map_slave_to_drive(slave)] * (M_PI /
                                                                                       180.0))),
                map_slave_to_drive(slave));

        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                (int32_t) (
                                        ((double) map_drive_scales[map_slave_to_drive(slave)].position_scale)
                                        *
                                        ((double) map_drive_neg_limit[map_slave_to_drive(slave)] * (M_PI /
                                                                                                    180.0))),
                                true)) {
            return E_SDO_WRITE_FAILURE;
        }
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Min position limit [%d] on drive [%d]",
                (int32_t) (((double) map_drive_scales[map_slave_to_drive(slave)].position_scale)
                           *
                           ((double) map_drive_neg_limit[map_slave_to_drive(slave)] * (M_PI /
                                                                                       180.0))),
                map_slave_to_drive(slave));
    }


    //Polarity	0x607E:0	USINT	8

    uint8_t polarity = 0;
    if (map_drive_direction[map_slave_to_drive(slave)] == 0) {
        polarity = 128;
    }
    //todo crit velocity polarity

    if (!ec_sdo_write_int32(slave, AW_J_SERIES_POLARITY_SDO_INDEX, AW_J_SERIES_POLARITY_SDO_SUB_INDEX,
                            polarity, true)) {
        return E_SDO_WRITE_FAILURE;
    }

#endif

    //Configure LED gpio output pin
    if (!ec_sdo_write_uint8(slave, AW_J_SERIES_LED_OP_PIN_CONFIG_SDO_INDEX, AW_J_SERIES_LED_OP_PIN_CONFIG_SDO_SUB_INDEX,
                            AW_J_SERIES_LED_OP_PIN_CONFIG_SDO_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    //Configure LED gpio output pin voltage
    if (!ec_sdo_write_uint8(slave, AW_J_SERIES_LED_OP_PIN_VOLTAGE_SDO_INDEX,
                            AW_J_SERIES_LED_OP_PIN_VOLTAGE_SDO_SUB_INDEX,
                            AW_J_SERIES_LED_OP_PIN_VOLTAGE_SDO_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    return E_SUCCESS;
}

/**
 * @brief get the value of the remote bit from an AW-J-series drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_aw_j_series(const uint16_t drive) {
    //remote bit not implemented on AW-J-series drives
    return true;
}

/**
 * @brief gets the mode of operation (MOO) for AW-J-series drives
 * @param drive_number
 * @return  moo value (8 for CSP)
 * @attention handles sub-drives
 */
int8_t ec_get_moo_pdo_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_input_int8(map_drive_to_slave[drive], AW_J_SERIES_MOODISP_PDO_INDEX);
}


/**
 * @brief gets the status of the follow error bit for AW-J-series drives
 * @param drive
 * @return
 */
bool ec_get_follow_error_aw_j_series(const uint16_t drive) {
    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_aw_j_series(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_FOLLOW_ERROR_BIT_NUM)) {
        return true;
    }
    return false;
}

/**
 * @brief Perform PDO writes for AW-J-series drives of the MOO
 * @param slave - drive number to send PDO writes
 * @retval E_SUCCESS - Normal
 * @attention
 */

gberror_t ec_set_moo_pdo_aw_j_series(const uint16_t drive, int8_t moo) {
    ec_pdo_set_output_int8(map_drive_to_slave[drive], AW_J_SERIES_MOOSET_PDO_INDEX, moo);

    return E_SUCCESS;
}

/**
 * @brief Perform first cycle PDO writes for AW-J-series drives
 * @param slave - slave number to send PDO writes
 * @retval E_SUCCESS - Normal
 * @attention
 */
gberror_t ec_initial_pdo_aw_j_series(const uint16_t slave) {
    //    ec_pdo_set_output_int8(slave, AW_J_SERIES_MOOSET_PDO_INDEX, map_drive_moo[map_slave_to_drive(slave)]);
    //
    //    UM_INFO(GBEM_UM_EN,
    //            "GBEM: Setting MOO with PDO write for AW-J-Series drive slave [%u], drive, [%u], offset [%u], value [%u]",
    //            slave, map_slave_to_drive(slave),
    //            AW_J_SERIES_MOOSET_PDO_INDEX, map_drive_moo[map_slave_to_drive(slave)]);

    return E_SUCCESS;
}


/**
 * @brief get actpos for an AW-J-Series drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actpos_wrd_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], AW_J_SERIES_ACTPOS_PDO_INDEX);
}

/**
 * @brief get actvel for an AW-J-Series drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actvel_wrd_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], AW_J_SERIES_ACTVEL_PDO_INDEX);
}

/**
 * @brief get acttorq for an AW-J-Series drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_acttorq_wrd_aw_j_series(const uint16_t drive) {
    int16_t acttorq = ec_pdo_get_input_int16(map_drive_to_slave[drive], AW_J_SERIES_ACTTORQ_PDO_INDEX);
    //    int32_t acttorq = ec_pdo_get_input_int16(map_drive_to_slave[drive], AW_J_SERIES_TORQ_DEMAND_PDO_INDEX);


    //    if (drive == 1) {
    //        printf("acttorq: %d\n", acttorq);
    //    }
    return acttorq;
}


/**
 * @brief get ctrlwrd for for an AW-J-series drive
 * @param drive
 * @return ctrlwrd
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
uint16_t ec_get_ctrl_wrd_rev_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AW_J_SERIES_CONTROLWORD_PDO_INDEX);
}


/**
 * @brief set control word for AW-J-series drive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_aw_j_series(const uint16_t drive, const uint16_t ctrlwrd) {
    ec_pdo_set_output_uint16(map_drive_to_slave[drive], AW_J_SERIES_CONTROLWORD_PDO_INDEX, ctrlwrd);

    return E_SUCCESS;
}

/**
 * @brief set status word for AW-J-series drive
 * @param drive
 * @return status word

 */
uint16_t ec_get_stat_wrd_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AW_J_SERIES_STATUSWORD_PDO_INDEX);
}

/**
 * @brief set setpos for an AW-J-series drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setpos_wrd_aw_j_series(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], AW_J_SERIES_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}

/**
 * @brief set setvel for an AW-J-series drive
 * @param drive
 * @param setvel
 * @return gberror
 */
gberror_t ec_set_setvel_wrd_aw_j_series(const uint16_t drive, const int32_t setvel) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], AW_J_SERIES_SETVEL_PDO_INDEX, setvel);
    return E_SUCCESS;
}

/**
 * @brief set setveloffset for an AW-J-series drive
 * @param drive
 * @param setveloffset
 * @return gberror
 */
gberror_t ec_set_setveloffset_wrd_aw_j_series(const uint16_t drive, const int32_t setveloffset) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], AW_J_SERIES_SETVEL_OFFSET_PDO_INDEX, setveloffset);
    return E_SUCCESS;
}

/**
 * @brief set settorq for an AW-J-series drive
 * @param drive
 * @param settorq
 * @return gberror
 */
gberror_t ec_set_settorq_wrd_aw_j_series(const uint16_t drive, const int32_t settorq) {
    ec_pdo_set_output_int16(map_drive_to_slave[drive], AW_J_SERIES_SETTORQ_PDO_INDEX, (int16_t) settorq);

    //    printf("settorq [%d]\n", settorq);

    return E_SUCCESS;
}

/**
 * @brief set settorqoffset for an AW-J-series drive
 * @param drive
 * @param settorqoffset
 * @return gberror
 */

gberror_t ec_set_settorqoffset_wrd_aw_j_series(const uint16_t drive, const int32_t settorqoffset) {
    ec_pdo_set_output_int16(map_drive_to_slave[drive], AW_J_SERIES_SETORQ_OFFSET_PDO_INDEX, settorqoffset);
    return E_SUCCESS;
}


uint8_t *ec_get_error_string_sdo_aw_j_series(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint16_t drive_error_code = 0;

    char no_error_prefix[] = "AW-J-Series: no error on drive. Detailed error report";

    if (ec_sdo_read_uint16(map_drive_to_slave[drive], AW_J_SERIES_ERROR_CODE_SDO_INDEX,
                           AW_J_SERIES_ERROR_CODE_SDO_SUB_INDEX,
                           &drive_error_code, false)) {
        if (drive_error_code == 0) {
            snprintf((char *) error_code_string, MAX_DRIVE_ERROR_MSG_LENGTH,
                     "%s [%s]", no_error_prefix,
                     ec_get_detailled_error_report_sdo_aw_j_series(drive));
            return error_code_string;
        }

        char error_code_first[] = "AW-J-Series error code";
        char error_code_second[] = "Detailed error report";

        for (int i = 0; i < NUM_OF_AW_J_SERIES_ERROR_STRINGS; i++) {
            if (aw_j_series_error[i].error_id == drive_error_code) {
                snprintf((char *) error_code_string, MAX_DRIVE_ERROR_MSG_LENGTH,
                         "%s [%s] %s [%s]", error_code_first, aw_j_series_error[i].text_string, error_code_second,
                         ec_get_detailled_error_report_sdo_aw_j_series(drive));

                error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH - 1] = '\0';
                return error_code_string;
            }
        }

        snprintf((char *) error_code_string, MAX_DRIVE_ERROR_MSG_LENGTH,
                 "AW-J-Series: error code returned by drive did not match any in the error table. Detailed error report [%s]",
                 ec_get_detailled_error_report_sdo_aw_j_series(drive));
        return error_code_string;
    }

    sprintf((char *) error_code_string,
            "AW-J-Series error code could not be read from drive");
    return error_code_string;
}

uint8_t *ec_get_detailled_error_report_sdo_aw_j_series(const uint16_t drive_number) {
    int os = 8;
    uint8_t octet_string[8];

    static uint8_t failed_error_code_read[50] = "AW-J-Series: Failed to read error code";
    static uint8_t failed_error_code_lookup[50] = "AW-J-Series: Error code does not match table";

    int rc = ec_SDOread(map_drive_to_slave[drive_number], AW_J_SERIES_ERROR_DESCRIPTION_SDO_INDEX,
                        AW_J_SERIES_ERROR_DESCRIPTION_SDO_SUB_INDEX, false, &os,
                        &octet_string, EC_TIMEOUTRXM);
    if (rc <= 0) {
        return failed_error_code_read;
    }

    for (int i = 0; i < NUM_OF_AW_J_SERIES_ERROR_REPORT_STRINGS; i++) {
        if (strcmp(aw_j_series_error_report[i].error_code, (char *) octet_string) != 0) {
            return (uint8_t *) aw_j_series_error_report[i].text_string;
        }
    }

    return failed_error_code_lookup;
}

/**
* @brief set status word for an AW-J-series drive
* @param drive
* @param statwrd
* @return gberror
* @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
*/
gberror_t ec_set_stat_wrd_rev_aw_j_series(const uint16_t drive, const uint16_t statwrd) {
    ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AW_J_SERIES_STATUSWORD_PDO_INDEX, statwrd);
    return E_SUCCESS;
}


/**
 * @brief set actpos for an AW-J-Series drive
 * @param drive
 * @param actpos
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_actpos_wrd_rev_aw_j_series(const uint16_t drive, const int32_t actpos) {
    ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AW_J_SERIES_ACTPOS_PDO_INDEX, actpos);
    return E_SUCCESS;
}

/**
 * @brief get setpos for an AW-J-Series drive
 * @param drive
 * @return setpos
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
int32_t ec_get_setpos_word_rev_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AW_J_SERIES_SETPOS_PDO_INDEX);
}


/**
 * @brief set moo for an AW-J-Series drive
 * @param drive
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_moo_pdo_rev_aw_j_series(const uint16_t drive) {
    ec_pdo_set_input_int8_rev(map_drive_to_slave[drive], AW_J_SERIES_MOODISP_PDO_INDEX, CIA_MOO_CSP);
    return E_SUCCESS;
}


gberror_t ec_print_slots_aw_j_series_fsoe(const uint16_t slave) {
    uint8_t no_slots = 0;

    if (!ec_sdo_read_uint8(slave, AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                           0,
                           &no_slots, true)) {
        return E_SDO_READ_FAILURE;
    }

    UM_INFO(GBEM_UM_EN, "GBEM: AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX [0x%02x] [%u]",
            AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX, no_slots);

    //print slot entries
    for (uint8_t i = 1; i <= no_slots; i++) {
        uint32_t module_id = 0;
        if (!ec_sdo_read_uint32(slave, AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                                i,
                                &module_id, true)) {
            return E_SDO_READ_FAILURE;
        }
        UM_INFO(GBEM_UM_EN, "GBEM: AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX [0x%02x] [%u] = 0x%08x",
                AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX, i, module_id);
    }


    return E_SUCCESS;
}

gberror_t ec_set_slots_aw_j_series_fsoe(const uint16_t slave) {
    UM_INFO(GBEM_UM_EN, "GBEM: Setting slots for AW-J-Series FSoE - slave [%u]", slave);

    ec_print_slots_aw_j_series_fsoe(slave);

    //clear slot entries
    if (!ec_sdo_write_int8(slave, AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                           0,
                           0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    //    bool ec_sdo_write_uint32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint32_t Value, bool umError) {

    if (!ec_sdo_write_uint32(slave, AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             1, AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint32(slave, AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             2, AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_2_ID, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_int8(slave, AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                           0,
                           2, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    return E_SUCCESS;
}

const aw_j_series_error_report_string_t aw_j_series_error_report[NUM_OF_AW_J_SERIES_ERROR_REPORT_STRINGS] = {
    {"NoFault", "No fault"},
    {"PuOcA", "Protection User-defined Over-current Phase A"},
    {"PuOcB", "Protection User-defined Over-current Phase B"},
    {"PuOcC", "Protection User-defined Over-current Phase C"},
    {"PuUv", "Protection User-defined Under-voltage"},
    {"PuOv", "Protection User-defined Over-voltage"},
    {"PhUdef", "Protection Hardware Undefined"},
    {"PhFault", "Protection Hardware Fault"},
    {"PhWtdg", "Protection Hardware Watchdog"},
    {"PhDeadTA", "Protection Hardware Deadtime Phase A"},
    {"PhDeadTB", "Protection Hardware Deadtime Phase B"},
    {"PhDeadTC", "Protection Hardware Deadtime Phase C"},
    {"PhDeadTD", "Protection Hardware Deadtime Phase D"},
    {"PhOvUvOt", "Protection Hardware Over-voltage / Under-voltage / Over-temperature"},
    {"PhOc", "Protection Hardware Overcurrent"},
    {"SfeDiIvd", "Safety Digital Input Invalid"},
    {"SfeFault", "Safety Fault"},
    {"BisErBit", "BiSS Error Bit active"},
    {"BisWnBit", "BiSS Warning Bit active"},
    {"BisAcBit", "BiSS Acknowledge Bit not received"},
    {"BisSrtBt", "BiSS Start Bit not received"},
    {"BisSloLo", "BiSS data (SLO) is permanently Low"},
    {"BisFrame", "BiSS Frame configuration error"},
    {"BisCrc", "BiSS CRC error"},
    {"BisRegEr", "BiSS register communication error"},
    {"SsiFrame", "SSI Frame configuration error"},
    {"SsiDtaLo", "SSI Data is permanently Low"},
    {"SsiChksm", "SSI Checksum error"},
    {"R16WkMgF", "REM16 Weak Magnetic Field"},
    {"R16Cont", "REM16 multiturn Counter error"},
    {"R16CorDc", "REM16 singleturn CORDIC error"},
    {"R16SpdOv", "REM16 multiturn Speed Overflow"},
    {"R16FtCfg", "REM16 Filter Configuration error"},
    {"R16FtSOF", "REM16 Filter Speed Overflow error"},
    {"R16UnCmd", "REM16 Unknown Command"},
    {"R16Chksm", "REM16 Checksum error"},
    {"R14Chksm", "REM14 Checksum error"},
    {"QeiLsTck", "QEI Lost Ticks"},
    {"QeiNoIdx", "QEI No Index"},
    {"QeiOpnWr", "QEI Open Wire"},
    {"HallSeq", "Hall Sequence error"},
    {"AftFrame", "A-Format Frame error"},
    {"AftTmout", "A-Format Timeout"},
    {"AftCrc", "A-Format CRC error"},
    {"AftBatt", "A-Format BATT status"},
    {"AftMTErr", "A-Format MTERR status"},
    {"AftOvFlw", "A-Format OvFlow status"},
    {"AftOvSpd", "A-Format OVSPD status"},
    {"AftMemEr", "A-Format MEMERR status"},
    {"AftSTErr", "A-Format STERR status"},
    {"AftPSErr", "A-Format PSERR status"},
    {"AftBusyF", "A-Format BUSY status"},
    {"AftMemBs", "A-Format MemBusy status"},
    {"AftOvTmp", "A-Format OvTemp status"},
    {"AftIncEr", "A-Format IncErr status"},
    {"SnsrAngl", "Sensor Angle error"},
    {"SnsrCfg", "Sensor Configuration error"},
    {"HwRsrcEr", "Hardware Resource Error"},
    {"IvldGpio", "Invalid Gpio configuration"},
    {"SnsrBatt", "Sensor battery error"},
    {"SkpCycls", "Serial encoder service is skipping cycles"},
    {"SwLimOut", "Software Limit Out"},
    {"BrkNtRls", "Brake release failed"},
    {"BkHiPull", "Pull brake voltage too high"},
    {"BkHiHold", "Hold brake voltage too high"},
    {"OpnTermA", "Open Terminal A"},
    {"OpnTermB", "Open Terminal B"},
    {"OpnTermC", "Open Terminal C"},
    {"OpnFetAH", "Open high FET at phase A"},
    {"OpnFetBH", "Open high FET at phase B"},
    {"OpnFetCH", "Open high FET at phase C"},
    {"OpnFetAL", "Open low FET at phase A"},
    {"OpnFetBL", "Open low FET at phase B"},
    {"OpnFetCL", "Open low FET at phase C"},
    {"IvldOfst", "Invalid offset"},
    {"IvldOpmd", "Invalid operational mode"},
    {"ZeroMxI", "Max. current is zero or negative"},
    {"ZeroMxT", "Max. torque is zero or negative"},
    {"ZeroMxMS", "Max. motor speed is zero or negative"},
    {"IvldPara", "Invalid Parameter"},
    {"SwLimOut", "Software Limit Out"},
    {"SiVelPfx", "Invalid velocity scaling factor"},
    {"SiVelTim", "Invalid SI velocity time factor"},
    {"IvldFeed", "Invalid Feed constant"},
    {"MxPwrLmt", "Maximum mechanical output power of the motor is reached"},
    {"FollwErr", "Following error"},
    {"MxTorLmt", "Maximum torque limit"},
    {"MxMSLmt", "Maximum motor speed limit"},
    {"SwPosLmt", "Software position limit"},
    {"PosRgLmt", "Encoder range limit"},
    {"IntgrLmt", "IntLmAct"},
    {"PosLmAct", "Positive Limit Switch Active"},
    {"NegLmAct", "Negative Limit"},
    {"CyclicHb", "Cyclic heartbeat"},
    {"OsCmdCol", "OS command collision"},
    {"I2tActv", "I2t protection is active"},
    {"HmInvMth", "Unsupported or invalid homing method"},
    {"Intern01", "Internal fault 1"},
    {"Intern02", "Internal fault 2"},
    {"Intern03", "Internal fault 3"},
    {"Intern04", "Internal fault 4"},
    {"Intern05", "Internal fault 5"},
    {"VeFiFcLo", "Velocity filter cut-off frequency too low"},
    {"VeFiFcHi", "Velocity filter cut-off frequency too high"},
    {"PoFiFcLo", "Position filter cut-off frequency too low"},
    {"PoFiFcHi", "Position filter cut-off frequency too high"},
    {"VffFcLo", "Velocity feed forward filter cut-off frequency too low"},
    {"VffFcHi", "Velocity feed forward filter cut-off frequency too high"},
    {"NhFiFcHi", "Notch filter center frequency too high"},
    {"NhFiPara", "Invalid Notch filter parameters"},
    {"SynDifHi", "Sync difference too high"},
    {"SmmNoCfg", "SMM not configured"},
    {"SmmFatal", "SMM internal fatal error"},
    {"SmmDead", "SMM is missing or is broken"},
    {"SmmOtw", "SMM temperature warning"},
    {"SmmFIO01", "Input 1 discrepancy"},
    {"SmmFIO02", "Input 2 discrepancy"},
    {"SmmFIO03", "Input 3 discrepancy"},
    {"SmmFIO04", "Input 4 discrepancy"},
    {"SmmFIO05", "Input 1 pulses missing"},
    {"SmmFIO06", "Input 2 pulses missing"},
    {"SmmFIO07", "Input 3 pulses missing"},
    {"SmmFIO08", "Input 4 pulses missing"},
    {"SmmFIn09", "Output 1 discrepancy"},
    {"SmmFIn10", "Output 2 discrepancy"},
    {"SmmFIn11", "Output 1 stuck at"},
    {"SmmFIn12", "Output 2 stuck at"},
    {"SmmFIn13", "STO output discrepancy"},
    {"SmmFIn14", "STO output fault"},
    {"SmmFIn15", "Brake output discrepancy"},
    {"SmmFIO16", "Brake output fault"},
    {"SmmFIn17", "Safety function discrepancy"},
    {"SmmFIn19", "Power supply external 5V analog fault"},
    {"SmmFIO20", "Analog input stuck at fault"},
    {"SmmFIO21", "Analog input sampler fault"},
    {"SmmFIn22", "Power supply external 5V encoder fault"},
    {"SmmFIO23", "Encoder fault"},
    {"SmmFIn24", "Power supply fault"},
    {"SmmFIn25", "Black-channel config fault"},
    {"SmmFIO26", "Black-channel fault"},
    {"SmmFIn27", "Core fault"},
    {"SmmFIO28", "Verification encoder direction error"},
    {"SmmFIO29", "24V Power Supply fault"},
    {"SmmFIO31", "Safety parameters invalid"},
    {"SmmFIn34", "Manufacturing parameters invalid"},
    {"SmmFIn35", "Temperature fault"},
    {"SmmFIn36", "Timing fault"},
    {"SmmFIn37", "Internal discrepancy"},
    {"SmmFIn38", "Internal fault"},
    {"SmmFIO39", "Internal Communication Fault"},
    {"QeiLsTck", "QEI Lost Ticks"},
    {"QeiNoIdx", "QEI No Index"},
    {"QeiOpnW", "QEI Open Wire"},
    {"InvConf", "Invalid Configuration"},
    {"InterLck", "DI for Interlock (Fault Reaction) is low"},
    {"CtrlCyEx", "Control cycle time is exceeded"},
    {"TmpCore", "Temperature of the core too high"},
    {"PhOtCore", "Temperature of the core too high"},
    {"TmpDrive", "Temperature of drive too high"},
    {"PhOtDriv", "Temperature of drive too high"},
    {"ExAnSnsr", "External Scaled Analog Sensor Output exceeded one of the threshold values"},
    {"DiagErr", "Diagnostic Error"}
};


/*array mapping an AW-J-Series drive detailed error reports to a text string */
const aw_j_series_error_string_t aw_j_series_error[NUM_OF_AW_J_SERIES_ERROR_STRINGS] = {
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
