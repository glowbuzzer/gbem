/**
 ******************************************************************************
 * @file           :  EL2808.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_EL2808_H
#define GBEM_EL2808_H

#include "beckhoff_shared.h"

//EL2808 8Ch. Dig. Output 24V, 0.5A
#define EL2808_EEP_NAME        "EL2808"
#define EL2808_EEP_MAN            BECKHOFF_MAN
#define EL2808_EEP_ID            0x0AF83052
#define EL2808_EEP_REV            0x0
//8 bits
#define EL2808_CH1_BIT_NUM        0
#define EL2808_CH2_BIT_NUM        1
#define EL2808_CH3_BIT_NUM        2
#define EL2808_CH4_BIT_NUM        3
#define EL2808_CH5_BIT_NUM        4
#define EL2808_CH6_BIT_NUM        5
#define EL2808_CH7_BIT_NUM        6
#define EL2808_CH8_BIT_NUM        7


#endif //GBEM_EL2808_H
