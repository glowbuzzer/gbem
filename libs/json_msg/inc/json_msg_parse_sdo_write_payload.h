/**
 ******************************************************************************
 * @file           :  json_msg_parse_sdo_write_payload
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_MSG_PARSE_SDO_WRITE_PAYLOAD_H
#define GBEM_JSON_MSG_PARSE_SDO_WRITE_PAYLOAD_H

#include "jansson.h"
#include "json_msg_response.h"
#include <string.h>
#include "json_msg.h"
#include "shared_mem_types.h"
#include "dpm.h"
#include "user_message.h"


gberror_t json_msg_parse_sdo_write_payload(json_t *sdo_payload, uint16_t *slave, ec_datatype *datatype, uint16_t *index,
                                           uint8_t *sub_index, uint8_t *len, ec_value *value);

#endif //GBEM_JSON_MSG_PARSE_SDO_WRITE_PAYLOAD_H
