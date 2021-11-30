
/**
 ******************************************************************************
 * @file           :  etg1020_diag_test.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include <stdbool.h>
#include <stdint.h>
#include "etg1020_diag.h"
#include <string.h>
#include "unity.h"

void test_miscEtg1020_diag_textid_decode(void) {
//    void etg1020_diag_textid_decode(uint16_t textid, etg1020_diag_textid_t *result);

    uint16_t test_textid;
    volatile etg1020_diag_textid_t test_result;

    memset(&test_result, 0, sizeof(etg1020_diag_code_t));
    test_textid = 0x4413;
    etg1020_diag_textid_decode(test_textid, &test_result);

    TEST_ASSERT_MESSAGE(test_result.x == ETG10120DIAG_X_WARNING, "x");
    TEST_ASSERT_MESSAGE(test_result.y == ETG10120DIAG_Y_DRIVE, "y");

    TEST_ASSERT_MESSAGE(test_result.short_code == 0x13, "short_code");
    TEST_ASSERT_MESSAGE(test_result.long_code == test_textid, "long_code");


//    test_textid = 65535;
//    etg1020_diag_textid_decode(test_textid, &test_result);




}

void test_miscEtg1020_diag_flag_decode(void) {
//    void etg1020_diag_flag_decode(uint16_t flags, etg1020_diag_flags_t *result);

    uint16_t test_flags;
    etg1020_diag_flags_t test_result;

    //expect ERROR_MESSAGE and local_time_stamp and four parameters

    test_flags = ((uint16_t) 0b00000100 << 8) | 0b00010010;;
    memset(&test_result, 0, sizeof(etg1020_diag_flags_t));
    etg1020_diag_flag_decode(test_flags, &test_result);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(4, test_result.number_of_parameters, "number_of_parameters");
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_type == ETG10120DIAG_TYPE_ERROR_MESSAGE, "etg1020_diag_type");
    TEST_ASSERT_MESSAGE(test_result.local_time_stamp == true, "local_time_stamp");

    //expect INFO_MESSAGE global_time_stamp and  zero oarameters
    test_flags = ((uint16_t) 0b00000000 << 8) | 0b00000000;
    memset(&test_result, 0, sizeof(etg1020_diag_flags_t));
    etg1020_diag_flag_decode(test_flags, &test_result);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, test_result.number_of_parameters, "number_of_parameters");
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_type == ETG10120DIAG_TYPE_INFO_MESSAGE, "etg1020_diag_type");
    TEST_ASSERT_MESSAGE(test_result.local_time_stamp == false, "local_time_stamp");

//expect UNSPECIFIED_MESSAGE local_time_stamp and thirty three parameters
    test_flags = ((uint16_t) 0b00100001 << 8) | 0b00010011;
    memset(&test_result, 0, sizeof(etg1020_diag_flags_t));
    etg1020_diag_flag_decode(test_flags, &test_result);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(33, test_result.number_of_parameters, "number_of_parameters");
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_type == ETG10120DIAG_TYPE_UNSPECIFIED_MESSAGE, "etg1020_diag_type");
    TEST_ASSERT_MESSAGE(test_result.local_time_stamp == true, "local_time_stamp");

//expect WARNING_MESSAGE and local_time_stamp and two parameters

    test_flags = ((uint16_t) 0b000000010 << 8) | 0b00010001;
    memset(&test_result, 0, sizeof(etg1020_diag_flags_t));
    etg1020_diag_flag_decode(test_flags, &test_result);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(2, test_result.number_of_parameters, "number_of_parameters");
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_type == ETG10120DIAG_TYPE_WARNING_MESSAGE, "etg1020_diag_type");
    TEST_ASSERT_MESSAGE(test_result.local_time_stamp == true, "local_time_stamp");

}

void test_miscEtg1020_diag_code_decode(void) {
//    void etg1020_diag_code_decode(uint32_t diag_code, etg1020_diag_code_t *result);

// 0xDfff - 57343
// 0xE7FF - 59391
// 0xE800 - 59392
// 0xEDFF - 60927
// 0xEFFF - 61439

    uint32_t test_diag_code;
    etg1020_diag_code_t test_result;

    memset(&test_result, 0, sizeof(etg1020_diag_code_t));
    test_diag_code = 0xDfff;
    etg1020_diag_code_decode(test_diag_code, &test_result);
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_code_type == ETG10120DIAG_CODE_TYPE_UNSPECIFIED,
                        "ETG10120DIAG_CODE_TYPE_UNSPECIFIED");

    memset(&test_result, 0, sizeof(etg1020_diag_code_t));
    test_diag_code = 0xE7FF;
    etg1020_diag_code_decode(test_diag_code, &test_result);
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_code_type == ETG10120DIAG_CODE_TYPE_MFR_SPECIFIC,
                        "ETG10120DIAG_CODE_TYPE_MFR_SPECIFIC");

    memset(&test_result, 0, sizeof(etg1020_diag_code_t));
    test_diag_code = 0xE800;
    etg1020_diag_code_decode(test_diag_code, &test_result);
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_code_type == ETG10120DIAG_CODE_TYPE_EMERGENCY_CODE_DS301,
                        "ETG10120DIAG_CODE_TYPE_EMERGENCY_CODE_DS301");

    memset(&test_result, 0, sizeof(etg1020_diag_code_t));
    test_diag_code = 0xEDFF;
    etg1020_diag_code_decode(test_diag_code, &test_result);
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_code_type == ETG10120DIAG_CODE_TYPE_UNSPECIFIED,
                        "ETG10120DIAG_CODE_TYPE_UNSPECIFIED");

    memset(&test_result, 0, sizeof(etg1020_diag_code_t));
    test_diag_code = 0xF000;
    etg1020_diag_code_decode(test_diag_code, &test_result);
    TEST_ASSERT_MESSAGE(test_result.etg1020_diag_code_type == ETG10120DIAG_CODE_TYPE_UNSPECIFIED,
                        "ETG10120DIAG_CODE_TYPE_UNSPECIFIED");


}


void test_miscEtg1020_diag_full_decode(void) {
//    bool etg1020_diag_full_decode(uint8_t *message, etg1020_diag_message_t *result);

    etg1020_diag_message_t test_result;

    uint8_t test_message[28];

    test_message[0] = 0x00; // 0xE800 = Emergency code
    test_message[1] = 0xE8;
    test_message[2] = 0x00; // 0x0000 = Code 0
    test_message[3] = 0x00;
    test_message[4] = 0b00010010; // Local time stamp error message
    test_message[5] = 0x00; // Number of parameters = 0
    test_message[6] = 0x00; // 0x0000, Text id as reference to ESI file = 0
    test_message[7] = 0x00;
    test_message[8] = 0x00; // Timestamp (none attached)
    test_message[9] = 0x00; // Timestamp (none attached)
    test_message[10] = 0x00; // Timestamp (none attached)
    test_message[11] = 0x00; // Timestamp (none attached)
    test_message[12] = 0x00; // Timestamp (none attached)
    test_message[13] = 0x00; // Timestamp (none attached)
    test_message[14] = 0x00; // Timestamp (none attached)
    test_message[15] = 0x00; // Timestamp (none attached)
    test_message[16] = 0b00000010; // Param1 = Signed8
    test_message[17] = 0b00000000;
    test_message[18] = 0b00000011; // Port 3
    test_message[19] = 0b00000011; // Param2 = Signed16
    test_message[20] = 0b00000000;
    test_message[21] = 0b11101000; // EventCode = 10#1000
    test_message[22] = 0b00000011;
    test_message[23] = 0b00000010; // Param3 = Signed8
    test_message[24] = 0b00000000;
    test_message[25] = 0b00010111; // Qualifier: Instance=Reserved_7, Source=Device, Type=Notification, Mode=Reserved
    test_message[26] = 0b00000000;
    test_message[27] = 0b00000000;

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, test_result.diag_flags.number_of_parameters, "diag_flags.number_of_parameters");
    TEST_ASSERT_MESSAGE(test_result.diag_textid.short_code == 0, "test_result.diag_textid.short_code");


    memset(&test_result, 0, sizeof(etg1020_diag_message_t));
    etg1020_diag_full_decode(test_message, &test_result);

    test_message[0] = 0x90; // 0xE290 = Manufacturer Specific
    test_message[1] = 0xE2;
    test_message[2] = 0x30; // 0x0000 = Code 0
    test_message[3] = 0x75;
    test_message[4] = 0b00000000; // Global time stamp &amp;amp;amp; info message
    test_message[5] = 0x02; // Number of parameters = 2
    test_message[6] = 0xA8; // 0x61A8, Text id as reference to ESI file = 10#25000
    test_message[7] = 0x61;
    test_message[8] = 0xC8; // Timestamp from DC clock, 16#07C8D11492616FC8 = '2017-10-10-05:20:39.893037000'
    test_message[9] = 0x6F;
    test_message[10] = 0x61;
    test_message[11] = 0x92;
    test_message[12] = 0x14;
    test_message[13] = 0xD1;
    test_message[14] = 0xC8;
    test_message[15] = 0x07;
    test_message[16] = 0b00000010; // Param1 = Signed8
    test_message[17] = 0b00000000;
    test_message[18] = 0b00000110; // Port 6
    test_message[19] = 0b00000011; // Param2 = Signed16
    test_message[20] = 0b00000000;
    test_message[21] = 0b11010010; // EventCode = 10#1234
    test_message[22] = 0b00000100;
    test_message[23] = 0b00000010; // Param3 = Signed8
    test_message[24] = 0b00000000;
    test_message[25] = 0b01000110; // Qualifier: Instance=Reserved_6, Source=Device, Type=Reserved, Mode=EventSingleShot
    test_message[26] = 0b00000000;
    test_message[27] = 0b00000000;

    memset(&test_result, 0, sizeof(etg1020_diag_message_t));

    etg1020_diag_full_decode(&test_message[0], &test_result);

    TEST_ASSERT_MESSAGE(test_result.diag_code.etg1020_diag_code_type == ETG10120DIAG_CODE_TYPE_MFR_SPECIFIC,
                        "diag_code.etg1020_diag_code_type");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, test_result.diag_code.code_specific, "diag_code.code_specific");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(25000, test_result.diag_textid.long_code, "diag_textid.long_code");
    TEST_ASSERT_MESSAGE(test_result.diag_flags.local_time_stamp == false, "local_time_stamp");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(2, test_result.diag_flags.number_of_parameters, "diag_flags.number_of_parameters");
    TEST_ASSERT_MESSAGE(test_result.diag_params[0].param_type == ETG10120DIAG_PARAM_TYPE_INTEGER8,
                        "diag_params[0].param_type");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(6, test_result.diag_params[0].param_value, "diag_params[0].param_value");
    TEST_ASSERT_MESSAGE(test_result.diag_params[1].param_type == ETG10120DIAG_PARAM_TYPE_INTEGER16,
                        "diag_params[0].param_type");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(1234, test_result.diag_params[1].param_value, "diag_params[1].param_value");


    test_message[0] = 0x01; // 0xE801 = Reserved for future use
    test_message[1] = 0xE8;
    test_message[2] = 0xFF; // 0xFFFF = Code 65535
    test_message[3] = 0xFF;
    test_message[4] = 0b00000001; // Global time stamp &amp;amp;amp; warning message
    test_message[5] = 0xFF; // Number of parameters = 255
    test_message[6] = 0xFF; // 0x61A8, Text id as reference to ESI file = 10#65535
    test_message[7] = 0xFF;
    test_message[8] = 0xFF; // Timestamp from DC clock, 16#FFFFFFFFFFFFFFFF = '2584-07-20-23:34:33.709551615'
    test_message[9] = 0xFF;
    test_message[10] = 0xFF;
    test_message[11] = 0xFF;
    test_message[12] = 0xFF;
    test_message[13] = 0xFF;
    test_message[14] = 0xFF;
    test_message[15] = 0xFF;
    test_message[16] = 0b00000010; // Param1 = Signed8
    test_message[17] = 0b00000000;
    test_message[18] = 0b00000101; // Port 5
    test_message[19] = 0b00000011; // Param2 = Signed16
    test_message[20] = 0b00000000;
    test_message[21] = 0b01000100; // EventCode = 10#65092
    test_message[22] = 0b11111110;
    test_message[23] = 0b00000010; // Param3 = Signed8
    test_message[24] = 0b00000000;
    test_message[25] = 0b11011111; // Qualifier: Instance=Reserved_7, Source=Master, Type=Notification, Mode=EventAppears
    test_message[26] = 0b00000000;
    test_message[27] = 0b00000000;

    memset(&test_result, 0, sizeof(etg1020_diag_message_t));

    etg1020_diag_full_decode(&test_message[0], &test_result);

    TEST_ASSERT_MESSAGE(test_result.diag_code.etg1020_diag_code_type == ETG10120DIAG_CODE_TYPE_UNSPECIFIED,
                        "diag_code.etg1020_diag_code_type");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(65535, test_result.diag_textid.long_code, "diag_textid.long_code");
    TEST_ASSERT_MESSAGE(test_result.diag_flags.local_time_stamp == false, "local_time_stamp");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(255, test_result.diag_flags.number_of_parameters,
                                    "diag_flags.number_of_parameters");
    TEST_ASSERT_MESSAGE(test_result.diag_params[0].param_type == ETG10120DIAG_PARAM_TYPE_INTEGER8,
                        "diag_params[0].param_type");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(5, test_result.diag_params[0].param_value, "diag_params[0].param_value");
    TEST_ASSERT_MESSAGE(test_result.diag_params[1].param_type == ETG10120DIAG_PARAM_TYPE_INTEGER16,
                        "diag_params[0].param_type");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(65092, test_result.diag_params[1].param_value, "diag_params[1].param_value");
    TEST_ASSERT_MESSAGE(test_result.diag_params[2].param_type == ETG10120DIAG_PARAM_TYPE_INTEGER8,
                        "diag_params[0].param_type");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(223, test_result.diag_params[2].param_value, "diag_params[1].param_value");


    test_message[0] = 0x01; // 0xE801 = Reserved for future use
    test_message[1] = 0xE8;
    test_message[2] = 0xFF; // 0xFFFF = Code 65535
    test_message[3] = 0xFF;
    test_message[4] = 0b00000001; // Global time stamp &amp;amp;amp; warning message
    test_message[5] = 0xFF; // Number of parameters = 255
    test_message[6] = 0xFF; // 0x61A8, Text id as reference to ESI file = 10#65535
    test_message[7] = 0xFF;
    test_message[8] = 0xFF; // Timestamp from DC clock, 16#FFFFFFFFFFFFFFFF = '2584-07-20-23:34:33.709551615'
    test_message[9] = 0xFF;
    test_message[10] = 0xFF;
    test_message[11] = 0xFF;
    test_message[12] = 0xFF;
    test_message[13] = 0xFF;
    test_message[14] = 0xFF;
    test_message[15] = 0xFF;
    test_message[16] = 0b00000010; // Param1 = Signed8
    test_message[17] = 0b00000000;
    test_message[18] = 0b00000101; // Port 5
    test_message[19] = 0b00000100; // Param2 = Signed32
    test_message[20] = 0b00000000;
    test_message[21] = 0b11111111; // EventCode = 10#‭4294967295‬ (though will be interpreted as maximum 16 bits = 65535)
    test_message[22] = 0b11111111;
    test_message[23] = 0b11111111;
    test_message[24] = 0b11111111;
    test_message[25] = 0b00000010; // Param3 = Signed8
    test_message[26] = 0b00000000;
    test_message[27] = 0b11011111; // Qualifier: Instance=Reserved_7, Source=Master, Type=Notification, Mode=EventAppears

    memset(&test_result, 0, sizeof(etg1020_diag_message_t));

    etg1020_diag_full_decode(&test_message[0], &test_result);

//todo
}

void test_miscEtg1020_diag_timestamp_decode(void) {
    time_t etg1020_diag_timestamp_decode(const uint8_t *nsec_array);

    uint8_t test_nsec_array[8];
    volatile time_t test_result;

    memset(&test_result, 0, sizeof(time_t));
    memset(test_nsec_array, 0, 8 * sizeof(uint8_t));

    test_nsec_array[0] = 0xC0;
    test_nsec_array[1] = 0x25;
    test_nsec_array[2] = 0x10;
    test_nsec_array[3] = 0xA7;
    test_nsec_array[4] = 0x60;
    test_nsec_array[5] = 0x65;
    test_nsec_array[6] = 0xC7;
    test_nsec_array[7] = 0x07;

    test_result = etg1020_diag_timestamp_decode(test_nsec_array);

//todo

}

void test_miscEtg1020_diag_decode_param_flag(void) {

//    void etg1020_diag_decode_param_flag(uint16_t param_flag, etg1020_diag_params_t *result);

    uint16_t test_param_flag;

    etg1020_diag_params_t test_result;

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x0;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNKNOWN, "param_type != UNKNOWN");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x01;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_BOOLEAN, "param_type != BOOLEAN");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x2;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_INTEGER8, "param_type != INTEGER8");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x3;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_INTEGER16, "param_type != INTEGER16");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x4;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_INTEGER32, "param_type != INTEGER32");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x5;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNSIGNED8, "param_type != UNSIGNED8");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x6;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNSIGNED16, "param_type != UNSIGNED16");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x7;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNSIGNED32, "param_type != UNSIGNED32");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x8;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_REAL32, "param_type != REAL32");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x11;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_REAL64, "param_type != REAL64");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x15;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_INTEGER64, "param_type != INTEGER64");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x1B;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNSIGNED64, "param_type != UNSIGNED64");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0x1F;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNKNOWN, "param_type is unknown");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0b1000000001111;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_BYTEARRAY, "param_type != BYTEARRAY");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(15, test_result.param_length, "param_length BYTEARRAY");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0b1111000000001111;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNKNOWN, "param_type is unknown");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0b10000000001111;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_ASCIISTRING, "param_type != ASCIISTRING");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(15, test_result.param_length, "param_length ASCIISTRING");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0b11000000001111;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNICODESTRING, "param_type != UNICODESTRING");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(15, test_result.param_length, "param_length UNICODESTRING");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0b100000000000010;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_TEXTID, "param_type != TEXTID");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(2, test_result.param_length, "param_length TEXTID");

    memset(&test_result, 0, sizeof(etg1020_diag_params_t));
    test_param_flag = 0b100000000000011;
    etg1020_diag_decode_param_flag(test_param_flag, &test_result);
    TEST_ASSERT_MESSAGE(test_result.param_type == ETG10120DIAG_PARAM_TYPE_UNKNOWN, "param_type != TEXTID");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(3, test_result.param_length, "param_length TEXTID");


}