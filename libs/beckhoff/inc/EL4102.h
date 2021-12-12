/**
 ******************************************************************************
 * @file           :  EL4102.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL4102_H
#define GBEM_EL4102_H
#include "beckhoff_shared.h"


//EL4102 2Ch. Ana. Output 0-10V
#define EL4102_EEP_NAME 		"EL4102"
#define EL4102_EEP_MAN 			BECKHOFF_MAN
#define EL4102_EEP_ID 			0x10063052
#define EL4102_EEP_REV 			0x0

//VALUE is int16
#define EL4102_CH1_VALUE_INDEX 	0
#define EL4102_CH2_VALUE_INDEX	2


#endif //GBEM_EL4102_H
