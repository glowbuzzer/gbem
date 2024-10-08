/**
 ******************************************************************************
 * @file           :  json_msg_reponse_get_version
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "json_msg_response_get_version.h"
#include "json_msg_response.h"
#include "shared_mem_types.h"
#include "gbem_ctx.h"

/**
 * @brief Get the version of the gbem and return it in the response json
 * @param request_id
 * @param response_json
 * @return
 */
gberror_t
json_msg_response_get_version(char *request_id, char *response_json) {

    json_t *response_payload = json_object();

    json_object_set(response_payload, "value", json_string(gbem_ctx.gbem_version));

    json_msg_response(request_id, GBEM_REQUEST_GET_VERSION, false, "success", response_payload, response_json);

    return E_SUCCESS;
}
