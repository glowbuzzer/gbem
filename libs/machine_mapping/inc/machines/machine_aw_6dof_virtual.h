/**
 ******************************************************************************
 * @file           :  machine_aw_6dof_vritual.h
 * @brief          :  machine map for aw tube robot 6DOF with J3 virtual - J32
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_MACHINE_AW_6DOF_VIRTUAL_H
#define GBEM_MACHINE_AW_6DOF_VIRTUAL_H

#include "stdbool.h"


#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */

#define MAP_AW_J32_CTRL_1           1


#define ECM_CHECK_EEP_MAN           0
#define ECM_CHECK_EEP_REV           0
#define ECM_CHECK_EEP_ID            0


#define MAP_NUM_DRIVES              6
#define MAP_NUM_SLAVES              1
#define MAP_NUM_FSOE_MASTER_SLOTS   0

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING      1


#define MAP_NUMBER_ESTOP_DIN        0


#endif //GBEM_MACHINE_AW_6DOF_VIRTUAL_H
