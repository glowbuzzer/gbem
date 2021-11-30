/**
 ******************************************************************************
 * @file           :  EL2084.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL2084_H
#define GB_MONOREPO_EL2084_H
#include "beckhoff_shared.h"

//EL2084 4Ch. Dig. Output 24V, 0.5A, switching to negative
#define EL2084_EEP_NAME 		"EL2084"
#define EL2084_EEP_MAN 			BECKHOFF_MAN
#define EL2084_EEP_ID 			0x08243052
#define EL2084_EEP_REV 			0x0
//4 bits
#define EL2084_CH1_BIT_NUM 		0
#define EL2084_CH2_BIT_NUM 		1
#define EL2084_CH3_BIT_NUM 		2
#define EL2084_CH4_BIT_NUM 		3

#endif //GB_MONOREPO_EL2084_H
