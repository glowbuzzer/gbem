/**
 ******************************************************************************
 * @file           :  EL1084.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1084_H
#define GB_MONOREPO_EL1084_H

#include "beckhoff_shared.h"


//EL1084 4Ch. Dig. Input 24V, 3ms, negative
#define EL1084_EEP_NAME 		"EL1084"
#define EL1084_EEP_MAN 			BECKHOFF_MAN
#define EL1084_EEP_ID 			0x043C3052
#define EL1084_EEP_REV 			0x0
//4 bits
#define EL1084_CH1_BIT_NUM 		0
#define EL1084_CH2_BIT_NUM 		1
#define EL1084_CH3_BIT_NUM 		2
#define EL1084_CH4_BIT_NUM 		3

#endif //GB_MONOREPO_EL1084_H
