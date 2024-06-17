/**
 ******************************************************************************
 * @file           :  sdos_read_based_on_datatype
 * @brief          :  read the sdo value based on the datatype
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
sdos_read_based_on_datatype(uint16_t slave, uint16_t index, uint8_t subindex, uint8_t datatype, int len,
                            ec_value *value);

#endif //GBEM_SDOS_READ_BASED_ON_DATATYPE_H
