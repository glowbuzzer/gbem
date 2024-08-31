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
#include "sdos_write_based_on_datatype.h"
#include "shared_mem_types.h"
#include "dpm.h"
#include "json_msg_response_get_version.h"
#include "json_msg_response_sdo_read.h"
#include "json_msg_response_get_file.h"
#include "json_msg_parse_sdo_read_payload.h"
#include "json_msg_parse_sdo_write_payload.h"
#include "json_msg_parse_get_file_payload.h"
#include "json_msg_response_sdo_write.h"
#include "foe_read_file.h"
#include "../../foe/inc/foe_read_file.h"
#include "dpm.h"
#include "user_message.h"

#define JSON_MSG_MAX_FILE_SIZE  (DPM_REQUEST_RESPONSE_DATA_LENGTH -100)

gberror_t
json_msg_request_extract(char *msg, int *request_type, json_t **payload) {


    json_t *root, *request;
    json_error_t error;

    root = json_loads(msg, 0, &error);
    printf("JSON: [%s]\n", json_dumps(root, JSON_INDENT(2)));


    if (!root) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: on line [%d ] [%s] of JSON request (json_msg_request_extract)\n",
                 error.line, error.text);
        return E_GENERAL_FAILURE;
    }

//    request = json_object_get(root, "request");
//    if (!json_is_object(request)) {
//        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: request not found in JSON request\n");
//        json_decref(root);
//        return E_GENERAL_FAILURE;
//    }


    json_t *request_type_json = json_object_get(root, "requestType");


    if (!json_is_integer(request_type_json)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: request_type is not a integer in JSON request\n");
        json_decref(root);
        return E_GENERAL_FAILURE;
    }

    *request_type = (int) json_integer_value(request_type_json);

//    {"payload": {"index": 1000, "sub_index": 0,"datatype":4, "slave":1}}


    json_t *payload_json = json_object_get(root, "payload");

    if (!json_is_object(payload_json)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: payload not found in JSON request\n");
        json_decref(root);
        return E_GENERAL_FAILURE;
    }

    *payload = payload_json;

    return E_SUCCESS;
}


/**
 *
 * @param msg - IN
 * @param response_json OUT
 * @param request_id IN
 * @return
 */
gberror_t json_msg_request_respond_handle(char *msg, char *response_json, char *request_id) {


    int request_type;
    json_t *payload;
    gberror_t rc;
    char filename[MAX_JSON_MSG_FILENAME_LENGTH] = {0};
    // Static allocation of the buffer for file contents
    static char file_contents[JSON_MSG_MAX_FILE_SIZE] = {0};

    if (strnlen(msg, DPM_REQUEST_RESPONSE_DATA_LENGTH) < 5) {
        json_msg_response_error(request_id, 0, "Empty request", response_json);
        return E_SUCCESS;
    }

    rc = json_msg_request_extract(msg, &request_type, &payload);

    if (rc != E_SUCCESS) {
        json_msg_response_error(request_id, 0, "Failed to understand request", response_json);

    }

    uint32_t password;
    uint16_t slave;
    ec_datatype datatype;
    uint16_t index;
    uint8_t sub_index;
    uint8_t len;
    ec_value value;


    UM_INFO(GBEM_UM_EN, "GBEM: [JSON MSG] Request type received [%d]\n", request_type);


    switch (request_type) {
        case GBEM_REQUEST_SDO_READ:

            if (json_msg_parse_sdo_read_payload(payload, &slave, &datatype, &index, &sub_index, &len) != E_SUCCESS) {
                json_msg_response_error(request_id, request_type, "Failed to parse SDO read payload", response_json);
                return E_SUCCESS;
            }

            //read sdo
//            printf("len [%d]\n", len);
            rc = sdos_read_based_on_datatype(slave, index, sub_index, datatype, len, &value);


            if (rc != E_SUCCESS) {
                json_msg_response_error(request_id, request_type, "SDO read failed", response_json);
                return E_SUCCESS;
            }

            //respond
            json_msg_response_sdo_read(request_id, value, datatype, false, "SDO read success", response_json);


            break;
        case GBEM_REQUEST_SDO_WRITE:

            if (json_msg_parse_sdo_write_payload(payload, &slave, &datatype, &index, &sub_index, &len, &value) !=
                E_SUCCESS) {
                json_msg_response_error(request_id, request_type, "Failed to parse SDO write payload", response_json);
                return E_SUCCESS;
            }

//            rc = sdos_read_based_on_datatype(slave, index, sub_index, datatype, len, &value);

            rc = sdos_write_based_on_datatype(slave, index, sub_index, datatype, len, value);

            if (rc != E_SUCCESS) {
                json_msg_response_error(request_id, request_type, "SDO write failed", response_json);
                return E_SUCCESS;
            }

            //respond
            json_msg_response_sdo_write(request_id, false, "SDO read success", response_json);


            break;
        case GBEM_REQUEST_GET_VERSION:

            json_msg_response_get_version(request_id, response_json);

            break;

        case GBEM_REQUEST_GET_FILE:

            if (json_msg_parse_get_file_payload(payload, &slave, &password, filename) != E_SUCCESS) {
                json_msg_response_error(request_id, request_type, "Failed to parse get file payload", response_json);
                return E_SUCCESS;
            }

            rc = foe_read_file(filename, password, slave, file_contents, JSON_MSG_MAX_FILE_SIZE);


            if (rc != E_SUCCESS) {
                json_msg_response_error(request_id, request_type, "Get file read failed", response_json);
                return E_SUCCESS;
            }

            //respond
            json_msg_response_get_file(request_id, response_json, file_contents);


            break;

        default:
            UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: request_type not supported\n");
            json_msg_response_error(request_id, request_type, "request type not supported", response_json);
            return E_SUCCESS;
    }

    return E_SUCCESS;


}

