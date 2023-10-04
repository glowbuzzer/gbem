/**
 ******************************************************************************
 * @file           :  machine_cartesian.h
 * @brief          :  machine map for cartesian
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_MACHINE_CARTESIAN_H
#define GBEM_MACHINE_CARTESIAN_H


#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U
#define MAP_CYCLE_TIME              (4)

#define MAP_EK1100_1                1
#define MAP_EL2008_1                2
#define MAP_EL1008_1                3
#define MAP_EL2622_1                4
#define MAP_EL5101_1                5
#define MAP_AZD_CTRL_1              6


#define MAP_NUM_DRIVES              2
#define MAP_NUM_SLAVES              6

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1

/** Number of DINS used to trigger EStop */
#define MAP_NUMBER_ESTOP_DIN        0










//1 rotation = 10,000 ppr
//x= 133.3333 pulses per mm
//y= 266.6666 pulses per mm






#endif //GBEM_MACHINE_CARTESIAN_H
