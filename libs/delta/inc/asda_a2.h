/**
 ******************************************************************************
 * @file           :  asda_a2.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_ASDA_A2_H
#define GBEM_ASDA_A2_H


#include "map.h"
#include "gberror.h"


#define MAX_LEN_ASDA_A2_DISPLAY_CODE 5
typedef struct {
    char display_code [MAX_LEN_ASDA_A2_DISPLAY_CODE];
    char description [MAX_DRIVE_ERROR_MSG_LENGTH];
    uint16_t numeric_code_1;
    uint16_t numeric_code_2;
}asda_a2_error_code_description_t;


#define NUM_ASDA_A2_ERROR_CODES 60

asda_a2_error_code_description_t asda_a2_error_code_description[NUM_ASDA_A2_ERROR_CODES];

/* PDO MAPPING */
//SM2 - 0x1c12
//0x1600
#define ASDA_A2_CONTROLWORD_PDO_INDEX           0
#define ASDA_A2_SETPOS_PDO_INDEX                2
#define ASDA_A2_SETVEL_PDO_INDEX                6
#define ASDA_A2_SETTORQ_PDO_INDEX               10
#define ASDA_A2_MOOSET_PDO_INDEX                12




//SM3 - 0x1c13
//0x1a00
#define ASDA_A2_STATUSWORD_PDO_INDEX           0
#define ASDA_A2_ACTPOS_PDO_INDEX                2
#define ASDA_A2_ACTVEL_PDO_INDEX                6
#define ASDA_A2_ACTTORQ_PDO_INDEX               10
#define ASDA_A2_MOOGET_PDO_INDEX                12

//SDOs

//Interpolation time units - 0x60C2:1 int8
#define ASDA_A2_INTERPOLATION_TIME_UNITS_SDO_INDEX          0x60c2
#define ASDA_A2_INTERPOLATION_TIME_UNITS_SDO_SUB_INDEX      1


//Error code - 0x603f:0 uint16
#define ASDA_A2_ERROR_CODE_SDO_INDEX 0x603f
#define ASDA_A2_ERROR_CODE_SDO_SUB_INDEX 0

//Shutdown option code - 0x605B:0 int16 0=disable drive, 1=dynamic brake - default=0
#define ASDA_A2_SHUTDOWN_OPTION_CODE_SDO_INDEX 0x605B
#define ASDA_A2_SHUTDOWN_OPTION_CODE_SDO_SUB_INDEX 0
#define ASDA_A2_SHUTDOWN_OPTION_CODE_VALUE 1

//Fault reaction option code - 0x605E:0 int16 0=disable drive, 1=slow ramp, 2=quick stop ramp - default = 2
#define ASDA_A2_FAULT_REACTION_OPTION_CODE_SDO_INDEX  0x605E
#define ASDA_A2_FAULT_REACTION_OPTION_CODE_SDO_SUB_INDEX 0
#define ASDA_A2_FAULT_REACTION_OPTION_CODE_VALUE 2

//Follow error window = 0x6065:0 uint32 - default=3840000
#define ASDA_A2_FOLLOWING_ERROR_WINDOW_SDO_INDEX  0x6065
#define ASDA_A2_FOLLOWING_ERROR_WINDOW_SDO_SUB_INDEX 0
#define ASDA_A2_FOLLOWING_ERROR_WINDOW_VALUE 3840000


//Quickstop deceleration 0x6085:0 uint32 - default=0
#define ASDA_A2_QUICKSTOP_DECELERATION_SDO_INDEX 0x6085
#define ASDA_A2_QUICKSTOP_DECELERATION_SDO_SUB_INDEX 0
#define ASDA_A2_QUICKSTOP_DECELERATION_VALUE 0


//Position factor numerator 0x6093:1 uint32 - default=1
#define ASDA_A2_POSITION_FACTOR_NUMERATOR_SDO_INDEX 0x6093
#define ASDA_A2_POSITION_FACTOR_NUMERATOR_SDO_SUB_INDEX 1
#define ASDA_A2_POSITION_FACTOR_NUMERATOR_VALUE 1

//Position factor feedconstant 0x6093:2 uint32 - default=1
#define ASDA_A2_POSITION_FACTOR_FEEDCONSTANT_SDO_INDEX 0x6093
#define ASDA_A2_POSITION_FACTOR_FEEDCONSTANT_SDO_SUB_INDEX 2
#define ASDA_A2_POSITION_FACTOR_FEEDCONSTANT_VALUE 1

//min position limit 0x607d:1 - default = -2147483648
#define ASDA_A2_MIN_LIMIT_SDO_INDEX 0x607d
#define ASDA_A2_MIN_LIMIT_SDO_SUB_INDEX 1

//max position limit 0x607d:2 int32 - default = 2147483648

#define ASDA_A2_MAX_LIMIT_SDO_INDEX 0x607d
#define ASDA_A2_MAX_LIMIT_SDO_SUB_INDEX 2

//0x607e:0 polarity uint8

#define ASDA_A2_DIRECTION_SDO_INDEX 0x607e
#define ASDA_A2_DIRECTION_SDO_SUB_INDEX 0







gberror_t ec_pdo_map_asda_a2(uint16_t slave);
gberror_t ec_initial_pdo_asda_a2(uint16_t slave);
int8_t ec_get_moo_pdo_asda_a2(uint16_t drive);
bool ec_get_remote_asda_a2(uint16_t drive);
bool ec_get_follow_error_asda_a2(uint16_t drive);
uint8_t *ec_get_error_string_sdo_asda_a2(uint16_t drive);
int32_t ec_get_actpos_wrd_asda_a2(uint16_t drive);
gberror_t ec_set_ctrl_wrd_asda_a2(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_asda_a2(uint16_t drive);
gberror_t ec_set_setpos_wrd_asda_a2(uint16_t drive, int32_t setpos);
gberror_t ec_standard_sdos_asda_a2(uint16_t slave);
gberror_t ec_print_params_asda_a2(uint16_t drive);


#define DELTA_MAN 0x000001DD
#define ASDA_A2_EEP_NAME "????"
#define ASDA_A2_EEP_MAN DELTA_MAN
#define ASDA_A2_EEP_REV 0x02040608
#define ASDA_A2_EEP_ID 0x10305070



#endif //GBEM_ASDA_A2_H
