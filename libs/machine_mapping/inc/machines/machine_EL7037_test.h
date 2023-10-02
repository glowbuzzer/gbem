/**
 ******************************************************************************
 * @file           :  machine_EL7037_test.h
 * @brief          :  machine map for EL7037 drive test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL7037_TEST_H
#define GBEM_EL7037_TEST_H




#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U
#define MAP_CYCLE_TIME              (4)


#define MAP_EK1100_1                1
#define MAP_EL7037_1                2


#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              2

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1

/** Use a digital input to reset the estop */
#define USE_ESTOP_RESET             0

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN              2

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN        1




#endif //GBEM_EL7037_TEST_H
