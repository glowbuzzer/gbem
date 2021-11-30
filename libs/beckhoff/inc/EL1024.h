/**
 ******************************************************************************
 * @file           :  EL1024.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1024_H
#define GB_MONOREPO_EL1024_H

#include "beckhoff_shared.h"

//EL1024 4Ch. Dig. Input 24V, Type 2, 3ms
#define EL1024_EEP_NAME 		"EL1024"
#define EL1024_EEP_MAN 			BECKHOFF_MAN
#define EL1024_EEP_ID 			0x04003052
#define EL1024_EEP_REV 			0x0
//4 bits
#define EL1024_CH1_BIT_NUM 		0
#define EL1024_CH2_BIT_NUM 		1
#define EL1024_CH3_BIT_NUM 		2
#define EL1024_CH4_BIT_NUM 		3


#endif //GB_MONOREPO_EL1024_H
