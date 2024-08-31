/**
 ******************************************************************************
 * @file           :  machine_mini.c
 * @brief          :  machine map for mini
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
#if MACHINE_MINI == 1

#include "azdxa-ked.h"
#include "beckhoff.h"


MAP_MACHINE_GET_SAFETY_STATE_FUNCTION(ec_get_safety_state_azdxa_ked);

/* FSOE MASTER INFO */
MAP_FSOE_MASTER_SLOT_CONFIG();
MAP_FSOE_MASTER_SLOT_TO_SLAVE();

MAP_FSOE_MASTER_SET_ERROR_ACK_STATE_FUNCTION(NULL);


//@formatter:off
map_pdo_object_t ctrl_estop_din = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_EL1008_1, .bit_num=1};


/* SLAVES */
//                                        Slave 1                       Slave 2                  Slave 3                   Slave 4
//                                        MAP_EK1100_1                  MAP_EL2008_1             MAP_EL1008_1              MAP_AZD_CTRL_1
//                                        Coupler                       8 dig out                8 dig in                  3 axis stepper
MAP_NUM_DRIVES_ATTACHED(                  0,                            0,                       0,                        2                          );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,                         NULL,                    NULL,                     ec_pdo_map_custom_azd3a_ked);
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,                         NULL,                    NULL,                     ec_nvram_sdos_azdxa_ked    );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         NULL,                         NULL,                    NULL,                     ec_apply_standard_sdos_azdxa_ked );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,                         NULL,                    NULL,                     ec_initial_pdo_azdxa_ked   );
MAP_SLAVE_CUSTOM_FMMU_SM_FUNCTIONS(       NULL,                         NULL,                    NULL,                     NULL);
MAP_SLAVE_DC_TYPE(                        EC_DC_NONE,                   EC_DC_NONE,              EC_DC_NONE,               EC_DC_0                    );
MAP_SLAVE_DC_CYCLE(                       0,                            0,                       0,                        4);
MAP_SLAVE_EXEC_FUNCTIONS(                 NULL,                         NULL,                    NULL,                     NULL);
MAP_SLAVE_OPTIONAL(                       false,                        false,                   false,                    false);



/* FSoE */
MAP_SLAVE_FSOE_SLAVE_TYPE(                 FSOE_SLAVE_TYPE_NONE,       FSOE_SLAVE_TYPE_NONE,     FSOE_SLAVE_TYPE_NONE,     FSOE_SLAVE_TYPE_NONE,           );
MAP_SLAVE_FSOE_SLAVE_FUNCTION(             FSOE_SLAVE_FUNCTION_NONE,   FSOE_SLAVE_FUNCTION_NONE, FSOE_SLAVE_FUNCTION_NONE, FSOE_SLAVE_FUNCTION_NONE,       );
MAP_SLAVE_FSOE_OFFSET_IN(                  0,                          0,                        0,                        0,                              );
MAP_SLAVE_FSOE_OFFSET_OUT(                 0,                          0,                        0,                        0,                              );
MAP_SLAVE_FSOE_GET_SLAVE_STATE_FUNCTIONS(  NULL,                       NULL,                     NULL,                     NULL,                           );
MAP_SLAVE_FSOE_GET_SLAVE_CON_ID_FUNCTIONS( NULL,                       NULL,                     NULL,                     NULL,                           );
MAP_SLAVE_FSOE_GET_MASTER_STATE_FUNCTIONS( NULL,                       NULL,                     NULL,                     NULL,                           );
MAP_FSOE_MASTER_CONTROL_FUNCTION(NULL);

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AZD1.1",                           "AZD1.2"                            );
MAP_DRIVE_TO_SLAVE(                         MAP_AZD_CTRL_1,                     MAP_AZD_CTRL_1                      );
MAP_DRIVE_TO_SUBDRIVE(                      1,                                  2                                   );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_azdxa_ked,           ec_get_moo_pdo_azdxa_ked            );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                               NULL                                );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_azdxa_ked,            ec_get_remote_azdxa_ked             );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_azdxa_ked,      ec_get_follow_error_azdxa_ked       );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL,                               NULL                                );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_azdxa_ked,  ec_get_error_string_sdo_azdxa_ked   );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_azdxa_ked,          ec_set_ctrl_wrd_azdxa_ked           );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_azdxa_ked,          ec_get_stat_wrd_azdxa_ked           );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_azdxa_ked,        ec_get_actpos_wrd_azdxa_ked         );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_azdxa_ked,        ec_set_setpos_wrd_azdxa_ked         );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         ec_get_actvel_wrd_azdxa_ked,        ec_get_actvel_wrd_azdxa_ked         );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL,                               NULL                                );
MAP_DRIVE_GET_CONTROL_EFFORT_WRD_FUNCTIONS( NULL,                               NULL                                );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         ec_set_setvel_wrd_azdxa_ked,        ec_set_setvel_wrd_azdxa_ked         );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL,                               NULL,                               );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  NULL,                               NULL,                               );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   NULL,                               NULL,                               );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            ec_set_moo_pdo_azdxa_ked,           ec_set_moo_pdo_azdxa_ked            );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                               NULL                                );
MAP_DRIVE_RUN_HOMING(                       0,                                  0                                   );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL,                               NULL                                );
MAP_DRIVE_GET_SECONDARY_NAME_FUNCTION(      NULL,                               NULL,                               );
MAP_DRIVE_TYPE(                             DRIVE_TYPE_OM,                      DRIVE_TYPE_OM,                      );


/* DRIVE PARAMETERS */
MAP_DRIVE_SCALES(                           {0},                                {0}                                 );



extern bool plc_din1, plc_din2, plc_din3, plc_dout1, plc_dout2, plc_dout3;


/*IO MAP*/
mapping_t map_iomap[4] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=1, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din1, .linked_task_name="Task1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1008_1,.bit_num=2, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_din2, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=1, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=0},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout1, .linked_task_name="Task1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2008_1,.bit_num=2, .datatype=ECT_BOOLEAN},                          {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=1},    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_dout2, .linked_task_name="Task1"}},
        };

uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {
        {.name = EK1100_EEP_NAME,   .eep_id = EK1100_EEP_ID,   .eep_man = EK1100_EEP_MAN,     .eep_rev = EK1100_EEP_REV },
        {.name = EL2008_EEP_NAME,   .eep_id = EL2008_EEP_ID,   .eep_man = EL2008_EEP_MAN,     .eep_rev = EL2008_EEP_REV },
        {.name = EL1008_EEP_NAME,   .eep_id = EL1008_EEP_ID,   .eep_man = EL1008_EEP_MAN,     .eep_rev = EL1008_EEP_REV },
        {.name = AZD3A_KED_EEP_NAME,      .eep_id = AZD3A_KED_EEP_ID,      .eep_man = AZD3A_KED_EEP_MAN,        .eep_rev = AZD3A_KED_EEP_REV    },
};


/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_azdxa_ked,        ec_get_ctrl_wrd_rev_azdxa_ked             );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_azdxa_ked,        ec_set_stat_wrd_rev_azdxa_ked             );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_azdxa_ked,      ec_set_actpos_wrd_rev_azdxa_ked           );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_azdxa_ked,     ec_get_setpos_word_rev_azdxa_ked          );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_azdxa_ked,         ec_set_moo_pdo_rev_azdxa_ked              );


//@formatter:on

#endif //MACHINE_MINI
