/**
 ******************************************************************************
 * @file           :  EL2032.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL2032_H
#define GBEM_EL2032_H

#include "beckhoff_shared.h"

//EL2032 2Ch. Dig. Output 24V, 2A Diag
#define EL2032_EEP_NAME 		"EL20322"
#define EL2032_EEP_MAN 			BECKHOFF_MAN
#define EL2032_EEP_ID 			0x07F03052
#define EL2032_EEP_REV 			0x0
//2 bits channels, 2 bits diag

#define EL2024_CH1_BIT_NUM 		0
#define EL2024_CH2_BIT_NUM 		1
#define EL2024_CH1_DIAG_BIT_NUM 0
#define EL2024_CH2_DIAG_BIT_NUM 1

#endif //GBEM_EL2032_H
