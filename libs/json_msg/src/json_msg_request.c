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
#include "json_msg_response.h"
#include "ethercattype.h"
#include "sdos_read_based_on_datatype.h"
#include "shared_mem_types.h"
#include "dpm.h"


gberror_t
json_msg_request_extract(char *msg, int *request_type, json_t **payload) {


    json_t *root, *request;
    json_error_t error;

    root = json_loads(msg, 0, &error);
    printf("JSON: [%s]\n", json_dumps(root, JSON_INDENT(2)));


    if (!root) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: on line [%d ] [%s] of JSON request (json_msg_request_extract)\n",
                 error.line, error.text);
        return E_GENERAL_FAILURE;
    }

//    request = json_object_get(root, "request");
//    if (!json_is_object(request)) {
//        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: request not found in JSON request\n");
//        json_decref(root);
//        return E_GENERAL_FAILURE;
//    }


    json_t *request_type_json = json_object_get(root, "requestType");


    if (!json_is_integer(request_type_json)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: request_type is not a integer in JSON request\n");
        json_decref(root);
        return E_GENERAL_FAILURE;
    }

    *request_type = (int) json_integer_value(request_type_json);

//    {"payload": {"index": 1000, "sub_index": 0,"datatype":4, "slave":1}}


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

    json_t *obj = json_object_get(sdo_payload, "index");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: index is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *index = (uint16_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "sub_index");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: sub_index is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *sub_index = (uint8_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "slave");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: slave is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *slave = (uint16_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "datatype");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: datatype is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *datatype = (ec_datatype) json_integer_value(obj);

    return E_SUCCESS;


}

gberror_t json_msg_request_respond_handle(char *msg, char *response_json, char *request_id) {


    int request_type;
    json_t *payload;
    gberror_t rc;


    if (strnlen(msg, DPM_REQUEST_RESPONSE_DATA_LENGTH) < 5) {
        json_msg_response_error(request_id, 0, "Empty request", response_json);
        return E_SUCCESS;
    }

    rc = json_msg_request_extract(msg, &request_type, &payload);

    if (rc != E_SUCCESS) {
        json_msg_response_error(request_id, 0, "Failed to understand request", response_json);

    }

    uint16_t slave;
    ec_datatype datatype;
    uint16_t index;
    uint8_t sub_index;
    ec_value value;


    switch (request_type) {
        case GBEM_REQUEST_SDO_READ:

            if (json_msg_parse_sdo_payload(payload, &slave, &datatype, &index, &sub_index) != E_SUCCESS) {
                json_msg_response_error(request_id, request_type, "Failed to parse SDO payload", response_json);
                return E_SUCCESS;
            }

            //read sdo

            rc = sdos_read_based_on_datatype(slave, index, sub_index, datatype, &value);

            if (rc != E_SUCCESS) {
                json_msg_response_error(request_id, request_type, "SDO read failed", response_json);
                return E_SUCCESS;
            }

            //respond
            json_msg_response_sdo_read(request_id, value, datatype, false, "SDO read success", response_json);


            break;
        default:
            UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: request_type not supported\n");
            json_msg_response_error(request_id, request_type, "request type not supported", response_json);
            return E_SUCCESS;
    }

    return E_SUCCESS;


}

