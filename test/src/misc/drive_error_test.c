
/**
 ******************************************************************************
 * @file           :  drive_error_test.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "azdxa-ked.h"
#include <stdio.h>
#include "unity.h"
#include "ethercatsetget.h"
#include "test.h"
#include "kollmorgen.h"


void test_miscAzdError(void) {


    TEST_ASSERT_EQUAL_STRING_MESSAGE("AZD: No warning", map_drive_get_error_string_pdo_function_ptr[0](0),
                                     "Drive[0] no fiddling");

    ec_pdo_set_input_uint16_rev(MAP_AZD_CTRL_1,
                                AZDXA_KED_ERROR_CODE_PDO_DRIVE1_INDEX, 0x25);

    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("AZD: Undervoltage", map_drive_get_error_string_pdo_function_ptr[0](0),
                                     "Drive[0] no set 0x25");
    ec_pdo_set_input_uint16_rev(MAP_AZD_CTRL_1,
                                AZDXA_KED_ERROR_CODE_PDO_DRIVE1_INDEX, 0x100);
    internal_advanceStateIncrementHeartbeatSetRemote(1);

    printf("msg:%s\n", map_drive_get_error_string_pdo_function_ptr[0](0));

}

bool ec_sdo_read_uint32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint32_t *return_val) {
//    *return_val = 0x1081; "General Error"
    *return_val = 0x3282; //Regen short circuit

//    *return_val = 0xFFF; "Error code not found"

    return true;
}


void test_miscEc_get_error_code_azd(void) {


    ec_pdo_set_input_int32_rev(MAP_AZD_CTRL_1,
                               AZDXA_KED_ERROR_CODE_PDO_DRIVE1_INDEX, 0x10);

    uint8_t *test_error_message_ptr;
    test_error_message_ptr = map_drive_get_error_string_pdo_function_ptr[0](0);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("AZD: Excessive position deviation", test_error_message_ptr, "AZD error string");
//    printf("%s\n",test_error_message_ptr );
}


void test_miscAkdError(void) {

    char *test_error_message;

    test_error_message = ec_get_error_string_sdo_akd(0);

    TEST_ASSERT_EQUAL_STRING_MESSAGE("AKD: Regen short circuit", test_error_message, "Drive[0] SDO read function overwritten");


}



