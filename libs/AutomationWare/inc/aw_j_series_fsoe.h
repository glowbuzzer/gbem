/**
 ******************************************************************************
 * @file           :  aw_j_series_fsoe
 * @brief          :  AW J Series control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef AW_J_SERIES_FSOE_H
#define AW_J_SERIES_FSOE_H
#include "std_headers.h"
#include "ecm_status.h"
#include "gberror.h"

//these work with standard pdo map
// #define AW_J_SERIES_EC_FSOE_SM2_OFFSET   35
// #define AW_J_SERIES_EC_FSOE_SM3_OFFSET   43

#define AW_J_SERIES_EC_FSOE_SM2_FSOE_SIZE   12
#define AW_J_SERIES_EC_FSOE_SM3_FSOE_SIZE   32


//these are for custom pdo map
#define AW_J_SERIES_EC_FSOE_SM2_OFFSET   39
#define AW_J_SERIES_EC_FSOE_SM3_OFFSET   47


gberror_t ec_set_slots_aw_j_series_fsoe(const uint16_t slave);


//FSoE

#define AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX 0xf030
#define AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID 67239937
#define AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_2_ID 584187905

gberror_t ec_custom_fmmu_sm_aw_j_series(const uint16_t slave);

bool ec_get_safety_state_aw_j_series_fsoe(void);

gberror_t ec_fsoe_get_slave_state_aw_j_series(uint16_t slave, uint32_t *state,
                                              fsoe_slave_high_level_state_t *high_level_state);

gberror_t ec_fsoe_get_slave_con_id_aw_j_series(uint16_t slave, uint16_t *con_id);

gberror_t ec_pdo_map_aw_j_series_fsoe(uint16_t slave);

gberror_t ec_apply_standard_sdos_aw_j_series_fsoe(uint16_t slave);

#endif //AW_J_SERIES_FSOE_H
