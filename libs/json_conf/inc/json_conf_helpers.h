/**
 ******************************************************************************
 * @file           :  json_conf_helpers
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef JANSSON_TEST_JSON_CONF_HELPERS_H
#define JANSSON_TEST_JSON_CONF_HELPERS_H

#include <stdbool.h>
#include "jansson.h"
#include "json_conf_ethercat_helpers.h"

bool json_conf_check_object(json_t *value, char *object_name);

int json_conf_match_key_extract_double(const char *key, char *key_name, double *act_value, json_t *json_value);

int json_conf_match_key_extract_int(const char *key, char *key_name, long long *act_value, json_t *json_value);

bool json_conf_match_key(const char *key, char *key_name);

bool json_conf_match_key_extract_ec_value(ec_datatype dt, const char *key, char *key_name, ec_value *act_value,
                                          json_t *json_value);

#endif //JANSSON_TEST_JSON_CONF_HELPERS_H
