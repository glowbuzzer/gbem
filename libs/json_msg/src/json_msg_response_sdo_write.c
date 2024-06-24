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

#include "json_msg_response_sdo_read.h"

#include <string.h>

#include "shared_mem_types.h"


gberror_t
json_msg_response_sdo_write(char *request_id, bool error, char *message,
                            char *response_json) {

    if (!error) {
//        json_t *response_payload = json_object();

//        switch (datatype) {
//
//            case ECT_BOOLEAN:
//                json_object_set(response_payload, "value", json_boolean(value.boolean));



        json_msg_response(request_id, GBEM_REQUEST_SDO_WRITE, false, "success", NULL, response_json);

    } else {
        return json_msg_response(request_id, GBEM_REQUEST_SDO_WRITE, true, message, NULL, response_json);
    }

    return E_SUCCESS;

}
