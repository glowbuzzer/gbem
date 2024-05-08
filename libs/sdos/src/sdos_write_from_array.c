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



gberror_t sdos_write_from_array(ec_sdo_array *array) {

    for (uint8_t i = 0; i < array->num_slaves; i++) {

        for (uint8_t j = 0; j < array->num_sdo[i]; j++) {


            if (sdos_write_based_on_datatype(i, array->sdo[i][j].index, array->sdo[i][j].subindex,
                                             array->sdo[i][j].datatype, array->sdo[i][j].value) != E_SUCCESS) {
                return E_SDO_WRITE_FAILURE;
            }


        }
    }

    return E_SUCCESS;

}