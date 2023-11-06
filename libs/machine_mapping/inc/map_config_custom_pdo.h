/**
 ******************************************************************************
 * @file           :  map_config_custom_pdo
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_MAP_CONFIG_CUSTOM_PDO_H
#define GBEM_MAP_CONFIG_CUSTOM_PDO_H


#include "ethercattype.h"
#include "ethercatsetget.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "cia402.h"

#define MAP_MAX_PDO_ASSIGNMENTS 14
#define MAP_MAX_SYNC_MANAGER_ASSIGNMENTS 10

typedef struct {
    ec_datatype datatype;
    uint16_t index;
    uint8_t offset;
} map_custom_pdo_assignment_t;


typedef struct {
    uint32_t pdo_assignment_object;
    uint8_t num_assignments;
    map_custom_pdo_assignment_t assignments[MAP_MAX_PDO_ASSIGNMENTS];
} map_custom_pdo_map_t;

typedef struct {
    uint16_t sm2_assignment_object;
    uint8_t num_sm2_assignments;
    map_custom_pdo_map_t rxpdo[MAP_MAX_SYNC_MANAGER_ASSIGNMENTS];
    uint16_t sm3_assignment_object;
    uint8_t num_sm3_assignments;
    map_custom_pdo_map_t txpdo[MAP_MAX_SYNC_MANAGER_ASSIGNMENTS];
} map_custom_pdo_t;

gberror_t map_apply_custom_pdo_mapping(uint16_t slave, map_custom_pdo_t map);


#endif //GBEM_MAP_CONFIG_CUSTOM_PDO_H
