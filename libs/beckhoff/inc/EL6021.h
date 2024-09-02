/**
 ******************************************************************************
 * @file           :  EL6021.h
 * @brief          :  1-channel communication interface, serial, RS422/RS485
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL6021_H
#define GBEM_EL6021_H

#include "beckhoff_shared.h"
#include "gberror.h"
#include "std_defs_and_macros.h"
#include "std_headers.h"

//EL6021 - 1-channel communication interface, serial, RS422/RS485
#define EL6021_EEP_NAME "EL6021"
#define EL6021_EEP_MAN BECKHOFF_MAN
#define EL6021_EEP_REV 0x00160000
#define EL6021_EEP_ID 17853052

//new
//Man: 00000002 ID: 17853052 Rev: 00160000

//old
// Man: 00000002 ID: 17853052 Rev: 00000000

#define EL6021_CTRL_PDO_INDEX 0
#define EL6021_DATA_OUT_PDO_INDEX 2

#define EL6021_STATUS_PDO_INDEX 0
#define EL6021_DATA_IN_PDO_INDEX 2

#define EL6021_DATA_SIZE 22


//enum SERIAL_CONTROL_WORD {
//    SERIAL_TRANSMIT_REQUEST_BIT_NUM                     = (0),
//    SERIAL_RECEIVE_ACCEPTED_BIT_NUM                     = (1),
//    SERIAL_INIT_REQUEST_BIT_NUM                         = (2),
//};
//enum SERIAL_STATUS_WORD {
//    SERIAL_TRANSMIT_ACCEPTED_BIT_NUM                    = (0),
//    SERIAL_RECEIVE_REQUEST_BIT_NUM                      = (1),
//    SERIAL_INIT_ACCEPTED_BIT_NUM                        = (2),
//    SERIAL_ERROR_BIT_NUM                                = (3),
//};

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

#define EL6021_PARITY_ERROR_BIT_NUM 4
#define EL6021_FRAMING_ERROR_BIT_NUM 5
#define EL6021_OVERRUN_ERROR_BIT_NUM 6


//uint16_t
#define EL6021_BAUD_RATE_SDO_INDEX 0x4073
#define EL6021_BAUD_RATE_SDO_SUB_INDEX 0x0

//uint16_t
#define EL6021_DATA_FRAME_SDO_INDEX 0x4074
#define EL6021_DATA_FRAME_SDO_SUB_INDEX 0x0

#define EL6021_FEATURE_BITS_SDO_INDEX 0x4075

gberror_t ec_slave_exec_el6021n(uint16_t slave);

gberror_t ec_slave_exec_el6021(uint16_t slave);

gberror_t ec_apply_standard_sdos_el6021(uint16_t slave);


#endif //GBEM_EL6021_H


/*
Slave: 4
 Name: EL6021
 Output size: 0bits
 Input size: 0bits
 State: 2
 Delay: 0[ns]
 Has DC: 0
 Activeports: 1.0.0.0
 Configured address: 1004
 Man: 00000002 ID: 17853052 Rev: 00000000
 SM0 A:1800 L: 246 F:00010026 Type:1
 SM1 A:18f6 L: 246 F:00010022 Type:2
 SM2 A:1000 L:   0 F:00010024 Type:3
 SM3 A:1400 L:   0 F:00010020 Type:4
 FMMUfunc 0:1 1:2 2:0 3:0
 MBX length wr: 246 rd: 246 MBX protocols : 0c
 CoE details: 0f FoE details: 01 EoE details: 00 SoE details: 00
 Ebus current: 190[mA]
 only LRD/LWR:0
PDO mapping according to CoE :
  SM2 outputs
  byte offset   addr.b         index:sub   bitl data_type    name
  [0          ] [0xFFF6AEC0.0] 0x3003:0x01 0x10 UNSIGNED16   Ctrl
  [2          ] [0xFFF6AEC2.0] 0x3003:0x02 0x08 UNSIGNED8    Data Out 0
  [3          ] [0xFFF6AEC3.0] 0x3003:0x03 0x08 UNSIGNED8    Data Out 1
  [4          ] [0xFFF6AEC4.0] 0x3003:0x04 0x08 UNSIGNED8    Data Out 2
  [5          ] [0xFFF6AEC5.0] 0x3003:0x05 0x08 UNSIGNED8    Data Out 3
  [6          ] [0xFFF6AEC6.0] 0x3003:0x06 0x08 UNSIGNED8    Data Out 4
  [7          ] [0xFFF6AEC7.0] 0x3003:0x07 0x08 UNSIGNED8    Data Out 5
  [8          ] [0xFFF6AEC8.0] 0x3003:0x08 0x08 UNSIGNED8    Data Out 6
  [9          ] [0xFFF6AEC9.0] 0x3003:0x09 0x08 UNSIGNED8    Data Out 7
  [10         ] [0xFFF6AECA.0] 0x3003:0x0A 0x08 UNSIGNED8    Data Out 8
  [11         ] [0xFFF6AECB.0] 0x3003:0x0B 0x08 UNSIGNED8    Data Out 9
  [12         ] [0xFFF6AECC.0] 0x3003:0x0C 0x08 UNSIGNED8    Data Out 10
  [13         ] [0xFFF6AECD.0] 0x3003:0x0D 0x08 UNSIGNED8    Data Out 11
  [14         ] [0xFFF6AECE.0] 0x3003:0x0E 0x08 UNSIGNED8    Data Out 12
  [15         ] [0xFFF6AECF.0] 0x3003:0x0F 0x08 UNSIGNED8    Data Out 13
  [16         ] [0xFFF6AED0.0] 0x3003:0x10 0x08 UNSIGNED8    Data Out 14
  [17         ] [0xFFF6AED1.0] 0x3003:0x11 0x08 UNSIGNED8    Data Out 15
  [18         ] [0xFFF6AED2.0] 0x3003:0x12 0x08 UNSIGNED8    Data Out 16
  [19         ] [0xFFF6AED3.0] 0x3003:0x13 0x08 UNSIGNED8    Data Out 17
  [20         ] [0xFFF6AED4.0] 0x3003:0x14 0x08 UNSIGNED8    Data Out 18
  [21         ] [0xFFF6AED5.0] 0x3003:0x15 0x08 UNSIGNED8    Data Out 19
  [22         ] [0xFFF6AED6.0] 0x3003:0x16 0x08 UNSIGNED8    Data Out 20
  [23         ] [0xFFF6AED7.0] 0x3003:0x17 0x08 UNSIGNED8    Data Out 21
  SM3 inputs
  byte offset   addr.b         index:sub   bitl data_type    name
  [0          ] [0xFFF6AEC0.0] 0x3103:0x01 0x10 UNSIGNED16   Status
  [2          ] [0xFFF6AEC2.0] 0x3103:0x02 0x08 UNSIGNED8    Data In 0
  [3          ] [0xFFF6AEC3.0] 0x3103:0x03 0x08 UNSIGNED8    Data In 1
  [4          ] [0xFFF6AEC4.0] 0x3103:0x04 0x08 UNSIGNED8    Data In 2
  [5          ] [0xFFF6AEC5.0] 0x3103:0x05 0x08 UNSIGNED8    Data In 3
  [6          ] [0xFFF6AEC6.0] 0x3103:0x06 0x08 UNSIGNED8    Data In 4
  [7          ] [0xFFF6AEC7.0] 0x3103:0x07 0x08 UNSIGNED8    Data In 5
  [8          ] [0xFFF6AEC8.0] 0x3103:0x08 0x08 UNSIGNED8    Data In 6
  [9          ] [0xFFF6AEC9.0] 0x3103:0x09 0x08 UNSIGNED8    Data In 7
  [10         ] [0xFFF6AECA.0] 0x3103:0x0A 0x08 UNSIGNED8    Data In 8
  [11         ] [0xFFF6AECB.0] 0x3103:0x0B 0x08 UNSIGNED8    Data In 9
  [12         ] [0xFFF6AECC.0] 0x3103:0x0C 0x08 UNSIGNED8    Data In 10
  [13         ] [0xFFF6AECD.0] 0x3103:0x0D 0x08 UNSIGNED8    Data In 11
  [14         ] [0xFFF6AECE.0] 0x3103:0x0E 0x08 UNSIGNED8    Data In 12
  [15         ] [0xFFF6AECF.0] 0x3103:0x0F 0x08 UNSIGNED8    Data In 13
  [16         ] [0xFFF6AED0.0] 0x3103:0x10 0x08 UNSIGNED8    Data In 14
  [17         ] [0xFFF6AED1.0] 0x3103:0x11 0x08 UNSIGNED8    Data In 15
  [18         ] [0xFFF6AED2.0] 0x3103:0x12 0x08 UNSIGNED8    Data In 16
  [19         ] [0xFFF6AED3.0] 0x3103:0x13 0x08 UNSIGNED8    Data In 17
  [20         ] [0xFFF6AED4.0] 0x3103:0x14 0x08 UNSIGNED8    Data In 18
  [21         ] [0xFFF6AED5.0] 0x3103:0x15 0x08 UNSIGNED8    Data In 19
  [22         ] [0xFFF6AED6.0] 0x3103:0x16 0x08 UNSIGNED8    Data In 20
  [23         ] [0xFFF6AED7.0] 0x3103:0x17 0x08 UNSIGNED8    Data In 21

  */


// 0x4075      "Feature bits"                                [RECORD  maxsub(0x06 / 6)]
//     0x00      "No of elements"                              [UNSIGNED8        RWRWRW]      0x06 / 6
//     0x01      "Enable half duplex"                          [BOOLEAN          RWRWRW]      FALSE
//     0x02      "Enable XON/XOFF supported tx data"           [BOOLEAN          RWRWRW]      FALSE
//     0x03      "Enable XON/XOFF supported rx data"           [BOOLEAN          RWRWRW]      FALSE
//     0x04      "Enable point to point connection (RS422)"      [BOOLEAN          RWRWRW]      FALSE
//     0x05      "Enable send FIFO data continous"             [BOOLEAN          RWRWRW]      FALSE
//     0x06      "Enable transfer rate optimization"           [BOOLEAN          RWRWRW]      FALSE
