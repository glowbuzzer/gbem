/**
 ******************************************************************************
 * @file           :  single_akd.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef MONOREPO_SINGLE_AKD_H
#define MONOREPO_SINGLE_AKD_H


#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

#define MAP_AKD_1 	                1

#define MAP_NUM_SLAVES              1
#define MAP_NUM_DRIVES              1


/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN                1

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN        2


#endif //MONOREPO_SINGLE_AKD_H
