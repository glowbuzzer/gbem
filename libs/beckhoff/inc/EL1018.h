/**
 ******************************************************************************
 * @file           :  EL1018.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL1018_H
#define GBEM_EL1018_H

#include "beckhoff_shared.h"

//EL1018 8Ch. Dig. Input 24V, 10µs
#define EL1018_EEP_NAME 		"EL1018"
#define EL1018_EEP_MAN 			BECKHOFF_MAN
#define EL1018_EEP_ID 			0x03FA3052
#define EL1018_EEP_REV 			0x0
//8 bits
#define EL1018_CH1_BIT_NUM 		0
#define EL1018_CH2_BIT_NUM 		1
#define EL1018_CH3_BIT_NUM 		2
#define EL1018_CH4_BIT_NUM 		3
#define EL1018_CH5_BIT_NUM 		4
#define EL1018_CH6_BIT_NUM 		5
#define EL1018_CH7_BIT_NUM 		6
#define EL1018_CH8_BIT_NUM 		7


#endif //GBEM_EL1018_H
