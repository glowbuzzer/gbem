/**
 ******************************************************************************
 * @file           :  json_conf_parse_gbem
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef JANSSON_TEST_JSON_CONF_PARSE_GBEM_H
#define JANSSON_TEST_JSON_CONF_PARSE_GBEM_H

#include "json_conf_ethercat_helpers.h"
#include "jansson.h"

bool json_conf_parse_gbem(json_t *json_gbem, int *cycle_time, int *drive_state_change_timeout,
                          bool *disable_drive_warn_check,
                          bool *disable_drive_limit_check);

#endif //JANSSON_TEST_JSON_CONF_PARSE_GBEM_H
