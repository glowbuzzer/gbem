
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
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"
#include "map.h"
#include "dpm.h"


// We should flip CW.0 when we have data to send - slave then flips SW.0 to ack this
// Then when there is rx data, (this is indicated by teh size in teh status word) we read teh data then flip CW.1 - slave acks this with flip of SW.1 and resets the length in the status word to zero

// /** !!!!!!!!!COM SETTINGS later hardware!!!!!  */
//
//
// //bool FALSE RTS/CTS not enabled, TRUE RTS/CTS not enabled
//
// //FALSE XON/XOFF is not supported for send data, FALSE XON/XOFF is not supported for send data
// #define EL6021_XON_XOFF_TX_SDO_BIT_NUM 0
//
// //FALSE XON/XOFF is not supported for receive data, FALSE XON/XOFF is not supported for receive data
// #define EL6021_XON_XOFF_RX_SDO_BIT_NUM 1
//
// //bool FALSE Full-duplex, TRUE half uplex - default 0
// #define EL6021_DUPLEX_SDO_BIT_NUM 2
//
// //TRUE RS422 - default 0
// #define EL6021_RS422_SDO_BIT_NUM 3
//
// #define EL6021_CONTINUOUS_SEND_SDO_BIT_NUM 4
//
// #define EL6021_ENABLE_OPTIMISATION_SDO_BIT_NUM 5


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


static uint16_t ec_get_status_word_el6021(uint16_t slave);

static gberror_t ec_set_control_word_el6021(uint16_t slave, const uint16_t control_word);

static gberror_t ec_set_data_el6021(uint16_t slave, uint8_t *data, uint8_t data_size);

static gberror_t ec_get_data_el6021(uint16_t slave, uint8_t *data, uint8_t data_size);

uint8_t ec_get_data_size_el6021(uint16_t *word);


gberror_t ec_set_data_size_el6021(uint16_t *word, uint8_t size);


gberror_t ec_set_data_size_el6021(uint16_t *word, uint8_t size) {
    *word = (uint16_t) ((size << 8) | (*word & 0x00FF));
    return E_SUCCESS;
}


uint8_t ec_get_data_size_el6021(uint16_t *word) {
    uint8_t size = (uint8_t) (*word >> 8);
    return size;
}


typedef enum {
    EL6021_BAUD_RATE_2400 = 4,
    EL6021_BAUD_RATE_4800 = 5,
    EL6021_BAUD_RATE_9600 = 6,
    EL6021_BAUD_RATE_19200 = 7,
    EL6021_BAUD_RATE_38400 = 8,
    EL6021_BAUD_RATE_57600 = 9,
    EL6021_BAUD_RATE_115200 = 10,
} el6021_baud_rates_t;

typedef enum {
    EL6021_ENCODING_0 = 4,
    EL6021_ENCODING_7E1 = 1,
    EL6021_ENCODING_7O1 = 2, EL6021_ENCODING_8N1 = 3, EL6021_ENCODING_8E1 = 4, EL6021_ENCODING_8O1 = 5,
    EL6021_ENCODING_7E2 = 9, EL6021_ENCODING_7O2 = 10, EL6021_ENCODING_8N2 = 11, EL6021_ENCODING_8E2 = 12,
    EL6021_ENCODING_8O2 = 13, EL6021_ENCODING_8S1 = 18, EL6021_ENCODING_8M1 = 19
} el6021_encoding_t;


/* This is used for the fixed POO remapping */
map_SM_assignment_object_t map_SM2_el6021 = {
    .number_of_entries = 2,
    .SM_assignment_index = 0x1c12
};

/* This is used for the fixed PDO remapping */
map_SM_assignment_object_t map_SM3_el6021 = {
    .number_of_entries = 2,
    .SM_assignment_index = 0x1c13
};


uint16_t map_SM2_index_of_assigned_PDO_el6021[ECM_MAX_PDO_MAPPING_ENTRIES] = {
    0x1602,
    0x1603
};

uint16_t map_SM3_index_of_assigned_PDO_el6021[ECM_MAX_PDO_MAPPING_ENTRIES] = {
    0x1a03,
    0x1a07
};

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

    if (!ec_sdo_write_uint16(slave, 0x1c12, 0, 0, true)) {
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


void print_status_word(uint16_t status_word) {
    printf("Status word: ");
    for (int i = 15; i >= 0; i--) {
        if (BIT_CHECK(status_word, i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
}


void print_control_word(uint16_t control_word) {
    printf("Control word: ");
    for (int i = 15; i >= 0; i--) {
        if (BIT_CHECK(control_word, i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}


gberror_t ec_slave_exec_el6021(uint16_t slave) {
    uint16_t el6021_status_word = ec_get_status_word_el6021(slave);
    uint16_t el6021_control_word = 0;


    // print_status_word(el6021_status_word);

    //RECEIVE
    uint8_t rx_size = ec_get_data_size_el6021(&el6021_status_word);

    if (rx_size > 0) {
        // printf("rx size = [%u]\n", rx_size);
    }


    //put rx_size in dpm
    ec_get_data_el6021(slave, &dpm_in->serial.length, rx_size);


    dpm_in->serial.status = 0;

    if (BIT_CHECK(el6021_status_word, EL6021_TRANSMIT_ACCEPTED_BIT_NUM)) {
        BIT_SET(dpm_in->serial.status, SERIAL_TRANSMIT_ACCEPTED_BIT_NUM);
        // printf("serial transmit accepted bit SET\n");
    } else {
        // printf("serial transmit accepted bit NOTSET\n");
    }

    if (BIT_CHECK(el6021_status_word, EL6021_RECEIVE_REQUEST_BIT_NUM)) {
        BIT_SET(dpm_in->serial.status, SERIAL_RECEIVE_ACCEPTED_BIT_NUM);
    }

    if (BIT_CHECK(el6021_status_word, EL6021_INIT_ACCEPTED_BIT_NUM)) {
        BIT_SET(dpm_in->serial.status, SERIAL_INIT_ACCEPTED_BIT_NUM);
    }

    if (BIT_CHECK(el6021_status_word, EL6021_RX_FIFO_FULL_BIT_NUM)) {
        BIT_SET(dpm_in->serial.status, SERIAL_ERROR_BIT_NUM);
        UM_ERROR(GBEM_UM_EN, "EL6021: RX FIFO full error");
    }
    if (BIT_CHECK(el6021_status_word, EL6021_PARITY_ERROR_BIT_NUM)) {
        BIT_SET(dpm_in->serial.status, SERIAL_ERROR_BIT_NUM);
        UM_ERROR(GBEM_UM_EN, "EL6021: Parity error");
    }
    if (BIT_CHECK(el6021_status_word, EL6021_FRAMING_ERROR_BIT_NUM)) {
        BIT_SET(dpm_in->serial.status, SERIAL_ERROR_BIT_NUM);
        UM_ERROR(GBEM_UM_EN, "EL6021: Framing error");
    }
    if (BIT_CHECK(el6021_status_word, EL6021_OVERRUN_ERROR_BIT_NUM)) {
        BIT_SET(dpm_in->serial.status, SERIAL_ERROR_BIT_NUM);
        UM_ERROR(GBEM_UM_EN, "EL6021: Overrun error");
    }

    // print_status_word(dpm_in->serial.status);

    uint8_t tx_size = dpm_out->serial.length;


    //set data size in el6021 control word (will be writen to slave later)
    ec_set_data_size_el6021(&el6021_control_word, tx_size);


    if (BIT_CHECK(dpm_out->serial.control, SERIAL_TRANSMIT_REQUEST_BIT_NUM)) {
        BIT_SET(el6021_control_word, EL6021_TRANSMIT_REQUEST_BIT_NUM);
        // printf("serial transmit request bit SET\n");
    } else {
        // printf("serial transmit request bit NOTSET\n");
    }

    if (BIT_CHECK(dpm_out->serial.control, SERIAL_RECEIVE_ACCEPTED_BIT_NUM)) {
        BIT_SET(el6021_control_word, EL6021_RECEIVE_ACCEPTED_BIT_NUM);
    }
    if (BIT_CHECK(dpm_out->serial.control, SERIAL_INIT_REQUEST_BIT_NUM)) {
        // printf("init bit set\n");
        BIT_SET(el6021_control_word, EL6021_INIT_REQUEST_BIT_NUM);
    }


    ec_set_data_el6021(slave, dpm_out->serial.data, tx_size);

    // print_control_word(el6021_control_word);


    ec_set_control_word_el6021(slave, el6021_control_word);
    return E_SUCCESS;
}


static gberror_t ec_set_data_el6021(const uint16_t slave, uint8_t *data, uint8_t data_size) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += EL6021_DATA_OUT_PDO_INDEX;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */

    for (int i = 0; i < data_size; i++) {
        *data_ptr = data[i];
        data_ptr++;
    }

    return E_SUCCESS;
}

static gberror_t ec_get_data_el6021(const uint16_t slave, uint8_t *data, uint8_t data_size) {
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += EL6021_DATA_IN_PDO_INDEX;

    for (int i = 0; i < data_size; i++) {
        data[i] = *data_ptr;
        data_ptr++;
    }
    return E_SUCCESS;
}


uint16_t ec_get_status_word_el6021(const uint16_t slave) {
    return ec_pdo_get_input_uint16(slave, EL6021_STATUS_PDO_INDEX);
}

gberror_t ec_set_control_word_el6021(const uint16_t slave, const uint16_t control_word) {
    ec_pdo_set_output_uint16(slave, EL6021_CTRL_PDO_INDEX, control_word);
    return E_SUCCESS;
}

#define EL6021_HALF_DUPLEX_SOD_SUB_INDEX 1
#define EL6021_XON_XOFF_TX_SDO_SUB_INDEX 2
#define EL6021_XON_XOFF_RX_SDO_SUB_INDEX 3
#define EL6021_RS422_SDO_SUB_INDEX 4
#define EL6021_CONTINUOUS_SEND_SDO_SUB_INDEX 5
#define EL6021_ENABLE_OPTIMISATION_SDO_SUB_INDEX 6


gberror_t ec_read_feature_bits_el6021(const uint16_t slave, uint8_t *feature_bits) {
    uint8_t half_duplex = 0;
    ec_sdo_read_uint8(slave, EL6021_FEATURE_BITS_SDO_INDEX, EL6021_HALF_DUPLEX_SOD_SUB_INDEX, &half_duplex, true);

    if (half_duplex == 1) {
        BIT_SET(*feature_bits, EL6021_HALF_DUPLEX_SOD_SUB_INDEX);
    } else {
        BIT_CLEAR(*feature_bits, EL6021_HALF_DUPLEX_SOD_SUB_INDEX);
    }
    //printf true false for bool
    UM_INFO(GBEM_UM_EN, "GBEM: EL6021 Half duplex [%s]", half_duplex==1 ? "true" : "false");
    uint8_t xon_xoff_tx = 0;
    ec_sdo_read_uint8(slave, EL6021_FEATURE_BITS_SDO_INDEX, EL6021_XON_XOFF_TX_SDO_SUB_INDEX,
                      &xon_xoff_tx, true);
    if (xon_xoff_tx == 1) {
        BIT_SET(*feature_bits, EL6021_XON_XOFF_TX_SDO_SUB_INDEX);
    } else {
        BIT_CLEAR(*feature_bits, EL6021_XON_XOFF_TX_SDO_SUB_INDEX);
    }

    UM_INFO(GBEM_UM_EN, "GBEM: EL6021 XON/XOFF TX [%s]", xon_xoff_tx==1 ? "true" : "false");

    uint8_t xon_xoff_rx = 0;

    ec_sdo_read_uint8(slave, EL6021_FEATURE_BITS_SDO_INDEX, EL6021_XON_XOFF_RX_SDO_SUB_INDEX,
                      &xon_xoff_rx, true);

    if (xon_xoff_rx == 1) {
        BIT_SET(*feature_bits, EL6021_XON_XOFF_RX_SDO_SUB_INDEX);
    } else {
        BIT_CLEAR(*feature_bits, EL6021_XON_XOFF_RX_SDO_SUB_INDEX);
    }

    UM_INFO(GBEM_UM_EN, "GBEM: EL6021 XON/XOFF RX [%s]", xon_xoff_rx==1 ? "true" : "false");
    uint8_t rs422 = 0;

    ec_sdo_read_uint8(slave, EL6021_FEATURE_BITS_SDO_INDEX, EL6021_RS422_SDO_SUB_INDEX, &rs422, true);

    if (rs422 == 1) {
        BIT_SET(*feature_bits, EL6021_RS422_SDO_SUB_INDEX);
    } else {
        BIT_CLEAR(*feature_bits, EL6021_RS422_SDO_SUB_INDEX);
    }

    UM_INFO(GBEM_UM_EN, "GBEM: EL6021 RS422 [%s]", rs422==1 ? "true" : "false");

    uint8_t continuous_send = 0;

    ec_sdo_read_uint8(slave, EL6021_FEATURE_BITS_SDO_INDEX, EL6021_CONTINUOUS_SEND_SDO_SUB_INDEX,
                      &continuous_send, true);

    if (continuous_send == 1) {
        BIT_SET(*feature_bits, EL6021_CONTINUOUS_SEND_SDO_SUB_INDEX);
    } else {
        BIT_CLEAR(*feature_bits, EL6021_CONTINUOUS_SEND_SDO_SUB_INDEX);
    }
    UM_INFO(GBEM_UM_EN, "GBEM: EL6021 Continuous send [%s]", continuous_send==1 ? "true" : "false");

    uint8_t enable_optimisation = 0;
    ec_sdo_read_uint8(slave, EL6021_FEATURE_BITS_SDO_INDEX,
                      EL6021_ENABLE_OPTIMISATION_SDO_SUB_INDEX, &enable_optimisation, true);
    UM_INFO(GBEM_UM_EN, "GBEM: EL6021 Enable optimisation [%s]", enable_optimisation==1 ? "true" : "false");

    if (enable_optimisation == 1) {
        BIT_SET(*feature_bits, EL6021_ENABLE_OPTIMISATION_SDO_SUB_INDEX);
    } else {
        BIT_CLEAR(*feature_bits, EL6021_ENABLE_OPTIMISATION_SDO_SUB_INDEX);
    }


    return E_SUCCESS;
}

gberror_t ec_apply_standard_sdos_el6021(const uint16_t slave) {
    uint8_t feature_bits = 0;
    ec_read_feature_bits_el6021(slave, &feature_bits);


    if (!ec_sdo_write_uint16(slave, EL6021_BAUD_RATE_SDO_INDEX, EL6021_BAUD_RATE_SDO_SUB_INDEX,
                             EL6021_BAUD_RATE_115200, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    if (!ec_sdo_write_uint16(slave, EL6021_DATA_FRAME_SDO_INDEX, EL6021_DATA_FRAME_SDO_SUB_INDEX,
                             EL6021_ENCODING_8N1, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    //Enable half duplex
    if (!ec_sdo_write_uint8(slave, EL6021_FEATURE_BITS_SDO_INDEX, EL6021_HALF_DUPLEX_SOD_SUB_INDEX,
                            1, true)) {
        return E_SDO_WRITE_FAILURE;
    }
    //Disable  point to point rs422
    if (!ec_sdo_write_uint8(slave, EL6021_FEATURE_BITS_SDO_INDEX, EL6021_RS422_SDO_SUB_INDEX,
                            0, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    ec_read_feature_bits_el6021(slave, &feature_bits);


    //all applied correctly
    return E_SUCCESS;
}


