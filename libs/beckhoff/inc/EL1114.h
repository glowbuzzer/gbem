/**
 ******************************************************************************
 * @file           :  EL1114.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1114_H
#define GB_MONOREPO_EL1114_H

#include "beckhoff_shared.h"

//EL1114 4Ch. Dig. Input 24V, 10µs, Sensor Power
#define EL1114_EEP_NAME 		"EL1114"
#define EL1114_EEP_MAN 			BECKHOFF_MAN
#define EL1114_EEP_ID 			0x045A3052
#define EL1114_EEP_REV 			0x0
//4 bits
#define EL1114_CH1_BIT_NUM 		0
#define EL1114_CH2_BIT_NUM 		1
#define EL1114_CH3_BIT_NUM 		2
#define EL1114_CH4_BIT_NUM 		3


#endif //GB_MONOREPO_EL1114_H
