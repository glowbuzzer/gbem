
/**
 ******************************************************************************
 * @file           :  machine_template_map.c
 * @brief          :  a blank machine_map template
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
#if BLANK  == 1



#include "azd.h"
#include "beckhoff.h"


//@formatter:off
/* SLAVES */
//                                        Slave 1             Slave 2           Slave 3         Slave 4         Slave 5
//                                        MAP_SLAVE_1         MAP_SLAVE_2       MAP_EL1008_1    MAP_EL5101_1    MAP_AZD_CTRL_1      );
//                                        Description Slv 1   Description Slv 1 Description Slv 1          8 dig out       8 dig in        Encoder         3 axis stepper
MAP_NUM_DRIVES_ATTACHED(                  0,                  0,              0,              0,              0                   );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,               NULL,           NULL,           NULL,           NULL                );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,               NULL,           NULL,           NULL,           ec_nvram_sdos_azd   );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         NULL,               NULL,           NULL,           NULL,           NULL                );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,               NULL,           NULL,           NULL,           ec_initial_pdo_azd  );
MAP_SLAVE_DC_TYPE(                        EC_DC_NONE,         EC_DC_NONE,     EC_DC_NONE,     EC_DC_NONE,     EC_DC_NON           );

//set moo is in either initial PDO or custom sdos

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AZD1.1",                       "AZD1.2"                            );
MAP_DRIVE_TO_SLAVE(                         MAP_AZD_CTRL_1,                 MAP_AZD_CTRL_1                      );
MAP_DRIVE_TO_SUBDRIVE(                      1,                              2                                   );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_azd,             ec_get_moo_pdo_azd                  );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                           NULL                                );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_azd,              ec_get_remote_azd                   );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_azd,        ec_get_follow_error_azd             );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   ec_get_error_string_pdo_azd,    ec_get_error_string_pdo_azd         );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   NULL,                           NULL                                );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_azd,            ec_set_ctrl_wrd_azd                 );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_azd,            ec_get_stat_wrd_azd                 );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_azd,          ec_get_actpos_wrd_azd               );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_azd,          ec_set_setpos_wrd_azd               );

/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        999,                            999                                 );
MAP_DRIVE_NEG_LIMIT(                        -999,                           -999                                );
MAP_DRIVE_DIRECTION(                        1,                              1                                   );



extern bool plc_din1, plc_din2, plc_din3, plc_dout1, plc_dout2, plc_dout3;


/*IO MAP*/
mapping_t map_iomap[5] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=1, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din1, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=2, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din2, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=1, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=0},    {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=2, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=1},    {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL5101_1, .byte_num=EL5101_ACT_VALUE_INDEX, .datatype=ECT_INTEGER16},   {.inout=MAP_IN, .datatype=ECT_INTEGER32, .ionum=0},   {}},
};

uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES] = {
        {.name = EK1100_EEP_NAME,   .eep_id = EK1100_EEP_ID,   .eep_man = EK1100_EEP_MAN,     .eep_rev = EK1100_EEP_REV },
        {.name = EL2008_EEP_NAME,   .eep_id = EL2008_EEP_ID,   .eep_man = EL2008_EEP_MAN,     .eep_rev = EL2008_EEP_REV },
        {.name = EL1008_EEP_NAME,   .eep_id = EL1008_EEP_ID,   .eep_man = EL1008_EEP_MAN,     .eep_rev = EL1008_EEP_REV },
        {.name = EL5101_EEP_NAME,   .eep_id = EL5101_EEP_ID,   .eep_man = EL5101_EEP_MAN,     .eep_rev = EL5101_EEP_REV },
        {.name = AZD3A_KED_EEP_NAME,      .eep_id = AZD_3A_KEDEEP_ID,      .eep_man = AZD3A_KED_EEP_MAN,        .eep_rev = AZD3A_KED_EEP_REV    },
};


/* REVERSE FUNCTIONS - USED FOR TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_azd,        ec_get_ctrl_wrd_rev_azd             );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_azd,        ec_set_stat_wrd_rev_azd             );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_azd,      ec_set_actpos_wrd_rev_azd           );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_azd,     ec_get_setpos_word_rev_azd          );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_azd,         ec_set_moo_pdo_rev_azd              );


//@formatter:on

#endif //MACHINE_TEMPLATE
