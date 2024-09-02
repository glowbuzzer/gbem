/**
 ******************************************************************************
 * @file           :  machine_bbh_j25.h
 * @brief          :  BBH SCU-1-EC + AW J25 FSoE safety PLC program
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_MACHINE_BBH_J25_H
#define GBEM_MACHINE_BBH_J25_H


//#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */
#define MAP_BBH_SCU_1_EC_1          1
#define MAP_EK1100_1                2
#define MAP_EL6021_1                3
#define MAP_EL2535_0005_1           4
#define MAP_ELM7231_9818_1          5
#define MAP_EK1110_1                6
#define MAP_AW_J25_FSOE_1           7

#define ECM_CHECK_EEP_MAN           0
#define ECM_CHECK_EEP_REV           0
#define ECM_CHECK_EEP_ID            0

#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              7
#define MAP_NUM_FSOE_MASTER_SLOTS   1


/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING      0


#define MAP_NUMBER_ESTOP_DIN        0


#endif //GBEM_MACHINE_BBH_J25_H
