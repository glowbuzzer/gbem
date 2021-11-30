/**
 ******************************************************************************
 * @file           :  EL1809.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL1809_H
#define GB_MONOREPO_EL1809_H

#include "beckhoff_shared.h"

//EL1809 16Ch. Dig. Input 24V, 3ms
#define EL1809_EEP_NAME 		"EL1809"
#define EL1809_EEP_MAN 			BECKHOFF_MAN
#define EL1809_EEP_ID 			0x07113052
#define EL1809_EEP_REV 			0x0
//8 bits
#define EL1809_CH1_BIT_NUM 		0
#define EL1809_CH2_BIT_NUM 		1
#define EL1809_CH3_BIT_NUM 		2
#define EL1809_CH4_BIT_NUM 		3
#define EL1809_CH5_BIT_NUM 		4
#define EL1809_CH6_BIT_NUM 		5
#define EL1809_CH7_BIT_NUM 		6
#define EL1809_CH8_BIT_NUM 		7

#endif //GB_MONOREPO_EL1809_H
