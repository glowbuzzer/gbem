/**
 ******************************************************************************
 * @file           :  n5.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_N5_H
#define GB_MONOREPO_N5_H

#include "map.h"
#include "gberror.h"

typedef struct {
    uint8_t number;
    char description [150];
} n5_error_number_t;

typedef struct {
    uint16_t code;
    char description [100];
} n5_error_code_t;



//SM2
#define N5_CONTROLWORD_PDO_INDEX           0
#define N5_SETPOS_PDO_INDEX                2
#define N5_MOOSET_PDO_INDEX                10

//SM3
#define N5_STATUSWORD_PDO_INDEX            0
#define N5_ACTPOS_PDO_INDEX                2
#define N5_MOOGET_PDO_INDEX                6

//These apply to 0x1001 which can be added to the TxPDO or these are bits 16-23 in 0x1003:n (uint32) error stack
#define N5_GENERAL_ERROR_BIT_NUM            0
#define N5_CURRENT_ERROR_BIT_NUM            1
#define N5_VOLTAGE_ERROR_BIT_NUM            2
#define N5_TEMPERATURE_ERROR_BIT_NUM        3
#define N5_COMMUNICATION_ERROR_BIT_NUM      4
#define N5_PROFILE_ERROR_BIT_NUM            5
//bit six is reserved
#define N5_MOTOR_WRONG_DIR_ERROR_BIT_NUM    7




#define N5_STATUSWORD_SYNC_WITH_FIELDBUS_BIT_NUM 8




//0x607d:1 int32 - min position limit
#define N5_MIN_POS_LIMIT_SDO_INDEX                          0x607d
#define N5_MIN_POS_LIMIT_SDO_SUB_INDEX                      0x1

//0x607d:2 int32 - max position limit
#define N5_MAX_POS_LIMIT_SDO_INDEX                          0x607d
#define N5_MAX_POS_LIMIT_SDO_SUB_INDEX                      0x2

//0x607e:0 uint8 - polarity (direction of rotation)
#define N5_DIRECTION_OF_ROTATION_SDO_INDEX                 0x607e
#define N5_DIRECTION_OF_ROTATION_SDO_SUB_INDEX             0x0


//0x605a:0 int16 - quick stop option code
// 0 = immediate stop
// 1 = braking with slow down ramp then change to switch on disabled <- default
// 2 = braking with quick stop ramp then change to switch on disabled

#define N5_QUICKSTOP_OPTION_CODE_SDO_INDEX                  0x605a
#define N5_QUICKSTOP_OPTION_CODE_SDO_SUB_INDEX              0
#define N5_QUICKSTOP_OPTION_CODE_VALUE                      1


/* GEAR RATIO */
//0x6091:1 uint32 - motor revolutions (numerator for gear ratio)
#define N5_GEAR_MOTOR_REVOLUTIONS_SDO_INDEX                      0x6091
#define N5_GEAR_MOTOR_REVOLUTIONS_SDO_SUB_INDEX                  1
#define N5_GEAR_MOTOR_REVOLUTIONS_VALUE                          1

//0x6091:2 uint32 - shaft revolutions (denominator for gear ratio)
#define N5_GEAR_SHAFT_REVOLUTIONS_SDO_INDEX                      0x6091
#define N5_GEAR_SHAFT_REVOLUTIONS_SDO_SUB_INDEX                  2
#define N5_GEAR_SHAFT_REVOLUTIONS_VALUE                          1

/* FEED CONSTANT */
//0x6092:1 uint32 - feed constant
#define N5_FEED_CONSTANT_SDO_INDEX                          0x6092
#define N5_FEED_CONSTANT_SDO_SUB_INDEX                      1
#define N5_FEED_CONSTANT_VALUE                              1

//0x6092:2 uint32 - feed shaft revolutions
#define N5_FEED_SHAFT_REVOLUTIONS_SDO_INDEX                 0x6092
#define N5_FEED_SHAFT_REVOLUTIONS_SDO_SUB_INDEX             2
#define N5_FEED_SHAFT_REVOLUTIONS_VALUE                     1

//0x60c2:1 uint8 - interpolation time period
#define N5_INTERPOLATION_TIME_PERIOD_SDO_INDEX              0x60c2
#define N5_INTERPOLATION_TIME_PERIOD_SDO_SUB_INDEX          1
#define N5_INTERPOLATION_TIME_PERIOD_VALUE

//0x603f:0 uint16 - error code (this is lower 16 bits of 0x1003)


//0x1003:0 uint8 number of errors
//0x1003:1-to-8 uint32 - error field -
//bits 24-31 = error number
//bits 16-23 = error class
//bits 0-15 = error code

//uint8
#define N5_PREDEFINED_ERROR_NUM_OF_ERRORS_SDO_INDEX         0x1003
#define N5_PREDEFINED_ERROR_NUM_OF_ERRORS_SDO_SUB_INDEX     0

#define N5_PREDEFINED_ERROR_SDO_INDEX                       0x1003




gberror_t ec_pdo_map_n5(uint16_t slave);
gberror_t ec_initial_pdo_n5(uint16_t slave);
int8_t ec_get_moo_pdo_n5(uint16_t drive);
bool ec_get_remote_n5(uint16_t drive);
bool ec_get_follow_error_n5(uint16_t drive);
uint8_t *ec_get_error_string_sdo_n5(uint16_t drive);
int32_t ec_get_actpos_wrd_n5(uint16_t drive);
gberror_t ec_set_ctrl_wrd_n5(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_n5(uint16_t drive);
gberror_t ec_set_setpos_wrd_n5(uint16_t drive, int32_t setpos);
gberror_t ec_standard_sdos_n5(uint16_t slave);
gberror_t ec_print_params_n5(uint16_t drive);


#define NANOTEC_MAN 0x0000026C
#define N5_EEP_NAME "Drive"
#define N5_EEP_MAN NANOTEC_MAN
#define N5_EEP_REV 0x0000026C
#define N5_EEP_ID 0x00000007


#endif //GB_MONOREPO_N5_H
