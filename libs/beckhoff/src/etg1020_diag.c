
/**
 ******************************************************************************
 * @file           :  etg1020_diag.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "etg1020_diag.h"
#include "EL7211_error_code.h"
#include "std_defs_and_macros.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "log.h"


const char *etg1020_diag_code_type_string[4] = {"ETG10120DIAG_CODE_TYPE_MFR_SPECIFIC",
                                                "ETG10120DIAG_CODE_TYPE_EMERGENCY_CODE_DS301",
                                                "ETG10120DIAG_CODE_TYPE_PROFILE_SPECIFIC",
                                                "ETG10120DIAG_CODE_TYPE_UNSPECIFIED"};

const char *etg1020_diag_x_string[4] = {
        "ETG10120DIAG_X_SYSTEMINFO",
        "ETG10120DIAG_X_INFO",
        "ETG10120DIAG_X_WARNING",
        "ETG10120DIAG_X_ERROR"};


const char *etg1020_diag_y_string[7] = {
        "ETG10120DIAG_Y_SYSTEM",
        "ETG10120DIAG_Y_GENERAL",
        "ETG10120DIAG_Y_COMMUNICATION",
        "ETG10120DIAG_Y_ENCODER",
        "ETG10120DIAG_Y_DRIVE",
        "ETG10120DIAG_Y_INPUTS",
        "ETG10120DIAG_Y_IOGENERAL"};

const char *etg1020_diag_param_type_string[15] = {
        "ETG10120DIAG_PARAM_TYPE_BOOLEAN",
        "ETG10120DIAG_PARAM_TYPE_INTEGER8",
        "ETG10120DIAG_PARAM_TYPE_INTEGER16",
        "ETG10120DIAG_PARAM_TYPE_INTEGER32",
        "ETG10120DIAG_PARAM_TYPE_UNSIGNED8",
        "ETG10120DIAG_PARAM_TYPE_UNSIGNED16",
        "ETG10120DIAG_PARAM_TYPE_UNSIGNED32",
        "ETG10120DIAG_PARAM_TYPE_REAL32",
        "ETG10120DIAG_PARAM_TYPE_REAL64",
        "ETG10120DIAG_PARAM_TYPE_INTEGER64",
        "ETG10120DIAG_PARAM_TYPE_UNSIGNED64",
        "ETG10120DIAG_PARAM_TYPE_BYTEARRAY",
        "ETG10120DIAG_PARAM_TYPE_ASCIISTRING",
        "ETG10120DIAG_PARAM_TYPE_UNICODESTRING",
        "ETG10120DIAG_PARAM_TYPE_TEXTID"};

const char *etg1020_diag_type_string[4] = {
        "ETG10120DIAG_TYPE_INFO_MESSAGE",
        "ETG10120DIAG_TYPE_WARNING_MESSAGE",
        "ETG10120DIAG_TYPE_ERROR_MESSAGE",
        "ETG10120DIAG_TYPE_UNSPECIFIED_MESSAGE"};


#define ETG1020_DIAG_PARAM_TYPE_LOOKUP_NUM 12
etg1020_diag_param_flag_lookup_t etg1020_diag_param_flag_lookup[ETG1020_DIAG_PARAM_TYPE_LOOKUP_NUM] = {
        {0x0000, ETG10120DIAG_PARAM_TYPE_UNKNOWN,    1},
        {0x0001, ETG10120DIAG_PARAM_TYPE_BOOLEAN,    1},
        {0x0002, ETG10120DIAG_PARAM_TYPE_INTEGER8,   1},
        {0x0003, ETG10120DIAG_PARAM_TYPE_INTEGER16,  2},
        {0x0004, ETG10120DIAG_PARAM_TYPE_INTEGER32,  4},
        {0x0005, ETG10120DIAG_PARAM_TYPE_UNSIGNED8,  1},
        {0x0006, ETG10120DIAG_PARAM_TYPE_UNSIGNED16, 2},
        {0x0007, ETG10120DIAG_PARAM_TYPE_UNSIGNED32, 4},
        {0x0008, ETG10120DIAG_PARAM_TYPE_REAL32,     4},
        {0x0011, ETG10120DIAG_PARAM_TYPE_REAL64,     8},
        {0x0015, ETG10120DIAG_PARAM_TYPE_INTEGER64,  8},
        {0x001B, ETG10120DIAG_PARAM_TYPE_UNSIGNED64, 8}
};

#define ETG1020_DIAG_TEXTID_LOOKUP_NUM 28

etg1020_diag_textid_lookup_t etg1020_diag_textid_lookup[28] = {
        {0x00, ETG10120DIAG_X_SYSTEMINFO, ETG10120DIAG_Y_SYSTEM},
        {0x01, ETG10120DIAG_X_SYSTEMINFO, ETG10120DIAG_Y_GENERAL},
        {0x02, ETG10120DIAG_X_SYSTEMINFO, ETG10120DIAG_Y_COMMUNICATION},
        {0x03, ETG10120DIAG_X_SYSTEMINFO, ETG10120DIAG_Y_ENCODER},
        {0x04, ETG10120DIAG_X_SYSTEMINFO, ETG10120DIAG_Y_DRIVE},
        {0x05, ETG10120DIAG_X_SYSTEMINFO, ETG10120DIAG_Y_INPUTS},
        {0x06, ETG10120DIAG_X_SYSTEMINFO, ETG10120DIAG_Y_IOGENERAL},
        {0x10, ETG10120DIAG_X_INFO,       ETG10120DIAG_Y_SYSTEM},
        {0x11, ETG10120DIAG_X_INFO,       ETG10120DIAG_Y_GENERAL},
        {0x12, ETG10120DIAG_X_INFO,       ETG10120DIAG_Y_COMMUNICATION},
        {0x13, ETG10120DIAG_X_INFO,       ETG10120DIAG_Y_ENCODER},
        {0x14, ETG10120DIAG_X_INFO,       ETG10120DIAG_Y_DRIVE},
        {0x15, ETG10120DIAG_X_INFO,       ETG10120DIAG_Y_INPUTS},
        {0x16, ETG10120DIAG_X_INFO,       ETG10120DIAG_Y_IOGENERAL},
        {0x40, ETG10120DIAG_X_WARNING,    ETG10120DIAG_Y_SYSTEM},
        {0x41, ETG10120DIAG_X_WARNING,    ETG10120DIAG_Y_GENERAL},
        {0x42, ETG10120DIAG_X_WARNING,    ETG10120DIAG_Y_COMMUNICATION},
        {0x43, ETG10120DIAG_X_WARNING,    ETG10120DIAG_Y_ENCODER},
        {0x44, ETG10120DIAG_X_WARNING,    ETG10120DIAG_Y_DRIVE},
        {0x45, ETG10120DIAG_X_WARNING,    ETG10120DIAG_Y_INPUTS},
        {0x46, ETG10120DIAG_X_WARNING,    ETG10120DIAG_Y_IOGENERAL},
        {0x80, ETG10120DIAG_X_ERROR,      ETG10120DIAG_Y_SYSTEM},
        {0x81, ETG10120DIAG_X_ERROR,      ETG10120DIAG_Y_GENERAL},
        {0x82, ETG10120DIAG_X_ERROR,      ETG10120DIAG_Y_COMMUNICATION},
        {0x83, ETG10120DIAG_X_ERROR,      ETG10120DIAG_Y_ENCODER},
        {0x84, ETG10120DIAG_X_ERROR,      ETG10120DIAG_Y_DRIVE},
        {0x85, ETG10120DIAG_X_ERROR,      ETG10120DIAG_Y_INPUTS},
        {0x86, ETG10120DIAG_X_ERROR,      ETG10120DIAG_Y_IOGENERAL},
};

/**
 * @brief decode param flags
 * @param param_flag
 * @return
 *
 * decodes:
 * parameter types (uint8, byte array etc.)
 * parameter number of bytes (length)
 *
 */
void etg1020_diag_decode_param_flag(uint16_t param_flag, etg1020_diag_params_t *result) {


//Bits 12-15 describe the high level param type (non-array or type of array)
//Bits 0-11 describe either the  non-array type (uinit8, int64 etc.) or the length of the array
    if ((param_flag >> 12) == 0x0) {
        //we have a non-array data type
        for (int i = 0; i < ETG1020_DIAG_PARAM_TYPE_LOOKUP_NUM; i++) {
            if (etg1020_diag_param_flag_lookup[i].id == (param_flag & 0b0000111111111111)) {
                result->param_type = etg1020_diag_param_flag_lookup[i].param_type;
                result->param_length = etg1020_diag_param_flag_lookup[i].num_bytes;
            }
        }
    }
    if ((param_flag >> 12) == 0x1) {
        //we have a byte array data type
        result->param_type = ETG10120DIAG_PARAM_TYPE_BYTEARRAY;
        result->param_length = (param_flag & 0b0000111111111111);
    }
    if ((param_flag >> 12) == 0x2) {
//we have a ascii string array data type
        result->param_type = ETG10120DIAG_PARAM_TYPE_ASCIISTRING;
        result->param_length = (param_flag & 0b0000111111111111);
    }
    if ((param_flag >> 12) == 0x3) {
//we have a unicode string array data type
        result->param_type = ETG10120DIAG_PARAM_TYPE_UNICODESTRING;
        result->param_length = (param_flag & 0b0000111111111111);
    }
    if ((param_flag >> 12) == 0x4) {
//we have a text id data type
        result->param_type = ETG10120DIAG_PARAM_TYPE_TEXTID;
        result->param_length = (param_flag & 0b0000111111111111);
        if (result->param_length != 2) {
            //this is a text id and should always be 2 bytes long
            result->param_type = ETG10120DIAG_PARAM_TYPE_UNKNOWN;
            result->param_length = (param_flag & 0b0000111111111111);
        }
    }
    if ((param_flag >> 12) > 0x4) {
        result->param_type = ETG10120DIAG_PARAM_TYPE_UNKNOWN;
        result->param_length = (param_flag & 0b0000111111111111);
    }
}

/**
 * @brief decodes text id
 * @param textid
 * @return
 *
 * decodes:
 * text id long and short codes (references the diag message text)
 * x and y - the system and sub-system the diag message orignated from
 */
void etg1020_diag_textid_decode(uint16_t textid, etg1020_diag_textid_t *result) {


    result->long_code = textid;

    uint8_t textid_xy = (uint8_t) (textid >> 8);
    for (int i = 0; i < ETG1020_DIAG_TEXTID_LOOKUP_NUM; i++) {
        if (etg1020_diag_textid_lookup[i].code == textid_xy) {
            result->x = etg1020_diag_textid_lookup[i].x;
            result->y = etg1020_diag_textid_lookup[i].y;
            break;
        }
    }
    result->short_code = ((uint8_t) (textid & 0xff));

}

//todo doxygen
/**
 * @brief decodes the flags
 * @param flags
 * @return
 *
 * decodes:
 * diag type
 * nunber of parameters
 *
 */
void etg1020_diag_flag_decode(uint16_t flags, etg1020_diag_flags_t *result) {

    if (BIT_CHECK(flags, 4)) {
        result->local_time_stamp = true;
    } else {
        result->local_time_stamp = false;
    }

    //pull out 4 most LSBs
    uint8_t temp_diag_type = (uint8_t) (flags & 0x7);

    switch (temp_diag_type) {
        case 0:
            result->etg1020_diag_type = ETG10120DIAG_TYPE_INFO_MESSAGE;
            break;
        case 1:
            result->etg1020_diag_type = ETG10120DIAG_TYPE_WARNING_MESSAGE;
            break;
        case 2:
            result->etg1020_diag_type = ETG10120DIAG_TYPE_ERROR_MESSAGE;
            break;
        default:
            result->etg1020_diag_type = ETG10120DIAG_TYPE_UNSPECIFIED_MESSAGE;
    }

    result->number_of_parameters = (uint8_t) (flags >> 8);

}

/**
 * @brief
 * @param diag_code
 * @return
 */
void etg1020_diag_code_decode(uint32_t diag_code, etg1020_diag_code_t *result) {

//    printf("diag short_code in fn:%d\n", diag_code);
//
//    printf("diag short_code lower sixteen:%d\n", (diag_code & 0x0000FFFF));

// 0xDfff - 57343
// 0xE7FF - 59391
// 0xE800 - 59392
// 0xEDFF - 60927
// 0xEFFF - 61439

    if ((diag_code & 0x0000FFFF) <= 0xDfff) {
        result->etg1020_diag_code_type = ETG10120DIAG_CODE_TYPE_UNSPECIFIED;
    } else if ((diag_code & 0x0000FFFF) <= 0xE7FF) {
        result->etg1020_diag_code_type = ETG10120DIAG_CODE_TYPE_MFR_SPECIFIC;
    } else if ((diag_code & 0x0000FFFF) == 0xE800) {
        result->etg1020_diag_code_type = ETG10120DIAG_CODE_TYPE_EMERGENCY_CODE_DS301;
    } else if ((diag_code & 0x0000FFFF) <= 0xEDFF) {
        result->etg1020_diag_code_type = ETG10120DIAG_CODE_TYPE_UNSPECIFIED;
    } else if ((diag_code & 0x0000FFFF) <= 0xEFFF) {
        result->etg1020_diag_code_type = ETG10120DIAG_CODE_TYPE_PROFILE_SPECIFIC;
    } else {
        result->etg1020_diag_code_type = ETG10120DIAG_CODE_TYPE_UNSPECIFIED;
    }
    result->code_specific = (uint16_t) diag_code << 16;
}

/**
 * @brief decodes the diag message timestamp
 * @param nsec_array (8 bytes from diag message)
 * @return time adjusted to EtherCAT clock in SECONDS
 */
time_t etg1020_diag_timestamp_decode(const uint8_t *nsec_array) {

    uint64_t nsecs =
            (uint64_t) nsec_array[0] |
            (uint64_t) nsec_array[1] << 8 |
            (uint64_t) nsec_array[2] << 16 |
            (uint64_t) nsec_array[3] << 24 |
            (uint64_t) nsec_array[4] << 32 |
            (uint64_t) nsec_array[5] << 40 |
            (uint64_t) nsec_array[6] << 48 |
            (uint64_t) nsec_array[7] << 56;


    time_t secs = nsecs / NSEC_PER_SEC;
    secs += 946684800UL;
    return secs;
}

/*
 * 0: Diag short_code 0
 * 1: Diag short_code 1
 * 2: Diag short_code 2
 * 3: Diag short_code 3
 * 4: Flag 0
 * 5: Flag 1
 * 6: TextID 0
 * 7: TextID 1
 * 9: Timestamp 0
 * 10: Timestamp 1
 * 11: Timestamp 2
 * 12: Timestamp 3
 * 13: Timestamp 4
 * 14: Timestamp 5
 * 15: Timestamp 6
 * 16: Timestamp 7
 * 17: Param 1 type 0
 * 18: Param 1 type 1
 * 19: Param 1 val
 * 20:
 * 21:
 * 22:
 * 23:
 * 24:
 * 25:
 * 26:
 * 27:
 */

//so with 28 bytes in the message (EL7211) there is only room for 3 uint8 or 2 uint 16
//some of the messages are like {"%s: Unsuccessful FwdClose-Response received from %d.%d.%d. %d (%s) (Error: %u)"} - that aint going to fit



void etg1020_diag_decode_params(const uint8_t *message, etg1020_diag_message_t *result) {
    if (result->diag_flags.number_of_parameters) {
        //we have a parameter
        uint8_t current_byte = 16;
        for (int i = 0; i < result->diag_flags.number_of_parameters; i++) {
            etg1020_diag_decode_param_flag(
                    (message[current_byte + 1] << 8) | (message[current_byte]), &result->diag_params[i]);
            if ((current_byte + result->diag_params[i].param_length + 2) > 28) {
                break;
            }
            //if we dont have an array type pull out the non array params
            //todo what about floats??
            if ((result->diag_params[i].param_type != ETG10120DIAG_PARAM_TYPE_BYTEARRAY) &&
                (result->diag_params[i].param_type != ETG10120DIAG_PARAM_TYPE_UNICODESTRING) &&
                (result->diag_params[i].param_type != ETG10120DIAG_PARAM_TYPE_ASCIISTRING) &&
                (result->diag_params[i].param_type != ETG10120DIAG_PARAM_TYPE_TEXTID)) {
                if (result->diag_params[i].param_length == 1) {
                    result->diag_params[i].param_value = (uint64_t) message[current_byte + 2];
                } else if (result->diag_params[i].param_length == 2) {
                    result->diag_params[i].param_value = ((uint64_t) (message[current_byte + 3])) << 8 |
                                                         ((uint64_t) (message[current_byte + 2]));

                } else if (result->diag_params[i].param_length == 4) {
                    result->diag_params[i].param_value = (((uint64_t) (message[current_byte + 5])) << 24 |
                                                          ((uint64_t) (message[current_byte + 4])) << 16 |
                                                          ((uint64_t) (message[current_byte + 3])) << 8 |
                                                          ((uint64_t) (message[current_byte + 2])));
                } else if (result->diag_params[i].param_length == 8) {
                    result->diag_params[i].param_value = (((uint64_t) (message[current_byte + 9])) << 56 |
                                                          ((uint64_t) (message[current_byte + 8])) << 48 |
                                                          ((uint64_t) (message[current_byte + 7])) << 40 |
                                                          ((uint64_t) (message[current_byte + 6])) << 32 |
                                                          ((uint64_t) (message[current_byte + 5])) << 24 |
                                                          ((uint64_t) (message[current_byte + 4])) << 16 |
                                                          ((uint64_t) (message[current_byte + 3])) << 8 |
                                                          ((uint64_t) (message[current_byte + 2])));
                }
            }
            if (result->diag_params[i].param_type == ETG10120DIAG_PARAM_TYPE_BYTEARRAY) {
                for (int j = 0; j < result->diag_params[i].param_length; j++) {
                    result->diag_params->param_byte_array[j] = message[current_byte + 2 + j];
                }
            }
            if (result->diag_params[i].param_type == ETG10120DIAG_PARAM_TYPE_ASCIISTRING) {

                for (int j = 0; j < result->diag_params[i].param_length; j++) {

                    result->diag_params->param_byte_array[j] = message[current_byte + 2 + j];

                }
            }
            if (result->diag_params[i].param_type == ETG10120DIAG_PARAM_TYPE_UNICODESTRING) {
//todo
            }
            if (result->diag_params[i].param_type == ETG10120DIAG_PARAM_TYPE_TEXTID) {
//todo
            }
//shift current byte over
//todo shift for array types
            current_byte = current_byte + result->diag_params[i].param_length + 2;

//        }
        }
    }

    int text_array_index = 0;
    for (text_array_index = 0; text_array_index < NUM_OF_EL7211_DIAG_STRINGS; text_array_index++) {
        if (etg1020_diag_text[text_array_index].text_id == result->diag_textid.long_code) {
//            printf("%d\n", text_array_index);
            break;

        }
    }

    uint8_t *err_msg = (uint8_t *) &etg1020_diag_text[text_array_index].text_string[0];

    if (etg1020_diag_text[text_array_index].num_params > result->diag_flags.number_of_parameters) {
        //num params in message dont match the error string we are about to print
        LL_FATAL("params mismatch");
    }

    /* %c, %d, %u, and %s form ETG2000 - If the order of the parameters is not the order of the parameters within the message the position can be
changed by adding “:n” to the specifier. Counting starts with 0.*/


    uint8_t arg_count = 0;
    for (int i = 0; i < strlen(etg1020_diag_text[text_array_index].text_string); i++) {
//todo %d vs %u
        if (err_msg[i] != '%') {
            sprintf((char *) &result->diag_message_text[i], "%c", err_msg[i]);
        } else {
            if (err_msg[i + 1] == 'd') {
                switch (result->diag_params[arg_count].param_type) {
                    case ETG10120DIAG_PARAM_TYPE_BOOLEAN:
                        if (result->diag_params[arg_count].param_value == 1) {
                            sprintf((char *) &result->diag_message_text[i], "true");
                        } else {
                            sprintf((char *) &result->diag_message_text[i], "false");
                        }
                        break;
                    case ETG10120DIAG_PARAM_TYPE_INTEGER8:
                        sprintf((char *) &result->diag_message_text[i], "%u",
                                (int8_t) result->diag_params[arg_count].param_value);
                        break;

                    case ETG10120DIAG_PARAM_TYPE_INTEGER16:
                        sprintf((char *) &result->diag_message_text[i], "%d",
                                (int16_t) result->diag_params[arg_count].param_value);
                        break;
                    case ETG10120DIAG_PARAM_TYPE_INTEGER32:
                        sprintf((char *) &result->diag_message_text[i], "%d",
                                (int32_t) result->diag_params[arg_count].param_value);
                        break;
                    case ETG10120DIAG_PARAM_TYPE_UNSIGNED8:
                        sprintf((char *) &result->diag_message_text[i], "%d",
                                (uint8_t) result->diag_params[arg_count].param_value);
                        break;
                    case ETG10120DIAG_PARAM_TYPE_UNSIGNED16:
                        sprintf((char *) &result->diag_message_text[i], "%d",
                                (uint16_t) result->diag_params[arg_count].param_value);
                        break;
                    case ETG10120DIAG_PARAM_TYPE_UNSIGNED32:
                        sprintf((char *) &result->diag_message_text[i], "%d",
                                (uint32_t) result->diag_params[arg_count].param_value);
                        break;
                    case ETG10120DIAG_PARAM_TYPE_INTEGER64:
                        sprintf((char *) &result->diag_message_text[i], "%ld",
                                (int64_t) result->diag_params[arg_count].param_value);
                        break;
                    case ETG10120DIAG_PARAM_TYPE_UNSIGNED64:
                        sprintf((char *) &result->diag_message_text[i], "%ld",
                                (uint64_t) result->diag_params[arg_count].param_value);
                        break;
                    default:
                        LL_FATAL("type not found");
                }
                if ((err_msg[i + 1] == 'x') || (err_msg[i + 1] == 'X')) {
                    switch (result->diag_params[arg_count].param_type) {
                        case ETG10120DIAG_PARAM_TYPE_INTEGER8:
                            sprintf((char *) &result->diag_message_text[i], "%x",
                                    (int8_t) result->diag_params[arg_count].param_value);
                            break;

                        case ETG10120DIAG_PARAM_TYPE_INTEGER16:
                            sprintf((char *) &result->diag_message_text[i], "%x",
                                    (int16_t) result->diag_params[arg_count].param_value);
                            break;
                        case ETG10120DIAG_PARAM_TYPE_INTEGER32:
                            sprintf((char *) &result->diag_message_text[i], "%x",
                                    (int32_t) result->diag_params[arg_count].param_value);
                            break;
                        case ETG10120DIAG_PARAM_TYPE_UNSIGNED8:
                            sprintf((char *) &result->diag_message_text[i], "%x",
                                    (uint8_t) result->diag_params[arg_count].param_value);
                            break;
                        case ETG10120DIAG_PARAM_TYPE_UNSIGNED16:
                            sprintf((char *) &result->diag_message_text[i], "%x",
                                    (uint16_t) result->diag_params[arg_count].param_value);
                            break;
                        case ETG10120DIAG_PARAM_TYPE_UNSIGNED32:
                            sprintf((char *) &result->diag_message_text[i], "%x",
                                    (uint32_t) result->diag_params[arg_count].param_value);
                            break;

                        case ETG10120DIAG_PARAM_TYPE_INTEGER64:
                            sprintf((char *) &result->diag_message_text[i], "%lx",
                                    (int64_t) result->diag_params[arg_count].param_value);
                            break;
                        case ETG10120DIAG_PARAM_TYPE_UNSIGNED64:
                            sprintf((char *) &result->diag_message_text[i], "%lx",
                                    (uint64_t) result->diag_params[arg_count].param_value);
                            break;
                        default:
                            LL_FATAL("type not found");

                    }
                }
                if (err_msg[i + 1] == 'f') {
                    switch (result->diag_params[arg_count].param_type) {
                        case ETG10120DIAG_PARAM_TYPE_REAL32:
                            sprintf((char *) &result->diag_message_text, "%f",
                                    (float) result->diag_params[arg_count].param_value);
                            break;
                        case ETG10120DIAG_PARAM_TYPE_REAL64:
                            sprintf((char *) &result->diag_message_text, "%lf",
                                    (double) result->diag_params[arg_count].param_value);
                            break;
                        default:
                            LL_FATAL("type not found");
                    }
                }
                arg_count++;
                i++;
            }
            if (err_msg[i + 1] == 's') {

                if (result->diag_params[arg_count].param_type == ETG10120DIAG_PARAM_TYPE_BYTEARRAY) {
                    sprintf((char *) &result->diag_message_text[i], "%s",
                            result->diag_params[arg_count].param_byte_array);
                }
                if (result->diag_params[arg_count].param_type == ETG10120DIAG_PARAM_TYPE_ASCIISTRING) {
                    sprintf((char *) &result->diag_message_text[i], "%s",
                            result->diag_params[arg_count].param_ascii_array);
                }
                if (result->diag_params[arg_count].param_type ==
                    ETG10120DIAG_PARAM_TYPE_UNICODESTRING) {
                    LL_FATAL("unicode message not supoorted");
                }

                if (result->diag_params[arg_count].param_type == ETG10120DIAG_PARAM_TYPE_TEXTID) {
                    LL_FATAL("textid message not supoorted");
                    //todo
                }
                arg_count++;
                i++;
            }
        }
    }


}

/**
 * @brief decode full diag message
 * @param [IN] message (byte array to be decoded, [OUT] result struct containing full diag message decode including the full diag message string
 * @return true valid message, invalid message
 *
 */
bool etg1020_diag_full_decode(uint8_t *message, etg1020_diag_message_t *result) {

    /*decode flags */
    etg1020_diag_flag_decode((message[5] << 8) | (message[4]), &result->diag_flags);
    /* decode textid */
    etg1020_diag_textid_decode((message[7] << 8) | (message[6]), &result->diag_textid);

//    if (result->diag_textid.long_code==0){
//        return false;
//    }

    /* decode diag code */
    etg1020_diag_code_decode(
            (uint32_t) ((message[3] << 24) | (message[2] << 16) | (message[1] << 8) | message[0]), &result->diag_code);

    /* decode time stamp */
    result->time_stamp_secs = etg1020_diag_timestamp_decode(&message[8]);


    /* decode params */

    etg1020_diag_decode_params(message, result);

    return true;

    /* if we have any parameters then lets get them out */
//    if (result->diag_flags.number_of_parameters) {
//        //we have a parameter
//        uint8_t current_byte = 16;
//        for (int i = 0; i < result->diag_flags.number_of_parameters; i++) {
//            etg1020_diag_decode_param_flag(
//                    (message[current_byte + 1] << 8) | (message[current_byte]), &result->diag_params[i]);
//
//            //if we dont have an array type pull out the non array params
//            //todo what about floats??
//        if ((result->diag_params[i].param_type!=ETG10120DIAG_PARAM_TYPE_BYTEARRAY) && (result->diag_params[i].param_type!=ETG10120DIAG_PARAM_TYPE_UNICODESTRING) && (result->diag_params[i].param_type!=ETG10120DIAG_PARAM_TYPE_ASCIISTRING) && (result->diag_params[i].param_type!=ETG10120DIAG_PARAM_TYPE_TEXTID) ) {
//            if (result->diag_params[i].param_length == 1) {
//                result->diag_params[i].param_value = (uint64_t)message[current_byte + 2];
//            } else if (result->diag_params[i].param_length == 2) {
//                result->diag_params[i].param_value = ((uint64_t)(message[current_byte + 2])) << 8 |
//                        ((uint64_t)(message[current_byte + 3]));
//            }
//            else if (result->diag_params[i].param_length == 4) {
//                result->diag_params[i].param_value = (((uint64_t)(message[current_byte + 2])) << 24 |
//                        ((uint64_t)(message[current_byte + 3])) <<16 | ((uint64_t)(message[current_byte + 4])) <<8 | ((uint64_t)(message[current_byte + 5])));
//            }
//
//            else if (result->diag_params[i].param_length == 8) {
//                result->diag_params[i].param_value = (((uint64_t)(message[current_byte + 2])) << 56 |
//                        ((uint64_t)(message[current_byte + 3])) <<48 |
//                        ((uint64_t)(message[current_byte + 4])) <<40 | ((uint64_t)(message[current_byte + 5])) << 32 |
//                        ((uint64_t)(message[current_byte + 6])) << 24 | ((uint64_t)(message[current_byte + 7])) << 16 |
//                        ((uint64_t)(message[current_byte + 8])) << 8 | ((uint64_t)(message[current_byte + 9])));
//            }
//        }
//            if (result->diag_params[i].param_type == ETG10120DIAG_PARAM_TYPE_BYTEARRAY) {
//                for (int j = 0; j < result->diag_params[i].param_length; j++) {
//                    result->diag_params->param_byte_array[j] = message[current_byte + 2 + j];
//                }
//            }
//            if (result->diag_params[i].param_type == ETG10120DIAG_PARAM_TYPE_ASCIISTRING) {
//
//                for (int j = 0; j < result->diag_params[i].param_length; j++) {
//
//                    result->diag_params->param_byte_array[j] = message[current_byte + 2 + j];
//
//                }
//            }
//            if (result->diag_params[i].param_type == ETG10120DIAG_PARAM_TYPE_UNICODESTRING) {
////todo
//            }
//            if (result->diag_params[i].param_type == ETG10120DIAG_PARAM_TYPE_TEXTID) {
////todo
//            }
////shift current byte over
////todo shift for array types
//            current_byte = current_byte + result->diag_params[i].param_length + 2;
//
////        }
//        }
//    }
//
//    int text_array_index = 0;
//    for (text_array_index = 0; text_array_index < NUM_OF_EL7211_DIAG_STRINGS; text_array_index++) {
//        if (etg1020_diag_text[text_array_index].text_id == result->diag_textid.long_code) {
//            printf("%d\n", text_array_index);
//            break;
//
//        }
//    }
//
//    uint8_t *err_msg = &etg1020_diag_text[text_array_index].text_string[0];
//
//    if (etg1020_diag_text[text_array_index].num_params > result->diag_flags.number_of_parameters) {
//        //num params in message dont match the error string we are about to print
//        assert(0 && "params mismatch");
//    }
//
//    /* %c, %d, %u, and %s form ETG2000 - If the order of the parameters is not the order of the parameters within the message the position can be
//changed by adding “:n” to the specifier. Counting starts with 0.*/
//
//
//    uint8_t arg_count = 0;
//    for (int i = 0; i < strlen(etg1020_diag_text[text_array_index].text_string); i++) {
////todo %d vs %u
//        if (err_msg[i] != '%') {
//            sprintf(&result->diag_message_text[i], "%c", err_msg[i]);
//        } else {
//            if (err_msg[i + 1] == 'd') {
//                switch (result->diag_params[arg_count].param_type) {
//                    case ETG10120DIAG_PARAM_TYPE_BOOLEAN:
//                        if (result->diag_params[arg_count].param_value == 1) {
//                            sprintf(&result->diag_message_text[i], "true");
//                        } else {
//                            sprintf(&result->diag_message_text[i], "false");
//                        }
//                        break;
//                    case ETG10120DIAG_PARAM_TYPE_INTEGER8:
//                        sprintf(&result->diag_message_text[i], "%u",
//                                (int8_t) result->diag_params[arg_count].param_value);
//                        break;
//
//                    case ETG10120DIAG_PARAM_TYPE_INTEGER16:
//                        sprintf(&result->diag_message_text[i], "%d",
//                                (int16_t) result->diag_params[arg_count].param_value);
//                        break;
//                    case ETG10120DIAG_PARAM_TYPE_INTEGER32:
//                        sprintf(&result->diag_message_text[i], "%d",
//                                (int32_t) result->diag_params[arg_count].param_value);
//                        break;
//                    case ETG10120DIAG_PARAM_TYPE_UNSIGNED8:
//                        sprintf(&result->diag_message_text[i], "%d",
//                                (uint8_t) result->diag_params[arg_count].param_value);
//                        break;
//                    case ETG10120DIAG_PARAM_TYPE_UNSIGNED16:
//                        sprintf(&result->diag_message_text[i], "%d",
//                                (uint16_t) result->diag_params[arg_count].param_value);
//                        break;
//                    case ETG10120DIAG_PARAM_TYPE_UNSIGNED32:
//                        sprintf(&result->diag_message_text[i], "%d",
//                                (uint32_t) result->diag_params[arg_count].param_value);
//                        break;
//                    case ETG10120DIAG_PARAM_TYPE_INTEGER64:
//                        sprintf(&result->diag_message_text[i], "%ld",
//                                (int64_t) result->diag_params[arg_count].param_value);
//                        break;
//                    case ETG10120DIAG_PARAM_TYPE_UNSIGNED64:
//                        sprintf(&result->diag_message_text[i], "%ld",
//                                (uint64_t) result->diag_params[arg_count].param_value);
//                        break;
//                    default:
//                        assert(0 && "type not found");
//                }
//                if ((err_msg[i + 1] == 'x') || (err_msg[i + 1] == 'X')){
//                    switch (result->diag_params[arg_count].param_type) {
//                        case ETG10120DIAG_PARAM_TYPE_INTEGER8:
//                            sprintf(&result->diag_message_text[i], "%x",
//                                    (int8_t) result->diag_params[arg_count].param_value);
//                            break;
//
//                        case ETG10120DIAG_PARAM_TYPE_INTEGER16:
//                            sprintf(&result->diag_message_text[i], "%x",
//                                    (int16_t) result->diag_params[arg_count].param_value);
//                            break;
//                        case ETG10120DIAG_PARAM_TYPE_INTEGER32:
//                            sprintf(&result->diag_message_text[i], "%x",
//                                    (int32_t) result->diag_params[arg_count].param_value);
//                            break;
//                        case ETG10120DIAG_PARAM_TYPE_UNSIGNED8:
//                            sprintf(&result->diag_message_text[i], "%x",
//                                    (uint8_t) result->diag_params[arg_count].param_value);
//                            break;
//                        case ETG10120DIAG_PARAM_TYPE_UNSIGNED16:
//                            sprintf(&result->diag_message_text[i], "%x",
//                                    (uint16_t) result->diag_params[arg_count].param_value);
//                            break;
//                        case ETG10120DIAG_PARAM_TYPE_UNSIGNED32:
//                            sprintf(&result->diag_message_text[i], "%x",
//                                    (uint32_t) result->diag_params[arg_count].param_value);
//                            break;
//
//                        case ETG10120DIAG_PARAM_TYPE_INTEGER64:
//                            sprintf(&result->diag_message_text[i], "%lx",
//                                    (int64_t) result->diag_params[arg_count].param_value);
//                            break;
//                        case ETG10120DIAG_PARAM_TYPE_UNSIGNED64:
//                            sprintf(&result->diag_message_text[i], "%lx",
//                                    (uint64_t) result->diag_params[arg_count].param_value);
//                            break;
//                        default:
//                            assert(0 && "type not found");
//
//                    }
//                }
//                if (err_msg[i + 1] == 'f') {
//                    switch (result->diag_params[arg_count].param_type) {
//                        case ETG10120DIAG_PARAM_TYPE_REAL32:
//                            sprintf(&result->diag_message_text, "%f",
//                                    (float) result->diag_params[arg_count].param_value);
//                            break;
//                        case ETG10120DIAG_PARAM_TYPE_REAL64:
//                            sprintf(&result->diag_message_text, "%lf",
//                                    (double) result->diag_params[arg_count].param_value);
//                            break;
//                        default:
//                            assert(0 && "type not found");
//                    }
//                }
//                arg_count++;
//                i++;
//            }
//            if (err_msg[i + 1] == 's') {
//
//                if (result->diag_params[arg_count].param_type == ETG10120DIAG_PARAM_TYPE_BYTEARRAY) {
//                    sprintf(&result->diag_message_text[i], "%s",
//                            result->diag_params[arg_count].param_byte_array);
//                }
//                if (result->diag_params[arg_count].param_type == ETG10120DIAG_PARAM_TYPE_ASCIISTRING) {
//                    sprintf(&result->diag_message_text[i], "%s",
//                            result->diag_params[arg_count].param_ascii_array);
//                }
//                if (result->diag_params[arg_count].param_type ==
//                    ETG10120DIAG_PARAM_TYPE_UNICODESTRING) {
//                    assert(0 && "unicode message not supoorted");
//                }
//
//                if (result->diag_params[arg_count].param_type == ETG10120DIAG_PARAM_TYPE_TEXTID) {
//                    assert(0 && "textid message not supoorted");
//                    //todo
//                }
//                arg_count++;
//                i++;
//            }
//        }
//    }
}
