/**
 ******************************************************************************
 * @file           :  machine_smc3_test.h
 * @brief          :  machine map for SMC3 drive test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_SMC3_TEST_H
#define GBEM_SMC3_TEST_H


#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U
#define MAP_CYCLE_TIME              (4)


#define MAP_SMC3_1                    1


#define MAP_NUM_DRIVES              1
#define MAP_NUM_SLAVES              1


/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1
/** Number of DINS used to trigger EStop */
#define MAP_NUMBER_ESTOP_DIN        0


#endif //GBEM_SMC3_TEST_H
