/**
 ******************************************************************************
 * @file           :  machine_virtual.h
 * @brief          :  machine map for EK1100 + El1008 + EL2008 + a number of virtual drives
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_MACHINE_J17_PLUS_VIRTUAL_H
#define GBEM_MACHINE_J17_PLUS_VIRTUAL_H


#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */
#define MAP_EK1100_1                1
#define MAP_EL2008_1                2
#define MAP_EL1008_1                3
#define MAP_AW_J17_CTRL             4

#define MAP_NUM_DRIVES              6
#define MAP_NUM_SLAVES              4

/** Use a digital input to reset the estop */
#define USE_ESTOP_RESET             1




/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN              1

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN        2


#endif //GBEM_MACHINE_J17_PLUS_VIRTUAL_H
