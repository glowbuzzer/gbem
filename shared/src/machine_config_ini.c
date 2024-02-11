/**
 ******************************************************************************
 * @file           :  machine_config_ini.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include <ctype.h>
#include <stdio.h>
#include "minIni.h"
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "gbem_config.h"
#include "user_message.h"
#include "map.h"
#include "control.h"


/*
machine_name = MACHINE_AW_J17
enable_optional_slave_1 = true
enable_optional_slave_2 = true
enable_optional_slave_3 = true
enable_optional_slave_4 = true
enable_optional_slave_5 = true
enable_optional_slave_6 = true
enable_optional_slave_7 = true
enable_optional_slave_8 = true
enable_optional_slave_9 = true
enable_optional_slave_10 = true

 */


#define MATCH(x, y) (strcasecmp(x, y) == 0)
#define NAMEOF(s) strchr((const char *)(#s), '>') == NULL \
? #s : (strchr((const char *)(#s), '>') + 1)


bool check_machine_config_ini_exists(void) {
    FILE *fp;
    fp = fopen(MACHINE_CONFIG_INI_FILENAME, "r");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}


static bool get_bool(char value) {
    bool ret = false;
    value = toupper(value);
    if (value == 'Y' || value == '1' || value == 'T')
        ret = true;
    else if (value == 'N' || value == '0' || value == 'F')
        ret = false;
    else
        ret = false;
    return ret;
}

static void parse_section(machine_config_optional_slaves_t *config, const char *key,
                          const char *value) {
    // printf("%s\n", value);

    if (MATCH("machine_name", key)) {
        if (MATCH(map_machine_type_strings[map_machine_type], value)) {
        } else {
            UM_FATAL(
                "GBEM: Error parsing machine config ini file. Unknown machine [%s]. The machine must match the value this GBEM version targets [%s]",
                value, map_machine_type_strings[map_machine_type]);
        }
    } else if (MATCH("enable_optional_slave_1", key)) {
        config->enable_optional_slave_1 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_2", key)) {
        config->enable_optional_slave_2 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_3", key)) {
        config->enable_optional_slave_3 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_4", key)) {
        config->enable_optional_slave_4 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_5", key)) {
        config->enable_optional_slave_5 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_6", key)) {
        config->enable_optional_slave_6 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_7", key)) {
        config->enable_optional_slave_7 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_8", key)) {
        config->enable_optional_slave_8 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_9", key)) {
        config->enable_optional_slave_9 = get_bool(value[0]);
    } else if (MATCH("enable_optional_slave_10", key)) {
        config->enable_optional_slave_10 = get_bool(value[0]);
    } else {
        UM_FATAL("GBEM: Error parsing ini file. Unknown key: %s", key);
    }
}


static gberror_t config_parser(const char *section, const char *key,
                               const char *value, void *config_v) {
    parse_section(config_v, key, value);

    return 1;
}

uint8_t get_machine_config_ini_keys(void) {
    int s;
    char section[40];
    uint8_t count = 0;
    for (s = 0; ini_getkey(NULL, s, section, sizeof section, MACHINE_CONFIG_INI_FILENAME) > 0; s++) {
        count++;
    }
    return count;
}


gberror_t read_machine_config_ini(uint8_t *number_of_items_found) {
    if (!ini_browse(config_parser, &machine_config_optional_slaves, MACHINE_CONFIG_INI_FILENAME)) {
        return E_LOOKUP_FAILED;
    }
    *number_of_items_found = get_machine_config_ini_keys();
    return E_SUCCESS;
}
