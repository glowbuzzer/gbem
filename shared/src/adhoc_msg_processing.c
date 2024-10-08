/**
 ******************************************************************************
 * @file           :  adhoc_msg_processing
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "dpm.h"
#include <string.h>
#include "linux_shm.h"
#include "gbem_ctx.h"
#include "json_msg_request.h"
#include "user_message.h"

static uint8_t last_request_id[DPM_REQUEST_ID_LENGTH] = {0};


void adhoc_msg_processing(void) {
    uint8_t *request_id;
    dpm_offline_in_t *input_struct = (dpm_offline_in_t *) gbem_ctx.shmp->sm_offline_buf_in;
    dpm_offline_out_t *output_struct = (dpm_offline_out_t *) gbem_ctx.shmp->sm_offline_buf_out;

    request_id = output_struct->ecm_request.request_id;

    // Ensure request_id is valid and null-terminated
    if (request_id[DPM_REQUEST_ID_LENGTH - 1] != '\0') {
        request_id[DPM_REQUEST_ID_LENGTH - 1] = '\0'; // Safeguard null termination
    }

    if (*request_id == '\0') {
        return;
    }

    //if request id is different from last request id - we have a msg req to process
    if (strncmp((char *) request_id, (char *) last_request_id, DPM_REQUEST_ID_LENGTH) == 0) {
        return; // No new request to process
    }

    //copy request id to last request id
    strncpy(last_request_id, request_id, DPM_REQUEST_ID_LENGTH);
    last_request_id[DPM_REQUEST_ID_LENGTH - 1] = '\0'; // Ensure null termination
    //call json_msg_request

    gberror_t rc = E_GENERAL_FAILURE;

//    memcpy(input_struct->ecm_response.request_id, request_id, DPM_REQUEST_ID_LENGTH);
    strncpy(input_struct->ecm_response.request_id, request_id, DPM_REQUEST_ID_LENGTH);


    UM_INFO(GBEM_UM_EN, "GBEM: [ADHOC MSG] Request ID [%s]", request_id);
    last_request_id[DPM_REQUEST_ID_LENGTH - 1] = '\0'; // Ensure null termination


    memset(input_struct->ecm_response.data, 0, DPM_REQUEST_RESPONSE_DATA_LENGTH);
    rc = json_msg_request_respond_handle((char *) output_struct->ecm_request.data, input_struct->ecm_response.data,
                                         request_id);

    if (rc != E_SUCCESS) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: json_msg_request_respond_handle failed\n");
    }

}