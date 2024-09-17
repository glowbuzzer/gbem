/**
 ******************************************************************************
 * @file           :  machine_aw_6dof_fsoe.h
 * @brief          :  machine map for aw tube robot 6DOF with FSoE
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_MACHINE_AW_6DOF_FSOE_H
#define GBEM_MACHINE_AW_6DOF_FSOE_H

#include "stdbool.h"

#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */
#define MAP_BBH_SCU_1_EC_1          1
#define MAP_EK1100_1                2
#define MAP_EL2808_1                3
#define MAP_EL1808_1                4
#define MAP_EL6021_1                5
#define MAP_AW_J40LP_CTRL_1         6
#define MAP_AW_J40HP_CTRL_1         7
#define MAP_AW_J32_CTRL_1           8
#define MAP_AW_J25_CTRL_1           9
#define MAP_AW_J25_CTRL_2           10
#define MAP_AW_J20_CTRL_1           11


#define ECM_CHECK_EEP_MAN           0
#define ECM_CHECK_EEP_REV           0
#define ECM_CHECK_EEP_ID            0


#define MAP_NUM_DRIVES              6
#define MAP_NUM_SLAVES              14
#define MAP_NUM_FSOE_MASTER_SLOTS   6

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING      1


#define MAP_NUMBER_ESTOP_DIN        0


#endif //GBEM_MACHINE_AW_6DOF_FSOE_H
