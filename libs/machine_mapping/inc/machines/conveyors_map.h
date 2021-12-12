/**
 ******************************************************************************
 * @file           :  conveyors_map.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_CONVEYORS_MAP_H
#define GBEM_CONVEYORS_MAP_H


#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

#define MAP_EK1100_1 	            1
#define MAP_EL2008_1 	            2
#define MAP_EL1008_1 	            3
#define MAP_EL2022_1 	            4
#define MAP_AKD_1 	                5
#define MAP_AKD_2 	                6


#define MAP_NUM_SLAVES              6
#define MAP_NUM_DRIVES              2


/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN              4

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN        2


/** Defines the maximum time the homing cycle can take before terminating */
#define DRIVE_HOMING_TIMEOUT_SECS   60






#endif //GBEM_CONVEYORS_MAP_H
