/**
 ******************************************************************************
 * @file           :  EL2088.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL2088_H
#define GBEM_EL2088_H


#include "beckhoff_shared.h"

//EL2088 8Ch. Dig. Output 24V, 0.5A, switching to negative
#define EL2088_EEP_NAME 		"EL2088"
#define EL2088_EEP_MAN 			BECKHOFF_MAN
#define EL2088_EEP_ID 			0x08283052
#define EL2088_EEP_REV 			0x0
//8 bits
#define EL2088_CH1_BIT_NUM 		0
#define EL2088_CH2_BIT_NUM 		1
#define EL2088_CH3_BIT_NUM 		2
#define EL2088_CH4_BIT_NUM 		3
#define EL2088_CH5_BIT_NUM 		4
#define EL2088_CH6_BIT_NUM 		5
#define EL2088_CH7_BIT_NUM 		6
#define EL2088_CH8_BIT_NUM 		7


#endif //GBEM_EL2088_H
