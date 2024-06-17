/**
 ******************************************************************************
 * @file           :  json_conf_check_sha256
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "json_conf_check_sha256.h"
#include "json_conf_calculate_sha256.h"
#include "json_conf_parse.h"
#include <openssl/sha.h>
#include <string.h>
#include "user_message.h"
#include <stdio.h>
#include "gbem_config.h"

bool json_conf_check_sha256(void) {
    FILE *file;
    file = fopen(GBEM_CONF_SHA256_FILE, "rb");
    if (!file) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: SHA256 file not found [%s]", GBEM_CONF_SHA256_FILE);
        perror("Error opening file");
        return false;
    }

    char buffer[1024];  // Define a buffer to hold the line

    if (fgets(buffer, sizeof(buffer), file)) {
        printf("First line: %s", buffer);
    } else {
        // Handle the case where fgets fails, e.g., empty file
        if (feof(file)) {
            printf("The file is empty.\n");
        } else {
            perror("Failed to read the line");
        }
    }

    //todo check size of buffer


    fclose(file);  // Close the file


    file = fopen(GBEM_CONF_FILE, "rb");
    if (!file) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: JSON file not found [%s]", GBEM_CONF_FILE);
        perror("Error opening file");
        return false;
    }

    unsigned char sha256[SHA256_DIGEST_LENGTH];

    if (json_conf_calculate_sha256(file, sha256)) {
        UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] Success: SHA256 calculated");
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: SHA256 calculation failed");
        return false;
    }

    fclose(file);


    if (strcmp(buffer, sha256) == 0) {
        UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] Success: SHA256 match");
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: SHA256 mismatch");
        return false;
    }

    return true;
}