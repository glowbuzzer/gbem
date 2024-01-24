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


gberror_t ec_pdo_map_aw_j_series_fsoe(const uint16_t slave) {
    ec_print_pdo_config_aw_series(slave);


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


bool ec_get_estop_state_aw_j_series_fsoe(void) {
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

#define AW_J_SERIES_FSOE_BASE_SM3_OFFSET                    43 //changes with PDO map
#define AW_J_SERIES_FSOE_STATUS_WORD_0_PDO_INDEX            (AW_J_SERIES_FSOE_BASE_SM3_OFFSET + 1)
#define AW_J_SERIES_FSOE_STATUS_WORD_1_PDO_INDEX            (AW_J_SERIES_FSOE_BASE_SM3_OFFSET + 5)
#define AW_J_SERIES_FSOE_CONNECTION_ID_PDO_INDEX            (AW_J_SERIES_FSOE_BASE_SM3_OFFSET + 29)

#define AW_J_SERIES_FSOE_BASE_SM2_OFFSET                    35 //changes with PDO map

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

