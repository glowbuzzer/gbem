/**
 ******************************************************************************
 * @file           :  sdos_write_from_array
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "sdo_write_from_array.h"


gberror_t sdos_write_all_from_array(ec_sdo_array *array) {

    //todo crit = slave should be plus 1 if we are zero indexing array
    for (uint8_t i = 0; i < array->num_slaves; i++) {

        for (uint8_t j = 0; j < array->num_sdo[i]; j++) {


            if (sdos_write_based_on_datatype(i, array->sdo[i][j].index, array->sdo[i][j].subindex,
                                             array->sdo[i][j].datatype, array->sdo[i][j].len, array->sdo[i][j].value) !=
                E_SUCCESS) {
                return E_SDO_WRITE_FAILURE;
            }


        }
    }

    return E_SUCCESS;

}

gberror_t sdos_write_one_from_array(ec_sdo_array *array, uint8_t slave) {
    // Check if the slave_index is valid
    if (slave >= array->num_slaves + 1) {
        return E_TOO_LONG;
    }

    for (uint8_t j = 0; j < array->num_sdo[slave - 1]; j++) {

        if (sdos_write_based_on_datatype(slave, array->sdo[slave - 1][j].index, array->sdo[slave - 1][j].subindex,
                                         array->sdo[slave - 1][j].datatype, array->sdo[slave - 1][j].len,
                                         array->sdo[slave - 1][j].value) !=
            E_SUCCESS) {
            return E_SDO_WRITE_FAILURE;
        }

    }

    return E_SUCCESS;
}