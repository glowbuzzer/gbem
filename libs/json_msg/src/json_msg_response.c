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

#if defined(JSON_MSG_PRINT_CONTENTS) && JSON_MSG_PRINT_CONTENTS == 1
    printf("JSON: [%s]\n", json_dumps(response, JSON_INDENT(2)));
#endif



    //num objects
//    printf("size [%zu]", json_object_size(response));


    size_t size = json_dumpb(responsebody, NULL, 0, 0);
    if (size > DPM_REQUEST_RESPONSE_DATA_LENGTH) {

        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: response size [%zu] overflow", size);


    } else {
        //todo debug remove
        UM_INFO(GBEM_UM_EN, "GBEM: [JSON MSG] Response size to message request [%zu]", size);

    }

    strcpy(response_json, json_dumps(response, JSON_COMPACT));


    json_decref(response);
    json_decref(responsebody);
    json_decref(response_payload);


    return E_SUCCESS;
}


gberror_t json_msg_response_error(char *request_id, int request_type, char *message, char *response_json) {
    json_msg_response(request_id, request_type, true, message, NULL, response_json);
    return E_SUCCESS;
}
