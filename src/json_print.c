/**
 ******************************************************************************
 * @file           :  json_print.c
 * @brief          :  ???
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include <stdio.h>
#include "mjson.h"

/**
 * @brief formatted output to stdout of a JSON buffer
 * @param json
 */
void json_print( char * json) {

    int koff, klen, voff, vlen, vtype, off;

    for (off = 0; (off = mjson_next(json, strlen(json), off, &koff, &klen,
                                    &voff, &vlen, &vtype)) != 0;) {
        printf("key: %.*s, value: %.*s\n", klen, json + koff, vlen, json + voff);
    }

}