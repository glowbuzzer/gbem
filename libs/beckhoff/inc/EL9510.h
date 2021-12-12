/**
 ******************************************************************************
 * @file           :  EL9510.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL9510_H
#define GBEM_EL9510_H

#include "beckhoff_shared.h"

//EL9510 Power supply terminal 10V
#define EL9510_EEP_NAME 		"EL9510"
#define EL9510_EEP_MAN 			BECKHOFF_MAN
#define EL9510_EEP_ID 			0x25263052
#define EL9510_EEP_REV 			0x0

#define EL9510_POWER_OK_BIT_NUM 0
#define EL9510_OVERLOAD_BIT_NUM 1


#endif //GBEM_EL9510_H
