/**
 ******************************************************************************
 * @file           :  json_conf_parse_cycle_shift
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_JSON_CONF_PARSE_CYCLE_SHIFT_H
#define GBEM_JSON_CONF_PARSE_CYCLE_SHIFT_H

#include "json_conf_parse_optional_slaves.h"
#include "json_conf_helpers.h"

bool json_conf_parse_cycle_shift(json_t *json_ethercat, int32_t *cycle_shift);

#endif //GBEM_JSON_CONF_PARSE_CYCLE_SHIFT_H
