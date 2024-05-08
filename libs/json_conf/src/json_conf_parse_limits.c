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

#include "json_conf_parse_limits.h"
#include "json_conf_helpers.h"


bool json_conf_parse_limits(json_t *json_drives, uint16_t num_drives, map_machine_limits_t *limits) {

    json_t *json_drives_limits;
    size_t index;
    json_t *value;
    const char *key;
    size_t size;

    json_drives_limits = json_object_get(json_drives, "limits");

    if (!json_conf_check_object(json_drives_limits, "limits")) {
        return false;
    }

    if (!json_is_array(json_drives_limits)) {
        printf("GBEM: [JSON config] Error: limits object not an array\n");
        return false;
    }


    size = json_array_size(json_drives_limits);

    printf("GBEM: [JSON config] Success: %zu limits objects found in array\n", size);

    if (size != num_drives) {
        printf("GBEM: [JSON config] Error: incorrect number of limits objects [%zu] should be [%d]\n", size,
               num_drives);
        return false;
    }

    json_array_foreach(json_drives_limits, index, value) {
//        printf("index: %lld\n", (long long) index);


//        int ju_rc=-1;
//        ju_rc=json_unpack(value, "{s:F, s:F, s:F, s:F, s:F, s:F}",
//                          "position_limit_max", &position_limit_max, "position_limit_min",
//                          &position_limit_min, "velocity_limit", &velocity_limit, "torque_limit",
//                          &torque_limit, "max_motor_speed", &max_motor_speed, "max_motor_torque", &max_motor_torque);
//
//        if (ju_rc == -1) {
//            fprintf(stderr, "error: json_unpack failed\n");
//            json_decref(json_root);
//            return 1;
//        }
//

        json_t *json_drives_limit_object;

        size = json_object_size(value);

        //check object size vs number expected

        if (size != 6) {
            printf("GBEM: [JSON config] Error: incorrect number of entries in limits [%zu] should be [6]\n", size);
            return false;
        }


        int true_count = 0;

        long long local_position_limit_max = 0;
        long long local_position_limit_min = 0;
        long long local_velocity_limit = 0;
        long long local_torque_limit = 0;
        long long local_max_motor_speed = 0;
        long long local_max_motor_torque = 0;


        json_object_foreach(value, key, json_drives_limit_object) {
            true_count += json_conf_match_key_extract_int(key, "position_limit_max", &local_position_limit_max,
                                                          json_drives_limit_object);
            true_count += json_conf_match_key_extract_int(key, "position_limit_min", &local_position_limit_min,
                                                          json_drives_limit_object);
            true_count += json_conf_match_key_extract_int(key, "velocity_limit", &local_velocity_limit,
                                                          json_drives_limit_object);
            true_count += json_conf_match_key_extract_int(key, "torque_limit", &local_torque_limit,
                                                          json_drives_limit_object);
            true_count += json_conf_match_key_extract_int(key, "max_motor_speed", &local_max_motor_speed,
                                                          json_drives_limit_object);
            true_count += json_conf_match_key_extract_int(key, "max_motor_torque", &local_max_motor_torque,
                                                          json_drives_limit_object);
        }

//        printf("position_limit_max: %lld\n", local_position_limit_max);
//        printf("position_limit_min: %lld\n", local_position_limit_min);
//        printf("velocity_limit: %lld\n", local_velocity_limit);
//        printf("torque_limit: %lld\n", local_torque_limit);
//        printf("max_motor_speed: %lld\n", local_max_motor_speed);
//        printf("max_motor_torque: %lld\n", local_max_motor_torque);

        if (true_count != 6) {
            printf("GBEM: [JSON config] Error: incorrect sections or names in limits object\n");
            return false;
        }


        limits[index].position_limit_max = (int32_t) local_position_limit_max;
        limits[index].position_limit_min = (int32_t) local_position_limit_min;
        limits[index].velocity_limit = (int32_t) local_velocity_limit;
        limits[index].torque_limit = (int32_t) local_torque_limit;
        limits[index].max_motor_speed = (int32_t) local_max_motor_speed;
        limits[index].max_motor_torque = (int32_t) local_max_motor_torque;


//        json_decref(json_drives_limit_object);

    }

//    json_decref(json_drives_limits);

    return true;
}
