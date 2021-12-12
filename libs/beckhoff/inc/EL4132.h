/**
 ******************************************************************************
 * @file           :  EL4132.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL4132_H
#define GBEM_EL4132_H

#include "beckhoff_shared.h"

//EL4132 2Ch. Ana. Output +/-10V
#define EL4132_EEP_NAME 		"EL4132"
#define EL4132_EEP_MAN 			BECKHOFF_MAN
#define EL4132_EEP_ID 			0x10243052
#define EL4132_EEP_REV 			0x0

//VALUE is int16
#define EL4132_CH1_VALUE_INDEX 	0
#define EL4132_CH2_VALUE_INDEX	2

#endif //GBEM_EL4132_H
