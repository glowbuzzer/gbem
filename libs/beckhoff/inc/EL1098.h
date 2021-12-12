/**
 ******************************************************************************
 * @file           :  EL1098.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL1098_H
#define GBEM_EL1098_H
#include "beckhoff_shared.h"


//EL1098 8Ch. Dig. Input 24V, 10µs, negative
#define EL1098_EEP_NAME 		"EL1098"
#define EL1098_EEP_MAN 			BECKHOFF_MAN
#define EL1098_EEP_ID 			0x044A3052
#define EL1098_EEP_REV 			0x0
//8 bits
#define EL1098_CH1_BIT_NUM 		0
#define EL1098_CH2_BIT_NUM 		1
#define EL1098_CH3_BIT_NUM 		2
#define EL1098_CH4_BIT_NUM 		3
#define EL1098_CH5_BIT_NUM 		4
#define EL1098_CH6_BIT_NUM 		5
#define EL1098_CH7_BIT_NUM 		6
#define EL1098_CH8_BIT_NUM 		7
#endif //GBEM_EL1098_H
