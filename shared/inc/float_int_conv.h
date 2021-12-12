/**
 ******************************************************************************
 * @file           :  float_int_conv.h
 * @brief          :  float_to_integer and integer_to_float utils
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_FLOAT_INT_CONV_H
#define GBEM_FLOAT_INT_CONV_H

float int32_to_float(float max_range, int32_t val);
float uint32_to_float(float max_range, uint32_t val);
float int16_to_float(float max_range, int16_t val);
float uint16_to_float(float max_range, uint16_t val);
float int8_to_float(float max_range, int8_t val);
float uint8_to_float(float max_range, uint8_t val);

int32_t float_to_int32(float max_range, float val);
uint32_t float_to_uint32(float max_range, float val);
int16_t float_to_int16(float max_range, float val);
uint16_t float_to_uint16(float max_range, float val);
int8_t float_to_int8(float max_range, float val);
uint8_t float_to_uint8(float max_range, float val);


#endif //GBEM_FLOAT_INT_CONV_H
