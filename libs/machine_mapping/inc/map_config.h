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




/* NEW_MACHINE */
//If you add a new machine you have to increment this count to match the number above
#define MAP_NUM_MACHINES                    36

extern const char *map_machine_type_strings[MAP_NUM_MACHINES];

/* NEW_MACHINE - add new #ifdef to include the header here */
#if MACHINE_MINI == 1

#include "machine_mini.h"

#endif

#if MACHINE_CARTESIAN == 1

#include "machine_cartesian.h"

#endif

#if MACHINE_DRIVE_SPINNERS == 1

#include "machine_drive_spinners.h"

#endif

#if MACHINE_DRIVE_SPINNERS_VIRTUAL == 1

#include "machine_drive_spinners_virtual.h"

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


#if MACHINE_AW_ROBOT_S == 1
#include "machine_aw_robot_s.h"
#endif

#if MACHINE_AW_ROBOT_L2 == 1

#include "machine_aw_robot_l2.h"

#endif

#if MACHINE_AW_ROBOT_L == 1

#include "machine_aw_robot_l.h"

#endif

#if MACHINE_VIRTUAL == 1

#include "machine_virtual.h"

#endif

#if MACHINE_J17_PLUS_VIRTUAL == 1

#include "machine_j17_plus_virtual.h"

#endif

#if MACHINE_AW_STEWART == 1

#include "machine_aw_stewart.h"

#endif

#if MACHINE_EL6900_DEADMAN == 1

#include "machine_el6900_deadman.h"

#endif

#if MACHINE_EL6900_J25 == 1

#include "machine_el6900_j25.h"

#endif

#if MACHINE_BBH_J25 == 1

#include "machine_bbh_j25.h"

#endif

#if MACHINE_AW_6DOF == 1

#include "machine_aw_6dof.h"

#endif

#if MACHINE_AW_6DOF_FSOE == 1

#include "machine_aw_6dof_fsoe.h"

#endif


#if MACHINE_AW_6DOF_VIRTUAL == 1

#include "machine_aw_6dof_virtual.h"

#endif

/* NEW_MACHINE - add new MAP_ enum here */
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
    MAP_MACHINE_AW_ROBOT_S,
    MAP_MACHINE_AW_ROBOT_L2,
    MAP_MACHINE_AW_ROBOT_L,
    MAP_MACHINE_VIRTUAL,
    MAP_MACHINE_J17_PLUS_VIRTUAL,
    MAP_MACHINE_AW_STEWART,
    MAP_MACHINE_DRIVE_SPINNERS_VIRTUAL,
    MAP_MACHINE_EL6900_DEADMAN,
    MAP_MACHINE_EL6900_J25,
    MAP_MACHINE_BBH_J25,
    MAP_MACHINE_AW_6DOF,
    MAP_MACHINE_AW_6DOF_FSOE,
    MAP_MACHINE_AW_6DOF_VIRTUAL
} map_machine_type_t;

#define MAX_DRIVE_NAME_LENGTH                   30

//the maximum number of entries in a PDO mapping object
#define ECM_MAX_PDO_MAPPING_ENTRIES             20


//the maximum number of drives that can be configured
#define MAP_MAX_NUM_DRIVES                      10

//the maximum number of rows we can have in an iomap
#define MAX_NUMBER_MAP_ENTRIES_IN_IOMAP         100


#endif //GBEM_MAP_CONFIG_H
