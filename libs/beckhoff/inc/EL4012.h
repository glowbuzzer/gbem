/**
 ******************************************************************************
 * @file           :  EL4012.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL4012_H
#define GBEM_EL4012_H

#include "beckhoff_shared.h"

//EL4012 2Ch. Ana. Output 0-20mA, 12bit
#define EL4012_EEP_NAME 		"EL4012"
#define EL4012_EEP_MAN 			BECKHOFF_MAN
#define EL4012_EEP_ID 			0x0fac3052
#define EL4012_EEP_REV 			0x0

//VALUE is int16
#define EL4012_CH1_VALUE_INDEX 	0
#define EL4012_CH2_VALUE_INDEX	2

#endif //GBEM_EL4012_H
