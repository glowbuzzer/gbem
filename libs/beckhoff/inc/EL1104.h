/**
 ******************************************************************************
 * @file           :  EL1104.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1104_H
#define GB_MONOREPO_EL1104_H

#include "beckhoff_shared.h"

//EL1104 4Ch. Dig. Input 24V, 3ms, Sensor Power
#define EL1104_EEP_NAME 		"EL1104"
#define EL1104_EEP_MAN 			BECKHOFF_MAN
#define EL1104_EEP_ID 			0x04503052
#define EL1104_EEP_REV 			0x0
//4 bits
#define EL1104_CH1_BIT_NUM 		0
#define EL1104_CH2_BIT_NUM 		1
#define EL1104_CH3_BIT_NUM 		2
#define EL1104_CH4_BIT_NUM 		3
#endif //GB_MONOREPO_EL1104_H
