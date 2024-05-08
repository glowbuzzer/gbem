/**
 ******************************************************************************
 * @file           :  json_conf_number_checks
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef JANSSON_TEST_JSON_CONF_NUMBER_CHECKS_H
#define JANSSON_TEST_JSON_CONF_NUMBER_CHECKS_H

#include <stdbool.h>
#include <limits.h>
#include <stdint.h>

bool json_conf_check_integer_fits_uint32(long long int value);
bool json_conf_check_integer_fits_int32(long long int value);

bool json_conf_check_integer_fits_uint16(long long int value);
bool json_conf_check_integer_fits_int16(long long int value);

bool json_conf_check_integer_fits_uint8(long long int value);
bool json_conf_check_integer_fits_int8(long long int value);


#endif //JANSSON_TEST_JSON_CONF_NUMBER_CHECKS_H
