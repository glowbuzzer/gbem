/**
 ******************************************************************************
 * @file           :  json_conf_parse_optional_slaves
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef JANSSON_TEST_JSON_CONF_PARSE_OPTIONAL_SLAVES_H
#define JANSSON_TEST_JSON_CONF_PARSE_OPTIONAL_SLAVES_H

#include "json_conf_ethercat_helpers.h"
#include "jansson.h"
#include "map.h"

bool json_conf_parse_optional_slaves(json_t *json_drives, uint16_t max_num_optional_slaves,
                                     machine_config_optional_slaves_t *optional_slaves);


#endif //JANSSON_TEST_JSON_CONF_PARSE_OPTIONAL_SLAVES_H
