/**
 ******************************************************************************
 * @file           :  json_conf_helpers
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "json_conf_helpers.h"
#include <string.h>
#include "user_message.h"

bool json_conf_check_object(json_t *value, char *object_name) {
    if (value == NULL) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: [%s] object not found", object_name);
        return false;
    }

    UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] Success: [%s] object found", object_name);
    return true;
}


int json_conf_match_key_extract_int(const char *key, char *key_name, long long *act_value, json_t *json_value) {

    if (strcmp(key, key_name) == 0) {

        if (json_is_integer(json_value)) {
            *act_value = json_integer_value(json_value);
//            printf("INTEGER key: %s, value: %lld\n", key, json_integer_value(json_value));
            return 1;

        } else {
            UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: unhandled value type at key [%s] - expected INTEGER", key);
            return 0;
        }

    }

    return 0;
}


int json_conf_match_key_extract_double(const char *key, char *key_name, double *act_value, json_t *json_value) {

    if (strcmp(key, key_name) == 0) {

        if (json_is_integer(json_value)) {
            *act_value = (double) json_integer_value(json_value);
//            printf("INTEGER key: %s, value: %lld\n", key, json_integer_value(json_value));
            return 1;

        } else if (json_is_real(json_value)) {
            *act_value = json_real_value(json_value);
//            printf("REAL key: %s, value: %f\n", key, json_real_value(json_value));
            return 1;
        } else {
            UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: unhandled value type at key [%s]", key);
            return 0;
        }

    }

    return 0;
}

bool json_conf_match_key(const char *key, char *key_name) {
    if (strcmp(key, key_name) == 0) {
        return true;
    }
    return false;

}


bool json_conf_match_key_extract_ec_value(ec_datatype dt, const char *key, char *key_name, ec_value *act_value,
                                          json_t *json_value) {

    if (strcmp(key, key_name) == 0) {


        switch (dt) {
            case 0:
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: [JSON config] Error: unhandled value type at key [%s] (ECT_NONE - 0 for datatype)",
                         key);
                return false;
            case ECT_BOOLEAN:
                if (json_is_integer(json_value)) {
                    act_value->boolean = (uint8_t) json_integer_value(json_value);
                    return true;
                } else {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: [JSON config] Error: unhandled value type at key [%s] (datatype is ECT_BOOLEAN but JSON value doesn't contain an 0 or 1",
                             key);
                    return false;
                }
            case ECT_INTEGER8:
                if (json_is_integer(json_value)) {
                    act_value->int8 = (int8_t) json_integer_value(json_value);
                    return true;
                } else {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: [JSON config] Error: unhandled value type at key [%s] (datatype is ECT_INTEGER8 but JSON value doesn't contain an integer)",
                             key);
                    return false;
                }
            case ECT_UNSIGNED8:

                if (json_is_integer(json_value)) {
                    act_value->uint8 = (uint8_t) json_integer_value(json_value);
                    return true;
                } else {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: [JSON config] Error: unhandled value type at key [%s] (datatype is ECT_UNSIGNED8 but JSON value doesn't contain an integer)",
                             key);
                    return false;
                }

            case ECT_INTEGER16:
                if (json_is_integer(json_value)) {
                    act_value->int16 = (int16_t) json_integer_value(json_value);
                    return true;
                } else {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: [JSON config] Error: unhandled value type at key [%s] (datatype is ECT_INTEGER16 but JSON value doesn't contain an integer)",
                             key);
                    return false;
                }
            case ECT_UNSIGNED16:
                if (json_is_integer(json_value)) {
                    act_value->uint16 = (uint16_t) json_integer_value(json_value);
                    return true;
                } else {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: [JSON config] Error: unhandled value type at key [%s] (datatype is ECT_UNSIGNED16 but JSON value doesn't contain an integer)",
                             key);
                    return false;
                }
            case ECT_UNSIGNED32:
                if (json_is_integer(json_value)) {
                    act_value->uint32 = (uint32_t) json_integer_value(json_value);
                    return true;
                } else {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: [JSON config] Error: unhandled value type at key [%s] (datatype is ECT_UNSIGNED32 but JSON value doesn't contain an integer)",
                             key);
                    return false;
                }
            case ECT_INTEGER32:
                if (json_is_integer(json_value)) {
                    act_value->int32 = (int32_t) json_integer_value(json_value);
                    return true;
                } else {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: [JSON config] Error: unhandled value type at key [%s] (datatype is ECT_INTEGER32 but JSON value doesn't contain an integer)",
                             key);
                    return false;
                }
            default:
                UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: unhandled value type at key [%s]", key);
                return false;


        }
    }
//this is a bit wierd returning true if we don't find the key
    return true;
}

