/**
 ******************************************************************************
 * @file           :  EL1088.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1088_H
#define GB_MONOREPO_EL1088_H

#include "beckhoff_shared.h"


//EL1088 8Ch. Dig. Input 24V, 3ms, negative
#define EL1088_EEP_NAME 		"EL1088"
#define EL1088_EEP_MAN 			BECKHOFF_MAN
#define EL1088_EEP_ID 			0x04403052
#define EL1088_EEP_REV 			0x0
//8 bits
#define EL1088_CH1_BIT_NUM 		0
#define EL1088_CH2_BIT_NUM 		1
#define EL1088_CH3_BIT_NUM 		2
#define EL1088_CH4_BIT_NUM 		3
#define EL1088_CH5_BIT_NUM 		4
#define EL1088_CH6_BIT_NUM 		5
#define EL1088_CH7_BIT_NUM 		6
#define EL1088_CH8_BIT_NUM 		7

#endif //GB_MONOREPO_EL1088_H
