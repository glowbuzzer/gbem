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

#include "json_conf_parse.h"
#include "json_conf_parse_limits.h"
#include "json_conf_parse_sdo.h"
#include "json_conf_parse_optional_slaves.h"
#include "json_conf_parse_gbem.h"
#include "json_conf_helpers.h"
#include "json_conf_print.h"
#include "json_conf_check_sha256.h"
#include "json_conf_ethercat_helpers.h"
#include <openssl/sha.h>


bool
json_conf_parse(map_machine_limits_t *limits, machine_config_optional_slaves_t *optional_slaves, ec_sdo_array *ar,
                int *ecm_cycle_shift, int *cycle_time, int *drive_state_change_timeout, bool *disable_drive_warn_check,
                bool *disable_drive_limit_check, uint16_t num_drives, uint16_t max_num_optional_slaves) {


    json_error_t error;
    json_t *json_root = json_load_file(GBEM_CONF_FILE, 0, &error);
    json_t *value;

    if (!json_conf_check_sha256()) {
        fprintf(stderr, "GBEM: [JSON config] Error: SHA256 check failed\n");
        return false;
    }


    if (json_root) {
        fprintf(stderr, "GBEM: [JSON config] Success: Initial parse of the [%s]\n", GBEM_CONF_FILE);

        /* print JSON structure */
        print_json(json_root);

        //parse ethercat
        value = json_object_get(json_root, "ethercat");
        if (!json_conf_check_object(value, "ethercat")) {
            json_decref(json_root);
            return false;
        }

        //parse sdo
        if (json_conf_parse_sdo(value, ar)) {
            printf("GBEM: [JSON config] Success: SDOs parsed\n");
        } else {
            printf("GBEM: [JSON config] Error: SDO parse failed\n");
            json_decref(json_root);
            return false;
        }

        //parse optional slaves

        if (json_conf_parse_optional_slaves(value, max_num_optional_slaves, optional_slaves)) {
            printf("GBEM: [JSON config] Success: Optional slaves parsed\n");
        } else {
            printf("GBEM: [JSON config] Error: Optional slaves parse failed\n");
            json_decref(json_root);
            return false;
        }



        //parse drives
        value = json_object_get(json_root, "drives");
        if (!json_conf_check_object(value, "drives")) {
            json_decref(json_root);
            return false;
        }

        if (json_conf_parse_limits(value, num_drives, limits)) {
            printf("GBEM: [JSON config] Success: Limits parsed\n");
        } else {
            printf("GBEM: [JSON config] Error: Limits parse failed\n");
            json_decref(json_root);
            return false;
        }

        //parse gbem
        value = json_object_get(json_root, "gbem");
        if (!json_conf_check_object(value, "gbem")) {
            json_decref(json_root);
            return false;
        }

        if (json_conf_parse_gbem(value, cycle_time, drive_state_change_timeout, disable_drive_warn_check,
                                 disable_drive_limit_check)) {
            printf("GBEM: [JSON config] Success: GBEM settings parsed\n");
        } else {
            printf("GBEM: [JSON config] Error: GBEM settings parse failed\n");
            json_decref(json_root);
            return false;
        }


        json_decref(value);
        return true;

//        Obtaining a Value: When you retrieve a value from a JSON object using json_object_get() or similar functions (like in your case with value2), the returned value is borrowed. This means that the reference count of the value is not increased, and it remains managed by the parent object (root in your example).
//        No Need to json_decref Borrowed Values: Since value2 is a borrowed reference (obtained via json_object_get()), you do not need to json_decref(value2) in your loop or anywhere after accessing it. The lifecycle of value2 is tied to the lifecycle of the parent JSON object root.
//                Decrementing References Only for Owned References: You should only call json_decref on JSON objects (json_t*) that you own. Ownership is generally indicated by directly creating a JSON object (e.g., json_pack, json_array, json_object, json_string, json_integer, json_real, json_true, json_false, json_null) or increasing its reference count explicitly via json_incref.



    } else {
        fprintf(stderr, "GBEM: [JSON config] Error: Loading JSON file [%s] failed with an error on line [%d] [%s]\n",
                GBEM_CONF_FILE, error.line, error.text);
        return false;
    }
}