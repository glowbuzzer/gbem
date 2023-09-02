
/**
 ******************************************************************************
 * @file           :  AX5101.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "log.h"
#include "std_defs_and_macros.h"
#include "ethercatsetget.h"
#include "cia402.h"
#include "etg1020_diag.h"
#include "log.h"
#include "user_message.h"
#include "string.h"
#include "beckhoff.h"
#include "ecm_status.h"

//see - https://github.com/sittner/linuxcnc-ethercat/blob/master/src/lcec_ax5200.c

//These drives are a little different
//From what I have understood so far, the particularity of this drive seems to be that it requires a start-up list to be sent
// and furthermore it takes quite a while to transition between the different states.

// I exported the Mailbox Init commands from twincat. The same is sent to drive using ec_SoeWrite function.

/*
The only essential IDNS required to bring AX5206 to Op mode are:


uint8 SoEWrData4[8] ={0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
retval += ec_SoEwrite( 1, 0, 64, 32778, 8, SoEWrData4, EC_TIMEOUTRXM );

retval += ec_SoEwrite ( 1, 1, 64, 32778, 8, SoEWrData4, EC_TIMEOUTRXM );

uint16 SoEWrData5 = 0x07D0;
retval += ec_SoEwrite ( 1, 0, 64, 1, 2, &SoEWrData5, EC_TIMEOUTRXM );

SoEWrData5 = 0x07D0;
retval += ec_SoEwrite ( 1, 0, 64, 2, 2, &SoEWrData5, EC_TIMEOUTRXM );

uint16 SoEWrData6 = 0x000B;
retval += ec_SoEwrite ( 1, 0, 64, 32, 2, &SoEWrData6, EC_TIMEOUTRXM );

retval += ec_SoEwrite ( 1, 1, 64, 32, 2, &SoEWrData6, EC_TIMEOUTRXM );

*/



//int t2::InitEtherCAT ( char *ifname )
//{
//    int i;
//    String str ;
//    // initialise SOEM, bind socket to ifname
//
//    if ( ec_init ( ifname ) )
//    {
//        str << Format ( "ec_init on %s succeeded.\n" , ifname );
//        // find and auto-config slaves
//
//        if ( ec_config_init ( FALSE ) > 0 )
//        {
//            LOG ( Format ( "%d slaves found and configured.\n", ec_slavecount ) );
//            ec_statecheck ( 1, EC_STATE_PRE_OP,  EC_TIMEOUTSTATE );
//            if ( ec_slave[1].state == EC_STATE_PRE_OP )
//            {
//
//                Ax5206Setup ( ); //
//
//                ec_config_map ( &IOmap );
//
//                //ec_statecheck ( 1, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE );
//                ec_configdc();
//                ec_dcsync01 ( 1, TRUE, SYNC0TIME, SYNC1TIME, 50000 );
//
//                /* wait for all slaves to reach SAFE_OP state */
//                ec_statecheck ( 1, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE );
//                ec_statecheck ( 1, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE );
//
//                oloop = ec_slave[1].Obytes;
//                iloop = ec_slave[1].Ibytes;
//
//
//
//                if ( ec_slave[1].state == EC_STATE_SAFE_OP )
//                {
//
//                    /* send one processdata cycle to init SM in slaves */
//                    ec_send_processdata();
//                    ec_receive_processdata ( EC_TIMEOUTRET );
//
//                    ec_slave[1].state = EC_STATE_OPERATIONAL;
//                    /* request OP state for all slaves */
//                    ec_writestate ( 1 );
//                    /* wait for all slaves to reach OP state */
//                    ec_statecheck ( 1, EC_STATE_OPERATIONAL,  EC_TIMEOUTSTATE );
//                    str <<  "Request operational state for all slaves\n" ;
//
//                    if ( ec_slave[0].state == EC_STATE_OPERATIONAL )
//                    {
//                        str << "Operational state reached for all slaves.\n" ;
//                        str <<  ec_slave[1].state ;
//
//                        dorun = 1;
//                        usleep ( 100000 ); // wait for linux to sync on DC
//
//                        for ( int cnt = 1; cnt <= ec_slavecount ; cnt++ )
//                        {
//                            if ( ec_slave[cnt].hasdc )
//                            {
//                                ec_dcsync01 ( cnt, TRUE, SYNC0TIME, SYNC1TIME, 50000 );
//                            }
//                        }
//
//
//                    }
//
//                    else
//                    {
//                        str <<  "Not all slaves reached operational state.\n" ;
//                    }
//                }
//
//                else
//                    str <<  "Not reached Safe Op.\n" ;
//
//            }
//            else 	str <<  "Not reached Pre Op.\n" ;
//
//            ec_readstate();
//
//            le1.Paste ( str.ToWString() );
//        }
//
//        else
//        {
//            str << "No slaves found ...";
//            return -1;
//        }
//
//    }
//
//
//    str.Clear();
//
//    str << "\n\nInput Bytes = " << iloop << "    Output Bytes = " << oloop << "\n";
//    le1.Paste ( str.ToWString() );
//
//}


//the basic configuration that you must send to OP:
//S-0-0015 axis 0 axis 1
//S-0-0016 axis 0 axis 1
//S-0-0024 axis 0 axis 1
//S-0-0001 axis 0
//S-0-0002 axis 0
//S-0-0032 axis 0
//P-0-0304 axis 0
//P-0-0201 axis 0
//P-0-0202 axis 0
//P-0-0203 axis 0
//P-0-0204 axis 0
//
//Sync0 = 250,000
//Sync0_Shift = 0
//Sync1 = 1,750,000
//Sync1_shift = 0



//LL_TRACE(GBEM_FUN_TRACE_LOG_EN ,
//         "GBEM: Linked ec_standard_sdo function: [%s] for slave [%u] (this is controlled by the MACHINE #define)",
//         __FUNCTION__ , slave);



/*
 * Status word:
 * Bit 0: Ready to switch on
 * Bit 1 : Switched on
 * Bit 2 : Operation enabled
 * Bit 3 : Fault
 * Bit 4 : reserved
 * Bit 5 : reserved
 * Bit 6 : Switch on disabled
 * Bit 7 : Warning
 * Bit 8 + 9 : reserved
 * Bit 10 : TxPDOToggle
 * Bit 11 : Internal limit active
 * Bit 12 : Drive follows the command value
 * Bit 13 : Input cycle counter
 * Bit 14 - 15 : reserved
 */

/* Control word:
 * Bit 0 : Switch on
 * Bit 1 : Enable voltage
 * Bit 2 : reserved
 * Bit 3 : Enable operation
 * Bit 4 - 6 : reserved
 * Bit 7 : Fault reset
 * Bit 8 - 15 : reserved
 */

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_AX5101 = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM3_AX5101 = {
        .number_of_entries = 7,
        .SM_assignment_index = 0x1c13};

/* This is used for the fixed POO remapping */
/*
 * 0x1600 - Control word (uint16)
 * 0x1606 - Target position (uint32) <- note the unsigned
 */
uint16_t map_SM2_index_of_assigned_PDO_AX5101[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1600,
        0x1606,
};


uint16_t map_SM3_index_of_assigned_PDO_AX5101[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a00,
        0x1a01,
        0x1a02,
        0x1a03,
        0x1a04,
        0x1a05,
        0x1a06};


const char *AX5101_error_warning_string[27] = {
        "AX5101_ERROR_ADCERROR",
        "AX5101_ERROR_OVERCURRENT",
        "AX5101_ERROR_UNDERVOLTAGE",
        "AX5101_ERROR_OVERVOLTAGE",
        "AX5101_ERROR_OVERTEMPERATURE",
        "AX5101_ERROR_I2TAMPLIFIER",
        "AX5101_ERROR_I2TMOTOR",
        "AX5101_ERROR_ENCODER",
        "AX5101_ERROR_WATCHDOG",
        "AX5101_ERROR_NONE",
        "AX5101_WARNING_UNDERVOLTAGE",
        "AX5101_WARNING_OVERVOLTAGE",
        "AX5101_WARNING_OVERTEMPERATURE",
        "AX5101_WARNING_I2TAMPLIFIER",
        "AX5101_WARNING_I2TMOTOR",
        "AX5101_WARNING_ENCODER",
        "AX5101_WARNING_NONE",
};

/**
 * @brief examines the infodata 1 and infodata2 PDO objects (if configured for error and warning) and decodes the bit pattern into an enum for warning and errrors
 * @param drive_number
 * @return an enum of the different warnings and errors
 * @warning returns only the first error or warning encountered!
 * @warning ignores 0 or 256?? todo
 */
AX5101_error_warning_t ec_warning_or_error_decode_AX5101(const uint16_t drive) {

//    printf("7211 decode\n");

    uint16_t infodata1 = ec_pdo_get_input_uint16(map_drive_to_slave[drive], AX5101_INFO_DATA1_PDO_INDEX);
    uint16_t infodata2 = ec_pdo_get_input_uint16(map_drive_to_slave[drive], AX5101_INFO_DATA2_PDO_INDEX);
//    printf("infodata1:%u\n", infodata1);
//    printf("infodata2:%u\n", infodata2);
    if ((infodata1 != 0) || (infodata2 != 0)) {

        LL_WARN(GBEM_GEN_LOG_EN, "GBEM: AX5101 infodata contained an error or warning");

        if (BIT_CHECK(infodata1, AX5101_ERROR_ADCERROR_BIT_NUM)) {
            return AX5101_ERROR_ADCERROR;
        }
        if (BIT_CHECK(infodata1, AX5101_ERROR_OVERCURRENT_BIT_NUM)) {
            return AX5101_ERROR_OVERCURRENT;
        }
        if (BIT_CHECK(infodata1, AX5101_ERROR_UNDERVOLTAGE_BIT_NUM)) {
            return AX5101_ERROR_UNDERVOLTAGE;
        }
        if (BIT_CHECK(infodata1, AX5101_ERROR_OVERVOLATGE_BIT_NUM)) {
            return AX5101_ERROR_OVERVOLTAGE;
        }
        if (BIT_CHECK(infodata1, AX5101_ERROR_OVERTEMPERATURE_BIT_NUM)) {
            return AX5101_ERROR_OVERTEMPERATURE;
        }
        if (BIT_CHECK(infodata1, AX5101_ERROR_I2TAMPLIFER_BIT_NUM)) {
            return AX5101_ERROR_I2TAMPLIFIER;
        }
        if (BIT_CHECK(infodata1, AX5101_ERROR_I2TMOTOR_BIT_NUM)) {
            return AX5101_ERROR_I2TMOTOR;
        }
        if (BIT_CHECK(infodata1, AX5101_ERROR_ENCODER_BIT_NUM)) {
            return AX5101_ERROR_ENCODER;
        }
        if (BIT_CHECK(infodata1, AX5101_ERROR_WATCHDOG_BIT_NUM)) {
            return AX5101_ERROR_WATCHDOG;
        }
        if (BIT_CHECK(infodata2, AX5101_WARNING_UNDERVOLATGE_BIT_NUM)) {
            return AX5101_WARNING_UNDERVOLATGE;
        }
        if (BIT_CHECK(infodata2, AX5101_WARNING_OVERVOLTAGE_BIT_NUM)) {
            return AX5101_WARNING_OVERVOLTAGE;
        }
        if (BIT_CHECK(infodata2, AX5101_WARNING_OVERTEMPERATURE_BIT_NUM)) {
            return AX5101_WARNING_OVERTEMPERATURE;
        }
        if (BIT_CHECK(infodata2, AX5101_WARNING_I2TAMPLIFIER_BIT_NUM)) {
            return AX5101_WARNING_I2TAMPLIFIER;
        }
        if (BIT_CHECK(infodata2, AX5101_WARNING_I2TMOTOR_BIT_NUM)) {
            return AX5101_WARNING_I2TMOTOR;
        }
        if (BIT_CHECK(infodata2, AX5101_WARNING_ENCODER_BIT_NUM)) {
            return AX5101_WARNING_ENCODER;
        }
    }
    return AX5101_ERROR_NONE;
}

gberror_t ec_initial_pdo_AX5101(const uint16_t slave) {
//we dont do any initial PDO writes on the AX5101
    return E_NOT_IMPLEMENTED;
}

gberror_t ec_standard_sdos_AX5101(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: Standard SDOs configured for AX5101 slave [%u] are:", slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying standard SDOs to AX5101 slave [%u]", slave);
    }

    /*The object 0xF081:01 (Download revision) describes the revision of the terminal / module, e.g.
* 0x00>18<00>0A< for EL7201-0010-0024, and is necessary to ensure compatibility.
 *
 * 9014 = 0x2336
 * rev 27 = 0x18
 *
 *AX5101-9014-0027
*/

//    0x001B233

    if (!ec_sdo_write_uint32(slave, 0xF081, 0x1, 0x001F2336)) {
//if (!ec_sdo_write_uint32(slave, 0xF081, 0x1, 0x001B233)){

        return E_SDO_WRITE_FAILURE;
    }




//if (!ec_sdo_write_uint8(map_controllers[controller_number], AX5101_AUTO_CONFIG_MOTOR_INDEX, AX5101_AUTO_CONFIG_MOTOR_SUB_INDEX,AX5101_AUTO_CONFIG_MOTOR_VALUE)){
//    return false;
//}

//if (!ec_sdo_write_uint8(map_controllers[controller_number], AX5101_AUTO_CONFIG_IDENTICAL_MOTOR_INDEX, AX5101_AUTO_CONFIG_IDENTICAL_MOTOR_SUB_INDEX, AX5101_AUTO_CONFIG_IDENTICAL_MOTOR_VALUE)){
//    return false;
//}

//    if (!ec_sdo_write_uint8(map_controllers[controller_number], AX5101_AUTO_CONFIG_NONIDENTICAL_MOTOR_INDEX,AX5101_AUTO_CONFIG_NONIDENTICAL_MOTOR_SUB_INDEX,AX5101_AUTO_CONFIG_NONIDENTICAL_MOTOR_VALUE)){
//        return false;
//    }


    //select infodata 1
    //0x8010:0x39
    // uint8
    // value - 0x05 = errors
    if (!ec_sdo_write_uint8(slave, 0x8010, 0x39, 0x05)) {
        return E_SDO_WRITE_FAILURE;
    }

    //select infodata 2
    //0x8010:0x3A
    // uint8
    // value - 0x06 = warnings
    if (!ec_sdo_write_uint8(slave, 0x8010, 0x3A, 0x06)) {
        return E_SDO_WRITE_FAILURE;
    }

//clear diag
//0xFB00:0x01
//uin16t
//value - 0x1000
//    if (!ec_sdo_write_uint16(map_controllers[controller_number], 0xFB00, 0x01, 0x1000)) {
//        return false;
//    }

//    uint8_t val[2]={0x4, 0x0};
//
//    int mos = sizeof(val);
//


//fault reaction option code
//0x8010:52
//uint16
//value 0 disable drive function 1 slow down ramp
    if (!ec_sdo_write_uint16(slave, 0x8010, 0x52, 1)) {
        return E_SDO_WRITE_FAILURE;
    }

//diag message flags
//    0x10F3:0x05
//    uinit16
// value 0x1 - send emergency messages over CoE

    if (!ec_sdo_write_uint16(slave, 0x10F3, 0x05, 0x1)) {
        return E_SDO_WRITE_FAILURE;
    }

//Modes of oepration
//0x7010:0x3
//uint8
//0x08
//    if (!ec_sdo_write_uint8(map_controllers[controller_number], 0x7010, 0x3, CIA_MOO_CSP)) {
//        return false;
//    }

//next two from github
//rated current
//0x8011:0x12
//uint32
//0x00001F40
    if (!ec_sdo_write_uint32(slave, 0x8011, 0x12, 0x00001F40)) {
        return E_SDO_WRITE_FAILURE;
    }

//Position loop proportional gain
//0x8010:17
//uint32
//0xa=10
    if (!ec_sdo_write_uint32(slave, 0x8010, 0x17, 0x0000000A)) {
        return E_SDO_WRITE_FAILURE;
    }

//Torque limitation
//0x7010:0b
//uint16
//0xbb8 =
    if (!ec_sdo_write_uint16(slave, 0x7010, 0x0b, 0xbb8)) {
        return E_SDO_WRITE_FAILURE;
    }

//Velocity limitation
//0x8010:31
//uint32
//0x00040000
    if (!ec_sdo_write_uint32(slave, 0x8010, 0x31, 0x00040000)) {
        return E_SDO_WRITE_FAILURE;
    }
//motor pole pairs
//0x8011:0x13
//uint8
//0x4
    if (!ec_sdo_write_uint8(slave, 0x8011, 0x13, 0x4)) {
        return E_SDO_WRITE_FAILURE;
    }

//max current
//0x8011:0x11
//uint32
//0x00008D68 =36000 (mA)
    if (!ec_sdo_write_uint32(slave, 0x8011, 0x11, 0x00008D68)) {
        return E_SDO_WRITE_FAILURE;
    }

//Torque constant
//0x8011:0x16
//unit32
//0x0000012C
    if (!ec_sdo_write_uint32(slave, 0x8011, 0x16, 0x0000012C)) {
        return E_SDO_WRITE_FAILURE;
    }

//Winding inductance
//0x8011:0x19
//unit16
//0x0023
    if (!ec_sdo_write_uint16(slave, 0x8011, 0x19, 0x0023)) {
        return E_SDO_WRITE_FAILURE;
    }

//Rotor moment of inertia
//0x8011:0x18
//unit32
//0x00000442
    if (!ec_sdo_write_uint32(slave, 0x8011, 0x18, 0x00000442)) {
        return E_SDO_WRITE_FAILURE;
    }
//Motor thermal time constant
//0x8011:0x2d
//uint16
//0x04CE
    if (!ec_sdo_write_uint16(slave, 0x8011, 0x2d, 0x04CE)) {
        return E_SDO_WRITE_FAILURE;
    }

//Commutation offset
//0x8011:0x15
//unit16
//0x010E = 270 (degrees)
    if (!ec_sdo_write_uint16(slave, 0x8011, 0x15, 0x010E)) {
        return E_SDO_WRITE_FAILURE;
    }

//Motor speed limitation
//0x8011:0x1b
//unit32
//0x00000483=1155

    if (!ec_sdo_write_uint32(slave, 0x8011, 0x1b, 0x00000483)) {
        return E_SDO_WRITE_FAILURE;
    }
//Current loop proportional gain - Kp
//0x8010:0x13
//unit16
//0x258=600 = 60.0
//0x019F = 415 = 41.5
//0x64 = 100 = 10.0
//0x3e8 = 1000 = 100.0
//0xc8 = 200 - 20.0
//2bc = 700 = 70.0

    if (!ec_sdo_write_uint16(slave, 0x8010, 0x13, 0x064)) {
        return E_SDO_WRITE_FAILURE;
    }
//Current loop integral time - Tn
//0x8010:0x12
//uint16
//0x0005
//0x1f40=8000=0.8
    if (!ec_sdo_write_uint16(slave, 0x8010, 0x12, 0x000A)) {
        return E_SDO_WRITE_FAILURE;
    }
//Velocity loop proportional gain - Kp
//0x8010:0x14
//uint32
//0x00000141 = 321
//0x64=100
//0x3e8 = 1000
//0xc8=200=0.2

    if (!ec_sdo_write_uint32(slave, 0x8010, 0x14, 0x96)) {
        return E_SDO_WRITE_FAILURE;
    }
//Velocity loop integral time
//0x8010:0x15
//unit32
//0x00000096=150
//0x1f4=500=
    if (!ec_sdo_write_uint32(slave, 0x8010, 0x15, 0x000032)) {
        return E_SDO_WRITE_FAILURE;
    }

    //8010:15 = vel loop prop gain 0x96
    //810:14 = vel looop integral time 0x32


//mooset
    if (!ec_sdo_write_uint8(slave, AX5101_MOO_SET_SDO_INDEX, AX5101_MOO_SET_SDO_SUB_INDEX,
                            CIA_MOO_CSP)) {
        return E_SDO_WRITE_FAILURE;
    }

    //return true, all applied without error
    return E_SUCCESS;
}

gberror_t ec_pdo_map_AX5101(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for AX5101 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to AX5101 slave [%u]",
                slave);
    }

    if (!ec_sdo_write_uint8(slave, map_SM2_AX5101.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    osal_usleep(500);

    if (!ec_sdo_write_uint8(slave, map_SM3_AX5101.SM_assignment_index, 0, 0)) {
        return E_SDO_WRITE_FAILURE;
    }
    osal_usleep(500);


    for (int i = 0; i < map_SM2_AX5101.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_AX5101.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_AX5101[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }
    osal_usleep(500);
    for (int i = 0; i < map_SM3_AX5101.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_AX5101.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_AX5101[i])) {
            return E_SDO_WRITE_FAILURE;
        }
    }
    osal_usleep(500);
    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */

    if (!ec_sdo_write_uint8(slave, map_SM2_AX5101.SM_assignment_index, 0, map_SM2_AX5101.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }

    osal_usleep(500);

    if (!ec_sdo_write_uint8(slave, map_SM3_AX5101.SM_assignment_index, 0, map_SM3_AX5101.number_of_entries)) {
        return E_SDO_WRITE_FAILURE;
    }
//all applied correctly
    return E_SUCCESS;
}

int8_t ec_get_moo_sdo_AX5101(const uint16_t drive) {

    uint8_t uib8;

    if (!ec_sdo_read_uint8(map_drive_to_slave[drive], AX5101_MOO_GET_SDO_INDEX, AX5101_MOO_GET_SDO_SUB_INDEX, &uib8)) {
        return -1;
    }
    LL_INFO(GBEM_GEN_LOG_EN, "Got MOO for AX5101 (slave %u) - value:%u", map_drive_to_slave[drive], uib8);
    return ((int8_t) uib8);
}

uint8_t *ec_get_error_string_sdo_AX5101(const uint16_t drive) {
    int os;
    char octet_string[29] = {0};
    int rc;
    char time_print_buf[30];
    //these error codes are only accessible through SDO reads and so can't be accessed every cycle
    //it only gets called when in fault state and then only as an entry action


    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);


//    10F3:04 - bool - new message is available
    uint8_t new_message_available = 0;

    bool error_reading_diag = false;

    if (!ec_sdo_read_uint8(map_drive_to_slave[drive], AX5101_ERROR_CODE_SDO_INDEX, 0x04, &new_message_available)) {
        error_reading_diag = true;
        //    printf("AX5101: New message is available: %d\n", uib8);

    }
    uint8_t max_num_messages = 0;

//    // 10F3:01 - uint8 - maximum number of messages - always 50

    if (!ec_sdo_read_uint8(map_drive_to_slave[drive], AX5101_ERROR_CODE_SDO_INDEX, 0x01, &max_num_messages)) {

        error_reading_diag = true;
        //    printf("Maximum number of messages:%d\n", uib8);

    }

//    10F3:02 - uint8 - newest message index

    uint8_t newest_message_index;

    if (!ec_sdo_read_uint8(map_drive_to_slave[drive], AX5101_ERROR_CODE_SDO_INDEX, 0x2, &newest_message_index)) {
        error_reading_diag = true;
        //    printf("Newest message index for diagnostic message:%d\n", newest_message_index);

    }

    if (!error_reading_diag && newest_message_index <= 0x6) {
        os = 28;
//    for (int i = 0; i < NUM_AX5101_ERROR_CODES; i++) {
//        printf("%d\n", i+6);
//        rc = ec_SDOread(map_drives_to_controllers[drive_number], AX5101_ERROR_CODE_INDEX, i+6, false, &os, &octet_string[0][i], EC_TIMEOUTRXM);
//        if (rc <= 0) {
//            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Could not read SDO index:0x%04x - sub-index:0x%04x (on slave:%u", AX5101_ERROR_CODE_INDEX, i+6, map_drives_to_controllers[drive_number]);
//        }
//        for (int j=0; j<28; j++){
//            printf("%d,", octet_string[j][i]);
//        }
//        printf("\n");
//    }


        rc = ec_SDOread(map_drive_to_slave[drive], AX5101_ERROR_CODE_SDO_INDEX, newest_message_index, false,
                        &os,
                        &octet_string,
                        EC_TIMEOUTRXM);
        if (rc <= 0) {
            LL_ERROR(GBEM_GEN_LOG_EN, "Could not read SDO index:0x%04x - sub-index:0x%04x (on slave:%u)",
                     AX5101_ERROR_CODE_SDO_INDEX,
                     newest_message_index, map_drive_to_slave[drive]);
        }

        if (EcatError) {
            LL_INFO(GBEM_GEN_LOG_EN, "GBEM: EtherCAT error detected: %s", ec_elist2string());
        }
//        for (int i = 0; i < 28; i++) {
//            printf("%x", octet_string[i]);
//        }
//        printf("\n");

        etg1020_diag_message_t msg_decode = {0};


        //diag full decode will return false if the text id is 0 or an parse error occurred
        if (etg1020_diag_full_decode((uint8_t *) &octet_string[0], &msg_decode)) {

            strcpy((char *) &error_code_string[0], (char *) msg_decode.diag_message_text);

//            printf("number of params in message:%d\n", msg_decode.diag_flags.number_of_parameters);
//            printf("param type [0] %s\n", etg1020_diag_param_type_string[msg_decode.diag_params[0].param_type]);
//            printf("param length [0] %d bytes\n", msg_decode.diag_params[0].param_length);
//            printf("param value [0] %d\n", msg_decode.diag_params[0].param_value);
//            printf("param type [1] %s\n", etg1020_diag_param_type_string[msg_decode.diag_params[1].param_type]);
//            printf("param length [1] %d bytes\n", msg_decode.diag_params[1].param_length);
//            printf("param value [1] %d\n", msg_decode.diag_params[1].param_value);
//            printf("text id short code:0x%x\n", msg_decode.diag_textid.short_code);
//            printf("text id long code:0x%x\n", msg_decode.diag_textid.long_code);
//            printf("text id x:%s\n", etg1020_diag_x_string[msg_decode.diag_textid.x]);
//            printf("text id y:%s\n", etg1020_diag_y_string[msg_decode.diag_textid.y]);
//            printf("diag code:%s\n", etg1020_diag_code_type_string[msg_decode.diag_code.etg1020_diag_code_type]);
//            //these dont appear to be used in the diag message from beckhoff these use the text id instead
//            printf("diag type:%s\n", etg1020_diag_type_string[msg_decode.diag_flags.etg1020_diag_type]);

            time_t time_stamp_secs = msg_decode.time_stamp_secs;
            struct tm time_struct = {0};

            time_struct = *localtime(&time_stamp_secs);
            strftime(time_print_buf, sizeof(time_print_buf), "%a %Y-%m-%d %H:%M:%S %Z", &time_struct);

//            printf("Time stamp:%s\n", time_print_buf);
//            printf("message:%s\n", msg_decode.diag_message_text);

            strcat((char *) error_code_string, " (");
            strcat((char *) error_code_string, time_print_buf);
            strcat((char *) error_code_string, ")");

        }
    }
    //todo doesn't do anything with warning codes at the moment


    AX5101_error_warning_t errorWarning = ec_warning_or_error_decode_AX5101(drive);
//printf("erroWarning:%u\n", errorWarning);
    //todo this will make the infodata warning take precedence?
    if (errorWarning != AX5101_ERROR_NONE) {
        memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
        strcpy((char *) error_code_string, AX5101_error_warning_string[ec_warning_or_error_decode_AX5101(drive)]);
    }
    return error_code_string;


}

gberror_t ec_nvram_sdos_AX5101(const uint16_t slave) {
    return E_NOT_IMPLEMENTED;
}

gberror_t ec_write_nvram_AX5101(const uint16_t slave) {
    return E_NOT_IMPLEMENTED;
}


/**
 * @brief set control word for AX5101 drive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_AX5101(const uint16_t drive, const uint16_t ctrlwrd) {
    ec_pdo_set_output_uint16(map_drive_to_slave[drive], AX5101_CONTROLWORD_PDO_INDEX, ctrlwrd);
    return E_SUCCESS;
}

/**
 * @brief get status word for AX5101 drive
 * @param drive
 * @return status word
 */
uint16_t ec_get_stat_wrd_AX5101(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AX5101_STATUSWORD_PDO_INDEX);
}

/**
 * @brief get ctrlwrd for for an AX5101 drive
 * @param drive
 * @return ctrlwrd
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
uint16_t ec_get_ctrl_wrd_rev_AX5101(const uint16_t drive) {
    return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AX5101_CONTROLWORD_PDO_INDEX);
}


/**
 * @brief set status word for an AX5101 drive
 * @param drive
 * @param statwrd
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_stat_wrd_rev_AX5101(const uint16_t drive, const uint16_t statwrd) {
    ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AX5101_STATUSWORD_PDO_INDEX, statwrd);
    return E_SUCCESS;
}


/**
 * @brief set actpos for an AX5101 drive
 * @param drive
 * @param actpos
 * @return gberror
 * @warning FREVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_actpos_wrd_rev_AX5101(const uint16_t drive, const int32_t actpos) {

    ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AX5101_ACTPOS_PDO_INDEX, actpos);
    return E_SUCCESS;
}

/**
 * @brief get setpos for an AX5101 drive
 * @param drive
 * @return setpos
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
int32_t ec_get_setpos_word_rev_AX5101(const uint16_t drive) {
    return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AX5101_SETPOS_PDO_INDEX);
}


gberror_t ec_set_moo_pdo_rev_AX5101(const uint16_t drive) {

    return E_NOT_IMPLEMENTED;

}


/**
 * @brief get actpos for an AX5101 drive
 * @param drive
 * @return int32 position
 */

int32_t ec_get_actpos_wrd_AX5101(const uint16_t drive) {
    return ec_pdo_get_input_int32(map_drive_to_slave[drive], AX5101_ACTPOS_PDO_INDEX);
}


/**
 * @brief set setpos for an AX5101 drive
 * @param drive
 * @param setpos
 * @return gberror
 */

gberror_t ec_set_setpos_wrd_AX5101(const uint16_t drive, const int32_t setpos) {
    ec_pdo_set_output_int32(map_drive_to_slave[drive], AX5101_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}


/**
 * @brief get the value of the remote bit from an AX5101 drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_AX5101(const uint16_t drive) {
//there is no remote bit on the AX5101
    return true;
}


/* Following error window (Index
0x8010:50 MDP742 / Index 0x6065 DS402). The value set here – multiplied by the scaling factor –
specifies by what position the actual position may differ from the set position, positively and negatively.
The total accepted tolerance is thus twice as large as the position entered in the Following error
window
*/


/*The time (in ms) allowed for a following error exceedance can be set with the Following error time out
(Index 0x8010:51 MDP742 / Index 0x6066 DS402). As soon as the target position is exceeded by
        more than the position entered in the Following error window for the time entered in the Following error
        time out, the terminal outputs an error and stops immediately.
*/


bool ec_get_follow_error_AX5101(const uint16_t drive) {

    //read follow error bytes and threshold
    //the follow error is large until the drive reaches operation enabled

//printf("follow error El7211 [%d]\n", ec_pdo_get_input_int32(map_drive_to_slave[drive], AX5101_FOLLOWERROR_ACTVAL_PDO_INDEX));

    if (ecm_status.machine_state != CIA_OPERATION_ENABLED) {
        return false;
    }
    if (abs(ec_pdo_get_input_int32(map_drive_to_slave[drive], AX5101_FOLLOWERROR_ACTVAL_PDO_INDEX)) >
        AX5101_FOLLOW_ERROR_TOLERANCE) {
        return true;
    }
    return false;

}
