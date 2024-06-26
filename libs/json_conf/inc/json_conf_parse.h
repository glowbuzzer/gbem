/**
 ******************************************************************************
 * @file           :  json_conf_parse
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef JANSSON_TEST_JSON_CONF_PARSE_H
#define JANSSON_TEST_JSON_CONF_PARSE_H

#include <stdbool.h>
#include <jansson.h>
#include "json_conf_ethercat_helpers.h"
#include "map.h"
#include "ethercattype.h"


bool
json_conf_parse(map_machine_limits_t *limits, bool *no_limits, machine_config_optional_slaves_t *optional_slaves,
                ec_sdo_array *ar,
                int *ecm_cycle_shift, int *cycle_time, int *drive_state_change_timeout, bool *disable_drive_warn_check,
                bool *disable_drive_limit_check, uint16_t num_drives, uint16_t max_num_optional_slaves,
                map_slave_map_t *slave_eep, size_t max_slaves);

#endif //JANSSON_TEST_JSON_CONF_PARSE_H
