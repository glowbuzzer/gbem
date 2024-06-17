/**
 ******************************************************************************
 * @file           :  json_msg_response
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "jansson.h"
#include "json_msg_response.h"
#include <string.h>
#include "json_msg.h"
#include "shared_mem_types.h"
#include "dpm.h"
#include "user_message.h"

//{
//response: {
//requestType: "get config" // not important to include, could be number
//error: true,
//message: "Error message here",
//// any other properties of the response (request type specific), eg:
//payload: {
//value: 100
//}
//}
//}



gberror_t
json_msg_response(char *request_id, int request_type, bool error, char *message, json_t *response_payload,
                  char *response_json) {

    json_t *response = json_object();
    json_t *responsebody = json_object();

    json_object_set(responsebody, "requestId", json_string(request_id));
    json_object_set(responsebody, "error", json_boolean(error));
    json_object_set(responsebody, "requestType", json_integer(request_type));
    json_object_set(responsebody, "message", json_string(message));

    if (response_payload != NULL) {
        json_object_set(responsebody, "payload", response_payload);
    }

    json_object_set(response, "response", responsebody);


    printf("JSON: [%s]\n", json_dumps(response, JSON_INDENT(2)));




    //num objects
//    printf("size [%zu]", json_object_size(response));


    size_t size = json_dumpb(responsebody, NULL, 0, 0);
    if (size > DPM_REQUEST_RESPONSE_DATA_LENGTH) {

        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: response size [%zu] overflow", size);


    } else {
        //todo debug remove
        printf("Response size to message request [%zu]", size);
    }

    strcpy(response_json, json_dumps(response, JSON_COMPACT));

    json_decref(response);
    json_decref(responsebody);
    json_decref(response_payload);


    return E_SUCCESS;
}


gberror_t
json_msg_response_sdo_read(char *request_id, ec_value value, ec_datatype datatype, bool error, char *message,
                           char *response_json) {

    if (!error) {
        json_t *response_payload = json_object();

        switch (datatype) {

            case ECT_BOOLEAN:
                json_object_set(response_payload, "value", json_boolean(value.boolean));
                break;
            case ECT_INTEGER8:
                json_object_set(response_payload, "value", json_integer(value.int8));
                break;
            case ECT_INTEGER16:
                json_object_set(response_payload, "value", json_integer(value.int16));
                break;
            case ECT_INTEGER32:
                json_object_set(response_payload, "value", json_integer(value.int32));
                break;
            case ECT_UNSIGNED8:
                json_object_set(response_payload, "value", json_integer(value.uint8));
                break;
            case ECT_UNSIGNED16:
                json_object_set(response_payload, "value", json_integer(value.uint16));
                break;
            case ECT_UNSIGNED32:
                json_object_set(response_payload, "value", json_integer(value.uint32));
                break;
            case ECT_REAL32:
                json_object_set(response_payload, "value", json_real(value.real32));
                break;
            case ECT_VISIBLE_STRING:
                json_object_set(response_payload, "value", json_string(value.visible_string));
                break;
            case ECT_OCTET_STRING:
                json_object_set(response_payload, "value", json_string((char *) value.octet_string));
                break;
            case ECT_UNICODE_STRING:
                json_object_set(response_payload, "value", json_string((char *) value.unicode_string));
                break;
            case ECT_TIME_OF_DAY:
                json_object_set(response_payload, "value", json_integer(value.time_of_day));
                break;
            case ECT_TIME_DIFFERENCE:
                json_object_set(response_payload, "value", json_integer(value.time_difference));
                break;
            case ECT_DOMAIN:
                json_object_set(response_payload, "value", json_string((char *) value.domain));
                break;
            case ECT_INTEGER24:
                json_object_set(response_payload, "value", json_integer(value.int24));
                break;
            case ECT_REAL64:
                json_object_set(response_payload, "value", json_real(value.real64));
                break;
            case ECT_INTEGER64:
                json_object_set(response_payload, "value", json_integer(value.int64));
                break;
            case ECT_UNSIGNED24:
                json_object_set(response_payload, "value", json_integer(value.uint24));
                break;
            case ECT_UNSIGNED64:
                json_object_set(response_payload, "value", json_integer(value.uint64));
                break;
            case ECT_BIT1:
                json_object_set(response_payload, "value", json_integer(value.bit));
                break;
            case ECT_BIT2:
                json_object_set(response_payload, "value", json_integer(value.bit));
                break;
            case ECT_BIT3:
                json_object_set(response_payload, "value", json_integer(value.bit));
                break;
            case ECT_BIT4:
                json_object_set(response_payload, "value", json_integer(value.bit));
                break;
            case ECT_BIT5:
                json_object_set(response_payload, "value", json_integer(value.bit));
                break;
            case ECT_BIT6:
                json_object_set(response_payload, "value", json_integer(value.bit));
                break;
            case ECT_BIT7:
                json_object_set(response_payload, "value", json_integer(value.bit));
                break;
            case ECT_BIT8:
                json_object_set(response_payload, "value", json_integer(value.bit));
                break;
            case ECT_STRING8:
                json_object_set(response_payload, "value", json_string(value.string8));
            default:
                //todo
//            LL_FATAL("Unknown datatype: %d", datatype);
                break;

        }

        json_msg_response(request_id, GBEM_REQUEST_SDO_READ, false, "success", response_payload, response_json);

    } else {
        return json_msg_response(request_id, GBEM_REQUEST_SDO_READ, true, message, NULL, response_json);
    }

    return E_SUCCESS;

}


gberror_t json_msg_response_error(char *request_id, int request_type, char *message, char *response_json) {
    json_msg_response(request_id, request_type, true, message, NULL, response_json);
    return E_SUCCESS;
}
