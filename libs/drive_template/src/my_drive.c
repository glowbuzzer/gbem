
/**
 ******************************************************************************
 * @file           :  my_drive.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */



#include "my_drive.h"
#include "ethercatsetget.h"

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_mydrive = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_mydrive = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c13};

/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_mydrive[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1600,
        0x1601};

/* This is used for the fixed PDO remapping */

uint16_t map_SM3_index_of_assigned_PDO_mydrive[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a03,
        0x1a04};


/**
 * @brief perform SDOs writes during boot to configure a slave
 * @param slave
 * @return gberror_t
 * @retval E_SUCCESS all ok
 */
gberror_t ec_standard_sdos_mydrive(const uint16_t slave) {

    ec_sdo_write_uint16()


    return E_SUCCESS;
}

/**
 * @brief sets fixed PDO mapping with a load of SDO writes
 * @return
 * @warning must be called in pre-op states
 *
 * @attention covers all drives attached to a controller
 */
gberror_t ec_pdo_map_azd3a_ked(const uint16_t slave) {


    if (!ec_sdo_write_uint16(slave, map_SM2_azd3a_ked.SM_assignment_index, 0, 0)) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: AZD SM2 assignment index write fail");
        return E_SDO_WRITE_FAILURE;
    }



    if (!ec_sdo_write_uint16(slave, map_SM3_azd3a_ked.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_azd3a_ked.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_azd3a_ked.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_azd3a_ked[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_azd.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_azd3a_ked.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_azd3a_ked[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_azd3a_ked.SM_assignment_index, 0,
                             map_SM2_azd3a_ked.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_azd3a_ked.SM_assignment_index, 0,
                             map_SM3_azd3a_ked.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

//all applied correctly
    return E_SUCCESS;
}


/**
 * @brief gets the mode of operation (MOO) for AZD drives
 * @param drive_number
 * @return  moo value (8 for CSP)
 * @attention handles sub-drives
 */
int8_t ec_get_moo_pdo_mydrive(const uint16_t drive) {
            return ec_pdo_get_input_int8(map_drive_to_slave[drive], AZD_MOODISP_PDO_DRIVE1_INDEX);

}


/**
 * @brief get the value of the remote bit from an AZD drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_azdxa_ked(const uint16_t drive) {

    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_mydrive(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}


bool ec_get_follow_error_azdxa_ked(const uint16_t drive) {
    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_mydrive(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_FOLLOW_ERROR_BIT_NUM)) {
        return true;
    }
    return false;
}


/**
 * @brief Perform first cycle PDO writes for AZD drives
 * @param slave - slave number to send PDO writes
 * @retval E_SUCCESS - Normal
 * @attention does PDO writes for all attached sub-drives
 */
gberror_t ec_initial_pdo_azdxa_ked(const uint16_t slave) {
                    ec_pdo_set_output_int8(slave, AZD_MOOSET_PDO_DRIVE1_INDEX, CIA_MOO_CSP);

}


/**
 * @brief reads drive error codes for a specific AZD drive and returns an array of error codes
 * @param drive
 * @return pointer to error code string
 */
uint8_t *ec_get_error_string_pdo_azdxa_ked(const uint16_t drive) {


    uint16_t drive_error_code = 0;
            drive_error_code = ec_pdo_get_input_uint16(map_drive_to_slave[drive], AZD_ERROR_CODE_PDO_DRIVE1_INDEX);
    return error_code_string;
}

/**
 * @brief Writes an SDO that triggers a config write to NVRAM for an AZD drive
 * @param drive_number
 * @return gberror
 * @attention Needed after a write of reverses or directions
 * @warning Writes the config for all sub-drives
 * @warning This will wear out the NVRAM if left enabled every boot!
 */
gberror_t ec_write_nvram_azdza_ked(const uint16_t slave) {

                    if (!ec_sdo_write_int8(slave, AZD_WRITE_CONFIG_SDO_DRIVE1_INDEX, AZD_WRITE_CONFIG_SDO_DRIVE1_SUB_INDEX,
                                           AZD_WRITE_CONFIG_SDO_DRIVE1_VALUE)) {
                        return E_SDO_WRITE_FAILURE;
                    }
    }
    return E_SUCCESS;
}


/**
 * @brief get actpos for an AZD drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actpos_wrd_azdxa_ked(const uint16_t drive) {
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZD_ACTPOS_PDO_DRIVE1_INDEX);

}

/**
 * @brief set control word for AZD drive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_azdxa_ked(const uint16_t drive, const uint16_t ctrlwrd) {

            ec_pdo_set_output_uint16(map_drive_to_slave[drive], AZD_CONTROLWORD_PDO_DRIVE1_INDEX, ctrlwrd);
    return E_SUCCESS;
}

/**
 * @brief set status word for AZD drive
 * @param drive
 * @return status word

 */
uint16_t ec_get_stat_wrd_azdza_ked(const uint16_t drive) {
            return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AZD_STATUSWORD_PDO_DRIVE1_INDEX);


    }
}

/**
 * @brief set setpos for an AZD drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setpos_wrd_azdxa_ked(const uint16_t drive, const int32_t setpos) {

            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZD_SETPOS_PDO_DRIVE1_INDEX, setpos);
    return E_SUCCESS;

}


/**
 * @brief set NVRAM SDOs for an AZD drive
 * @param slave
 * @return gberror
 * NVRAM sdos are things like limits and drive rotation direction that must be saved to NVRAM and the drive restarted before they take effect
 *
 */
gberror_t ec_nvram_sdos_azdxa_ked(const uint16_t slave) {

                    if (!ec_sdo_write_int32(slave, AZD_MIN_LIMIT_SDO_DRIVE1_INDEX,
                                            AZD_MIN_LIMIT_SDO_DRIVE1_SUB_INDEX, map_drive_neg_limit[i])) {

                    }
                    if (!ec_sdo_write_int32(slave, AZD_MAX_LIMIT_SDO_DRIVE2_INDEX,
                                            AZD_MAX_LIMIT_SDO_DRIVE2_SUB_INDEX, map_drive_pos_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZD_DIRECTION_SDO_DRIVE2_INDEX,
                                            AZD_DIRECTION_SDO_DRIVE2_SUB_INDEX, map_drive_direction[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
    gberror_t rc = ec_write_nvram_azd(slave);
    if (rc != E_SUCCESS) {

        return E_NVRAM_WRITE_FAILURE;
    } else {
        return E_SUCCESS;
    }
}
