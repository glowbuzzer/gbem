/**
 ******************************************************************************
 * @file           :  EL9515.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL9515_H
#define GB_MONOREPO_EL9515_H

#include "beckhoff_shared.h"

//EL9515 Power supply terminal 15V
#define EL9515_EEP_NAME 		"EL9515"
#define EL9515_EEP_MAN 			BECKHOFF_MAN
#define EL9515_EEP_ID 			0x252b3052
#define EL9515_EEP_REV 			0x0

#define EL9515_POWER_OK_BIT_NUM 0
#define EL9515_OVERLOAD_BIT_NUM 1



#endif //GB_MONOREPO_EL9515_H
