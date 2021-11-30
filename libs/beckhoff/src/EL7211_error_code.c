
/**
 ******************************************************************************
 * @file           :  EL7211_error_code.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */



#include "stdint.h"

/* Text ID structure: xyzz
 *
 * x:
 * 0: Systeminfo
 * 2: reserved
 * 1: Info
 * 4: Warning
 * 8: Error
 *
 * y:
 * 0: System
 * 1: General
 * 2: Communication
 * 3: Encoder
 * 4: Drive
 * 5: Inputs
 * 6: I/O general
 * 7: reserved
 *
 * z: id (two bytes)
 */

#include "EL7211_error_code.h"


etg1020_diag_text_t etg1020_diag_text[NUM_OF_EL7211_DIAG_STRINGS] = {
        {0x0001, "No error", 0},
        {0x0002, "Communication established", 0},
        {0x0003, "Initialization: 0x%X, 0x%X, 0x%X", 3},
        {0x1000, "Information: 0x%X, 0x%X, 0x%X", 3},
        {0x1012, "EtherCAT state change Init - PreOp", 0},
        {0x1021, "EtherCAT state change PreOp - Init", 0},
        {0x1024, "EtherCAT state change PreOp - Safe-Op", 0},
        {0x1042, "EtherCAT state change SafeOp - PreOp", 0},
        {0x1048, "EtherCAT state change SafeOp - Op", 0},
        {0x1084, "EtherCAT state change Op - SafeOp", 0},
        {0x1100, "Detection of operation mode completed: 0x%X, %d", 2},
        {0x1135, "Cycle time o.k.: %d", 1},
        {0x1157, "Data manually saved (Idx: 0x%X,SubIdx: 0x%X)", 2},
        {0x1158, "Data automatically saved (Idx: 0x %X, SubIdx: 0x%X)", 2},
        {0x1159, "Data deleted (Idx: 0x%X, SubIdx:0x%X)", 2},
        {0x117F, "Information: 0x%X, 0x%X, 0x%X Information", 3},
        {0x1201, "Communication re-established", 0},
        {0x1300, "Position set: %d, %d", 2},
        {0x1303, "Encoder Supply ok", 0},
        {0x1304, "Encoder initialization successfully, channel: %X", 1},
        {0x1305, "Sent command encoder reset, channel: %X", 1},
        {0x1400, "Drive is calibrated: %d, %d Drive is calibrated", 2},
        {0x1401, "Actual drive state: 0x%X, %d Current drive status", 2},
        {0x1705, "CPU usage returns in normal range (< 85%%)", 0},
        {0x1706, "Channel is not in saturation anymore", 0},
        {0x1707, "Channel is not in overload anymore", 0},
        {0x170A, "No channel range error anymore", 0},
        {0x170C, "Calibration data saved", 0},
        {0x170D, "Calibration data will be applied and saved after sending the command 0x5AFE", 0},
        {0x2000, "%s: %s", 2},
        {0x2001, "%s: Network link lost Network connection lost", 1},
        {0x2002, "%s: Network link detected Network connection found", 1},
        {0x2003, "%s: no valid IP Configuration - Dhcp client started", 1},
        {0x2004, "%s: valid IP Configuration (IP:%d.%d.%d.%d) assigned by Dhcp server %d.%d.%d.%d", 9},
        {0x2005, "%s: Dhcp client timed out DHCP client timeout", 1},
        {0x2006, "%s: Duplicate IP Address detected (%d.%d.%d.%d)", 5},
        {0x2007, "%s: UDP handler initialized UDP handler initialized", 1},
        {0x2008, "%s: TCP handler initialized TCP handler initialized", 1},
        {0x2009, "%s: No more free TCP sockets available", 1},
        {0x4000, "Warning: 0x%X, 0x%X, 0x%X", 3},
        {0x4001, "Warning: 0x%X, 0x%X, 0x%X", 3},
        {0x4002, "%s: %s Connection Open (IN:%d OUT:%d API:%dms) from %d. %d.%d.%d successful", 9},
        {0x4003, "%s: %s Connection Close (IN:%d OUT:%d) from %d.%d.%d.%d successful", 8},
        {0x4004, "%s: %s Connection (IN:%d OUT: %d) with %d.%d.%d.%d timed out", 8},
        {0x4005, "%s: %s Connection Open (IN:%d OUT:%d) from %d.%d.%d.%d denied (Error: %u)", 9},
        {0x4006, "%s: %s Connection Open (IN:%d OUT:%d) from %d.%d.%d.%d denied (I/P Size: %d Byte(s) rx: %d Byte(s))", 10},
        {0x4007, "%s: %s Connection Open (IN:%d OUT:%d) from %d.%d.%d.%d denied (O/P Size: %d Byte(s) rx: %d Byte(s))", 10},
        {0x4008, "%s: %s Connection Open (IN:%d OUT:%d) from %d.%d.%d.%d denied (RPI:%dms not supported -> //API:%dms)", 10},
        {0x4101, "Terminal-Overtemperature", 0},
        {0x4102, "Discrepancy in the PDO-Configuration", 0},
        {0x417F, "Warning: 0x%X, 0x%X, 0x%X", 3},
        {0x428D, " Challenge is not Random", 0},
        {0x4300, "Subincrements deactivated: %d, %d", 2},
        {0x4301, "Encoder-Warning", 0},
        {0x4302, "Warning Encoder Maximum frequency of the input signal is nearly reached (channel %d)", 1},
        {0x4303, "Warning Encoder Limit counter value was reduced because of the PDO configuration (channel %d)", 1},
        {0x4304, "Warning Encoder Reset counter value was reduced because of the PDO configuration (channel %d)", 1},
        {0x4400, "Drive is not calibrated: %d, %d", 2},
        {0x4401, "Starttype not supported: 0x%X, %d", 2},
        {0x4402, "Command rejected: %d, %d", 2},
        {0x4405, "Invalid modulo subtype: %d, %d", 2},
        {0x4410, "Target overrun: %d, %d", 2},
        {0x4411, "DC-Link undervoltage (Warning)", 0},
        {0x4412, "DC-Link overvoltage (Warning)", 0},
        {0x4413, "I2T-Model Amplifier overload (Warning)", 0},
        {0x4414, "I2T-Model Motor overload (Warning)", 0},
        {0x4415, "Speed limitation active", 0},
        {0x4416, "Warning Drive Step lost detected at position: 0x%X%X", 2},
        {0x4417, "Motor overtemperature", 0},
        {0x4418, "Limit: Current Limit: current is limited", 0},
        {0x4419, "Limit: Amplifier I2T-model exceeds 100%%", 0},
        {0x441A, "Limit: Motor I2T-model exceeds 100%%", 0},
        {0x441B, "Limit: Velocity limitation", 0},
        {0x441C, "STO while the axis was enabled", 0},
        {0x4600, "IO Wrong supply voltage range", 0},
        {0x4610, "IO Wrong output voltage range", 0},
        {0x4705, "Processor usage at %d %%", 2},
        {0x470A, "EtherCAT Frame missed (change Settings or DC Operation Mode or Sync0 Shift Time)", 0},
        {0x8000, "%s: %s", 2},
        {0x8001, "Error: 0x%X, 0x%X, 0x%X", 3},
        {0x8002, "Communication aborted", 0},
        {0x8003, "Configuration error: 0x%X, 0x%X, 0x%X", 3},
        {0x8004, "%s: Unsuccessful FwdOpen-Response received from %d.%d.%d. %d (%s) (Error: %u)", 7},
        {0x8005, "%s: FwdClose-Request sent to %d.%d.%d.%d (%s)", 6},
        {0x8006, "%s: Unsuccessful FwdClose-Response received from %d.%d.%d. %d (%s) (Error: %u)", 7},
        {0x8007, "%s: Connection with %d.%d.%d. %d (%s) closed", 6},
        {0x8100, "Status word set: 0x%X, %d", 2},
        {0x8101, "Operation mode incompatible to PDO interface: 0x%X, %d", 2},
        {0x8102, "Invalid combination of Inputs and Outputs PDOs", 0},
        {0x8103, "No variable linkage No variables linked", 0},
        {0x8104, "Terminal-Overtemperature", 0},
        {0x8105, "PD-Watchdog - comms to o/p stage stopped if fieldbus comms interrupted", 0},
        {0x8135, "Cycle time has to be a multiple of 125 μs", 0},
        {0x8136, "Configuration error: invalid sampling rate", 0},
        {0x8137, "Electronic type plate: CRC error", 0},
        {0x8140, "Sync Error", 0},
        {0x8141, "Sync%X Interrupt lost", 0},
        {0x8142, "Sync Interrupt asynchronous", 0},
        {0x8143, "Jitter too big", 0},
        {0x817F, "Error: 0x%X, 0x%X, 0x%X", 3},
        {0x8200, "Write access error: %d, %d", 2},
        {0x8201, "No communication to field-side (Auxiliary voltage missing)", 0},
        {0x8281, "Ownership failed: %X", 1},
        {0x8282, "To many Keys found", 0},
        {0x8283, "Key Creation failed: %X", 1},
        {0x8284, "Key loading failed", 0},
        {0x8285, "Reading Public Key failed: %X", 1},
        {0x8286, "Reading Public EK failed: %X", 1},
        {0x8287, "Reading PCR Value failed: %X", 1},
        {0x8288, "Reading Certificate EK failed: %X", 1},
        {0x8289, "Challenge could not be hashed: %X", 1},
        {0x828A, "Tickstamp Process failed", 0},
        {0x828B, "PCR Process failed: %X", 1},
        {0x828C, "Quote Process failed: %X", 1},
        {0x82FF, "Bootmode not activated", 0},
        {0x8300, "Set position error: 0x%X, %d", 2},
        {0x8301, "Encoder increments not configured: 0x%X, %d", 2},
        {0x8302, "The amplitude of the resolver is too small", 0},
        {0x8303, "Encoder power missing (channel %d)", 1},
        {0x8304, "Encoder communication error, channel: %X", 1},
        {0x8305, "EnDat2.2 is not supported, channel: %X", 1},
        {0x8306, "Delay time, tolerance limit exceeded, 0x%X, channel: %X", 2},
        {0x8307, "Delay time, maximum value exceeded, 0x%X, channel: %X", 2},
        {0x8308, "Unsupported ordering designation, 0x%X, channel: %X (only 02 and 22 is supported)", 2},
        {0x8309, "Encoder CRC error, channel: %X", 1},
        {0x830A, "Temperature %X could not be read, channel: %X", 1},
        {0x830C, "Encoder Single-Cycle-Data Error, channel. %X", 1},
        {0x830D, "Encoder Watchdog Error, channel. %X", 1},
        {0x8310, "Initialisation error", 0},
        {0x8311, "Maximum frequency of the input signal is exceeded (channel %d)", 1},
        {0x8312, "Encoder plausibility error (channel %d)", 1},
        {0x8313, "Configuration error (channel %d)", 1},
        {0x8314, "Synchronisation error", 0},
        {0x8315, "Error status input (channel %d)", 1},
        {0x8400, "Incorrect drive configuration: 0x %X, %d", 2},
        {0x8401, "Limiting of calibration velocity: %d, %d", 2},
        {0x8402, "Emergency stop activated: 0x%X, %d", 2},
        {0x8403, "ADC Error Error during current measurement in the ADC", 0},
        {0x8404, "Overcurrent in phase U, V or W", 0},
        {0x8405, "Invalid modulo position: %d", 1},
        {0x8406, "DC-Link undervoltage (Error)", 0},
        {0x8407, "DC-Link overvoltage (Error)", 0},
        {0x8408, "I2T-Model Amplifier overload (Error)", 0},
        {0x8409, "I2T-Model motor overload (Error)", 0},
        {0x840A, "Overall current threshold exceeded", 0},
        {0x8415, "Invalid modulo factor: %d", 1},
        {0x8416, "Motor overtemperature", 0},
        {0x8417, "Maximum rotating field velocity exceeded", 0},
        {0x841C, "STO while the axis was enabled", 0},
        {0x8550, "Zero crossing phase %X missing", 1},
        {0x8551, "Phase sequence Error Wrong direction of rotation", 0},
        {0x8552, "Overcurrent phase %X", 1},
        {0x8553, "Overcurrent neutral wire", 0},
        {0x8581, "Wire broken Ch %D", 1},
        {0x8600, "IO Wrong supply voltage range", 0},
        {0x8601, "IO Supply voltage to low", 0},
        {0x8602, "IO Supply voltage to high", 0},
        {0x8603, "IO Over current of supply voltage", 0},
        {0x8610, "IO Wrong output voltage range", 0},
        {0x8611, "IO Output voltage to low", 0},
        {0x8612, "IO Output voltage to high", 0},
        {0x8613, "IO Over current of output voltage", 0},
        {0x8700, "Channel/Interface not calibrated or Channel/interface not synchronized", 0},
        {0x8701, "Operating time was manipulated", 0},
        {0x8702, "Oversampling setting is not possible", 0},
        {0x8703, "No slave controller found", 0},
        {0x8704, "Slave controller is not in Bootstrap", 0},
        {0x8705, "Processor usage to high", 0},
        {0x8706, "Channel in saturation", 0},
        {0x8707, "Channel overload", 0},
        {0x8708, "Overloadtime was manipulated", 0},
        {0x8709, "Saturationtime was manipulated", 0},
        {0x870A, "Channel range error", 0},
        {0x870B, "no ADC clock", 0},
        {0xFFFF, "Debug: 0x%X, 0x%X, 0x%X Debug: 0x%X, 0x%X, 0x%X", 6}
};