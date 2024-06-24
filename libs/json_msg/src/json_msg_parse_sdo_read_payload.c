/**
 ******************************************************************************
 * @file           :  json_msg_parse_sdo_read_payload
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "json_msg_parse_sdo_read_payload.h"

gberror_t json_msg_parse_sdo_read_payload(json_t *sdo_payload, uint16_t *slave, ec_datatype *datatype, uint16_t *index,
                                          uint8_t *sub_index, uint8_t *len) {

    json_t *obj = json_object_get(sdo_payload, "index");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: index is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *index = (uint16_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "subindex");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: sub_index is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *sub_index = (uint8_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "slave");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: slave is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *slave = (uint16_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "datatype");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: datatype is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *datatype = (ec_datatype) json_integer_value(obj);


    obj = json_object_get(sdo_payload, "length");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [json_mesg] Error: length is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }


//    *len = (uint8_t) json_object_size(sdo_payload);

    *len = (uint8_t) json_integer_value(obj);

    return E_SUCCESS;


}
