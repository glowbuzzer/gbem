/**
 ******************************************************************************
 * @file           :  aw-j-series.h
 * @brief          :  common defines and prototypes for all AutomationWare j-series drives
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_AW_J_SERIES_H
#define GBEM_AW_J_SERIES_H


#include <map_config_custom_pdo.h>

#include "map.h"
#include "gberror.h"
#include "automationware.h"

extern map_custom_pdo_t aw_j_series_custom_pdo_map;

/** Machine functions */
bool ec_get_safety_state_aw_j_series(uint16_t slave);

/** Slave functions */

gberror_t ec_initial_pdo_aw_j_series(uint16_t slave);

int8_t ec_get_moo_pdo_aw_j_series(uint16_t drive);

gberror_t ec_apply_limits_aw_j_series(uint16_t slave);

/** Error functions */
uint8_t *ec_get_error_string_sdo_aw_j_series(uint16_t drive, bool *error);

uint8_t *ec_get_error_string_pdo_aw_j_series(uint16_t drive, bool *error);

uint8_t *ec_get_detailled_error_report_sdo_aw_j_series(uint16_t drive_number, bool short_report);

uint8_t *ec_get_detailled_error_report_pdo_aw_j_series(uint16_t drive_number, bool short_report);

/** Drive functions */

gberror_t ec_get_log_file_aw_j_series(const uint16_t drive);

gberror_t ec_set_ctrl_wrd_aw_j_series(uint16_t drive, uint16_t ctrlwrd);

uint16_t ec_get_stat_wrd_aw_j_series(uint16_t drive);

int32_t ec_get_actpos_wrd_aw_j_series(uint16_t drive);

int32_t ec_get_actvel_wrd_aw_j_series(uint16_t drive);

int32_t ec_get_acttorq_wrd_aw_j_series(uint16_t drive);

int32_t ec_get_control_effort_wrd_aw_j_series(uint16_t drive);

gberror_t ec_set_setpos_wrd_aw_j_series(uint16_t drive, int32_t setpos);

bool ec_get_remote_aw_j_series(uint16_t drive);

bool ec_get_follow_error_aw_j_series(uint16_t drive);

gberror_t ec_set_moo_pdo_aw_j_series(uint16_t drive, int8_t moo);

uint16_t ec_get_ctrl_wrd_rev_aw_j_series(uint16_t drive);

gberror_t ec_set_stat_wrd_rev_aw_j_series(uint16_t drive, uint16_t statwrd);

gberror_t ec_set_actpos_wrd_rev_aw_j_series(uint16_t drive, int32_t actpos);

int32_t ec_get_setpos_word_rev_aw_j_series(uint16_t drive);

gberror_t ec_set_moo_pdo_rev_aw_j_series(uint16_t drive);

gberror_t ec_set_setvel_wrd_aw_j_series(uint16_t drive, int32_t setvel);

gberror_t ec_set_setveloffset_wrd_aw_j_series(uint16_t drive, int32_t setveloffset);

gberror_t ec_set_settorq_wrd_aw_j_series(uint16_t drive, int32_t settorq);

gberror_t ec_set_settorqoffset_wrd_aw_j_series(uint16_t drive, int32_t settorq_offset);

gberror_t ec_apply_standard_sdos_aw_j_series(uint16_t slave);

gberror_t ec_pdo_map_aw_j_series(uint16_t slave);

gberror_t ec_print_pdo_config_aw_series(const uint16_t slave);


#define AW_EEP_NAME "SOMANET"
#define AW_EEP_MAN AW_MOTOR_MAN
#define AW_EEP_REV 0x0a000002
#define AW_EEP_ID 0x00000201


//Number of error strings and error report strings for the AW J series drives
#define NUM_OF_AW_J_SERIES_ERROR_STRINGS 61
#define NUM_OF_AW_J_SERIES_ERROR_REPORT_STRINGS 162


//PDO addresses
#define AW_J_SERIES_CONTROLWORD_ADDRESS                     0x6040
#define AW_J_SERIES_MODES_OF_OPERATION_ADDRESS              0x6060
#define AW_J_SERIES_TARGET_TORQUE_ADDRESS                   0x6071
#define AW_J_SERIES_TARGET_POSITION_ADDRESS                 0x607A
#define AW_J_SERIES_TARGET_VELOCITY_ADDRESS                 0x60FF
#define AW_J_SERIES_TORQUE_OFFSET_ADDRESS                   0x60B2
#define AW_J_SERIES_TUNING_COMMAND_ADDRESS                  0x2701


#define AW_J_SERIES_PHYSICAL_OUTPUTS_ADDRESS                0x60FE
#define AW_J_SERIES_BIT_MASK_ADDRESS                        0x60FE

#define AW_J_SERIES_USER_MOSI_ADDRESS                       0x2703
#define AW_J_SERIES_VELOCITY_OFFSET_ADDRESS                 0x60B1
#define AW_J_SERIES_LED_COLOUR_ADDRESS                      0x2215

#define AW_J_SERIES_STATUSWORD_ADDRESS                      0x6041
#define AW_J_SERIES_MODES_OF_OPERATION_DISPLAY_ADDRESS      0x6061
#define AW_J_SERIES_POSITION_ACTUAL_VALUE_ADDRESS           0x6064
// 0x60FC Position demand internal value
#define AW_J_SERIES_POSITION_DEMAND_INTERNAL_VALUE_ADDRESS  0x60FC
#define AW_J_SERIES_CONTROL_EFFORT_ADDRESS                  0x60FA


#define AW_J_SERIES_VELOCITY_DEMAND_VALUE_ADDRESS           0x606B
#define AW_J_SERIES_VELOCITY_ACTUAL_VALUE_ADDRESS           0x606C
#define AW_J_SERIES_TORQUE_ACTUAL_VALUE_ADDRESS             0x6077
#define AW_J_SERIES_TORQUE_OFFSET_ADDRESS                   0x60B2
#define AW_J_SERIES_DIGITAL_INPUTS_ADDRESS                  0x60FD
#define AW_J_SERIES_TORQUE_DEMAND_ADDRESS                   0x6074

#define AW_J_SERIES_TORQUE_OFFSET_ADDRESS                   0x60B2

#define AW_J_SERIES_TIMESTAMP_ADDDRESS                      0x20F0

#define AW_J_SERIES_USER_MISO_ADDRESS                       0x2704

#define AW_J_SERIES_DIGITAL_INPUTS_ADDRESS                  0x60FD

#define AW_J_SERIES_ANALOG_INPUT_1_ADDRESS                  0x2401
#define AW_J_SERIES_ANALOG_INPUT_2_ADDRESS                  0x2402
#define AW_J_SERIES_ANALOG_INPUT_3_ADDRESS                  0x2403
#define AW_J_SERIES_ANALOG_INPUT_4_ADDRESS                  0x2404

#define AW_J_SERIES_TUNING_STATUS_ADDDRESS                  0x2702

#define AW_J_SERIES_ERROR_CODE_ADDRESS                      0x603F
#define AW_J_SERIES_ERROR_REPORT_ADDRESS                    0x203F
#define AW_J_SERIES_CONTROL_EFFORT_ADDRESS                  0x60FA

//PDO indexes for the AW J series drives - these are most easily found from a netscan of the drive
/*
  SM2 outputs
  byte offset   addr.b         index:sub   bitl data_type    name
  [0          ] [0x0052.0] 0x6040:0x00 0x10 UNSIGNED16   Controlword
  [2          ] [0x0054.0] 0x6060:0x00 0x08 INTEGER8     Modes of operation
  [3          ] [0x0055.0] 0x6071:0x00 0x10 INTEGER16    Target Torque
  [5          ] [0x0057.0] 0x607A:0x00 0x20 INTEGER32    Target position
  [9          ] [0x005B.0] 0x60FF:0x00 0x20 INTEGER32    Target velocity
  [13         ] [0x005F.0] 0x60B2:0x00 0x10 INTEGER16    Torque offset
  [15         ] [0x0061.0] 0x2701:0x00 0x20 UNSIGNED32   Tuning command
  [19         ] [0x0065.0] 0x60FE:0x01 0x20 UNSIGNED32   Physical outputs
  [23         ] [0x0069.0] 0x60FE:0x02 0x20 UNSIGNED32   Bit mask
  [27         ] [0x006D.0] 0x2703:0x00 0x20 UNSIGNED32   User MOSI
  [31         ] [0x0071.0] 0x60B1:0x00 0x20 INTEGER32    Velocity offset
  [35         ] [0x0075.0] 0x2215:0x01 0x20 UNSIGNED32   LED color
  [39         ] [0x0079.0] 0x6770:0x01 0x08 UNSIGNED8    FSoE Command
  [40         ] [0x007A.0] 0x6640:0x00 0x01 BOOLEAN      STO command
  [40         ] [0x007A.1] 0x6650:0x01 0x01 BOOLEAN      SS1
  [40         ] [0x007A.2] 0x6670:0x01 0x01 BOOLEAN      SS2
  [40         ] [0x007A.3] 0x6668:0x01 0x01 BOOLEAN      SOS
  [40         ] [0x007A.4] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [40         ] [0x007A.5] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [40         ] [0x007A.6] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [40         ] [0x007A.7] 0x6632:0x00 0x01 BOOLEAN      Error acknowledge
  [41         ] [0x007B.0] 0x6690:0x01 0x01 BOOLEAN      SLS Instance 1
  [41         ] [0x007B.1] 0x6690:0x02 0x01 BOOLEAN      SLS Instance 2
  [41         ] [0x007B.2] 0x6690:0x03 0x01 BOOLEAN      SLS Instance 3
  [41         ] [0x007B.3] 0x6690:0x04 0x01 BOOLEAN      SLS Instance 4
  [41         ] [0x007B.4] 0x6630:0x00 0x01 BOOLEAN      Restart acknowledge
  [41         ] [0x007B.5] 0x6660:0x00 0x01 BOOLEAN      SBC command
  [41         ] [0x007B.6] 0x260A:0x00 0x01
  [41         ] [0x007B.7] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [42         ] [0x007C.0] 0x6770:0x03 0x10 UNSIGNED16   FSoE CRC_0
  [44         ] [0x007E.0] 0x2001:0x00 0x08 INTEGER16    Commutation angle offset
  [45         ] [0x007F.0] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [45         ] [0x007F.1] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [45         ] [0x007F.2] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [45         ] [0x007F.3] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [45         ] [0x007F.4] 0x26F0:0x01 0x01 BOOLEAN      Safe output 1
  [45         ] [0x007F.5] 0x26F0:0x02 0x01 BOOLEAN      Safe output 2
  [45         ] [0x007F.6] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [45         ] [0x007F.7] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [46         ] [0x0080.0] 0x6770:0x04 0x10 UNSIGNED16   FSoE CRC_1
  [48         ] [0x0082.0] 0x6770:0x02 0x10 UNSIGNED16   FSoE ConnectionID
  SM3 inputs
  byte offset   addr.b         index:sub   bitl data_type    name
  [0          ] [0x00BB.0] 0x6041:0x00 0x10 UNSIGNED16   Statusword
  [2          ] [0x00BD.0] 0x6061:0x00 0x08 INTEGER8     Modes of operation display
  [3          ] [0x00BE.0] 0x6064:0x00 0x20 INTEGER32    Position actual value
  [7          ] [0x00C2.0] 0x606C:0x00 0x20 INTEGER32    Velocity actual value
  [11         ] [0x00C6.0] 0x6077:0x00 0x10 INTEGER16    Torque actual value
  [13         ] [0x00C8.0] 0x603F:0x00 0x10 UNSIGNED16   Error code
  [15         ] [0x00CA.0] 0x203F:0x01 0x40 VISIBLE_STR(64) Description
  [23         ] [0x00D2.0] 0x2702:0x00 0x20 UNSIGNED32   Tuning status
  [27         ] [0x00D6.0] 0x60FD:0x00 0x20 UNSIGNED32   Digital inputs
  [31         ] [0x00DA.0] 0x60FA:0x00 0x20 INTEGER32    Control effort
  [35         ] [0x00DE.0] 0x60FC:0x00 0x20 INTEGER32    Position demand internal value
  [39         ] [0x00E2.0] 0x606B:0x00 0x20 INTEGER32    Velocity demand value
  [43         ] [0x00E6.0] 0x6074:0x00 0x10 INTEGER16    Torque demand
  [45         ] [0x00E8.0] 0x6760:0x01 0x08 UNSIGNED8    FSoE Command
  [46         ] [0x00E9.0] 0x6640:0x00 0x01 BOOLEAN      STO command
  [46         ] [0x00E9.1] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [46         ] [0x00E9.2] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [46         ] [0x00E9.3] 0x6668:0x01 0x01 BOOLEAN      SOS
  [46         ] [0x00E9.4] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [46         ] [0x00E9.5] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [46         ] [0x00E9.6] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [46         ] [0x00E9.7] 0x6632:0x00 0x01 BOOLEAN      Error acknowledge
  [47         ] [0x00EA.0] 0x6650:0x01 0x01 BOOLEAN      SS1
  [47         ] [0x00EA.1] 0x6670:0x01 0x01 BOOLEAN      SS2
  [47         ] [0x00EA.2] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [47         ] [0x00EA.3] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [47         ] [0x00EA.4] 0x6690:0x01 0x01 BOOLEAN      SLS Instance 1
  [47         ] [0x00EA.5] 0x6690:0x02 0x01 BOOLEAN      SLS Instance 2
  [47         ] [0x00EA.6] 0x6690:0x03 0x01 BOOLEAN      SLS Instance 3
  [47         ] [0x00EA.7] 0x6690:0x04 0x01 BOOLEAN      SLS Instance 4
  [48         ] [0x00EB.0] 0x6768:0x03 0x10
  [50         ] [0x00ED.0] 0x6630:0x00 0x01 BOOLEAN      Restart acknowledge
  [50         ] [0x00ED.1] 0x6660:0x00 0x01 BOOLEAN      SBC command
  [50         ] [0x00ED.2] 0x2600:0x00 0x01 BOOLEAN      Temperature warning
  [50         ] [0x00ED.3] 0x2601:0x00 0x01 BOOLEAN      Safe position valid
  [50         ] [0x00ED.4] 0x2602:0x00 0x01 BOOLEAN      Safe speed valid
  [50         ] [0x00ED.5] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [50         ] [0x00ED.6] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [50         ] [0x00ED.7] 0x2001:0x00 0x01 INTEGER16    Commutation angle offset
  [51         ] [0x00EE.0] 0x2603:0x01 0x01 BOOLEAN      Safe input 1
  [51         ] [0x00EE.1] 0x2603:0x02 0x01 BOOLEAN      Safe input 2
  [51         ] [0x00EE.2] 0x2603:0x03 0x01 BOOLEAN      Safe input 3
  [51         ] [0x00EE.3] 0x2603:0x04 0x01 BOOLEAN      Safe input 4
  [51         ] [0x00EE.4] 0x2604:0x01 0x01 BOOLEAN      Safe output monitor 1
  [51         ] [0x00EE.5] 0x2604:0x02 0x01 BOOLEAN      Safe output monitor 2
  [51         ] [0x00EE.6] 0x2605:0x01 0x01 BOOLEAN      Analog diagnostic active
  [51         ] [0x00EE.7] 0x2605:0x01 0x01 BOOLEAN      Analog diagnostic active
  [52         ] [0x00EF.0] 0x6760:0x04 0x10 UNSIGNED16   FSoE CRC_1
  [54         ] [0x00F1.0] 0x6611:0x00 0x10 INTEGER32    Safe position actual value
  [56         ] [0x00F3.0] 0x6760:0x05 0x10 UNSIGNED16   FSoE CRC_2
  [58         ] [0x00F5.0] 0x2001:0x00 0x10 INTEGER16    Commutation angle offset
  [60         ] [0x00F7.0] 0x6760:0x06 0x10 UNSIGNED16   FSoE CRC_3
  [62         ] [0x00F9.0] 0x6613:0x00 0x10 INTEGER32    Safe velocity actual value
  [64         ] [0x00FB.0] 0x6760:0x07 0x10 UNSIGNED16   FSoE CRC_4
  [66         ] [0x00FD.0] 0x2001:0x00 0x10 INTEGER16    Commutation angle offset
  [68         ] [0x00FF.0] 0x6760:0x08 0x10 UNSIGNED16   FSoE CRC_5
  [70         ] [0x0101.0] 0x2605:0x03 0x10 INTEGER16    Safe analog value (scaled)
  [72         ] [0x0103.0] 0x6760:0x09 0x10 UNSIGNED16   FSoE CRC_6
  [74         ] [0x0105.0] 0x6760:0x02 0x10 UNSIGNED16   FSoE ConnectionID
*/

//These apply to the standard PDO mapping
#define AW_J_SERIES_SETPOS_PDO_INDEX            5
#define AW_J_SERIES_ACTPOS_PDO_INDEX            3
#define AW_J_SERIES_POS_DEMAND_PDO_INDEX        37

#define AW_J_SERIES_CONTROLWORD_PDO_INDEX       0
#define AW_J_SERIES_STATUSWORD_PDO_INDEX        0

#define AW_J_SERIES_CONTROL_EFFORT_PDO_INDEX    31

#define AW_J_SERIES_MOOSET_PDO_INDEX            2
#define AW_J_SERIES_MOODISP_PDO_INDEX           2

#define AW_J_SERIES_SETVEL_PDO_INDEX            9
#define AW_J_SERIES_ACTVEL_PDO_INDEX            7
#define AW_J_SERIES_VEL_DEMAND_PDO_INDEX        39
#define AW_J_SERIES_SETVEL_OFFSET_PDO_INDEX     31

#define AW_J_SERIES_SETTORQ_PDO_INDEX           3
#define AW_J_SERIES_SETORQ_OFFSET_PDO_INDEX     13
#define AW_J_SERIES_TORQ_DEMAND_PDO_INDEX       43
#define AW_J_SERIES_ACTTORQ_PDO_INDEX           11
#define AW_J_SERIES_ERROR_CODE_PDO_INDEX        13
#define AW_J_SERIES_ERROR_DESCRIPTION_PDO_INDEX 15


//SDO indexes for the AW J series drives

//Communication cycle period	0x1006:0	DINT	32			100		readwrite
#define AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_INDEX      0x1006
#define AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_SUB_INDEX  0x0

//Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
//Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)
#define AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX      0x607D
#define AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX  0x1
#define AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX      0x607D
#define AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX  0x2

//Error code	0x603F:0	UINT	16			0		readonly (default)
#define AW_J_SERIES_ERROR_CODE_SDO_INDEX      0x603F
#define AW_J_SERIES_ERROR_CODE_SDO_SUB_INDEX  0x0

//Max torque	0x6072:0	UINT	16	0	32767	3000		readwrite
#define AW_J_SERIES_MAX_TORQUE_SDO_INDEX      0x6072
#define AW_J_SERIES_MAX_TORQUE_SDO_SUB_INDEX  0x0

//Motor rated torque	0x6076:0	UDINT	32	1		1	mNm	readwrite
#define AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_INDEX      0x6076
#define AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_SUB_INDEX  0x0

//Motor revolutions	0x6091:1	UDINT	32			1		readwrite
#define AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_INDEX      0x6091
#define AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_SUB_INDEX  0x1

//Shaft revolutions	0x6091:2	UDINT	32			1		readwrite
#define AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_INDEX      0x6091
#define AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_SUB_INDEX  0x2

//Polarity	0x607E:0	USINT	8 (None	0, Position polarity flipped	128, Velocity polarity flipped	64, Both flipped	192		readwrite
#define AW_J_SERIES_POLARITY_SDO_INDEX      0x607E
#define AW_J_SERIES_POLARITY_SDO_SUB_INDEX  0x0

//Description	0x203F:1	STRING(8)	64			0		readonly (default)
#define AW_J_SERIES_ERROR_DESCRIPTION_SDO_INDEX      0x203F
#define AW_J_SERIES_ERROR_DESCRIPTION_SDO_SUB_INDEX  0x1

//Manufacturer software version	0x100A:0	STRING(50)	400			0		readonly (default)
#define AW_J_SERIES_MANUFACTURER_SOFTWARE_VERSION_SDO_INDEX      0x100A
#define AW_J_SERIES_MANUFACTURER_SOFTWARE_VERSION_SDO_SUB_INDEX  0x0

//    Max motor speed	0x6080:0	UDINT	32	0		1000	rpm	readwrite
#define AW_J_SERIES_MAX_MOTOR_SPEED_SDO_INDEX      0x6080
#define AW_J_SERIES_MAX_MOTOR_SPEED_SDO_SUB_INDEX  0x0

//SI unit velocity	0x60A9:0	UDINT	32
#define AW_J_SERIES_SI_UNIT_VELOCITY_SDO_INDEX      0x60A9
#define AW_J_SERIES_SI_UNIT_VELOCITY_SDO_SUB_INDEX  0x0

//Feed	0x6092:1	UDINT	32	0		4294967295		readwrite
#define AW_J_SERIES_FEED_CONSTANT_SDO_INDEX      0x6092
#define AW_J_SERIES_FEED_CONSTANT_SDO_SUB_INDEX  0x1

//Resolution1	0x2110:3	UDINT	32	1		1	Inc/Revolution
#define AW_J_SERIES_RESOLUTION_ENC1_SDO_INDEX      0x2110
#define AW_J_SERIES_RESOLUTION_ENC1_SDO_SUB_INDEX  0x3

//Resolution2	0x2112:3	UDINT	32	1		1	Inc/Revolution	readwrite
#define AW_J_SERIES_RESOLUTION_ENC2_SDO_INDEX      0x2112
#define AW_J_SERIES_RESOLUTION_ENC2_SDO_SUB_INDEX  0x3

//Assigned name 0x20F2:0	STRING(50)	400
#define AW_J_SERIES_ASSIGNED_NAME_SDO_INDEX      0x20F2
#define AW_J_SERIES_ASSIGNED_NAME_SDO_SUB_INDEX  0x0

//torque constant	0x2003:2	DINT	32			1	μNm/A	readwrite
#define AW_J_SERIES_TORQUE_CONSTANT_SDO_INDEX      0x2003
#define AW_J_SERIES_TORQUE_CONSTANT_SDO_SUB_INDEX  0x2

//Order	0x2027:2	USINT	8	0	31	3		readwrite
#define AW_J_SERIES_FIR_ORDER_SDO_INDEX      0x2027
#define AW_J_SERIES_FIR_ORDER_SDO_SUB_INDEX  0x2

//Led output pin config USINT	0x2210:1 26
#define AW_J_SERIES_LED_OP_PIN_CONFIG_SDO_INDEX      0x2210
#define AW_J_SERIES_LED_OP_PIN_CONFIG_SDO_SUB_INDEX  0x1
#define AW_J_SERIES_LED_OP_PIN_CONFIG_SDO_VALUE      26

//Led output pin voltage   USINT  0x2214:1 2
#define AW_J_SERIES_LED_OP_PIN_VOLTAGE_SDO_INDEX      0x2214
#define AW_J_SERIES_LED_OP_PIN_VOLTAGE_SDO_SUB_INDEX  0x1
#define AW_J_SERIES_LED_OP_PIN_VOLTAGE_SDO_VALUE      2


/** Max torques and speeds */
#define AW_J17_MAX_TORQUE       4016
#define AW_J17_MAX_SPEED        3000000
#define AW_J20_MAX_TORQUE       1220
#define AW_J20_MAX_SPEED        3500000
#define AW_J25_MAX_TORQUE       3991
#define AW_J25_MAX_SPEED        2500000
#define AW_J32_MAX_TORQUE       4012
#define AW_J32_MAX_SPEED        2500000
#define AW_J40_HP_MAX_TORQUE    4056
#define AW_J40_HP_MAX_SPEED     1400000
#define AW_J40_LP_MAX_TORQUE    4012
#define AW_J40_LP_MAX_SPEED     2500000


//Typedefs for the error strings and error report strings for the AW J series drives
typedef struct {
    uint16_t error_id;
    char text_string[100];
} aw_j_series_error_string_t;

typedef struct {
    char error_code[8];
    char text_string[100];
} aw_j_series_error_report_string_t;

extern const aw_j_series_error_string_t aw_j_series_error[NUM_OF_AW_J_SERIES_ERROR_STRINGS];
extern const aw_j_series_error_report_string_t aw_j_series_error_report[NUM_OF_AW_J_SERIES_ERROR_REPORT_STRINGS];


#endif //GBEM_AW_J_SERIES_H
