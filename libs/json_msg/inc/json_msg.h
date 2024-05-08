/**
 ******************************************************************************
 * @file           :  json_msg
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_MSG_H
#define GBEM_JSON_MSG_H


#define JSON_MSG_NUM_REQUEST_TYPES 1
typedef enum {
    SDO_READ
} json_msg_request_types_t;

const char *json_msg_request_type_strings_t[JSON_MSG_NUM_REQUEST_TYPES] = {
        "sdo_read"
};


#endif //GBEM_JSON_MSG_H
