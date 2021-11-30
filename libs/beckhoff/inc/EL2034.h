/**
 ******************************************************************************
 * @file           :  EL2034.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL2034_H
#define GB_MONOREPO_EL2034_H

#include "beckhoff_shared.h"

//EL2034 4Ch. Dig. Output 24V, 2A, Diagnostic
#define EL2034_EEP_NAME 		EL2034
#define EL2034_EEP_MAN 			BECKHOFF_MAN
#define EL2034_EEP_ID 			0x07F23052
#define EL2034_EEP_REV 			0x0
//4 bits channels, 4 bits diag
#define EL2034_CH1_BIT_NUM 		0
#define EL2034_CH2_BIT_NUM 		1
#define EL2034_CH3_BIT_NUM 		2
#define EL2034_CH4_BIT_NUM 		3
#define EL2034_CH1_DIAG_BIT_NUM 0
#define EL2034_CH2_DIAG_BIT_NUM 1
#define EL2034_CH3_DIAG_BIT_NUM 2
#define EL2034_CH4_DIAG_BIT_NUM 3



#endif //GB_MONOREPO_EL2034_H
