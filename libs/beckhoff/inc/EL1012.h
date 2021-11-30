/**
 ******************************************************************************
 * @file           :  EL1012.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1012_H
#define GB_MONOREPO_EL1012_H
#include "beckhoff_shared.h"

//EL1012 2Ch. Dig. Input 24V, 10µs
#define EL1012_EEP_NAME 		"EL1012"
#define EL1012_EEP_MAN 			BECKHOFF_MAN
#define EL1012_EEP_ID 			0x03F43052
#define EL1012_EEP_REV 			0x0
//2 bits
#define EL1012_CH1_BIT_NUM 		0
#define EL1012_CH2_BIT_NUM 		1

#endif //GB_MONOREPO_EL1012_H
