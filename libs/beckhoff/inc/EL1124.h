/**
 ******************************************************************************
 * @file           :  EL1124.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL1124_H
#define GBEM_EL1124_H
#include "beckhoff_shared.h"

//EL1124 4Ch. Dig. Input 5V, 10µs, Sensor Power
#define EL1124_EEP_NAME 		"EL1124"
#define EL1124_EEP_MAN 			BECKHOFF_MAN
#define EL1124_EEP_ID 			0x04643052
#define EL1124_EEP_REV 			0x0
//4 bits
#define EL1124_CH1_BIT_NUM 		0
#define EL1124_CH2_BIT_NUM 		1
#define EL1124_CH3_BIT_NUM 		2
#define EL1124_CH4_BIT_NUM 		3


#endif //GBEM_EL1124_H
