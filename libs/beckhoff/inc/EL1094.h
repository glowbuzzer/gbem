/**
 ******************************************************************************
 * @file           :  EL1094.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1094_H
#define GB_MONOREPO_EL1094_H

#include "beckhoff_shared.h"
//EL1094 4Ch. Dig. Input 24V, 10µs, negative
#define EL1094_EEP_NAME 		"EL1094"
#define EL1094_EEP_MAN	 		BECKHOFF_MAN
#define EL1094_EEP_ID 			0x04463052
#define EL1094_EEP_REV 			0x0
//4 bits
#define EL1094_CH1_BIT_NUM 		0
#define EL1094_CH2_BIT_NUM 		1
#define EL1094_CH3_BIT_NUM 		2
#define EL1094_CH4_BIT_NUM 		3

#endif //GB_MONOREPO_EL1094_H
