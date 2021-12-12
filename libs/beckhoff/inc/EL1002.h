/**
 ******************************************************************************
 * @file           :  EL1002.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL1002_H
#define GBEM_EL1002_H
#include "beckhoff_shared.h"

//EL1002 2Ch. Dig. Input 24V, 3ms
#define EL1002_EEP_NAME 		"EL1002"
#define EL1002_EEP_MAN 			BECKHOFF_MAN
#define EL1002_EEP_ID 			0x03EA3052
#define EL1002_EEP_REV 			0x0

//2 bits
#define EL1002_CH1_BIT_NUM 		0
#define EL1002_CH2_BIT_NUM 		1

#endif //GBEM_EL1002_H
