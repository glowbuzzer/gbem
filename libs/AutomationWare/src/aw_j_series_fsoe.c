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


/**
 * \brief This is the custom PDO mapping for a AW J Series drive (FSoE)
 */
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
            .num_assignments = 3,
            .assignments = {
                // {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ANALOG_INPUT_1_ADDRESS}, //0x2401 Analog input 1
                // {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ANALOG_INPUT_2_ADDRESS}, //0x2402 Analog input 2
                // {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ANALOG_INPUT_3_ADDRESS}, //0x2403 Analog input 3
                // {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ANALOG_INPUT_4_ADDRESS}, //0x2404 Analog input 4
                {.datatype = ECT_UNSIGNED16, .index = AW_J_SERIES_ERROR_CODE_ADDRESS}, // 0x603F Error code
                {.datatype = ECT_STRING8, .index = AW_J_SERIES_ERROR_REPORT_ADDRESS, .offset = 0x1},
                //0x603F Error report
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
            .num_assignments = 4,
            .assignments = {
                // {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_USER_MISO_ADDRESS}, //0x2704 User MISO
                // {.datatype = ECT_UNSIGNED32, .index = AW_J_SERIES_TIMESTAMP_ADDDRESS}, //0x20F0 Timestamp
                {.datatype = ECT_INTEGER32, .index = AW_J_SERIES_CONTROL_EFFORT_ADDRESS}, //0x60B0 Control effort
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


/**
 * \brief calculates the custom PDO map size in bits based on the custom config struct - SM2
 * \param map
 * \return size in bits
 */
uint32_t ec_pdo_map_find_size_of_custom_map_rxpdo(map_custom_pdo_t map) {
    uint32_t size = 0;
    //todo crit handling things like octect strings
    for (int i = 0; i < map.num_sm2_assignments; i++) {
        for (int j = 0; j < map.rxpdo[i].num_assignments; j++) {
            size += map_get_datatype_size_bits(map.rxpdo[i].assignments[j].datatype);
        }
    }
    return size;
}

/**
 * \brief calculates the custom PDO map size in bits based on the custom config struct - SM3
 * \param map
 * \return size in bits
 */
uint32_t ec_pdo_map_find_size_of_custom_map_txpdo(map_custom_pdo_t map) {
    uint32_t size = 0;
    //todo crit handling things like octect strings
    for (int i = 0; i < map.num_sm3_assignments; i++) {
        for (int j = 0; j < map.txpdo[i].num_assignments; j++) {
            size += map_get_datatype_size_bits(map.txpdo[i].assignments[j].datatype);
        }
    }
    return size;
}


/**
 * \brief sets custom FMMU and SM sizes for AW J Series drive (FSoE)
 * \param slave
 * \return gberror_t
 */
gberror_t ec_custom_fmmu_sm_aw_j_series(const uint16_t slave) {
    ec_slave[slave].configindex = 998;

    // ec_slave[slave].Ibits = 600; // 75*8
    // ec_slave[slave].Obits = 376; // 47*8

    ec_slave[slave].Ibits = 8 * (AW_J_SERIES_EC_FSOE_SM3_FSOE_SIZE + AW_J_SERIES_EC_FSOE_SM3_OFFSET);
    ec_slave[slave].Obits = 8 * (AW_J_SERIES_EC_FSOE_SM2_FSOE_SIZE + AW_J_SERIES_EC_FSOE_SM2_OFFSET);

    // ec_slave[slave].FMMU[0].LogLength = 82;
    // ec_slave[slave].FMMU[1].LogLength = 54;


    // ec_slave[slave].SM[2].SMlength = 47; //35 (SM2 size) + 12 (FSoE size)
    // ec_slave[slave].SM[3].SMlength = 75; //43 (SM3 size) + 32 (FSoE size)

    ec_slave[slave].SM[2].SMlength = (AW_J_SERIES_EC_FSOE_SM2_FSOE_SIZE + AW_J_SERIES_EC_FSOE_SM2_OFFSET);
    ec_slave[slave].SM[3].SMlength = (AW_J_SERIES_EC_FSOE_SM3_FSOE_SIZE + AW_J_SERIES_EC_FSOE_SM3_OFFSET);

    return E_SUCCESS;
}

#define USE_CUSTOM_PDO_MAP_AW_J_SERIES_FSOE 1

gberror_t ec_pdo_map_aw_j_series_fsoe(const uint16_t slave) {
    ec_print_pdo_config_aw_series(slave);


    UM_INFO(GBEM_UM_EN, "GBEM: Size of rxpdo for AW series drive [%u] is [%f] bytes", map_slave_to_drive(slave),
            (double)ec_pdo_map_find_size_of_custom_map_rxpdo(aw_j_series_fsoe_custom_pdo_map)/8.0);

    UM_INFO(GBEM_UM_EN, "GBEM: Size of txpdo for AW series drive [%u] is [%f] bytes", map_slave_to_drive(slave),
            (double)ec_pdo_map_find_size_of_custom_map_txpdo(aw_j_series_fsoe_custom_pdo_map)/8.0);


#if USE_CUSTOM_PDO_MAP_AW_J_SERIES_FSOE == 1
    return map_apply_custom_pdo_mapping(slave, aw_j_series_fsoe_custom_pdo_map);
#else

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
#endif

    ec_print_pdo_config_aw_series(slave);
    return E_SUCCESS;
}

/**
 * \brief print slot (MDP) profile for AW J Series drive
 * \param slave
 * \return gberror_t
 */
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

/**
 * \brief sets slot (MDP) info for an AW J Series drive (FSoE)
 * \param slave
 * \return
 */
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

/**
 * \brief Applies standard SDOs for an AW J Series drive (FSoE)
 * \param slave
 * \return
 */
gberror_t ec_apply_standard_sdos_aw_j_series_fsoe(const uint16_t slave) {
    ec_set_slots_aw_j_series_fsoe(slave);

    ec_apply_limits_aw_j_series(slave);


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


/**
 * \brief gets the FSoE state of an AW J Series drive
 * \param slave
 * \param state
 * \param slave_high_level_state
 * \return gberror_t
 */
gberror_t ec_fsoe_get_slave_state_aw_j_series(uint16_t slave, uint32_t *state,
                                              enum FSOE_SLAVE_HIGH_LEVEL_STATE *slave_high_level_state) {
    uint16_t status_word_0 = 0;
    uint16_t status_word_1 = 0;


    status_word_0 = ec_pdo_get_input_uint16(slave, AW_J_SERIES_FSOE_STATUS_WORD_0_PDO_INDEX);
    status_word_1 = ec_pdo_get_input_uint16(slave, AW_J_SERIES_FSOE_STATUS_WORD_1_PDO_INDEX);

    // printf("Slave [%u]\n", slave);
    // printf("Status word 0 [%u]\n", status_word_0);
    // printf("Status word 1 [%u]\n", status_word_1);

    uint8_t slave_fsoe_txpdo_command = ec_pdo_get_input_uint8(slave, AW_J_SERIES_EC_FSOE_SM3_OFFSET);

    // printf("txpo %#x\n", slave_fsoe_txpdo_command);

    //note use of reverse function here

    uint8_t slave_fsoe_rxpdo_command = ec_pdo_get_output_uint8_rev(slave, AW_J_SERIES_EC_FSOE_SM2_OFFSET);
    // printf("rxpdo %#x\n", slave_fsoe_rxpdo_command);

    if (slave_fsoe_rxpdo_command == slave_fsoe_txpdo_command) {
        *slave_high_level_state = map_fsoe_command_to_state(slave_fsoe_rxpdo_command);
    } else {
        *slave_high_level_state = FSOE_SLAVE_HIGH_LEVEL_STATE_UNKNOWN;
    }

    *state = ((uint32_t) status_word_1 << 16) | status_word_0;


    return E_SUCCESS;
}

/**
 * \brief gets the FSoE connection ID of an AW J Series drive
 * \param slave
 * \param con_id
 * \return
 */
gberror_t ec_fsoe_get_slave_con_id_aw_j_series(uint16_t slave, uint16_t *con_id) {
    *con_id = ec_pdo_get_input_uint16(slave, AW_J_SERIES_FSOE_CONNECTION_ID_PDO_INDEX);

    return E_SUCCESS;
}

