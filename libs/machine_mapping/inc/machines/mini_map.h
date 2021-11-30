/**
 ******************************************************************************
 * @file           :  mini_map.h
 * @brief          :  key defines for MACHINE_MINI
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef MONOREPO_MINI_MAP_H
#define MONOREPO_MINI_MAP_H



#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */
#define MAP_EK1100_1                1
#define MAP_EL2008_1                2
#define MAP_EL1008_1                3
#define MAP_EL5101_1                4
#define MAP_AZD_CTRL_1              5

#define MAP_NUM_DRIVES              2
#define MAP_NUM_SLAVES              5

/** Use a digital input to reset the estop */
#define USE_ESTOP_RESET             1

///** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
//#define CTRL_ESTOP_DIN              1
//
///** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
//#define CTRL_ESTOP_RESET_DIN        2


//1 rotation = 10,000 ppr
//x= 133.3333 pulses per mm
//y= 266.6666 pulses per mm
//azd limits = (−2147483648) (+2147483648)

//unint8 direction of rotation
//0: Positive side=Counterclockwise direction
//1: Positive side=Clockwise direction
//default value is 1

#endif //MONOREPO_MINI_MAP_H
