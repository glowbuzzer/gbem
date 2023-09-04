/**
 ******************************************************************************
 * @file           :  aw-j25.h
 * @brief          :  AutomationWare J25 control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_AW_J25_H
#define GBEM_AW_J25_H


#include "map.h"
#include "gberror.h"
#include "automationware.h"

//Slave functions
gberror_t ec_apply_standard_sdos_aw_j25(uint16_t slave);


#define AW_J25_EEP_NAME "SOMANET"
#define AW_J25_EEP_MAN AW_MOTOR_MAN
#define AW_J25_EEP_REV 0x0a000002
#define AW_J25_EEP_ID 0x00000201


#endif //GBEM_AW_J25_H
