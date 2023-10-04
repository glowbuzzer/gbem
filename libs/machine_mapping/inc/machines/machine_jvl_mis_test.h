/**
 ******************************************************************************
 * @file           :  machine_jvl_mis_test.h
 * @brief          :  machine map for JVL MIS drive test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_JVL_MIS_TEST_H
#define GBEM_JVL_MIS_TEST_H




#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U
#define MAP_CYCLE_TIME              (4)


#define MAP_EK1100_1                1
#define MAP_EL2008_1                2
#define MAP_EL1008_1                3
#define MAP_EL5101_1                4
#define MAP_JVL_MIS_1               5



#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              5

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1






#endif //GBEM_JVL_MIS_TEST_H
