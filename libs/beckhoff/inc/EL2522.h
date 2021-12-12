/**
 ******************************************************************************
 * @file           :  EL2522.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL2522_H
#define GBEM_EL2522_H

#include "beckhoff_shared.h"
#include "map.h"
#include "gberror.h"


//SM2
#define EL2522_SETPOS_CH1_PDO_INDEX                2
#define EL2522_SETPOS_CH2_PDO_INDEX                8

#define EL2522_CTRLWRD_CH1_PDO_INDEX               0
#define EL2522_CTRLWRD_CH2_PDO_INDEX               6


//SM3
#define EL2522_STATUSWORD_CH1_PDO_INDEX            1
#define EL2522_ACTPOS_CH1_PDO_INDEX                2

#define EL2522_STATUSWORD_CH2_PDO_INDEX            7
#define EL2522_ACTPOS_CH2_PDO_INDEX                8


map_SM_assignment_object_t map_SM2_el2522;
map_SM_assignment_object_t map_SM3_el2522;
uint16_t map_SM2_index_of_assigned_PDO_el2522[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_el2522[ECM_MAX_PDO_MAPPING_ENTRIES];


gberror_t ec_pdo_map_el2522(uint16_t slave);
int8_t ec_get_moo_pdo_el2522(uint16_t drive);
bool ec_get_remote_el2522(uint16_t drive);
bool ec_get_follow_error_el2522(uint16_t drive);
uint8_t *ec_get_error_string_sdo_el2522(uint16_t drive);
int32_t ec_get_actpos_wrd_el2522(uint16_t drive);
gberror_t ec_set_ctrl_wrd_el2522(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_el2522(uint16_t drive);
gberror_t ec_set_setpos_wrd_el2522(uint16_t drive, int32_t setpos);
gberror_t ec_standard_sdos_el2522(uint16_t slave);
gberror_t ec_print_params_el2522(uint16_t drive);
gberror_t ec_initial_pdo_el2522(uint16_t slave);


#define EL2522_EEP_NAME "EL2522"
#define EL2522_EEP_MAN BECKHOFF_MAN
#define EL2522_EEP_REV 0x00160000
#define EL2522_EEP_ID 09DA3052



//CH1

//0x8000:12 uint32 - Base frequency 1 (default = 50000 Hz = 0x0000C350

//0x8000:13 - uint32 - Base frequency 2 (default = 100000 Hz = 0x000186A0)

//0x8000:0E - BIT2/unit8 - Operating mode permitted values - 1 pulse dir
#define EL2522_OPERATION_MODE_CH1_SDO_INDEX                 0x8000
#define EL2522_OPERATION_MODE_CH1_SDO_SUB_INDEX             0x0e
#define EL2522_OPERATION_MODE_CH1_SDO_VALUE                 1



//0x8000:10 - BOOL - Negative logic TRUE Negative logic FALSE Positive logic

//0x7000:0 - uint8 - automatic dir

#define EL2522_AUTO_DIR_CH1_SDO_INDEX                 0x7000
#define EL2522_AUTO_DIR_CH1_SDO_SUB_INDEX                 0x4
#define EL2522_AUTO_DIR_CH1_VALUE                 true



//CH2
//0x8010:12 uint32 - Base frequency 1 (default = 50000 Hz = 0x0000C350

//0x8010:13 - uint32 - Base frequency 2 (default = 100000 Hz = 0x000186A0)

//0x8010:0E - BIT2/unit8 - Operating mode permitted values - 1 pulse dir
#define EL2522_OPERATION_MODE_CH2_SDO_INDEX                 0x8010
#define EL2522_OPERATION_MODE_CH2_SDO_SUB_INDEX             0x0e
#define EL2522_OPERATION_MODE_CH2_SDO_VALUE                 1


//0x8010:10 - BOOL - Negative logic TRUE Negative logic FALSE Positive logic




//status word bit numbers
#define EL2522_STATUS_SYNC_ERROR_BIT_NUM            5



#endif //GBEM_EL2522_H
