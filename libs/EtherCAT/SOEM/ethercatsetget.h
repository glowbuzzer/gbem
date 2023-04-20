//
// Created by David on 21/09/2020.
//

#ifndef STATEMACHINE_ETHERCATSETGET_H
#define STATEMACHINE_ETHERCATSETGET_H




#include <stdint.h>
#include "ethercat.h"
#include <stdbool.h>

extern const char * ec_datatype_string[0x0037+2];

void ec_pdo_set_output_int8(uint16_t slave_no, uint8_t byte_no, int8_t value);
void ec_pdo_set_output_uint8(uint16_t slave_no, uint8_t byte_no, uint8_t value);
void ec_pdo_set_output_uint16(uint16_t slave_no, uint8_t byte_no, uint16_t value);
void ec_pdo_set_output_int16(uint16_t slave_no, uint8_t byte_no, int16_t value);
void ec_pdo_set_output_uint32(uint16_t slave_no, uint8_t byte_no, uint32_t value);
void ec_pdo_set_output_int32(uint16_t slave_no, uint8_t byte_no, int32_t value);
uint32_t ec_pdo_get_input_uint32(uint16_t slave_no, uint8_t byte_no);
int32_t ec_pdo_get_input_int32(uint16_t slave_no, uint8_t byte_no);
uint16_t ec_pdo_get_input_uint16(uint16_t slave_no, uint8_t byte_no);
int16_t ec_pdo_get_input_int16(uint16_t slave_no, uint8_t byte_no);
uint8_t ec_pdo_get_input_uint8(uint16_t slave_no, uint8_t byte_no);
int8_t ec_pdo_get_input_int8(uint16_t slave_no, uint8_t byte_no);
bool ec_pdo_get_input_bit (uint16 slave_no, uint8 bit_no);
void ec_pdo_set_output_bit (uint16 slave_no, uint8 bit_no, bool value);
float ec_pdo_get_input_float(uint16_t slave_no, uint8_t byte_no);
void ec_pdo_set_output_float(uint16_t slave_no, uint8_t byte_no, float value);

bool ec_pdo_get_input_bit_from_byte_slave(uint16_t slave_no, ec_datatype_t datatype, uint8_t byte_no, uint8_t bit_no);
void ec_pdo_set_output_bit_from_byte_slave(uint16_t slave_no, ec_datatype_t datatype, uint8_t byte_no, uint8_t bit_no, bool val);


uint8_t ec_pdo_get_output_uint8_rev(uint16_t slave_no, uint8_t byte_no);
int8_t ec_pdo_get_output_int8_rev(uint16_t slave_no, uint8_t byte_no);
void ec_pdo_set_input_uint32_rev(uint16_t slave_no, uint8_t byte_no, uint32_t value);
void ec_pdo_set_input_int16_rev(uint16_t slave_no, uint8_t byte_no, int16_t value);
void ec_pdo_set_input_uint8(uint16_t slave_no, uint8_t byte_no, uint8_t value);
void ec_pdo_set_input_uint16_rev(uint16_t slave_no, uint8_t byte_no, uint16_t value);
void ec_pdo_set_input_int32_rev(uint16_t slave_no, uint8_t byte_no, int32_t value);
int32_t ec_pdo_get_output_int32_rev(uint16_t slave_no, uint8_t byte_no);
uint16_t ec_pdo_get_output_uint16_rev(uint16_t slave_no, uint8_t byte_no);
void ec_pdo_set_input_bit_rev (uint16_t slave_no, uint8_t bit_no, bool value);
void ec_pdo_set_input_int8_rev(uint16_t slave_no, uint8_t byte_no, int8_t value);
bool cs_pdo_get_output_bit_rev (uint16 slave_no, uint8_t bit_no);
uint32_t ec_pdo_get_output_uint32_rev(uint16_t slave_no, uint8_t byte_no);
int16_t ec_pdo_get_output_int16_rev(uint16_t slave_no, uint8_t byte_no);
void ec_pdo_set_input_float_rev(uint16_t slave_no, uint8_t byte_no, float value);
float ec_pdo_get_output_float_rev(uint16_t slave_no, uint8_t byte_no);


bool ec_sdo_write_uint8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t Value);
bool ec_sdo_write_int8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int8_t Value);
bool ec_sdo_write_uint16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint16_t Value);
bool ec_sdo_write_int16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int16_t Value);
bool ec_sdo_write_uint32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint32_t Value);
bool ec_sdo_write_int32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int32_t Value);

bool ec_sdo_read_int32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int32_t * return_val);
bool ec_sdo_read_uint32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint32_t * return_val);
bool ec_sdo_read_int16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int16_t * return_val);
bool ec_sdo_read_uint16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint16_t * return_val);
bool ec_sdo_read_int8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int8_t * return_val);
bool ec_sdo_read_uint8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t * return_val);


extern bool ec_printSDO;

#endif //STATEMACHINE_ETHERCATSETGET_H
