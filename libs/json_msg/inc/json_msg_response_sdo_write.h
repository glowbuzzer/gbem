/**
 ******************************************************************************
 * @file           :  json_msg_response_sdo_write
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_MSG_RESPONSE_SDO_WRITE_H
#define GBEM_JSON_MSG_RESPONSE_SDO_WRITE_H

#include "jansson.h"
#include "json_msg_response.h"

gberror_t
json_msg_response_sdo_write(char *request_id, bool error, char *message,
                            char *response_json);


#endif //GBEM_JSON_MSG_RESPONSE_SDO_WRITE_H
