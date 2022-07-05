
/**
 ******************************************************************************
 * @file           :  machine_EL7211_test.c
 * @brief          :  machine map for EL7211 test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "map.h"

#if MACHINE_EL7211_TEST == 1
#include "beckhoff.h"
#include "EL7211.h"


/* SLAVES */
//@formatter:off
//                                          Slave 1             Slave 2                 Slave 3
//                                          MAP_EK1100_1        MAP_EL7211_1            MAP_EL2008_1        MAP_EL1008_1
//                                          Coupler             DC servo                8 dig out           8 dig in
MAP_NUM_DRIVES_ATTACHED(                    0,                  1,                      0,                  0           );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(            NULL,               ec_pdo_map_EL7211,      NULL,               NULL        );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(              NULL,               NULL,                   NULL,               NULL        );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(           NULL,               ec_standard_sdos_EL7211,NULL,               NULL        );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(            NULL,               NULL,                   NULL,               NULL        );
MAP_SLAVE_DC_TYPE(                          EC_DC_NONE,            EC_DC_01,               EC_DC_NONE,         EC_DC_NONE  );
MAP_SLAVE_DC_CYCLE(                         0,        4,        0 , 0                            );

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "BECK_1",                       );
MAP_DRIVE_TO_SLAVE(                         MAP_EL7211_1,                   );
MAP_DRIVE_TO_SUBDRIVE(                      1,                              );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            NULL,                           );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            ec_get_moo_sdo_EL7211,          );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_EL7211,           );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_EL7211      );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_EL7211, );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL,                           );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_EL7211,         );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_EL7211,         );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_EL7211,       );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_EL7211,       );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                           );
MAP_DRIVE_RUN_HOMING(                       0,                              );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL                            );



/* DRIVE PARAMETERS */
//think direction might be set in an SDO write on El7211
//no limits on EL7211
MAP_DRIVE_POS_LIMIT(                        0,                              );
MAP_DRIVE_NEG_LIMIT(                        0,                              );
MAP_DRIVE_DIRECTION(                        1,                              );

/*IO MAP*/
mapping_t map_iomap[2] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=7, .datatype=ECT_BOOLEAN},    {.inout=MAP_IN, .ionum=1, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=8, .datatype=ECT_BOOLEAN},    {.inout=MAP_IN, .ionum=2, .datatype=ECT_BOOLEAN},    {}},
};

uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

//This is a zero indexed array even though the slaves are 1 indexed
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES] = {

        {.name = EK1100_EEP_NAME},
        {.name = EL7211_EEP_NAME, .eep_id = EL7211_EEP_ID, .eep_man = EL7211_EEP_MAN, .eep_rev = EL7211_EEP_REV},
        {.name = EL2008_EEP_NAME},
        {.name = EL1008_EEP_NAME}};

/* REVERSE FUNCTIONS - USED FOR TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_EL7211,     );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_EL7211,     );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_EL7211,   );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_EL7211,  );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_EL7211,      );




#endif


