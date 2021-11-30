
/**
 ******************************************************************************
 * @file           :  float_int_conv.c
 * @brief          :  float_to_integer and integer_to_float utils
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include <stdint.h>

/**
 * @brief converts int32 to float with a bounded range
 * @param max_range
 * @param val to convert
 * @return float val
 */
float int32_to_float(const float max_range, const int32_t val) {
    return ((max_range) * ((float) val / (float) 0x7FFFFFFF));
}
/**
 * @brief converts uint32 to float with a bounded range
 * @param max_range
 * @param val to convert
 * @return float val
 */
float uint32_to_float(const float max_range, const uint32_t val) {
    return ((max_range) * ((float) val / (float) 0xFFFFFFFF));
}
/**
 * @brief converts int16 to float with a bounded range
 * @param max_range
 * @param val to convert
 * @return float val
 */
float int16_to_float(const float max_range, const int16_t val) {
    return ((max_range) * ((float) val / (float) 0x7FFF));
}

/**
 * @brief converts uint16 to float with a bounded range
 * @param max_range
 * @param val to convert
 * @return float val
 */
float uint16_to_float(const float max_range, const uint16_t val) {
    return ((max_range) * ((float) val / (float) 0xFFFF));
}

/**
 * @brief converts int8 to float with a bounded range
 * @param max_range
 * @param val to convert
 * @return float val
 */
float int8_to_float(const float max_range, const int8_t val) {
    return ((max_range) * ((float) val / (float) 0x7F));
}

/**
 * @brief converts uint8 to float with a bounded range
 * @param max_range
 * @param val to convert
 * @return float val
 */
float uint8_to_float(const float max_range, const uint8_t val) {
    return ((max_range) * ((float) val / (float) 0xFF));
}


/**
 * @brief
 * @param max_range
 * @param val to convert
 * @return integer val
 */
int32_t float_to_int32(const float max_range, const float val) {
    return (int32_t)((val / max_range) * ((float) 0x7FFFFFFF));
}

/**
 * @brief converts float to uint32 with a bounded range
 * @param max_range
 * @param val to convert
 * @return integer val
 */
uint32_t float_to_uint32(const float max_range, const float val) {
    return (uint32_t)((val / max_range) * ((float) 0xFFFFFFFF));
}

/**
 * @brief converts float to int16 with a bounded range
 * @param max_range
 * @param val to convert
 * @return integer val
 */
int16_t float_to_int16(const float max_range, const float val) {
    return (int16_t)((val / max_range) * ((float) 0x7FFF));
}

/**
 * @brief converts float to uint16 with a bounded range
 * @param max_range
 * @param val to convert
 * @return integer val
 */
uint16_t float_to_uint16(const float max_range, const float val) {
    return (uint16_t)((val / max_range) * ((float) 0xFFFF));
}

/**
 * @brief converts float to int8 with a bounded range
 * @param max_range
 * @param val to convert
 * @return integer val
 */
int8_t float_to_int8(const float max_range, const float val) {
    return (int8_t)((val / max_range) * ((float) 0x7F));
}

/**
 * @brief converts float to uint8 with a bounded range
 * @param max_range
 * @param val to convert
 * @return integer val
 */
uint8_t float_to_uint8(const float max_range, const float val) {
    return (uint8_t)((val / max_range) * ((float) 0xFF));
}

