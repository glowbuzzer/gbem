/**
 ******************************************************************************
 * @file           :  EL3142.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL3142_H
#define GB_MONOREPO_EL3142_H


#include "beckhoff_shared.h"
//EL3142 2Ch. Ana. Input 0-20mA
#define EL3142_EEP_NAME 		"EL3142"
#define EL3142_EEP_MAN 			BECKHOFF_MAN
#define EL3142_EEP_ID 			0x0C463052
#define EL3142_EEP_REV 			0x0

//STATUS is uint8
//VALUE is int16

#define EL3142_CH1_STATUS_INDEX 0
#define EL3142_CH1_VALUE_INDEX 	1
#define EL3142_CH2_STATUS_INDEX 0
#define EL3142_CH2_VALUE_INDEX	1


#endif //GB_MONOREPO_EL3142_H
