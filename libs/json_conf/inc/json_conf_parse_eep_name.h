/**
 ******************************************************************************
 * @file           :  json_conf_parse_eep_name
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_CONF_PARSE_EEP_NAME_H
#define GBEM_JSON_CONF_PARSE_EEP_NAME_H

#include "json_conf_parse_optional_slaves.h"
#include "json_conf_helpers.h"

bool json_conf_parse_eep_name(json_t *json_ethercat, map_slave_map_t *slave_maps, size_t max_slaves);

#endif //GBEM_JSON_CONF_PARSE_EEP_NAME_H
