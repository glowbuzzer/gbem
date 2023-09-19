
/**
 ******************************************************************************
 * @file           :  machine_drive_spinners.c
 * @brief          :  machine map for drive spinners
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "map.h"

#if MACHINE_DRIVE_SPINNERS == 1

#include "azdxa-ked.h"
#include "beckhoff.h"

/* SLAVES */
//@formatter:off
//                                          Slave 1             Slave 2         Slave 3          Slave 4
//                                          MAP_EK1100_1        MAP_EL2008_1    MAP_EL1008_1     MAP_AZD_CTRL_1
//                                          Coupler             8 dig out       8 dig in         3 axis stepper
MAP_NUM_DRIVES_ATTACHED(                    0,                  0,              0,               2,                          );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(            NULL,               NULL,           NULL,            ec_pdo_map_azd3a_ked,       );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(              NULL,               NULL,           NULL,            ec_nvram_sdos_azdxa_ked,    );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(           NULL,               NULL,           NULL,            NULL,                       );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(            NULL,               NULL,           NULL,            ec_initial_pdo_azdxa_ked,   );
MAP_SLAVE_DC_TYPE(                          EC_DC_NONE,         EC_DC_NONE,     EC_DC_NONE,      EC_DC_0,                    );
MAP_SLAVE_DC_CYCLE(                         0,                  0,              0,               1,                          );

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AZD1.1",                           "AZD1.2"                            );
MAP_DRIVE_TO_SLAVE(                         MAP_AZD_CTRL_1,                     MAP_AZD_CTRL_1                      );
MAP_DRIVE_TO_SUBDRIVE(                      1,                                  2                                   );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_azdxa_ked,           ec_get_moo_pdo_azdxa_ked            );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                               NULL                                );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_azdxa_ked,            ec_get_remote_azdxa_ked             );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_azdxa_ked,      ec_get_follow_error_azdxa_ked       );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   ec_get_error_string_pdo_azdxa_ked,  ec_get_error_string_pdo_azdxa_ked   );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   NULL,                               NULL                                );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_azdxa_ked,          ec_set_ctrl_wrd_azdxa_ked           );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_azdxa_ked,          ec_get_stat_wrd_azdxa_ked           );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_azdxa_ked,        ec_get_actpos_wrd_azdxa_ked         );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_azdxa_ked,        ec_set_setpos_wrd_azdxa_ked         );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL,                               NULL                                );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL,                               NULL                                );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                               NULL                                );
MAP_DRIVE_RUN_HOMING(                       0,                                  0                                   );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL,                               NULL);


/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        999,                                999                                 );
MAP_DRIVE_NEG_LIMIT(                        -999,                               -999                                );
MAP_DRIVE_DIRECTION(                        1,                                  1                                   );

/*IO MAP*/
mapping_t map_iomap[2] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=7, .datatype=ECT_BOOLEAN},    {.inout=MAP_IN, .ionum=1, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=8, .datatype=ECT_BOOLEAN},    {.inout=MAP_IN, .ionum=2, .datatype=ECT_BOOLEAN},    {}},
};

uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES] = {
        {
                .name = EK1100_EEP_NAME},
        {
                .name = EL2008_EEP_NAME},
        {
                .name = EL1008_EEP_NAME},
        {
            .name = AZD3A_KED_EEP_NAME,
            .eep_id = AZD3A_KED_EEP_ID,
            .eep_man = AZD3A_KED_EEP_MAN,
            .eep_rev = AZD3A_KED_EEP_REV},

};

/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_azdxa_ked,        ec_get_ctrl_wrd_rev_azdxa_ked             );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_azdxa_ked,        ec_set_stat_wrd_rev_azdxa_ked             );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_azdxa_ked,      ec_set_actpos_wrd_rev_azdxa_ked           );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_azdxa_ked,     ec_get_setpos_word_rev_azdxa_ked          );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_azdxa_ked,         ec_set_moo_pdo_rev_azdxa_ked              );

//formatter:on

#endif