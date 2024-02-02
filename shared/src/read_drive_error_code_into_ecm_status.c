/**
 ******************************************************************************
 * @file           :  read_drive_error_code_into_ecm_status.c
 * @brief          :  read a drive error code into the ecm status
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "read_drive_error_code_into_ecm_status.h"
#include "map.h"
#include "ecm_status.h"
#include "log.h"

void read_drive_error_code_into_ecm_status(uint16_t drive, bool pdo_only,bool historic) {
    uint8_t *error_code_string;


    if (*map_drive_get_error_string_pdo_function_ptr[drive] != NULL) {
        error_code_string = map_drive_get_error_string_pdo_function_ptr[drive](drive);
        if (historic) {
            memset(&ecm_status.drives[drive].historical_error_message[0], 0,
                   sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].historical_error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        } else {
            memset(&ecm_status.drives[drive].error_message[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        }
    } else {
        LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                 "GBEM: Missing function pointer for map_drive_get_error_string_sdo or pdo on drive [%u]", drive);
    }

    if (pdo_only) {
        return;
    }

    if (*map_drive_get_error_string_sdo_function_ptr[drive] != NULL) {
        error_code_string = map_drive_get_error_string_sdo_function_ptr[drive](drive);
        if (historic) {
            memset(&ecm_status.drives[drive].historical_error_message[0], 0,
                   sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].historical_error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        } else {
            memset(&ecm_status.drives[drive].error_message[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        }
    }
}

