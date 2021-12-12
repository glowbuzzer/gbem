/**
 ******************************************************************************
 * @file           :  map_config.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_MAP_CONFIG_H
#define GBEM_MAP_CONFIG_H


/* MACHINE_SWAP - set the correct map here - ofc only one machine at a time can be active << THIS IS THE WAY TO SWAP MACHINES*/
#define MACHINE_MINI                0
#define MACHINE_CARTESIAN           0
#define MACHINE_DRIVE_SPINNERS      0
#define MACHINE_IGUS                0
#define MACHINE_STAUBLI             0
#define MACHINE_SINGLE_AKD          0
#define MACHINE_CONVEYORS           0
#define MACHINE_EL7211_TEST         0
#define MACHINE_TEST                0
#define MACHINE_JVL_MIS_TEST        0
#define MACHINE_G5_TEST             0
#define MACHINE_N5_TEST             0
#define MACHINE_EL7031_TEST         0
#define MACHINE_EL7041_TEST         0
#define MACHINE_EL2522_TEST         0
#define MACHINE_EL7037_TEST         0
#define MACHINE_SK1                 1
#define MACHINE_SK2                 0
#define MACHINE_ASDA2_TEST          0
#define MACHINE_SMC3_TEST           0

//If you add a new machine you have to increment this count
#define MAP_NUM_MACHINES            21

extern const char *map_machine_type_strings[MAP_NUM_MACHINES];

/* MACHINE_NEW - add new #ifdef to include the header here */
#if MACHINE_MINI == 1
#include "mini_map.h"
#endif

#if MACHINE_CARTESIAN == 1
#include "cartesian_map.h"
#endif

#if MACHINE_DRIVE_SPINNERS == 1
#include "drive_spinners_map.h"
#endif

#if MACHINE_IGUS == 1
#include "igus_map.h"
#endif

#if MACHINE_STAUBLI == 1
#include "staubli_map.h"
#endif

#if  MACHINE_CONVEYORS == 1
#include "conveyors_map.h"
#endif

#if MACHINE_SINGLE_AKD == 1
#include "single_akd.h"
#endif

#if MACHINE_EL7211_TEST == 1
#include "EL7211_test_map.h"
#endif

#if MACHINE_TEST == 1
#include "test_map.h"
#endif

#if MACHINE_JVL_MIS_TEST == 1
#include "jvl_mis_test_map.h"
#endif

#if MACHINE_G5_TEST == 1
#include "g5_test_map.h"
#endif

#if MACHINE_N5_TEST == 1
#include "n5_test_map.h"
#endif


#if MACHINE_EL7031_TEST == 1
#include "EL7031_test_map.h"
#endif

#if MACHINE_EL7041_TEST == 1
#include "EL7041_test_map.h"
#endif

#if MACHINE_EL2522_TEST == 1
#include "EL2522_test_map.h"
#endif

#if MACHINE_EL7037_TEST == 1
#include "EL7037_test_map.h"
#endif


#if MACHINE_ASDA2_TEST == 1
#include "asda2_test_map.h"
#endif

#if MACHINE_SMC3_TEST == 1
#include "smc3_test_map.h"
#endif


#if MACHINE_SK1 == 1
#include "sk1_map.h"
#endif
#if MACHINE_SK2 == 1
#include "sk1_map.h"
#endif

/* MACHINE_NEW - add new MAP_ enum here */
typedef enum {
    MAP_MACHINE_UNKNOWN,
    MAP_MACHINE_MINI,
    MAP_MACHINE_CARTESIAN,
    MAP_MACHINE_DRIVE_SPINNERS,
    MAP_MACHINE_IGUS,
    MAP_MACHINE_STAUBLI,
    MAP_MACHINE_SINGLE_AKD,
    MAP_MACHINE_CONVEYORS,
    MAP_MACHINE_EL7211_TEST,
    MAP_MACHINE_TEST,
    MAP_MACHINE_JVL_MIS_TEST,
    MAP_MACHINE_G5_TEST,
    MAP_MACHINE_N5_TEST,
    MAP_MACHINE_EL7031_TEST,
    MAP_MACHINE_EL7041_TEST,
    MAP_MACHINE_EL2522_TEST,
    MAP_MACHINE_EL7037_TEST,
    MAP_MACHINE_SK1,
    MAP_MACHINE_SK2,
    MAP_MACHINE_ASDA2,
    MAP_MACHINE_SMC3,
    } map_machine_type_t;

#define MAX_DRIVE_NAME_LENGTH                   30

//the maximum number of entries in a PDO mapping object
#define ECM_MAX_PDO_MAPPING_ENTRIES             20

//the maximum length of a drive error message
#define MAX_DRIVE_ERROR_MSG_LENGTH              200

//the maximum number of drives that can be configured
#define MAP_MAX_NUM_DRIVES                      10

//the maximum number of rows we can have in an iomap
#define MAX_NUMBER_MAP_ENTRIES_IN_IOMAP         100


#endif //GBEM_MAP_CONFIG_H