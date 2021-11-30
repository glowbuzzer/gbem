/**
 ******************************************************************************
 * @file           :  EL1134.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1134_H
#define GB_MONOREPO_EL1134_H


#include "beckhoff_shared.h"
//EL1134 4Ch. Dig. Input 48V, 10µs, Sensor Power
#define EL1134_EEP_NAME 		"EL1134"
#define EL1134_EEP_MAN 			BECKHOFF_MAN
#define EL1134_EEP_ID 			0x046E3052
#define EL1134_EEP_REV 			0x0
//4 bits
#define EL1134_CH1_BIT_NUM 		0
#define EL1134_CH2_BIT_NUM 		1
#define EL1134_CH3_BIT_NUM 		2
#define EL1134_CH4_BIT_NUM 		3



#endif //GB_MONOREPO_EL1134_H
