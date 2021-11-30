/**
 ******************************************************************************
 * @file           :  EL2004.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL2004_H
#define GB_MONOREPO_EL2004_H

#include "beckhoff_shared.h"

//EL2004 4Ch. Dig. Output 24V, 0.5A
#define EL2004_EEP_NAME 		"EL2004"
#define EL2004_EEP_MAN 			BECKHOFF_MAN
#define EL2004_EEP_ID 			0x07D43052
#define EL2004_EEP_REV 			0x0
//4 bits
#define EL2004_CH1_BIT_NUM 		0
#define EL2004_CH2_BIT_NUM 		1
#define EL2004_CH3_BIT_NUM 		2
#define EL2004_CH4_BIT_NUM 		3


#endif //GB_MONOREPO_EL2004_H
