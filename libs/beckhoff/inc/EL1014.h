/**
 ******************************************************************************
 * @file           :  EL1014.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1014_H
#define GB_MONOREPO_EL1014_H

#include "beckhoff_shared.h"


//EL1014 4Ch. Dig. Input 24V, 10µs
#define EL1014_EEP_NAME 		"EL1014"
#define EL1014_EEP_MAN 			BECKHOFF_MAN
#define EL1014_EEP_ID 			0x03F63052
#define EL1014_EEP_REV 			0x0
//4 bits
#define EL1014_CH1_BIT_NUM 		0
#define EL1014_CH2_BIT_NUM 		1
#define EL1014_CH3_BIT_NUM 		2
#define EL1014_CH4_BIT_NUM 		3


#endif //GB_MONOREPO_EL1014_H
