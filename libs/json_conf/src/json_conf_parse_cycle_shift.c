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

#include "json_conf_parse_cycle_shift.h"
#include <string.h>
#include "user_message.h"

bool json_conf_parse_cycle_shift(json_t *json_ethercat, int32_t *cycle_shift) {


    json_t *cycle_shift_object;

    size_t index1;
    size_t size;

    *cycle_shift = 0;


    cycle_shift_object = json_object_get(json_ethercat, "ecm_cycle_shift");
    if (!json_conf_check_object(cycle_shift_object, "ecm_cycle_shift")) {
        return false;
    }

    UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] Success: ec_cycle_shift object found");

    if (!json_is_integer(cycle_shift_object)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: ec_cycle_shift object not an integer");
        return false;
    }

    *cycle_shift = json_integer_value(cycle_shift_object);

    return true;

}