/**
 ******************************************************************************
 * @file           :  json_conf_parse_limits
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef JANSSON_TEST_JSON_CONF_PARSE_LIMITS_H
#define JANSSON_TEST_JSON_CONF_PARSE_LIMITS_H

#include <stdbool.h>
#include "jansson.h"
#include "json_conf_ethercat_helpers.h"
#include "map.h"

bool json_conf_parse_limits(json_t *json_drives, uint16_t num_drives, map_machine_limits_t *limits);

#endif //JANSSON_TEST_JSON_CONF_PARSE_LIMITS_H
