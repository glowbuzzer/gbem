/**
 ******************************************************************************
 * @file           :  EL9505.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL9505_H
#define GB_MONOREPO_EL9505_H
#include "beckhoff_shared.h"

//EL9505 Power supply terminal 5V
#define EL9505_EEP_NAME 		"EL9505"
#define EL9505_EEP_MAN 			BECKHOFF_MAN
#define EL9505_EEP_ID 			0x25213052
#define EL9505_EEP_REV 			0x0

#define EL9505_POWER_OK_BIT_NUM 0
#define EL9505_OVERLOAD_BIT_NUM 1


#endif //GB_MONOREPO_EL9505_H
