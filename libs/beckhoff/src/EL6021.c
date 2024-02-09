
/**
 ******************************************************************************
 * @file           :  EL6021
 * @brief          :  1-channel communication interface, serial, RS422/RS485
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "EL6021.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"

#define EL6021_CTRL_PDO_INDEX 0
#define EL6021_DATA_OUT_PDO_INDEX 2

#define EL6021_STATUS_PDO_INDEX 0
#define EL6021_DATA_IN_PDO_INDEX 2

#define EL6021_DATA_SIZE 22

//Control word
#define EL6021_TRANSMIT_REQUEST_BIT_NUM 0 //Via a change of state of this bit the controller notifies the terminal that the DataOut bytes contain the number of bytes indicated via the OL bits. The terminal acknowledges receipt of the data in the status byte via a change of state of bit SW.0 (TA). Only now new data can be transferred from the controller to the terminal.
#define EL6021_RECEIVE_ACCEPTED_BIT_NUM 1 //The controller acknowledges receipt of data by changing the state of this bit. Only then new data can be transferred from the terminal to the controller
#define EL6021_INIT_REQUEST_BIT_NUM 2 //The controller requests terminal for initialization. The send and receive functions are blocked, the FIFO pointers are reset, and the interface is initialized with the values of the responsible objects: baud rate 4073, data frame 4074, feature bits 4075. The terminal acknowledges completion of the initialization via bit SW.2 (IA).
#define EL6021_SEND_CONTINUOUS_BIT_NUM 3 //Continuous sending of data from the FIFO.

//Status word
#define EL6021_TRANSMIT_ACCEPTED_BIT_NUM 0 //The terminal acknowledges receipt of data by changing the state of this bit. Only now new data can be transferred from the controller to the terminal.
#define EL6021_RECEIVE_REQUEST_BIT_NUM 1 //Via a change of state of this bit the terminal notifies the controller that the DataIn bytes contain the number of bytes indicated via the IL bits. The controller has to acknowledge receipt of the data in the control byte via a change of state of bit CW.1 (RA). Only then new data can be transferred from the terminal to the controller.
#define EL6021_INIT_ACCEPTED_BIT_NUM 2 //Initialization was completed by the terminal.
#define EL6021_RX_FIFO_FULL_BIT_NUM 3 //The reception FIFO is full. All further incoming data will be lost!


//uint16_t
#define EL6021_BAUD_RATE_SDO_INDEX 0x4073
#define EL6021_BAUD_RATE_SDO_SUB_INDEX 0x0

//uint16_t
#define EL6021_DATA_FRAME_SDO_INDEX 0x4074
#define EL6021_DATA_FRAME_SDO_SUB_INDEX 0x0


/** !!!!!!!!!COM SETTINGS later hardware!!!!!  */


//bool FALSE RTS/CTS not enabled, TRUE RTS/CTS not enabled
#define EL6021_FEATURE_BITS_SDO_INDEX 0x8000
#define EL6021_FEATURE_BITS_SDO_SUB_INDEX 0x00

//FALSE XON/XOFF is not supported for send data, FALSE XON/XOFF is not supported for send data
#define EL6021_XON_XOFF_TX_SDO_BIT_NUM 0

//FALSE XON/XOFF is not supported for receive data, FALSE XON/XOFF is not supported for receive data
#define EL6021_XON_XOFF_RX_SDO_BIT_NUM 1

//bool FALSE Full-duplex, TRUE half uplex - default 0
#define EL6021_DUPLEX_SDO_BIT_NUM 2

//TRUE RS422 - default 0
#define EL6021_RS422_SDO_BIT_NUM 3

#define EL6021_CONTINUOUS_SEND_SDO_BIT_NUM 4

#define EL6021_ENABLE_OPTIMISATION_SDO_BIT_NUM 5


/*
3 state machines
main
read
write


main

0 - STANDBY
reset m_connected
wait for m_connect = true --> 1
1 - SET CONFIG
set baud rate and data frame via SDO's
wait for Status.InitAccepted (SW.2) = false --> 2
2 - APPLY CONFIG
set Control.InitRequest (CW.2)
wait for Status.InitAccepted (SW.2) = true --> 3
3 - CONFIGURED
clear Control.InitRequest (CW.2)
clear Control.TransmitRequest (CW.0)
clear Control.ReceiveAccepted (CW.1)
reset m_transmitAccepted
reset m_receiveRequest
wait for Status.InitAccepted (SW.2) = false --> 4
4 - READY
set m_connected
wait for m_connect = false --> 0

Read State Machine

0 - STANDBY
wait for m_connected = true --> 1
1 - READY
wait for
{
if m_connected = false --> 0
if m_receiveRequest != Status.ReceiveRequest (SW.1)
{
set m_receiveRequest = Status.ReceiveRequest (SW.1)
--> 2
}
if Status.ReceiveBufferFull (SW.3) --> 2
}
2 - READ DATA
cnt = Status.ReceiveCount (SW.8..SW.15)
for (i = 0; i < cnt; i++)
{
if (m_readCnt >= COMM_PORT_BUFF_SIZE)
{
m_readErrors++
break;
}
idx = (m_readIdx + m_readCnt) % COMM_PORT_BUFF_SIZE
m_readData[idx] = ReceiveChar[i]
m_readCnt++
}
set Control.ReceiveAccepted (CW.1) = ! Control.ReceiveAccepted (CW.1)
--> 1


Write State Machine

0 - STANDBY
wait for m_connected = true -->1
1 - READY
wait for
{
if m_connected = false --> 0
if m_writeCnt > 0 --> 2
}
2 - WRITE DATA
cnt = min(m_writeCnt, 22)
set Control.TransmitCount (CW.8..CW..15) = cnt
for (i = 0; i < cnt; i++)
{
TransmitChar[i] = m_writeData[m_writeIdx]
m_writeIdx = (m_writeIdx + 1) % COMM_PORT_BUFF_SIZE
m_writeCnt--
}
set Control.TransmitRequest (CW.0) = ! Control.TransmitRequest (CW.0)
wait for
{
if (TIME_IN_STEP > 1 second)
{
set Control.TransmitRequest (CW.0) = ! Control.TransmitRequest (CW.0)
reset TIME_IN_STEP
}
if m_connect = false --> 0
if m_transmitAccepted != Status.TransmitAccepted (SW.0)
{
m_transmitAccepted != m_transmitAccepted
--> 1
}
}

bool m_connect
bool m_connected
bool m_transmitAccepted
bool m_receiveRequest

int m_readCnt
int m_readErrors
int m_readIdx
int m_readCnt
char m_readData[COMM_PORT_BUFF_SIZE]

int m_writeIdx
int m_writeCnt
char m_writeData[COMM_PORT_BUFF_SIZE]

 */


typedef enum {
EL6021_BAUD_RATE_2400=4,
    EL6021_BAUD_RATE_4800=5,
    EL6021_BAUD_RATE_9600=6,
    EL6021_BAUD_RATE_19200=7,
    EL6021_BAUD_RATE_38400=8,
    EL6021_BAUD_RATE_57600=9,
    EL6021_BAUD_RATE_115200=10,
}el6021_baud_rates_t

typedef enum {
    EL6021_ENCODING_0=4,
    EL6021_ENCODING_7E1=1,
    EL6021_ENCODING_7O1=2, EL6021_ENCODING_8N1=3, EL6021_ENCODING_8E1=4, EL6021_ENCODING_8O1=5, EL6021_ENCODING_7E2=9, EL6021_ENCODING_7O2=10, EL6021_ENCODING_8N2=11, EL6021_ENCODING_8E2=12, EL6021_ENCODING_8O2=13, EL6021_ENCODING_8S1=18, EL6021_ENCODING_8M1=19
    }el6021_encoding_t


/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_el6021 = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c12};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_el6021 = {
        .number_of_entries = 2,
        .SM_assignment_index = 0x1c13};


uint16_t map_SM2_index_of_assigned_PDO_el6021[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1602,
        0x1603};

uint16_t map_SM3_index_of_assigned_PDO_el6021[ECM_MAX_PDO_MAPPING_ENTRIES] = {
        0x1a03,
        0x1a07};

gberror_t ec_pdo_map_elel6021(const uint16_t slave) {

    if (ec_printSDO) {
        UM_INFO(GBEM_UM_EN, "GBEM: SODs configured for PDO mapping for EL6021 slave [%u] are:",
                slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Applying PDO mapping to EL6021 slave [%u]",
                slave);
    }

    // map_SM2.SM_assignment_index

    if (!ec_sdo_write_uint16(slave, map_SM2_el6021.SM_assignment_index, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    if (!ec_sdo_write_uint16(slave, map_SM3_el6021.SM_assignment_index, 0, 0, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    for (int i = 0; i < map_SM2_el6021.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM2_el6021.SM_assignment_index, i + 1,
                                 map_SM2_index_of_assigned_PDO_el6021[i], true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    for (int i = 0; i < map_SM3_el6021.number_of_entries; i++) {
        if (!ec_sdo_write_uint16(slave, map_SM3_el6021.SM_assignment_index, i + 1,
                                 map_SM3_index_of_assigned_PDO_el6021[i], true)) {
            return E_SDO_WRITE_FAILURE;
        }
    }

    /*
     * set the SM2 & SM3 assignment object number of entries to actual number (sub-index 0)
     */
    if (!ec_sdo_write_uint16(slave, map_SM2_el6021.SM_assignment_index, 0,
                             map_SM2_el6021.number_of_entries, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    if (!ec_sdo_write_uint16(slave, map_SM3_el6021.SM_assignment_index, 0,
                             map_SM3_el6021.number_of_entries, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //all applied correctly if here
    return E_SUCCESS;


}


gberror_t ec_standard_sdos_el6021(const uint16_t slave) {


uint8_t feature_bits = 0;

BIT_SET(


    if (!ec_sdo_write_uint8_t(slave, EL7031_MAX_CURRENT_SDO_INDEX, EL7031_MAX_CURRENT_SDO_SUB_INDEX,
                             EL7031_MAX_CURRENT_SDO_VALUE, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    // if (!ec_sdo_write_uint16(slave, EL7031_MAX_CURRENT_SDO_INDEX, EL7031_MAX_CURRENT_SDO_SUB_INDEX,
    //                          EL7031_MAX_CURRENT_SDO_VALUE, true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    // if (!ec_sdo_write_uint16(slave, EL7031_REDUCED_CURRENT_SDO_INDEX, EL7031_REDUCED_CURRENT_SDO_SUB_INDEX,
    //                          EL7031_REDUCED_CURRENT_SDO_VALUE, true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    // if (!ec_sdo_write_uint16(slave, EL7031_NOMINAL_VOLTAGE_SDO_INDEX, EL7031_NOMINAL_VOLTAGE_SDO_SUB_INDEX,
    //                          EL7031_NOMINAL_VOLTAGE_SDO_VALUE, true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    // if (!ec_sdo_write_uint16(slave, EL7031_MOTOR_COIL_RESISTANCE_SDO_INDEX, EL7031_MOTOR_COIL_RESISTANCE_SDO_SUB_INDEX,
    //                          EL7031_MOTOR_COIL_RESISTANCE_SDO_VALUE, true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    // if (!ec_sdo_write_uint16(slave, EL7031_MOTOR_FULL_STEPS_SDO_INDEX, EL7031_MOTOR_FULL_STEPS_SDO_SUB_INDEX,
    //                          EL7031_MOTOR_FULL_STEPS_SDO_VALUE, true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    // if (!ec_sdo_write_uint8(slave, EL7031_OPERATION_MODE_SDO_INDEX, EL7031_OPERATION_MODE_SDO_SUB_INDEX,
    //                         EL7031_OPERATION_MODE_SDO_VALUE, true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    // if (!ec_sdo_write_uint8(slave, EL7031_SPEED_RANGE_SDO_INDEX, EL7031_SPEED_RANGE_SDO_SUB_INDEX,
    //                         EL7031_SPEED_RANGE_SDO_VALUE, true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    // if (!ec_sdo_write_uint8(slave, EL7031_INVERT_MOTOR_POLARITY_SDO_INDEX, EL7031_INVERT_MOTOR_POLARITY_SDO_SUB_INDEX,
    //                         map_drive_direction[slave - 1], true)) {
    //     return E_SDO_WRITE_FAILURE;
    // }
    //all applied correctly
    return E_SUCCESS;

}


// uint8_t *ec_get_error_string_sdo_el7031(const uint16_t drive) {
//
//
//     uint64_t read_val = 0;
//     int size = EL7031_DIAG_SIZE; //48bits
//     //read in complete access mode
//     int rc = ec_SDOread(map_drive_to_slave[drive], EL7031_DIAG_SDO_INDEX, EL7031_DIAG_SDO_SUB_INDEX, true, &size,
//                         &read_val, EC_TIMEOUTRXM);
//
//     if (rc <= 0) {
//         return (uint8_t *) "Can't read EL7031 error code from SDO";
//     }
//
//     if (BIT_CHECK(read_val, EL7031_ERROR_SATURATED_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Saturated error";
//     }
//     if (BIT_CHECK(read_val, EL7031_ERROR_OVER_TEMPERATURE_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Over temperature error";
//     }
//
//     if (BIT_CHECK(read_val, EL7031_ERROR_TORQUE_OVERLOAD_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Torque overload error";
//     }
//
//     if (BIT_CHECK(read_val, EL7031_ERROR_UNDER_VOLTAGE_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Under voltage error";
//     }
//     if (BIT_CHECK(read_val, EL7031_ERROR_OVER_VOLTAGE_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Over volatge error";
//     }
//
//     if (BIT_CHECK(read_val, EL7031_ERROR_SHORT_CIRCUIT_A_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Short circuit A error";
//     }
//
//     if (BIT_CHECK(read_val, EL7031_ERROR_SHORT_CIRCUIT_B_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Short circuit B error";
//     }
//
//     if (BIT_CHECK(read_val, EL7031_ERROR_NO_CONTROL_POWER_BIT_NUM)) {
//         return (uint8_t *) "EL7031: No control power error";
//     }
//
//     if (BIT_CHECK(read_val, EL7031_ERROR_MISC_ERROR_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Misc error";
//     }
//
//     if (BIT_CHECK(read_val, EL7031_ERROR_CONFIG_ERROR_BIT_NUM)) {
//         return (uint8_t *) "EL7031: Config error";
//     }
//
//     return (uint8_t *) "EL7031: Unknown error";
//
//
// }

