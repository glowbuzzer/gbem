/**
 ******************************************************************************
 * @file           :  EL3001.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL3001_H
#define GBEM_EL3001_H
#include "beckhoff_shared.h"

//EL3001 1Ch. Ana. Input +/-10V
#define EL3001_EEP_NAME "EL3001"
#define EL3001_EEP_MAN BECKHOFF_MAN
#define EL3001_EEP_REV 0x00110000
#define EL3001_EEP_ID 0xbb93052

//int16
#define EL3001_VALUE_INDEX 2

//Underrange event active
#define EL3001_STATUS_UNDERRANGE_BIT_NUM 0
//        Overrange event active
#define EL3001_STATUS_OVERRANGE_BIT_NUM 1

//Value smaller/equal Limit1
#define EL3001_STATUS_LIMIT_1_1_BIT_NUM 2

//Value bigger/equal Limit1
#define EL3001_STATUS_LIMIT_1_2_BIT_NUM 3

//Value smaller/equal Limit2 &
#define EL3001_STATUS_LIMIT_2_1_BIT_NUM 4

// Bit1: Value bigger/equal Limit2
#define EL3001_STATUS_LIMIT_2_2_BIT_NUM 5

//Bit set when Over- or Underrange
#define EL3001_STATUS_ERROR_BIT_NUM 6

// TRUE when this TxPDO is not valid
#define EL3001_STATUS_TXPDO_STATE_BIT_NUM 13

// Bit toggles everytime when new value available
#define EL3001_STATUS_TXPDO_TOGGLE_BIT_NUM_8 14




#endif //GBEM_EL3001_H
