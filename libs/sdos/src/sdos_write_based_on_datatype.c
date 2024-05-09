/**
 ******************************************************************************
 * @file           :  sdos_write_based_on_datatype
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "sdos_write_based_on_datatype.h"

gberror_t
sdos_write_based_on_datatype(const uint16_t slave, const uint16_t index, const uint8_t subindex, const uint8_t datatype,
                             const ec_value value) {

    switch (datatype) {

        case ECT_BOOLEAN:


            break;
        case ECT_INTEGER8:
            break;
        case ECT_INTEGER16:
            break;
        case ECT_INTEGER32:
            if (ec_sdo_write_int32(slave, index, subindex, value.int32, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
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
    return E_SUCCESS;
}