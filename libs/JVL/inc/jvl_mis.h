/**
 ******************************************************************************
 * @file           :  jvl_mis.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_JVL_MIS_H
#define GB_MONOREPO_JVL_MIS_H

#include "map.h"
#include "gberror.h"


map_PDO_configuration_object_t map_txpdo_object_jvl_mis;
map_PDO_configuration_object_t map_rxpdo_object_jvl_mis;

uint32_t map_rxpdo_contents_jvl_mis[ECM_MAX_PDO_MAPPING_ENTRIES];
uint32_t map_txpdo_contents_jvl_mis[ECM_MAX_PDO_MAPPING_ENTRIES];


#define JVL_MAN 0x2BE
#define JVL_MIS_EEP_NAME "MISxxx with CiA402, DC"
#define JVL_MIS_EEP_MAN JVL_MAN
#define JVL_MIS_EEP_REV 0x1d
#define JVL_MIS_EEP_ID 0x201

//SM2
#define JVL_MIS_CONTROLWORD_PDO_INDEX           0
#define JVL_MIS_MOOSET_PDO_INDEX                2
#define JVL_MIS_SETPOS_PDO_INDEX                3

//SM3
#define JVL_MIS_STATUSWORD_PDO_INDEX            0
#define JVL_MIS_MOODISP_PDO_INDEX               2
#define JVL_MIS_ACTPOS_PDO_INDEX                3
#define JVL_MIS_ACTVEL_PDO_INDEX                7
#define JVL_MIS_ACTACC_PDO_INDEX                11
#define JVL_MIS_ACTFOLLOWWERROR_PDO_INDEX       13
#define JVL_MIS_ERROR_CODE_PDO_INDEX            17


//SDOs

//position positive limit 0x607d:2 int32
#define JVL_MIS_POS_POSLIMIT_SDO_INDEX          0x607d
#define JVL_MIS_POS_POSLIMIT_SDO_SUB_INDEX      2

//position negative limit 0x607d:1 int32
#define JVL_MIS_NEG_POSLIMIT_SDO_INDEX          0x607d
#define JVL_MIS_NEG_POSLIMIT_SDO_SUB_INDEX      1

//polarity (direction) 0x607e:0 uint8
#define JVL_MIS_DIRECTION_SDO_INDEX             0x607e
#define JVL_MIS_DIRECTION_SDO_SUB_INDEX     0

//uint32
#define JVL_MIS_GEAR_MOTOR_REV_SDO_INDEX        0x6091
#define JVL_MIS_GEAR_MOTOR_REV_SDO_SUB_INDEX    1

//uint32
#define JVL_MIS_GEAR_SHAFT_REV_SDO_INDEX        0x6091
#define JVL_MIS_GEAR_SHAFT_REV_SDO_SUB_INDEX    2

//uint32
#define JVL_MIS_FEED_FEED_SDO_INDEX             0x6092
#define JVL_MIS_FEED_FEED_SDO_SUB_INDEX         1

//uint32
#define JVL_MIS_FEED_SHAFT_REV_SDO_INDEX        0x6092
#define JVL_MIS_FEED_SHAFT_REV_SDO_SUB_INDEX    2


//quickstop option 0x605a:0 int16
#define JVL_MIS_QUICKSTOP_OPTION_CODE_SDO_INDEX 0x605a
#define JVL_MIS_QUICKSTOP_OPTION_CODE_SDO_SUB_INDEX 0

/* We have a motor with a 3.5:1 gear box connected to a belt drive. The diameter of the drive wheel is 12.4 cm. We want the unit of position to be in millimetres.
The circumference of the drive wheel is 389.56mm (124mm*pi). The parameters should be set as follows:
 0x6091 sub index 1 Gear ratio / Motor revolutions 35
0x6091 sub index 2 Gear ratio / Shaft revolutions 10
0x6092 sub index 1 Feed constant / Feed 38956
0x6092 sub index 2 Feed constant / Shaft revolutions 100
 */


typedef struct {
    uint16_t error_id;
    char text_string[100];
} jvl_mis_error_string_t;

gberror_t ec_pdo_map_jvl_mis(uint16_t slave);
gberror_t ec_initial_pdo_jvl_mis(uint16_t slave);
int8_t ec_get_moo_pdo_jvl_mis(uint16_t drive);
bool ec_get_remote_jvl_mis(uint16_t drive);
bool ec_get_follow_error_jvl_mis(uint16_t drive);
uint8_t *ec_get_error_string_pdo_jvl_mis(uint16_t drive);
int32_t ec_get_actpos_wrd_jvl_mis(uint16_t drive);
gberror_t ec_set_ctrl_wrd_jvl_mis(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_jvl_mis(uint16_t drive);
gberror_t ec_set_setpos_wrd_jvl_mis(uint16_t drive, int32_t setpos);
gberror_t ec_standard_sdos_jvl_mis(uint16_t slave);
gberror_t ec_print_params_jvl_mis(uint16_t drive);

#endif //GB_MONOREPO_JVL_MIS_H
