/**
 ******************************************************************************
 * @file           :  EL9508.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL9508_H
#define GB_MONOREPO_EL9508_H

#include "beckhoff_shared.h"

//EL9508 Power supply terminal 8V
#define EL9508_EEP_NAME 		"EL9508"
#define EL9508_EEP_MAN 			BECKHOFF_MAN
#define EL9508_EEP_ID 			0x25243052
#define EL9508_EEP_REV 			0x0

#define EL9508_POWER_OK_BIT_NUM 0
#define EL9508_OVERLOAD_BIT_NUM 1

#endif //GB_MONOREPO_EL9508_H
