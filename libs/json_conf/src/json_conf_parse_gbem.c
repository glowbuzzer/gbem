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
#include "json_conf_parse_gbem.h"
#include "json_conf_helpers.h"
#include <string.h>

bool json_conf_parse_gbem(json_t *json_gbem, int *cycle_time, int *drive_state_change_timeout,
                          bool *disable_drive_warn_check,
                          bool *disable_drive_limit_check) {

    json_t *gbem_params_object;
    json_t *gbem_debug_params_object;
    json_t *cycle_time_object;
    json_t *drive_state_change_timeout_object;
    json_t *disable_drive_warn_check_object;
    json_t *disable_drive_limit_check_object;

    size_t size;
    size_t index1;

    char *key;

    *cycle_time = 0;
    *drive_state_change_timeout = 0;
    *disable_drive_warn_check = false;
    *disable_drive_limit_check = false;

    gbem_params_object = json_object_get(json_gbem, "params");
    if (!json_conf_check_object(gbem_params_object, "params")) {
        return false;
    }

    cycle_time_object = json_object_get(gbem_params_object, "cycle_time");
    if (!json_conf_check_object(cycle_time_object, "cycle_time")) {
        return false;
    }

    if (!json_is_integer(cycle_time_object)) {
        fprintf(stderr, "GBEM: [JSON config] Error: unhandled value type at key [cycle_time] - expected INTEGER\n");
        return false;
    } else {
        *cycle_time = (int) json_integer_value(cycle_time_object);
        //todo check if cycle_time is in range
    }

    drive_state_change_timeout_object = json_object_get(gbem_params_object, "drive_state_change_timeout");
    if (!json_conf_check_object(drive_state_change_timeout_object, "drive_state_change_timeout")) {
        return false;
    }

    if (!json_is_integer(drive_state_change_timeout_object)) {
        fprintf(stderr,
                "GBEM: [JSON config] Error: unhandled value type at key [drive_state_change_timeout] - expected INTEGER\n");
        return false;
    } else {
        *drive_state_change_timeout = (int) json_integer_value(drive_state_change_timeout_object);
        //todo check if drive_state_change_timeout is in range
    }


    gbem_debug_params_object = json_object_get(json_gbem, "debug_params");
    if (!json_conf_check_object(gbem_debug_params_object, "debug_params")) {
        return false;
    }
//    "debug_params": {
//        "disable_drive_warn_check": false,
//                "disable_drive_limit_check": false

    disable_drive_warn_check_object = json_object_get(gbem_debug_params_object, "disable_drive_warn_check");
    if (!json_conf_check_object(disable_drive_warn_check_object, "disable_drive_warn_check")) {
        return false;
    }

    if (!json_is_boolean(disable_drive_warn_check_object)) {
        fprintf(stderr,
                "GBEM: [JSON config] Error: unhandled value type at key [disable_drive_warn_check] - expected BOOLEAN\n");
        return false;
    } else {
        *disable_drive_warn_check = json_boolean_value(disable_drive_warn_check_object);
    }


    disable_drive_limit_check_object = json_object_get(gbem_debug_params_object, "disable_drive_limit_check");
    if (!json_conf_check_object(disable_drive_limit_check_object, "disable_drive_limit_check")) {
        return false;
    }


    if (!json_is_boolean(disable_drive_limit_check_object)) {
        fprintf(stderr,
                "GBEM: [JSON config] Error: unhandled value type at key [disable_drive_limit_check] - expected BOOLEAN\n");
        return false;
    } else {
        *disable_drive_limit_check = json_boolean_value(disable_drive_limit_check_object);
    }

    return true;

}