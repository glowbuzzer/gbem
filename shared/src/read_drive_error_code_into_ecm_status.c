/**
 ******************************************************************************
 * @file           :  read_drive_error_code_into_ecm_status.c
 * @brief          :
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

void read_drive_error_code_into_ecm_status(uint16_t drive) {

    uint8_t *error_code_string;

    if (*map_drive_get_error_string_sdo_function_ptr[drive] != NULL) {
        error_code_string = map_drive_get_error_string_sdo_function_ptr[drive](drive);

        //                printf("drive err msg: %s\n", ecm_status.drives[i].error_message);
        memset(&ecm_status.drives[drive].error_message[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
        strncpy(&ecm_status.drives[drive].error_message[0], (char *) error_code_string,
                (sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH) - 1);
        memcpy(&ecm_status.drives[drive].error_message[0], error_code_string,
               strlen((char *) error_code_string) + 1);
    } else {
        LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                 "GBEM: Missing function pointer for map_drive_get_error_string_sdo on drive [%u]", drive);
    }
}