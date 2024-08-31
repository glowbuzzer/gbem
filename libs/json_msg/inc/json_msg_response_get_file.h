/**
 ******************************************************************************
 * @file           :  json_msg_response_get_file
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_MSG_RESPONSE_GET_FILE_H
#define GBEM_JSON_MSG_RESPONSE_GET_FILE_H

#include "jansson.h"
#include "gberror.h"

gberror_t
json_msg_response_get_file(char *request_id, char *response_json, char *file);

#endif //GBEM_JSON_MSG_RESPONSE_GET_FILE_H
