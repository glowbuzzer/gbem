/**
 ******************************************************************************
 * @file           :  EL3102.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL3102_H
#define GBEM_EL3102_H

#include "beckhoff_shared.h"

//EL3102 2Ch. Ana. Input +/-10V, Diff.
#define EL3102_EEP_NAME 		"EL3102"
#define EL3102_EEP_MAN 			BECKHOFF_MAN
#define EL3102_EEP_ID 			0x0C1E3052
#define EL3102_EEP_REV 			0x0

/* for 0x1A00 & 0x1A01 PDO mapping */
//STATUS is uint8
//VALUE is int16

#define EL3102_CH1_STATUS_INDEX 0
#define EL3102_CH1_VALUE_INDEX 	1
#define EL3102_CH2_STATUS_INDEX 3
#define EL3102_CH2_VALUE_INDEX	4



//Underrange event active
#define EL3001_STATUS_UNDERRANGE_BIT_NUM 0
//        Overrange event active
#define EL3001_STATUS_OVERRANGE_BIT_NUM 1

//Value at Limit1
#define EL3001_STATUS_LIMIT_1_BIT_NUM 3

//Value at Limit2
#define EL3001_STATUS_LIMIT_2_BIT_NUM 5

//Bit set when Over- or Underrange
#define EL3001_STATUS_ERROR_BIT_NUM 6





#endif //GBEM_EL3102_H
