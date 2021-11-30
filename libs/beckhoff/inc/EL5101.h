/**
 ******************************************************************************
 * @file           :  EL5101.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_EL5101_H
#define GB_MONOREPO_EL5101_H
#include "beckhoff_shared.h"




//EL5101 1Ch. Encoder 5V
#define EL5101_EEP_NAME "EL5101"
#define EL5101_EEP_MAN BECKHOFF_MAN
#define EL5101_EEP_REV 0x0
#define EL5101_EEP_ID 0x13ed3052

//uint8
#define EL5101_STATUS_INDEX     0
//uint16
#define EL5101_ACT_VALUE_INDEX  1
//uint16
#define EL5101_LATCH_INDEX      3
//uint8
#define EL5101_CTRL_INDEX       0
//uint16 (this is the value to preset the encoder to
#define EL5101_SET_VALUE_INDEX  1

//ctrl word
#define EL5101_EN_LATCH_BIT_NO          0
#define EL5101_EN_LATCH_EXTP_BIT_NO     1
#define EL5101_CNT_SET_BIT_NO           2
#define EL5101_EN_LATCH_EXTN_BIT_NO     4

//status word
#define EL5101_LATC_VAL_BIT_NO          0
#define EL5101_LAT_EXT_VAL_BIT_NO       1
#define EL5101_CNTSET_ACC_BIT_NO        2
#define EL5101_UNDERFLOW_BIT_NO         3
#define EL5101_OVERFLOW_BIT_NO          4
#define EL5101_STATUS_INPUT_BIT_NO      5


#endif //GB_MONOREPO_EL5101_H
