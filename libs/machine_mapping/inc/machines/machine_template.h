/**
 ******************************************************************************
 * @file           :  blank_map.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_BLANK_MAP_H
#define GBEM_BLANK_MAP_H



#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/* Slaves must be defined in order that they appear on the EtherCAT network */
#define MAP_SLAVETYPE1_1                1
#define MAP_SLAVETYPE2_1                2
#define MAP_SLAVETYPE3_1                3
#define MAP_SLAVETYPE4_1                4
#define MAP_DRIVETYPE1_1                5

#define MAP_NUM_DRIVES                  1
#define MAP_NUM_SLAVES                  5

/** Use a digital input to reset the estop */
#define USE_ESTOP_RESET                 0

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop */
#define CTRL_ESTOP_DIN                  1

/** Number of DIN (0-63 as defined in GBC layout ) that corresponds  to estop reset */
#define CTRL_ESTOP_RESET_DIN            2


#endif //GBEM_BLANK_MAP_H
