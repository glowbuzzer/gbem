/**
 ******************************************************************************
 * @file           :  single_akd.h
 * @brief          :  machine map for single AKD drive
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_MACHINE_SINGLE_AKD_H
#define GBEM_MACHINE_SINGLE_AKD_H


#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

#define MAP_AKD_1 	                1

#define MAP_NUM_SLAVES              1
#define MAP_NUM_DRIVES              1

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1

#define USE_ESTOP_RESET             0

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN              1

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN        2


#endif //GBEM_MACHINE_SINGLE_AKD_H
