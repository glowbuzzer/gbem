/**
 ******************************************************************************
 * @file           :  machine_aw_robot_m.c
 * @brief          :  machine map for aw medium sized robot with EK1100 + El1808 + EL2908
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "map.h"
//#ifdef must surround all the machine specific code so that when this machine is not defined it is not compiled
#if MACHINE_AW_STEWART == 1

#include "trio.h"
#include "RTA.h"


//@formatter:off
map_pdo_object_t ctrl_estop_din = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_P372_1, .bit_num=1};
map_pdo_object_t ctrl_estop_reset_din = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_P372_1, .bit_num=2};


/* SLAVES */
//                                        Slave 1             Slave 2         Slave 3                           Slave 4                          Slave 5                            Slave 6                         Slave 7                         Slave 8                         Slave 9
//                                        MAP_P366_1          MAP_P372_1      MAP_MSE_408_1                     MAP_MSE_408_2                    MAP_MSE_408_3                      MAP_P366_1               MAP_MSE_408_4               MAP_MSE_408_5                              MAP_MSE_408_6
//                                        Coupler             Input module    RTA drive                         RTA drive                        RTA Drive                          Coupler                       RTA Drive                       RTA Drive                       RTA Drive
MAP_NUM_DRIVES_ATTACHED(                  0,                  0,              1,                                1,                               1,                                 0,                              1,                              1,                              1                                   );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,               NULL,           NULL,                             NULL,                            NULL,                              NULL,                           NULL,                           NULL,                           NULL                                );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,               NULL,           NULL,                             NULL,                            NULL,                              NULL,                           NULL,                           NULL,                           NULL                                );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         NULL,               NULL,           ec_apply_standard_sdos_mse_408,   ec_apply_standard_sdos_mse_408,  ec_apply_standard_sdos_mse_408,    NULL,                           ec_apply_standard_sdos_mse_408, ec_apply_standard_sdos_mse_408, ec_apply_standard_sdos_mse_408      );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,               NULL,           ec_initial_pdo_mse_408,           ec_initial_pdo_mse_408,          ec_initial_pdo_mse_408,            NULL,                           ec_initial_pdo_mse_408,         ec_initial_pdo_mse_408,         ec_initial_pdo_mse_408              );
MAP_SLAVE_DC_TYPE(                        EC_DC_NONE,         EC_DC_NONE,     EC_DC_0,                          EC_DC_0,                         EC_DC_0,                           EC_DC_NONE,                     EC_DC_0,                        EC_DC_0,                        EC_DC_0                             );
MAP_SLAVE_DC_CYCLE(                       0,                  0,              4,                                4,                               4,                                 0,                              4,                              4,                              4                                   );

//set moo is in either initial PDO or custom sdos

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "J1",                               "J2",                               "J3",                               "J4",                               "J5",                               "J6"                                );
MAP_DRIVE_TO_SLAVE(                         MAP_MSE_408_1,                      MAP_MSE_408_2,                      MAP_MSE_408_3,                      MAP_MSE_408_4,                      MAP_MSE_408_5,                      MAP_MSE_408_6                       );
MAP_DRIVE_TO_SUBDRIVE(                      1,                                  1,                                  1,                                  1,                                  1,                                  1                                   );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_mse_408,             ec_get_moo_pdo_mse_408,             ec_get_moo_pdo_mse_408,             ec_get_moo_pdo_mse_408,             ec_get_moo_pdo_mse_408,             ec_get_moo_pdo_mse_408              );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                               NULL,                               NULL,                               NULL,                               NULL,                               NULL                                );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_mse_408,              ec_get_remote_mse_408,              ec_get_remote_mse_408,              ec_get_remote_mse_408,              ec_get_remote_mse_408,              ec_get_remote_mse_408               );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_mse_408,        ec_get_follow_error_mse_408,        ec_get_follow_error_mse_408,        ec_get_follow_error_mse_408,        ec_get_follow_error_mse_408,        ec_get_follow_error_mse_408         );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL,                               NULL,                               NULL,                               NULL,                               NULL,                               NULL                                );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_mse_408,    ec_get_error_string_sdo_mse_408,    ec_get_error_string_sdo_mse_408,    ec_get_error_string_sdo_mse_408,    ec_get_error_string_sdo_mse_408,    ec_get_error_string_sdo_mse_408     );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_mse_408,            ec_set_ctrl_wrd_mse_408,            ec_set_ctrl_wrd_mse_408,            ec_set_ctrl_wrd_mse_408,            ec_set_ctrl_wrd_mse_408,            ec_set_ctrl_wrd_mse_408             );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_mse_408,            ec_get_stat_wrd_mse_408,            ec_get_stat_wrd_mse_408,            ec_get_stat_wrd_mse_408,            ec_get_stat_wrd_mse_408,            ec_get_stat_wrd_mse_408             );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_mse_408,          ec_get_actpos_wrd_mse_408,          ec_get_actpos_wrd_mse_408,          ec_get_actpos_wrd_mse_408,          ec_get_actpos_wrd_mse_408,          ec_get_actpos_wrd_mse_408           );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_mse_408,          ec_set_setpos_wrd_mse_408,          ec_set_setpos_wrd_mse_408,          ec_set_setpos_wrd_mse_408,          ec_set_setpos_wrd_mse_408,          ec_set_setpos_wrd_mse_408           );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL,                               NULL,                               NULL,                               NULL,                               NULL,                               NULL                                );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL,                               NULL,                               NULL,                               NULL,                               NULL,                               NULL                                );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL,                               NULL,                               NULL,                               NULL,                               NULL,                               NULL                                );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL,                               NULL,                               NULL,                               NULL,                               NULL,                               NULL                                );
MAP_DRIVE_MOO(                              CIA_MOO_CSP,                        CIA_MOO_CSP,                        CIA_MOO_CSP,                        CIA_MOO_CSP,                        CIA_MOO_CSP,                        CIA_MOO_CSP                         );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                               NULL,                               NULL,                               NULL,                               NULL,                               NULL                                );
MAP_DRIVE_RUN_HOMING(                       0,                                  0,                                  0,                                  0,                                  0,                                  0                                   );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL,                               NULL,                               NULL,                               NULL,                               NULL,                               NULL                                );



/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        99999999,                               99999999,                               99999999,                               99999999,                               99999999,                               99999999                                );
MAP_DRIVE_NEG_LIMIT(                        -99999999,                              -99999999,                              -99999999,                              -99999999,                              -99999999,                              -99999999                               );
MAP_DRIVE_DIRECTION(                        1,                                      1,                                      1,                                      1,                                      1,                                      1                                       );



extern bool plc_din1, plc_din2, plc_din3, plc_dout1, plc_dout2, plc_dout3;


/*IO MAP*/
mapping_t map_iomap[8] = {
        {{.inout=MAP_IN, .slave_num=MAP_P372_1,.bit_num=1, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_P372_1,.bit_num=2, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_P372_1,.bit_num=3, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_P372_1,.bit_num=4, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_P372_1,.bit_num=5, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_P372_1,.bit_num=6, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_P372_1,.bit_num=7, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_P372_1,.bit_num=8, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=7},     {}},
        };

uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES] = {
        {.name = P366_EEP_NAME,       .eep_id = P366_EEP_ID,        .eep_man = P366_EEP_MAN,          .eep_rev = P366_EEP_REV   },
        {.name = P372_EEP_NAME,       .eep_id = P372_EEP_ID,        .eep_man = P372_EEP_MAN,          .eep_rev = P372_EEP_REV   },
        {.name = MSE_408_EEP_NAME,       .eep_id = MSE_408_EEP_ID,        .eep_man = MSE_408_EEP_MAN,          .eep_rev = MSE_408_EEP_REV   },
        {.name = MSE_408_EEP_NAME,       .eep_id = MSE_408_EEP_ID,        .eep_man = MSE_408_EEP_MAN,          .eep_rev = MSE_408_EEP_REV   },
        {.name = MSE_408_EEP_NAME,       .eep_id = MSE_408_EEP_ID,        .eep_man = MSE_408_EEP_MAN,          .eep_rev = MSE_408_EEP_REV   },
        {.name = P366_EEP_NAME,       .eep_id = P366_EEP_ID,        .eep_man = P366_EEP_MAN,          .eep_rev = P366_EEP_REV   },
        {.name = MSE_408_EEP_NAME,       .eep_id = MSE_408_EEP_ID,        .eep_man = MSE_408_EEP_MAN,          .eep_rev = MSE_408_EEP_REV   },
        {.name = MSE_408_EEP_NAME,       .eep_id = MSE_408_EEP_ID,        .eep_man = MSE_408_EEP_MAN,          .eep_rev = MSE_408_EEP_REV   },
        {.name = MSE_408_EEP_NAME,       .eep_id = MSE_408_EEP_ID,        .eep_man = MSE_408_EEP_MAN,          .eep_rev = MSE_408_EEP_REV   },
};


/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_mse_408,       ec_get_ctrl_wrd_rev_mse_408,        ec_get_ctrl_wrd_rev_mse_408,        ec_get_ctrl_wrd_rev_mse_408,        ec_get_ctrl_wrd_rev_mse_408,        ec_get_ctrl_wrd_rev_mse_408         );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_mse_408,       ec_set_stat_wrd_rev_mse_408,        ec_set_stat_wrd_rev_mse_408,        ec_set_stat_wrd_rev_mse_408,        ec_set_stat_wrd_rev_mse_408,        ec_set_stat_wrd_rev_mse_408         );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_mse_408,     ec_set_actpos_wrd_rev_mse_408,      ec_set_actpos_wrd_rev_mse_408,      ec_set_actpos_wrd_rev_mse_408,      ec_set_actpos_wrd_rev_mse_408,      ec_set_actpos_wrd_rev_mse_408       );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_mse_408,    ec_get_setpos_word_rev_mse_408,     ec_get_setpos_word_rev_mse_408,     ec_get_setpos_word_rev_mse_408,     ec_get_setpos_word_rev_mse_408,     ec_get_setpos_word_rev_mse_408      );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_mse_408,        ec_set_moo_pdo_rev_mse_408,         ec_set_moo_pdo_rev_mse_408,         ec_set_moo_pdo_rev_mse_408,         ec_set_moo_pdo_rev_mse_408,         ec_set_moo_pdo_rev_mse_408          );


//@formatter:on

#endif //MACHINE_AW_ROBOT_M
