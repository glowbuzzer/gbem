/**
 ******************************************************************************
 * @file           :  g5.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_G5_H
#define GBEM_G5_H

#include "map.h"
#include "gberror.h"

//SM2
#define G5_CONTROLWORD_PDO_INDEX           0
#define     G5_SETPOS_PDO_INDEX                2

//SM3
#define G5_ERROR_CODE_PDO_INDEX            0
#define G5_STATUSWORD_PDO_INDEX            2
#define G5_ACTPOS_PDO_INDEX                4
#define G5_ACTACC_PDO_INDEX                8
#define G5_ACTFOLLOWWERROR_PDO_INDEX       10


//SDOS

//The limit function has four limits, the torque limit, speed limit, drive prohibition input, and software limit.

//int16
#define G5_DRIVE_PROHIBITION_INPUT_SDO_INDEX 0x3504
#define G5_DRIVE_PROHIBITION_INPUT_SDO_SUB_INDEX 0
#define G5_DRIVE_PROHIBITION_INPUT_SDO_VALUE 1

//int32
#define G5_SOFTWARE_POS_LIMIT_MIN_SDO_INDEX 0x607d
#define G5_SOFTWARE_POS_LIMIT_MIN_SDO_SUB_INDEX 1

//int32
#define G5_SOFTWARE_POS_LIMIT_MAX_SDO_INDEX 0x607d
#define G5_SOFTWARE_POS_LIMIT_MAX_SDO_SUB_INDEX 2




//0x6060:0 int8 - Modes of operation set
#define G5_MOOSET_SDO_INDEX                    0x6060
#define G5_MOOSET_SDO_SUB_INDEX                0

//0x6061:0 int8 - Modes of operation get
#define G5_MOOGET_SDO_INDEX                    0x6061
#define G5_MOOGET_SDO_SUB_INDEX                0

//0x3000:0 int16 0/1 changes motor direction, default is 1
#define G5_DIRECTION_SDO_INDEX                  0x3000
#define G5_DIRECTION_SDO_SUB_INDEX              1


//0x3002:0 int16 realtime auto-tuning - 0 disabled, 1 focus on stability (default), 2 focus on position, 3 vertical axis, 4 friction comp for vertical, 5 load estimate, 6 custom
#define G5_REALTIME_AUTOTUNING_MODE_SDO_INDEX       0x03002
#define G5_REALTIME_AUTOTUNING_MODE_SDO_SUB_INDEX   0
#define G5_REALTIME_AUTOTUNING_MODE_SDO_VALUE       1

//0x3003:0 int16 realtime auto-tuning rigidity - 0 low 31 high (default 13)
#define G5_REALTIME_AUTOTUNING_RIGIDITY_SDO_INDEX       0x03003
#define G5_REALTIME_AUTOTUNING_RIGIDITY_SDO_SUB_INDEX   0
#define G5_REALTIME_AUTOTUNING_RIGIDITY_SDO_VALUE       13

//0x3004:0 int16 inertia ratio - (default 250)
#define G5_REALTIME_AUTOTUNING_INERTIA_SDO_INDEX       0x3004
#define G5_REALTIME_AUTOTUNING_INERTIA_SDO_SUB_INDEX   0
#define G5_REALTIME_AUTOTUNING_INERTIA_SDO_VALUE       13


//6091 hex 01 hex Motor revolutions Set the numerator of the electronic gear

#define G5_MOTOR_REVOLUTIONS_SDO_INDEX       0x6091
#define G5_MOTOR_REVOLUTIONS_SDO_SUB_INDEX   1
#define G5_MOTOR_REVOLUTIONS_SDO_VALUE       10000000

//0x6091:02 Shaft revolutions Set the denominator of the electronic gear
#define G5_SHAFT_REVOLUTIONS_SDO_INDEX       0x6091
#define G5_SHAFT_REVOLUTIONS_SDO_SUB_INDEX   2
#define G5_SHAFT_REVOLUTIONS_SDO_VALUE       1

//0x605E:0 int16 - default -1 - range is -7-to-0
#define G5_QUICKSTOP_OPTION_SDO_INDEX       0x605E
#define G5_QUICKSTOP_OPTION_SDO_SUB_INDEX   0
#define G5_QUICKSTOP_OPTION_SDO_VALUE       -1

//0x3514: int16 - Overrun Limit Setting - range = 0-1000 - unit 0.1 revolutions - default 10

#define G5_OVERRUN_LIMIT_SDO_INDEX       0x3514
#define G5_OVERRUN_LIMIT_SDO_SUB_INDEX   2
#define G5_OVERRUN_LIMIT_SDO_VALUE       1

//VIP: this is only for fully closed loop mode!



//20- bit encoder (1,048,576 pulses per rotation)



gberror_t ec_pdo_map_g5(uint16_t slave);
gberror_t ec_initial_pdo_g5(uint16_t slave);
int8_t ec_get_moo_sdo_g5(uint16_t drive);
bool ec_get_remote_g5(uint16_t drive);
bool ec_get_follow_error_g5(uint16_t drive);
uint8_t *ec_get_error_string_pdo_g5(uint16_t drive);
int32_t ec_get_actpos_wrd_g5(uint16_t drive);
gberror_t ec_set_ctrl_wrd_g5(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_g5(uint16_t drive);
gberror_t ec_set_setpos_wrd_g5(uint16_t drive, int32_t setpos);
gberror_t ec_standard_sdos_g5(uint16_t slave);
gberror_t ec_print_params_g5(uint16_t drive);


#define OMRON_MAN 0x83
#define G5_EEP_NAME "R88D-KN01H-ECT"
#define G5_EEP_MAN OMRON_MAN
#define G5_EEP_REV 0x020001
#define G5_EEP_ID 0x5


#endif //GBEM_G5_H
