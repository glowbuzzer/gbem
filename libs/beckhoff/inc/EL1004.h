/**
 ******************************************************************************
 * @file           :  EL1004.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL1004_H
#define GBEM_EL1004_H
#include "beckhoff_shared.h"

//EL1004 4Ch. Dig. Input 24V, 3ms
#define EL1004_EEP_NAME 		"EL1004"
#define EL1004_EEP_MAN 			BECKHOFF_MAN
#define EL1004_EEP_ID 			0x03EC3052
#define EL1004_EEP_REV 			0x0

//4 bits
#define EL1004_CH1_BIT_NUM 		0
#define EL1004_CH2_BIT_NUM 		1
#define EL1004_CH3_BIT_NUM 		2
#define EL1004_CH4_BIT_NUM 		3

#endif //GBEM_EL1004_H
