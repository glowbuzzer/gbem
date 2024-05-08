/**
 ******************************************************************************
 * @file           :  datatypes
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "json_conf_ethercat_helpers.h"

bool ec_is_valid_ec_datatype(uint16_t value) {
    switch (value) {
        case ECT_BOOLEAN:
        case ECT_INTEGER8:
        case ECT_INTEGER16:
        case ECT_INTEGER32:
        case ECT_UNSIGNED8:
        case ECT_UNSIGNED16:
        case ECT_UNSIGNED32:
        case ECT_REAL32:
        case ECT_VISIBLE_STRING:
        case ECT_OCTET_STRING:
        case ECT_UNICODE_STRING:
        case ECT_TIME_OF_DAY:
        case ECT_TIME_DIFFERENCE:
        case ECT_DOMAIN:
        case ECT_INTEGER24:
        case ECT_REAL64:
        case ECT_INTEGER64:
        case ECT_UNSIGNED24:
        case ECT_UNSIGNED64:
        case ECT_BIT1:
        case ECT_BIT2:
        case ECT_BIT3:
        case ECT_BIT4:
        case ECT_BIT5:
        case ECT_BIT6:
        case ECT_BIT7:
        case ECT_BIT8:
        case ECT_STRING8:
            return true;
        default:
            return false;
    }
}

