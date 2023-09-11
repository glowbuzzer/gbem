/**
 ******************************************************************************
 * @file           :  azd-ked.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_AZD_KED_H
#define GBEM_AZD_KED_H


#include "map.h"
#include "gberror.h"

map_SM_assignment_object_t map_SM2_azd_ked;
map_SM_assignment_object_t map_SM3_azd_ked;


uint16_t map_SM2_index_of_assigned_PDO_azd_ked[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_azd_ked[ECM_MAX_PDO_MAPPING_ENTRIES];

/** Slave functions */
gberror_t ec_pdo_map_azd_ked(uint16_t slave);

gberror_t ec_initial_pdo_azd_ked(uint16_t slave);

gberror_t ec_apply_standard_sdos_azd_ked(uint16_t slave);

int8_t ec_get_moo_pdo_azd_ked(uint16_t drive);

uint8_t *ec_get_error_string_pdo_azd_ked(uint16_t drive);

gberror_t ec_nvram_sdos_azd_ked(uint16_t slave);

gberror_t ec_write_nvram_azd_ked(uint16_t slave);

/** Drive functions */
gberror_t ec_set_ctrl_wrd_azd_ked(uint16_t drive, uint16_t ctrlwrd);

uint16_t ec_get_stat_wrd_azd_ked(uint16_t drive);

int32_t ec_get_actpos_wrd_azd_ked(uint16_t drive);

gberror_t ec_set_setpos_wrd_azd_ked(uint16_t drive, int32_t setpos);

bool ec_get_remote_azd_ked(uint16_t drive);

bool ec_get_follow_error_azd_ked(uint16_t drive);

uint16_t ec_get_ctrl_wrd_rev_azd_ked(uint16_t drive);

gberror_t ec_set_stat_wrd_rev_azd_ked(uint16_t drive, uint16_t statwrd);

gberror_t ec_set_actpos_wrd_rev_azd_ked(uint16_t drive, int32_t actpos);

int32_t ec_get_setpos_word_rev_azd_ked(uint16_t drive);

gberror_t ec_set_moo_pdo_rev_azd_ked(uint16_t drive);

#define ORIENTAL_MOTOR_MAN 0x2BE
#define AZD_KED_EEP_NAME "AZD-KED"
#define AZD_KED_EEP_MAN ORIENTAL_MOTOR_MAN
#define AZD_KED_EEP_REV 0x0
#define AZD_KED_EEP_ID 0x13e5


#define NUM_OF_AZD_ERROR_STRINGS 42

/* AZD drive extended statusword bit numbers (CSP) */
#define AZD_STATUSWORD_TLC_BIT_NUM                            (15)


/* Homing mode status word */

#define AZD_KED_HOMING_WARNING_BIT_NUM                              7
#define AZD_KED_HOMING_REMOTE_BIT_NUM                               9
#define AZD_KED_HOMING_TARGET_REACHED_BIT_NUM                       10
#define AZD_KED_HOMING_LIMIT_ACTIVE_BIT_NUM                         11
#define AZD_KED_HOMING_ATTAINED_BIT_NUM                             12
#define AZD_KED_HOMING_ERROR_BIT_NUM                                13

// homing complete = AZD_HOMING_ATTAINED_BIT_NUM && AZD_HOMING_TARGET_REACHED_BIT_NUM && ! AZD_HOMING_ERROR_BIT_NUM


/* Homing mode control word */
#define AZD_KED_HOMING_START_BIT_NUM               4



/* AZD controller - controlword bit numbers (for controller command 1 */
#define AZD_KED_CONTROLLER_CONTROLWORD_ALARM_RESET_BIT_NUM            (0)

/* AZD controller - controlword bit numbers (for controller command 2 */
#define AZD_KED_CONTROLLER_CONTROLWORD_FAULT_RESET_BIT_NUM            (0)
#define AZD_KED_CONTROLLER_CONTROLWORD_STOP_BIT_NUM                    (1)

/* AZD controller - statusword bit numbers (for controller status 1 */
#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_BIT_NUM                 (0)
#define AZD_KED_CONTROLLER_STATUSWORD_CSUC_BIT_NUM                  (2)
#define AZD_KED_CONTROLLER_STATUSWORD_THALARM_BIT_NUM               (4)
#define AZD_KED_CONTROLLER_STATUSWORD_FAN_ALARM_BIT_NUM             (6)
#define AZD_KED_CONTROLLER_STATUSWORD_OH_ALARM_BIT_NUM              (7)

/* AZD controller - statusword bit numbers (for controller status 2 */
#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_1_BIT_NUM         (0)
#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_2_BIT_NUM         (1)
#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_3_BIT_NUM         (2)
#define AZD_KED_CONTROLLER_STATUSWORD_ALARM_DRIVE_4_BIT_NUM         (3)

typedef struct {
    uint16_t error_id;
    char text_string[100];
} azd_ked_error_string_t;


const azd_ked_error_string_t azd_ked_alarm_code[NUM_OF_AZD_ERROR_STRINGS];

/* These define the position of objects in the PDO - number of bytes from start of input or output */


#define AZD_KED_SETPOS_PDO_INDEX        2

#define AZD_KED_ACTPOS_PDO_INDEX        2

#define AZD_KED_CONTROLWORD_PDO_INDEX   0

#define AZD_KED_STATUSWORD_PDO_INDEX    0


#define AZD_KED_MOOSET_PDO_INDEX        10

#define AZD_KED_MOODISP_PDO_INDEX       6

#define AZD_KED_ERROR_CODE_PDO_INDEX    17


#define AZD_KED_MIN_LIMIT_SDO_INDEX     (0x607d)
#define AZD_KED_MIN_LIMIT_SDO_SUB_INDEX (0x1)

#define AZD_KED_MAX_LIMIT_SDO_INDEX     (0x607d)
#define AZD_KED_MAX_LIMIT_SDO_SUB_INDEX (0x2)

#define AZD_KED_DIRECTION_SDO_INDEX       0x41C2
#define AZD_KED_DIRECTION_SDO_SUB_INDEX   0x1

#define AZD_KED_WRITE_CONFIG_SDO_INDEX      0x40C6
#define AZD_KED_WRITE_CONFIG_SDO_SUB_INDEX  0x1
#define AZD_KED_WRITE_CONFIG_SDO_VALUE      0x1



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

#endif //GBEM_AZD_KED_H
