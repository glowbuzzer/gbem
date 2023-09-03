
/**
 ******************************************************************************
 * @file           :  machine_jvl_test.c
 * @brief          :  machine map for JVL MIS drive test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "map.h"

#if MACHINE_JVL_MIS_TEST == 1
#include "jvl_mis.h"

//@formatter:off
/* SLAVES */
//                                        Slave 1           Slave 2         Slave 3         Slave 4         Slave 5
//                                        MAP_EK1100_1      MAP_EL2008_1    MAP_EL1008_1   MAP_EL5101_1     MAP_JVL_MIS_1
//                                        Coupler           8xDOUT          8xDIN           Encoder         Drive
MAP_NUM_DRIVES_ATTACHED(                  0,                0,              0,              0,              1                                 );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,             NULL,           NULL,           NULL,           ec_pdo_map_jvl_mis                );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,             NULL,           NULL,           NULL,           NULL                              );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         NULL,             NULL,           NULL,           NULL,           ec_standard_sdos_jvl_mis,         );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,             NULL,           NULL,           NULL,           ec_initial_pdo_jvl_mis            );
MAP_SLAVE_DC_TYPE(                        EC_DC_NONE,       EC_DC_NONE,     EC_DC_NONE,     EC_DC_NONE,     EC_DC_0,                          );
MAP_SLAVE_DC_CYCLE(                       0,                0,              0,              0,              4,                                );

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "JVL1"                          );
MAP_DRIVE_TO_SLAVE(                         MAP_JVL_MIS_1                   );
MAP_DRIVE_TO_SUBDRIVE(                      1                               );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_jvl_mis          );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL                            );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_jvl_mis           );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_jvl_mis     );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   ec_get_error_string_pdo_jvl_mis );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   NULL                            );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_jvl_mis         );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_jvl_mis         );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_jvl_mis       );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_jvl_mis       );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL                            );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL                            );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL                            );
MAP_DRIVE_RUN_HOMING(                       0                               );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           ec_print_params_jvl_mis         );

/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        INT32_MAX                      );
MAP_DRIVE_NEG_LIMIT(                        -INT32_MAX                      );
MAP_DRIVE_DIRECTION(                        0,                              );



/* IO MAP */
mapping_t map_iomap[0] = {};


uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/* This must be laid out in the order they appear in netscan */
MAP_SLAVE_EEP ({JVL_MIS_EEP_NAME});

/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       NULL     );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       NULL     );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     NULL   );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     NULL  );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            NULL,                       );




#endif