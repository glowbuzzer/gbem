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

#include "json_conf_parse_eep_name.h"
#include <string.h>
#include "user_message.h"

bool json_conf_parse_eep_name(json_t *json_ethercat, map_slave_map_t *slave_maps, size_t max_slaves) {

    json_t *ethercat_slave_object;
    json_t *slaves;
    json_t *eep_name_object;

    size_t index1;
    size_t size;
//    memset(optional_slaves, 0, sizeof(machine_config_optional_slaves_t));


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


    json_array_foreach(ethercat_slave_object, index1, slaves) {
//        printf("index1: %lld\n", (long long) index1);

        eep_name_object = json_object_get(slaves, "eep_name");

        if (!json_conf_check_object(eep_name_object, "ee_name")) {
            return false;
        }

        if (!json_is_string(eep_name_object)) {
            UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: eep_name object not a string");
            return false;
        }

        const char *eep_name = json_string_value(eep_name_object);

        // Ensure safe copying to the map_slave_map_t name field
        strncpy(slave_maps[index1].name, eep_name, EC_MAXNAME);

        // Null terminate the string
        slave_maps[index1].name[EC_MAXNAME] = '\0';


    }


    return true;

}