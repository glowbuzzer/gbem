/**
 ******************************************************************************
 * @file           :  machine_starter_kit_2.h
 * @brief          :  machine map for starter kit 2 (AZD3A-KED plus Beckhoff IO)
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_MACHINE_STARTER_KIT_2_H
#define GBEM_MACHINE_STARTER_KIT_2_H


#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U
#define MAP_CYCLE_TIME              (4)

#define MAP_EK1100_1 	            1
#define MAP_EL2008_1 	            2
#define MAP_EL1008_1 	            3
#define MAP_AZD_CTRL_1              4


#define MAP_NUM_DRIVES              2
#define MAP_NUM_SLAVES              4


#define USE_ESTOP_RESET             0

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN              2

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN        1







//1 rotation = 10,000 ppr
//x= 133.3333 pulses per mm
//y= 266.6666 pulses per mm






#endif //GBEM_MACHINE_STARTER_KIT_2_H
