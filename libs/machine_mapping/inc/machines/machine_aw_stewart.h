/**
 ******************************************************************************
 * @file           :  machine_aw_stewart.h
 * @brief          :  machine map for aw stewart platform
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_MACHINE_AW_STEWART_H
#define GBEM_MACHINE_AW_STEWART_H


#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */
#define MAP_P366_1                  1
#define MAP_P372_1                  2
#define MAP_MSE_408_1               3
#define MAP_MSE_408_2               4
#define MAP_MSE_408_3               5
#define MAP_P366_2                  6
#define MAP_MSE_408_4               7
#define MAP_MSE_408_5               8
#define MAP_MSE_408_6               9


#define ECM_CHECK_EEP_MAN           0
#define ECM_CHECK_EEP_REV           0
#define ECM_CHECK_EEP_ID            0


#define MAP_NUM_DRIVES              6
#define MAP_NUM_SLAVES              9

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          0

/** Use a digital input to reset the estop */
#define USE_ESTOP_RESET             1




/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN              1

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN        2


#endif //GBEM_MACHINE_AW_STEWART_H
