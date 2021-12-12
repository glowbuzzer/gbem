/**
 ******************************************************************************
 * @file           :  EL3122.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL3122_H
#define GBEM_EL3122_H

#include "beckhoff_shared.h"

//EL3122 2Ch. Ana. Input 4-20mA, Diff.
#define EL3122_EEP_NAME 		"EL3122"
#define EL3122_EEP_MAN 			BECKHOFF_MAN
#define EL3122_EEP_ID 			0x0C323052
#define EL3122_EEP_REV 			0x0

//STATUS is uint8
//VALUE is int16

#define EL3122_CH1_STATUS_INDEX 0
#define EL3122_CH1_VALUE_INDEX 	1
#define EL3122_CH2_STATUS_INDEX 0
#define EL3122_CH2_VALUE_INDEX	1


#endif //GBEM_EL3122_H
