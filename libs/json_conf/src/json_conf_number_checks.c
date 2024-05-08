/**
 ******************************************************************************
 * @file           :  json_conf_number_checks
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "json_conf_number_checks.h"


bool json_conf_check_integer_fits_uint32(long long int value){

    if (value >= 0 && (unsigned long long) value <= UINT32_MAX) {
        return true;  // Value fits within the unsigned 32-bit range
    } else {
        return false; // Value is outside the unsigned 32-bit range
    }

}

bool json_conf_check_integer_fits_int32(long long int value){
    if (value >= INT32_MIN && value <= INT32_MAX) {
        return true;  // Value fits within the signed 32-bit range
    } else {
        return false; // Value is outside the signed 32-bit range
    }
}
bool json_conf_check_integer_fits_uint16(long long int value){

    if (value >= 0 && (unsigned long long) value <= UINT16_MAX) {
        return true;  // Value fits within the unsigned 16-bit range
    } else {
        return false; // Value is outside the unsigned 16-bit range
    }

}

bool json_conf_check_integer_fits_int16(long long int value){
    if (value >= INT16_MIN && value <= INT16_MAX) {
        return true;  // Value fits within the signed 16-bit range
    } else {
        return false; // Value is outside the signed 16-bit range
    }
}

bool json_conf_check_integer_fits_uint8(long long int value){

    if (value >= 0 && (unsigned long long) value <= UINT8_MAX) {
        return true;  // Value fits within the unsigned 8-bit range
    } else {
        return false; // Value is outside the unsigned 8-bit range
    }

}

bool json_conf_check_integer_fits_int8(long long int value){
    if (value >= INT16_MIN && value <= INT8_MAX) {
        return true;  // Value fits within the signed 8-bit range
    } else {
        return false; // Value is outside the signed 8-bit range
    }
}