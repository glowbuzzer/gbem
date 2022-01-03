/**
 ******************************************************************************
 * @file           :  AX5101.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_AX5101_H
#define GBEM_AX5101_H

#include "beckhoff_shared.h"
#include "map.h"
#include "gberror.h"



map_SM_assignment_object_t map_SM2_AX5101;
map_SM_assignment_object_t map_SM3_AX5101;
uint16_t map_SM2_index_of_assigned_PDO_AX5101[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_AX5101[ECM_MAX_PDO_MAPPING_ENTRIES];

gberror_t ec_initial_pdo_AX5101(uint16_t slave);
gberror_t ec_standard_sdos_AX5101(uint16_t slave);
gberror_t ec_pdo_map_AX5101(uint16_t slave);
int8_t ec_get_moo_sdo_AX5101(uint16_t drive);
uint8_t * ec_get_error_string_sdo_AX5101(uint16_t drive);
gberror_t ec_nvram_sdos_AX5101(uint16_t slave);
gberror_t ec_write_nvram_AX5101(uint16_t slave);
gberror_t ec_set_ctrl_wrd_AX5101(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_AX5101(uint16_t drive);
uint16_t ec_get_ctrl_wrd_rev_AX5101(uint16_t drive);
gberror_t ec_set_stat_wrd_rev_AX5101(uint16_t drive, uint16_t statwrd);
gberror_t ec_set_actpos_wrd_rev_AX5101(uint16_t drive, int32_t actpos);
int32_t ec_get_setpos_word_rev_AX5101(uint16_t drive);
gberror_t ec_set_moo_pdo_rev_AX5101(uint16_t drive);
int32_t ec_get_actpos_wrd_AX5101(uint16_t drive);
gberror_t ec_set_setpos_wrd_AX5101(uint16_t drive, int32_t setpos);
bool ec_get_remote_AX5101(uint16_t drive);
bool ec_get_follow_error_AX5101(uint16_t drive);



#define AX5101_EEP_NAME "?"
#define AX5101_EEP_MAN BECKHOFF_MAN
#define AX5101_EEP_REV  0x001b2336
#define AX5101_EEP_ID   0x1c2b3052



#define NUM_AX5101_ERROR_CODES 49
#define NUM_AX5101_WARNING_CODES 3


#define AX5101_SETPOS_PDO_INDEX          2 //4 bytes
#define AX5101_ACTPOS_PDO_INDEX          0 //4 bytes
#define AX5101_ACTVEL_PDO_INDEX          6 //4 bytes
#define AX5101_ACTTORQ_PDO_INDEX         10 //2 bytes
#define AX5101_INFO_DATA1_PDO_INDEX          12 //2 bytes
#define AX5101_INFO_DATA2_PDO_INDEX          14 //2 bytes
#define AX5101_FOLLOWERROR_ACTVAL_PDO_INDEX          16 //4 bytes


#define AX5101_CONTROLWORD_PDO_INDEX     0 //2 bytes
#define AX5101_STATUSWORD_PDO_INDEX      4 //2bytes

#define AX5101_MOODISP_PDO_INDEX         20 //1 byte
#define AX5101_MOOSET_PDO_INDEX          6 // 1byte

#define AX5101_MOO_SET_SDO_INDEX 0x7010
#define AX5101_MOO_SET_SDO_SUB_INDEX 0x3


#define AX5101_MOO_GET_SDO_INDEX 0x6010
#define AX5101_MOO_GET_SDO_SUB_INDEX 0x3



#define AX5101_ERROR_CODE_SDO_INDEX 0x10F3

#define AX5101_FOLLOW_ERROR_TOLERANCE     100000

#define AX5101_AUTO_CONFIG_MOTOR_SDO_INDEX                      0x8008
#define AX5101_AUTO_CONFIG_MOTOR_SDO_SUB_INDEX                  0x1
#define AX5101_AUTO_CONFIG_MOTOR_SDO_VALUE                      0x1

#define AX5101_AUTO_CONFIG_IDENTICAL_MOTOR_SDO_INDEX            0x8008
#define AX5101_AUTO_CONFIG_IDENTICAL_MOTOR_SUB_SDO_INDEX        0x2
#define AX5101_AUTO_CONFIG_IDENTICAL_MOTOR_SDO_VALUE            0x0

#define AX5101_AUTO_CONFIG_NONIDENTICAL_MOTOR_SDO_INDEX         0x8008
#define AX5101_AUTO_CONFIG_NONIDENTICAL_MOTOR_SDO_SUB_INDEX     0x3
#define AX5101_AUTO_CONFIG_NONIDENTICAL_MOTOR_SDO_VALUE         0x1

//AX5101 doesnt have position limits

//0x8010:65 direction of rotation??
//#define EL721_DIRECTION_SDO_INDEX       0x8010
//#define EL721_DIRECTION_SDO_SUB_INDEX   65
//#define EL721_DIRECTION_SDO_VALUE       0x0

typedef enum {
    AX5101_ERROR_ADCERROR,
    AX5101_ERROR_OVERCURRENT,
    AX5101_ERROR_UNDERVOLTAGE,
    AX5101_ERROR_OVERVOLTAGE,
    AX5101_ERROR_OVERTEMPERATURE,
    AX5101_ERROR_I2TAMPLIFIER,
    AX5101_ERROR_I2TMOTOR,
    AX5101_ERROR_ENCODER,
    AX5101_ERROR_WATCHDOG,
    AX5101_ERROR_NONE,
    AX5101_WARNING_UNDERVOLATGE,
    AX5101_WARNING_OVERVOLTAGE,
    AX5101_WARNING_OVERTEMPERATURE,
    AX5101_WARNING_I2TAMPLIFIER,
    AX5101_WARNING_I2TMOTOR,
    AX5101_WARNING_ENCODER,
    AX5101_WARNING_NONE,
} AX5101_error_warning_t;


#define AX5101_ERROR_ADCERROR_BIT_NUM           0
#define AX5101_ERROR_OVERCURRENT_BIT_NUM        1
#define AX5101_ERROR_UNDERVOLTAGE_BIT_NUM       2
#define AX5101_ERROR_OVERVOLATGE_BIT_NUM        3
#define AX5101_ERROR_OVERTEMPERATURE_BIT_NUM    4
#define AX5101_ERROR_I2TAMPLIFER_BIT_NUM        5
#define AX5101_ERROR_I2TMOTOR_BIT_NUM           6
#define AX5101_ERROR_ENCODER_BIT_NUM            7
#define AX5101_ERROR_WATCHDOG_BIT_NUM           8


#define AX5101_WARNING_UNDERVOLATGE_BIT_NUM     2
#define AX5101_WARNING_OVERVOLTAGE_BIT_NUM      3
#define AX5101_WARNING_OVERTEMPERATURE_BIT_NUM  4
#define AX5101_WARNING_I2TAMPLIFIER_BIT_NUM     5
#define AX5101_WARNING_I2TMOTOR_BIT_NUM         6
#define AX5101_WARNING_ENCODER_BIT_NUM          7




#endif //GBEM_AX5101_H
