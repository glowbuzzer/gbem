/**
 ******************************************************************************
 * @file           :  json_msg_parse_sdo_write_payload
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "json_msg_parse_sdo_write_payload.h"

gberror_t json_msg_parse_sdo_write_payload(json_t *sdo_payload, uint16_t *slave, ec_datatype *datatype, uint16_t *index,
                                           uint8_t *sub_index, uint8_t *len, ec_value *value) {

    json_t *obj = json_object_get(sdo_payload, "index");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: index is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *index = (uint16_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "subindex");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: sub_index is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *sub_index = (uint8_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "slave");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: slave is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *slave = (uint16_t) json_integer_value(obj);

    obj = json_object_get(sdo_payload, "datatype");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: datatype is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    *datatype = (ec_datatype) json_integer_value(obj);


    obj = json_object_get(sdo_payload, "length");

    if (!json_is_integer(obj)) {
        UM_ERROR(GBEM_UM_EN, "GBEM: [JSON MSG] Error: length is not a integer in JSON request\n");
        return E_GENERAL_FAILURE;
    }

    obj = json_object_get(sdo_payload, "value");


    //todo switch on data type here

    if (json_is_integer(obj)) {
        printf("value is integer\n");
    } else if (json_is_real(obj)) {
        printf("value is real\n");
    } else if (json_is_string(obj)) {
        printf("value is string\n");
    } else if (json_is_boolean(obj)) {
        printf("value is boolean\n");
    } else {
        printf("value is not integer, real, string, or boolean\n");
    }


    switch (*datatype) {

        case ECT_BOOLEAN:

            break;
        case ECT_INTEGER8:

            break;
        case ECT_INTEGER16:

            break;
        case ECT_INTEGER32:

            break;
        case ECT_UNSIGNED8:

            break;
        case ECT_UNSIGNED16:

            break;
        case ECT_UNSIGNED32:

            break;
        case ECT_REAL32:

            break;
        case ECT_VISIBLE_STRING:

            break;
        case ECT_OCTET_STRING:

            break;
        case ECT_UNICODE_STRING:

            break;
        case ECT_TIME_OF_DAY:

            break;
        case ECT_TIME_DIFFERENCE:

            break;
        case ECT_DOMAIN:

            break;
        case ECT_INTEGER24:

            break;
        case ECT_REAL64:

            break;
        case ECT_INTEGER64:

            break;
        case ECT_UNSIGNED24:

            break;
        case ECT_UNSIGNED64:

            break;
        case ECT_BIT1:

            break;
        case ECT_BIT2:

            break;
        case ECT_BIT3:

            break;
        case ECT_BIT4:

            break;
        case ECT_BIT5:

            break;
        case ECT_BIT6:

            break;
        case ECT_BIT7:

            break;
        case ECT_BIT8:

            break;
        case ECT_STRING8:

        default:
            //todo
//            LL_FATAL("Unknown datatype: %d", datatype);
            break;

    }



//    *len = (uint8_t) json_object_size(sdo_payload);

    *len = (uint8_t) json_integer_value(obj);

    return E_SUCCESS;


}
