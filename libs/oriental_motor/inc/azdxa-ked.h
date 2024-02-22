/**
 ******************************************************************************
 * @file           :  azdxa-ked.h
 * @brief          :  azdxa-ked drive control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy, Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef INC_AZDXA_KED_H_
#define INC_AZDXA_KED_H_

#include "map.h"
#include "gberror.h"

extern map_SM_assignment_object_t map_SM2_azd2a_ked;
extern map_SM_assignment_object_t map_SM3_azd2a_ked;
extern map_SM_assignment_object_t map_SM2_azd3a_ked;
extern map_SM_assignment_object_t map_SM3_azd3a_ked;
extern map_SM_assignment_object_t map_SM2_azd4a_ked;
extern map_SM_assignment_object_t map_SM3_azd4a_ked;


extern uint16_t map_SM2_index_of_assigned_PDO_azd2a_ked[ECM_MAX_PDO_MAPPING_ENTRIES];
extern uint16_t map_SM3_index_of_assigned_PDO_azd2a_ked[ECM_MAX_PDO_MAPPING_ENTRIES];
extern uint16_t map_SM2_index_of_assigned_PDO_azd3a_ked[ECM_MAX_PDO_MAPPING_ENTRIES];
extern uint16_t map_SM3_index_of_assigned_PDO_azd3a_ked[ECM_MAX_PDO_MAPPING_ENTRIES];
extern uint16_t map_SM2_index_of_assigned_PDO_azd4a_ked[ECM_MAX_PDO_MAPPING_ENTRIES];
extern uint16_t map_SM3_index_of_assigned_PDO_azd4a_ked[ECM_MAX_PDO_MAPPING_ENTRIES];

/** Machine functions */
bool ec_get_safety_state_azdxa_ked(uint16_t slave);


/** Slave functions */
gberror_t ec_pdo_map_azd2a_ked(uint16_t slave);

gberror_t ec_pdo_map_azd3a_ked(uint16_t slave);

gberror_t ec_pdo_map_azd4a_ked(uint16_t slave);

gberror_t ec_pdo_map_custom_azd3a_ked(uint16_t slave);

gberror_t ec_initial_pdo_azdxa_ked(uint16_t slave);

gberror_t ec_apply_standard_sdos_azdxa_ked(uint16_t slave);

int8_t ec_get_moo_pdo_azdxa_ked(uint16_t drive);

uint8_t *ec_get_error_string_pdo_azdxa_ked(uint16_t drive);

uint8_t *ec_get_error_string_sdo_azdxa_ked(uint16_t drive);

gberror_t ec_nvram_sdos_azdxa_ked(uint16_t slave);

gberror_t ec_write_nvram_azdxa_ked(uint16_t slave);

/** Drive functions */
gberror_t ec_set_ctrl_wrd_azdxa_ked(uint16_t drive, uint16_t ctrlwrd);

uint16_t ec_get_stat_wrd_azdxa_ked(uint16_t drive);

int32_t ec_get_actpos_wrd_azdxa_ked(uint16_t drive);

int32_t ec_get_actvel_wrd_azdxa_ked(uint16_t drive);


gberror_t ec_set_setpos_wrd_azdxa_ked(uint16_t drive, int32_t setpos);

gberror_t ec_set_setvel_wrd_azdxa_ked(uint16_t drive, int32_t setvel);


bool ec_get_remote_azdxa_ked(uint16_t drive);

bool ec_get_follow_error_azdxa_ked(uint16_t drive);

uint16_t ec_get_ctrl_wrd_rev_azdxa_ked(uint16_t drive);

gberror_t ec_set_stat_wrd_rev_azdxa_ked(uint16_t drive, uint16_t statwrd);

gberror_t ec_set_actpos_wrd_rev_azdxa_ked(uint16_t drive, int32_t actpos);

int32_t ec_get_setpos_word_rev_azdxa_ked(uint16_t drive);

gberror_t ec_set_moo_pdo_rev_azdxa_ked(uint16_t drive);

#define ORIENTAL_MOTOR_MAN 0x2BE
#define AZD3A_KED_EEP_NAME "AZD3A-KED"
#define AZD3A_KED_EEP_MAN ORIENTAL_MOTOR_MAN
#define AZD3A_KED_EEP_REV 0x13
#define AZD3A_KED_EEP_ID 0x1


#define AZD2A_KED_EEP_NAME "AZD2A-KED"
#define AZD2A_KED_EEP_MAN ORIENTAL_MOTOR_MAN
#define AZD2A_KED_EEP_REV 0x00110301
#define AZD2A_KED_EEP_ID 0x000013AF

#define AZD4A_KED_EEP_NAME "AZD4A-KED"
#define AZD4A_KED_EEP_MAN ORIENTAL_MOTOR_MAN
#define AZD4A_KED_EEP_REV 0x13
#define AZD4A_KED_EEP_ID 0x1


/* AZD SDO addresses */
//6065h 00h Following error window U32 RW No 300 1 to 30,000 (1=0.01 rev) - Axis1

#define AZD_KED_FOLLOWING_ERROR_WINDOW_AXIS_1_SDO_INDEX 0x6065
#define AZD_KED_FOLLOWING_ERROR_WINDOW_AXIS_1_SDO_SUB_INDEX 0x0

//6865h 00h Following error window U32 RW No 300 1 to 30,000 (1=0.01 rev) - Axis2
#define AZD_KED_FOLLOWING_ERROR_WINDOW_AXIS_2_SDO_INDEX 0x6865
#define AZD_KED_FOLLOWING_ERROR_WINDOW_AXIS_2_SDO_SUB_INDEX 0x0

//7065h 00h Following error window U32 RW No 300 1 to 30,000 (1=0.01 rev) - Axis3
#define AZD_KED_FOLLOWING_ERROR_WINDOW_AXIS_3_SDO_INDEX 0x7065
#define AZD_KED_FOLLOWING_ERROR_WINDOW_AXIS_3_SDO_SUB_INDEX 0x0

//7865h 00h Following error window U32 RW No 300 1 to 30,000 (1=0.01 rev) - Axis4
#define AZD_KED_FOLLOWING_ERROR_WINDOW_AXIS_4_SDO_INDEX 0x7865
#define AZD_KED_FOLLOWING_ERROR_WINDOW_AXIS_4_SDO_SUB_INDEX 0x0

#define AZD_KED_FOLLOWING_ERROR_WINDOW 10

#define AZD_KED_ALARM_HISTORY_1_SDO_INDEX 0x4041


//    40C2h 00h Clear alarm history Clears alarm history.
#define AZD_KED_CLEAR_ALARM_HISTORY_SDO_INDEX 0x40C2


#define NUM_OF_AZD_ERROR_STRINGS 42

/* AZD drive extended statusword bit numbers (CSP) */
#define AZD_STATUSWORD_TLC_BIT_NUM                            (15)


/* Homing mode status word */

#define AZDXA_KED_HOMING_WARNING_BIT_NUM                              7
#define AZDXA_KED_HOMING_REMOTE_BIT_NUM                               9
#define AZDXA_KED_HOMING_TARGET_REACHED_BIT_NUM                       10
#define AZDXA_KED_HOMING_LIMIT_ACTIVE_BIT_NUM                         11
#define AZDXA_KED_HOMING_ATTAINED_BIT_NUM                             12
#define AZDXA_KED_HOMING_ERROR_BIT_NUM                                13

// homing complete = AZD_HOMING_ATTAINED_BIT_NUM && AZD_HOMING_TARGET_REACHED_BIT_NUM && ! AZD_HOMING_ERROR_BIT_NUM


/* Homing mode control word */
#define AZDXA_KED_HOMING_START_BIT_NUM               4


/* AZD controller - controlword bit numbers (for controller command 1 */
#define AZDXA_KED_CONTROLLER_CONTROLWORD_ALARM_RESET_BIT_NUM            (0)

/* AZD controller - controlword bit numbers (for controller command 2 */
#define AZDXA_KED_CONTROLLER_CONTROLWORD_FAULT_RESET_BIT_NUM            (0)
#define AZDXA_KED_CONTROLLER_CONTROLWORD_STOP_BIT_NUM                    (1)

/* AZD controller - statusword bit numbers (for controller status 1 */
#define AZDXA_KED_CONTROLLER_STATUSWORD_ALARM_BIT_NUM                 (0)
#define AZDXA_KED_CONTROLLER_STATUSWORD_CSUC_BIT_NUM                  (2)
#define AZDXA_KED_CONTROLLER_STATUSWORD_THALARM_BIT_NUM               (4)
#define AZDXA_KED_CONTROLLER_STATUSWORD_FAN_ALARM_BIT_NUM             (6)
#define AZDXA_KED_CONTROLLER_STATUSWORD_OH_ALARM_BIT_NUM              (7)

/* AZD controller - statusword bit numbers (for controller status 2 */
#define AZDXA_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_1_BIT_NUM         (0)
#define AZDXA_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_2_BIT_NUM         (1)
#define AZDXA_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_3_BIT_NUM         (2)
#define AZDXA_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_4_BIT_NUM         (3)

typedef struct {
    uint16_t error_id;
    char text_string[100];
} azdxa_ked_error_string_t;


extern const azdxa_ked_error_string_t azdxa_ked_alarm_code[NUM_OF_AZD_ERROR_STRINGS];

/* These define the position of objects in the PDO - number of bytes from start of input or output */

//static const uint16_t AZDXA_KED_SETPOS_DRIVE_1_INDEX = 2;

#if AZDXA_STANDARD_PDO_MAPPING
#define AZDXA_KED_SETPOS_PDO_DRIVE1_INDEX        2
#define AZDXA_KED_SETPOS_PDO_DRIVE2_INDEX        9
#define AZDXA_KED_SETPOS_PDO_DRIVE3_INDEX        16
#define AZDXA_KED_SETPOS_PDO_DRIVE4_INDEX        23

#define AZDXA_KED_ACTPOS_PDO_DRIVE1_INDEX        2
#define AZDXA_KED_ACTPOS_PDO_DRIVE2_INDEX        25
#define AZDXA_KED_ACTPOS_PDO_DRIVE3_INDEX        48
#define AZDXA_KED_ACTPOS_PDO_DRIVE4_INDEX        71

#define AZDXA_KED_CONTROLWORD_PDO_DRIVE1_INDEX   0
#define AZDXA_KED_CONTROLWORD_PDO_DRIVE2_INDEX   7
#define AZDXA_KED_CONTROLWORD_PDO_DRIVE3_INDEX   14
#define AZDXA_KED_CONTROLWORD_PDO_DRIVE4_INDEX   32

#define AZDXA_KED_STATUSWORD_PDO_DRIVE1_INDEX    0
#define AZDXA_KED_STATUSWORD_PDO_DRIVE2_INDEX    23
#define AZDXA_KED_STATUSWORD_PDO_DRIVE3_INDEX    46
#define AZDXA_KED_STATUSWORD_PDO_DRIVE4_INDEX    69

#define AZDXA_KED_CONTROLLER_COMMAND1_PDO_INDEX   21
#define AZDXA_KED_CONTROLLER_COMMAND2_PDO_INDEX   22
#define AZDXA_KED_CONTROLLER_STATUS1_PDO_INDEX    69
#define AZDXA_KED_CONTROLLER_STATUS2_PDO_INDEX    70

#define AZDXA_KED_MOOSET_PDO_DRIVE1_INDEX        6
#define AZDXA_KED_MOOSET_PDO_DRIVE2_INDEX        13
#define AZDXA_KED_MOOSET_PDO_DRIVE3_INDEX        20
#define AZDXA_KED_MOOSET_PDO_DRIVE4_INDEX        27

#define AZDXA_KED_MOODISP_PDO_DRIVE1_INDEX       6
#define AZDXA_KED_MOODISP_PDO_DRIVE2_INDEX       29
#define AZDXA_KED_MOODISP_PDO_DRIVE3_INDEX       52
#define AZDXA_KED_MOODISP_PDO_DRIVE4_INDEX       75

#define AZDXA_KED_ERROR_CODE_PDO_DRIVE1_INDEX    17
#define AZDXA_KED_ERROR_CODE_PDO_DRIVE2_INDEX    40
#define AZDXA_KED_ERROR_CODE_PDO_DRIVE3_INDEX    63
#define AZDXA_KED_ERROR_CODE_PDO_DRIVE4_INDEX    86

#else

#define AZDXA_KED_SETPOS_PDO_DRIVE1_INDEX        2
#define AZDXA_KED_SETPOS_PDO_DRIVE2_INDEX        13 // 13-2=11
#define AZDXA_KED_SETPOS_PDO_DRIVE3_INDEX        24 //24-13=11
#define AZDXA_KED_SETPOS_PDO_DRIVE4_INDEX        35


#define AZDXA_KED_SETVEL_PDO_DRIVE1_INDEX        6
#define AZDXA_KED_SETVEL_PDO_DRIVE2_INDEX        17
#define AZDXA_KED_SETVEL_PDO_DRIVE3_INDEX        28
#define AZDXA_KED_SETVEL_PDO_DRIVE4_INDEX        39


#define AZDXA_KED_ACTPOS_PDO_DRIVE1_INDEX        2
#define AZDXA_KED_ACTPOS_PDO_DRIVE2_INDEX        15 // 15-2 = 13
#define AZDXA_KED_ACTPOS_PDO_DRIVE3_INDEX        28 // 28-15 = 13
#define AZDXA_KED_ACTPOS_PDO_DRIVE4_INDEX        41


#define AZDXA_KED_ACTVEL_PDO_DRIVE1_INDEX        6
#define AZDXA_KED_ACTVEL_PDO_DRIVE2_INDEX        19
#define AZDXA_KED_ACTVEL_PDO_DRIVE3_INDEX        32
#define AZDXA_KED_ACTVEL_PDO_DRIVE4_INDEX        45


#define AZDXA_KED_CONTROLWORD_PDO_DRIVE1_INDEX   0
#define AZDXA_KED_CONTROLWORD_PDO_DRIVE2_INDEX   11
#define AZDXA_KED_CONTROLWORD_PDO_DRIVE3_INDEX   22
#define AZDXA_KED_CONTROLWORD_PDO_DRIVE4_INDEX   33

#define AZDXA_KED_STATUSWORD_PDO_DRIVE1_INDEX    0
#define AZDXA_KED_STATUSWORD_PDO_DRIVE2_INDEX    13
#define AZDXA_KED_STATUSWORD_PDO_DRIVE3_INDEX    26
#define AZDXA_KED_STATUSWORD_PDO_DRIVE4_INDEX    39


#define AZDXA_KED_MOOSET_PDO_DRIVE1_INDEX        10
#define AZDXA_KED_MOOSET_PDO_DRIVE2_INDEX        21
#define AZDXA_KED_MOOSET_PDO_DRIVE3_INDEX        32
#define AZDXA_KED_MOOSET_PDO_DRIVE4_INDEX        43

#define AZDXA_KED_MOODISP_PDO_DRIVE1_INDEX       10
#define AZDXA_KED_MOODISP_PDO_DRIVE2_INDEX       23
#define AZDXA_KED_MOODISP_PDO_DRIVE3_INDEX       36
#define AZDXA_KED_MOODISP_PDO_DRIVE4_INDEX       49


#define AZDXA_KED_ERROR_CODE_PDO_DRIVE1_INDEX    11
#define AZDXA_KED_ERROR_CODE_PDO_DRIVE2_INDEX    24
#define AZDXA_KED_ERROR_CODE_PDO_DRIVE3_INDEX    37
#define AZDXA_KED_ERROR_CODE_PDO_DRIVE4_INDEX    50

//depends on whether AZD-2A, AZD-3A or AZD-4A
//#define AZDXA_KED_CONTROLLER_COMMAND1_PDO_INDEX   ??
//#define AZDXA_KED_CONTROLLER_COMMAND2_PDO_INDEX   ??
//#define AZDXA_KED_CONTROLLER_STATUS1_PDO_INDEX    ??
//#define AZDXA_KED_CONTROLLER_STATUS2_PDO_INDEX    ??

#endif


#define AZDXA_KED_MIN_LIMIT_SDO_DRIVE1_INDEX     (0x607d)
#define AZDXA_KED_MIN_LIMIT_SDO_DRIVE1_SUB_INDEX (0x1)

#define AZDXA_KED_MAX_LIMIT_SDO_DRIVE1_INDEX     (0x607d)
#define AZDXA_KED_MAX_LIMIT_SDO_DRIVE1_SUB_INDEX (0x2)

#define AZDXA_KED_MIN_LIMIT_SDO_DRIVE2_INDEX     (0x607d+(1*0x800))
#define AZDXA_KED_MIN_LIMIT_SDO_DRIVE2_SUB_INDEX (0x1)

#define AZDXA_KED_MAX_LIMIT_SDO_DRIVE2_INDEX     (0x607d+(1*0x800))
#define AZDXA_KED_MAX_LIMIT_SDO_DRIVE2_SUB_INDEX (0x2)

#define AZDXA_KED_MIN_LIMIT_SDO_DRIVE3_INDEX     (0x607d+(2*0x800))
#define AZDXA_KED_MIN_LIMIT_SDO_DRIVE3_SUB_INDEX (0x1)

#define AZDXA_KED_MAX_LIMIT_SDO_DRIVE3_INDEX     (0x607d+(2*0x800))
#define AZDXA_KED_MAX_LIMIT_SDO_DRIVE3_SUB_INDEX (0x2)

#define AZDXA_KED_MIN_LIMIT_SDO_DRIVE4_INDEX     (0x607d+(3*0x800))
#define AZDXA_KED_MIN_LIMIT_SDO_DRIVE4_SUB_INDEX (0x1)

#define AZDXA_KED_MAX_LIMIT_SDO_DRIVE4_INDEX     (0x607d+(3*0x800))
#define AZDXA_KED_MAX_LIMIT_SDO_DRIVE4_SUB_INDEX (0x2)

#define AZDXA_KED_DIRECTION_SDO_DRIVE1_INDEX       0x41C2
#define AZDXA_KED_DIRECTION_SDO_DRIVE1_SUB_INDEX   0x1

#define AZDXA_KED_DIRECTION_SDO_DRIVE2_INDEX       0x41C2
#define AZDXA_KED_DIRECTION_SDO_DRIVE2_SUB_INDEX   0x2


#define AZDXA_KED_DIRECTION_SDO_DRIVE3_INDEX       0x41C2
#define AZDXA_KED_DIRECTION_SDO_DRIVE3_SUB_INDEX   0x3


#define AZDXA_KED_DIRECTION_SDO_DRIVE4_INDEX       0x41C2
#define AZDXA_KED_DIRECTION_SDO_DRIVE4_SUB_INDEX   0x4


#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE1_INDEX      0x40C6
#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE1_SUB_INDEX  0x1
#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE1_VALUE      0x1

#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE2_INDEX      0x40C6
#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE2_SUB_INDEX  0x2
#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE2_VALUE      0x1

#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE3_INDEX      0x40C6
#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE3_SUB_INDEX  0x3
#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE3_VALUE      0x1

#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE4_INDEX      0x40C6
#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE4_SUB_INDEX  0x4
#define AZDXA_KED_WRITE_CONFIG_SDO_DRIVE4_VALUE      0x1


/*
0x10=4 blinks of LED - Excessive position deviation
0x20=5  blinks of LED - Overcurrent
0x21=2 blinks of LED - Main circuit overheat
0x22=3 blinks of LED - Overvoltage
0x23=3 blinks of LED - Main power off
0x25=3 blinks of LED - Undervoltage
0x26=8 blinks of LED - Motor overheat
0x28=8 blinks od LED - Sensor error
0x29=9 blinks of LED - CPU peripheral circuit error
0x2A=8 blinks of LED -  ABZO sensor communication error
0x30=2 blinks of LED - Overload
0x31=2 blinks of LED - Overspeed
0x33=7 blinks of LED - Absolute position error
0x34=2 blinks of LED - Command pulse error
0x35=2 blinks of LED - Alarm detection of other axes
0x41=9 blinks of LED -  EEPROM error
0x42=8 blinks of LED -  Sensor error at power on
0x43=8 blinks of LED - Rotation error at power on
0x44=8 blinks of LED - Encoder EEPROM error
0x45=8 blinks of LED - Motor combination error
0x4A=7 blinks of LED - Return-to-home incomplete
0x51=2 blinks of LED - Regeneration resistor overheat
0x54=2 blinks of LED - Fan rotation deterioration
0x60=7 blinks of LED -  ±LS both sides active
0x61=7 blinks of LED - Reverse ±LS connection
0x62=7 blinks of LED - Return-to-home operation error
0x63=7 blinks of LED -  No HOMES
0x64=7 blinks of LED - TIM, Z, SLIT signal error
0x66=7 blinks of LED -  Hardware overtravel
0x67=7 blinks of LED -  Software overtravel
0x6A=7 blinks of LED - Return-to-home operation offset error
0x6D=7 blinks of LED - Mechanical overtravel
0x70=7 blinks of LED -  Operation data error
0x71=7 blinks of LED - Electronic gear setting error
0x72=7 blinks of LED -  Wrap setting error
0x81=7 blinks of LED -  Network bus error
0x82=7 blinks of LED - Driver internal communication error 1
0x84=7 blinks of LED - Driver internal communication error 2
0x85=7 blinks of LED - Driver internal communication timeout
0xF0=LED Lit - CPU error
*/
#endif /* INC_AZDXA_KED_H_ */
