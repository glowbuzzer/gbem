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

#include <string.h>
#include "json_msg.h"
#include "json_msg_request.h"
#include "ethercattype.h"


gberror_t
json_msg_request_extract(char *msg, const char **request_id, json_msg_request_types_t *request_type, json_t **payload) {

    json_t *root;
    json_error_t error;

    root = json_loads(msg, 0, &error);
    if (!root) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: on line [%d ] [%s] of JSON request\n", error.line, error.text);
        return E_GENERAL_FAILURE;
    }

    json_t *request_id_json = json_object_get(root, "request_id");

    if (!json_is_object(request_id_json)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: request_id not found in JSON request\n");
        json_decref(root);
        return E_GENERAL_FAILURE;
    }
    if (!json_is_string(request_id_json)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: request_id is not a string in JSON request\n");
        json_decref(root);
        return E_GENERAL_FAILURE;
    }

    *request_id = json_string_value(request_id_json);


    json_t *request_type_json = json_object_get(root, "request_type");

    if (!json_is_object(request_type_json)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: request_type not found in JSON request\n");
        json_decref(root);
        return E_GENERAL_FAILURE;
    }
    if (!json_is_string(request_type_json)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: request_type is not a string in JSON request\n");
        json_decref(root);
        return E_GENERAL_FAILURE;
    }

    const char *request_type_str = json_string_value(request_type_json);
    for (int i = 0; i < JSON_MSG_NUM_REQUEST_TYPES; i++) {
        if (strcmp(request_type_str, json_msg_request_type_strings_t[i]) == 0) {
            *request_type = (json_msg_request_types_t) i;
            break;
        }
    }

    json_t *payload_json = json_object_get(root, "payload");

    if (!json_is_object(payload_json)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: payload not found in JSON request\n");
        json_decref(root);
        return E_GENERAL_FAILURE;
    }

    *payload = payload_json;

    return E_SUCCESS;
}


gberror_t json_msg_parse_sdo_payload(json_t *sdo_payload, uint16_t *slave, ec_datatype *datatype, uint16_t *index,
                                     uint8_t *sub_index) {


}

gberror_t json_msg_request(char *msg) {

    const char *request_id;
    json_msg_request_types_t request_type;
    json_t *payload;

    gberror_t ret = json_msg_request_extract(msg, &request_id, &request_type, &payload);

    if (ret != E_SUCCESS) {
        return ret;
    }

    uint16_t slave;
    ec_datatype datatype;
    uint16_t index;
    uint8_t sub_index;

    switch (request_type) {
        case SDO_READ:
            json_msg_parse_sdo_payload(payload, &slave, &datatype, &index, &sub_index);

            //todo
            //read sdo

            break;
        default:
            UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: request_type not supported\n");
            return E_GENERAL_FAILURE;
    }

    return E_SUCCESS;


}

