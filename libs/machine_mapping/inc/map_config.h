/**
 ******************************************************************************
 * @file           :  map_config.h
 * @brief          :  configures the machine mapping
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
/* WARNING! Don't enable more than one or bad things happen when linking */
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
#define MACHINE_STARTER_KIT_1       0
#define MACHINE_STARTER_KIT_2       0
#define MACHINE_ASDA_A2_TEST        0
#define MACHINE_SMC3_TEST           0
#define MACHINE_AX5101_TEST         0
#define MACHINE_AW_J17              0
#define MACHINE_AW_ROBOT_M          0
#define MACHINE_VIRTUAL             1


//If you add a new machine you have to increment this count to match the number above
#define MAP_NUM_MACHINES            25

extern const char *map_machine_type_strings[MAP_NUM_MACHINES];

/* MACHINE_NEW - add new #ifdef to include the header here */
#if MACHINE_MINI == 1
#include "machine_mini.h"
#endif

#if MACHINE_CARTESIAN == 1
#include "machine_cartesian.h"
#endif

#if MACHINE_DRIVE_SPINNERS == 1
#include "machine_drive_spinners.h"
#endif

#if MACHINE_IGUS == 1
#include "machine_igus.h"
#endif

#if MACHINE_STAUBLI == 1
#include "machine_staubli.h"
#endif

#if  MACHINE_CONVEYORS == 1
#include "machine_conveyors.h"
#endif

#if MACHINE_SINGLE_AKD == 1
#include "machine_single_akd.h"
#endif

#if MACHINE_EL7211_TEST == 1
#include "machine_EL7211_test.h"
#endif

#if MACHINE_TEST == 1
#include "machine_test.h"
#endif

#if MACHINE_JVL_MIS_TEST == 1
#include "machine_jvl_mis_test.h"
#endif

#if MACHINE_G5_TEST == 1
#include "machine_g5_test.h"
#endif

#if MACHINE_N5_TEST == 1
#include "machine_n5_test.h"
#endif


#if MACHINE_EL7031_TEST == 1
#include "machine_EL7031_test.h"
#endif

#if MACHINE_EL7041_TEST == 1
#include "machine_EL7041_test.h"
#endif

#if MACHINE_EL2522_TEST == 1
#include "machine_EL2522_test.h"
#endif

#if MACHINE_EL7037_TEST == 1
#include "machine_EL7037_test.h"
#endif


#if MACHINE_ASDA_A2_TEST == 1
#include "machine_asda_a2_test.h"
#endif

#if MACHINE_SMC3_TEST == 1
#include "machine_smc3_test.h"
#endif

#if MACHINE_STARTER_KIT_1 == 1
#include "machine_starter_kit_1.h"
#endif

#if MACHINE_STARTER_KIT_2 == 1
#include "machine_starter_kit_2.h"
#endif

#if MACHINE_AX5101_TEST == 1
#include "machine_AX5101_test.h"
#endif

#if MACHINE_AW_J17 == 1

#include "machine_aw_j17.h"

#endif

#if MACHINE_AW_ROBOT_M == 1
#include "machine_aw_robot_m.h"
#endif

#if MACHINE_AW_ROBOT_M == 1
#include "machine_aw_robot_m.h"
#endif

#if MACHINE_VIRTUAL == 1

#include "machine_virtual.h"

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
    MAP_MACHINE_STARTER_KIT_1,
    MAP_MACHINE_STARTER_KIT_2,
    MAP_MACHINE_ASDA_A2_TEST,
    MAP_MACHINE_SMC3_TEST,
    MAP_MACHINE_AX5101_TEST,
    MAP_MACHINE_AW_J17,
    MAP_MACHINE_AW_ROBOT_M,
    MAP_MACHINE_VIRTUAL
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
