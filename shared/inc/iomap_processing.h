/**
 ******************************************************************************
 * @file           :  iomap_processing.h
 * @brief          :  functions for interacting with the iomap
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_IOMAP_PROCESSING_H
#define GBEM_IOMAP_PROCESSING_H

#include "map.h"

bool iomap_get_pdo_in_bool(bool byte_slave, uint16_t slave_num, uint32_t byte_num, uint8_t bit_num);

int32_t iomap_get_pdo_in_int32(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num);

uint32_t iomap_get_pdo_in_uint32(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num);

float iomap_get_pdo_in_float(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num, float max_val);

void iomap_set_pdo_out_bool(bool byte_slave, uint16_t slave_num, uint32_t byte_num, uint8_t bit_num, bool val);

void iomap_set_pdo_out_int32(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num, int32_t val, float max_val);

void iomap_set_pdo_out_uint32(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num, uint32_t val, float max_val);

void iomap_set_pdo_out_float(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num, float val, float max_val);

void iomap_set_pdo_out_union(uint16_t slave_num, uint32_t byte_num, ec_datatype pdo_type, ec_datatype_value_t val);

void iomap_set_gbc_in_union(uint16_t io_num, ec_datatype gbc_type, ec_datatype_value_t val);

void iomap_set_plc_in_union(ec_datatype plc_type, void *io, ec_datatype_value_t val);

void iomap_set_gbc_digital_in_from_pdo(bool byte_slave, uint16_t slave_num, uint32_t byte_num, uint8_t bit_num,
                                       uint16_t gbc_io_id, map_gbc_io_type_t type);

void iomap_set_gbc_in_int32_from_pdo(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num, uint16_t gbc_io_id,
                                     float max_val);

void iomap_set_gbc_uint32_in_from_pdo(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num, uint16_t gbc_io_id,
                                      float max_val);

void iomap_set_gbc_float_in_from_pdo(ec_datatype pdo_type, uint16_t slave_num, uint32_t byte_num, uint16_t gbc_io_id,
                                     float max_val);


#endif //GBEM_IOMAP_PROCESSING_H
