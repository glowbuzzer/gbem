/**
 ******************************************************************************
 * @file           :  smc3.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_SMC3_H
#define GBEM_SMC3_H

#include "map.h"
#include "gberror.h"

/* PDO MAPPING */
//SM2 - 0x1c12
//0x1602
#define SMC3_CONTROLWORD_PDO_INDEX           0
#define SMC3_SETPOS_PDO_INDEX                2
#define SMC3_MOOSET_PDO_INDEX                6



//SM3 - 0x1c13
//0x1a02
#define SMC3_STATUSWORD_PDO_INDEX            0
#define SMC3_ACTPOS_PDO_INDEX                2
#define SMC3_MOOGET_PDO_INDEX                6


//0x60c2 - interpolation



//0x2023:0 uint32 - Stall Guard Turns - value = 0x2e8 (1000)



//0x2022:0 uint32 - Stall Guard Offset - value = 0


//0x2000:0 uint32 - KvFactor - value = 0x1E (30)


//0x8010:7 uint32  - Motor steps per rev - value = 0xc8 (200)


//0x8010:8 uint32 - Encoder increment per rev - value = 0


//0x8010:10 uint32 - Steps - value = 0x100 (256)


//0x8010:11 uint32 - Desired current = 0xc8 (200) (2A)


//0x8010:12 uint32 - Value of current in standstill = 0x32 (50) (500mA)




gberror_t ec_pdo_map_smc3(uint16_t slave);
gberror_t ec_initial_pdo_smc3(uint16_t slave);
int8_t ec_get_moo_pdo_smc3(uint16_t drive);
bool ec_get_remote_smc3(uint16_t drive);
bool ec_get_follow_error_smc3(uint16_t drive);
uint8_t *ec_get_error_string_sdo_smc3(uint16_t drive);
int32_t ec_get_actpos_wrd_smc3(uint16_t drive);
gberror_t ec_set_ctrl_wrd_smc3(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_smc3(uint16_t drive);
gberror_t ec_set_setpos_wrd_smc3(uint16_t drive, int32_t setpos);
gberror_t ec_standard_sdos_smc3(uint16_t slave);
gberror_t ec_print_params_smc3(uint16_t drive);


#define CANNON_MAN 0x00000642
#define SMC3_EEP_NAME "CA-SMC3"
#define SMC3_EEP_MAN CANNON_MAN
#define SMC3_EEP_REV 0x0000014
#define SMC3_EEP_ID 0x70069900


#endif //GBEM_SMC3_H
