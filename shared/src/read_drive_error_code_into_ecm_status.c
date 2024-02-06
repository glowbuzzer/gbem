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
#include "user_message.h"


void read_drive_error_code_into_ecm_status_sdo(uint16_t drive) {
    uint8_t *error_code_string;
    bool error;

    if (*map_drive_get_error_string_sdo_function_ptr[drive] != NULL) {
        error_code_string = map_drive_get_error_string_sdo_function_ptr[drive](drive, &error);
        if (error) {
            //copy to history
            memset(&ecm_status.drives[drive].historical_error_message[0], 0,
                   sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].historical_error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
            //copy to current error
            memset(&ecm_status.drives[drive].error_message[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        } else {
            //no error so just copy to current
            memset(&ecm_status.drives[drive].error_message[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        }
    }
}


void read_drive_error_code_into_ecm_status_pdo(uint16_t drive) {
    uint8_t *error_code_string;
    bool error;

    if (*map_drive_get_error_string_pdo_function_ptr[drive] != NULL) {
        error_code_string = map_drive_get_error_string_pdo_function_ptr[drive](drive, &error);
        if (error) {
            //copy to history
            memset(&ecm_status.drives[drive].historical_error_message[0], 0,
                   sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].historical_error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
            //copy to current error
            memset(&ecm_status.drives[drive].error_message[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        } else {
            //no error so just copy to current
            memset(&ecm_status.drives[drive].error_message[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
            memcpy(&ecm_status.drives[drive].error_message[0], error_code_string,
                   strlen((char *) error_code_string) + 1);
        }
    } else {
        LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                 "GBEM: Missing function pointer for map_drive_get_error_string_sdo or pdo on drive [%u]", drive);
    }
}

void read_drive_error_code_into_ecm_status(uint16_t drive, drive_error_message_read_type_t read_type) {
    uint8_t *error_code_string;

    bool error;

    switch (read_type) {
        case DRIVE_ERROR_MESSAGE_READ_TYPE_NONE:
            return;
        case DRIVE_ERROR_MESSAGE_READ_TYPE_PDO:
            read_drive_error_code_into_ecm_status_pdo(drive);
            break;
        case DRIVE_ERROR_MESSAGE_READ_TYPE_SDO:
            read_drive_error_code_into_ecm_status_sdo(drive);
            break;
        case DRIVE_ERROR_MESSAGE_READ_TYPE_BOTH:
            read_drive_error_code_into_ecm_status_pdo(drive);
            read_drive_error_code_into_ecm_status_sdo(drive);
            break;
        default:
            UM_FATAL("GBEM: Invalid drive error message read type");
    }
}

