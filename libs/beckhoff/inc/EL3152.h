/**
 ******************************************************************************
 * @file           :  EL3152.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL3152_H
#define GBEM_EL3152_H

#include "beckhoff_shared.h"

//EL3152 2Ch. Ana. Input 4-20mA
#define EL3152_EEP_NAME 		"EL3152"
#define EL3152_EEP_MAN 			BECKHOFF_MAN
#define EL3152_EEP_ID 			0x0C503052
#define EL3152_EEP_REV 			0x0

//STATUS is uint8
//VALUE is int16

#define EL3152_CH1_STATUS_INDEX 0
#define EL3152_CH1_VALUE_INDEX 	1
#define EL3152_CH2_STATUS_INDEX 0
#define EL3152_CH2_VALUE_INDEX	1

#endif //GBEM_EL3152_H
