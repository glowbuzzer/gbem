
/**
 ******************************************************************************
 * @file           :  MSE-408.c
 * @brief          :  MSE-408 EtherCAT FLEX-Drive
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */



#include "MSE-408.h"
#include "RTA.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_mse_408 = {
        .number_of_entries = 1,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_mse_408 = {
        .number_of_entries = 1,
        .SM_assignment_index = 0x1c13};


/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_mse_408[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1601,
};

/* This is used for the fixed PDO remapping */
uint16_t map_SM3_index_of_assigned_PDO_mse_408[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a01,
};



/** this is your standard sdos needed to config a slave applied during the boot process */
/**
 * @brief perform SDOs writes during boot to configure a slave
 * @param slave
 * @return gberror_t
 * @retval E_SUCCESS all ok
 */
gberror_t ec_apply_standard_sdos_mse_408(const uint16_t slave) {

    if (nolimits) {
        if (!ec_sdo_write_int32(slave, MSE_408_MAX_POSITION_LIMIT_SDO_INDEX,
                                MSE_408_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                                0)) {
            return E_SDO_WRITE_FAILURE;
        }

        if (!ec_sdo_write_int32(slave, MSE_408_MIN_POSITION_LIMIT_SDO_INDEX,
                                MSE_408_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                0)) {
            return E_SDO_WRITE_FAILURE;
        }
    } else {
        if (!ec_sdo_write_int32(slave, MSE_408_MAX_POSITION_LIMIT_SDO_INDEX,
                                MSE_408_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                                map_drive_pos_limit[map_slave_to_drive(slave)])) {
            return E_SDO_WRITE_FAILURE;
        }

        if (!ec_sdo_write_int32(slave, MSE_408_MIN_POSITION_LIMIT_SDO_INDEX,
                                MSE_408_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                map_drive_neg_limit[map_slave_to_drive(slave)])) {
            return E_SDO_WRITE_FAILURE;
        }
    }


    uint8_t polarity = 0;
    if (map_drive_direction[map_slave_to_drive(slave)] == 1) {
        polarity = 1;
    }

    if (!ec_sdo_write_uint8(slave, MSE_408_DIRECTION_SDO_INDEX, MSE_408_DIRECTION_SDO_SUB_INDEX,
                            polarity)) {
        return E_SDO_WRITE_FAILURE;
    }

//Max Motor Speed 0x6080 0 U32 RW 1 300 000 0…800 000
    if (!ec_sdo_write_uint32(slave, MSE_408_MAX_MOTOR_SPEED_SDO_INDEX,
                             MSE_408_MAX_MOTOR_SPEED_SDO_SUB_INDEX,
                             MSE_408_MAX_MOTOR_SPEED)) {
        return E_SDO_WRITE_FAILURE;
    }

//Homing Speeds - Speed during Search for Switch - 0x6099 1 U32 RW 1 0x5DC0 (24000) 16001…400 000
    if (!ec_sdo_write_uint32(slave, MSE_408_HOMING_SPEEDS_SEARCH_FOR_SWITCH_SDO_INDEX,
                             MSE_408_HOMING_SPEEDS_SEARCH_FOR_SWITCH_SDO_SUB_INDEX,
                             MSE_408_HOMING_SPEED_SEARCH_FOR_SWITCH)) {
        return E_SDO_WRITE_FAILURE;
    }


//Homing Speeds - Speed during Search for Zero - 0x6099 2 U16 or U32 RW 1 0x0C80 (3200) 0… 16000
    if (!ec_sdo_write_uint32(slave, MSE_408_HOMING_SPEEDS_SEARCH_FOR_ZERO_SDO_INDEX,
                             MSE_408_HOMING_SPEEDS_SEARCH_FOR_ZERO_SDO_SUB_INDEX,
                             MSE_408_HOMING_SPEED_SEARCH_FOR_ZERO)) {
        return E_SDO_WRITE_FAILURE;
    }

//Homing Acceleration 0x609A 0 U32 RW 1 0x9C400 (640000) 2000…10 000 000

    if (!ec_sdo_write_uint32(slave, MSE_408_HOMING_ACCELERATION_SDO_INDEX,
                             MSE_408_HOMING_ACCELERATION_SDO_SUB_INDEX,
                             MSE_408_HOMING_ACCELERATION)) {
        return E_SDO_WRITE_FAILURE;
    }


//Motor Code 0x3210 0 U32 RW RO 1,2 MSE-408: 0x00001544 (5444)

    if (!ec_sdo_write_uint32(slave, MSE_408_MOTOR_CODE_SDO_INDEX,
                             MSE_408_MAX_MOTOR_SPEED_SDO_SUB_INDEX,
                             MSE_408_MOTOR_CODE)) {
        return E_SDO_WRITE_FAILURE;
    }


    return E_SUCCESS;
}


/**
 * @brief sets fixed PDO mapping with a load of SDO writes for MSE-408 drive
 * @return
 * @warning must be called in pre-op states
 *
 * @attention covers all drives attached to a controller
 */
gberror_t ec_pdo_map_mse_408(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for MSE-408 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to MSE-408 slave [%u]",
                slave);
    }

    if (!ec_sdo_write_uint8(slave, map_SM2_mse_408.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, map_SM3_mse_408.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_mse_408.number_of_entries; i++) {
        if (!ec_sdo_write_uint8(slave, map_SM2_mse_408.SM_assignment_index, i + 1,
                                map_SM2_index_of_assigned_PDO_mse_408[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_mse_408.number_of_entries; i++) {
        if (!ec_sdo_write_uint8(slave, map_SM3_mse_408.SM_assignment_index, i + 1,
                                map_SM3_index_of_assigned_PDO_mse_408[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_mse_408.SM_assignment_index, 0,
                             map_SM2_mse_408.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_mse_408.SM_assignment_index, 0,
                             map_SM3_mse_408.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;


}


/**
 * @brief gets the mode of operation (MOO) for MSE-408 drives
 * @param drive_number
 * @return  moo value (8 for CSP)
 * @attention handles sub-drives
 */
int8_t ec_get_moo_pdo_mse_408(const uint16_t drive) {

    return ec_pdo_get_input_int8(map_drive_to_slave[drive], MSE_408_MOODISP_PDO_INDEX);

}


/**
 * @brief get the value of the remote bit from an MSE-408 drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_mse_408(const uint16_t drive) {

    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_mse_408(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}


bool ec_get_follow_error_mse_408(const uint16_t drive) {
    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_mse_408(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_FOLLOW_ERROR_BIT_NUM)) {
        return true;
    }
    return false;
}


/**
 * @brief Perform first cycle PDO writes for MSE-408 drives
 * @param slave - slave number to send PDO writes
 * @retval E_SUCCESS - Normal
 * @attention does PDO writes for all attached sub-drives
 */
gberror_t ec_initial_pdo_mse_408(const uint16_t slave) {

    ec_pdo_set_output_int8(slave, MSE_408_MOOSET_PDO_INDEX, CIA_MOO_CSP);

    LL_INFO(GBEM_GEN_LOG_EN,
            "GBEM: Applying initial PDO writes for MSE-408 drive slave [%u], offset [%u], value [%u]", slave,
            MSE_408_MOOSET_PDO_INDEX, CIA_MOO_CSP);

    if (ec_iserror()) {
        LL_ERROR(GBEM_GEN_LOG_EN,
                 "GBEM: EtherCAT error detected after initial PDO writes: %s", ec_elist2string());
        return E_ETHERCAT_ERROR_DETECTED;
    }

    return E_SUCCESS;
}


uint8_t *ec_get_error_string_sdo_mse_408(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint16_t drive_error_code = 0;


    if (ec_sdo_read_uint16(map_drive_to_slave[drive], MSE_408_ERROR_CODE_SDO_INDEX,
                           MSE_408_ERROR_CODE_SDO_SUB_INDEX,
                           &drive_error_code)) {

        if (drive_error_code == 0) {
            sprintf((char *) error_code_string, "MSE-408: no error on drive");
            return error_code_string;
        }

        for (int i = 0; i < NUM_OF_MSE_408_ERROR_STRINGS; i++) {
            if (mse_408_error_code[i].error_id == drive_error_code) {
                sprintf((char *) error_code_string, "MSE-408 error [%s]",
                        mse_408_error_code[i].text_string);
                return error_code_string;
            }
        }

        sprintf((char *) error_code_string,
                "MSE-408: Error code returned by drive did not match any in the error table");
        return error_code_string;
    }

    sprintf((char *) error_code_string,
            "MSE-408: Error code could not be read from drive");
    return error_code_string;
}

/**
 * @brief get actpos for an MSE-408 drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actpos_wrd_mse_408(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], MSE_408_ACTPOS_PDO_INDEX);
}


/**
 * @brief set status word for an MSE-408 drive
 * @param drive
 * @param statwrd
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_stat_wrd_rev_mse_408(const uint16_t drive, const uint16_t statwrd) {
    ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], MSE_408_STATUSWORD_PDO_INDEX, statwrd);
    return E_SUCCESS;
}

/**
 * @brief set control word for MSE-408 drive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_mse_408(const uint16_t drive, const uint16_t ctrlwrd) {

    ec_pdo_set_output_uint16(map_drive_to_slave[drive], MSE_408_CONTROLWORD_PDO_INDEX, ctrlwrd);

    return E_SUCCESS;
}

/**
 * @brief set status word for MSE-408 drive
 * @param drive
 * @return status word

 */
uint16_t ec_get_stat_wrd_mse_408(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], MSE_408_STATUSWORD_PDO_INDEX);

}

/**
 * @brief set setpos for an MSE-408 drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setpos_wrd_mse_408(const uint16_t drive, const int32_t setpos) {

    ec_pdo_set_output_int32(map_drive_to_slave[drive], MSE_408_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}


/**
 * @brief get ctrlwrd for for an MSE-408 drive
 * @param drive
 * @return ctrlwrd
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
uint16_t ec_get_ctrl_wrd_rev_mse_408(const uint16_t drive) {
    return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], MSE_408_CONTROLWORD_PDO_INDEX);

}


/**
 * @brief set actpos for an MSE-408 drive
 * @param drive
 * @param actpos
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_actpos_wrd_rev_mse_408(const uint16_t drive, const int32_t actpos) {
    ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], MSE_408_ACTPOS_PDO_INDEX, actpos);
    return E_SUCCESS;
}

/**
 * @brief get setpos for an MSE-408 drive
 * @param drive
 * @return setpos
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
int32_t ec_get_setpos_word_rev_mse_408(const uint16_t drive) {
    return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], MSE_408_SETPOS_PDO_INDEX);

}


/**
 * @brief set moo for an MSE-408 drive
 * @param drive
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_moo_pdo_rev_mse_408(const uint16_t drive) {
    ec_pdo_set_input_int8_rev(map_drive_to_slave[drive], MSE_408_MOODISP_PDO_INDEX, CIA_MOO_CSP);
    return E_SUCCESS;
}


/*array mapping an MSE-408 drive error code to a text string */
const mse_408_error_string_t mse_408_error_code[NUM_OF_MSE_408_ERROR_STRINGS] = {
        {0x8611, "Motor following error"},
        {0x8500, "Error encoder short circuit” (or overcurrent on encoder)"},
        {0x8400, "Axis speed too high” (see object 0x3321, subindex 4)"},
        {0x5100, "Error power supply out of range"},
        {0x4310, "Error drive excessive temperature"},
        {0x2130, "Error short circuit” (or overcurrent on motor phase)"},
        {0x0000, "Emergency end"},
};
