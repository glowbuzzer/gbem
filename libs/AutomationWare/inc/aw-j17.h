/**
 ******************************************************************************
 * @file           :  aw-j17.h
 * @brief          :  AutomationWare J17 control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_AW_J17_H
#define GBEM_AW_J17_H


#include "map.h"
#include "gberror.h"
#include "automationware.h"
//
//map_SM_assignment_object_t map_SM2_aw_j17;
//map_SM_assignment_object_t map_SM3_aw_j17;
//
//
//uint16_t map_SM2_index_of_assigned_PDO_aw_j17[ECM_MAX_PDO_MAPPING_ENTRIES];
//uint16_t map_SM3_index_of_assigned_PDO_aw_j17[ECM_MAX_PDO_MAPPING_ENTRIES];

/** Slave functions */
gberror_t ec_pdo_map_aw_j17(const uint16_t slave);
gberror_t ec_initial_pdo_aw_j17(const uint16_t slave);
gberror_t ec_apply_standard_sdos_aw_j17(uint16_t slave);
int8_t ec_get_moo_pdo_aw_j17(const uint16_t drive);
//uint8_t * ec_get_error_string_pdo_aw_j17(const uint16_t drive);
uint8_t * ec_get_error_string_sdo_aw_j17(const uint16_t drive);
gberror_t ec_nvram_sdos_aw_j17(const uint16_t slave);
gberror_t ec_write_nvram_aw_j17(const uint16_t slave);

/** Drive functions */
gberror_t ec_set_ctrl_wrd_aw_j17(const uint16_t drive, const uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_aw_j17(const uint16_t drive);
int32_t ec_get_actpos_wrd_aw_j17(const uint16_t drive);
gberror_t ec_set_setpos_wrd_aw_j17(const uint16_t drive, const int32_t setpos);
bool ec_get_remote_aw_j17(const uint16_t drive);
bool ec_get_follow_error_aw_j17(const uint16_t drive);

uint16_t ec_get_ctrl_wrd_rev_aw_j17(const uint16_t drive);
gberror_t ec_set_stat_wrd_rev_aw_j17(const uint16_t drive, const uint16_t statwrd);
gberror_t ec_set_actpos_wrd_rev_aw_j17(const uint16_t drive, const int32_t actpos);
int32_t ec_get_setpos_word_rev_aw_j17(const uint16_t drive);
gberror_t ec_set_moo_pdo_rev_aw_j17(const uint16_t drive);

#define AW_J17_EEP_NAME "SOMANET"
#define AW_J17_EEP_MAN AW_MOTOR_MAN
#define AW_J17_EEP_REV 0x0a000002
#define AW_J17_EEP_ID 0x00000201

#define NUM_OF_AW_J17_ERROR_STRINGS 26




//
///* AZD drive extended statusword bit numbers (CSP) */
//#define AZD_STATUSWORD_TLC_BIT_NUM                      		(15)
//
//
///* Homing mode status word */
//
//#define AZD_KED_HOMING_WARNING_BIT_NUM                              7
//#define AZD_KED_HOMING_REMOTE_BIT_NUM                               9
//#define AZD_KED_HOMING_TARGET_REACHED_BIT_NUM                       10
//#define AZD_KED_HOMING_LIMIT_ACTIVE_BIT_NUM                         11
//#define AZD_KED_HOMING_ATTAINED_BIT_NUM                             12
//#define AZD_KED_HOMING_ERROR_BIT_NUM                                13
//
//// homing complete = AZD_HOMING_ATTAINED_BIT_NUM && AZD_HOMING_TARGET_REACHED_BIT_NUM && ! AZD_HOMING_ERROR_BIT_NUM
//
//
///* Homing mode control word */
//#define AZD_KED_HOMING_START_BIT_NUM               4
//
//
//
///* AZD controller - controlword bit numbers (for controller command 1 */
//#define AZD_KED_CONTROLLER_CONTROLWORD_ALARM_RESET_BIT_NUM    		(0)
//
///* AZD controller - controlword bit numbers (for controller command 2 */
//#define AZD_KED_CONTROLLER_CONTROLWORD_FAULT_RESET_BIT_NUM    		(0)
//#define AZD_KED_CONTROLLER_CONTROLWORD_STOP_BIT_NUM            		(1)
//
///* AZD controller - statusword bit numbers (for controller status 1 */
//#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_BIT_NUM                 (0)
//#define AZD_KED_CONTROLLER_STATUSWORD_CSUC_BIT_NUM                  (2)
//#define AZD_KED_CONTROLLER_STATUSWORD_THALARM_BIT_NUM               (4)
//#define AZD_KED_CONTROLLER_STATUSWORD_FAN_ALARM_BIT_NUM             (6)
//#define AZD_KED_CONTROLLER_STATUSWORD_OH_ALARM_BIT_NUM              (7)
//
///* AZD controller - statusword bit numbers (for controller status 2 */
//#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_1_BIT_NUM         (0)
//#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_2_BIT_NUM         (1)
//#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_3_BIT_NUM         (2)
//#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_4_BIT_NUM         (3)
//
////
////
//
///* These define the position of objects in the PDO - number of bytes from start of input or output */
////need to decide on these
//
///*

typedef struct {
    uint16_t error_id;
    char text_string[100];
} aw_j17_error_string_t;

extern const aw_j17_error_string_t aw_j17_alarm_code[NUM_OF_AW_J17_ERROR_STRINGS];


#define AW_J17_SETPOS_PDO_INDEX        5

#define AW_J17_ACTPOS_PDO_INDEX        3

#define AW_J17_CONTROLWORD_PDO_INDEX   0

#define AW_J17_STATUSWORD_PDO_INDEX    0

#define AW_J17_MOOSET_PDO_INDEX        2

#define AW_J17_MOODISP_PDO_INDEX       2

#define AW_J17_ACTVEL_PDO_INDEX        7

#define AW_J17_ACTTORQ_PDO_INDEX       11


#define AZD_KED_MIN_LIMIT_SDO_INDEX     (0x607d)
#define AZD_KED_MIN_LIMIT_SDO_SUB_INDEX (0x1)

#define AZD_KED_MAX_LIMIT_SDO_INDEX     (0x607d)
#define AZD_KED_MAX_LIMIT_SDO_SUB_INDEX (0x2)

#define AZD_KED_DIRECTION_SDO_INDEX       0x41C2
#define AZD_KED_DIRECTION_SDO_SUB_INDEX   0x1

#define AZD_KED_WRITE_CONFIG_SDO_INDEX      0x40C6
#define AZD_KED_WRITE_CONFIG_SDO_SUB_INDEX  0x1
#define AZD_KED_WRITE_CONFIG_SDO_VALUE      0x1

//Communication cycle period	0x1006:0	DINT	32			100		readwrite
#define AW_J17_COMMUNICATION_CYCLE_PERIOD_SDO_INDEX      0x1006
#define AW_J17_COMMUNICATION_CYCLE_PERIOD_SDO_SUB_INDEX  0x0


//Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
//Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)
#define AW_J17_MIN_POSITION_LIMIT_SDO_INDEX      0x607D
#define AW_J17_MIN_POSITION_LIMIT_SDO_SUB_INDEX  0x1
#define AW_J17_MAX_POSITION_LIMIT_SDO_INDEX      0x607D
#define AW_J17_MAX_POSITION_LIMIT_SDO_SUB_INDEX  0x2

//Error code	0x603F:0	UINT	16			0		readonly (default)
#define AW_J17_ERROR_CODE_SDO_INDEX      0x603F
#define AW_J17_ERROR_CODE_SDO_SUB_INDEX  0x0


//Max torque	0x6072:0	UINT	16	0	32767	3000		readwrite

#define AW_J17_MAX_TORQUE_SDO_INDEX      0x6072
#define AW_J17_MAX_TORQUE_SDO_SUB_INDEX  0x0


//Motor rated torque	0x6076:0	UDINT	32	1		1	mNm	readwrite
#define AW_J17_MOTOR_RATED_TORQUE_SDO_INDEX      0x6076
#define AW_J17_MOTOR_RATED_TORQUE_SDO_SUB_INDEX  0x0

//Motor revolutions	0x6091:1	UDINT	32			1		readwrite
#define AW_J17_MOTOR_REVOLUTIONS_SDO_INDEX      0x6091
#define AW_J17_MOTOR_REVOLUTIONS_SDO_SUB_INDEX  0x1

//Shaft revolutions	0x6091:2	UDINT	32			1		readwrite
#define AW_J17_SHAFT_REVOLUTIONS_SDO_INDEX      0x6091
#define AW_J17_SHAFT_REVOLUTIONS_SDO_SUB_INDEX  0x2



//Polarity	0x607E:0	USINT	8 (None	0, Position polarity flipped	128, Velocity polarity flipped	64, Both flipped	192		readwrite
#define AW_J17_POLARITY_SDO_INDEX      0x607E
#define AW_J17_POLARITY_SDO_SUB_INDEX  0x0


//Description	0x203F:1	STRING(8)	64			0		readonly (default)
#define AW_J17_ERROR_DESCRIPTION_SDO_INDEX      0x203F
#define AW_J17_ERROR_DESCRIPTION_SDO_SUB_INDEX  0x1




#endif //GBEM_AW_J17_H
