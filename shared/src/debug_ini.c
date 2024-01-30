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

typedef enum {
    J0,
    J1,
    J2,
    J3,
    J4,
    J5,
    J6,
    J7,
    J8,
    J9,
    UNKNOWN_SECTION
} debug_ini_config_section_t;

static const char *debug_ini_config_sections[UNKNOWN_SECTION + 1] = {
    "J0",
    "J1",
    "J2",
    "J3",
    "J4",
    "J5",
    "J6",
    "J7",
    "J8",
    "J9",
    NULL
};


static debug_ini_config_section_t get_section(const char *section) {
    if (MATCH(debug_ini_config_sections[J0], section)) {
        return J0;
    } else if (MATCH(debug_ini_config_sections[J1], section)) {
        return J1;
    } else if (MATCH(debug_ini_config_sections[J2], section)) {
        return J2;
    } else if (MATCH(debug_ini_config_sections[J3], section)) {
        return J3;
    } else if (MATCH(debug_ini_config_sections[J4], section)) {
        return J4;
    } else if (MATCH(debug_ini_config_sections[J5], section)) {
        return J5;
    } else if (MATCH(debug_ini_config_sections[J6], section)) {
        return J6;
    } else if (MATCH(debug_ini_config_sections[J7], section)) {
        return J7;
    } else if (MATCH(debug_ini_config_sections[J8], section)) {
        return J8;
    } else if (MATCH(debug_ini_config_sections[J9], section)) {
        return J9;
    } else {
        return UNKNOWN_SECTION;
    }
}


bool check_debug_ini_exists(void) {
    FILE *fp;
    fp = fopen(DEBUG_INI_FILENAME, "r");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}

static void parse_section(map_machine_limits_t *config, const char *key,
                          const char *value, uint32_t section) {
    // printf("%s\n", value);

    if (MATCH("position_limit_max", key)) {
        config[section].position_limit_max = (uint32_t) atoi(value);
    } else if (MATCH("position_limit_min", key)) {
        config[section].position_limit_min = (uint32_t) atoi(value);
    } else if (MATCH("velocity_limit", key)) {
        config[section].velocity_limit = (uint32_t) atoi(value);
    } else if (MATCH("torque_limit", key)) {
        config[section].torque_limit = (uint32_t) atoi(value);
    } else if (MATCH("max_motor_speed", key)) {
        config[section].max_motor_speed = (uint32_t) atoi(value);
    } else if (MATCH("max_motor_torque", key)) {
        config[section].max_motor_torque = (uint32_t) atoi(value);
    } else {
        UM_FATAL("GBEM: Error parsing ini file. Unknown key: %s", key);
    }
}

static uint32_t parse_count = 0;


static gberror_t config_parser(const char *section, const char *key,
                               const char *value, void *config_v) {
    debug_ini_config_section_t s = get_section(section);


    parse_section(config_v, key, value, s);

    parse_count++;

    return 1;
}

uint8_t get_debug_ini_sections(void) {
    int s;
    char section[40];
    uint8_t count = 0;
    for (s = 0; ini_getsection(s, section, sizeof section, DEBUG_INI_FILENAME) > 0; s++) {
        count++;
    }
    return count;
}


gberror_t read_debug_ini(uint8_t *number_of_items_found) {
    if (!ini_browse(config_parser, &map_machine_limits, DEBUG_INI_FILENAME)) {
        return E_LOOKUP_FAILED;
    }
    *number_of_items_found = parse_count;
    return E_SUCCESS;
}
