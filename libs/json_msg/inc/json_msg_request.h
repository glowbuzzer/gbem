/**
 ******************************************************************************
 * @file           :  json_msg_request
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_MSG_REQUEST_H
#define GBEM_JSON_MSG_REQUEST_H

#include "jansson.h"
#include "user_message.h"
#include "gberror.h"

gberror_t json_msg_request_respond_handle(char *msg, char *response_json, char *request_id);


#endif //GBEM_JSON_MSG_REQUEST_H
