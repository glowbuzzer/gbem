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

gberror_t ec_apply_standard_sdos_bbh_scu_1_ec(uint16_t slave);

gberror_t ec_custom_fmmu_sm_bbh_scu_1_ec(uint16_t slave);

gberror_t ec_pdo_map_bbh_scu_1_ec(uint16_t slave);

gberror_t ec_fsoe_get_master_state_bbh_scu_1_ec(uint16_t slave, uint32_t *state,
                                                enum FSOE_MASTER_HIGH_LEVEL_STATE *high_level_state,
                                                uint32_t *error_code);

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

#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BASE                         20 //this varies with slots
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG     7
//there are 34 bytes of functional outputs available
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BASE + BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 1)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 2)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_3  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 3)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_4  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 4)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_5  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 5)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_6  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 6)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_7  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 7)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_8  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 8)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_9  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 9)


#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE                         64 //this varies with slots
//there are 18 bytes of functional inputs available
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 1)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_2  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 2)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_3  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 3)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_4  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 4)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_5  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 5)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_6  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 6)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_7  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 7)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_8  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 8)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_9  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE + 9)


#endif //GBEM_BBH_SCU_1_EC_H

