/**
 ******************************************************************************
 * @file           :  EL7211_error_code.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef MONOREPO_EL7211_ERROR_CODE_H
#define MONOREPO_EL7211_ERROR_CODE_H

typedef struct {
    uint16_t text_id;
    char text_string[100];
    uint8_t num_params;
} etg1020_diag_text_t;

#define NUM_OF_EL7211_DIAG_STRINGS 175
etg1020_diag_text_t etg1020_diag_text[NUM_OF_EL7211_DIAG_STRINGS];



#endif //MONOREPO_EL7211_ERROR_CODE_H
