/**
 ******************************************************************************
 * @file           :  error_code.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef MONOREPO_AKD_ERROR_CODE_H
#define MONOREPO_AKD_ERROR_CODE_H


#include "std_headers.h"

#define MAX_LEN_AKD_ERROR_DESC 200
#define MAX_LEN_AKD_TEXT_CODE 9

typedef enum {
    AKD_WARNING,
    AKD_ERROR
}akd_error_type_t;

typedef struct {
    uint32_t numeric_code;
            akd_error_type_t type;
    char text_code [MAX_LEN_AKD_TEXT_CODE];
    char description [MAX_LEN_AKD_ERROR_DESC];
}akd_error_code_description_t;

#define NUM_OF_AKD_ERROR_STRINGS 216
const akd_error_code_description_t akd_error_code_description[NUM_OF_AKD_ERROR_STRINGS];


#endif //MONOREPO_AKD_ERROR_CODE_H
