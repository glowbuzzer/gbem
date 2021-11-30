/**
 ******************************************************************************
 * @file           :  EL4122.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL4122_H
#define GB_MONOREPO_EL4122_H
#include "beckhoff_shared.h"

//EL4122 2Ch. Ana. Output 4-20mA
#define EL4122_EEP_NAME 		"EL4122"
#define EL4122_EEP_MAN 			BECKHOFF_MAN
#define EL4122_EEP_ID 			0x101A3052
#define EL4122_EEP_REV 			0x0

//VALUE is int16
#define EL4122_CH1_VALUE_INDEX 	0
#define EL4122_CH2_VALUE_INDEX	2


#endif //GB_MONOREPO_EL4122_H
