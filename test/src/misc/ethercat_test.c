
#include <stdint.h>
#include "unity.h"
#include "ethercatsetget.h"
#include "test.h"
void test_miscEthercatGetSet(void) {

    /*** OUTPUTS ***/
    uint32_t otest_uint32;
    ec_pdo_set_output_uint32(1, 1, 4294967295);
    otest_uint32 = ec_pdo_get_output_uint32_rev(1, 1);
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(4294967295, otest_uint32, "OUT: uint32");
    internal_Cleardown();

    int32_t otest_int32;
    ec_pdo_set_output_int32(1, 1, -2147483647);
    otest_int32 = ec_pdo_get_output_int32_rev(1, 1);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-2147483647, otest_int32, "OUT: int32");
    internal_Cleardown();

    uint16_t otest_uint16;
    ec_pdo_set_output_uint16(1, 1, 65535);
    otest_uint16 = ec_pdo_get_output_uint16_rev(1, 1);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(65535, otest_uint16, "OUT: uint16");
    internal_Cleardown();

    int16_t otest_int16;
    ec_pdo_set_output_int16(1, 1, -32767);
    otest_int16 = ec_pdo_get_output_int16_rev(1, 1);
    TEST_ASSERT_EQUAL_INT16_MESSAGE(-32767, otest_int16, "OUT: int16");
    internal_Cleardown();

    uint8_t otest_uint8;
    ec_pdo_set_output_uint8(1, 1, 254);
    otest_uint8 = ec_pdo_get_output_uint8_rev(1, 1);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(254, otest_uint8, "OUT: uint8");
    internal_Cleardown();

    int8_t otest_int8;
    ec_pdo_set_output_int8(1, 1, -127);
    otest_int8 = ec_pdo_get_output_int8_rev(1, 1);
    TEST_ASSERT_EQUAL_INT8_MESSAGE(-127, otest_int8, "OUT: int8");
    internal_Cleardown();

    bool otest_bool;
    ec_pdo_set_output_bit(1, 1, true);
    otest_bool = cs_pdo_get_output_bit_rev(1, 1);
    TEST_ASSERT_MESSAGE(otest_bool, "OUT: bit");
    internal_Cleardown();

    /*** INPUTS ***/
    uint32_t itest_uint32;
    ec_pdo_set_input_uint32_rev(1, 1, 4294967295);
    itest_uint32 = ec_pdo_get_input_uint32(1, 1);
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(4294967295, itest_uint32, "IN: uint32");
    internal_Cleardown();

    int32_t itest_int32;
    ec_pdo_set_input_int32_rev(1, 1, -2147483647);
    itest_int32 = ec_pdo_get_input_int32(1, 1);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-2147483647, itest_int32, "IN: int32");
    internal_Cleardown();

    uint16_t itest_uint16;
    ec_pdo_set_input_uint16_rev(1, 1, 65535);
    itest_uint16 = ec_pdo_get_input_uint16(1, 1);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(65535, itest_uint16, "IN: uint16");
    internal_Cleardown();

    int16_t itest_int16;
    ec_pdo_set_input_int16_rev(1, 1, -32767);
    itest_int16 = ec_pdo_get_input_int16(1, 1);
    TEST_ASSERT_EQUAL_INT16_MESSAGE(-32767, itest_int16, "IN: int16");
    internal_Cleardown();

    uint8_t itest_uint8;
    ec_pdo_set_input_uint8(1, 1, 256);
    itest_uint8 = ec_pdo_get_input_uint8(1, 1);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(256, itest_uint8, "IN: uint8");
    internal_Cleardown();

    int8_t itest_int8;
    ec_pdo_set_input_int8_rev(1, 1, -128);
    itest_int8 = ec_pdo_get_input_int8(1, 1);
    TEST_ASSERT_EQUAL_INT8_MESSAGE(-128, itest_int8, "IN: int8");
    internal_Cleardown();

    bool itest_bool;
    ec_pdo_set_input_bit_rev(1, 1, true);
    itest_bool = ec_pdo_get_input_bit(1, 1);
    TEST_ASSERT_MESSAGE(itest_bool, "IN: bit");
    internal_Cleardown();
}
