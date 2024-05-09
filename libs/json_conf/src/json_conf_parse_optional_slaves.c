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

#include "json_conf_parse_optional_slaves.h"
#include "json_conf_helpers.h"
#include <string.h>
#include "user_message.h"

bool json_conf_parse_optional_slaves(json_t *json_ethercat, uint16_t max_num_optional_slaves,
                                     machine_config_optional_slaves_t *optional_slaves) {

    json_t *ethercat_slave_object;
    json_t *slaves;
    json_t *optional;
    json_t *is_configurable;
    json_t *is_set;


    size_t size;
    size_t index1;
    size_t index2;
    char *key;

    memset(optional_slaves, 0, sizeof(machine_config_optional_slaves_t));


    ethercat_slave_object = json_object_get(json_ethercat, "slaves");
    if (!json_conf_check_object(ethercat_slave_object, "slaves")) {
        return false;
    }

    if (!json_is_array(ethercat_slave_object)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: slaves object not an array");
        return false;
    }

    size = json_array_size(ethercat_slave_object);

    UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] Success: Slave definitions found for [%zu] slaves", size);


    int num_of_optional_slaves_found = 0;

    json_array_foreach(ethercat_slave_object, index1, slaves) {
//        printf("index1: %lld\n", (long long) index1);

        optional = json_object_get(slaves, "optional");

        if (!json_conf_check_object(optional, "optional")) {
            return false;
        }


        is_configurable = json_object_get(optional, "is_configurable");
        if (!json_conf_check_object(is_configurable, "is_configurable")) {
            return false;
        }

        bool is_configurable_slave = false;

        if (json_is_boolean(is_configurable)) {
            is_configurable_slave = json_boolean_value(is_configurable);
        } else {
            UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: is_configurable object not a boolean");
            return false;
        }

        if (is_configurable_slave) {

            is_set = json_object_get(optional, "is_set");
            if (!json_conf_check_object(is_set, "is_set")) {
                return false;
            }

            if (json_is_boolean(is_set)) {
                optional_slaves->enable_optional_slave[index1] = json_boolean_value(is_set);

                if (json_boolean_value(is_set)) {
                    num_of_optional_slaves_found++;
                }

//todo fix me
            } else {
                UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: is_set not a boolean");
                return false;
            }

        }

    }
    if (num_of_optional_slaves_found > max_num_optional_slaves) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: too many optional slaves found [%d] should be [%d]",
                 num_of_optional_slaves_found, max_num_optional_slaves);
        return false;
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] Success: Optional slaves found [%d]", num_of_optional_slaves_found);
    }

    return true;

}