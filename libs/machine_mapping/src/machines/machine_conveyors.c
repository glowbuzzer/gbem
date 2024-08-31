
/**
 ******************************************************************************
 * @file           :  machine_conveyors.c
 * @brief          :  machine map for conveyors machine
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "map.h"

#if MACHINE_CONVEYORS == 1
#include "kollmorgen.h"
#include "beckhoff.h"

//@formatter:off
/* SLAVES */
//                                        Slave 1             Slave 2         Slave 3         Slave 4         Slave 5               Slave 6
//                                        MAP_EK1100_1        MAP_EL2008_1    MAP_EL1008_1    MAP_EL2022_1   MAP_AKD1              MAP_AKD2
//                                        Coupler             8 dig out       8 dig in        2 fig ou       single servo          single servo
MAP_NUM_DRIVES_ATTACHED(                  0,                  0,              0,              0,              1,                    1                   );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,               NULL,           NULL,           NULL,           ec_pdo_map_akd,       ec_pdo_map_akd      );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,               NULL,           NULL,           NULL,           ec_nvram_sdos_akd,    ec_nvram_sdos_akd   );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         NULL,               NULL,           NULL,           NULL,           ec_standard_sdos_akd, ec_standard_sdos_akd);
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,               NULL,           NULL,           NULL,           NULL,                 NULL                );
MAP_SLAVE_DC_TYPE(                        EC_DC_0,            EC_DC_NONE,     EC_DC_NONE,     EC_DC_NONE,     EC_DC_0,              EC_DC_0             );
MAP_SLAVE_DC_CYCLE(                       0,                  0,              0,              0,              4,                    4                   );
//set moo is in either initial PDO or custom sdos

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AKD1",                         "AKD2"                              );
MAP_DRIVE_TO_SLAVE(                         MAP_AKD_1,                      MAP_AKD_2                           );
MAP_DRIVE_TO_SUBDRIVE(                      1,                              1                                   );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            NULL,                           NULL                                );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            ec_get_moo_sdo_akd,             ec_get_moo_sdo_akd                  );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_akd,              ec_get_remote_akd                   );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_akd,        ec_get_follow_error_akd             );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL,                           NULL                                );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_akd,    ec_get_error_string_sdo_akd         );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_akd,            ec_set_ctrl_wrd_akd                 );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_akd,            ec_get_stat_wrd_akd                 );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_akd,          ec_get_actpos_wrd_akd               );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_akd,          ec_set_setpos_wrd_akd               );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL,                           NULL                                );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL,                           NULL                                );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL,                           NULL,                               );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL,                           NULL,                               );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  NULL,                           NULL,                               );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   NULL,                           NULL,                               );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            NULL,                           NULL,                               );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                           NULL                                );
MAP_DRIVE_RUN_HOMING(                       0,                              0                                   );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL,                           NULL                                );

/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        999,                            999                                 );
MAP_DRIVE_NEG_LIMIT(                        -999,                           -999                                );
MAP_DRIVE_TORQ_LIMIT(                       0,                              0                                   );
MAP_DRIVE_VEL_LIMIT(                        0,                              0                                   );
MAP_DRIVE_SCALES(                           {0},                            {0}                                 );



/* IO MAP */
mapping_t map_iomap[18] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=1, .datatype=ECT_BOOLEAN},   {.inout=MAP_IN, .ionum=0, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=2, .datatype=ECT_BOOLEAN},   {.inout=MAP_IN, .ionum=1, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=3, .datatype=ECT_BOOLEAN},   {.inout=MAP_IN, .ionum=2, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=4, .datatype=ECT_BOOLEAN},   {.inout=MAP_IN, .ionum=3, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=5, .datatype=ECT_BOOLEAN},   {.inout=MAP_IN, .ionum=4, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=6, .datatype=ECT_BOOLEAN},   {.inout=MAP_IN, .ionum=5, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=7, .datatype=ECT_BOOLEAN},   {.inout=MAP_IN, .ionum=6, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=8, .datatype=ECT_BOOLEAN},   {.inout=MAP_IN, .ionum=7, .datatype=ECT_BOOLEAN},    {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=1, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=0, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=2, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=1, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=3, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=2, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=4, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=3, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=5, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=4, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=6, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=5, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=7, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=6, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=8, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=7, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2022_1,.bit_num=1, .datatype=ECT_BOOLEAN},  {.inout=MAP_OUT, .ionum=8, .datatype=ECT_BOOLEAN},   {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2022_1,.bit_num=2, .datatype=ECT_BOOLEAN}, {.inout=MAP_OUT, .ionum=9, .datatype=ECT_BOOLEAN},   {}},
};



uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/* This must be laid out in the order they appear in netscan */
MAP_SLAVE_EEP ({EK1100_EEP_NAME},{EL2008_EEP_NAME}, {EL1008_EEP_NAME},{EL2022_EEP_NAME}, {AKD_EEP_NAME, AKD_EEP_ID, AKD_EEP_MAN, AKD_EEP_REV}, {AKD_EEP_NAME, AKD_EEP_ID, AKD_EEP_MAN, AKD_EEP_REV});


/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_akd,        ec_get_ctrl_wrd_rev_akd             );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_akd,        ec_set_stat_wrd_rev_akd             );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_akd,      ec_set_actpos_wrd_rev_akd           );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_akd,     ec_get_setpos_word_rev_akd          );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            NULL,                           NULL                                );

//@formatter:on

#endif


/*
 * EK1100
 * EL1008 (input)
 *  Term 1 - Pnu extended switch (Box 3)
 *  Term 2 - Pnu retracted switch (Box 4)
 *  Term 3 - Magic eye input (Box 5)
 *  Term 4 -
 *  Term 5 - Safety relay
 *  Term 6 -
 *  Term 7 -
 *  Term 8 -
 * EL2008 (output)
 *  Term 1 -
 *  Term 2 -
 *  Term 3 -
 *  Term 4 - camera trigger 1
 *  Term 5 - camera trigger 2
 *  Term 6 - red tower led
 *  Term 7 - orange tower led
 *  Term 8 - green lower led
 * EL2022
 *  Term 1 - Pnu extend (Box 1)
 *  Term 2 - Pnu retract  (Box 2)
 */



/*
 * o/p from netscan
 *
 *
 * Slave:1 - Name:EK1100 - 0x1001
 * Slave:2 - Name:EL2008 - 0x1002
 * Slave:3 - Name:EL1008 - 0x1003
 * Slave:4 - Name:EL2022 - 0x1004
 * Slave:5 - Name:AKD - 0x1005
 * Slave:6 - Name:AKD - 0x1006





 *
 *
 */

