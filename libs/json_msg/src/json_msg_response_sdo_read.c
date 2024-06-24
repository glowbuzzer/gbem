/**
 ******************************************************************************
 * @file           :  json_msg_response_sdo_read
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "json_msg_response_sdo_read.h"

#include <string.h>

#include "shared_mem_types.h"


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
