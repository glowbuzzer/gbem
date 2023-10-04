/**
 ******************************************************************************
 * @file           :  machine_EL7041_test.h
 * @brief          :  machine map for EL7041 test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL7041_TEST_H
#define GBEM_EL7041_TEST_H




#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U
#define MAP_CYCLE_TIME              (4)


#define MAP_EK1100_1                1
#define MAP_EL7041_1                2


#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              2

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1





#endif //GBEM_EL7041_TEST_H
