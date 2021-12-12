/**
 ******************************************************************************
 * @file           :  EL2002.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL2002_H
#define GBEM_EL2002_H

#include "beckhoff_shared.h"

//EL2002 2Ch. Dig. Output 24V, 0.5A
#define EL2002_EEP_NAME 		"EL2002"
#define EL2002_EEP_MAN 			BECKHOFF_MAN
#define EL2002_EEP_ID 			0x07D23052
#define EL2002_EEP_REV 			0x0
//2 bits
#define EL2002_CH1_BIT_NUM 		0
#define EL2002_CH2_BIT_NUM 		1

#endif //GBEM_EL2002_H
