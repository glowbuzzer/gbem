/**
 ******************************************************************************
 * @file           :  EL4001.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL4001_H
#define GBEM_EL4001_H
#include "beckhoff_shared.h"


//EL4001 1Ch. Ana. Output 0-10V, 12bit
#define EL4001_EEP_NAME "EL4001"
#define EL4001_EEP_MAN BECKHOFF_MAN
#define EL4001_EEP_REV 0x00100000
#define EL4001_EEP_ID 0xfa13052

//int16
#define EL4001_ANALOG_OUPUT_INDEX 0




#endif //GBEM_EL4001_H
