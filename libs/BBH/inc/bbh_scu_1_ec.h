/**
 ******************************************************************************
 * @file           :  bbh_scu_1_ec
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_BBH_SCU_1_EC_H
#define GBEM_BBH_SCU_1_EC_H

#include "std_headers.h"
#include "map_config_custom_pdo.h"
#include "std_defs_and_macros.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"
#include "bbh.h"
#include "ecm_status.h"

gberror_t ec_apply_standard_sdos_bbh_scu_1_ec(const uint16_t slave);

gberror_t ec_custom_fmmu_sm_bbh_scu_1_ec(const uint16_t slave);

gberror_t ec_pdo_map_bbh_scu_1_ec(const uint16_t slave);

gberror_t ec_fsoe_get_master_state_bbh_scu_1_ec(uint16_t slave, uint32_t *state,
                                                fsoe_slave_high_level_state_t *high_level_state, uint32_t *error_code);

gberror_t ec_fsoe_get_master_con_id_bbh_scu_1_ec(uint16_t slave, uint16_t *con_id);


#define BBH_SCU_1_EC_EEP_NAME "SCU-1-EC"
#define BBH_SCU_1_EC_EEP_ID 0x00000006
#define BBH_SCU_1_EC_EEP_MAN BBH_MAN
#define BBH_SCU_1_EC_EEP_REV 0x00000001


//SCU_1_EC_

#define BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX         0xf030
#define BBH_SCU_1_EC_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID  0x6212 //14 in and 4 out
#define BBH_SCU_1_EC_SLOT_33_SCU_AS_FSOE_SLAVE_CONNECTION           0x6220
#define BBH_SCU_1_EC_SLOT_34_MASTER_TO_MASTER                       0x6194
#define BBH_SCU_1_EC_SLOT_35_FUNCTIONAL_IN_OUT                      0x6195


#endif //GBEM_BBH_SCU_1_EC_H
