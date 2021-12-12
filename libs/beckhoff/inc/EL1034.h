/**
 ******************************************************************************
 * @file           :  EL1034.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL1034_H
#define GBEM_EL1034_H

#include "beckhoff_shared.h"

//EL1034 4Ch. Dig. Input 24V, potential-free, 10µs
#define EL1034_EEP_NAME 		"EL1034"
#define EL1034_EEP_MAN 			BECKHOFF_MAN
#define EL1034_EEP_ID 			0x040A3052
#define EL1034_EEP_REV 			0x0
//4 bits
#define EL1034_CH1_BIT_NUM 		0
#define EL1034_CH2_BIT_NUM 		1
#define EL1034_CH3_BIT_NUM 		2
#define EL1034_CH4_BIT_NUM 		3



#endif //GBEM_EL1034_H
