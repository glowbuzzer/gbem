/**
 ******************************************************************************
 * @file           :  sdo_write_from_array
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_SDO_WRITE_FROM_ARRAY_H
#define GBEM_SDO_WRITE_FROM_ARRAY_H

#include "ethercattype.h"
#include "sdos_write_based_on_datatype.h"
#include "gberror.h"

gberror_t sdos_write_from_array(ec_sdo_array *array);

#endif //GBEM_SDO_WRITE_FROM_ARRAY_H
