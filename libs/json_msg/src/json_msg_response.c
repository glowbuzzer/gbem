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
#include <stdbool.h>
#include "json_msg.h"

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



char *json_msg_response(json_msg_request_types_t request_type, bool error, char *message, json_t *response_payload) {

    json_t *response = json_object();

    json_object_set(response, "error", json_boolean(error));
    //todojson_object_set(response, "requestType", json_string(request_type));
    json_object_set(response, "message", json_string(message));
    json_object_set(response, "payload", response_payload);

    printf("JSON: [%s]\n", json_dumps(response, JSON_INDENT(2)));


    static char resp[200];

    //num objects
//    printf("size [%zu]", json_object_size(response));


    size_t size = json_dumpb(response, NULL, 0, 0);
    if (size > 199) {
        printf("size [%zu] overlfow", size);

//        write short error replay

        //todo
    } else {
        printf("size [%zu]", size);
    }

    strcpy(resp, json_dumps(response, JSON_COMPACT));

    json_decref(response);
    json_decref(response_payload);


    return resp;

}


char *json_msg_response_sdo_read(ec_value value, ec_datatype datatype) {
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
            json_object_set(response_payload, "value", json_integer(value.bit1));
            break;
        case ECT_BIT2:
            json_object_set(response_payload, "value", json_integer(value.bit1));
            break;
        case ECT_BIT3:
            json_object_set(response_payload, "value", json_integer(value.bit1));
            break;
        case ECT_BIT4:
            json_object_set(response_payload, "value", json_integer(value.bit1));
            break;
        case ECT_BIT5:
            json_object_set(response_payload, "value", json_integer(value.bit1));
            break;
        case ECT_BIT6:
            json_object_set(response_payload, "value", json_integer(value.bit1));
            break;
        case ECT_BIT7:
            json_object_set(response_payload, "value", json_integer(value.bit1));
            break;
        case ECT_BIT8:
            json_object_set(response_payload, "value", json_integer(value.bit1));
            break;
        case ECT_STRING8:
            json_object_set(response_payload, "value", json_string(value.string8));
        default:
            //todo
//            LL_FATAL("Unknown datatype: %d", datatype);
            break;

    }


    return json_msg_response(request_id, status, response_payload);

}