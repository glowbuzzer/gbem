/**
 ******************************************************************************
 * @file           :  bbh_scu_1_ec
 * @brief          :
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
}


gberror_t ec_apply_standard_sdos_bbh_scu_1_ec(const uint16_t slave) {
    ec_set_slots_bbh_scu_1_ec(slave);
}

gberror_t ec_set_slots_bbh_scu_1_ec(const uint16_t slave) {
    printf("GBEM: Setting slots for BBH_SCU_1_EC\n");
    ec_print_slots_bbh_scu_1_ec(slave);


    //clear slot entries
    if (!ec_sdo_write_uint8(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                            (uint8_t) 0,
                            35, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //set first slot to 0x6212
    if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             1, BBH_SCU_1_EC_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID,
                             true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //fill in the rest of the slots with 0
    for (uint8_t i = 2; i <= 32; i++) {
        if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                                 i,
                                 0, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             33, BBH_SCU_1_EC_SLOT_33_SCU_AS_FSOE_SLAVE_CONNECTION,
                             true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             34, BBH_SCU_1_EC_SLOT_34_MASTER_TO_MASTER,
                             true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint32(slave, BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX,
                             35, BBH_SCU_1_EC_SLOT_35_FUNCTIONAL_IN_OUT, true)) {
        return E_SDO_WRITE_FAILURE;
    }

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

gberror_t ec_custom_fmmu_sm_bbh_scu_1_ec(const uint16_t slave) {
    ec_slave[slave].configindex = 999;

    ec_slave[slave].Ibits = 432;
    ec_slave[slave].Obits = 656;

    //todo crit
    // ec_slave[slave].FMMU[0].LogLength = 82;
    // ec_slave[slave].FMMU[1].LogLength = 54;


    ec_slave[slave].SM[2].SMlength = 82;
    ec_slave[slave].SM[3].SMlength = 54;

    return E_SUCCESS;
}

#define BBH_SCU_1_EC_FSOE_STATUS_WORD_BYTE_OFFSET 20

gberror_t ec_fose_get_master_state_bbh_scu_1_ec(const uint16_t slave) {

    //functional outputs

    /*
     * offset 20 = byte 0-1 status word
     */
//todo is this an input?
    return (ec_pdo_get_input_uint16(slave, BBH_SCU_1_EC_FSOE_STATUS_WORD_BYTE_OFFSET));


}

