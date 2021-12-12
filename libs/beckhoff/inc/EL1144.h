/**
 ******************************************************************************
 * @file           :  EL1144.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL1144_H
#define GBEM_EL1144_H

#include "beckhoff_shared.h"

//EL1144 4Ch. Dig. Input 12V, 10µs, Sensor Power
#define EL1144_EEP_NAME 		"EL1144"
#define EL1144_EEP_MAN 			BECKHOFF_MAN
#define EL1144_EEP_ID 			0x04783052
#define EL1144_EEP_REV 			0x0
//4 bits
#define EL1144_CH1_BIT_NUM 		0
#define EL1144_CH2_BIT_NUM 		1
#define EL1144_CH3_BIT_NUM 		2
#define EL1144_CH4_BIT_NUM 		3


#endif //GBEM_EL1144_H
