/**
 ******************************************************************************
 * @file           :  json_msg_reponse_get_file
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "json_msg_response_get_file.h"
#include "json_msg_response.h"
#include "shared_mem_types.h"
#include "gbem_ctx.h"


gberror_t
json_msg_response_get_file(char *request_id, char *response_json, char *file) {

    json_t *response_payload = json_object();

    json_object_set(response_payload, "value", json_string(file));

    json_msg_response(request_id, GBEM_REQUEST_GET_FILE, false, "success", response_payload, response_json);

    return E_SUCCESS;
}
