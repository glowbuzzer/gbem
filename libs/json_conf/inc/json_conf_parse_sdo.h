/**
 ******************************************************************************
 * @file           :  json_conf_parse_sdo
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef JANSSON_TEST_JSON_CONF_PARSE_SDO_H
#define JANSSON_TEST_JSON_CONF_PARSE_SDO_H

#include <stdbool.h>
#include "jansson.h"
#include "json_conf_ethercat_helpers.h"

bool json_conf_parse_sdo(json_t *json_ethercat, ec_sdo_array *ar);

#endif //JANSSON_TEST_JSON_CONF_PARSE_SDO_H
