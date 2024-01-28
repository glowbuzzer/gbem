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


gberror_t ec_set_slots_aw_j_series_fsoe(const uint16_t slave);

gberror_t ec_custom_fmmu_sm_aw_j_series(const uint16_t slave);

bool ec_get_safety_state_aw_j_series_fsoe(void);

gberror_t ec_fsoe_get_slave_state_aw_j_series(uint16_t slave, uint32_t *state,
                                              enum FSOE_SLAVE_HIGH_LEVEL_STATE *high_level_state);

gberror_t ec_fsoe_get_slave_con_id_aw_j_series(uint16_t slave, uint16_t *con_id);

gberror_t ec_pdo_map_aw_j_series_fsoe(uint16_t slave);

gberror_t ec_apply_standard_sdos_aw_j_series_fsoe(uint16_t slave);


#define AW_J_SERIES_EC_FSOE_SM2_FSOE_SIZE   12
#define AW_J_SERIES_EC_FSOE_SM3_FSOE_SIZE   32

//these work with standard pdo map
// #define AW_J_SERIES_EC_FSOE_SM2_OFFSET   35
// #define AW_J_SERIES_EC_FSOE_SM3_OFFSET   43

//these are for custom pdo map
#define AW_J_SERIES_EC_FSOE_SM2_OFFSET   39
#define AW_J_SERIES_EC_FSOE_SM3_OFFSET   47


//FSoE

#define AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX 0xf030
#define AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID 67239937
#define AW_J_SERIES_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_2_ID 584187905


//standard
// #define AW_J_SERIES_FSOE_BASE_SM3_OFFSET                    43 //changes with PDO map
// #define AW_J_SERIES_FSOE_BASE_SM3_OFFSET                    (AW_J_SERIES_EC_FSOE_SM3_OFFSET) //changes with PDO map


//custom
// #define AW_J_SERIES_FSOE_BASE_SM3_OFFSET                    49 //changes with PDO map

#define AW_J_SERIES_FSOE_STATUS_WORD_0_PDO_INDEX            (AW_J_SERIES_EC_FSOE_SM3_OFFSET + 1)
#define AW_J_SERIES_FSOE_STATUS_WORD_1_PDO_INDEX            (AW_J_SERIES_EC_FSOE_SM3_OFFSET + 5)
#define AW_J_SERIES_FSOE_CONNECTION_ID_PDO_INDEX            (AW_J_SERIES_EC_FSOE_SM3_OFFSET + 29)

//standard
// #define AW_J_SERIES_FSOE_BASE_SM2_OFFSET                    35 //changes with PDO map
// #define AW_J_SERIES_FSOE_BASE_SM2_OFFSET                    (AW_J_SERIES_EC_FSOE_SM2_OFFSET) //changes with PDO map
//custom
// #define AW_J_SERIES_FSOE_BASE_SM2_OFFSET                    35 //changes with PDO map

#define AW_J_SERIES_FSOE_STATUS_ERROR_BIT_NO                7
#define AW_J_SERIES_FSOE_STATUS_RESTART_ACK_REQ_BIT_NO      15


#endif //AW_J_SERIES_FSOE_H
