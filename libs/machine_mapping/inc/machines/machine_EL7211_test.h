/**
 ******************************************************************************
 * @file           :  EL7211_test.h
 * @brief          :  machine map for EL7211 test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL7211_TEST_H
#define GBEM_EL7211_TEST_H


#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U
//#define MAP_CYCLE_TIME              (4)

#define MAP_EK1100_1                1
#define MAP_EL7211_1                2
#define MAP_EL2008_1                3
#define MAP_EL1008_1                4


#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              4

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1

/** Number of DINS used to trigger EStop */
#define MAP_NUMBER_ESTOP_DIN        0


#endif //GBEM_EL7211_TEST_H
