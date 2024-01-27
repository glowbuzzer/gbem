/**
 ******************************************************************************
 * @file           :  aw_j_series_fsoe
 * @brief          :  AW J Series control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "std_headers.h"
#include "aw_j_series_fsoe.h"
#include "aw_j_series.h"
#include <stdio.h>
#include "std_defs_and_macros.h"
#include "map.h"
#include "gberror.h"
#include "ecm_status.h"
#include "ethercatsetget.h"
#include "user_message.h"
#include "map_config_custom_pdo.h"


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

//19
//4
//12

map_custom_pdo_t aw_j_series_fsoe_custom_pdo_map = {
    .sm2_assignment_object = 0x1C12,
    .num_sm2_assignments = 4,
    .rxpdo = {
        {
            .pdo_assignment_object = 0x1600,
            .num_assignments = 7,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_CONTROLWORD_ADDRESS}, //0x6040 Controlword
                {.datatype = ECT_INTEGER8, .index = AW_J_SERIES_MODES_OF_OPERATION_ADDRESS}, //0x6060 Modes of operation
                {.datatype = ECT_INTEGER16, .index = AW_J_SERIES_TARGET_TORQUE_ADDRESS}, //0x6071 Target Torque
                {.datatype = ECT_INTEGER32, .index = AW_J_SERIES_TARGET_POSITION_ADDRESS}, //0x607A Target position
                {.datatype = ECT_INTEGER32, .index = AW_J_SERIES_TARGET_VELOCITY_ADDRESS}, //0x60FF Target velocity
                {.datatype = ECT_INTEGER16, .index = AW_J_SERIES_TORQUE_OFFSET_ADDRESS}, //0x60B2 Torque offset
                {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_TUNING_COMMAND_ADDRESS}, //0x2701 Tuning command
            }
        },
        {
            .pdo_assignment_object = 0x1601,
            .num_assignments = 2,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_PHYSICAL_OUTPUTS_ADDRESS, .offset = 0x1},
                //0x60FE Physical outputs
                {.datatype = ECT_UNSIGNED32, .index = 0x60FE, .offset = 0x2}, //0x60FE Bit mask
            }
        },
        {
            .pdo_assignment_object = 0x1602,
            .num_assignments = 3,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_USER_MOSI_ADDRESS}, //0x2703 User MOSI
                {.datatype = ECT_INTEGER32, .index = AW_J_SERIES_VELOCITY_OFFSET_ADDRESS}, //0x60B1 Velocity offset
                {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_LED_COLOUR_ADDRESS, .offset = 0x1},
                //0x2215 LED colour
            }
        },
        {.pdo_assignment_object = 0x1700}
    },
    .sm3_assignment_object = 0x1C13,
    .num_sm3_assignments = 5,
    .txpdo = {
        {
            .pdo_assignment_object = 0x1A00,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_STATUSWORD_ADDRESS}, //0x6041Statusword
                {.datatype = ECT_INTEGER8, .index = AW_J_SERIES_MODES_OF_OPERATION_DISPLAY_ADDRESS},
                //0x6061 Modes of operation display
                {.datatype = ECT_INTEGER32, .index = AW_J_SERIES_POSITION_ACTUAL_VALUE_ADDRESS},
                //0x6064 Position actual value
                {.datatype = ECT_INTEGER32, .index = AW_J_SERIES_VELOCITY_ACTUAL_VALUE_ADDRESS},
                //0x606C Velocity actual value
                {.datatype = ECT_INTEGER16, .index = AW_J_SERIES_TORQUE_ACTUAL_VALUE_ADDRESS},
                //0x6077 Torque actual value
            }
        },
        {
            .pdo_assignment_object = 0x1A01,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ANALOG_INPUT_1_ADDRESS}, //0x2401 Analog input 1
                {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ANALOG_INPUT_2_ADDRESS}, //0x2402 Analog input 2
                {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ANALOG_INPUT_3_ADDRESS}, //0x2403 Analog input 3
                {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ANALOG_INPUT_4_ADDRESS}, //0x2404 Analog input 4
                {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_TUNING_STATUS_ADDDRESS}, //0x2702Tuning status
            }
        },
        {
            .pdo_assignment_object = 0x1A02,
            .num_assignments = 1,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_DIGITAL_INPUTS_ADDRESS}, //0x60FD Digital inputs
            }
        },
        {
            .pdo_assignment_object = 0x1A03,
            .num_assignments = 5,
            .assignments = {
                {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_USER_MISO_ADDRESS}, //0x2704 User MISO
                {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_TIMESTAMP_ADDDRESS}, //0x20F0 Timestamp
                {.datatype = ECT_INTEGER32, .index = AW_J_SERIES_POSITION_DEMAND_INTERNAL_VALUE_ADDRESS},
                //0x60FC Position demand internal value
                {.datatype = ECT_INTEGER32, .index = AW_J_SERIES_VELOCITY_DEMAND_VALUE_ADDRESS},
                //0x606B Velocity demand value
                {.datatype = ECT_INTEGER16, .index = AW_J_SERIES_TORQUE_DEMAND_ADDRESS} //0x6074 Torque demand
            }
        },
        {
            .pdo_assignment_object = 0x1B00
        }
    }
};
// ECT_BOOLEAN = 0x0001,
//  ECT_INTEGER8 = 0x0002,
//  ECT_INTEGER16 = 0x0003,
//  ECT_INTEGER32 = 0x0004,
//  ECT_UNSIGNED8 = 0x0005,
//  ECT_UNSIGNED16 = 0x0006,
//  ECT_UNSIGNED32 = 0x0007,
//  ECT_REAL32 = 0x0008,
//  ECT_VISIBLE_STRING = 0x0009,
//  ECT_OCTET_STRING = 0x000A,
//  ECT_UNICODE_STRING = 0x000B,
//  ECT_TIME_OF_DAY = 0x000C,
//  ECT_TIME_DIFFERENCE = 0x000D,
//  ECT_DOMAIN = 0x000F,
//  ECT_INTEGER24 = 0x0010,
//  ECT_REAL64 = 0x0011,
//  ECT_INTEGER64 = 0x0015,
//  ECT_UNSIGNED24 = 0x0016,
//  ECT_UNSIGNED64 = 0x001B,
//  ECT_BIT1 = 0x0030,
//  ECT_BIT2 = 0x0031,
//  ECT_BIT3 = 0x0032,
//  ECT_BIT4 = 0x0033,
//  ECT_BIT5 = 0x0034,
//  ECT_BIT6 = 0x0035,
//  ECT_BIT7 = 0x0036,
//  ECT_BIT8 = 0x0037

uint32_t ec_get_datatype_size_bits(ec_datatype datatype) {
    switch (datatype) {
        case ECT_BOOLEAN:
            return 1;
        case ECT_INTEGER8:
            return 8;
        case ECT_INTEGER16:
            return 16;
        case ECT_INTEGER24:
            return 24;
        case ECT_INTEGER32:
            return 32;
        case ECT_INTEGER64:
            return 64;
        case ECT_UNSIGNED8:
            return 8;
        case ECT_UNSIGNED16:
            return 16;
        case ECT_UNSIGNED24:
            return 24;
        case ECT_UNSIGNED32:
            return 32;
        case ECT_UNSIGNED64:
            return 64;
        case ECT_REAL32:
            return 32;
        case ECT_REAL64:
            return 64;
        //todo crit finish these sizes
        case ECT_VISIBLE_STRING:
            return 8;
        case ECT_OCTET_STRING:
            return 0;
        case ECT_UNICODE_STRING:
            return 0;
        case ECT_TIME_OF_DAY:
            return 0;
        case ECT_TIME_DIFFERENCE:
            return 0;
        case ECT_DOMAIN:
            return 0;
        case ECT_BIT1:
            return 1;
        case ECT_BIT2:
            return 2;
        case ECT_BIT3:
            return 3;
        case ECT_BIT4:
            return 4;
        case ECT_BIT5:
            return 5;
        case ECT_BIT6:
            return 6;
        case ECT_BIT7:
            return 7;
        case ECT_BIT8:
            return 8;
        default:
            LL_FATAL("GBEM: missing ec_datatype to size definition");
    }
}

uint32_t ec_pdo_map_find_size_of_custom_map_rxpdo(map_custom_pdo_t map) {
    uint32_t size = 0;
    //todo crit handling things like octect strings
    for (int i = 0; i < map.num_sm2_assignments; i++) {
        for (int j = 0; j < map.rxpdo[i].num_assignments; j++) {
            size += ec_get_datatype_size_bits(map.rxpdo[i].assignments[j].datatype);
        }
    }
    return size;
}

uint32_t ec_pdo_map_find_size_of_custom_map_txpdo(map_custom_pdo_t map) {
    uint32_t size = 0;
    //todo crit handling things like octect strings
    for (int i = 0; i < map.num_sm3_assignments; i++) {
        for (int j = 0; j < map.txpdo[i].num_assignments; j++) {
            size += ec_get_datatype_size_bits(map.txpdo[i].assignments[j].datatype);
        }
    }
    return size;
}


//works with standard map
gberror_t ec_custom_fmmu_sm_aw_j_series(const uint16_t slave) {
    ec_slave[slave].configindex = 998;

    // ec_slave[slave].Ibits = 600; // 75*8
    // ec_slave[slave].Obits = 376; // 47*8

    ec_slave[slave].Ibits = 8 * (AW_J_SERIES_EC_FSOE_SM3_FSOE_SIZE + AW_J_SERIES_EC_FSOE_SM3_OFFSET);
    ec_slave[slave].Obits = 8 * (AW_J_SERIES_EC_FSOE_SM2_FSOE_SIZE + AW_J_SERIES_EC_FSOE_SM2_OFFSET);

    //todo crit
    // ec_slave[slave].FMMU[0].LogLength = 82;
    // ec_slave[slave].FMMU[1].LogLength = 54;


    // ec_slave[slave].SM[2].SMlength = 47; //35 (SM2 size) + 12 (FSoE size)
    // ec_slave[slave].SM[3].SMlength = 75; //43 (SM3 size) + 32 (FSoE size)

    ec_slave[slave].SM[2].SMlength = (AW_J_SERIES_EC_FSOE_SM2_FSOE_SIZE + AW_J_SERIES_EC_FSOE_SM2_OFFSET);
    ec_slave[slave].SM[3].SMlength = (AW_J_SERIES_EC_FSOE_SM3_FSOE_SIZE + AW_J_SERIES_EC_FSOE_SM3_OFFSET);

    return E_SUCCESS;
}


gberror_t ec_pdo_map_aw_j_series_fsoe(const uint16_t slave) {
    ec_print_pdo_config_aw_series(slave);


    UM_INFO(GBEM_UM_EN, "GBEM: Size of rxpdo for AW series drive [%u] is [%f] bytes", map_slave_to_drive(slave),
            (double)ec_pdo_map_find_size_of_custom_map_rxpdo(aw_j_series_fsoe_custom_pdo_map)/8.0);

    UM_INFO(GBEM_UM_EN, "GBEM: Size of txpdo for AW series drive [%u] is [%f] bytes", map_slave_to_drive(slave),
            (double)ec_pdo_map_find_size_of_custom_map_txpdo(aw_j_series_fsoe_custom_pdo_map)/8.0);

    //todo crit - does not work if we apply this
    return map_apply_custom_pdo_mapping(slave, aw_j_series_fsoe_custom_pdo_map);


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

    ec_print_pdo_config_aw_series(slave);
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


gberror_t ec_apply_standard_sdos_aw_j_series_fsoe(const uint16_t slave) {
    ec_set_slots_aw_j_series_fsoe(slave);


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


bool ec_get_safety_state_aw_j_series_fsoe(void) {
    //this will look in the functional outputs of the bbh and decide what the overall estop states is
}


/* Status word 0
    0.0 - STO state
    0.1 - res
    0.2 - res
    0.3 - SOS state
    0.4 - res
    0.5 - res
    0.6 - res
    0.7 - Error active
    1.0 - SS1 state
    1.1 - SS2 state
    1.2 - res
    1.3 - res
    1.4 - SLS1 state
    1.5 - SLS2 state
    1.6 - SLS3 state
    1.7 - SLS4 state
 */
/* Status word 1
    0.0 - restart ack required
    0.1 - SBC state
    0.2 - Temp warning
    0.3 - Safe pos valid
    0.4 - Safe speed active
    0.5 - res
    0.6 - res
    0.7 - res
    1.0 - safe input 1
    1.1 - safe input 2
    1.2 - res
    1.3 - res
    1.4 - safe ouput 1
    1.5 - res
    1.6 - analog inpuut diagnostic active
    1.7 - analog input valid

 */

//standard
// #define AW_J_SERIES_FSOE_BASE_SM3_OFFSET                    43 //changes with PDO map
#define AW_J_SERIES_FSOE_BASE_SM3_OFFSET                    (AW_J_SERIES_EC_FSOE_SM3_OFFSET) //changes with PDO map


//custom
// #define AW_J_SERIES_FSOE_BASE_SM3_OFFSET                    49 //changes with PDO map

#define AW_J_SERIES_FSOE_STATUS_WORD_0_PDO_INDEX            (AW_J_SERIES_FSOE_BASE_SM3_OFFSET + 1)
#define AW_J_SERIES_FSOE_STATUS_WORD_1_PDO_INDEX            (AW_J_SERIES_FSOE_BASE_SM3_OFFSET + 5)
#define AW_J_SERIES_FSOE_CONNECTION_ID_PDO_INDEX            (AW_J_SERIES_FSOE_BASE_SM3_OFFSET + 29)

//standard
// #define AW_J_SERIES_FSOE_BASE_SM2_OFFSET                    35 //changes with PDO map
#define AW_J_SERIES_FSOE_BASE_SM2_OFFSET                    (AW_J_SERIES_EC_FSOE_SM2_OFFSET) //changes with PDO map
//custom
// #define AW_J_SERIES_FSOE_BASE_SM2_OFFSET                    35 //changes with PDO map

#define AW_J_SERIES_FSOE_STATUS_ERROR_BIT_NO                7
#define AW_J_SERIES_FSOE_STATUS_RESTART_ACK_REQ_BIT_NO      15


gberror_t ec_fsoe_get_slave_state_aw_j_series(uint16_t slave, uint32_t *state,
                                              fsoe_slave_high_level_state_t *high_level_state) {
    uint16_t status_word_0 = 0;
    uint16_t status_word_1 = 0;


    status_word_0 = ec_pdo_get_input_uint16(slave, AW_J_SERIES_FSOE_STATUS_WORD_0_PDO_INDEX);
    status_word_1 = ec_pdo_get_input_uint16(slave, AW_J_SERIES_FSOE_STATUS_WORD_1_PDO_INDEX);

    // printf("Slave [%u]\n", slave);
    // printf("Status word 0 [%u]\n", status_word_0);
    // printf("Status word 1 [%u]\n", status_word_1);


    *state = ((uint32_t) status_word_1 << 16) | status_word_0;

    bool error = false;
    bool restart_ack_req = false;

    if (BIT_CHECK(*state, AW_J_SERIES_FSOE_STATUS_ERROR_BIT_NO)) {
        error = true;
    }
    if (BIT_CHECK(*state, AW_J_SERIES_FSOE_STATUS_RESTART_ACK_REQ_BIT_NO)) {
        restart_ack_req = true;
    }

    if (error && restart_ack_req) {
        *high_level_state = FSOE_SLAVE_HIGH_LEVEL_STATE_ERROR_AND_ACK_REQ;
    } else if (error) {
        *high_level_state = FSOE_SLAVE_HIGH_LEVEL_STATE_ERROR;
    } else if (restart_ack_req) {
        *high_level_state = FSOE_SLAVE_HIGH_LEVEL_STATE_ACK_REQ;
    } else {
        *high_level_state = FSOE_SLAVE_HIGH_LEVEL_STATE_NONE;
    }

    return E_SUCCESS;
}


gberror_t ec_fsoe_get_slave_con_id_aw_j_series(uint16_t slave, uint16_t *con_id) {
    *con_id = ec_pdo_get_input_uint16(slave, AW_J_SERIES_FSOE_CONNECTION_ID_PDO_INDEX);

    return E_SUCCESS;
}

