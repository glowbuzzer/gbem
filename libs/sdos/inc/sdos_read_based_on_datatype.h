/**
 ******************************************************************************
 * @file           :  sdos_read_based_on_datatype
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_SDOS_READ_BASED_ON_DATATYPE_H
#define GBEM_SDOS_READ_BASED_ON_DATATYPE_H


gberror_t
sdos_read_based_on_datatype(const uint16_t slave, const uint16_t index, const uint8_t subindex, const uint8_t datatype,
                            ec_value *value);

#endif //GBEM_SDOS_READ_BASED_ON_DATATYPE_H
