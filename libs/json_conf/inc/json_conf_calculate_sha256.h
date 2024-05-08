/**
 ******************************************************************************
 * @file           :  json_conf_calculate_sha256
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef JANSSON_TEST_JSON_CONF_CALCULATE_SHA256_H
#define JANSSON_TEST_JSON_CONF_CALCULATE_SHA256_H

#include <stdbool.h>
#include <stdio.h>

bool json_conf_calculate_sha256(FILE *file, unsigned char *hash);

#endif //JANSSON_TEST_JSON_CONF_CALCULATE_SHA256_H
