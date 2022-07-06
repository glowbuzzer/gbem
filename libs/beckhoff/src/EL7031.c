
/**
 ******************************************************************************
 * @file           :  EL7031.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "EL7031.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"

typedef struct {
    bool error;
    bool warning;
    bool ready;
    bool ready_to_enable;
    bool moving_positive;
    bool moving_negative;
    bool torque_reduced;
    bool motor_stall;
    bool sync_error;
} el7031_drive_state_bits_t;

cia_state_t el7031_drive_state[MAP_NUM_DRIVES] = {CIA_SWITCH_ON_DISABLED};
el7031_drive_state_bits_t el7031_drive_state_bits[MAP_NUM_DRIVES] = {false};
uint16_t el7031_drive_ctrlwrd[MAP_NUM_DRIVES] = {0};


/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_el7031 = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_el7031 = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c13};


uint16_t map_SM2_index_of_assigned_PDO_el7031[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1602,
        0x1603};

uint16_t map_SM3_index_of_assigned_PDO_el7031[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a03,
        0x1a07};

gberror_t ec_pdo_map_el7031(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "SODs configured for PDO mapping for EL7031 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to EL7031 slave [%u]",
                slave);
    }

    // map_SM2.SM_assignment_index

    if (!ec_sdo_write_uint16(slave, map_SM2_el7031.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }


    if (!ec_sdo_write_uint16(slave, map_SM3_el7031.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_el7031.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_el7031.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_el7031[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_el7031.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_el7031.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_el7031[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_el7031.SM_assignment_index, 0,
                             map_SM2_el7031.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_el7031.SM_assignment_index, 0,
                             map_SM3_el7031.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;


}




gberror_t ec_standard_sdos_el7031(const uint16_t slave) {

    if (!ec_sdo_write_uint16(slave, EL7031_MAX_CURRENT_SDO_INDEX, EL7031_MAX_CURRENT_SDO_SUB_INDEX, EL7031_MAX_CURRENT_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, EL7031_REDUCED_CURRENT_SDO_INDEX, EL7031_REDUCED_CURRENT_SDO_SUB_INDEX, EL7031_REDUCED_CURRENT_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, EL7031_NOMINAL_VOLTAGE_SDO_INDEX, EL7031_NOMINAL_VOLTAGE_SDO_SUB_INDEX, EL7031_NOMINAL_VOLTAGE_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, EL7031_MOTOR_COIL_RESISTANCE_SDO_INDEX, EL7031_MOTOR_COIL_RESISTANCE_SDO_SUB_INDEX, EL7031_MOTOR_COIL_RESISTANCE_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, EL7031_MOTOR_FULL_STEPS_SDO_INDEX, EL7031_MOTOR_FULL_STEPS_SDO_SUB_INDEX, EL7031_MOTOR_FULL_STEPS_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, EL7031_OPERATION_MODE_SDO_INDEX, EL7031_OPERATION_MODE_SDO_SUB_INDEX, EL7031_OPERATION_MODE_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, EL7031_SPEED_RANGE_SDO_INDEX, EL7031_SPEED_RANGE_SDO_SUB_INDEX, EL7031_SPEED_RANGE_SDO_VALUE)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, EL7031_INVERT_MOTOR_POLARITY_SDO_INDEX, EL7031_INVERT_MOTOR_POLARITY_SDO_SUB_INDEX, map_drive_direction[slave - 1])) {
        return E_SDO_WRITE_FAILURE;
    }
    //all applied correctly
    return E_SUCCESS;

}

int8_t ec_get_moo_pdo_el7031(const uint16_t drive) {

    return CIA_MOO_CSP;
}

bool ec_get_remote_el7031(const uint16_t drive) {
    return true;
}


bool ec_get_follow_error_el7031(const uint16_t drive) {
    return false;
}



uint8_t *ec_get_error_string_sdo_el7031(const uint16_t drive) {


    uint64_t read_val =0;
    int  size = EL7031_DIAG_SIZE; //48bits
    //read in complete access mode
    int rc = ec_SDOread(map_drive_to_slave[drive], EL7031_DIAG_SDO_INDEX, EL7031_DIAG_SDO_SUB_INDEX, true, &size, &read_val,  EC_TIMEOUTRXM);

    if (rc <= 0) {
        return (uint8_t*)"Can't read EL7031 error code from SDO";
    }

    if(BIT_CHECK(read_val, EL7031_ERROR_SATURATED_BIT_NUM)){
        return (uint8_t *) "EL7031: Saturated error";
    }
    if(BIT_CHECK(read_val, EL7031_ERROR_OVER_TEMPERATURE_BIT_NUM)){
        return (uint8_t *) "EL7031: Over temperature error";
    }

    if(BIT_CHECK(read_val, EL7031_ERROR_TORQUE_OVERLOAD_BIT_NUM)){
        return (uint8_t *) "EL7031: Torque overload error";
    }

    if(BIT_CHECK(read_val, EL7031_ERROR_UNDER_VOLTAGE_BIT_NUM)){
        return (uint8_t *) "EL7031: Under voltage error";
    }
    if(BIT_CHECK(read_val, EL7031_ERROR_OVER_VOLTAGE_BIT_NUM)){
        return (uint8_t *) "EL7031: Over volatge error";
    }

    if(BIT_CHECK(read_val, EL7031_ERROR_SHORT_CIRCUIT_A_BIT_NUM)){
        return (uint8_t *) "EL7031: Short circuit A error";
    }

    if(BIT_CHECK(read_val, EL7031_ERROR_SHORT_CIRCUIT_B_BIT_NUM)){
        return (uint8_t *) "EL7031: Short circuit B error";
    }

    if(BIT_CHECK(read_val, EL7031_ERROR_NO_CONTROL_POWER_BIT_NUM)){
        return (uint8_t *) "EL7031: No control power error";
    }

    if(BIT_CHECK(read_val, EL7031_ERROR_MISC_ERROR_BIT_NUM)){
        return (uint8_t *) "EL7031: Misc error";
    }

    if(BIT_CHECK(read_val, EL7031_ERROR_CONFIG_ERROR_BIT_NUM)){
        return (uint8_t *) "EL7031: Config error";
    }

    return (uint8_t *) "EL7031: Unknown error";


}

int32_t ec_get_actpos_wrd_el7031(const uint16_t drive) {
    return (int32_t) ec_pdo_get_input_uint32(map_drive_to_slave[drive], EL7031_ACTPOS_PDO_INDEX);
}

void ec_sim_cia_el7031(const uint16_t drive, const uint16_t ctrlwrd, cia_state_t * state) {
    // if fault bit is set then we are always in fault reaction active
    if (el7031_drive_state_bits[drive].error ){
        *state = CIA_FAULT_REACTION_ACTIVE;
        return;
    }

    if (el7031_drive_state_bits[drive].sync_error){
        printf("sync error");
    }

    //if we don't have a ready to enable bit, state is always Not Ready To Switch On
    if(!el7031_drive_state_bits[drive].ready_to_enable){
        *state = CIA_NOT_READY_TO_SWITCH_ON;
        return;
    }

    switch (*state) {
        case CIA_FAULT_REACTION_ACTIVE:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_FAULT_RESET) {
                *state = CIA_SWITCH_ON_DISABLED;
            }else{
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
                    if (el7031_drive_state_bits[drive].ready_to_enable){
                        *state = CIA_SWITCH_ON_DISABLED;
                    } else {
                        *state = CIA_NOT_READY_TO_SWITCH_ON;
                    }
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
                                        *state= CIA_SWITCHED_ON;
                                    } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                                        *state = CIA_READY_TO_SWITCH_ON;
                                    } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_QUICK_STOP) {
                                        *state = CIA_QUICK_STOP_ACTIVE;
                                    } else {
                                        *state= CIA_OPERATION_ENABLED;
                                    }

                                    //if we are in Operation enabled and lose the ready bit on the drive ?
                                    if (!el7031_drive_state_bits[drive].ready){
                                        printf("B\n");
                                        *state = CIA_FAULT_REACTION_ACTIVE;
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




#define EL7031_CONTROL_ENABLE_BIT_NUM 0
#define EL7031_CONTROL_RESET_BIT_NUM 1


gberror_t ec_set_ctrl_wrd_el7031(const uint16_t drive, const uint16_t ctrlwrd) {
    el7031_drive_ctrlwrd[drive] = ctrlwrd;

    return E_SUCCESS;

}


/**
 * @brief EL7031 - decodes a status word into a struct
 * @param statusword (uint16_t)
 * @return el7031_drive_state_t
 */
el7031_drive_state_bits_t ec_decode_drive_state_el7031(uint16_t statusword) {

    el7031_drive_state_bits_t state = {0};

    state.error = BIT_CHECK(statusword, EL7031_STATUS_ERROR_BIT_NUM) ? true : false;
    state.warning = BIT_CHECK(statusword, EL7031_STATUS_WARNING_BIT_NUM) ? true : false;
    state.ready_to_enable = BIT_CHECK(statusword, EL7031_STATUS_READY_TO_ENABLE_BIT_NUM) ? true : false;
    state.ready = BIT_CHECK(statusword, EL7031_STATUS_READY_BIT_NUM) ? true : false;
    state.moving_positive = BIT_CHECK(statusword, EL7031_STATUS_MOVING_POS_BIT_NUM) ? true : false;
    state.moving_negative = BIT_CHECK(statusword, EL7031_STATUS_MOVING_NEG_BIT_NUM) ? true : false;
    state.torque_reduced = BIT_CHECK(statusword, EL7031_STATUS_TORQUE_REDUCED_BIT_NUM) ? true : false;
//    state.motor_stall = BIT_CHECK(statusword, EL7031_STATUS_MOTOR_STALL_BIT_NUM) ? true : false;
    state.sync_error = BIT_CHECK(statusword, EL7031_STATUS_SYNC_ERROR_BIT_NUM) ? true : false;
    return state;
}




uint16_t ec_get_stat_wrd_el7031(const uint16_t drive) {
    uint8_t ctrlwrd = 0;

    el7031_drive_state_bits[drive] = ec_decode_drive_state_el7031(ec_pdo_get_input_uint16(map_drive_to_slave[drive], EL7031_STATUSWORD_PDO_INDEX));

    ec_sim_cia_el7031(drive, el7031_drive_ctrlwrd[drive], &el7031_drive_state[drive]);


    if (el7031_drive_state[drive] == CIA_FAULT_REACTION_ACTIVE){
        BIT_SET(ctrlwrd, EL7031_CONTROL_RESET_BIT_NUM);
    }

    if (el7031_drive_state[drive] == CIA_SWITCHED_ON || el7031_drive_state[drive] == CIA_READY_TO_SWITCH_ON || el7031_drive_state[drive] == CIA_OPERATION_ENABLED ){
        BIT_SET(ctrlwrd, EL7031_CONTROL_ENABLE_BIT_NUM);
    }

    ec_pdo_set_output_uint8(map_drive_to_slave[drive], EL7031_CONTROLWORD_PDO_INDEX, ctrlwrd);

    return cia_state_to_statwrd(el7031_drive_state[drive]);


}

gberror_t ec_set_setpos_wrd_el7031(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_uint32(map_drive_to_slave[drive], EL7031_SETPOS_PDO_INDEX, (uint32) setpos);
    return E_SUCCESS;
}


gberror_t ec_print_params_el7031(const uint16_t drive) {
    return E_SUCCESS;
}