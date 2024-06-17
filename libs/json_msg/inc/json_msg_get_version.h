/**
 ******************************************************************************
 * @file           :  json_msg_get_version
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_MSG_GET_VERSION_H
#define GBEM_JSON_MSG_GET_VERSION_H

#include "jansson.h"
#include "gberror.h"

gberror_t
json_msg_response_get_version(char *request_id, char *response_json);

#endif //GBEM_JSON_MSG_GET_VERSION_H
