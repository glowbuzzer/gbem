/**
 ******************************************************************************
 * @file           :  machine_test.h
 * @brief          :  machine map for test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_MINI_MAP_H
#define GBEM_MINI_MAP_H


//#define MAP_CYCLE_TIME              (4)
#define MAP_GLOBAL_DC_SYNC_OFFSET   2000U

/** This is the total number of drives on the EtherCAT network */
#define MAP_NUM_DRIVES              4

/** This is the total number of slaves that are on the EtherCAT network*/
#define MAP_NUM_SLAVES              9



#define MAP_EK1100_1                1
#define MAP_EL2008_1                2
#define MAP_EL1008_1                3
#define MAP_EL5101_1                4
#define MAP_AZD_CTRL_1              5
#define MAP_ELTESTI_1               6
#define MAP_ELTESTO_1               7
#define MAP_AZD_CTRL_2              8
#define MAP_AKD_1                   9







#endif //GBEM_MINI_MAP_H
