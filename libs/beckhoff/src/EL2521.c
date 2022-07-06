
/**
 ******************************************************************************
 * @file           :  EL2521.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "EL2521.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"

cia_state_t el2521_cia_requested_state[MAP_NUM_DRIVES] = {CIA_SWITCH_ON_DISABLED};
cia_state_t el2521_cia_actual_state[MAP_NUM_DRIVES] = {CIA_SWITCH_ON_DISABLED};

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_el2521 = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_el2521 = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c13};


uint16_t map_SM2_index_of_assigned_PDO_el2521[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1600,
        0x1604};

uint16_t map_SM3_index_of_assigned_PDO_el2521[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a00,
        0x1a04};


gberror_t ec_pdo_map_el2521(const uint16_t slave) {

//    char fw_ver[2] = {0};
//
//    ec_sdo_read_uint16(slave, 0x100A, 0, &fw_ver);
//
//    printf("fw ver: %s\n", fw_ver);


    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "SODs configured for PDO mapping for EL2521 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to EL2521 slave [%u]",
                slave);
    }

    // map_SM2.SM_assignment_index

    if (!ec_sdo_write_uint8(slave, map_SM2_el2521.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, map_SM3_el2521.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_el2521.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_el2521.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_el2521[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_el2521.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_el2521.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_el2521[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint8(slave, map_SM2_el2521.SM_assignment_index, 0,
                             map_SM2_el2521.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, map_SM3_el2521.SM_assignment_index, 0,
                             map_SM3_el2521.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;


}

//0x8000:0xe uint8 - Operating mode - Value = 1 (freq cnt pulse mode)
#define EL2521_OPERATING_MODE_SDO_INDEX         0x8000
#define EL2521_OPERATING_MODE_SDO_SUB_INDEX     0x0e
#define EL2521_OPERATING_MODE_SDO_VALUE         1

gberror_t ec_standard_sdos_el2521(const uint16_t slave) {

    if (!ec_sdo_write_uint8(slave, EL2521_OPERATING_MODE_SDO_INDEX, EL2521_OPERATING_MODE_SDO_SUB_INDEX, EL2521_OPERATING_MODE_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }

return E_SUCCESS;

}


int8_t ec_get_moo_pdo_el2521(const uint16_t drive) {

    return CIA_MOO_CSP;
}

bool ec_get_remote_el2521(const uint16_t drive) {
    return true;
}


bool ec_get_follow_error_el2521(const uint16_t drive) {
    return false;
}


uint8_t *ec_get_error_string_sdo_el2521(const uint16_t drive) {

    static uint8_t empty_message[]="no string yet";

    return empty_message;
    //sync error
    //general error?
}

int32_t ec_get_actpos_wrd_el2521(const uint16_t drive) {
    return (int32_t) ec_pdo_get_input_uint32(map_drive_to_slave[drive], EL2521_ACTPOS_PDO_INDEX);
}

gberror_t ec_set_ctrl_wrd_el2521(const uint16_t drive, const uint16_t ctrlwrd) {
    uint8_t el7031_ctrlwrd = 0;

    switch (el2521_cia_actual_state[drive]) {
        case CIA_OPERATION_ENABLED:
            if (ctrlwrd == CIA_SHUTDOWN_CTRLWRD) {
                el2521_cia_requested_state[drive] = CIA_READY_TO_SWITCH_ON;
            }
            if (ctrlwrd == CIA_SWITCH_ON_CTRLWRD) {
                el2521_cia_requested_state[drive] = CIA_SWITCHED_ON;
            }
            if (ctrlwrd == CIA_QUICK_STOP_CTRLWRD) {
                el2521_cia_requested_state[drive] = CIA_QUICK_STOP_ACTIVE;
            }
            if (ctrlwrd == CIA_DISABLE_VOLTAGE_CTRLWRD) {
                el2521_cia_requested_state[drive] = CIA_SWITCH_ON_DISABLED;
            }
            break;
            case CIA_SWITCHED_ON:
                if (ctrlwrd == CIA_ENABLE_OPERATION_CTRLWRD) {
                    el2521_cia_requested_state[drive] = CIA_OPERATION_ENABLED;
//                    BIT_SET(el7031_ctrlwrd, EL7031_CONTROL_ENABLE_BIT_NUM);
                }
                if (ctrlwrd == CIA_SHUTDOWN_CTRLWRD) {
                    el2521_cia_requested_state[drive] = CIA_READY_TO_SWITCH_ON;
                }
                if (ctrlwrd == CIA_DISABLE_VOLTAGE_CTRLWRD) {
                    el2521_cia_requested_state[drive] = CIA_SWITCH_ON_DISABLED;
                }
                break;
                case CIA_SWITCH_ON_DISABLED:
                    if (ctrlwrd == CIA_SHUTDOWN_CTRLWRD) {
                        el2521_cia_requested_state[drive] = CIA_READY_TO_SWITCH_ON;
                    }
                    break;
                    case CIA_QUICK_STOP_ACTIVE:
                        break;
                    case CIA_READY_TO_SWITCH_ON:
                        if (ctrlwrd == CIA_DISABLE_VOLTAGE_CTRLWRD) {
                            el2521_cia_requested_state[drive] = CIA_SWITCH_ON_DISABLED;
                        }
                        if (ctrlwrd == CIA_SWITCH_ON_CTRLWRD) {
//                            BIT_SET(el7031_ctrlwrd, EL7031_CONTROL_ENABLE_BIT_NUM);
                            el2521_cia_requested_state[drive] = CIA_SWITCHED_ON;
                        }
                        break;
                        case CIA_NOT_READY_TO_SWITCH_ON:
                            el2521_cia_requested_state[drive] = CIA_SWITCH_ON_DISABLED;
                            break;
                            case CIA_FAULT_REACTION_ACTIVE:
                                el2521_cia_requested_state[drive] = CIA_FAULT;
                                break;
                                case CIA_FAULT:
                                    if (ctrlwrd == CIA_FAULT_RESET_CTRLWRD) {
                                        el2521_cia_requested_state[drive] = CIA_SWITCH_ON_DISABLED;
                                    }
                                    break;
    }


    ec_pdo_set_output_uint8(map_drive_to_slave[drive], EL2521_CONTROLWORD_PDO_INDEX, el7031_ctrlwrd);
    //    ec_pdo_set_output_uint8(map_drive_to_slave[drive], EL7031_QUICKSTOP_WORD_PDO_INDEX, el7031_quickstopwrd);


    return E_SUCCESS;
}


uint16_t ec_get_stat_wrd_el2521(const uint16_t drive) {

    uint8_t el2521_status = 0;


    el2521_status = ec_pdo_get_input_uint8(map_drive_to_slave[drive], EL2521_STATUSWORD_PDO_INDEX);
        printf("el7031_status: %u\n", el2521_status);

//
//    if (BIT_CHECK(el2521_status, EL7031_STATUS_ERROR_BIT_NUM)) {
//        el2521_drive_state[drive].error = true;
//    } else {
//        el2521_drive_state[drive].error = false;
//    }
//
//    if (BIT_CHECK(el7031_status, EL7031_STATUS_WARNING_BIT_NUM)) {
//        el2521_drive_state[drive].warning = true;
//    } else {
//        el2521_drive_state[drive].warning = false;
//    }



//    if (el2521_drive_state[drive].error) {
//        el2521_cia_requested_state[drive] = CIA_FAULT_REACTION_ACTIVE;
//        BIT_SET(el2521_control_word, EL7031_CONTROL_RESET_BIT_NUM);
//        ec_pdo_set_output_uint8(map_drive_to_slave[drive], EL7031_CONTROLWORD_PDO_INDEX, el7031_control_word);
//    }
//
//    if (el2521_cia_actual_state[drive] == CIA_FAULT_REACTION_ACTIVE) {
//        if (!el2521_drive_state[drive].error) {
//            el2521_cia_requested_state[drive] = CIA_FAULT;
//        }
//    }








    switch (el2521_cia_requested_state[drive]) {
        case CIA_OPERATION_ENABLED:
            if (el2521_cia_actual_state[drive] == CIA_OPERATION_ENABLED) {
                el2521_cia_actual_state[drive] = CIA_OPERATION_ENABLED;
            }
            if (el2521_cia_actual_state[drive] == CIA_SWITCHED_ON) {
//                if (el2521_drive_state[drive].ready && el2521_drive_state[drive].ready_to_enable) {
                    el2521_cia_actual_state[drive] = CIA_OPERATION_ENABLED;
//                }
            }
            break;
            case CIA_SWITCHED_ON:
                if (el2521_cia_actual_state[drive] == CIA_OPERATION_ENABLED) {
                    el2521_cia_actual_state[drive] = CIA_SWITCHED_ON;
                }
                if (el2521_cia_actual_state[drive] == CIA_READY_TO_SWITCH_ON) {
//                    if (el2521_drive_state[drive].ready && el2521_drive_state[drive].ready_to_enable) {
                        el2521_cia_actual_state[drive] = CIA_SWITCHED_ON;
//                    }
                }
                break;
                case CIA_SWITCH_ON_DISABLED:
                    if (el2521_cia_actual_state[drive] == CIA_READY_TO_SWITCH_ON) {
                        el2521_cia_actual_state[drive] = CIA_SWITCH_ON_DISABLED;
                    }
                    if (el2521_cia_actual_state[drive] == CIA_OPERATION_ENABLED) {
                        el2521_cia_actual_state[drive] = CIA_SWITCH_ON_DISABLED;
                    }
                    if (el2521_cia_actual_state[drive] == CIA_SWITCHED_ON) {
                        el2521_cia_actual_state[drive] = CIA_SWITCH_ON_DISABLED;
                    }
                    if (el2521_cia_actual_state[drive] == CIA_NOT_READY_TO_SWITCH_ON) {
                        el2521_cia_actual_state[drive] = CIA_SWITCH_ON_DISABLED;
                    }
                    if (el2521_cia_actual_state[drive] == CIA_FAULT) {
                        el2521_cia_actual_state[drive] = CIA_SWITCH_ON_DISABLED;
                    }
                    break;
                    case CIA_QUICK_STOP_ACTIVE:
                        break;
                    case CIA_READY_TO_SWITCH_ON:
                        if (el2521_cia_actual_state[drive] == CIA_SWITCH_ON_DISABLED) {
                            el2521_cia_actual_state[drive] = CIA_READY_TO_SWITCH_ON;
                        }
                        if (el2521_cia_actual_state[drive] == CIA_SWITCHED_ON) {
                            el2521_cia_actual_state[drive] = CIA_READY_TO_SWITCH_ON;
                        }
                        if (el2521_cia_actual_state[drive] == CIA_OPERATION_ENABLED) {
                            el2521_cia_actual_state[drive] = CIA_READY_TO_SWITCH_ON;
                        }
                        break;
                        case CIA_NOT_READY_TO_SWITCH_ON:
                            el2521_cia_actual_state[drive] = CIA_NOT_READY_TO_SWITCH_ON;
                            break;
                            case CIA_FAULT_REACTION_ACTIVE:
                                el2521_cia_actual_state[drive] = CIA_FAULT_REACTION_ACTIVE;
                                break;
                                case CIA_FAULT:
                                    if (el2521_cia_actual_state[drive] == CIA_FAULT_REACTION_ACTIVE) {
                                        el2521_cia_actual_state[drive] = CIA_FAULT;
                                    }
                                    break;


    }
    //    printf("actual state: %s\n", cia_state_names[el7031_cia_actual_state[drive]]);
    //    printf("requested state: %s\n", cia_state_names[el7031_cia_requested_state[drive]]);

    return cia_state_to_statwrd(el2521_cia_actual_state[drive]);
}

gberror_t ec_set_setpos_wrd_el2521(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_uint32(map_drive_to_slave[drive], EL2521_SETPOS_PDO_INDEX, (uint32) setpos);
    return E_SUCCESS;
}


gberror_t ec_print_params_el2521(const uint16_t drive) {
    return E_SUCCESS;
}