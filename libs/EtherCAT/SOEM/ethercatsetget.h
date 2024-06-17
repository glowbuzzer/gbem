//
// Created by David on 21/09/2020.
//

#ifndef STATEMACHINE_ETHERCATSETGET_H
#define STATEMACHINE_ETHERCATSETGET_H


#include <stdint.h>
#include "ethercat.h"
#include <stdbool.h>


extern const char *ec_datatype_string[0x0037 + 2];

void ec_copy_between_slave_pdos(uint16_t src_slave_no, uint16_t dst_slave_no, uint8_t src_byte_no, uint8_t dst_byte_no,
                                uint8_t num_bytes);

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

bool ec_pdo_get_input_bit(uint16 slave_no, uint8 bit_no);

void ec_pdo_set_output_bit(uint16 slave_no, uint8 bit_no, bool value);

float ec_pdo_get_input_float(uint16_t slave_no, uint8_t byte_no);

void ec_pdo_set_output_float(uint16_t slave_no, uint8_t byte_no, float value);

bool ec_pdo_get_input_bit_from_byte_slave(uint16_t slave_no, uint8_t byte_no, uint8_t bit_no);

void ec_pdo_set_output_bit_from_byte_slave(uint16_t slave_no, uint8_t byte_no, uint8_t bit_no,
                                           bool val);


uint8_t ec_pdo_get_output_uint8_rev(uint16_t slave_no, uint8_t byte_no);

int8_t ec_pdo_get_output_int8_rev(uint16_t slave_no, uint8_t byte_no);

void ec_pdo_set_input_uint32_rev(uint16_t slave_no, uint8_t byte_no, uint32_t value);

void ec_pdo_set_input_int16_rev(uint16_t slave_no, uint8_t byte_no, int16_t value);

void ec_pdo_set_input_uint8(uint16_t slave_no, uint8_t byte_no, uint8_t value);

void ec_pdo_set_input_uint16_rev(uint16_t slave_no, uint8_t byte_no, uint16_t value);

void ec_pdo_set_input_int32_rev(uint16_t slave_no, uint8_t byte_no, int32_t value);

int32_t ec_pdo_get_output_int32_rev(uint16_t slave_no, uint8_t byte_no);

uint16_t ec_pdo_get_output_uint16_rev(uint16_t slave_no, uint8_t byte_no);

void ec_pdo_set_input_bit_rev(uint16_t slave_no, uint8_t bit_no, bool value);

void ec_pdo_set_input_int8_rev(uint16_t slave_no, uint8_t byte_no, int8_t value);

bool cs_pdo_get_output_bit_rev(uint16 slave_no, uint8_t bit_no);

uint32_t ec_pdo_get_output_uint32_rev(uint16_t slave_no, uint8_t byte_no);

int16_t ec_pdo_get_output_int16_rev(uint16_t slave_no, uint8_t byte_no);

void ec_pdo_set_input_float_rev(uint16_t slave_no, uint8_t byte_no, float value);

float ec_pdo_get_output_float_rev(uint16_t slave_no, uint8_t byte_no);


bool ec_sdo_write_uint8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t Value, bool umError);

bool ec_sdo_write_int8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int8_t Value, bool umError);

bool ec_sdo_write_uint16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint16_t Value, bool umError);

bool ec_sdo_write_int16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int16_t Value, bool umError);

bool ec_sdo_write_uint32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint32_t Value, bool umError);

bool ec_sdo_write_int32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int32_t Value, bool umError);

bool ec_sdo_write_string_n(uint16_t Slave, uint16_t Index, uint8_t SubIndex, const char *Value, size_t n, bool umError);

bool ec_sdo_write_real32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, float Value, bool umError);


bool
ec_sdo_read_real32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, float *return_val, bool umError);

bool ec_sdo_read_int32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int32_t *return_val, bool umError);

bool ec_sdo_read_uint32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint32_t *return_val, bool umError);

bool ec_sdo_read_int16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int16_t *return_val, bool umError);

bool ec_sdo_read_uint16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint16_t *return_val, bool umError);

bool ec_sdo_read_int8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int8_t *return_val, bool umError);

bool ec_sdo_read_uint8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t *return_val, bool umError);

/*
   * ec_sdo_write_bool
   * ec_sdo_write_real
   * ec_sdo_write_lreal
   * ec_sdo_write_byte_n
   * ec_sdo_write_uin64_t
   * ec_sdo_write_int64_t
   *
   * ec_sdo_read_bool
   * ec_sdo_read_real
   * ec_sdo_read_lreal
   * ec_sdo_read_byte_n
   * ec_sdo_read_string_n
   * ec_sdo_read_uint64_t
   * ec_sdo_read_int64_t
   */


//    {index: 0x0001, dataType: "BOOL"},
//    {index: 0x0002, dataType: "SINT", bitSize: 8},
//    {index: 0x0003, dataType: "INT", bitSize: 16},
//    {index: 0x0004, dataType: "DINT", bitSize: 32},
//    {index: 0x0005, dataType: "USINT", bitSize: 8},
//    {index: 0x0006, dataType: "UINT", bitSize: 16},
//    {index: 0x0007, dataType: "UDINT", bitSize: 32},
//    {index: 0x0008, dataType: "REAL", bitSize: 32},
//    {index: 0x0009, dataType: "STRING(n)"},//help
//    {index: 0x000A, dataType: "ARRAY [0..1] OF BYTE"},//help
//    {index: 0x000B, dataType: "UTF8_STRING(n)"},//help
//    {index: 0x000C, dataType: "TIME_OF_DAY"},
//    {index: 0x000D, dataType: "TIME_DIFFERENCE"},
//    {index: 0x000F, dataType: "DOMAIN"},
//    {index: 0x0010, dataType: "INT24", bitSize: 24},
//    {index: 0x0011, dataType: "LREAL", bitSize: 64},
//    {index: 0x0015, dataType: "LINT", bitSize: 64},
//    {index: 0x0016, dataType: "UINT24", bitSize: 24},
//    {index: 0x001B, dataType: "ULINT", bitSize: 64},
//    {index: 0x001E, dataType: "BYTE", bitSize: 8}, //extra from SOEM
//    {index: 0x0030, dataType: "BIT1", bitSize: 1},
//    {index: 0x0031, dataType: "BIT2", bitSize: 2},
//    {index: 0x0032, dataType: "BIT3", bitSize: 3},
//    {index: 0x0033, dataType: "BIT4", bitSize: 4},
//    {index: 0x0034, dataType: "BIT5", bitSize: 5},
//    {index: 0x0035, dataType: "BIT6", bitSize: 6},
//    {index: 0x0036, dataType: "BIT7", bitSize: 7},
//    {index: 0x0037, dataType: "BIT8", bitSize: 8}


extern bool ec_printSDO;

#endif //STATEMACHINE_ETHERCATSETGET_H
