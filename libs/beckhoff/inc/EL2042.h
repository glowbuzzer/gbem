/**
 ******************************************************************************
 * @file           :  EL2042.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL2042_H
#define GBEM_EL2042_H

#include "beckhoff_shared.h"

//EL2042 2Ch. Dig. Output 24V, 4A
#define EL2042_EEP_NAME 		"EL2042"
#define EL2042_EEP_MAN 			BECKHOFF_MAN
#define EL2042_EEP_ID 			0x07FA3052
#define EL2042_EEP_REV 			0x0
//2 bits
#define EL2042_CH1_BIT_NUM 		0
#define EL2042_CH2_BIT_NUM 		1


#endif //GBEM_EL2042_H
