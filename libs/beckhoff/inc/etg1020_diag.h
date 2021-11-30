/**
 ******************************************************************************
 * @file           :  etg1020_diag.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef MONOREPO_ETG1020_DIAG_H
#define MONOREPO_ETG1020_DIAG_H

#include "stdint.h"
#include <time.h>
#include <stdbool.h>

//Diagnosis short_code (4 bytes) – mandatory
//Flags (2 bytes) – mandatory
//Text ID (2 bytes) – mandatory
//Timestamp (8 bytes) – mandatory
//Optional parameters – optional


//[dddd cccc ffff mmmm tttttttttttttttt pppp(i) kk(i)]
//
//dddd = DiagCode: z.B. (00 E0): 0xE000 standard Beckhoff Message
//
//cccc = ProductCode (21 50): 0x5021 = Code for ELM
//
//ffff = Flags, amongst others indication of the number (i) of parameters (pppp kk) to be given.
//E.g. (02 00) = 0x0002; bit 4 is set, when not in DC operation
//
//mmmm = Message ID – respective text can be found here: basic principles of diag messages
//
//tttttttttttttttt = TimeStamp
//
//pppp(i) = Datatype of the parameters, e.g. (05 00) = 0x0005 for datatype UINT8
//
//kk(i) = parameter value
//
//e.g. 2 x UINT8 parameters as indicated by ffff (Flags), with values 0x3C and 0x89
//= “05003C050089“

//This is the first two bytes of the diag message
//4 bytes long
//Bit 0-15	        Bit 16-31
//0x0000-0xDFFF	    not used
//0xE000-0xE7FF	    can be used manufacturer specific
//0xE800	        Emergency Error Code as  defined in DS301 or DS4xxx
//0xE801-0xEDFF	    reserved for future standardization
//0xEE00-0xEFFF	    Profile specific
//0xF000-0xFFFF	    not used

typedef enum {
    ETG10120DIAG_CODE_TYPE_MFR_SPECIFIC,
    ETG10120DIAG_CODE_TYPE_EMERGENCY_CODE_DS301,
    ETG10120DIAG_CODE_TYPE_PROFILE_SPECIFIC,
    ETG10120DIAG_CODE_TYPE_UNSPECIFIED
} etg1020_diag_code_type_t;

const char * etg1020_diag_code_type_string[4];


/*
 * Flags:
 * Bit 0-3:
 *      0: Info message
 *      1: Warning message
 *      2: Error message
 * 3-15: Reserved for future use
 * Bit 4: Time stamp is a local time stamp
 * Bit 5-7:	Reserved for future use
 * Bit 8-15: Number of parameters in this diagnosis message
 *
 */

//ETG1020 diag message types
typedef enum {
    ETG10120DIAG_TYPE_INFO_MESSAGE,
    ETG10120DIAG_TYPE_WARNING_MESSAGE,
    ETG10120DIAG_TYPE_ERROR_MESSAGE,
    ETG10120DIAG_TYPE_UNSPECIFIED_MESSAGE
} etg1020_diag_type_t;
const char * etg1020_diag_type_string[4];



//the text id includes x and y which indicate the system and sub-system responsible for raising the diag message (this is Beckhoff specific not ETC1020)
typedef enum {
    ETG10120DIAG_X_SYSTEMINFO,
    ETG10120DIAG_X_INFO,
    ETG10120DIAG_X_WARNING,
    ETG10120DIAG_X_ERROR,
} etg1020_diag_x_t;
const char * etg1020_diag_x_string[4];

//the text id includes x and y which indicate the system and sub-system responsible for raising the diag message (this is Beckhoff specific not ETC1020)
typedef enum {
    ETG10120DIAG_Y_SYSTEM,
    ETG10120DIAG_Y_GENERAL,
    ETG10120DIAG_Y_COMMUNICATION,
    ETG10120DIAG_Y_ENCODER,
    ETG10120DIAG_Y_DRIVE,
    ETG10120DIAG_Y_INPUTS,
    ETG10120DIAG_Y_IOGENERAL,
} etg1020_diag_y_t;
const char * etg1020_diag_y_string[7];

//types of parameters in diag message including array types
typedef enum {
    ETG10120DIAG_PARAM_TYPE_UNKNOWN,
    ETG10120DIAG_PARAM_TYPE_BOOLEAN,
    ETG10120DIAG_PARAM_TYPE_INTEGER8,
    ETG10120DIAG_PARAM_TYPE_INTEGER16,
    ETG10120DIAG_PARAM_TYPE_INTEGER32,
    ETG10120DIAG_PARAM_TYPE_UNSIGNED8,
    ETG10120DIAG_PARAM_TYPE_UNSIGNED16,
    ETG10120DIAG_PARAM_TYPE_UNSIGNED32,
    ETG10120DIAG_PARAM_TYPE_REAL32,
    ETG10120DIAG_PARAM_TYPE_REAL64,
    ETG10120DIAG_PARAM_TYPE_INTEGER64,
    ETG10120DIAG_PARAM_TYPE_UNSIGNED64,
    ETG10120DIAG_PARAM_TYPE_BYTEARRAY,
    ETG10120DIAG_PARAM_TYPE_ASCIISTRING,
    ETG10120DIAG_PARAM_TYPE_UNICODESTRING,
    ETG10120DIAG_PARAM_TYPE_TEXTID,
} etg1020_diag_param_type_t;
const char * etg1020_diag_param_type_string[15];

//struct for text id lookup
typedef struct {
    uint8_t code;
    etg1020_diag_x_t x;
    etg1020_diag_y_t y;
} etg1020_diag_textid_lookup_t;



//struct for decide diag code
typedef struct {
    etg1020_diag_code_type_t etg1020_diag_code_type;
    uint16_t code_specific;
} etg1020_diag_code_t;


//struct for decoded text id
typedef struct {
    uint16_t long_code;
    uint8_t short_code;
    etg1020_diag_x_t x;
    etg1020_diag_y_t y;
} etg1020_diag_textid_t;


//struct for decoded flags
typedef struct {
    etg1020_diag_type_t etg1020_diag_type;
    uint8_t number_of_parameters;
    bool local_time_stamp;
} etg1020_diag_flags_t;

//struct for decoded parameters
typedef struct {
    etg1020_diag_param_type_t param_type;
    uint8_t param_length;
    uint64_t param_value;
    uint8_t param_byte_array[50];
    uint8_t param_ascii_array[50];
    uint16_t param_unicode_array[100];
    uint16_t textid;
} etg1020_diag_params_t;

//struct for decoded message
typedef struct {
    etg1020_diag_code_t diag_code;
    etg1020_diag_flags_t diag_flags;
    etg1020_diag_textid_t diag_textid;
    etg1020_diag_params_t diag_params[15];
    uint8_t diag_message_text[200];
    time_t time_stamp_secs;
}etg1020_diag_message_t;

//struct for parameter flags lookup array
typedef struct {
    uint8_t id;
    etg1020_diag_param_type_t param_type;
    uint8_t num_bytes;
} etg1020_diag_param_flag_lookup_t;

#define ETG1020_DIAG_PARAM_TYPE_LOOKUP_NUM 12
//declaration of lookup arrays
etg1020_diag_param_flag_lookup_t etg1020_diag_param_flag_lookup[ETG1020_DIAG_PARAM_TYPE_LOOKUP_NUM];
etg1020_diag_textid_lookup_t etg1020_diag_textid_lookup[28];


//declaration of functions
void etg1020_diag_decode_param_flag(uint16_t param_flag, etg1020_diag_params_t * result);
void etg1020_diag_textid_decode(uint16_t textid, etg1020_diag_textid_t * result);
void etg1020_diag_flag_decode(uint16_t flags, etg1020_diag_flags_t * result);
void etg1020_diag_code_decode(uint32_t diag_code, etg1020_diag_code_t *result);
bool etg1020_diag_full_decode(uint8_t * message, etg1020_diag_message_t * result);
time_t etg1020_diag_timestamp_decode(const uint8_t * nsec_array );


#endif //MONOREPO_ETG1020_DIAG_H
