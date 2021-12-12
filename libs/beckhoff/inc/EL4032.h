/**
 ******************************************************************************
 * @file           :  EL4032.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL4032_H
#define GBEM_EL4032_H

#include "beckhoff_shared.h"

//EL4032 2Ch. Ana. Output +/-10V, 12bit
#define EL4032_EEP_NAME 		"EL4032"
#define EL4032_EEP_MAN 			BECKHOFF_MAN
#define EL4032_EEP_ID 			0x0fc03052
#define EL4032_EEP_REV 			0x0

//VALUE is int16
#define EL4032_CH1_VALUE_INDEX 	0
#define EL4032_CH2_VALUE_INDEX	2

#endif //GBEM_EL4032_H
