/**
 ******************************************************************************
 * @file           :  map_config_custom_pdo
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "map_config_custom_pdo.h"


uint32_t map_calc_index_length(ec_datatype datatype, uint16_t index, uint8_t offset) {
    uint16_t length = 0;
    switch (datatype) {
        case ECT_INTEGER8:
            length = 0x8;
            break;
        case ECT_INTEGER16:
            length = 0x10;
            break;
        case ECT_INTEGER32:
            length = 0x20;
            break;
        case ECT_INTEGER64:
            length = 0x40;
            break;
        case ECT_UNSIGNED8:
            length = 0x8;
            break;
        case ECT_UNSIGNED16:
            length = 0x10;
            break;
        case ECT_UNSIGNED32:
            length = 0x20;
            break;
        case ECT_UNSIGNED64:
            length = 0x40;
            break;
        case ECT_REAL32:
            length = 0x20;
            break;
        case ECT_REAL64:
            length = 0x40;
            break;
        // case ECT_STRING8:
        //     length = 0x80;
        // break;
        default:
            UM_FATAL("GBEM: Invalid custom pdo map datatype");
    }

    uint16_t offset_length = ((uint16_t) offset << 8) | length;
    uint32_t combinedValue = ((uint32_t) index << 16) | offset_length;

    return combinedValue;
}


gberror_t map_apply_custom_pdo_mapping(const uint16_t slave, map_custom_pdo_t map) {
    //    printf("map_apply_custom_pdo_mapping for slave [%d]\n", slave);


    /* clear SM assignment objects */

    if (!ec_sdo_write_uint16(slave, map.sm2_assignment_object, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map.sm3_assignment_object, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //find number of PDOs

    if (map.num_sm2_assignments == 0) {
        LL_FATAL("GBEM: map.num_sm2_assignments == 0 ");
    }

    if (map.num_sm3_assignments == 0) {
        LL_FATAL("GBEM: map.num_sm3_assignments == 0 ");
    }


    uint8_t rxpdo_count = map.num_sm2_assignments;
    uint8_t txpdo_count = map.num_sm3_assignments;


    /* set SM assignment objects contents*/

    for (int i = 0; i < rxpdo_count; i++) {
        if (!ec_sdo_write_uint16(slave, map.sm2_assignment_object, i + 1,
                                 map.rxpdo[i].pdo_assignment_object, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < txpdo_count; i++) {
        if (!ec_sdo_write_uint16(slave, map.sm3_assignment_object, i + 1,
                                 map.txpdo[i].pdo_assignment_object, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /* set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0} */
    if (!ec_sdo_write_uint16(slave, map.sm2_assignment_object, 0,
                             rxpdo_count, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map.sm3_assignment_object, 0,
                             txpdo_count, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    /* clear PDO assignment objects */

    for (int i = 0; i < rxpdo_count; i++) {
        int number_of_entries = sizeof(map.rxpdo[i].assignments) / sizeof(map.rxpdo[i].assignments[0]);

        if (map.rxpdo[i].num_assignments == 0) {

            continue;
        }

        if (!ec_sdo_write_uint8(slave, map.rxpdo[i].pdo_assignment_object, 0, 0, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }
    for (int i = 0; i < txpdo_count; i++) {
        int number_of_entries = sizeof(map.txpdo[i].assignments) / sizeof(map.txpdo[i].assignments[0]);

        if (map.txpdo[i].num_assignments == 0) {

            continue;
        }

        if (!ec_sdo_write_uint8(slave, map.txpdo[i].pdo_assignment_object, 0, 0, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /* set PDO assignment objects contents */

    for (int i = 0; i < rxpdo_count; i++) {
        if (map.rxpdo[i].num_assignments == 0) {
            continue;
        }

        uint8_t number_of_entries = map.rxpdo[i].num_assignments;

        for (int j = 0; j < number_of_entries; j++) {
            if (!ec_sdo_write_uint32(slave, map.rxpdo[i].pdo_assignment_object, j + 1,
                                     map_calc_index_length(map.rxpdo[i].assignments[j].datatype,
                                                           map.rxpdo[i].assignments[j].index,
                                                           map.rxpdo[i].assignments[j].offset), true)) {
                return E_SDO_WRITE_FAILURE;
            }
        }
    }

    for (int i = 0; i < txpdo_count; i++) {
        if (map.txpdo[i].num_assignments == 0) {
            continue;
        }

        int number_of_entries = map.txpdo[i].num_assignments;

        for (int j = 0; j < number_of_entries; j++) {
            if (!ec_sdo_write_uint32(slave, map.txpdo[i].pdo_assignment_object, j + 1,
                                     map_calc_index_length(map.txpdo[i].assignments[j].datatype,
                                                           map.txpdo[i].assignments[j].index,
                                                           map.txpdo[i].assignments[j].offset), true)) {
                return E_SDO_WRITE_FAILURE;
            }
        }
    }

    /** set the PDO assignment object number of entries to actual number (sub-index 0} */

    for (int i = 0; i < rxpdo_count; i++) {
        if (map.rxpdo[i].num_assignments == 0) {
            continue;
        }
        int number_of_entries = map.rxpdo[i].num_assignments;

        if (!ec_sdo_write_uint8(slave, map.rxpdo[i].pdo_assignment_object, 0,
                                number_of_entries, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < txpdo_count; i++) {
        if (map.txpdo[i].num_assignments == 0) {
            continue;
        }

        int number_of_entries = map.txpdo[i].num_assignments;

        if (!ec_sdo_write_uint8(slave, map.txpdo[i].pdo_assignment_object, 0,
                                number_of_entries, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    return E_SUCCESS;
}


//PDO mapping
//RxPDO – device receives this process data
//0x1600:00
//10 slots
//0x1601:00
//10 slots
//0x1602:00
//10 slots
//        TxPDO – device sends this process data
//0x1A00:00
//10 slots
//0x1a01
//10 slots
//0x1A02:00
//10 slots


