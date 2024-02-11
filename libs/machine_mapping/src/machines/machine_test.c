
/**
 ******************************************************************************
 * @file           :  machine_test.c
 * @brief          :  machine map for test
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "map.h"

#if MACHINE_TEST == 1

#include "azdxa-ked.h"
#include "beckhoff.h"
#include "kollmorgen.h"

//ELTESTO Out test float
#define ELTESTO_EEP_NAME "ELTESTO"
//float 32
#define ELTEST0_ANALOG_OUPUT_INDEX 0

//ELTESTI In test float
#define ELTESTI_EEP_NAME "ELTESTI"
//float 32
#define ELTESTI_ANALOG_INPUT_INDEX 0

bool  slave_1_pdo_map(uint16_t slave){
//printf("hello world\n");
}

//@formatter:off

map_pdo_object_t ctrl_estop_din = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_EL1008_1, .bit_num=1};


/* SLAVES - must be defined in order that they appear on the EtherCAT network */
//                                        Slave 1             Slave 2         Slave 3         Slave 4         Slave 5              Slave 6          Slave 7          Slave 8             Slave 9
//                                        MAP_EK1100_1        MAP_EL2008_1    MAP_EL1008_1    MAP_EL5101_1    MAP_AZD_CTRL_1      MAP_ELTESTI_1     MAP_ELTESTO_1    MAP_AZD_CTRL_2      MAP_AKD_CTRL_1
MAP_NUM_DRIVES_ATTACHED(                  0,                  0,              0,              0,              2,                   0,               0,               1,                  1);
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,               NULL,           NULL,           NULL,           ec_pdo_map_azd3a_ked,      NULL,            NULL,            ec_pdo_map_azd3a_ked,     ec_pdo_map_akd );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,               NULL,           NULL,           NULL,           ec_nvram_sdos_azdxa_ked,   NULL,            NULL,            ec_nvram_sdos_azdxa_ked,  ec_nvram_sdos_akd);
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         NULL,               NULL,           NULL,           NULL,           NULL,                NULL,            NULL,            NULL,               ec_standard_sdos_akd);
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,               NULL,           NULL,           NULL,           ec_initial_pdo_azdxa_ked,  NULL,            NULL,            ec_initial_pdo_azdxa_ked, NULL);
MAP_SLAVE_DC_TYPE(                        EC_DC_NONE,         EC_DC_NONE,     EC_DC_NONE,     EC_DC_NONE,     EC_DC_0,             EC_DC_NONE,      EC_DC_NONE,      EC_DC_0,            EC_DC_NONE            );
MAP_SLAVE_DC_CYCLE(                       0,                  0,              0,              0,              4,                    0,               0,              4,                  0     );



/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AZD1.1",                           "AZD1.2",                           "AZD2.1",                           "AKD1");
MAP_DRIVE_TO_SLAVE(                         MAP_AZD_CTRL_1,                     MAP_AZD_CTRL_1,                     MAP_AZD_CTRL_2,                     MAP_AKD_1);
MAP_DRIVE_TO_SUBDRIVE(                      1,                                  2,                                  1,                                  1);
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_azdxa_ked,           ec_get_moo_pdo_azdxa_ked,           ec_get_moo_pdo_azdxa_ked,           NULL);
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                               NULL,                               NULL,                               ec_get_moo_sdo_akd);
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_azdxa_ked,            ec_get_remote_azdxa_ked,            ec_get_remote_azdxa_ked,            ec_get_remote_akd);
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_azdxa_ked,      ec_get_follow_error_azdxa_ked,      ec_get_follow_error_azdxa_ked,      ec_get_follow_error_akd);
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   ec_get_error_string_pdo_azdxa_ked,  ec_get_error_string_pdo_azdxa_ked,  ec_get_error_string_pdo_azdxa_ked,  NULL );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   NULL,                               NULL,                               NULL,                               ec_get_error_string_sdo_akd);
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_azdxa_ked,          ec_set_ctrl_wrd_azdxa_ked,          ec_set_ctrl_wrd_azdxa_ked,          ec_set_ctrl_wrd_akd);
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_azdxa_ked,          ec_get_stat_wrd_azdxa_ked,          ec_get_stat_wrd_azdxa_ked,          ec_get_stat_wrd_akd );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_azdxa_ked,        ec_get_actpos_wrd_azdxa_ked,        ec_get_actpos_wrd_azdxa_ked,        ec_get_actpos_wrd_akd);
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_azdxa_ked,        ec_set_setpos_wrd_azdxa_ked,        ec_set_setpos_wrd_azdxa_ked,        ec_set_setpos_wrd_akd);
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL,                               NULL,                               NULL,                               NULL);                                 );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL,                               NULL,                               NULL,                               NULL);                                 );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL,                               NULL,                               NULL,                               NULL);
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL,                               NULL,                               NULL,                               NULL);
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  NULL,                               NULL,                               NULL,                               NULL);
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(  NULL,                               NULL,                               NULL,                               NULL);
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            NULL,                               NULL,                               NULL,                               NULL);
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                               NULL,                               NULL,                               NULL                                );
MAP_DRIVE_RUN_HOMING(                       0,                                  0,                                  0,                                  0                                   );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL,                               NULL,                               NULL,                               NULL);



/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        999,                            999,                                999,                            999);
MAP_DRIVE_NEG_LIMIT(                        -999,                           -999,                               -999,                           -999);
MAP_DRIVE_DIRECTION(                        1,                              1,                                  0,                              1);
MAP_DRIVE_TORQ_LIMIT(                       0,                              0,                                  0,                              0);
MAP_DRIVE_VEL_LIMIT(                        0,                              0,                                  0,                              0);
MAP_DRIVE_SCALES(                           {0},                            {0},                                {0},                            {0});

extern bool mydin1_test, mydin2_test, mydout1_test, mydout2_test, plc_din1;

///*IO MAP*/
mapping_t map_iomap[10] = {
        {{MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=1, .datatype=ECT_BOOLEAN},                            {MAP_IN, .ionum=1, .datatype=ECT_BOOLEAN},    {MAP_IN, .linked_task_name="test_task1", .io=&mydin1_test, .datatype=ECT_BOOLEAN}},
        {{MAP_IN,  .slave_num=MAP_EL1008_1,.bit_num=2, .datatype=ECT_BOOLEAN},                           {MAP_IN, .ionum=2, .datatype=ECT_BOOLEAN},    {}},
        {{MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=1, .datatype=ECT_BOOLEAN},                           {MAP_OUT, .ionum=1, .datatype=ECT_BOOLEAN},    {.inout=MAP_OUT, .linked_task_name="test_task1", .io=&mydout1_test, .datatype=ECT_BOOLEAN}},
        {{MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=2, .datatype=ECT_BOOLEAN},                           {MAP_OUT, .ionum=2, .datatype=ECT_BOOLEAN},    {MAP_OUT, .linked_task_name="david", .io=&plc_din1, .datatype=ECT_BOOLEAN}},
        {{MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=3, .datatype=ECT_BOOLEAN},                           {MAP_OUT, .ionum=3, .datatype=ECT_BOOLEAN},    {}},
        {{MAP_IN, .slave_num=MAP_ELTESTI_1,.byte_num=ELTESTI_ANALOG_INPUT_INDEX, .datatype=ECT_REAL32, .max_val=50},   {MAP_IN, .ionum=0, .datatype=ECT_REAL32},     {}},
        {{MAP_OUT, .slave_num=MAP_ELTESTO_1,.byte_num=ELTEST0_ANALOG_OUPUT_INDEX, .datatype=ECT_REAL32},  {MAP_OUT, .ionum=0, .datatype=ECT_REAL32},     {}},
        {{MAP_IN, .slave_num=MAP_EL5101_1, .byte_num=EL5101_ACT_VALUE_INDEX, .datatype=ECT_INTEGER16},    {MAP_IN, .ionum=0, .datatype=ECT_INTEGER32},  {}},
        {{MAP_IN, .slave_num=MAP_EL5101_1, .byte_num=EL5101_ACT_VALUE_INDEX, .datatype=ECT_INTEGER16, .max_val=50},    {MAP_IN, .ionum=1, .datatype=ECT_REAL32},  {}},
        {{MAP_OUT, .slave_num=MAP_EL5101_1, .byte_num=EL5101_SET_VALUE_INDEX, .datatype=ECT_INTEGER16},   {MAP_OUT, .ionum=0, .datatype=ECT_INTEGER32},  {}},
};

uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);


/* This must be laid out in the order they appear in netscan  */
const map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {
        {
                .name = EK1100_EEP_NAME},
        {
                .name = EL2008_EEP_NAME},
        {
                .name = EL1008_EEP_NAME},
        {
                .name = EL5101_EEP_NAME},
        {
            .name = AZD3A_KED_EEP_NAME,
            .eep_id = AZD3A_KED_EEP_ID,
            .eep_man = AZD3A_KED_EEP_MAN,
            .eep_rev = AZD3A_KED_EEP_REV},
        {
                .name = ELTESTI_EEP_NAME},
        {
                .name= ELTESTO_EEP_NAME},
        {
            .name = AZD3A_KED_EEP_NAME},
        {.name = AKD_EEP_NAME}};


/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_azdxa_ked,        ec_get_ctrl_wrd_rev_azdxa_ked,             ec_get_ctrl_wrd_rev_azdxa_ked,       ec_get_ctrl_wrd_rev_akd);
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_azdxa_ked,        ec_set_stat_wrd_rev_azdxa_ked,             ec_set_stat_wrd_rev_azdxa_ked,       ec_set_stat_wrd_rev_akd);
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_azdxa_ked,      ec_set_actpos_wrd_rev_azdxa_ked,           ec_set_actpos_wrd_rev_azdxa_ked,     ec_set_actpos_wrd_rev_akd);
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_azdxa_ked,     ec_get_setpos_word_rev_azdxa_ked,          ec_get_setpos_word_rev_azdxa_ked,    ec_get_setpos_word_rev_akd);
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_azdxa_ked,         ec_set_moo_pdo_rev_azdxa_ked,              ec_set_moo_pdo_rev_azdxa_ked,        NULL);

#endif
