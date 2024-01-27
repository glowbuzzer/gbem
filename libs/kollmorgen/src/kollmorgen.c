/**
 ******************************************************************************
 * @file           :  kollmorgen.c
 * @brief          :  kollmorgen drive functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "kollmorgen.h"
#include "log.h"
#include "std_defs_and_macros.h"
#include "ethercatsetget.h"
#include "akd_error_code.h"
#include "cia402.h"
#include "gberror.h"
#include "user_message.h"


/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_akd = {
    .number_of_entries = 1,
    .SM_assignment_index = 0x1c12
};

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM3_akd = {
    .number_of_entries = 1,
    .SM_assignment_index = 0x1c13
};

/* This is used for the fixed POO remapping */
/*0x1724 Target position for cyclic synchronous position mode (4 bytes - DINT), Control word (2byte), Torque feed forward (2 bytes) */
uint16_t map_SM2_index_of_assigned_PDO_akd[ECM_MAX_PDO_MAPPING_ENTRIES] = {
    0x1724
};

/* This is used for the fixed POO remapping */
/*
 * 0x1B20 - This predefined PDO mapping for inputs includes:
 * Position actual internal value ( 4 bytes), 2nd position feedback position (4 bytes),
 * velocity actual value (4 bytes), digital inputs (4 bytes), following error (4 bytes),
 * latch position positive (4 bytes), status word (2 bytes), torque actual value (2bytes),
 * latch status (2 bytes), analog input value (2 bytes)
*/
//uint16_t map_SM3_index_of_assigned_PDO_akd[ECM_MAX_PDO_MAPPING_ENTRIES] = {
//        0x1b20};


/*0x1b26 - This predefined PDO mapping is the ds402 scaled actpos version (later AZD firmware only). It includeS:
 * status word (2 bytes), Position actual internal value ( 4 bytes),torque actual value (2bytes),digital inputs (4 bytes),
 *  analog input value (2 bytes), follow error actual value (2 bytes)
 *
*/
uint16_t map_SM3_index_of_assigned_PDO_akd[ECM_MAX_PDO_MAPPING_ENTRIES] = {
    0x1b26
};

/**
 * @brief applies the standard SDOs needed by AKD drive
 * @param slave
 * @return
 */
gberror_t ec_standard_sdos_akd(const uint16_t slave) {
    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: Standard SDOs configured for AKD slave [%u] are:", slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying standard SDOs to AKD slave [%u]", slave);
    }

    /* in twincat this is written very early in the boot proccess
     * this is some magic version number and maybe twincat specific?
    */

    //if(!ec_sdo_write_uint32(slave, 0xF081,0x1,0x001B2336 )){
    //    return E_SDO_WRITE_FAILURE;
    //}

    //interpolation time index
    //0x60C:0x2
    //int8
    if (!ec_sdo_write_int8(slave, 0x60C2, 0x2, -3, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //interpolation time period
    //0x60C2:0x1
    //uint8
    if (!ec_sdo_write_int8(slave, 0x60C2, 0x1, MAP_CYCLE_TIME, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //FBUS.PARAM02:
    //This parameter activates the synchronization feature of the AKD.
    if (!ec_sdo_write_uint32(slave, 0x36E6, 0x0, 1, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //param04 - This parameter enables (1) or disables (0) the synchronization supervision of the CANOpen or EtherCAT fieldbus
    //0x36E8:0x0
    //uint32
    if (!ec_sdo_write_uint32(slave, 0x36E8, 0x0, 1, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //param05
    uint32_t uob32 = 0x0;
    // BIT 11 = No emergency messages over CANopen are triggered when a drive warning occurs.
    //    BIT_SET(uob32, 11);
    // BIT 4 = Scaling is done using special DS402 - objects (independent on units)
    BIT_SET(uob32, 4);
    if (!ec_sdo_write_uint32(slave, 0x36E9, 0x0, uob32, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //drv.opmode
    //0x35B4:0x0
    //int32
    if (!ec_sdo_write_int32(slave, 0x35B4, 0x0, 2, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //motor revolutions
    //0x6091:0x1
    //uint32
    //0x1
    if (!ec_sdo_write_uint32(slave, 0x6091, 0x1, 0x1, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //shaft revolutions
    //0x6091:0x2
    //uint32
    //0x1
    if (!ec_sdo_write_uint32(slave, 0x6091, 0x2, 0x1, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //feed
    //0x6092:0x1
    //unit32
    //655,360

    //    if (!ec_sdo_write_uint32(slave, 0x6092, 0x1, 0x00100000)) {
    if (!ec_sdo_write_uint32(slave, 0x6092, 0x1, 0x2710, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //shaft revolutions
    //0x6092:0x2
    //uint32
    //0x1
    if (!ec_sdo_write_uint32(slave, 0x6092, 0x2, 0x1, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* Subindex 2, known as the interpolation time index, defines the power of ten of the time value
    (e.g. -3 means 10-3 or milliseconds) while subindex 1, known as interpolation time units,
    gives the number of units (e.g. 4 means 4 units).
    You can run a 2 ms cycle using various combinations. For example,
    Index = -3, Units = 2 or
    Index = -4, Units = 20 etc.
    The FBUS.SAMPLEPERIOD parameter is counted in multiples of 62.5us microseconds
    within the device. This means, for example that 2 ms equates to FBUS.SAMPLEPERIOD
    value of 32.

     */

    if (!ec_sdo_write_int8(slave, AKD_MOO_SET_SDO_INDEX, AKD_MOO_SET_SDO_SUB_INDEX, CIA_MOO_CSP, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    return E_SUCCESS;
}

/**
 * @brief sets fixed PDO mapping with a load of SDO writes
 * @return
 * @warning must be called in pre-op states
 *
 * @attention covers all drives attached to a controller
 */
gberror_t ec_pdo_map_akd(const uint16_t slave) {
    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for AKD slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to AKD slave [%u]",
                slave);
    }

    osal_usleep(200000);

    // map_SM2.SM_assignment_index

    if (!ec_sdo_write_uint8(slave, map_SM2_akd.SM_assignment_index, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    osal_usleep(500);

    if (!ec_sdo_write_uint8(slave, map_SM3_akd.SM_assignment_index, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    osal_usleep(500);
    for (int i = 0; i < map_SM2_akd.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_akd.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_akd[i], true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }
    osal_usleep(500);
    for (int i = 0; i < map_SM3_akd.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_akd.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_akd[i], true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }
    osal_usleep(500);
    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint8(slave, map_SM2_akd.SM_assignment_index, 0, map_SM2_akd.number_of_entries, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    osal_usleep(500);
    if (!ec_sdo_write_uint8(slave, map_SM3_akd.SM_assignment_index, 0, map_SM3_akd.number_of_entries, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;
}

/**
 * @brief this is an a la carte PDO mapping function
 * @param controller_number
 * @return
 */
gberror_t ec_pdo_map_alt_akd(const uint16_t slave) {
    if (!ec_sdo_write_uint8(slave, 0x1C12, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1600, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1601, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1602, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1602, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //    ec_SDOwrite(map_controllers[controller_number], 0x1C12, 0, false, os, &ob8, EC_TIMEOUTRXM);
    //    ec_SDOwrite(map_controllers[controller_number], 0x1600, 0, false, os, &ob8, EC_TIMEOUTRXM);
    //    ec_SDOwrite(map_controllers[controller_number], 0x1601, 0, false, os, &ob8, EC_TIMEOUTRXM);
    //    ec_SDOwrite(map_controllers[controller_number], 0x1602, 0, false, os, &ob8, EC_TIMEOUTRXM);
    //    ec_SDOwrite(map_controllers[controller_number], 0x1602, 0, false, os, &ob8, EC_TIMEOUTRXM);


    /* Define RxPdo */

    //    ob32 = 0x60400010;
    /* 0x6040:0/16bits, control word */
    if (!ec_sdo_write_uint32(slave, 0x1600, 1, 0x60400010, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* 0x60C1:1/32bits */
    if (!ec_sdo_write_uint32(slave, 0x1600, 2, 0x60C10120, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* set number of PDO entries for 0x1600 */
    if (!ec_sdo_write_uint8(slave, 0x1600, 0, 2, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* list all RxPdo in 0x1C12:1-4 */
    if (!ec_sdo_write_uint16(slave, 0x1C12, 1, 0x1600, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* set number of RxPDO */
    if (!ec_sdo_write_uint8(slave, 0x1C12, 0, 1, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* Clear TxPdo */
    //    os = sizeof(ob8);
    //    ob8 = 0;

    if (!ec_sdo_write_uint8(slave, 0x1C13, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1A00, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1A01, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1A02, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1A03, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    //    ec_SDOwrite(slave, 0x1C13, 0, false, os, &ob8, EC_TIMEOUTRXM);
    //    ec_SDOwrite(slave, 0x1A00, 0, false, os, &ob8, EC_TIMEOUTRXM);
    //    ec_SDOwrite(slave, 0x1A01, 0, false, os, &ob8, EC_TIMEOUTRXM);
    //    ec_SDOwrite(slave, 0x1A02, 0, false, os, &ob8, EC_TIMEOUTRXM);
    //    ec_SDOwrite(slave, 0x1A03, 0, false, os, &ob8, EC_TIMEOUTRXM);

    /* Define TxPdo */
    /* 0x6041:0/16bits, status word */
    if (!ec_sdo_write_uint32(slave, 0x1A00, 1, 0x60410010, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* 0x3470:4/16bits, analog input */
    if (!ec_sdo_write_uint32(slave, 0x1A00, 2, 0x34700410, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    //    /* 0x60FD:0/32bits, digital inputs */
    if (!ec_sdo_write_uint32(slave, 0x1A00, 3, 0x60FD0020, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    /* set number of PDO entries for 0x1A00  */

    if (!ec_sdo_write_uint8(slave, 0x1600, 0, 3, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //    /* 0x606c:0/32bits, act velocity */
    if (!ec_sdo_write_uint32(slave, 0x1A01, 1, 0x606C0020, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* 0x6063:0/32bits, act position */
    if (!ec_sdo_write_uint32(slave, 0x1A01, 2, 0x60630020, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //    /* set number of PDO entries for 0x1A01  */
    if (!ec_sdo_write_uint8(slave, 0x1600, 0, 3, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //    /* list all RxPdo in 0x1C12:1-4 */
    if (!ec_sdo_write_uint16(slave, 0x1C13, 1, 0x1A00, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //    /* list all RxPdo in 0x1C12:1-4 */
    if (!ec_sdo_write_uint16(slave, 0x1C13, 2, 0x1A01, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //    /* set number of TxPDO */

    if (!ec_sdo_write_uint8(slave, 0x1C13, 0, 2, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    return E_SUCCESS;
}


/**
 * @brief checks a drive pdo to get the actual follow error and see if it is over tollerance
 * @param
 * @return false no follow error, true a follow error
 */
bool ec_get_follow_error_akd(const uint16_t drive) {
    return false;
    //read follow error bytes and threshold
    if (ec_pdo_get_input_uint32(map_drive_to_slave[drive], AKD_FOLLOWERROR_ACTVAL_PDO_INDEX) >
        AKD_FOLLOW_ERROR_TOLLERANCE) {
        printf("follow error %u\n",
               ec_pdo_get_input_uint32(map_drive_to_slave[drive], AKD_FOLLOWERROR_ACTVAL_PDO_INDEX));

        return true;
    }
    return false;
}


/**
 * @brief returns the value of MOO for an AKD drive
 * @param drive
 * @return moo
 *
 *
 */
__attribute__(( weak ))int8_t ec_get_moo_sdo_akd(const uint16_t drive) {
    int8_t ib8;

    if (!ec_sdo_read_int8(map_drive_to_slave[drive], AKD_MOO_GET_SDO_INDEX, AKD_MOO_GET_SDO_SUB_INDEX, &ib8, true)) {
        return 0;
    } else {
        return (ib8);
    }
    //these  codes are only accessible through SDO reads and so can't be accessed every cycle
}

/**
 * @brief returns an array of error codes for an AKD drive
 * @param drive_number
 * @return array of uint32_t error codes (sized MAX_NUM_DRIVE_ERROR_CODES)
 *
 * Does an SDO read
 * @warning doesn't process warning codes (but does read them)
 * @warning only returns one fault
 */
const uint8_t *ec_get_error_string_sdo_akd(const uint16_t drive) {
    uint32_t ib32 = 0;

    //these error codes are only accessible through SDO reads and so can't be accessed every cycle
    //it only gets called when in fault state and then only as an entry action

    static uint8_t error_reading_drive_error[] = "Error reading drive error";
    static uint8_t error_code_not_found[] = "Error code not found";
    static uint8_t error_code_no_error[] = "No error on drive";

    //if value is zero then no fault
    if (!ec_sdo_read_uint32(map_drive_to_slave[drive], AKD_ERROR_CODE_SDO_INDEX, 0, &ib32, false)) {
        return error_reading_drive_error;
    }

    if (ib32 == 0) {
        return error_code_no_error;
    }


    //subindexes 1-a contain faults - here we cheat and just read the first one
    if (!ec_sdo_read_uint32(map_drive_to_slave[drive], AKD_ERROR_CODE_SDO_INDEX, 1, &ib32, false)) {
        return error_reading_drive_error;
    }

    //loop over all few hundred error codes to match the one we have - could do a better search algo here
    for (int i = 0; i < NUM_OF_AKD_ERROR_STRINGS; i++) {
        if (akd_error_code_description[i].numeric_code == ib32) {
            return (uint8_t *) akd_error_code_description[i].description;
        }
    }
    return error_code_not_found;

    //This checks the warning code index - it is always 3
    //    uint8_t uib8;
    //    os= sizeof(uib8);
    //    rc = ec_SDOread(map_drives[drive_number], AKD_WARNING_CODE_INDEX, 0, false, &os,
    //                    &uib8, EC_TIMEOUTRXM);
    //    if (rc <= 0) {
    //        LL_ERROR(GBEM_GEN_LOG_EN, "Could not read SDO index:0x%04x - sub-index:0x%04x (on slave:%u)",
    //                 AKD_WARNING_CODE_INDEX, 0, map_drives[drive_number]);
    //    }
    //    printf("warning code index:%u\n",uib8);
    //
    //

    if (!ec_sdo_read_uint32(map_drive_to_slave[drive], AKD_WARNING_CODE_SDO_INDEX, 1, &ib32, false)) {
        return error_reading_drive_error;
    }
    //todo doesn't do anything with warning codes at the moment


    //0 in 0x2000.1 = no warning
    //0 in 0x2001.1 = no fault
    /*
     *
     * 0x2000.1 = System Warning 1 - U32
     * 0x2000.2 = System Warning 2 - U32
     * 0x2000.3 = System Warning 3 - U32
     *
     * 0x2001.1 = System Fault 1 - U32
     * 0x2001.2 = System Fault 2 - U32
     * 0x2001.3 = System Fault 3 - U32
     * 0x2001.4 = System Fault 4 - U32
     * 0x2001.5 = System Fault 5 - U32
     * 0x2001.6 = System Fault 6 - U32
     * 0x2001.7 = System Fault 7 - U32
     * 0x2001.8 = System Fault 8 - U32
     * 0x2001.9 = System Fault 9 - U32
     * 0x2001.10 = System Fault 10 - U32

     *
     *
     *
    *
     */
}

gberror_t ec_write_nvram_akd(const uint16_t slave) {
    if (!ec_sdo_write_uint32(map_drive_to_slave[slave], AKD_WRITE_CONFIG_SDO_INDEX,
                             AKD_WRITE_CONFIG_SDO_SUB_INDEX, AKD_WRITE_CONFIG_SDO_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    return E_SUCCESS;
}


/**
 * @brief get the value of the remote bit from an AKD drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_akd(const uint16_t drive) {
    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_akd(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}

/**
 * @brief set NVRAM SDOs for an AKD drive
 * @param slave
 * @return gberror
 * NVRAM sdos are things like limits and drive rotation direction that must be saved to NVRAM and the drive restarted before they take effect
 *
 */
gberror_t ec_nvram_sdos_akd(const uint16_t slave) {
    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: NVRAM SDOs configured for AKD slave [%u] are:", slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying NVRAM SDOs to AKD slave [%u]", slave);
    }
    //todo
    // if (!ec_sdo_write_int32(slave, AKD_MIN_LIMIT_SDO_INDEX,
    //                         AKD_MIN_LIMIT_SDO_SUB_INDEX, map_drive_neg_limit[slave], true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    // if (!ec_sdo_write_int32(slave, AKD_MAX_LIMIT_SDO_INDEX,
    //                         AKD_MAX_LIMIT_SDO_SUB_INDEX, map_drive_pos_limit[slave], true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    if (!ec_sdo_write_int32(slave, AKD_DIRECTION_SDO_INDEX,
                            AKD_DIRECTION_SDO_SUB_INDEX, map_drive_direction[slave], true)) {
        return E_SDO_WRITE_FAILURE;
    }

    gberror_t grc = ec_write_nvram_akd(slave);
    if (grc != E_SUCCESS) {
        return E_NVRAM_WRITE_FAILURE;
    } else {
        return E_SUCCESS;
    }
}


/**
 * @brief get actpos for an AKD drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actpos_wrd_akd(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], AKD_ACTPOS_PDO_INDEX);
}

/**
 * @brief get ctrlwrd for for an AKD drive
 * @param drive
 * @return ctrlwrd
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
uint16_t ec_get_ctrl_wrd_rev_akd(const uint16_t drive) {
    return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AKD_CONTROLWORD_PDO_INDEX);
}

/**
 * @brief set status word for an AKD drive
 * @param drive
 * @param statwrd
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_stat_wrd_rev_akd(const uint16_t drive, const uint16_t statwrd) {
    ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AKD_STATUSWORD_PDO_INDEX, statwrd);
    return E_SUCCESS;
}

/**
 * @brief set control word for AKD drive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_akd(const uint16_t drive, const uint16_t ctrlwrd) {
    ec_pdo_set_output_uint16(map_drive_to_slave[drive], AKD_CONTROLWORD_PDO_INDEX, ctrlwrd);
    return E_SUCCESS;
}

/**
 * @brief get status word for AKD drive
 * @param drive
 * @return status word
 */
uint16_t ec_get_stat_wrd_akd(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AKD_STATUSWORD_PDO_INDEX);
}

/**
 * @brief set setpos for an AKD drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setpos_wrd_akd(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], AKD_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}

/**
 * @brief set actpos for an AKD drive
 * @param drive
 * @param actpos
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_actpos_wrd_rev_akd(const uint16_t drive, const int32_t actpos) {
    ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AKD_ACTPOS_PDO_INDEX, actpos);
    return E_SUCCESS;
}

/**
 * @brief get setpos for an AKD drive
 * @param drive
 * @return setpos
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
int32_t ec_get_setpos_word_rev_akd(const uint16_t drive) {
    return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AKD_SETPOS_PDO_INDEX);
}


/**
 * @brief utility function to do SDO read Kollmorgen AKD sync0 params
 * @param slave
 * @return
 *@warning will mess up the real time cycle time if called during cyclic PDO exchange!
 */
bool print_akd_sync_params(const uint16_t slave) {
    uint32_t ib32;

    ec_sdo_read_uint32(slave, 0x3496, 0x2, &ib32, true);
    LL_INFO(GBEM_GEN_LOG_EN, "GBEM: AKD drive actual sync time [%d]", ib32);
    return true;
}
