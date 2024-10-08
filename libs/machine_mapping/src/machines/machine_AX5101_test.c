
/**
 ******************************************************************************
 * @file           :  machine_AX5101_test_map.c
 * @brief          :  machine map for AX5101 test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "map.h"

#if MACHINE_AX5101_TEST == 1
#include "beckhoff.h"


/* SLAVES */
//@formatter:off
//                                          Slave 1
//                                          MAP_AX5101_1
//                                          AC Servo
MAP_NUM_DRIVES_ATTACHED(                    1                       );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(            ec_pdo_map_AX5101       );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(              NULL                    );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(           ec_standard_sdos_AX5101 );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(            NULL                    );
MAP_SLAVE_DC_TYPE(                          EC_DC_0                 );
MAP_SLAVE_DC_CYCLE(                         4                       );

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "BECK_1",                       );
MAP_DRIVE_TO_SLAVE(                         MAP_AX5101_1,                   );
MAP_DRIVE_TO_SUBDRIVE(                      1,                              );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            NULL,                           );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            ec_get_moo_sdo_AX5101,          );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_AX5101,           );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_AX5101      );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_AX5101, );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL,                           );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_AX5101,         );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_AX5101,         );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_AX5101,       );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_AX5101,       );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL                            );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL                            );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL,                           );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL,                           );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  NULL,                           );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   NULL,                           );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            NULL,                           );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                           );
MAP_DRIVE_RUN_HOMING(                       0,                              );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL                            );



/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        0,                              );
MAP_DRIVE_NEG_LIMIT(                        0,                              );
//MAP_DRIVE_DIRECTION(                        1,                              );
MAP_DRIVE_TORQ_LIMIT(                       0,                              );
MAP_DRIVE_VEL_LIMIT(                        0                               );
MAP_DRIVE_SCALES(                           {0}                             );


/*IO MAP*/
mapping_t map_iomap[0] = {

};

uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

//This is a zero indexed array even though the slaves are 1 indexed
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {

        {.name = AX5101_EEP_NAME, .eep_id = AX5101_EEP_ID, .eep_man = AX5101_EEP_MAN, .eep_rev = AX5101_EEP_REV},
};

/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_AX5101,     );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_AX5101,     );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_AX5101,   );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_AX5101,  );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_AX5101,      );




#endif


