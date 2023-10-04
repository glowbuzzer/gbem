/**
 ******************************************************************************
 * @file           :  machine_drive_spinners.h
 * @brief          :  machine map for drive spinners
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_MACHINE_DRIVE_SPINNERS_H
#define GBEM_MACHINE_DRIVE_SPINNERS_H


#define MAP_CYCLE_TIME              (1)
#define MAP_GLOBAL_DC_SYNC_OFFSET   20000U


#define MAP_EK1100_1                1
#define MAP_EL2008_1                2
#define MAP_EL1008_1                3
#define MAP_AZD_CTRL_1                4

#define MAP_NUM_SLAVES              4
#define MAP_NUM_DRIVES              2

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1

/** Number of DINS used to trigger EStop */
#define MAP_NUMBER_ESTOP_DIN        0




//1 rotation = 10,000 ppr
//x= 133.3333 pulses per mm
//y= 266.6666 pulses per mm

//unint8
//0: Positive side=Counterclockwise direction
//1: Positive side=Clockwise direction
//default value is 1



#endif //GBEM_MACHINE_DRIVE_SPINNERS_H