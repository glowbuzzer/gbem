/**
 ******************************************************************************
 * @file           :  EL2521.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL2521_H
#define GBEM_EL2521_H

#include "beckhoff_shared.h"
#include "map.h"
#include "gberror.h"

//EL2521 1Ch. Pulse Train Output
#define EL2521_EEP_NAME 		"EL2521"
#define EL2521_EEP_MAN 			BECKHOFF_MAN
#define EL2521_EEP_ID 			0x09D93052
#define EL2521_EEP_REV 			0x0014007C



//SM2
#define EL2521_CONTROLWORD_PDO_INDEX           0
#define EL2521_SETPOS_PDO_INDEX                2

//SM3
#define EL2521_STATUSWORD_PDO_INDEX            0
#define EL2521_ACTPOS_PDO_INDEX                2



//# Emergency ramp active (if the watchdog timer responds)
//# 0 = Disabled ramp (default)
//# 1 = Enabled ramp
//# Setting 0
//ecmcConfigOrDie "Cfg.EcAddSdo(${ECMC_EC_SLAVE_NUM},0x8000,0x2,0,1)"


//# Ramp active
//# The ramp controller should only be deactivated if a higher-level
//# controller (e.g. NC controller) takes over the control of travel dis-
//# tance and frequency.
//# 0 = Disabled ramp (default)
//# 1 = Enabled ramp
//# Setting 0 since ECMC controls ramps
//ecmcConfigOrDie "Cfg.EcAddSdo(${ECMC_EC_SLAVE_NUM},0x8000,0x6,0,1)"

//# Direct Input Mode
//# 0 = Relative input mode (default)
//# 1 = Direct input mode
//# Setting 1
//ecmcConfigOrDie "Cfg.EcAddSdo(${ECMC_EC_SLAVE_NUM},0x8000,0x8,1,1)"


//# Frequency factor (Digit x10 mHz) for direct input
//# 8001:06
//# Defualt setting 100
//ecmcConfigOrDie "Cfg.EcAddSdo(${ECMC_EC_SLAVE_NUM},0x8001,0x6,100,2)"


map_SM_assignment_object_t map_SM2_el2521;
map_SM_assignment_object_t map_SM3_el2521;
uint16_t map_SM2_index_of_assigned_PDO_el2521[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_el2521[ECM_MAX_PDO_MAPPING_ENTRIES];


gberror_t ec_pdo_map_el2521(uint16_t slave);
int8_t ec_get_moo_pdo_el2521(uint16_t drive);
bool ec_get_remote_el2521(uint16_t drive);
bool ec_get_follow_error_el2521(uint16_t drive);
uint8_t *ec_get_error_string_sdo_el2521(uint16_t drive);
int32_t ec_get_actpos_wrd_el2521(uint16_t drive);
gberror_t ec_set_ctrl_wrd_el2521(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_el2521(uint16_t drive);
gberror_t ec_set_setpos_wrd_el2521(uint16_t drive, int32_t setpos);
gberror_t ec_standard_sdos_el2521(uint16_t slave);
gberror_t ec_print_params_el2521(uint16_t drive);




#endif //GBEM_EL2521_H
