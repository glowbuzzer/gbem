/**
 ******************************************************************************
 * @file           :  json_conf_parse_sdo
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "json_conf_parse_sdo.h"
#include "json_conf_helpers.h"
#include "json_conf_number_checks.h"
#include <string.h>
#include "user_message.h"

bool json_conf_parse_sdo(json_t *json_ethercat, ec_sdo_array *ar) {
    json_t *ethercat_slave_object;
    json_t *slaves;
    json_t *sdos;
    json_t *sdo;
    json_t *sdo_entry;


    size_t size;
    size_t index1;
    size_t index2;
    char *key;

    memset(ar, 0, sizeof(ec_sdo_array));


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

    ar->num_slaves = (uint8_t) size;

    json_array_foreach(ethercat_slave_object, index1, slaves) {
//        printf("index1: %lld\n", (long long) index1);

        sdos = json_object_get(slaves, "sdos");

        if (!json_conf_check_object(sdos, "sdos")) {
            return false;
        }

        if (!json_is_array(sdos)) {
            UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: sdos object not an array");
            return false;
        }


        json_array_foreach(sdos, index2, sdo) {
//            printf("index2: %lld\n", (long long) index2);
            ar->num_sdo[index1]++;

            size = json_object_size(sdo);
            if (size != 5) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: [JSON config] xxxError: SDO object size [%lld] should be 5 (sub_index, index, slave, datatype, length)",
                         (long long) size);
                return false;
            }

            long long ll_index = 0;
            long long ll_sub_index = 0;
            long long ll_value = 0;
            long long ll_datatype = 0;


            bool found_index = false;
            bool found_sub_index = false;
            bool found_value = false;
            bool found_datatype = false;


            json_object_foreach(sdo, key, sdo_entry) {
                if (found_datatype == false) {
                    found_datatype = json_conf_match_key(key, "datatype");
                }
                if (found_index == false) {
                    found_index = json_conf_match_key(key, "index");
                }
                if (found_sub_index == false) {
                    found_sub_index = json_conf_match_key(key, "sub_index");
                }
                if (found_value == false) {
                    found_value = json_conf_match_key(key, "value");
                }
            }

            if (!found_index || !found_sub_index || !found_value || !found_datatype) {
                UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: incorrect sections or names in SDO object");
                return false;
            }

            json_object_foreach(sdo, key, sdo_entry) {
                json_conf_match_key_extract_int(key, "datatype", &ll_datatype, sdo_entry);
                json_conf_match_key_extract_int(key, "index", &ll_index, sdo_entry);
                json_conf_match_key_extract_int(key, "sub_index", &ll_sub_index, sdo_entry);
            }


            if (json_conf_check_integer_fits_uint16(ll_index)) {
                ar->sdo[index1][index2].index = (uint16_t) ll_index;
            } else {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: [JSON config] Error: index value [%lld] does not fit in int16 on slave [%zu]", ll_index,
                         index1);
                return false;
            }

            if (json_conf_check_integer_fits_uint8(ll_sub_index)) {
                ar->sdo[index1][index2].subindex = (uint8_t) ll_sub_index;
            } else {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: [JSON config] Error: sub_index value [%lld] does not fit in int8 on slave [%zu]",
                         ll_sub_index, index1);
                return false;
            }
            if (json_conf_check_integer_fits_uint16(ll_datatype)) {
                ar->sdo[index1][index2].datatype = (uint16_t) ll_datatype;
            } else {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: [JSON config] Error: datatype value [%lld] does not fit in int8 on slave [%zu]",
                         ll_datatype, index1);
                return false;

            }

            if (!ec_is_valid_ec_datatype(ar->sdo[index1][index2].datatype)) {
                UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: datatype value [%d] is not valid on slave [%zu]",
                         ar->sdo[index1][index2].datatype, index1);
                return false;
            }

            json_object_foreach(sdo, key, sdo_entry) {
                if (!json_conf_match_key_extract_ec_value(ar->sdo[index1][index2].datatype, key, "value",
                                                          &ar->sdo[index1][index2].value, sdo_entry)) {
                    UM_ERROR(GBEM_UM_EN, "GBEM: [JSON config] Error: could not extract value from SDO");
                    return false;
                }
            }


        }


    }

    return true;

}
