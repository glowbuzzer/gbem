/**
 ******************************************************************************
 * @file           :  json_msg_response
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_MSG_RESPONSE_H
#define GBEM_JSON_MSG_RESPONSE_H

#include "ethercattype.h"

char *json_msg_response_sdo_read(char *request_id, ec_value value, ec_datatype datatype, int status);

#endif //GBEM_JSON_MSG_RESPONSE_H
