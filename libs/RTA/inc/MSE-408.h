/**
 ******************************************************************************
 * @file           :  MSE-408.h
 * @brief          :  MSE-408 EtherCAT FLEX-Drive
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_MSE_408_H
#define GBEM_MSE_408_H


#include "map.h"
#include "gberror.h"
#include "RTA.h"


map_SM_assignment_object_t map_SM2_mse_408;
map_SM_assignment_object_t map_SM3_mse_408;


uint16_t map_SM2_index_of_assigned_PDO_mse_408[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_mse_408[ECM_MAX_PDO_MAPPING_ENTRIES];

/** Slave functions */
gberror_t ec_pdo_map_mse_408(uint16_t slave);

gberror_t ec_initial_pdo_mse_408(uint16_t slave);

gberror_t ec_apply_standard_sdos_mse_408(uint16_t slave);

int8_t ec_get_moo_pdo_mse_408(uint16_t drive);


/** Drive functions */
gberror_t ec_set_ctrl_wrd_mse_408(uint16_t drive, uint16_t ctrlwrd);

uint16_t ec_get_stat_wrd_mse_408(uint16_t drive);

int32_t ec_get_actpos_wrd_mse_408(uint16_t drive);

gberror_t ec_set_setpos_wrd_mse_408(uint16_t drive, int32_t setpos);

bool ec_get_remote_mse_408(uint16_t drive);

bool ec_get_follow_error_mse_408(uint16_t drive);

uint16_t ec_get_ctrl_wrd_rev_mse_408(uint16_t drive);

gberror_t ec_set_stat_wrd_rev_mse_408(uint16_t drive, uint16_t statwrd);

gberror_t ec_set_actpos_wrd_rev_mse_408(uint16_t drive, int32_t actpos);

int32_t ec_get_setpos_word_rev_mse_408(uint16_t drive);

gberror_t ec_set_moo_pdo_rev_mse_408(uint16_t drive);

uint8_t *ec_get_error_string_sdo_mse_408(uint16_t drive);

#define MSE_408_EEP_NAME "RTA MSE-408 EtherCAT FLEX-Drive"
#define MSE_408_EEP_MAN RTA_MOTOR_MAN
#define MSE_408_EEP_REV 0x0000001F
#define MSE_408_EEP_ID 0x00000044


#define NUM_OF_MSE_408_ERROR_STRINGS 7




/* Homing mode status word */

#define MSE_408_HOMING_WARNING_BIT_NUM                              7
#define MSE_408_HOMING_REMOTE_BIT_NUM                               9
#define MSE_408_HOMING_TARGET_REACHED_BIT_NUM                       10
#define MSE_408_HOMING_LIMIT_ACTIVE_BIT_NUM                         11
#define MSE_408_HOMING_ATTAINED_BIT_NUM                             12


/* Homing mode control word */


typedef struct {
    uint16_t error_id;
    char text_string[100];
} mse_408_error_string_t;


const mse_408_error_string_t mse_408_error_code[NUM_OF_MSE_408_ERROR_STRINGS];

/* These define the position of objects in the PDO - number of bytes from start of input or output */


#define MSE_408_CONTROLWORD_PDO_INDEX   0
#define MSE_408_MOOSET_PDO_INDEX        2
#define MSE_408_SETPOS_PDO_INDEX        3

#define MSE_408_STATUSWORD_PDO_INDEX    0
#define MSE_408_MOODISP_PDO_INDEX       2
#define MSE_408_ACTPOS_PDO_INDEX        3


#define MSE_408_ERROR_CODE_PDO_INDEX    17

//1 I32 RW
// 0 = no limit?
//Minimum position range limit
#define MSE_408_MIN_POSITION_LIMIT_SDO_INDEX     (0x607b)
#define MSE_408_MIN_POSITION_LIMIT_SDO_SUB_INDEX (0x1)

//2 I32 RW
#define MSE_408_MAX_POSITION_LIMIT_SDO_INDEX     (0x607b)
#define MSE_408_MAX_POSITION_LIMIT_SDO_SUB_INDEX (0x2)

//Revolution Direction (CW / CCW setting) 0x3212 0 U8 RW 1,2 0x00 (0) 0 / 1

#define MSE_408_DIRECTION_SDO_INDEX       0x3212
#define MSE_408_DIRECTION_SDO_SUB_INDEX   0x0


//Homing Method - 0x6098 - 0 - I8 - RW - 37 (default) -  1 - 3 / 4 / 5 / 6 / 19 / 20 / 21 / 22 / 33 / 34 / 35 / 37 / -1 (255) / -2 (254)

#define MSE_408_HOMING_METHOD_SDO_INDEX       0x6098
#define MSE_408_HOMING_METHOD_SDO_SUB_INDEX   0x0

//Modes of Operation 0x6060 0 I8 RW 0 1 / 3 / 6 / 8 / 9

#define MSE_408_MODE_OF_OPERATION_SDO_INDEX       0x6060
#define MSE_408_MODE_OF_OPERATION_SDO_SUB_INDEX   0x0

//Mode of Operation Display 0x6061 0 I8 RO 0 1 / 3 / 6 / 8 / 9
#define MSE_408_MODE_OF_OPERATION_DISPLAY_SDO_INDEX       0x6061
#define MSE_408_MODE_OF_OPERATION_DISPLAY_SDO_SUB_INDEX   0x0

//Max Motor Speed 0x6080 0 U32 RW 1 300 000 0…800 000
#define MSE_408_MAX_MOTOR_SPEED_SDO_INDEX       0x6080
#define MSE_408_MAX_MOTOR_SPEED_SDO_SUB_INDEX   0x0


//Homing Speeds - Speed during Search for Switch - 0x6099 1 U32 RW 1 0x5DC0 (24000) 16001…400 000
#define MSE_408_HOMING_SPEEDS_SEARCH_FOR_SWITCH_SDO_INDEX       0x6099
#define MSE_408_HOMING_SPEEDS_SEARCH_FOR_SWITCH_SDO_SUB_INDEX   0x1

//Homing Speeds - Speed during Search for Zero - 0x6099 2 U16 or U32 RW 1 0x0C80 (3200) 0… 16000
#define MSE_408_HOMING_SPEEDS_SEARCH_FOR_ZERO_SDO_INDEX       0x6099
#define MSE_408_HOMING_SPEEDS_SEARCH_FOR_ZERO_SDO_SUB_INDEX   0x2

//Homing Acceleration 0x609A 0 U32 RW 1 0x9C400 (640000) 2000…10 000 000
#define MSE_408_HOMING_ACCELERATION_SDO_INDEX       0x609A
#define MSE_408_HOMING_ACCELERATION_SDO_SUB_INDEX   0x0


//Error Code 0x603F 0 U16 RO 1 0x0000 (0) 0x 0000…0x8611
#define MSE_408_ERROR_CODE_SDO_INDEX       0x603F
#define MSE_408_ERROR_CODE_SDO_SUB_INDEX   0x0

//Motor Code 0x3210 0 U32 RW RO 1,2 MSE-408: 0x00001544 (5444)
#define MSE_408_MOTOR_CODE_SDO_INDEX       0x3210
#define MSE_408_MOTOR_CODE_SDO_SUB_INDEX   0x0


#define MSE_408_MAX_MOTOR_SPEED (300000)

#define MSE_408_HOMING_SPEED_SEARCH_FOR_SWITCH (24000)
#define MSE_408_HOMING_SPEED_SEARCH_FOR_ZERO (3200)
#define MSE_408_HOMING_ACCELERATION (640000)

#define MSE_408_MOTOR_CODE (5444)


#endif //GBEM_MSE_408_H
