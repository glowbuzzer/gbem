/**
 ******************************************************************************
 * @file           :  debug_ini.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include <stdio.h>
#include "minIni.h"
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "gbem_config.h"
#include "user_message.h"
#include "map.h"


/*
turn_off_limits_checking = true
*/

#define MATCH(x, y) (strcasecmp(x, y) == 0)
#define NAMEOF(s) strchr((const char *)(#s), '>') == NULL \
? #s : (strchr((const char *)(#s), '>') + 1)


typedef struct {
    uint32_t value;
    bool enable_limits_checking;
} debug_settings_t;

static debug_settings_t debug_settings;


bool check_debug_ini_exists(void) {
    FILE *fp;
    fp = fopen(DEBUG_INI_FILENAME, "r");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}

static void parse_section(debug_settings_t *config, const char *key,
                          const char *value) {
    // printf("%s\n", value);

    if (MATCH("value", key)) {
        config->value = (uint32_t) atoi(value);
    } else if (MATCH("enable_limits_checking", key)) {
        config->enable_limits_checking = value;
    } else {
        UM_FATAL("GBEM: Error parsing ini file. Unknown key: %s", key);
    }
}


static gberror_t config_parser(const char *section, const char *key,
                               const char *value, void *config_v) {
    parse_section(config_v, key, value);

    return 1;
}

uint8_t get_debug_ini_keys(void) {
    int s;
    char section[40];
    uint8_t count = 0;
    for (s = 0; ini_getkey(NULL, s, section, sizeof section, DEBUG_INI_FILENAME) > 0; s++) {
        count++;
    }
    return count;
}


gberror_t read_debug_ini(uint8_t *number_of_items_found) {
    if (!ini_browse(config_parser, &debug_settings, DEBUG_INI_FILENAME)) {
        return E_LOOKUP_FAILED;
    }
    *number_of_items_found = get_debug_ini_keys();
    return E_SUCCESS;
}
