/**
 ******************************************************************************
 * @file           :  json_msg_parse_get_file_payload
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "json_msg_parse_get_file_payload.h"
#include "json_msg.h"


gberror_t
json_msg_parse_get_file_payload(json_t *get_file_payload, uint16_t *slave, uint32_t *password, char *filename) {

    json_t *obj = json_object_get(get_file_payload, "filename");

    if (!json_is_string(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: filename is not a string in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    const char *filename_str = json_string_value(obj);

    // Check if the filename length is less than 50 characters
    size_t length = strlen(filename_str);
    if (length >= MAX_JSON_MSG_FILENAME_LENGTH) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: filename exceeds maximum length of %d characters\n",
                 MAX_JSON_MSG_FILENAME_LENGTH - 1);
        return E_GENERAL_FAILURE;
    }


    // Copy the string value to the provided filename buffer
    strcpy(filename, filename_str);

    // Ensure null termination in case of a missing null terminator in the source (paranoia check)
    filename[MAX_JSON_MSG_FILENAME_LENGTH - 1] = '\0';


    obj = json_object_get(get_file_payload, "password");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: password is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    int64_t temp_password = json_integer_value(obj);

    // Check if the value fits within a 32-bit unsigned integer
    if (temp_password < 0 || temp_password > UINT32_MAX) {
        fprintf(stderr, "GBEM: [JSON MSG] Error: password value is out of range for a 32-bit unsigned integer\n");
        return E_GENERAL_FAILURE;
    }


    *password = (uint32_t) temp_password;


    obj = json_object_get(get_file_payload, "slave");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: slave is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *slave = (uint16_t) json_integer_value(obj);


    return E_SUCCESS;


}
