/**
 ******************************************************************************
 * @file           :  EL3162.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL3162_H
#define GBEM_EL3162_H

#include "beckhoff_shared.h"

//EL3162 2Ch. Ana. Input 0-10V
#define EL3162_EEP_NAME 		"EL31622"
#define EL3162_EEP_MAN 			BECKHOFF_MAN
#define EL3162_EEP_ID 			0x0C5A3052
#define EL3162_EEP_REV 			0x0

//STATUS is uint8
//VALUE is int16

#define EL3162_CH1_STATUS_INDEX 0
#define EL3162_CH1_VALUE_INDEX 	1
#define EL3162_CH2_STATUS_INDEX 0
#define EL3162_CH2_VALUE_INDEX	1


#endif //GBEM_EL3162_H
