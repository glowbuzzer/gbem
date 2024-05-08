/**
 ******************************************************************************
 * @file           :  machine_bbhj25.c
 * @brief          :  BBH SCU-1-EC + AW J25 FSoE safety PLC program
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
#if MACHINE_BBH_J25 == 1

#include "bbh.h"
#include "automationware.h"

//@formatter:off

/* MACHINE INFO */
MAP_MACHINE_GET_SAFETY_STATE_FUNCTION(          ec_fsoe_get_safety_state_bbh_scu_1_ec);

/* FSOE MASTER INFO */
MAP_FSOE_MASTER_SLOT_CONFIG(                    MDP_SLOT_TYPE_BBH_32_12);
MAP_FSOE_MASTER_SLOT_TO_SLAVE(                  MAP_AW_J25_FSOE_1);
MAP_FSOE_MASTER_SET_ERROR_ACK_STATE_FUNCTION(   ec_fsoe_set_error_ack_state_bbh_scu_1_ec);


/* SLAVES */
//                                          Slave 1                                 Slave 2
//                                          MAP_BBH_SCU_1_EC_                       MAP_AW_J25_FSOE_1
//                                          Safety PLC                              AW J25 joint FSoE
MAP_NUM_DRIVES_ATTACHED(                    0,                                      1);
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(            NULL,                                   ec_pdo_map_aw_j_series_fsoe);
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(              NULL,                                   NULL);
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(           ec_apply_standard_sdos_bbh_scu_1_ec,    ec_apply_standard_sdos_aw_j_series_fsoe );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(            NULL,                                   ec_initial_pdo_aw_j_series);
MAP_SLAVE_CUSTOM_FMMU_SM_FUNCTIONS(         ec_custom_fmmu_sm_bbh_scu_1_ec,         ec_custom_fmmu_sm_aw_j_series);
MAP_SLAVE_DC_TYPE(                          EC_DC_NONE,                             EC_DC_0);
MAP_SLAVE_DC_CYCLE(                         0,                                      4) ;
MAP_SLAVE_EXEC_FUNCTIONS(                   NULL,                                   NULL);
MAP_SLAVE_OPTIONAL(                         false,                                  false);



/* FSoE */
MAP_SLAVE_FSOE_SLAVE_TYPE(                  FSOE_SLAVE_TYPE_SCU_1_EC,               FSOE_SLAVE_TYPE_SYNAPTICON              );
MAP_SLAVE_FSOE_SLAVE_FUNCTION(              FSOE_SLAVE_FUNCTION_MASTER,             FSOE_SLAVE_FUNCTION_SLAVE               );
MAP_SLAVE_FSOE_OFFSET_IN(                   BBH_SCU_1_EC_FSOE_IN_OFFSET,            AW_J_SERIES_EC_FSOE_SM2_OFFSET          );
MAP_SLAVE_FSOE_OFFSET_OUT(                  BBH_SCU_1_EC_FSOE_OUT_OFFSET,           AW_J_SERIES_EC_FSOE_SM3_OFFSET          );
MAP_SLAVE_FSOE_GET_SLAVE_STATE_FUNCTIONS(   NULL,                                   ec_fsoe_get_slave_state_aw_j_series     );
MAP_SLAVE_FSOE_GET_SLAVE_CON_ID_FUNCTIONS(  NULL,                                   ec_fsoe_get_slave_con_id_aw_j_series    );
MAP_SLAVE_FSOE_GET_MASTER_STATE_FUNCTIONS(  ec_fsoe_get_master_state_bbh_scu_1_ec,  NULL                                    );
MAP_FSOE_MASTER_CONTROL_FUNCTION(NULL);

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {
        {.name = BBH_SCU_1_EC_EEP_NAME, .eep_id = BBH_SCU_1_EC_EEP_ID, .eep_man = BBH_SCU_1_EC_EEP_MAN, .eep_rev = BBH_SCU_1_EC_EEP_REV},
        {.name = AW_EEP_NAME, .eep_id = AW_EEP_ID, .eep_man = AW_EEP_MAN, .eep_rev = AW_EEP_REV    },
};



/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AW_J25_FSOE"                           );
MAP_DRIVE_TO_SLAVE(                         MAP_AW_J25_FSOE_1,                      );
MAP_DRIVE_TO_SUBDRIVE(                      1,                                      );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_aw_j_series,             );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                                   );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_aw_j_series,              );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_aw_j_series,        );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   ec_get_error_string_pdo_aw_j_series,                                   );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   NULL     );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_aw_j_series,            );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_aw_j_series,            );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_aw_j_series,          );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         ec_get_actvel_wrd_aw_j_series,          );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        ec_get_acttorq_wrd_aw_j_series,         );
MAP_DRIVE_GET_CONTROL_EFFORT_WRD_FUNCTIONS( ec_get_control_effort_wrd_aw_j_series   );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_aw_j_series,          );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         ec_set_setvel_wrd_aw_j_series,          );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        ec_set_settorq_wrd_aw_j_series,         );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  ec_set_settorqoffset_wrd_aw_j_series    );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   ec_set_setveloffset_wrd_aw_j_series     );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            ec_set_moo_pdo_aw_j_series,             );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                                   );
MAP_DRIVE_RUN_HOMING(                       0,                                      );
MAP_DRIVE_GET_SECONDARY_NAME_FUNCTION(      ec_get_secondary_name_aw_j_series       );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           ec_print_params_aw_j_series             );
MAP_DRIVE_TYPE(                             DRIVE_TYPE_AW_J25                       );
MAP_DRIVE_GET_LOG_FILE_FUNCTIONS(           ec_get_log_file_aw_j_series             );

/* DRIVE PARAMETERS */

MAP_DRIVE_DIRECTION(                        1,                                      );
MAP_DRIVE_SCALES(                           {166886,9549,32.67}                     );

/* PLC IO CONFIG */



/*IO MAP*/
mapping_t map_iomap[18] = {
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},             {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_0, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=7, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},             {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=8, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=9, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=10, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=11, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=12, .type = GBC_IO_TYPE_SAFETY},      {}},

    //bit 0 is used by gbem to set the error ack
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=0, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=1, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=2, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=3, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=4, .type = GBC_IO_TYPE_SAFETY},     {}},
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
