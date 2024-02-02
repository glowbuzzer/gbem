/**
 ******************************************************************************
 * @file           :  read_drive_error_code_into_ecm_status
 * @brief          :  read a drive error code into the ecm status
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_READ_DRIVE_ERROR_CODE_INTO_ECM_STATUS_H
#define GBEM_READ_DRIVE_ERROR_CODE_INTO_ECM_STATUS_H

#include "std_headers.h"

void read_drive_error_code_into_ecm_status(uint16_t drive, bool pdo_only,bool historic);

#endif //GBEM_READ_DRIVE_ERROR_CODE_INTO_ECM_STATUS_H
