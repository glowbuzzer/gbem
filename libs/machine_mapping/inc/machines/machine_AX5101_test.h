/**
 ******************************************************************************
 * @file           :  machine_AX5101_test.h
 * @brief          :  machine map for AX5101 test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_AX5101_TEST_H
#define GBEM_AX5101_TEST_H



#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U
#define MAP_CYCLE_TIME              (4)

#define MAP_AX5101_1 	            1


#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              1

/** Use a digital input to reset the estop */
#define USE_ESTOP_RESET             0



/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN              1

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN        2


#endif //GBEM_AX5101_TEST_H
