/**
 ******************************************************************************
 * @file           :  EL7211.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL7211_H
#define GBEM_EL7211_H

#include "beckhoff_shared.h"
#include "map.h"
#include "gberror.h"



map_SM_assignment_object_t map_SM2_EL7211;
map_SM_assignment_object_t map_SM3_EL7211;
uint16_t map_SM2_index_of_assigned_PDO_EL7211[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_EL7211[ECM_MAX_PDO_MAPPING_ENTRIES];

gberror_t ec_initial_pdo_EL7211(uint16_t slave);
gberror_t ec_standard_sdos_EL7211(uint16_t slave);
gberror_t ec_pdo_map_EL7211(uint16_t slave);
int8_t ec_get_moo_sdo_EL7211(uint16_t drive);
uint8_t * ec_get_error_string_sdo_EL7211(uint16_t drive);
gberror_t ec_nvram_sdos_EL7211(uint16_t slave);
gberror_t ec_write_nvram_EL7211(uint16_t slave);
gberror_t ec_set_ctrl_wrd_EL7211(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_EL7211(uint16_t drive);
uint16_t ec_get_ctrl_wrd_rev_EL7211(uint16_t drive);
gberror_t ec_set_stat_wrd_rev_EL7211(uint16_t drive, uint16_t statwrd);
gberror_t ec_set_actpos_wrd_rev_EL7211(uint16_t drive, int32_t actpos);
int32_t ec_get_setpos_word_rev_EL7211(uint16_t drive);
gberror_t ec_set_moo_pdo_rev_EL7211(uint16_t drive);
int32_t ec_get_actpos_wrd_EL7211(uint16_t drive);
gberror_t ec_set_setpos_wrd_EL7211(uint16_t drive, int32_t setpos);
bool ec_get_remote_EL7211(uint16_t drive);
bool ec_get_follow_error_EL7211(uint16_t drive);


// EL7211-9014 1Ch. MDP742 Servo motor output stage with OCT (50V, 4.5A RMS) and STO inpu

#define EL7211_EEP_NAME "EL7211-9014"
#define EL7211_EEP_MAN BECKHOFF_MAN
#define EL7211_EEP_REV  0x001b2336
#define EL7211_EEP_ID   0x1c2b3052



#define NUM_EL7211_ERROR_CODES 49
#define NUM_EL7211_WARNING_CODES 3


#define EL7211_SETPOS_PDO_INDEX          2 //4 bytes
#define EL7211_ACTPOS_PDO_INDEX          0 //4 bytes
#define EL7211_ACTVEL_PDO_INDEX          6 //4 bytes
#define EL7211_ACTTORQ_PDO_INDEX         10 //2 bytes
#define EL7211_INFO_DATA1_PDO_INDEX          12 //2 bytes
#define EL7211_INFO_DATA2_PDO_INDEX          14 //2 bytes
#define EL7211_FOLLOWERROR_ACTVAL_PDO_INDEX          16 //4 bytes


#define EL7211_CONTROLWORD_PDO_INDEX     0 //2 bytes
#define EL7211_STATUSWORD_PDO_INDEX      4 //2bytes

#define EL7211_MOODISP_PDO_INDEX         20 //1 byte
#define EL7211_MOOSET_PDO_INDEX          6 // 1byte

#define EL7211_MOO_SET_SDO_INDEX 0x7010
#define EL7211_MOO_SET_SDO_SUB_INDEX 0x3


#define EL7211_MOO_GET_SDO_INDEX 0x6010
#define EL7211_MOO_GET_SDO_SUB_INDEX 0x3



#define EL7211_ERROR_CODE_SDO_INDEX 0x10F3

#define EL7211_FOLLOW_ERROR_TOLERANCE     100000

#define EL7211_AUTO_CONFIG_MOTOR_SDO_INDEX                      0x8008
#define EL7211_AUTO_CONFIG_MOTOR_SDO_SUB_INDEX                  0x1
#define EL7211_AUTO_CONFIG_MOTOR_SDO_VALUE                      0x1

#define EL7211_AUTO_CONFIG_IDENTICAL_MOTOR_SDO_INDEX            0x8008
#define EL7211_AUTO_CONFIG_IDENTICAL_MOTOR_SUB_SDO_INDEX        0x2
#define EL7211_AUTO_CONFIG_IDENTICAL_MOTOR_SDO_VALUE            0x0

#define EL7211_AUTO_CONFIG_NONIDENTICAL_MOTOR_SDO_INDEX         0x8008
#define EL7211_AUTO_CONFIG_NONIDENTICAL_MOTOR_SDO_SUB_INDEX     0x3
#define EL7211_AUTO_CONFIG_NONIDENTICAL_MOTOR_SDO_VALUE         0x1

//EL7211 doesnt have position limits

//0x8010:65 direction of rotation??
//#define EL721_DIRECTION_SDO_INDEX       0x8010
//#define EL721_DIRECTION_SDO_SUB_INDEX   65
//#define EL721_DIRECTION_SDO_VALUE       0x0

typedef enum {
    EL7211_ERROR_ADCERROR,
    EL7211_ERROR_OVERCURRENT,
    EL7211_ERROR_UNDERVOLTAGE,
    EL7211_ERROR_OVERVOLTAGE,
    EL7211_ERROR_OVERTEMPERATURE,
    EL7211_ERROR_I2TAMPLIFIER,
    EL7211_ERROR_I2TMOTOR,
    EL7211_ERROR_ENCODER,
    EL7211_ERROR_WATCHDOG,
    EL7211_ERROR_NONE,
    EL7211_WARNING_UNDERVOLATGE,
    EL7211_WARNING_OVERVOLTAGE,
    EL7211_WARNING_OVERTEMPERATURE,
    EL7211_WARNING_I2TAMPLIFIER,
    EL7211_WARNING_I2TMOTOR,
    EL7211_WARNING_ENCODER,
    EL7211_WARNING_NONE,
} EL7211_error_warning_t;


#define EL7211_ERROR_ADCERROR_BIT_NUM           0
#define EL7211_ERROR_OVERCURRENT_BIT_NUM        1
#define EL7211_ERROR_UNDERVOLTAGE_BIT_NUM       2
#define EL7211_ERROR_OVERVOLATGE_BIT_NUM        3
#define EL7211_ERROR_OVERTEMPERATURE_BIT_NUM    4
#define EL7211_ERROR_I2TAMPLIFER_BIT_NUM        5
#define EL7211_ERROR_I2TMOTOR_BIT_NUM           6
#define EL7211_ERROR_ENCODER_BIT_NUM            7
#define EL7211_ERROR_WATCHDOG_BIT_NUM           8


#define EL7211_WARNING_UNDERVOLATGE_BIT_NUM     2
#define EL7211_WARNING_OVERVOLTAGE_BIT_NUM      3
#define EL7211_WARNING_OVERTEMPERATURE_BIT_NUM  4
#define EL7211_WARNING_I2TAMPLIFIER_BIT_NUM     5
#define EL7211_WARNING_I2TMOTOR_BIT_NUM         6
#define EL7211_WARNING_ENCODER_BIT_NUM          7




#endif //GBEM_EL7211_H
