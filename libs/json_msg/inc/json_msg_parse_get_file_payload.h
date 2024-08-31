/**
 ******************************************************************************
 * @file           :  json_msg_parse_get_file_payload
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_MSG_PARSE_GET_FILE_PAYLOAD_H
#define GBEM_JSON_MSG_PARSE_GET_FILE_PAYLOAD_H

#include "jansson.h"
#include "json_msg_response.h"
#include <string.h>
#include "json_msg.h"
#include "shared_mem_types.h"
#include "dpm.h"
#include "user_message.h"


gberror_t
json_msg_parse_get_file_payload(json_t *get_file_payload, uint16_t *slave, uint32_t *password, char *filename);

#endif //GBEM_JSON_MSG_PARSE_GET_FILE_PAYLOAD_H
