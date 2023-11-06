
/**
 ******************************************************************************
 * @file           :  machine_smc3_test.c
 * @brief          :  machine map for SMC3 drive test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "map.h"

#if MACHINE_SMC3_TEST == 1
#include "smc3.h"

//@formatter:off
/* SLAVES */
//                                        Slave 1
//                                        MAP_SMC3_1
//                                        Drive
MAP_NUM_DRIVES_ATTACHED(                  1                                 );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          ec_pdo_map_smc3                              );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL                              );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         ec_standard_sdos_smc3               );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          ec_initial_pdo_smc3                 );
MAP_SLAVE_DC_TYPE(                        EC_DC_0,                          );
MAP_SLAVE_DC_CYCLE(                       4);

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "SMC3"                           );
MAP_DRIVE_TO_SLAVE(                         MAP_SMC3_1                        );
MAP_DRIVE_TO_SUBDRIVE(                      1                               );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_smc3               );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL                            );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_smc3                );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_smc3          );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL                            );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_smc3      );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_smc3              );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_smc3              );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_smc3            );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_smc3            );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL                              );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL                              );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL,                             );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL,                             );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  NULL,                             );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   NULL,                             );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            NULL,                             );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL                            );
MAP_DRIVE_RUN_HOMING(                       0                               );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           ec_print_params_smc3              );

/* DRIVE PARAMETERS */
//MAP_DRIVE_POS_LIMIT(                        1073741823                       );
//MAP_DRIVE_NEG_LIMIT(                        -1073741823                      );
MAP_DRIVE_POS_LIMIT(                        0                       );
MAP_DRIVE_NEG_LIMIT(                        0                      );
MAP_DRIVE_DIRECTION(                        0,                              );
MAP_DRIVE_TORQ_LIMIT(                       0,                              );
MAP_DRIVE_VEL_LIMIT(                        0,                              );
MAP_DRIVE_SCALES(                           {0},                            );



/* IO MAP */
mapping_t map_iomap[0] = {};


uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/* This must be laid out in the order they appear in netscan */
MAP_SLAVE_EEP ({SMC3_EEP_NAME});

/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       NULL        );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       NULL        );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     NULL        );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     NULL        );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            NULL        );



#endif