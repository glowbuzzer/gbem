
/**
 ******************************************************************************
 * @file           :  EL2522.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "EL2522.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"

typedef struct {
    bool error;
    bool sync_error;
} el2522_drive_state_bits_t;

cia_state_t el2522_drive_state[MAP_NUM_DRIVES] = {CIA_SWITCH_ON_DISABLED};
el2522_drive_state_bits_t el2522_drive_state_bits[MAP_NUM_DRIVES] = {false};
uint16_t el2522_drive_ctrlwrd[MAP_NUM_DRIVES] = {0};


/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_el2522 = {
        .number_of_entries = 4,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_el2522 = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c13};


uint16_t map_SM2_index_of_assigned_PDO_el2522[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1601,
        0x1606,
        0x160b,
        0x160d};

uint16_t map_SM3_index_of_assigned_PDO_el2522[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a03,
        0x1a05};

gberror_t ec_pdo_map_el2522(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for EL2522 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to EL2522 slave [%u]",
                slave);
    }

    // map_SM2.SM_assignment_index

    if (!ec_sdo_write_uint16(slave, map_SM2_el2522.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_el2522.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_el2522.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_el2522.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_el2522[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_el2522.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_el2522.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_el2522[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_el2522.SM_assignment_index, 0,
                             map_SM2_el2522.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_el2522.SM_assignment_index, 0,
                             map_SM3_el2522.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }
    //all applied correctly
    return E_SUCCESS;
}


gberror_t ec_standard_sdos_el2522(const uint16_t slave) {

//    if (!ec_sdo_write_uint8(slave, EL2522_AUTO_DIR_CH1_SDO_INDEX, EL2522_AUTO_DIR_CH1_SDO_SUB_INDEX, EL2522_AUTO_DIR_CH1_VALUE)) {
//        return E_SDO_WRITE_FAILURE;
//    }

    if (!ec_sdo_write_uint8(slave, EL2522_OPERATION_MODE_CH1_SDO_INDEX, EL2522_OPERATION_MODE_CH1_SDO_SUB_INDEX,
                            EL2522_OPERATION_MODE_CH1_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, EL2522_OPERATION_MODE_CH2_SDO_INDEX, EL2522_OPERATION_MODE_CH2_SDO_SUB_INDEX,
                            EL2522_OPERATION_MODE_CH2_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;

}

gberror_t ec_initial_pdo_el2522(uint16_t slave) {

    ec_pdo_set_output_uint16(slave, EL2522_CTRLWRD_CH1_PDO_INDEX, 0b1000);

    ec_pdo_set_output_uint16(slave, EL2522_CTRLWRD_CH2_PDO_INDEX, 0b1000);

    return E_SUCCESS;

}

int8_t ec_get_moo_pdo_el2522(const uint16_t drive) {

    return CIA_MOO_CSP;
}

bool ec_get_remote_el2522(const uint16_t drive) {
    return true;
}


bool ec_get_follow_error_el2522(const uint16_t drive) {
    return false;
}


uint8_t *ec_get_error_string_sdo_el2522(const uint16_t drive) {

    return (uint8_t *) "EL2522: This device doesn't support error messages";

}

int32_t ec_get_actpos_wrd_el2522(const uint16_t drive) {

    switch (map_drive_to_subdrive[drive]) {
        case 1:
//            printf("act pos 1 %u\n", ec_pdo_get_input_uint32(map_drive_to_slave[drive], EL2522_ACTPOS_CH1_PDO_INDEX));
            return (int32_t) ec_pdo_get_input_uint32(map_drive_to_slave[drive], EL2522_ACTPOS_CH1_PDO_INDEX);
        case 2:
//            printf("act pos 2 %u\n", ec_pdo_get_input_uint32(map_drive_to_slave[drive], EL2522_ACTPOS_CH2_PDO_INDEX));
            return (int32_t) ec_pdo_get_input_uint32(map_drive_to_slave[drive], EL2522_ACTPOS_CH2_PDO_INDEX);
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            return 0;
    }
}

//need to sim state machine
void ec_sim_cia_el2522(const uint16_t drive, const uint16_t ctrlwrd, cia_state_t *state) {
    // if fault bit is set then we are always in fault reaction active
    if (el2522_drive_state_bits[drive].sync_error) {
        *state = CIA_FAULT_REACTION_ACTIVE;
        return;
    }


    switch (*state) {
        case CIA_FAULT_REACTION_ACTIVE:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_FAULT_RESET) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else {
                *state = CIA_FAULT_REACTION_ACTIVE;
            }

            break;
        case CIA_FAULT:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_FAULT_RESET) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else {
                *state = CIA_FAULT;
            }
            break;
        case CIA_NOT_READY_TO_SWITCH_ON:
            *state = CIA_SWITCH_ON_DISABLED;
            break;
        case CIA_SWITCH_ON_DISABLED:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else {
                *state = CIA_SWITCH_ON_DISABLED;
            }

            break;
        case CIA_READY_TO_SWITCH_ON:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SWITCH_ON) {
                *state = CIA_SWITCHED_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else {
                *state = CIA_READY_TO_SWITCH_ON;
            }
            break;
        case CIA_SWITCHED_ON:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_ENABLE_OPERATION) {
                *state = CIA_OPERATION_ENABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else {
                *state = CIA_SWITCHED_ON;
            }

            break;
        case CIA_OPERATION_ENABLED:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_OPERATION) {
                *state = CIA_SWITCHED_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_QUICK_STOP) {
                *state = CIA_QUICK_STOP_ACTIVE;
            } else {
                *state = CIA_OPERATION_ENABLED;
            }

            break;
        case CIA_QUICK_STOP_ACTIVE:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_ENABLE_OPERATION) {
                *state = CIA_OPERATION_ENABLED;
            } else {
                *state = CIA_QUICK_STOP_ACTIVE;
            }
            break;
        default:
            break;
    }
}


gberror_t ec_set_ctrl_wrd_el2522(const uint16_t drive, const uint16_t ctrlwrd) {
    el2522_drive_ctrlwrd[drive] = ctrlwrd;
    return E_SUCCESS;
}


/**
 * @brief EL2522 - decodes a status word into a struct
 * @param statusword (uint16_t)
 * @return el2522_drive_state_t
 */
el2522_drive_state_bits_t ec_decode_drive_state_el2522(uint16_t statusword) {

    el2522_drive_state_bits_t state = {0};

    state.sync_error = BIT_CHECK(statusword, EL2522_STATUS_SYNC_ERROR_BIT_NUM) ? true : false;
    return state;
}


uint16_t ec_get_stat_wrd_el2522(const uint16_t drive) {
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            el2522_drive_state_bits[drive] = ec_decode_drive_state_el2522(
                    ec_pdo_get_input_uint16(map_drive_to_slave[drive], EL2522_STATUSWORD_CH1_PDO_INDEX));
            break;
        case 2:
            el2522_drive_state_bits[drive] = ec_decode_drive_state_el2522(
                    ec_pdo_get_input_uint16(map_drive_to_slave[drive], EL2522_STATUSWORD_CH2_PDO_INDEX));
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
    }


    ec_sim_cia_el2522(drive, el2522_drive_ctrlwrd[drive], &el2522_drive_state[drive]);

    return cia_state_to_statwrd(el2522_drive_state[drive]);

}

gberror_t ec_set_setpos_wrd_el2522(const uint16_t drive, int32_t setpos) {

    switch (map_drive_to_subdrive[drive]) {
        case 1:
//            printf("setpos for drive 1 %u\n", setpos );
            ec_pdo_set_output_uint32(map_drive_to_slave[drive], EL2522_SETPOS_CH1_PDO_INDEX, (uint32) setpos);
            break;
        case 2:
//            printf("setpos for drive 2 %u\n", setpos );
            ec_pdo_set_output_uint32(map_drive_to_slave[drive], EL2522_SETPOS_CH2_PDO_INDEX, (uint32) setpos);
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            return E_SUB_DRIVE_OUT_OF_RANGE;
    }
    return E_SUCCESS;
}


gberror_t ec_print_params_el2522(const uint16_t drive) {
    return E_SUCCESS;
}