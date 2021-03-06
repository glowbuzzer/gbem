/**
 ******************************************************************************
 * @file           :  EL4112.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL4112_H
#define GBEM_EL4112_H

#include "beckhoff_shared.h"

//EL4112 2Ch. Ana. Output 0-20mA
#define EL4112_EEP_NAME 		"EL4112"
#define EL4112_EEP_MAN 			BECKHOFF_MAN
#define EL4112_EEP_ID 			0x10103052
#define EL4112_EEP_REV	 		0x0

//VALUE is int16
#define EL4112_CH1_VALUE_INDEX 	0
#define EL4112_CH2_VALUE_INDEX	2

#endif //GBEM_EL4112_H
