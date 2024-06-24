
/**
 ******************************************************************************
 * @file           :  nvram.c
 * @brief          :  functions to write SDOs and then trigger slave NVRAM write
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "gberror.h"
#include "ec_functions.h"
#include "user_message.h"
#include <stdio.h>
#include <ctype.h>
#include "map.h"
#include "main.h"
#include <stdlib.h>

static gberror_t nvram_check_for_function(uint16_t slave);

static gberror_t nvram_step_3(uint16_t slave);

static gberror_t nvram_step_2(uint16_t slave);


/**
 * @brief inits the EtherCAT network then calls the function to write sdos and then trigger the nvram write
 * @return
 */
gberror_t nvram_step_1(void) {
    gberror_t grc;
    bool nvram_boot_proceed = ec_step_1_init();
    /* now let's try and Enumerate and init all slaves.*/
    if (nvram_boot_proceed) {
        nvram_boot_proceed = ec_step_2_config();
    } else {
        UM_FATAL("GBEM: We have failed to initialise the network interface. This is terminal.");
    }

    /* now let's try and Enumerate and init all slaves.*/
    if (nvram_boot_proceed) {
        nvram_boot_proceed = ec_step_2_config();
    } else {
        UM_FATAL(
                "GBEM: We have failed to enumerate and initialise the slaves on the EtherCAT network. This is terminal.");
    }

    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                          Write NVRAM SDOs                          ***");
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");

    grc = nvram_step_2(slave_to_write_nvram);
    if (grc != E_SUCCESS) {
        UM_ERROR(GBEM_UM_EN, "GBEM: SDO write and NVRAM write FAILED, error [%s] GBEM will exit\n", gb_strerror(grc));
        exit(EXIT_FAILURE);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: SDO write and NVRAM write was successful, GBEM will exit\n");
        exit(EXIT_SUCCESS);
    }
}


static gberror_t nvram_check_for_function(const uint16_t slave) {

    if (map_slave_nvram_sdo_function_ptr[slave - 1] == NULL) {
        return E_NO_FUNCTION_FOUND;
    }
    return E_SUCCESS;
}

static gberror_t nvram_step_3(const uint16_t slave) {
    gberror_t grc;
    if (map_slave_nvram_sdo_function_ptr[slave - 1] != NULL) {
        grc = (*map_slave_nvram_sdo_function_ptr[slave - 1])(slave);
        if (grc == E_SUCCESS) {
            UM_INFO(GBEM_UM_EN, "GBEM: Applying nvram SDOs succeeded for slave [%u]", slave);
            return E_SUCCESS;
        } else if (grc == E_SDO_WRITE_FAILURE) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Applying NVRAM SDOs failed for slave [%u]. Error [%s]", slave,
                     gb_strerror(grc));
            return E_GENERAL_FAILURE;
        } else if (grc == E_NVRAM_WRITE_FAILURE) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Applying NVRAM SDOs failed for slave [%u]. Error [%s]", slave,
                     gb_strerror(grc));
            return E_GENERAL_FAILURE;
        } else {
            UM_ERROR(GBEM_UM_EN, "GBEM: Applying NVRAM SDOs failed for slave [%u]", slave);
            return E_GENERAL_FAILURE;
        }
    } else {
        return E_NO_FUNCTION_FOUND;
    }
}


static gberror_t nvram_step_2(const uint16_t slave) {
    gberror_t grc;

    UM_INFO(GBEM_UM_EN, "GBEM: ***         THIS WILL WRITE THE SET SDOS TO THE DRIVE'S NVRAM          ***");
    UM_INFO(GBEM_UM_EN, "GBEM: This will write values to slave [%u] only\n", slave);

    grc = nvram_check_for_function(slave);

    if (grc == E_NO_FUNCTION_FOUND) {
        UM_ERROR(GBEM_UM_EN,
                 "GBEM: No function is available to write SDOs to NVRAM to this slave. It could be that the slave does not support this type of operation or that no function has been written yet\n");
        return E_NO_FUNCTION_FOUND;
    } else if (grc != E_SUCCESS) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Unknown error checking NVRAM write function\n");
        return E_UNKNOWN_ERROR;
    }

    printf("Please enter Y to continue this operation or anything else to exit the program:\n");
    int response;
    response = getchar();
    if (toupper(response) != 'Y') {
        printf("You have decided to not write to a slave's NVRAM \n");
        return E_USER_CANCELLED_OPERATION;
    }

    printf("Starting write to slave's NVRAM\n");

    grc = nvram_step_3(slave);
    if (grc == E_SUCCESS) {
        return E_SUCCESS;
    } else {
        return E_OPERATION_FAILED;
    }
}