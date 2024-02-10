/**
 ******************************************************************************
 * @file           :  machine_aw_j17.h
 * @brief          :  machine map for aw j17 single joint with EK1100 + El1008 + EL2008
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_MACHINE_AW_J17_H
#define GBEM_MACHINE_AW_J17_H


#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */
#define MAP_EK1100_1                1
#define MAP_EL2008_1                2
#define MAP_EL1008_1                3
#define MAP_EL6021_1                4
#define MAP_AW_J17_CTRL             5


#define ECM_CHECK_EEP_MAN           0
#define ECM_CHECK_EEP_REV           0
#define ECM_CHECK_EEP_ID            0


#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              5
#define MAP_NUM_FSOE_MASTER_SLOTS   0


/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING      0

#define MAP_NUMBER_ESTOP_DIN        1


#endif //GBEM_MACHINE_AW_J17_H
