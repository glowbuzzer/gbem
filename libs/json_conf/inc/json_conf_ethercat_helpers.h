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
#ifndef JANSSON_TEST_DATATYPES_H
#define JANSSON_TEST_DATATYPES_H

#include <stdint.h>
#include <stdbool.h>
#include "ethercattype.h"

///** Ethercat data types */
//typedef enum {
//    ECT_BOOLEAN = 0x0001,
//    ECT_INTEGER8 = 0x0002,
//    ECT_INTEGER16 = 0x0003,
//    ECT_INTEGER32 = 0x0004,
//    ECT_UNSIGNED8 = 0x0005,
//    ECT_UNSIGNED16 = 0x0006,
//    ECT_UNSIGNED32 = 0x0007,
//    ECT_REAL32 = 0x0008,
//    ECT_VISIBLE_STRING = 0x0009,
//    ECT_OCTET_STRING = 0x000A,
//    ECT_UNICODE_STRING = 0x000B,
//    ECT_TIME_OF_DAY = 0x000C,
//    ECT_TIME_DIFFERENCE = 0x000D,
//    ECT_DOMAIN = 0x000F,
//    ECT_INTEGER24 = 0x0010,
//    ECT_REAL64 = 0x0011,
//    ECT_INTEGER64 = 0x0015,
//    ECT_UNSIGNED24 = 0x0016,
//    ECT_UNSIGNED64 = 0x001B,
//    ECT_BIT1 = 0x0030,
//    ECT_BIT2 = 0x0031,
//    ECT_BIT3 = 0x0032,
//    ECT_BIT4 = 0x0033,
//    ECT_BIT5 = 0x0034,
//    ECT_BIT6 = 0x0035,
//    ECT_BIT7 = 0x0036,
//    ECT_BIT8 = 0x0037,
//    ECT_STRING8 = 0x0100,
//} ec_datatype;
//
//
//typedef union {
//    uint8_t boolean; // For ECT_BOOLEAN
//    int8_t int8;     // For ECT_INTEGER8
//    int16_t int16;   // For ECT_INTEGER16
//    int32_t int32;   // For ECT_INTEGER32
//    uint8_t uint8;   // For ECT_UNSIGNED8
//    uint16_t uint16; // For ECT_UNSIGNED16
//    uint32_t uint32; // For ECT_UNSIGNED32
//    float real32;    // For ECT_REAL32
//    char *visible_string; // For ECT_VISIBLE_STRING
//    uint8_t *octet_string; // For ECT_OCTET_STRING
//    uint16_t *unicode_string; // For ECT_UNICODE_STRING
//    uint64_t time_of_day; // For ECT_TIME_OF_DAY
//    int64_t time_difference; // For ECT_TIME_DIFFERENCE
//    void *domain; // For ECT_DOMAIN
//    int32_t int24; // For ECT_INTEGER24, store in a 32-bit field
//    double real64; // For ECT_REAL64
//    int64_t int64; // For ECT_INTEGER64
//    uint32_t uint24; // For ECT_UNSIGNED24, store in a 32-bit field
//    uint64_t uint64; // For ECT_UNSIGNED64
//    uint8_t bit1;  // For ECT_BIT1 to ECT_BIT8
//    char *string8; // For ECT_STRING8
//} ec_value;
//
//
//typedef struct {
//    uint16_t index;
//    uint8_t subindex;
//    ec_datatype datatype;
//    ec_value value;
//} ec_sdo;
//
//typedef struct {
//    ec_sdo sdo[10][10];
//    uint8_t num_slaves;
//    uint8_t num_sdo[10];
//} ec_sdo_array;
//
//
//#define MAX_NUM_OPTIONAL_SLAVES 10
//
//typedef struct {
//    bool enable_optional_slave[MAX_NUM_OPTIONAL_SLAVES]; // Array of 10 boolean values
//} machine_config_optional_slaves_t;
//
//
//typedef struct {
//    int32_t position_limit_max;
//    int32_t position_limit_min;
//    uint32_t velocity_limit;
//    uint32_t torque_limit;
//    uint32_t max_motor_speed;
//    uint32_t max_motor_torque;
//} map_machine_limits_t;


bool ec_is_valid_ec_datatype(uint16_t value);


#endif //JANSSON_TEST_DATATYPES_H
