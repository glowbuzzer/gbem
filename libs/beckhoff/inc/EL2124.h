/**
 ******************************************************************************
 * @file           :  EL2124.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL2124_H
#define GB_MONOREPO_EL2124_H

#include "beckhoff_shared.h"

//EL2124 4Ch. Dig. Output 5V, 20mA
#define EL2124_EEP_NAME 		"EL2124"
#define EL2124_EEP_MAN 			BECKHOFF_MAN
#define EL2124_EEP_ID 			0x084C3052
#define EL2124_EEP_REV 			0x0
//4 bits
#define EL2124_CH1_BIT_NUM 		0
#define EL2124_CH2_BIT_NUM 		1
#define EL2124_CH3_BIT_NUM 		2
#define EL2124_CH4_BIT_NUM 		3


#endif //GB_MONOREPO_EL2124_H
