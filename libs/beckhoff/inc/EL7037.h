/**
 ******************************************************************************
 * @file           :  EL7037.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL7037_H
#define GB_MONOREPO_EL7037_H

#include "beckhoff_shared.h"
#include "map.h"
#include "gberror.h"


//SM2
#define EL7037_CONTROLWORD_PDO_INDEX           6
#define EL7037_SETPOS_PDO_INDEX                8

//SM3
#define EL7037_STATUSWORD_PDO_INDEX            10
#define EL7037_ACTPOS_PDO_INDEX                2



map_SM_assignment_object_t map_SM2_el7037;
map_SM_assignment_object_t map_SM3_el7037;
uint16_t map_SM2_index_of_assigned_PDO_el7037[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_el7037[ECM_MAX_PDO_MAPPING_ENTRIES];


gberror_t ec_pdo_map_el7037(uint16_t slave);
int8_t ec_get_moo_pdo_el7037(uint16_t drive);
bool ec_get_remote_el7037(uint16_t drive);
bool ec_get_follow_error_el7037(uint16_t drive);
uint8_t *ec_get_error_string_sdo_el7037(uint16_t drive);
int32_t ec_get_actpos_wrd_el7037(uint16_t drive);
gberror_t ec_set_ctrl_wrd_el7037(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_el7037(uint16_t drive);
gberror_t ec_set_setpos_wrd_el7037(uint16_t drive, int32_t setpos);
gberror_t ec_standard_sdos_el7037(uint16_t slave);
gberror_t ec_print_params_el7037(uint16_t drive);


///todo
#define EL7037_EEP_NAME "EL7037"
#define EL7037_EEP_MAN BECKHOFF_MAN
#define EL7037_EEP_REV 0x00190000
#define EL7037_EEP_ID 0x1B773052


//0x8012:05 uint8 - speed range = 0=1000 steps/sec, 1=2000 steps/sec, 2=4000 steps/sec, 3=8000 steps/sec, 4=16000 steps/sec, 5=32000 steps/sec
#define EL7037_SPEED_RANGE_SDO_INDEX                    0x8012
#define EL7037_SPEED_RANGE_SDO_SUB_INDEX                5
#define EL7037_SPEED_RANGE_SDO_VALUE                    1


//0x8012:08 bool - Feedback type = 0=Encoder, 1= Internal counter (default)
#define EL7037_FEEDBACK_TYPE_SDO_INDEX                    0x8012
#define EL7037_FEEDBACK_TYPE_SDO_SUB_INDEX                8
#define EL7037_FEEDBACK_TYPE_SDO_VALUE                    1

//0x8010:3 uint16 nominal voltage - value 0x960 = 2400 = 24V
#define EL7037_NOMINAL_VOLTAGE_SDO_INDEX                0x8010
#define EL7037_NOMINAL_VOLTAGE_SDO_SUB_INDEX            3
#define EL7037_NOMINAL_VOLTAGE_SDO_VALUE                0x960

//0x8010:4 uint16 motor coil resistance - value = 0x64 = 100 = 100 ohms
#define EL7037_MOTOR_COIL_RESISTANCE_SDO_INDEX          0x8010
#define EL7037_MOTOR_COIL_RESISTANCE_SDO_SUB_INDEX      4
#define EL7037_MOTOR_COIL_RESISTANCE_SDO_VALUE          0x64


//0x8011:1 uint16  Kp factor current - default = 0x96 = 150
#define EL7037_KP_CURRENT_SDO_INDEX                    0x8011
#define EL7037_KP_CURRENT_SDO_SUB_INDEX                1
#define EL7037_KP_CURRENT_SDO_VALUE                    0x96


//0x8011:2 uint16 Ki factor current - default = 0xa = 10
#define EL7037_KI_CURRENT_SDO_INDEX                    0x8011
#define EL7037_KI_CURRENT_SDO_SUB_INDEX                2
#define EL7037_KI_CURRENT_SDO_VALUE                    0xa

//0x8014:1 uint32 feed forward pos - default = 100000
#define EL7037_FEED_FORWARD_SDO_INDEX                    0x8014
#define EL7037_FEED_FORWARD_SDO_SUB_INDEX                1
#define EL7037_FEED_FORWARD_SDO_VALUE                    100000


//0x8014:2 unit16 Kp factorpos - default = 0x1f4 = 500
#define EL7037_KP_POS_SDO_INDEX                    0x8014
#define EL7037_KP_POS_SDO_SUB_INDEX                2
#define EL7037_KP_POS_SDO_VALUE                    0x1f4


//0x8012:01 uint8 - operation mode = 3 = position controller - 5 = ext pos controller
#define EL7037_OPERATION_MODE_SDO_INDEX                 0x8012
#define EL7037_OPERATION_MODE_SDO_SUB_INDEX             1
#define EL7037_OPERATION_MODE_SDO_VALUE                 3


//Only stepper motors from Beckhoff (AS10xx) are supported with extended pos mode


//0x8010:1 uint16 max current - value 0x5dc = 1500 = 1.5A????
#define EL7037_MAX_CURRENT_SDO_INDEX                    0x8010
#define EL7037_MAX_CURRENT_SDO_SUB_INDEX                1
#define EL7037_MAX_CURRENT_SDO_VALUE                    0x2ee

//0x8010:2 uint16 reduced current - value 0x2ee = 750 = 0.75A
#define EL7037_REDUCED_CURRENT_SDO_INDEX                0x8010
#define EL7037_REDUCED_CURRENT_SDO_SUB_INDEX            2
#define EL7037_REDUCED_CURRENT_SDO_VALUE                0x2ee



//0x8010:6 uint16 motor full steps - value = 0xc8 = 200 = 200 steps
#define EL7037_MOTOR_FULL_STEPS_SDO_INDEX               0x8010
#define EL7037_MOTOR_FULL_STEPS_SDO_SUB_INDEX           6
#define EL7037_MOTOR_FULL_STEPS_SDO_VALUE               0xc8



#define EL7037_CONTROL_ENABLE_BIT_NUM           0
#define EL7037_CONTROL_RESET_BIT_NUM            1
#define EL7037_CONTROL_REDUCE_TORQUE_BIT_NUM    1


//0x8012:09 uint8 - invert motor polarity = 0 or 1
#define EL7037_INVERT_MOTOR_POLARITY_SDO_INDEX          0x8012
#define EL7037_INVERT_MOTOR_POLARITY_SDO_SUB_INDEX      9


//status word bit numbers
#define EL7037_STATUS_READY_TO_ENABLE_BIT_NUM       0
#define EL7037_STATUS_READY_BIT_NUM                 1
#define EL7037_STATUS_WARNING_BIT_NUM               2
#define EL7037_STATUS_ERROR_BIT_NUM                 3
#define EL7037_STATUS_MOVING_POS_BIT_NUM            4
#define EL7037_STATUS_MOVING_NEG_BIT_NUM            5
#define EL7037_STATUS_TORQUE_REDUCED_BIT_NUM        6
#define EL7037_STATUS_MOTOR_STALL_BIT_NUM           7
#define EL7037_STATUS_SYNC_ERROR_BIT_NUM            14



//0xa010: array of bools - diag data
#define EL7037_DIAG_SDO_INDEX       0xa010
#define EL7037_DIAG_SDO_SUB_INDEX   0
#define EL7037_DIAG_SIZE            (48/8)



//EL7037 diag bit numbers - taken from esi xml
    #define EL7037_ERROR_SATURATED_BIT_NUM              16
    #define EL7037_ERROR_OVER_TEMPERATURE_BIT_NUM       17
    #define EL7037_ERROR_TORQUE_OVERLOAD_BIT_NUM        18
    #define EL7037_ERROR_UNDER_VOLTAGE_BIT_NUM          19
    #define EL7037_ERROR_OVER_VOLTAGE_BIT_NUM           20
    #define EL7037_ERROR_SHORT_CIRCUIT_A_BIT_NUM        21
    #define EL7037_ERROR_SHORT_CIRCUIT_B_BIT_NUM        22
    #define EL7037_ERROR_NO_CONTROL_POWER_BIT_NUM       23
    #define EL7037_ERROR_MISC_ERROR_BIT_NUM             24
    #define EL7037_ERROR_CONFIG_ERROR_BIT_NUM           25
    #define EL7037_ERROR_MOTOR_STALL_BIT_NUM            26




//0x8012:05 - speed range = default 2000 (1)


//0x8010:03  Nominal voltage - value 0xc350 = 50000 = 50v


#endif //GB_MONOREPO_EL7037_H
