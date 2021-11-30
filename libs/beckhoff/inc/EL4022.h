/**
 ******************************************************************************
 * @file           :  EL4022.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL4022_H
#define GB_MONOREPO_EL4022_H

#include "beckhoff_shared.h"

//EL4022 2Ch. Ana. Output 4-20mA, 12bit
#define EL4022_EEP_NAME 		"EL4022"
#define EL4022_EEP_MAN 			BECKHOFF_MAN
#define EL4022_EEP_ID 			0x0fb63052
#define EL4022_EEP_REV 			0x0

//VALUE is int16
#define EL4022_CH1_VALUE_INDEX 	0
#define EL4022_CH2_VALUE_INDEX	2
#endif //GB_MONOREPO_EL4022_H
