/**
 ******************************************************************************
 * @file           :  azd.c
 * @brief          :  azd (2-3-4) drive control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "azdxa-ked.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"

//LL_TRACE(GBEM_FUN_TRACE_LOG_EN ,
//         "GBEM: Linked ec_standard_sdo function: [%s] for slave [%u] (this is controlled by the MACHINE #define)",
//         __FUNCTION__ , slave);


/*
 * Three models in AZDxA-KED range:
 * AZD2A-KED - 2 axis
 * AZD3A-KED - 3 axis
 * AZD4A-KED - 4 axis
 *
 * We have some functions (like PDO setting) that are specific to the different models and are named azd2a_ked, azd3a_ked and azd4a_ked
 * but most are generic functions and cover the range of drives in a single function and are named azdxa_ked
 */


/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_azd2a_ked = {
        .number_of_entries = 3,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_azd2a_ked = {
        .number_of_entries = 3,
        .SM_assignment_index = 0x1c13};

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_azd3a_ked = {
        .number_of_entries = 4,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_azd3a_ked = {
        .number_of_entries = 4,
        .SM_assignment_index = 0x1c13};

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_azd4a_ked = {
        .number_of_entries = 5,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_azd4a_ked = {
        .number_of_entries = 5,
        .SM_assignment_index = 0x1c13};


/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_azd2a_ked[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1600,
        0x1610,
        0x1700};

/* This is used for the fixed PDO remapping */
uint16_t map_SM3_index_of_assigned_PDO_azd2a_ked[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a03,
        0x1a13,
        0x1b00};


/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_azd3a_ked[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1600,
        0x1610,
        0x1620,
        0x1700};

/* This is used for the fixed PDO remapping */
uint16_t map_SM3_index_of_assigned_PDO_azd3a_ked[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a03,
        0x1a13,
        0x1a23,
        0x1b00};

/* This is used for the fixed PDO remapping */
uint16_t map_SM2_index_of_assigned_PDO_azd4a_ked[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1600,
        0x1610,
        0x1620,
        0x1630,
        0x1700};

/* This is used for the fixed PDO remapping */
uint16_t map_SM3_index_of_assigned_PDO_azd4a_ked[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a03,
        0x1a13,
        0x1a23,
        0x1a23,
        0x1b00};



/** this is your standard sdos needed to config a slave applied during the boot process */
/**
 * @brief perform SDOs writes during boot to configure a slave
 * @param slave
 * @return gberror_t
 * @retval E_SUCCESS all ok
 */
gberror_t ec_standard_sdos_azdxa_ked(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: No standard SDOs configured for AZD slave [%u]", slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: No standard SDOs configured for AZD slave [%u]", slave);
    }
    return E_SUCCESS;
}


/**
 * @brief sets fixed PDO mapping with a load of SDO writes for AZD3A-KED drive
 * @return
 * @warning must be called in pre-op states
 *
 * @attention covers all drives attached to a controller
 */
gberror_t ec_pdo_map_azd2a_ked(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for AZD2A-KED slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to AZD2A-KED slave [%u]",
                slave);
    }

    if (!ec_sdo_write_uint16(slave, map_SM2_azd2a_ked.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_azd2a_ked.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_azd2a_ked.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_azd2a_ked.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_azd2a_ked[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_azd2a_ked.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_azd2a_ked.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_azd2a_ked[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_azd2a_ked.SM_assignment_index, 0,
                             map_SM2_azd2a_ked.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_azd2a_ked.SM_assignment_index, 0,
                             map_SM3_azd2a_ked.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;


}

/**
 * @brief sets fixed PDO mapping with a load of SDO writes for AZD3A-KED drive
 * @return
 * @warning must be called in pre-op states
 *
 * @attention covers all drives attached to a controller
 */
gberror_t ec_pdo_map_azd3a_ked(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for AZD3A-KED slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to AZD3A-KED slave [%u]",
                slave);
    }

    //ESM MUST be is pre-operational
    /*
     * The process for mapping the PDOs is laid out below, dead simple.
     *
     * The ESM (EtherCAT State Machine) MUST be is pre-operational state to do this - if you are in any other state it will just plain not work
     *
     * Then:
     * 1. Set the number of entries of the sync manager 2/sync manager 3 PDO assignment object to 0.
     * 		this is say index 1c12 sub-index 0
     * 2. Set the number of entries of the PDO mapping object to 0.
     * 		this is say index 1600 sub-index 0 (only do this if you are changing the PDO mapping object)
     * 3. Change the PDO mapping object.
     * 4. Change the number of entries of the PDO mapping object to the number of objects mapped in the step 3.
     * 5. Change the sync manager 2/sync manager 3 PDO assignment object.
     * 6. Change the number of entries of the sync manager 2/sync manager 3 PDO assignment object to the number assigned in the step 5.
     */

    /* There are two levels to the pdo mapping:
     * 1. you can fiddle with the SM2/SM3 assignment object and use the defaults on the EtherCAT device for the the PDO mapping object
     * 2. you can fiddle with the SM2/SM3 assignment object and also set the PDO mapping objects to point at individual objects
     * Crystal clear now i hope.
     */

    /*
     * First let's set the SM2 & SM3 assignment object number of entries to 0 (sub-index 0) - i.e. zero them out
     */


    if (!ec_sdo_write_uint16(slave, map_SM2_azd3a_ked.SM_assignment_index, 0, 0)) {
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

    for (int i = 0; i < map_SM3_azd3a_ked.number_of_entries; i++) {
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
 * @brief sets fixed PDO mapping with a load of SDO writes for AZD3A-KED drive
 * @return
 * @warning must be called in pre-op states
 *
 * @attention covers all drives attached to a controller
 */
gberror_t ec_pdo_map_azd4a_ked(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for AZD4A-KED slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to AZD4A-KED slave [%u]",
                slave);
    }

    if (!ec_sdo_write_uint16(slave, map_SM2_azd4a_ked.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_azd4a_ked.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_azd4a_ked.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_azd4a_ked.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_azd4a_ked[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_azd4a_ked.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_azd4a_ked.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_azd4a_ked[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_azd4a_ked.SM_assignment_index, 0,
                             map_SM2_azd4a_ked.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_azd4a_ked.SM_assignment_index, 0,
                             map_SM3_azd4a_ked.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly
    return E_SUCCESS;


}


//This is the custom mapping
//SM2 outputs
//byte offset   addr.b         index:sub   bitl data_type    name
//[0          ] [0x0001.0] 0x6040:0x00 0x10 UNSIGNED16   Controlword
//[2          ] [0x0003.0] 0x607A:0x00 0x20 INTEGER32    Target position
//[6          ] [0x0007.0] 0x60FF:0x00 0x20 INTEGER32    Target velocity
//[10         ] [0x000B.0] 0x6060:0x00 0x08 INTEGER8     Modes of operation
//[11         ] [0x000C.0] 0x6840:0x00 0x10 UNSIGNED16   Controlword
//[13         ] [0x000E.0] 0x687A:0x00 0x20 INTEGER32    Target position
//[17         ] [0x0012.0] 0x68FF:0x00 0x20 INTEGER32    Target velocity
//[21         ] [0x0016.0] 0x6860:0x00 0x08 INTEGER8     Modes of operation
//[22         ] [0x0017.0] 0x7040:0x00 0x10 UNSIGNED16   Controlword
//[24         ] [0x0019.0] 0x707A:0x00 0x20 INTEGER32    Target position
//[28         ] [0x001D.0] 0x70FF:0x00 0x20 INTEGER32    Target velocity
//[32         ] [0x0021.0] 0x7060:0x00 0x08 INTEGER8     Modes of operation
//[33         ] [0x0022.0] 0x2F01:0x00 0x08 UNSIGNED8    Controller command 1
//[34         ] [0x0023.0] 0x2F02:0x00 0x08 UNSIGNED8    Controller command 2
//SM3 inputs
//byte offset   addr.b         index:sub   bitl data_type    name
//[0          ] [0x0025.0] 0x6041:0x00 0x10 UNSIGNED16   Statusword
//[2          ] [0x0027.0] 0x6064:0x00 0x20 INTEGER32    Position actual value
//[6          ] [0x002B.0] 0x606C:0x00 0x20 INTEGER32    Velocity actual value
//[10         ] [0x002F.0] 0x6061:0x00 0x08 INTEGER8     Modes of operation display
//[11         ] [0x0030.0] 0x603F:0x00 0x10 UNSIGNED16   Error code
//[13         ] [0x0032.0] 0x6841:0x00 0x10 UNSIGNED16   Statusword
//[15         ] [0x0034.0] 0x6864:0x00 0x20 INTEGER32    Position actual value
//[19         ] [0x0038.0] 0x686C:0x00 0x20 INTEGER32    Velocity actual value
//[23         ] [0x003C.0] 0x6861:0x00 0x08 INTEGER8     Modes of operation display
//[24         ] [0x003D.0] 0x683F:0x00 0x10 UNSIGNED16   Error code
//[26         ] [0x003F.0] 0x7041:0x00 0x10 UNSIGNED16   Statusword
//[28         ] [0x0041.0] 0x7064:0x00 0x20 INTEGER32    Position actual value
//[32         ] [0x0045.0] 0x706C:0x00 0x20 INTEGER32    Velocity actual value
//[36         ] [0x0049.0] 0x7061:0x00 0x08 INTEGER8     Modes of operation display
//[37         ] [0x004A.0] 0x703F:0x00 0x10 UNSIGNED16   Error code
//[39         ] [0x004C.0] 0x2E31:0x00 0x08 UNSIGNED8    Controller status 1
//[40         ] [0x004D.0] 0x2E32:0x00 0x08 UNSIGNED8    Controller status 2

/**
 * @brief this is an a la carte PDO mapping function
 * @param controller_number
 * @return
 */
gberror_t ec_pdo_map_custom_azd3a_ked(const uint16_t slave) {

    if (!ec_sdo_write_uint8(slave, 0x1C12, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, 0x1600, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1610, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1620, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1700, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* Define RxPdo */

    //Axis 1 0x1600
    /* 0x6040:1/16bits, control word */
    if (!ec_sdo_write_uint32(slave, 0x1600, 1, 0x60400010)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* 0x607A:2/32bits, target position */
    if (!ec_sdo_write_uint32(slave, 0x1600, 2, 0x607A0020)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* 0x60FF:3/32bits, target velocity */
    if (!ec_sdo_write_uint32(slave, 0x1600, 3, 0x60FF0020)) {
        return E_SDO_WRITE_FAILURE;
    }


    /* 0x6060:4/8bits, modes of operation */
    if (!ec_sdo_write_uint32(slave, 0x1600, 4, 0x60600008)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* set number of PDO entries for 0x1600 */

    if (!ec_sdo_write_uint8(slave, 0x1600, 0, 4)) {
        return E_SDO_WRITE_FAILURE;
    }


    //Axis 2 - 0x1610
/* 0x6840:0/16bits, control word */
    if (!ec_sdo_write_uint32(slave, 0x1610, 1, 0x68400010)) {
        return E_SDO_WRITE_FAILURE;
    }

/* 0x687A:1/32bits, target position */
    if (!ec_sdo_write_uint32(slave, 0x1610, 2, 0x687A0020)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x68FF:2/32bits, target velocity */
    if (!ec_sdo_write_uint32(slave, 0x1610, 3, 0x68FF0020)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x6860:3/8bits, modes of operation */
    if (!ec_sdo_write_uint32(slave, 0x1610, 4, 0x68600008)) {
        return E_SDO_WRITE_FAILURE;
    }
    /* set number of PDO entries for 0x1610 */

    if (!ec_sdo_write_uint8(slave, 0x1610, 0, 4)) {
        return E_SDO_WRITE_FAILURE;
    }

//Axis 3 - 0x1620
/* 0x7040:0/16bits, control word */
    if (!ec_sdo_write_uint32(slave, 0x1620, 1, 0x70400010)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x707A:1/32bits, target position */
    if (!ec_sdo_write_uint32(slave, 0x1620, 2, 0x707A0020)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x70FF:2/32bits, target velocity */
    if (!ec_sdo_write_uint32(slave, 0x1620, 3, 0x70FF0020)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x7060:3/8bits, modes of operation */
    if (!ec_sdo_write_uint32(slave, 0x1620, 4, 0x70600008)) {
        return E_SDO_WRITE_FAILURE;
    }
    /* set number of PDO entries for 0x1620 */

    if (!ec_sdo_write_uint8(slave, 0x1620, 0, 4)) {
        return E_SDO_WRITE_FAILURE;
    }


    //Controller receive PDO mapping - 0x1700
/* 0x2F01:1/8bits, controller command 1 */

    if (!ec_sdo_write_uint32(slave, 0x1700, 1, 0x2F010008)) {
        return E_SDO_WRITE_FAILURE;
    }
    /* 0x2F02:2/8bits, controller command 2 */
    if (!ec_sdo_write_uint32(slave, 0x1700, 2, 0x2F020008)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, 0x1700, 0, 2)) {
        return E_SDO_WRITE_FAILURE;
    }


    //final stage

    if (!ec_sdo_write_uint16(slave, 0x1C12, 1, 0x1600)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, 0x1C12, 2, 0x1610)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, 0x1C12, 3, 0x1620)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, 0x1C12, 4, 0x1700)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* set number of RxPDO */
    if (!ec_sdo_write_uint8(slave, 0x1C12, 0, 4)) {
        return E_SDO_WRITE_FAILURE;
    }

/* Clear TxPdo */
    if (!ec_sdo_write_uint8(slave, 0x1C13, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1A00, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1A10, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1A20, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint8(slave, 0x1B00, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* Define TxPdo */

    //Axis 1 - 1A00

/* 0x6041:1/16bits, status word */
    if (!ec_sdo_write_uint32(slave, 0x1A00, 1, 0x60410010)) {
        return E_SDO_WRITE_FAILURE;
    }

/* 0x6064:2/32bits, position actual value */

    if (!ec_sdo_write_uint32(slave, 0x1A00, 2, 0x60640020)) {
        return E_SDO_WRITE_FAILURE;
    }

/* 0x606C:3/32bits, velocity actual value */
    if (!ec_sdo_write_uint32(slave, 0x1A00, 3, 0x606C0020)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* 0x6061:4/32bits, moo display */
    if (!ec_sdo_write_uint32(slave, 0x1A00, 4, 0x60610008)) {
        return E_SDO_WRITE_FAILURE;
    }

    /* 0x603Fh:5//16bits, error code */
    if (!ec_sdo_write_uint32(slave, 0x1A00, 5, 0x603F0010)) {
        return E_SDO_WRITE_FAILURE;
    }


    if (!ec_sdo_write_uint8(slave, 0x1A00, 0, 5)) {
        return E_SDO_WRITE_FAILURE;
    }

//Axis 2 - 1A10
/* 0x6841:1/16bits, status word */
    if (!ec_sdo_write_uint32(slave, 0x1A10, 1, 0x68410010)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x6864:2/32bits, position actual value */
    if (!ec_sdo_write_uint32(slave, 0x1A10, 2, 0x68640020)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x686C:2/32bits, velocity actual value */
    if (!ec_sdo_write_uint32(slave, 0x1A10, 3, 0x686C0020)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x6861:4/32bits, moo display */
    if (!ec_sdo_write_uint32(slave, 0x1A10, 4, 0x68610008)) {
        return E_SDO_WRITE_FAILURE;
    }

/* 0x683Fh:5//16bits, error code */
    if (!ec_sdo_write_uint32(slave, 0x1A10, 5, 0x683F0010)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, 0x1A10, 0, 5)) {
        return E_SDO_WRITE_FAILURE;
    }


//Axis 3 - 1A20
/* 0x7041:1/16bits, status word */
    if (!ec_sdo_write_uint32(slave, 0x1A20, 1, 0x70410010)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x7064:2/32bits, position actual value */
    if (!ec_sdo_write_uint32(slave, 0x1A20, 2, 0x70640020)) {
        return E_SDO_WRITE_FAILURE;
    }

/* 0x706C:3/32bits, velocity actual value */
    if (!ec_sdo_write_uint32(slave, 0x1A20, 3, 0x706C0020)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x7061:4/32bits, moo display */
    if (!ec_sdo_write_uint32(slave, 0x1A20, 4, 0x70610008)) {
        return E_SDO_WRITE_FAILURE;
    }

/* 0x703Fh:5//16bits, error code */
    if (!ec_sdo_write_uint32(slave, 0x1A20, 5, 0x703F0010)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, 0x1A20, 0, 5)) {
        return E_SDO_WRITE_FAILURE;
    }

//Controller transmit PDO mapping - 1B00
/* 0x2E31:1/8bits, controller status 1 */
    if (!ec_sdo_write_uint32(slave, 0x1B00, 1, 0x2E310008)) {
        return E_SDO_WRITE_FAILURE;
    }
/* 0x2E32:2/8bits, controller status 2 */
    if (!ec_sdo_write_uint32(slave, 0x1B00, 2, 0x2E320008)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint8(slave, 0x1B00, 0, 2)) {
        return E_SDO_WRITE_FAILURE;
    }


    if (!ec_sdo_write_uint16(slave, 0x1C13, 1, 0x1A00)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, 0x1C13, 2, 0x1A10)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, 0x1C13, 3, 0x1A20)) {
        return E_SDO_WRITE_FAILURE;
    }
    if (!ec_sdo_write_uint16(slave, 0x1C13, 4, 0x1B00)) {
        return E_SDO_WRITE_FAILURE;
    }

//    /* set number of TxPDO */

    if (!ec_sdo_write_uint8(slave, 0x1C13, 0, 4)) {
        return E_SDO_WRITE_FAILURE;
    }
    return E_SUCCESS;
}


/**
 * @brief gets the mode of operation (MOO) for AZD drives
 * @param drive_number
 * @return  moo value (8 for CSP)
 * @attention handles sub-drives
 */
int8_t ec_get_moo_pdo_azdxa_ked(const uint16_t drive) {
//    LL_TRACE(GBEM_FUN_TRACE_LOG_EN,
//            "GBEM: Linked ec_get_modes_of_operation function: %s (this is controlled by the MACHINE #define)",
//            __FUNCTION__);

    switch (map_drive_to_subdrive[drive]) {
        case 1:
//            printf("returning %d for sub drive 1", ec_pdo_get_input_int8(map_drive_to_slave[drive], AZD_MOODISP_DRIVE_1_INDEX));
            return ec_pdo_get_input_int8(map_drive_to_slave[drive], AZDXA_KED_MOODISP_PDO_DRIVE1_INDEX);

        case 2:
//            printf("returning %d for sub drive 2", ec_pdo_get_input_int8(map_drive_to_slave[drive], AZD_MOODISP_DRIVE_2_INDEX));
            return ec_pdo_get_input_int8(map_drive_to_slave[drive], AZDXA_KED_MOODISP_PDO_DRIVE2_INDEX);
        case 3:
            return ec_pdo_get_input_int8(map_drive_to_slave[drive], AZDXA_KED_MOODISP_PDO_DRIVE3_INDEX);
        case 4:
            return ec_pdo_get_input_int8(map_drive_to_slave[drive], AZDXA_KED_MOODISP_PDO_DRIVE4_INDEX);
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            //return a zero moo
            return 0;
    }
}


/**
 * @brief get the value of the remote bit from an AZD drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_azdxa_ked(const uint16_t drive) {

    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_azdxa_ked(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_REMOTE_BIT_NUM)) {
        return true;
    }
    return false;
}


bool ec_get_follow_error_azdxa_ked(const uint16_t drive) {
    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_azdxa_ked(drive);

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

    //for the AZD we need to set the modes of operation in the initial PDO writes
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (map_drive_to_slave[i] == slave) {
            //this is a drive attached to the slave
            switch (map_drive_to_subdrive[i]) {
                case 1:
                    ec_pdo_set_output_int8(slave, AZDXA_KED_MOOSET_PDO_DRIVE1_INDEX, map_drive_moo[i]);
//                    printf("mooset azd sub drive 1\n");
                    break;
                case 2:
                    ec_pdo_set_output_int8(slave, AZDXA_KED_MOOSET_PDO_DRIVE2_INDEX, map_drive_moo[i]);
//                    printf("mooset azd sub drive 2\n");
                    break;
                case 3:
                    ec_pdo_set_output_int8(slave, AZDXA_KED_MOOSET_PDO_DRIVE3_INDEX, map_drive_moo[i]);
                    break;
                case 4:
                    ec_pdo_set_output_int8(slave, AZDXA_KED_MOOSET_PDO_DRIVE4_INDEX, map_drive_moo[i]);
                    break;
                default:
                    LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
                    return E_SUB_DRIVE_OUT_OF_RANGE;
            }
        }

    }
    if (ec_iserror()) {
        LL_ERROR(GBEM_GEN_LOG_EN,
                 "GBEM: EtherCAT error detected after initial PDO writes: %s", ec_elist2string());
        return E_ETHERCAT_ERROR_DETECTED;
    }

    return E_SUCCESS;
}


/**
 * @brief reads drive error codes for a specific AZD drive and returns an array of error codes
 * @param drive
 * @return pointer to error code string
 */
uint8_t *ec_get_error_string_pdo_azdxa_ked(const uint16_t drive) {

    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];


    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);

    uint16_t drive_error_code = 0;
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            drive_error_code = ec_pdo_get_input_uint16(map_drive_to_slave[drive],
                                                       AZDXA_KED_ERROR_CODE_PDO_DRIVE1_INDEX);
            break;
        case 2:
            drive_error_code = ec_pdo_get_input_uint16(map_drive_to_slave[drive],
                                                       AZDXA_KED_ERROR_CODE_PDO_DRIVE2_INDEX);
            break;
        case 3:
            drive_error_code = ec_pdo_get_input_uint16(map_drive_to_slave[drive],
                                                       AZDXA_KED_ERROR_CODE_PDO_DRIVE3_INDEX);
            break;
        case 4:
            drive_error_code = ec_pdo_get_input_uint16(map_drive_to_slave[drive],
                                                       AZDXA_KED_ERROR_CODE_PDO_DRIVE4_INDEX);
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            //return a zero error code
            return 0;
    }

    if (drive_error_code != 0) {
        BITMASK_CLEAR(drive_error_code, 0xFF00);
    }
    for (int i = 0; i < NUM_OF_AZD_ERROR_STRINGS; i++) {
        if (azdxa_ked_alarm_code[i].error_id == drive_error_code) {
            memset(&error_code_string[0], 0, sizeof(error_code_string));
            strncpy((char *) &error_code_string[0], azdxa_ked_alarm_code[i].text_string, sizeof(error_code_string) - 1);
            break;
        }
    }
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
gberror_t ec_write_nvram_azdxa_ked(const uint16_t slave) {

    //this writes stuff to drive NVRAM - DO NOT LEAVE ENABLED - maybe

    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (map_drive_to_slave[i] == slave) {
            //this is a drive attached to the slave
            switch (map_drive_to_subdrive[i]) {
                case 1:
                    if (!ec_sdo_write_int8(slave, AZDXA_KED_WRITE_CONFIG_SDO_DRIVE1_INDEX,
                                           AZDXA_KED_WRITE_CONFIG_SDO_DRIVE1_SUB_INDEX,
                                           AZDXA_KED_WRITE_CONFIG_SDO_DRIVE1_VALUE)) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
                case 2:
                    if (!ec_sdo_write_int8(slave, AZDXA_KED_WRITE_CONFIG_SDO_DRIVE2_INDEX,
                                           AZDXA_KED_WRITE_CONFIG_SDO_DRIVE2_SUB_INDEX,
                                           AZDXA_KED_WRITE_CONFIG_SDO_DRIVE2_VALUE)) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
                case 3:
                    if (!ec_sdo_write_int8(slave, AZDXA_KED_WRITE_CONFIG_SDO_DRIVE3_INDEX,
                                           AZDXA_KED_WRITE_CONFIG_SDO_DRIVE3_SUB_INDEX,
                                           AZDXA_KED_WRITE_CONFIG_SDO_DRIVE3_VALUE)) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
                case 4:
                    if (!ec_sdo_write_int8(slave, AZDXA_KED_WRITE_CONFIG_SDO_DRIVE4_INDEX,
                                           AZDXA_KED_WRITE_CONFIG_SDO_DRIVE4_SUB_INDEX,
                                           AZDXA_KED_WRITE_CONFIG_SDO_DRIVE4_VALUE)) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
                default:
                    LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
                    return E_SUB_DRIVE_OUT_OF_RANGE;
            }
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
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZDXA_KED_ACTPOS_PDO_DRIVE1_INDEX);
        case 2:
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZDXA_KED_ACTPOS_PDO_DRIVE2_INDEX);
        case 3:
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZDXA_KED_ACTPOS_PDO_DRIVE3_INDEX);
        case 4:
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZDXA_KED_ACTPOS_PDO_DRIVE4_INDEX);
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            //return 404 as a magic number error
            return 404;
    }
}


/**
 * @brief get actvel for an AZD drive
 * @param drive
 * @return int32 velocity
 */
int32_t ec_get_actvel_wrd_azdxa_ked(const uint16_t drive) {
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZDXA_KED_ACTVEL_PDO_DRIVE1_INDEX);
        case 2:
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZDXA_KED_ACTVEL_PDO_DRIVE2_INDEX);
        case 3:
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZDXA_KED_ACTVEL_PDO_DRIVE3_INDEX);
        case 4:
            return ec_pdo_get_input_int32(map_drive_to_slave[drive], AZDXA_KED_ACTVEL_PDO_DRIVE4_INDEX);
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            //return 404 as a magic number error
            return 404;
    }
}

/**
 * @brief get ctrlwrd for for an AZD drive
 * @param drive
 * @return ctrlwrd
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
uint16_t ec_get_ctrl_wrd_rev_azdxa_ked(const uint16_t drive) {
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AZDXA_KED_CONTROLWORD_PDO_DRIVE1_INDEX);
        case 2:
            return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AZDXA_KED_CONTROLWORD_PDO_DRIVE2_INDEX);
        case 3:
            return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AZDXA_KED_CONTROLWORD_PDO_DRIVE3_INDEX);
        case 4:
            return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AZDXA_KED_CONTROLWORD_PDO_DRIVE4_INDEX);
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            //return a zero ctrl wrd
            return 0;
    }

}

/**
 * @brief set status word for an AZD drive
 * @param drive
 * @param statwrd
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_stat_wrd_rev_azdxa_ked(const uint16_t drive, const uint16_t statwrd) {
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AZDXA_KED_STATUSWORD_PDO_DRIVE1_INDEX, statwrd);
            break;
        case 2:
            ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AZDXA_KED_STATUSWORD_PDO_DRIVE2_INDEX, statwrd);
            break;
        case 3:
            ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AZDXA_KED_STATUSWORD_PDO_DRIVE3_INDEX, statwrd);
            break;
        case 4:
            ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AZDXA_KED_STATUSWORD_PDO_DRIVE4_INDEX, statwrd);
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            return E_SUB_DRIVE_OUT_OF_RANGE;
    }
    return E_SUCCESS;
}

/**
 * @brief set control word for AZD drive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_azdxa_ked(const uint16_t drive, const uint16_t ctrlwrd) {

    switch (map_drive_to_subdrive[drive]) {
        case 1:
            ec_pdo_set_output_uint16(map_drive_to_slave[drive], AZDXA_KED_CONTROLWORD_PDO_DRIVE1_INDEX, ctrlwrd);
            break;
        case 2:
            ec_pdo_set_output_uint16(map_drive_to_slave[drive], AZDXA_KED_CONTROLWORD_PDO_DRIVE2_INDEX, ctrlwrd);
            break;
        case 3:
            ec_pdo_set_output_uint16(map_drive_to_slave[drive], AZDXA_KED_CONTROLWORD_PDO_DRIVE3_INDEX, ctrlwrd);
            break;
        case 4:
            ec_pdo_set_output_uint16(map_drive_to_slave[drive], AZDXA_KED_CONTROLWORD_PDO_DRIVE4_INDEX, ctrlwrd);
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            return E_SUB_DRIVE_OUT_OF_RANGE;
    }
    return E_SUCCESS;
}

/**
 * @brief set status word for AZD drive
 * @param drive
 * @return status word

 */
uint16_t ec_get_stat_wrd_azdxa_ked(const uint16_t drive) {
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AZDXA_KED_STATUSWORD_PDO_DRIVE1_INDEX);
        case 2:
            return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AZDXA_KED_STATUSWORD_PDO_DRIVE2_INDEX);
        case 3:
            return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AZDXA_KED_STATUSWORD_PDO_DRIVE3_INDEX);
        case 4:
            return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AZDXA_KED_STATUSWORD_PDO_DRIVE4_INDEX);
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            //return a zero stat wrd
            return 0;


    }
}

/**
 * @brief set setpos for an AZD drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setpos_wrd_azdxa_ked(const uint16_t drive, const int32_t setpos) {

    switch (map_drive_to_subdrive[drive]) {
        case 1:
            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZDXA_KED_SETPOS_PDO_DRIVE1_INDEX, setpos);
            break;
        case 2:
            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZDXA_KED_SETPOS_PDO_DRIVE2_INDEX, setpos);
            break;
        case 3:
            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZDXA_KED_SETPOS_PDO_DRIVE3_INDEX, setpos);
            break;
        case 4:
            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZDXA_KED_SETPOS_PDO_DRIVE4_INDEX, setpos);
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            return E_SUB_DRIVE_OUT_OF_RANGE;

    }
    return E_SUCCESS;

}

/**
 * @brief set setvel for an AZD drive
 * @param drive
 * @param setvel
 * @return gberror
 */
gberror_t ec_set_setvel_wrd_azdxa_ked(const uint16_t drive, const int32_t setvel) {

    switch (map_drive_to_subdrive[drive]) {
        case 1:
            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZDXA_KED_SETVEL_PDO_DRIVE1_INDEX, setvel);
            break;
        case 2:
            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZDXA_KED_SETVEL_PDO_DRIVE2_INDEX, setvel);
            break;
        case 3:
            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZDXA_KED_SETVEL_PDO_DRIVE3_INDEX, setvel);
            break;
        case 4:
            ec_pdo_set_output_int32(map_drive_to_slave[drive], AZDXA_KED_SETVEL_PDO_DRIVE4_INDEX, setvel);
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            return E_SUB_DRIVE_OUT_OF_RANGE;

    }
    return E_SUCCESS;

}


/**
 * @brief set actpos for an AZD drive
 * @param drive
 * @param actpos
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_actpos_wrd_rev_azdxa_ked(const uint16_t drive, const int32_t actpos) {
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AZDXA_KED_ACTPOS_PDO_DRIVE1_INDEX, actpos);
            break;
        case 2:
            ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AZDXA_KED_ACTPOS_PDO_DRIVE2_INDEX, actpos);
            break;
        case 3:
            ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AZDXA_KED_ACTPOS_PDO_DRIVE3_INDEX, actpos);
            break;
        case 4:
            ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AZDXA_KED_ACTPOS_PDO_DRIVE4_INDEX, actpos);
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Sub-drive index out of range");
            return E_SUB_DRIVE_OUT_OF_RANGE;

    }
    return E_SUCCESS;


}

/**
 * @brief get setpos for an AZD drive
 * @param drive
 * @return setpos
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
int32_t ec_get_setpos_word_rev_azdxa_ked(const uint16_t drive) {
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AZDXA_KED_SETPOS_PDO_DRIVE1_INDEX);
        case 2:
//            printf("setpos [%d]\n",ec_pdo_get_output_int32_sim(map_drive_to_slave[drive], AZD_SETPOS_DRIVE_2_INDEX));
            return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AZDXA_KED_SETPOS_PDO_DRIVE2_INDEX);
        case 3:
            return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AZDXA_KED_SETPOS_PDO_DRIVE3_INDEX);
        case 4:
            return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AZDXA_KED_SETPOS_PDO_DRIVE4_INDEX);
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "Sub-drive index out of range");
            //return a zero setpos
            return 0;

    }
}


/**
 * @brief set moo for an AZD drive
 * @param drive
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_moo_pdo_rev_azdxa_ked(const uint16_t drive) {
    switch (map_drive_to_subdrive[drive]) {
        case 1:
            ec_pdo_set_input_int8_rev(map_drive_to_slave[drive], AZDXA_KED_MOODISP_PDO_DRIVE1_INDEX,
                                      map_drive_moo[drive]);
            break;
        case 2:
            ec_pdo_set_input_int8_rev(map_drive_to_slave[drive], AZDXA_KED_MOODISP_PDO_DRIVE2_INDEX,
                                      map_drive_moo[drive]);

            break;
        case 3:
            ec_pdo_set_input_int8_rev(map_drive_to_slave[drive], AZDXA_KED_MOODISP_PDO_DRIVE3_INDEX,
                                      map_drive_moo[drive]);
            break;
        case 4:
            ec_pdo_set_input_int8_rev(map_drive_to_slave[drive], AZDXA_KED_MOODISP_PDO_DRIVE4_INDEX,
                                      map_drive_moo[drive]);

            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "Sub-drive index out of range");
            return E_SUB_DRIVE_OUT_OF_RANGE;
    }
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

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: NVRAM SDOs configured for AZD slave [%u] are:", slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying NVRAM SDOs to AZD slave [%u]", slave);
    }

    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (map_drive_to_slave[i] == slave) {
            //this is a drive attached to the slave

            switch (map_drive_to_subdrive[i]) {
                case 1:
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_MIN_LIMIT_SDO_DRIVE1_INDEX,
                                            AZDXA_KED_MIN_LIMIT_SDO_DRIVE1_SUB_INDEX, map_drive_neg_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_MAX_LIMIT_SDO_DRIVE1_INDEX,
                                            AZDXA_KED_MAX_LIMIT_SDO_DRIVE1_SUB_INDEX, map_drive_pos_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_DIRECTION_SDO_DRIVE1_INDEX,
                                            AZDXA_KED_DIRECTION_SDO_DRIVE1_SUB_INDEX, map_drive_direction[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
                case 2:
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_MIN_LIMIT_SDO_DRIVE2_INDEX,
                                            AZDXA_KED_MIN_LIMIT_SDO_DRIVE2_SUB_INDEX, map_drive_neg_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_MAX_LIMIT_SDO_DRIVE2_INDEX,
                                            AZDXA_KED_MAX_LIMIT_SDO_DRIVE2_SUB_INDEX, map_drive_pos_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_DIRECTION_SDO_DRIVE2_INDEX,
                                            AZDXA_KED_DIRECTION_SDO_DRIVE2_SUB_INDEX, map_drive_direction[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
                case 3:
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_MIN_LIMIT_SDO_DRIVE3_INDEX,
                                            AZDXA_KED_MIN_LIMIT_SDO_DRIVE3_SUB_INDEX, map_drive_neg_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_MAX_LIMIT_SDO_DRIVE3_INDEX,
                                            AZDXA_KED_MAX_LIMIT_SDO_DRIVE3_SUB_INDEX, map_drive_pos_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_DIRECTION_SDO_DRIVE3_INDEX,
                                            AZDXA_KED_DIRECTION_SDO_DRIVE3_SUB_INDEX, map_drive_direction[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
                case 4:
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_MIN_LIMIT_SDO_DRIVE4_INDEX,
                                            AZDXA_KED_MIN_LIMIT_SDO_DRIVE4_SUB_INDEX, map_drive_neg_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_MAX_LIMIT_SDO_DRIVE4_INDEX,
                                            AZDXA_KED_MAX_LIMIT_SDO_DRIVE4_SUB_INDEX, map_drive_pos_limit[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    if (!ec_sdo_write_int32(slave, AZDXA_KED_DIRECTION_SDO_DRIVE4_INDEX,
                                            AZDXA_KED_DIRECTION_SDO_DRIVE4_SUB_INDEX, map_drive_direction[i])) {
                        return E_SDO_WRITE_FAILURE;
                    }
                    break;
                default:
                    LL_ERROR(GBEM_GEN_LOG_EN, "Sub-drive index out of range");
                    return E_SUB_DRIVE_OUT_OF_RANGE;
            }
        }
    }
    gberror_t rc = ec_write_nvram_azdxa_ked(slave);
    if (rc != E_SUCCESS) {

        return E_NVRAM_WRITE_FAILURE;
    } else {
        return E_SUCCESS;
    }
}

//gberror_t map_drive_homing_exec_azd(uint16_t drive) {
//    static uint64_t homing_cycle_count_ms = 0;
//    //just do for drive 1
//    static int state = 0;
//    //set moo
//    uint16_t ctrl_word = 0;
//    uint16_t stat_word = 0;
//
//    if (homing_cycle_count_ms > DRIVE_HOMING_TIMEOUT_SECS) {
//        //homing failed
//
//    }
//
//    //get to openabled!
//
//    switch (state) {
//        case 0:
//            ec_pdo_set_output_int8(map_drive_to_slave[drive], AZD_MOOSET_PDO_DRIVE1_INDEX, CIA_MOO_HOMING);
//            if (ec_pdo_get_input_int8(map_drive_to_slave[drive], AZD_MOODISP_PDO_DRIVE1_INDEX) == CIA_MOO_HOMING) {
//                state = 1;
//            }
//            break;
//        case 1:
//            BIT_SET(ctrl_word, AZD_HOMING_BIT_NUM);
//            ec_pdo_set_output_uint16(map_drive_to_slave[drive], AZD_CONTROLWORD_PDO_DRIVE1_INDEX, ctrl_word);
//            stat_word = ec_pdo_get_input_uint16(map_drive_to_slave[drive], AZD_STATUSWORD_PDO_DRIVE1_INDEX);
//
//            if (BIT_CHECK(stat_word, AZD_HOMING_DONE_BIT_NUM) {
//                state = 2;
//            }
//            break;
//        case 2:
//            //homing succeeded
//            ec_rxtx_mode = EC_RXTX_OPMODE;
//            break;
//        default:
//            break;
//    }
//    homing_cycle_count_ms = homing_cycle_count_ms + MAP_CYCLE_TIME;
//
//
//    return E_SUCCESS;
//}
//

/*array mapping an AZD drive error code to a text string */
const azdxa_ked_error_string_t azdxa_ked_alarm_code[NUM_OF_AZD_ERROR_STRINGS] = {
        {0x0,  "AZD: No warning"},
        {0x10, "AZD: Excessive position deviation"},
        {0x20, "AZD: Overcurrent"},
        {0x21, "AZD: Main circuit overheat"},
        {0x22, "AZD: Overvoltage"},
        {0x23, "AZD: Main power off"},
        {0x25, "AZD: Undervoltage"},
        {0x26, "AZD: Motor overheat"},
        {0x28, "AZD: Sensor error"},
        {0x29, "AZD: CPU peripheral circuit error"},
        {0x2A, "AZD: ABZO sensor communication error"},
        {0x30, "AZD: Overload"},
        {0x31, "AZD: Overspeed"},
        {0x33, "AZD: Absolute position error"},
        {0x34, "AZD: Command pulse error"},
        {0x35, "AZD: Alarm detection OF other axes"},
        {0x41, "AZD: EEPROM error"},
        {0x42, "AZD: Sensor error AT power on"},
        {0x43, "AZD: Rotation error AT power on"},
        {0x44, "AZD: Encoder EEPROM error"},
        {0x45, "AZD: Motor combination error"},
        {0x4A, "AZD: RETURN-TO-home incomplete"},
        {0x51, "AZD: Regeneration resistor overheat"},
        {0x54, "AZD: Fan rotation deterioration"},
        {0x60, "AZD: +/-LS both sides active"},
        {0x61, "AZD: Reverse +/-LS connection"},
        {0x62, "AZD: RETURN-TO-home operation error"},
        {0x63, "AZD: No HOMES"},
        {0x64, "AZD: TIM, Z, SLIT signal error"},
        {0x66, "AZD: Hardware overtravel"},
        {0x67, "AZD: Software overtravel"},
        {0x6A, "AZD: RETURN-TO-home operation offset error"},
        {0x6D, "AZD: Mechanical overtravel"},
        {0x70, "AZD: Operation data error"},
        {0x71, "AZD: Electronic gear setting error"},
        {0x72, "AZD: Wrap setting error"},
        {0x81, "AZD: Network bus error"},
        {0x82, "AZD: Driver INTERNAL communication error 1"},
        {0x84, "AZD: AZD: Driver INTERNAL communication error 2"},
        {0x85, "AZD: Driver INTERNAL communication timeout"},
        {0xF0, "AZD: CPU error"},
        {0xFF, "AZD: End of error code list marker"}
};
