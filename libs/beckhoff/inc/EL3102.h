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
#define EL3102_CH2_STATUS_INDEX 0
#define EL3102_CH2_VALUE_INDEX	1


#endif //GBEM_EL3102_H
