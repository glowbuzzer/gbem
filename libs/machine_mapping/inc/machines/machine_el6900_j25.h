/**
 ******************************************************************************
 * @file           :  machine_el6900_j25.h
 * @brief          :  EL6900+EL1904+EL2904 with AW J25 safety PLC program
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_MACHINE_EL6900_J25_H
#define GBEM_MACHINE_EL6900_J25_H


#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */
#define MAP_EK1100_1                1
#define MAP_EL6900_1                2
#define MAP_EL1904_1                3
#define MAP_EL2904_1                4
#define MAP_AW_J25_FSOE_CTRL        5


#define ECM_CHECK_EEP_MAN           0
#define ECM_CHECK_EEP_REV           0
#define ECM_CHECK_EEP_ID            0


#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              5

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING      1


#define MAP_NUMBER_ESTOP_DIN        0


#endif //GBEM_MACHINE_EL6900_J25_H
