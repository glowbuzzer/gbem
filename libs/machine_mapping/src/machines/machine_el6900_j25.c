/**
 ******************************************************************************
 * @file           :  machine_el6900_j25.c
 * @brief          :  EL6900+EL1904+EL2904+AW J25
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
#if MACHINE_EL6900_J25 == 1

#include "beckhoff.h"
#include "automationware.h"

//@formatter:off

/* SLAVES */
//                                          Slave 1                 Slave 2             Slave 3             Slave 4             Slave 5
//                                          MAP_EK1100_1            MAP_EL6900_1        MAP_EL1904_1        MAP_EL2904          MAP_AW_J25_1
//                                          Coupler                 Safety PLC          4 safe dig in       4 safe dig out      AW J25 joint
MAP_NUM_DRIVES_ATTACHED(                    0,                      0,                  0,                  0,                  1);
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(            NULL,                   NULL,               NULL,               NULL,               ec_pdo_map_aw_j_series);
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(              NULL,                   NULL,               NULL,               NULL,               NULL);
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(           NULL,                   NULL,               NULL,               NULL,               ec_apply_standard_sdos_aw_j17 );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(            NULL,                   NULL,               NULL,               NULL,               ec_initial_pdo_aw_j_series);
MAP_SLAVE_DC_TYPE(                          EC_DC_NONE,             EC_DC_NONE,         EC_DC_NONE,         EC_DC_NONE,         EC_DC_0);
MAP_SLAVE_DC_CYCLE(                         0,                      0,                  0,                  0,                  4 ;
MAP_SLAVE_FSOE_MASTER(                      false,                  true,              false,              false,               false);
MAP_SLAVE_FSOE_MASTER_TO_SLAVE_BYTES(       0,                      0,                  6,                  6,                  0);
MAP_SLAVE_FSOE_SLAVE_TO_MASTER_BYTES(       0,                      0,                  6,                  6,                  0);

MAP_FSOE_MASTER_CONTROL_FUNCTION(NULL);

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {
        {.name = EK1100_EEP_NAME, .eep_id = EK1100_EEP_ID, .eep_man = EK1100_EEP_MAN, .eep_rev = EK1100_EEP_REV},
        {.name = EL6900_EEP_NAME, .eep_id = EL6900_EEP_ID, .eep_man = EL6900_EEP_MAN, .eep_rev = EL6900_EEP_REV},
        {.name = EL1904_EEP_NAME, .eep_id = EL1904_EEP_ID, .eep_man = EL1904_EEP_MAN, .eep_rev = EL1904_EEP_REV},
        {.name = EL2904_EEP_NAME, .eep_id = EL2904_EEP_ID, .eep_man = EL2904_EEP_MAN, .eep_rev = EL2904_EEP_REV},
        {.name = AW_J25_EEP_NAME, .eep_id = AW_J17_EEP_ID, .eep_man = AW_J17_EEP_MAN, .eep_rev = AW_J17_EEP_REV    },
};



/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AW_J25_FSOE"                           );
MAP_DRIVE_TO_SLAVE(                         MAP_AW_J25_FSOE_CTRL,                   );
MAP_DRIVE_TO_SUBDRIVE(                      1,                                      );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_aw_j_series,             );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                                   );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_aw_j_series,              );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_aw_j_series,        );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL,                                   );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_aw_j_series     );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_aw_j_series,            );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_aw_j_series,            );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_aw_j_series,          );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         ec_get_actvel_wrd_aw_j_series,          );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        ec_get_acttorq_wrd_aw_j_series,         );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_aw_j_series,          );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         ec_set_setvel_wrd_aw_j_series,          );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        ec_set_settorq_wrd_aw_j_series,         );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  ec_set_settorqoffset_wrd_aw_j_series    );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   ec_set_setveloffset_wrd_aw_j_series     );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            ec_set_moo_pdo_aw_j_series,             );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                                   );
MAP_DRIVE_RUN_HOMING(                       0,                                      );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           ec_print_params_aw_j_series             );


/* DRIVE PARAMETERS */
//FOR AW DRIVES THESE ARE IN DEGREES
MAP_DRIVE_POS_LIMIT(                        9999,                                  );
MAP_DRIVE_NEG_LIMIT(                        -9999,                                 );
//percentage of max torque
MAP_DRIVE_TORQ_LIMIT(                       30,                                  );
MAP_DRIVE_VEL_LIMIT(                        50,                                  );
MAP_DRIVE_SCALES(                           {166886,9549,32.67});


/* PLC IO CONFIG */

/*IO MAP*/
mapping_t map_iomap[32] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},             {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=7},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},             {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=8},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=9},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=10},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=11},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=12},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=13},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=14},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=15},     {}},

        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=0},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=1},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=2},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=3},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=4},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=5},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=6},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=7},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=8},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=9},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=10},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=11},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=12},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=13},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=14},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=13, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=15},     {}}
};


uint16_t map_num_rows_in_iomap = sizeof(map_iomap) / sizeof(map_iomap[0]);



/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS();
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS();
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS();
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS();
MAP_DRIVE_SET_MOO_REV_FUNCTIONS();


//@formatter:on




#endif //MACHINE_EL6900_J25
