/**
 ******************************************************************************
 * @file           :  optidrive_p2.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_OPTIDRIVE_P2_H
#define GB_MONOREPO_OPTIDRIVE_P2_H

#include "map.h"
#include "gberror.h"

#define INVERTEK_MAN 0xE000001
#define OPTIDRIVE_P2_MIS_EEP_NAME "Invertek Optidrive"
#define OPTIDRIVE_P2_EEP_MAN INVERTEK_MAN
#define OPTIDRIVE_P2_EEP_REV 0x00010009
#define OPTIDRIVE_P2_EEP_ID 0x00000034


#define OPTIDRIVE_P2_CONTROLWORD1_PDO_INDEX           0
#define OPTIDRIVE_P2_STATUSWORD1_PDO_INDEX            0

//uint16_t controlword[3] = 48 bits

//uint16_t statusword[4] = 64 bits


#define  OPTIDRIVE_P2_STATUS_STOPPED_BIT_NUMBER                     0
#define  OPTIDRIVE_P2_STATUS_FAULT_BIT_NUMBER                       1
#define  OPTIDRIVE_P2_STATUS_HAND_CONTROL_MODE_BIT_NUMBER           2
#define  OPTIDRIVE_P2_STATUS_INHIBIT_BIT_NUMBER                     3
#define  OPTIDRIVE_P2_STATUS_MAINTENANCE_TIME_REACHED_BIT_NUMBER    4

//bits




#endif //GB_MONOREPO_OPTIDRIVE_P2_H
