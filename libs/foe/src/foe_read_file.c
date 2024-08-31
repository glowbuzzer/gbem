/**
 ******************************************************************************
 * @file           :  foe_read_file
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "gberror.h"
#include "foe_read_file.h"
#include "ethercatsetget.h"
#include "log.h"
#include "user_message.h"


#define SYNAPTICON_FOE_PASSWORD 0

//char *file_name = "logging_curr.log";


gberror_t foe_read_file(char *filename, uint32_t password, uint16_t slave, char *buffer, size_t buffer_size) {

    size_t bytes_read = 50000; //gets overwritten with bytes read
    char *file_buff = (char *) malloc(bytes_read);

    if (file_buff == NULL) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Memory allocation failure when creating buffer for log file download over FoE");
        return E_MALLOC;
    }

    int rc = ec_FOEread(slave + 1, filename, password, (int *) &bytes_read,
                        file_buff,
                        EC_TIMEOUTSTATE * 10);

    if (rc < 1) {
        free(file_buff);
        return E_GENERAL_FAILURE;
    }

    UM_INFO(GBEM_UM_EN, "GBEM: Bytes read over FoE on slave [%u] for filename [%s] is [%u]", slave, filename,
            bytes_read);

    if (bytes_read == 0) {
        free(file_buff);
        return E_NOT_FOUND;
    }

    // Determine the number of bytes to copy
    size_t copy_size = (bytes_read > buffer_size) ? buffer_size : bytes_read;


    // Log a warning if the file is truncated
    if (bytes_read > buffer_size) {
        UM_WARN(GBEM_UM_EN,
                "GBEM: File [%s] is truncated. Only the last [%zu] bytes are copied out of [%zu] bytes read.", filename,
                copy_size, bytes_read);
    }


    // Copy the last 'copy_size' bytes from 'file_buff' to 'buffer'
    memcpy(buffer, file_buff + bytes_read - copy_size, copy_size);


    free(file_buff);
    return E_SUCCESS;
}


