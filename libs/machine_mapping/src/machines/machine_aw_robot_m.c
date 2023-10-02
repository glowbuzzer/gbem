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
#if MACHINE_AW_ROBOT_M == 1

#include "automationware.h"
#include "beckhoff.h"

//LARGE
//J20 -> J25 -> J32 ->J40HP -> J40-LP

//MEDIUM
//J17 -> J20 -> J20 ->J25 -> J32 ->J32

//SMALL
//J17 -> J17 -> J20 ->J25 -> J32 -> J25

//@formatter:off
map_pdo_object_t ctrl_estop_din = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_EL1808_1, .bit_num=1};
map_pdo_object_t ctrl_estop_reset_din = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_EL1808_1, .bit_num=2};


/* SLAVES */
//                                        Slave 1             Slave 2         Slave 3          Slave 4                          Slave 5                         Slave 6                         Slave 7                         Slave 8                         Slave 9
//                                        MAP_EK1100_1        MAP_EL2808_1    MAP_EL1808_1     MAP_AW_J17_CTRL_1                MAP_AW_J20_CTRL_1               MAP_AW_J20_CTRL_2               MAP_AW_J25_CTRL_1               MAP_AW_J32_CTRL_1               MAP_AW_J32_CTRL_2
//                                        Coupler             8 dig out       8 dig in         J17 joint                        J20 joint                       J20 joint                       J25 joint                       J32 joint                       J32 joint
MAP_NUM_DRIVES_ATTACHED(                  0,                  0,              0,               1,                               1,                              1,                              1,                              1,                              1                                   );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,               NULL,           NULL,            NULL,                            NULL,                           NULL,                           NULL,                           NULL,                           NULL                                );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,               NULL,           NULL,            NULL,                            NULL,                           NULL,                           NULL,                           NULL,                           NULL                                );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         NULL,               NULL,           NULL,            ec_apply_standard_sdos_aw_j17,   ec_apply_standard_sdos_aw_j20,  ec_apply_standard_sdos_aw_j20,  ec_apply_standard_sdos_aw_j25,  ec_apply_standard_sdos_aw_j32,  ec_apply_standard_sdos_aw_j32       );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,               NULL,           NULL,            ec_initial_pdo_aw_j_series,      ec_initial_pdo_aw_j_series,     ec_initial_pdo_aw_j_series,     ec_initial_pdo_aw_j_series,     ec_initial_pdo_aw_j_series,     ec_initial_pdo_aw_j_series          );
MAP_SLAVE_DC_TYPE(                        EC_DC_NONE,         EC_DC_NONE,     EC_DC_NONE,      EC_DC_0,                         EC_DC_0,                        EC_DC_0,                        EC_DC_0,                        EC_DC_0,                        EC_DC_0                             );
MAP_SLAVE_DC_CYCLE(                       0,                  0,              0,               4,                               4,                              4,                              4,                              4,                              4                                   );

//set moo is in either initial PDO or custom sdos

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "J1",                                   "J2",                                   "J3",                                   "J4",                                   "J5",                                   "J6"                                    );
MAP_DRIVE_TO_SLAVE(                         MAP_AW_J17_CTRL_1,                      MAP_AW_J20_CTRL_1,                      MAP_AW_J20_CTRL2,                       MAP_AW_J25_CTRL_1,                      MAP_AW_J32_CTRL_1,                      MAP_AW_J32_CTRL_2                       );
MAP_DRIVE_TO_SUBDRIVE(                      1,                                      1,                                      1,                                      1,                                      1,                                      1                                       );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series              );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series               );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series         );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_aw_j_series,    ec_get_error_string_sdo_aw_j_series,    ec_get_error_string_sdo_aw_j_series,    ec_get_error_string_sdo_aw_j_series,    ec_get_error_string_sdo_aw_j_series,    ec_get_error_string_sdo_aw_j_series     );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series             );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series             );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series           );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series           );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series           );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series          );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_MOO(                              CIA_MOO_CSP,                            CIA_MOO_CSP,                            CIA_MOO_CSP,                            CIA_MOO_CSP,                            CIA_MOO_CSP,                            CIA_MOO_CSP                             );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_RUN_HOMING(                       0,                                      0,                                      0,                                      0,                                      0,                                      0                                       );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );



/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        99999999,                               99999999,                               99999999,                               99999999,                               99999999,                               99999999                                );
MAP_DRIVE_NEG_LIMIT(                        -99999999,                              -99999999,                              -99999999,                              -99999999,                              -99999999,                              -99999999                               );
MAP_DRIVE_DIRECTION(                        1,                                      1,                                      1,                                      1,                                      1,                                      1                                       );



extern bool plc_din1, plc_din2, plc_din3, plc_din4, plc_din5, plc_din7, plc_din8, plc_dout1, plc_dout2, plc_dout3, plc_dout4, plc_dout5, plc_dout6, plc_dout7, plc_dout8;


/*IO MAP*/
mapping_t map_iomap[16] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=1, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din1, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=2, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din2, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=3, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din3, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=4, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din4, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=5, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din5, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=6, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din6, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=7, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din7, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=8, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=7},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din8, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=1, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=0},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout1, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=2, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=1},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout2, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=3, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=2},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout3, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=4, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=3},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout4, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=5, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=4},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout5, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=6, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=5},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout6, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=7, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=6},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout7, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=8, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=7},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout8, .linked_task_name="Task1"}},
        };

uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES] = {
        {.name = EK1100_EEP_NAME,       .eep_id = EK1100_EEP_ID,        .eep_man = EK1100_EEP_MAN,          .eep_rev = EK1100_EEP_REV   },
        {.name = EL2808_EEP_NAME,       .eep_id = EL2808_EEP_ID,        .eep_man = EL2808_EEP_MAN,          .eep_rev = EL2808_EEP_REV   },
        {.name = EL1808_EEP_NAME,       .eep_id = EL1808_EEP_ID,        .eep_man = EL1808_EEP_MAN,          .eep_rev = EL1808_EEP_REV   },
        {.name = AW_J17_EEP_NAME,       .eep_id = AW_J17_EEP_ID,        .eep_man = AW_J17_EEP_MAN,          .eep_rev = AW_J17_EEP_REV   },
        {.name = AW_J20_EEP_NAME,       .eep_id = AW_J20_EEP_ID,        .eep_man = AW_J20_EEP_MAN,          .eep_rev = AW_J20_EEP_REV   },
        {.name = AW_J20_EEP_NAME,       .eep_id = AW_J20_EEP_ID,        .eep_man = AW_J20_EEP_MAN,          .eep_rev = AW_J20_EEP_REV   },
        {.name = AW_J25_EEP_NAME,       .eep_id = AW_J25_EEP_ID,        .eep_man = AW_J25_EEP_MAN,          .eep_rev = AW_J25_EEP_REV   },
        {.name = AW_J32_EEP_NAME,       .eep_id = AW_J32_EEP_ID,        .eep_man = AW_J32_EEP_MAN,          .eep_rev = AW_J32_EEP_REV   },
        {.name = AW_J32_EEP_NAME,       .eep_id = AW_J32_EEP_ID,        .eep_man = AW_J32_EEP_MAN,          .eep_rev = AW_J32_EEP_REV   },
};


/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_aw_j_series,       ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series         );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_aw_j_series,       ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series         );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_aw_j_series,     ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series       );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_aw_j_series,    ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series      );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_aw_j_series,        ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series          );


//@formatter:on

#endif //MACHINE_AW_ROBOT_M
