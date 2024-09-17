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

#define BBH_SCU_1_EC_FSOE_IN_OFFSET     0
#define BBH_SCU_1_EC_FSOE_OUT_OFFSET    0

#define BBH_SCU_1_EC_DEBUG_0_PDO_OFFSET                 2
#define BBH_SCU_1_EC_DEBUG_1_PDO_OFFSET                 3
#define BBH_SCU_1_EC_DEBUG_2_PDO_OFFSET                 4
#define BBH_SCU_1_EC_DEBUG_3_PDO_OFFSET                 5
#define BBH_SCU_1_EC_DEBUG_4_PDO_OFFSET                 6

#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_BASE_OFFSET     8
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_BASE_OFFSET      32

#define BBH_SCU_1_MASTER_MASTER_OUTPUT_MSG_SIZE 8
#define BBH_SCU_1_MASTER_MASTER_INPUT_MSG_SIZE 32

#define BBH_SCU_1_EC_FUNCTIONAL_OUPUTS_OVERALL_SIZE 34
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OVERALL_SIZE 18


gberror_t ec_fsoe_set_error_ack_state_bbh_scu_1_ec(bool state, uint16_t slave);

gberror_t ec_apply_standard_sdos_bbh_scu_1_ec(uint16_t slave);

gberror_t ec_custom_fmmu_sm_bbh_scu_1_ec(uint16_t slave);

gberror_t ec_pdo_map_bbh_scu_1_ec(uint16_t slave);

gberror_t ec_fsoe_get_master_state_bbh_scu_1_ec(uint16_t slave,
                                                enum FSOE_MASTER_HIGH_LEVEL_STATE *master_high_level_state,
                                                uint32_t *error_code);

gberror_t ec_fsoe_get_master_con_id_bbh_scu_1_ec(uint16_t slave, uint16_t *con_id);


gberror_t ec_compute_functional_offset_bbh_scu_1_ec(void);


bool ec_fsoe_get_safety_state_bbh_scu_1_ec(uint16_t slave);

#define BBH_SCU_1_EC_EEP_NAME "SCU-1-EC"
#define BBH_SCU_1_EC_EEP_ID 0x00000006
#define BBH_SCU_1_EC_EEP_MAN BBH_MAN
#define BBH_SCU_1_EC_EEP_REV 0x00000001


//SCU_1_EC_

extern int bbh_scu_1_ec_functional_outputs_offset_base_dynamic;
extern int bbh_scu_1_ec_functional_inputs_offset_base_dynamic;


#define BBH_SCU_1_EC_CONFIGURED_MODULE_IDENT_LIST_SDO_INDEX         0xf030
#define BBH_SCU_1_EC_FSOE_CONFIGURED_MODULE_IDENT_LIST_MODULE_1_ID  0x6212 //14 in and 4 out - for synapticon
#define BBH_SCU_1_EC_SLOT_33_SCU_AS_FSOE_SLAVE_CONNECTION           0x6220
#define BBH_SCU_1_EC_SLOT_34_MASTER_TO_MASTER                       0x6194
#define BBH_SCU_1_EC_SLOT_35_FUNCTIONAL_IN_OUT                      0x6195

#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BASE                         20 //this varies with slots
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_SIZE_OF_STATUS_AND_DEBUG     7

#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_MAGIC_NUM 100
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_MAGIC_NUM 200

//there are 34 bytes of functional outputs available
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0  BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_MAGIC_NUM
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 1)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 2)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_3  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 3)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_4  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 4)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_5  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 5)
#define BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_6  (BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0 + 6)


#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BASE                         64 //this varies with slots
//there are 18 bytes of functional inputs available
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0    BBH_SCU_1_EC_FUNCTIONAL_INPUTS_MAGIC_NUM
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0 + 1)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_2  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0 + 2)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_3  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0 + 3)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_4  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0 + 4)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_5  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0 + 5)
#define BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_6  (BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0 + 6)


//extern int BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0;
//extern int BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1;
//extern int BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_2;
//extern int BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_3;
//extern int BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_4;
//extern int BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_5;
//extern int BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_6;
//
//extern int BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0;
//extern int BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1;
//extern int BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2;
//extern int BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_3;
//extern int BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_4;
//extern int BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_5;
//extern int BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_6;


#endif //GBEM_BBH_SCU_1_EC_H

