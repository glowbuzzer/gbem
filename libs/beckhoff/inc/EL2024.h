/**
 ******************************************************************************
 * @file           :  EL2024.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL2024_H
#define GB_MONOREPO_EL2024_H

#include "beckhoff_shared.h"

//EL2024 4Ch. Dig. Output 24V, 2A
#define EL2024_EEP_NAME 		"EL2024"
#define EL2024_EEP_MAN 			BECKHOFF_MAN
#define EL2024_EEP_ID 			0x07E83052
#define EL2024_EEP_REV 			0x0
//4 bits
#define EL2024_CH1_BIT_NUM 		0
#define EL2024_CH2_BIT_NUM 		1
#define EL2024_CH3_BIT_NUM 		2
#define EL2024_CH4_BIT_NUM 		3

#endif //GB_MONOREPO_EL2024_H
