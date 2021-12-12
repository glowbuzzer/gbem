/**
 ******************************************************************************
 * @file           :  EL9512.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL9512_H
#define GBEM_EL9512_H

#include "beckhoff_shared.h"

//EL9512 Power supply terminal 12V
#define EL9512_EEP_NAME 		"EL9512"
#define EL9512_EEP_MAN 			BECKHOFF_MAN
#define EL9512_EEP_ID 			0x25283052
#define EL9512_EEP_REV 			0x0

#define EL9512_POWER_OK_BIT_NUM 0
#define EL9512_OVERLOAD_BIT_NUM 1


#endif //GBEM_EL9512_H
