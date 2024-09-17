/**
 ******************************************************************************
 * @file           :  bbh_scu_1_ec
 * @brief          :  BBH SCU-1-EC control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "bbh_scu_1_ec.h"
#include "map.h"
#include "ecm_status.h"
#include "gberror.h"
#include "shared_mem_types.h"
#include "gbem_config.h"

int bbh_scu_1_ec_functional_outputs_offset_base_dynamic = 0;
int bbh_scu_1_ec_functional_inputs_offset_base_dynamic = 0;

int bbh_scu_1_ec_total_in_size_bytes = 0;
int bbh_scu_1_ec_total_out_size_bytes = 0;





/*functional outputs (which are our inputs, things like status)

these start at offset 8 but have n times the slot size (12 bytes with padding) added

these have 2 bytes of status then 5 bytes of debug before the actual ouputs

so for one slave:

functional outputs start at 8 + 12 + 2 + 5 = 27

status lives at 8 + 12 = 20

debug 0 lives at 8 + 12 + 2 = 22

*/

/*function inputs (which are our outputs)

these start at offset 32 but have n times the slot size (32 bytes with padding) added
so for one slave they start at 32+32 = 64

*/


bool ec_fsoe_get_safety_state_bbh_scu_1_ec(uint16_t slave) {
    //this will look in the functional outputs of the bbh and decide what the overall estop states is


    return !ec_pdo_get_input_bit_from_byte_slave(slave, BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0,
                                                 FSOE_STANDARD_OVERALL_SAFETY_STATE_BIT_NUM);
}

/**
 * \brief sets the error ack bit for the BBH SCU 1 EC Safety Controller functional inputs
 * \param state
 * \param slave
 * \return
 */
gberror_t ec_fsoe_set_error_ack_state_bbh_scu_1_ec(bool state, uint16_t slave) {
    ec_pdo_set_output_bit_from_byte_slave(slave, BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0,
                                          FSOE_STANDARD_ERROR_ACK_CMD_BIT_NUM, state);

    UM_WARN(GBEM_UM_EN, "GBEM: Sending error acknowledgement to BBH SCU 1 EC");
    return E_SUCCESS;
}

//todo crit slave not needed as param
uint32_t get_functional_outputs_offset_bbh_scu_1_ec(uint16_t slave) {
    uint32_t offset = BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_BASE_OFFSET;

    offset = offset + map_fsoe_master_get_overall_slot_size_out();


    // switch (map_slave_mdp_slot_type[slave - 1]) {
    //     case MDP_SLOT_TYPE_NONE:
    //         UM_FATAL("GBEM: No slot type set for slave %u", slave);
    //         break;
    //     case MDP_SLOT_TYPE_MASTER_AGGREGATE:
    //         break;
    //     case MDP_SLOT_TYPE_32_12:
    //         //todo crit TODAY
    //         // offset = BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_BASE_OFFSET + (12 * (map_slave_number_of_slots[slave - 1]));
    //         // printf("offset get op offset = %u\n", offset);
    //         break;
    //     default:
    //         UM_FATAL("GBEM: Unknown slot type set for slave %u", slave);
    // }

    return offset;
}


gberror_t ec_fsoe_get_master_state_bbh_scu_1_ec(uint16_t slave,
                                                enum FSOE_MASTER_HIGH_LEVEL_STATE *master_high_level_state,
                                                uint32_t *error_code) {
    static bool first_run = true;
    static uint32_t offset = 0;

    if (first_run) {
        first_run = false;
        offset = get_functional_outputs_offset_bbh_scu_1_ec(slave);
    }

    uint8_t debug_0 = 0;

    debug_0 = ec_pdo_get_input_uint8(slave, offset + BBH_SCU_1_EC_DEBUG_0_PDO_OFFSET);

    //mask the upper 4 bits to get the SCU state
    uint8_t bbh_state = debug_0 & 0x0F;

    // printf("BBH_SCU_1_EC: SCU state = %u\n", bbh_state);

    switch (bbh_state) {
        case BBH_SCU_MODE_NONE:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_NONE;
        case BBH_SCU_MODE_START_UP:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_START_UP;
            break;
        case BBH_SCU_MODE_SENDCONFIG:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_SENDCONFIG;
            break;
        case BBH_SCU_MODE_STARTUP_BUS:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_STARTUP_BUS;
            break;
        case BBH_SCU_MODE_RUN:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_RUN;
            break;
        case BBH_SCU_MODE_STOP:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_STOP;
            break;
        case BBH_SCU_MODE_ERROR:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_ERROR;
            break;
        case BBH_SCU_MODE_ALARM:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_ALARM;
            break;
        case BBH_SCU_MODE_LOCAL_MODE:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_NO_NETWORK;
            break;
        default:
            *master_high_level_state = FSOE_MASTER_HIGH_LEVEL_STATE_NONE;
            break;
    }


    uint8_t debug_3 = ec_pdo_get_input_uint8(slave, offset + BBH_SCU_1_EC_DEBUG_3_PDO_OFFSET);
    uint8_t debug_4 = ec_pdo_get_input_uint8(slave, offset + BBH_SCU_1_EC_DEBUG_4_PDO_OFFSET);

    *error_code = ((uint32_t) debug_4 << 8) | debug_3;


    return E_SUCCESS;
}


gberror_t ec_set_slots_bbh_scu_1_ec(const uint16_t slave);

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_bbh_scu_1_ec = {
        .number_of_entries = 7,
        .SM_assignment_index = 0x1c12
};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_bbh_scu_1_ec = {
        .number_of_entries = 4,
        .SM_assignment_index = 0x1c13
};

/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_bbh_scu_1_ec[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1600,
        0x1601,
        0x1684,
        0x1685,
        0x1686,
        0x1687,
        0x1688,
};

/* This is used for the fixed PDO remapping */
uint16_t map_SM3_index_of_assigned_PDO_bbh_scu_1_ec[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a00,
        0x1a01,
        0x1a84,
        0x1a88,
};

gberror_t ec_pdo_map_bbh_scu_1_ec(const uint16_t slave) {
    if (!ec_sdo_write_uint16(slave, map_SM2_bbh_scu_1_ec.SM_assignment_index, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_bbh_scu_1_ec.SM_assignment_index, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_bbh_scu_1_ec.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_bbh_scu_1_ec.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_bbh_scu_1_ec[i], true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_bbh_scu_1_ec.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_bbh_scu_1_ec.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_bbh_scu_1_ec[i], true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_bbh_scu_1_ec.SM_assignment_index, 0,
                             map_SM2_bbh_scu_1_ec.number_of_entries, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_bbh_scu_1_ec.SM_assignment_index, 0,
                             map_SM3_bbh_scu_1_ec.number_of_entries, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    return E_SUCCESS;
}


gberror_t ec_print_slots_bbh_scu_1_ec(const uint16_t slave) {
    uint8_t no_slots = 0;

    if (!ec_sdo_read_uint8(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                           0,
                           &no_slots, true)) {
        return E_SDO_READ_FAILURE;
    }

    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX [0x%02x] [%u]",
            BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX, no_slots);

    //print slot entries
    for (uint8_t i = 1; i <= no_slots; i++) {
        uint32_t module_id = 0;
        if (!ec_sdo_read_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                                i,
                                &module_id, true)) {
            return E_SDO_READ_FAILURE;
        }
        UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX [0x%02x] [%u] = 0x%08x",
                BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX, i, module_id);
    }
    return E_SUCCESS;
}


gberror_t ec_apply_standard_sdos_bbh_scu_1_ec(const uint16_t slave) {
    ec_set_slots_bbh_scu_1_ec(slave);
    return E_SUCCESS;
}


gberror_t ec_set_slots_bbh_scu_1_ec(const uint16_t slave) {
    UM_INFO(GBEM_UM_EN, "GBEM: Setting slots for BBH_SCU_1_EC");
    UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Number of FSoE master slots: %u", MAP_NUM_FSOE_MASTER_SLOTS);


    UM_INFO(GBEM_UM_EN,
            "GBEM: BBH Slot setting: Checking slots configuration versus functional input and output offsets for BBH_SCU_1_EC");

    UM_INFO(GBEM_UM_EN,
            "GBEM: BBH Slot setting: OUT Slot size for BBH_SCU_1_EC is %u (all slot sizes summed excluding master to master output size)",
            map_fsoe_master_get_overall_slot_size_out());
    UM_INFO(GBEM_UM_EN,
            "GBEM: BBH Slot setting: IN Slot size for BBH_SCU_1_EC is %u (all slot sizes summed excluding master to master input size)",
            map_fsoe_master_get_overall_slot_size_in());


    bbh_scu_1_ec_functional_outputs_offset_base_dynamic =
            map_fsoe_master_get_overall_slot_size_out() + BBH_SCU_1_MASTER_MASTER_OUTPUT_MSG_SIZE;
    bbh_scu_1_ec_functional_inputs_offset_base_dynamic =
            map_fsoe_master_get_overall_slot_size_in() + BBH_SCU_1_MASTER_MASTER_INPUT_MSG_SIZE;

    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: functional_outputs_offset_base_dynamic = %d",
            bbh_scu_1_ec_functional_outputs_offset_base_dynamic);
    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: functional_inputs_offset_base_dynamic = %d",
            bbh_scu_1_ec_functional_inputs_offset_base_dynamic);

//compute values of the functional ins/outs offsets
    ec_compute_functional_offset_bbh_scu_1_ec();

//    if ((map_fsoe_master_get_overall_slot_size_out() +
//         BBH_SCU_1_MASTER_MASTER_OUTPUT_MSG_SIZE) != BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BASE) {
//        UM_FATAL(
//                "GBEM: BBH Slot setting: OUT Slot size for BBH_SCU_1_EC is not correct (or functional IO base address). Expected %u, got %u",
//                BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BASE, map_fsoe_master_get_overall_slot_size_out() +
//                                                             BBH_SCU_1_MASTER_MASTER_OUTPUT_MSG_SIZE);
//    } else {
//        UM_INFO(GBEM_UM_EN,
//                "GBEM: BBH Slot setting: OUT Slot size for BBH_SCU_1_EC is correct (or functional IO base address). Expected [%u], got [%u]",
//                BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BASE,
//                map_fsoe_master_get_overall_slot_size_out() + BBH_SCU_1_MASTER_MASTER_OUTPUT_MSG_SIZE);
//    }
//
//    if (map_fsoe_master_get_overall_slot_size_in() + BBH_SCU_1_MASTER_MASTER_INPUT_MSG_SIZE !=
//        BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE) {
//        UM_FATAL(
//                "GBEM: BBH Slot setting: IN Slot size for BBH_SCU_1_EC is not correct (or functional IO base address). Expected %u, got %u",
//                BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE,
//                map_fsoe_master_get_overall_slot_size_in() + BBH_SCU_1_MASTER_MASTER_INPUT_MSG_SIZE);
//    } else {
//        UM_INFO(GBEM_UM_EN,
//                "GBEM: BBH Slot setting: IN Slot size for BBH_SCU_1_EC is correct (or functional IO base address). Expected [%u], got [%u]",
//                BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE,
//                map_fsoe_get_slot_size_master_in(slave) + BBH_SCU_1_MASTER_MASTER_INPUT_MSG_SIZE);
//    }


    ec_print_slots_bbh_scu_1_ec(slave);


    //clear slot entries
    UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Slot 0 set to [35] (Clearing slots for BBH_SCU_1_EC)");
    if (!ec_sdo_write_uint8(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                            (uint8_t) 0,
                            35, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    int number_of_slots_filled = 1;
    for (uint8_t i = 0; i < MAP_NUM_SLAVES; i++) {

        switch (map_slave_fsoe_slave_type[i]) {
            case FSOE_SLAVE_TYPE_NONE:
                UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Slave [%u] is not an FSoE slave", i + 1);
                break;
            case FSOE_SLAVE_TYPE_SYNAPTICON:
                UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Slave [%u] is a Synapticon slave", i + 1);
                UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Setting slot [%u] to [%#x]", number_of_slots_filled,
                        BBH_SCU_1_EC_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID);
                if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                                         number_of_slots_filled,
                                         BBH_SCU_1_EC_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID,
                                         true)) {
                    return E_SDO_WRITE_FAILURE;
                }
                number_of_slots_filled++;
                break;
            case FSOE_SLAVE_TYPE_EL1904:
                UM_FATAL("GBEM: BBH Slot setting: Slave [%u] is an EL1904 slave - this is not supported", i + 1);
                break;
            case FSOE_SLAVE_TYPE_EL2904:
                UM_FATAL("GBEM: BBH Slot setting: Slave [%u] is an EL2904 slave - this is not supported", i + 1);
                break;
            case FSOE_SLAVE_TYPE_SCU_1_EC:
                UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Slave [%u] is an SCU_1_EC slave - this will be  ignored",
                        i + 1);
                break;
            case FSOE_SLAVE_TYPE_EL6900:
                UM_FATAL("GBEM: BBH Slot setting: Slave [%u] is an EL6900 slave - this is not supported", i + 1);
                break;
            case FSOE_SLAVE_TYPE_EL6910:
                UM_FATAL("GBEM: BBH Slot setting: Slave [%u] is an EL6910 slave - this is not supported", i + 1);
                break;
            case FSOE_SLAVE_TYPE_ELM7231:

                break;
            default:
                UM_FATAL("GBEM: BBH Slot setting: Slave [%u] has an unknown slave type", i + 1);

        }


    }

    //set first slot to 0x6212
//    if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
//                             1, BBH_SCU_1_EC_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID,
//                             true)) {
//        return E_SDO_WRITE_FAILURE;
//    }

    //fill in the rest of the slots with 0
    for (uint8_t i = number_of_slots_filled; i <= 32; i++) {

        UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Setting slot [%u] to [0x0] (empty slot)", i);

        if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                                 i,
                                 0, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Setting slot [33] to [%#x]",
            BBH_SCU_1_EC_SLOT_33_SCU_AS_FSOE_SLAVE_CONNECTION);

    if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             33, BBH_SCU_1_EC_SLOT_33_SCU_AS_FSOE_SLAVE_CONNECTION,
                             true)) {
        return E_SDO_WRITE_FAILURE;
    }

    UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Setting slot [34] to [%#x]", BBH_SCU_1_EC_SLOT_34_MASTER_TO_MASTER);

    if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             34, BBH_SCU_1_EC_SLOT_34_MASTER_TO_MASTER,
                             true)) {
        return E_SDO_WRITE_FAILURE;
    }


    UM_INFO(GBEM_UM_EN, "GBEM: BBH Slot setting: Setting slot [35] to %u", BBH_SCU_1_EC_SLOT_35_FUNCTIONAL_IN_OUT);

    if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             35, BBH_SCU_1_EC_SLOT_35_FUNCTIONAL_IN_OUT, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //todo crit why isn't this here?
    //    //write number of slots
    //    if (!ec_sdo_write_uint8(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
    //                            0,
    //                            35, true)) {
    //        return E_SDO_WRITE_FAILURE;
    //    }
    //print slots to confirm
    ec_print_slots_bbh_scu_1_ec(slave);

    return E_SUCCESS;
}


gberror_t ec_compute_functional_offset_bbh_scu_1_ec(void) {


    UM_INFO(GBEM_UM_EN, "GBEM: Computing functional offsets for BBH_SCU_1_EC");

    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
            BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
            BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 1);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
            BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 2);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_3 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
            BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 3);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_4 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
            BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 4);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_5 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
            BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 5);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_6 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
            BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 6);


    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 1);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_2 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 2);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_3 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 3);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_4 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 4);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_5 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 5);
    UM_INFO(GBEM_UM_EN, "GBEM: BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_6 [%u]",
            (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 6);


    int num_inputs_replaced = 0;
    int num_outputs_replaced = 0;

    //loop over map_iomap and replace values
    for (int i = 0; i < map_num_rows_in_iomap; i++) {

        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
                                        BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG;

            num_outputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
                                        BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 1;

            num_outputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
                                        BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 2;
            num_outputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_3) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
                                        BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 3;
            num_outputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_4) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
                                        BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 4;
            num_outputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_5) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
                                        BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 5;
            num_outputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_6) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_outputs_offset_base_dynamic +
                                        BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG + 6;
            num_outputs_replaced++;
        }


        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic;
            num_inputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 1;
            num_inputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_2) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 2;
            num_inputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_3) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 3;
            num_inputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_4) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 4;
            num_inputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_5) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 5;
            num_inputs_replaced++;
        }
        if (map_iomap[i].pdo.byte_num == BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_6) {
            map_iomap[i].pdo.byte_num = (uint32_t) bbh_scu_1_ec_functional_inputs_offset_base_dynamic + 6;
            num_inputs_replaced++;
        }


    }

    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: Replaced %d outputs", num_outputs_replaced);
    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: Replaced %d inputs", num_inputs_replaced);

    return E_SUCCESS;


}


gberror_t ec_custom_fmmu_sm_bbh_scu_1_ec(const uint16_t slave) {
    ec_slave[slave].configindex = 999;

    bbh_scu_1_ec_total_out_size_bytes =
            map_fsoe_master_get_overall_slot_size_out() + BBH_SCU_1_MASTER_MASTER_OUTPUT_MSG_SIZE +
            BBH_SCU_1_EC_FUNCTIONAL_OUPUTS_OVERALL_SIZE; //6=114, 1=54
    bbh_scu_1_ec_total_in_size_bytes =
            map_fsoe_master_get_overall_slot_size_in() + BBH_SCU_1_MASTER_MASTER_INPUT_MSG_SIZE +
            BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OVERALL_SIZE; //6=242, 1=82


    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: total_out_size_bytes  [%d]", bbh_scu_1_ec_total_out_size_bytes);
    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: total_in_size_bytes  [%d]", bbh_scu_1_ec_total_in_size_bytes);


//    ec_slave[slave].Ibits = 432; //54 bytes
//    ec_slave[slave].Obits = 656; //82 bytes

    ec_slave[slave].Ibits = bbh_scu_1_ec_total_out_size_bytes * 8;
    ec_slave[slave].Obits = bbh_scu_1_ec_total_in_size_bytes * 8;


    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: Ibits [%d]", ec_slave[slave].Ibits);
    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: Obits [%d]", ec_slave[slave].Obits);




    //todo crit
    // ec_slave[slave].FMMU[0].LogLength = 82;
    // ec_slave[slave].FMMU[1].LogLength = 54;


//    ec_slave[slave].SM[2].SMlength = 82;
//    ec_slave[slave].SM[3].SMlength = 54;


    ec_slave[slave].SM[2].SMlength = bbh_scu_1_ec_total_in_size_bytes;
    ec_slave[slave].SM[3].SMlength = bbh_scu_1_ec_total_out_size_bytes;

    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: SM[2].SMlength [%d]", ec_slave[slave].SM[2].SMlength);
    UM_INFO(GBEM_UM_EN, "BBH_SCU_1_EC: SM[3].SMlength [%d]", ec_slave[slave].SM[3].SMlength);


    return E_SUCCESS;
}

// #define BBH_SCU_1_EC_FSOE_STATUS_WORD_BYTE_OFFSET 20

// gberror_t ec_fose_get_master_state_bbh_scu_1_ec(const uint16_t slave) {
//     //functional outputs
//
//     /*
//      * offset 20 = byte 0-1 status word
//      */
//     //todo is this an input?
//     return (ec_pdo_get_input_uint16(slave, BBH_SCU_1_EC_FSOE_STATUS_WORD_BYTE_OFFSET));
// }
//

typedef struct {
    uint32_t address;
    char *description;
} bbh_scu_1_slot_addresses_t;

bbh_scu_1_slot_addresses_t bbh_scu_1_slot_addresses[50] = {
        {0x6220, "FSoE Slave Conn 12In/12Out (Bytes)"},
        {0x6192, "FSoE Conn 58In/16Out (Bytes)"},
        {0x6193, "FSoE Conn 12In/12Out (Bytes)"},
        {0x6200, "FSoE Conn 01In/01Out (Bytes)"},
        {0x6201, "FSoE Conn 01In/01Out (Bytes)"},
        {0x6202, "FSoE Conn 02In/02Out (Bytes)"},
        {0x6203, "FSoE Conn 08In/04Out (Bytes)"},
        {0x6204, "FSoE Conn 22In/08Out (Bytes)"},
        {0x6205, "FSoE Conn 10In/10Out (Bytes)"},
        {0x6206, "FSoE Conn 12In/08Out (Bytes)"},
        {0x6207, "FSoE Conn 01In/01Out (Bytes)"},
        {0x6208, "FSoE Conn 01In/01Out (Bytes)"},
        {0x6209, "FSoE Conn 01In/01Out (Bytes)"},
        {0x620A, "FSoE Conn 01In/01Out (Bytes)"},
        {0x620B, "FSoE Conn 12In/12Out (Bytes)"},
        {0x620C, "FSoE Conn 08In/08Out (Bytes)"},
        {0x620D, "FSoE Conn 04In/04Out (Bytes)"},
        {0x620E, "FSoE Conn 06In/06Out (Bytes)"},
        {0x620F, "FSoE Conn 08In/08Out (Bytes)"},
        {0x6210, "FSoE Conn 01In/01Out (Bytes)"},
        {0x6211, "FSoE Conn 12In/04Out (Bytes)"},
        {0x6212, "FSoE Conn 14In/04Out (Bytes)"},
        {0x6213, "FSoE Conn 12In/02Out (Bytes)"},
        {0x6215, "FSoE Conn 01In/01Out (Bytes)"},
        {0x6216, "FSoE Conn 01In/01Out (Bytes)"},
        {0x6217, "FSoE Conn 66In/66Out (Bytes)"},
        {0x6218, "FSoE Conn 08In/06Out (Bytes)"},
        {0x6219, "FSoE Conn 08In/02Out (Bytes)"},
        {0x621B, "FSoE Conn 16In/16Out (Bytes)"},
        {0x621C, "FSoE Conn 6In/4Out (Bytes)"},
};

