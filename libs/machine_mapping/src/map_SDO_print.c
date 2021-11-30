
/**
 ******************************************************************************
 * @file           :  map_SDO_print.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "map_SDO_print.h"
#include "map.h"
#include "user_message.h"

extern bool ec_printSDO;


/**
 * @brief enables global var ec_printSDO then calls all PDO mapping and SDO writing functions in the function table
 * @return gberror_t
 */
gberror_t map_print_sdos(void) {

    //call all functions in function table
    ec_printSDO = true;
    printf("***                     Starting output of SDO writes                        ***\n");
    printf("***                       PDO remapping SDO writes                           ***\n");
    for (uint16_t slave = 1; slave < MAP_NUM_SLAVES + 1; slave++) {

        if (*map_slave_pdo_map_function_ptr[slave - 1] != NULL) {
            (*map_slave_pdo_map_function_ptr[slave - 1])(slave);
        }

    }
    printf("***                           Standard SDO writes                            ***\n");
    for (uint16_t slave = 1; slave < MAP_NUM_SLAVES + 1; slave++) {
        if (*map_slave_standard_sdo_function_ptr[slave - 1] != NULL) {
            (*map_slave_standard_sdo_function_ptr[slave - 1])(slave);
        }
    }
    printf("***                             NVRAM SDO writes                             ***\n");
    for (uint16_t slave = 1; slave < MAP_NUM_SLAVES + 1; slave++) {
        if (*map_slave_nvram_sdo_function_ptr[slave - 1] != NULL) {
            (*map_slave_nvram_sdo_function_ptr[slave - 1])(slave);
        }
    }
    printf("***                      End of output of SDO writes                         ***\n");
    ec_printSDO = false;
    return E_SUCCESS;
}
