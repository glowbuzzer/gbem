/**
 ******************************************************************************
 * @file           :  EL4002.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL4002_H
#define GB_MONOREPO_EL4002_H

#include "beckhoff_shared.h"

//EL4002 2Ch. Ana. Output 0-10V, 12bit
#define EL4002_EEP_NAME 		"EL4002"
#define EL4002_EEP_MAN 			BECKHOFF_MAN
#define EL4002_EEP_ID 			0x0fa23052
#define EL4002_EEP_REV	 		0x0

//VALUE is int16
#define EL4002_CH1_VALUE_INDEX 	0
#define EL4002_CH2_VALUE_INDEX	2


#endif //GB_MONOREPO_EL4002_H
