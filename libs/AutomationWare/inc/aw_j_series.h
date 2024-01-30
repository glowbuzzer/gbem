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
bool ec_get_safety_state_aw_j_series(void);

/** Slave functions */

gberror_t ec_initial_pdo_aw_j_series(uint16_t slave);

int8_t ec_get_moo_pdo_aw_j_series(uint16_t drive);

gberror_t ec_apply_limits_aw_j_series(uint16_t slave);

/** Error functions */
uint8_t *ec_get_error_string_sdo_aw_j_series(uint16_t drive);

uint8_t *ec_get_detailled_error_report_sdo_aw_j_series(uint16_t drive_number);

/** Drive functions */
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


//Number of error strings and error report strings for the AW J series drives
#define NUM_OF_AW_J_SERIES_ERROR_STRINGS 26
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

//PDO indexes for the AW J series drives - these are most easily found from a netscan of the drive

/*
PDO mapping according to CoE :
SM2 outputs
[byte_offset] [addr b  ] [index:sub  ] [bitl] [data_type   ] [name                                    ]
[0          ] [0xFFF6F13C.0] [0x6040:0x00] [0x10] [UNSIGNED16  ] [Controlword                             ]
[2          ] [0xFFF6F13E.0] [0x6060:0x00] [0x08] [INTEGER8    ] [Modes of operation                      ]
[3          ] [0xFFF6F13F.0] [0x6071:0x00] [0x10] [INTEGER16   ] [Target Torque                           ]
[5          ] [0xFFF6F141.0] [0x607A:0x00] [0x20] [INTEGER32   ] [Target position                         ]
[9          ] [0xFFF6F145.0] [0x60FF:0x00] [0x20] [INTEGER32   ] [Target velocity                         ]
[13         ] [0xFFF6F149.0] [0x60B2:0x00] [0x10] [INTEGER16   ] [Torque offset                           ]
[15         ] [0xFFF6F14B.0] [0x2701:0x00] [0x20] [UNSIGNED32  ] [Tuning command                          ]
[19         ] [0xFFF6F14F.0] [0x60FE:0x01] [0x20] [UNSIGNED32  ] [Physical outputs                        ]
[23         ] [0xFFF6F153.0] [0x60FE:0x02] [0x20] [UNSIGNED32  ] [Bit mask                                ]
[27         ] [0xFFF6F157.0] [0x2703:0x00] [0x20] [UNSIGNED32  ] [User MOSI                               ]
[31         ] [0xFFF6F15B.0] [0x60B1:0x00] [0x20] [INTEGER32   ] [Velocity offset                         ]
SM3 inputs
[byte_offset] [addr b  ] [index: sub ] [bitl] [data_type   ] [name                                    ]
[0          ] [0xFFF6F13C.0] [0x6041:0x00] [0x10] [UNSIGNED16  ] [Statusword                              ]
[2          ] [0xFFF6F13E.0] [0x6061:0x00] [0x08] [INTEGER8    ] [Modes of operation display              ]
[3          ] [0xFFF6F13F.0] [0x6064:0x00] [0x20] [INTEGER32   ] [Position actual value                   ]
[7          ] [0xFFF6F143.0] [0x606C:0x00] [0x20] [INTEGER32   ] [Velocity actual value                   ]
[11         ] [0xFFF6F147.0] [0x6077:0x00] [0x10] [INTEGER16   ] [Torque actual value                     ]
[13         ] [0xFFF6F149.0] [0x2401:0x00] [0x10] [UNSIGNED16  ] [Analog input 1                          ]
[15         ] [0xFFF6F14B.0] [0x2402:0x00] [0x10] [UNSIGNED16  ] [Analog input 2                          ]
[17         ] [0xFFF6F14D.0] [0x2403:0x00] [0x10] [UNSIGNED16  ] [Analog input 3                          ]
[19         ] [0xFFF6F14F.0] [0x2404:0x00] [0x10] [UNSIGNED16  ] [Analog input 4                          ]
[21         ] [0xFFF6F151.0] [0x2702:0x00] [0x20] [UNSIGNED32  ] [Tuning status                           ]
[25         ] [0xFFF6F155.0] [0x60FD:0x00] [0x20] [UNSIGNED32  ] [Digital inputs                          ]
[29         ] [0xFFF6F159.0] [0x2704:0x00] [0x20] [UNSIGNED32  ] [User MISO                               ]
[33         ] [0xFFF6F15D.0] [0x20F0:0x00] [0x20] [UNSIGNED32  ] [Timestamp                               ]
[37         ] [0xFFF6F161.0] [0x60FC:0x00] [0x20] [INTEGER32   ] [Position demand internal value          ]
[41         ] [0xFFF6F165.0] [0x606B:0x00] [0x20] [INTEGER32   ] [Velocity demand value                   ]
[45         ] [0xFFF6F169.0] [0x6074:0x00] [0x10] [INTEGER16   ] [Torque demand                           ]
*/


//These apply to the standard PDO mapping
#define AW_J_SERIES_SETPOS_PDO_INDEX        5
#define AW_J_SERIES_ACTPOS_PDO_INDEX        3
#define AW_J_SERIES_POS_DEMAND_PDO_INDEX    37

#define AW_J_SERIES_CONTROLWORD_PDO_INDEX   0
#define AW_J_SERIES_STATUSWORD_PDO_INDEX    0

//todo crit
#define AW_J_SERIES_CONTROL_EFFORT_PDO_INDEX 0

#define AW_J_SERIES_MOOSET_PDO_INDEX        2
#define AW_J_SERIES_MOODISP_PDO_INDEX       2

#define AW_J_SERIES_SETVEL_PDO_INDEX        9
#define AW_J_SERIES_ACTVEL_PDO_INDEX        7
#define AW_J_SERIES_VEL_DEMAND_PDO_INDEX    41
#define AW_J_SERIES_SETVEL_OFFSET_PDO_INDEX 31

#define AW_J_SERIES_SETTORQ_PDO_INDEX       3
#define AW_J_SERIES_SETORQ_OFFSET_PDO_INDEX 13
#define AW_J_SERIES_TORQ_DEMAND_PDO_INDEX   45
#define AW_J_SERIES_ACTTORQ_PDO_INDEX       11


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
