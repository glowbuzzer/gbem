/**
 ******************************************************************************
 * @file           :  machine_aw_6dof_fsoe
 * @brief          :  machine map for aw 6dof robot with FSoE
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
#if MACHINE_AW_6DOF_FSOE == 1

#include "automationware.h"
#include "beckhoff.h"
#include "bbh.h"
//@formatter:off

/* MACHINE INFO */
MAP_MACHINE_GET_SAFETY_STATE_FUNCTION(          ec_fsoe_get_safety_state_bbh_scu_1_ec);

/* FSOE MASTER INFO */
MAP_FSOE_MASTER_SLOT_CONFIG(MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12 );
MAP_FSOE_MASTER_SLOT_TO_SLAVE(MAP_AW_J40LP_CTRL_1, MAP_AW_J40HP_CTRL_1, MAP_AW_J32_CTRL_1, MAP_AW_J25_CTRL_1, MAP_AW_J25_CTRL_2, MAP_AW_J20_CTRL_1 );
MAP_FSOE_MASTER_SET_ERROR_ACK_STATE_FUNCTION(ec_fsoe_set_error_ack_state_bbh_scu_1_ec);


/* SLAVES */
//                                        Slave 1                                  Slave 2                     Slave 3                   Slave 4                   Slave 5                         Slave 6                              Slave 7                              Slave 8                              Slave 9                                   Slave 10                                    Slave 11                                    Slave 12                                    Slave 13                                    Slave 14
//                                        MAP_BBH_SCU_1_EC_1                       MAP_EK1100_1                MAP_EL2808_1              MAP_EL1808_1              MAP_EL6021_1                    MAP_EL2535-0005_1                    MAP_ELM7231_1                        MAP_EK1110_1                         MAP_AW_J40LP_FSOE_1                       MAP_AW_J40LP_FSOE_1                         MAP_AW_J32_FSOE_1                           MAP_AW_J25_FSOE_1                           MAP_AW_J25_FSOE_2                           MAP_AW_J20_FSOE_1
//                                        Safety PLC                               Coupler                     8 dig out                 8 dig in                  RS485                           PWM                                  Drive                                Coupler                              J40LP joint                               J40HP joint                                 J32 joint                                   J25 joint                                   J25 joint                                   J20 joint
MAP_NUM_DRIVES_ATTACHED(                  0,                                       0,                          0,                        0,                        0,                              0,                                   1,                                   0,                                   1,                                        1,                                          1,                                          1,                                          1,                                          1                                           );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,                                    NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                ec_pdo_map_aw_j_series_fsoe,              ec_pdo_map_aw_j_series_fsoe,                ec_pdo_map_aw_j_series_fsoe,                ec_pdo_map_aw_j_series_fsoe,                ec_pdo_map_aw_j_series_fsoe,                ec_pdo_map_aw_j_series_fsoe                 );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,                                    NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                NULL,                                     NULL,                                       NULL,                                       NULL,                                       NULL,                                       NULL                                        );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         ec_apply_standard_sdos_bbh_scu_1_ec,     NULL,                       NULL,                     NULL,                     ec_apply_standard_sdos_el6021,  NULL,                                NULL,                                NULL,                                ec_apply_standard_sdos_aw_j_series_fsoe,  ec_apply_standard_sdos_aw_j_series_fsoe,    ec_apply_standard_sdos_aw_j_series_fsoe,    ec_apply_standard_sdos_aw_j_series_fsoe,    ec_apply_standard_sdos_aw_j_series_fsoe,    ec_apply_standard_sdos_aw_j_series_fsoe     );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,                                    NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                ec_initial_pdo_aw_j_series,               ec_initial_pdo_aw_j_series,                 ec_initial_pdo_aw_j_series,                 ec_initial_pdo_aw_j_series,                 ec_initial_pdo_aw_j_series,                 ec_initial_pdo_aw_j_series                  );
MAP_SLAVE_CUSTOM_FMMU_SM_FUNCTIONS(       ec_custom_fmmu_sm_bbh_scu_1_ec,          NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                ec_custom_fmmu_sm_aw_j_series,            ec_custom_fmmu_sm_aw_j_series,              ec_custom_fmmu_sm_aw_j_series,              ec_custom_fmmu_sm_aw_j_series,              ec_custom_fmmu_sm_aw_j_series,              ec_custom_fmmu_sm_aw_j_series               );
MAP_SLAVE_DC_TYPE(                        EC_DC_NONE,                              EC_DC_NONE,                 EC_DC_NONE,               EC_DC_NONE,               EC_DC_NONE,                     EC_DC_NONE,                          EC_DC_0,                             EC_DC_NONE,                          EC_DC_0,                                  EC_DC_0,                                    EC_DC_0,                                    EC_DC_0,                                    EC_DC_0,                                    EC_DC_0                                     );
MAP_SLAVE_DC_CYCLE(                       0,                                       0,                          0,                        0,                        0,                              0,                                   4,                                   0,                                   4,                                        4,                                          4,                                          4,                                          4,                                          4                                           );
MAP_SLAVE_EXEC_FUNCTIONS(                 NULL,                                    NULL,                       NULL,                     NULL,                     ec_slave_exec_el6021,           NULL,                                NULL,                                NULL,                                NULL,                                     NULL,                                       NULL,                                       NULL,                                       NULL,                                       NULL,                                       );
MAP_SLAVE_OPTIONAL(                       false,                                   false,                      false,                    false,                    true,                           true,                                true,                                true,                                false,                                    false,                                      false,                                      false,                                      false,                                      false,                                      );



/* FSoE */
MAP_SLAVE_FSOE_SLAVE_TYPE(                 FSOE_SLAVE_TYPE_SCU_1_EC,                FSOE_SLAVE_TYPE_NONE,       FSOE_SLAVE_TYPE_NONE,     FSOE_SLAVE_TYPE_NONE,     FSOE_SLAVE_TYPE_NONE,            FSOE_SLAVE_TYPE_NONE,               FSOE_SLAVE_TYPE_ELM7231,             FSOE_SLAVE_TYPE_NONE,                FSOE_SLAVE_TYPE_SYNAPTICON,              FSOE_SLAVE_TYPE_SYNAPTICON,                 FSOE_SLAVE_TYPE_SYNAPTICON,                 FSOE_SLAVE_TYPE_SYNAPTICON,                 FSOE_SLAVE_TYPE_SYNAPTICON,                 FSOE_SLAVE_TYPE_SYNAPTICON                  );
MAP_SLAVE_FSOE_SLAVE_FUNCTION(             FSOE_SLAVE_FUNCTION_MASTER,              FSOE_SLAVE_FUNCTION_NONE,   FSOE_SLAVE_FUNCTION_NONE, FSOE_SLAVE_FUNCTION_NONE, FSOE_SLAVE_FUNCTION_NONE,        FSOE_SLAVE_FUNCTION_NONE,           FSOE_SLAVE_FUNCTION_SLAVE,           FSOE_SLAVE_FUNCTION_NONE,            FSOE_SLAVE_FUNCTION_SLAVE,               FSOE_SLAVE_FUNCTION_SLAVE,                  FSOE_SLAVE_FUNCTION_SLAVE,                  FSOE_SLAVE_FUNCTION_SLAVE,                  FSOE_SLAVE_FUNCTION_SLAVE,                  FSOE_SLAVE_FUNCTION_SLAVE                   );
MAP_SLAVE_FSOE_OFFSET_IN(                  BBH_SCU_1_EC_FSOE_IN_OFFSET,             0,                          0,                        0,                        0,                               0,                                  0,                                   0,                                   AW_J_SERIES_EC_FSOE_SM2_OFFSET,          AW_J_SERIES_EC_FSOE_SM2_OFFSET,             AW_J_SERIES_EC_FSOE_SM2_OFFSET,             AW_J_SERIES_EC_FSOE_SM2_OFFSET,             AW_J_SERIES_EC_FSOE_SM2_OFFSET,             AW_J_SERIES_EC_FSOE_SM2_OFFSET              );
MAP_SLAVE_FSOE_OFFSET_OUT(                 BBH_SCU_1_EC_FSOE_OUT_OFFSET,            0,                          0,                        0,                        0,                               0,                                  0,                                   0,                                   AW_J_SERIES_EC_FSOE_SM3_OFFSET,          AW_J_SERIES_EC_FSOE_SM3_OFFSET,             AW_J_SERIES_EC_FSOE_SM3_OFFSET,             AW_J_SERIES_EC_FSOE_SM3_OFFSET,             AW_J_SERIES_EC_FSOE_SM3_OFFSET,             AW_J_SERIES_EC_FSOE_SM3_OFFSET              );
MAP_SLAVE_FSOE_GET_SLAVE_STATE_FUNCTIONS(  NULL,                                    NULL,                       NULL,                     NULL,                     NULL,                            NULL,                               NULL,                                NULL,                                ec_fsoe_get_slave_state_aw_j_series,     ec_fsoe_get_slave_state_aw_j_series,        ec_fsoe_get_slave_state_aw_j_series,        ec_fsoe_get_slave_state_aw_j_series,        ec_fsoe_get_slave_state_aw_j_series,        ec_fsoe_get_slave_state_aw_j_series         );
MAP_SLAVE_FSOE_GET_SLAVE_CON_ID_FUNCTIONS( NULL,                                    NULL,                       NULL,                     NULL,                     NULL,                            NULL,                               NULL,                                NULL,                                ec_fsoe_get_slave_con_id_aw_j_series,    ec_fsoe_get_slave_con_id_aw_j_series,       ec_fsoe_get_slave_con_id_aw_j_series,       ec_fsoe_get_slave_con_id_aw_j_series,       ec_fsoe_get_slave_con_id_aw_j_series,       ec_fsoe_get_slave_con_id_aw_j_series        );
MAP_SLAVE_FSOE_GET_MASTER_STATE_FUNCTIONS( ec_fsoe_get_master_state_bbh_scu_1_ec,   NULL,                       NULL,                     NULL,                     NULL,                            NULL,                               NULL,                                NULL,                                NULL,                                    NULL,                                       NULL,                                       NULL,                                       NULL,                                       NULL                                        );
MAP_FSOE_MASTER_CONTROL_FUNCTION(          NULL);





/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {
        {.name = BBH_SCU_1_EC_EEP_NAME, .eep_id = BBH_SCU_1_EC_EEP_ID, .eep_man = BBH_SCU_1_EC_EEP_MAN,     .eep_rev = BBH_SCU_1_EC_EEP_REV     },
        {.name = EK1100_EEP_NAME,       .eep_id = EK1100_EEP_ID,        .eep_man = EK1100_EEP_MAN,          .eep_rev = EK1100_EEP_REV           },
        {.name = EL2808_EEP_NAME,       .eep_id = EL2808_EEP_ID,        .eep_man = EL2808_EEP_MAN,          .eep_rev = EL2808_EEP_REV           },
        {.name = EL1808_EEP_NAME,       .eep_id = EL1808_EEP_ID,        .eep_man = EL1808_EEP_MAN,          .eep_rev = EL1808_EEP_REV           },
        {.name = EL6021_EEP_NAME,       .eep_id = EL6021_EEP_ID,        .eep_man = EL6021_EEP_MAN,          .eep_rev = EL6021_EEP_REV           },
        {.name = EL2535_0005_EEP_NAME,  .eep_id = EL2535_0005_EEP_ID,   .eep_man = EL2535_0005_EEP_MAN,     .eep_rev = EL2535_0005_EEP_REV      },
        {.name = ELM7231_9018_EEP_NAME, .eep_id = ELM7231_9018_EEP_ID,  .eep_man = ELM7231_9018_EEP_MAN,    .eep_rev = ELM7231_9018_EEP_REV     },
        {.name = EK1110_EEP_NAME,       .eep_id = EK1110_EEP_ID,        .eep_man = EK1110_EEP_MAN,          .eep_rev = EK1110_EEP_REV           },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,              .eep_rev = AW_EEP_REV               },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,              .eep_rev = AW_EEP_REV               },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,              .eep_rev = AW_EEP_REV               },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,              .eep_rev = AW_EEP_REV               },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,              .eep_rev = AW_EEP_REV               },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,              .eep_rev = AW_EEP_REV               },
};


/* DRIVES */
MAP_DRIVE_TO_NAME(                          "J1",                                   "J2",                                   "J3",                                   "J4",                                   "J5",                                   "J6"                                    );
MAP_DRIVE_TO_SLAVE(                         MAP_AW_J40LP_CTRL_1,                    MAP_AW_J40HP_CTRL_1,                    MAP_AW_J32_CTRL_1,                      MAP_AW_J25_CTRL_1,                      MAP_AW_J25_CTRL_2,                      MAP_AW_J20_CTRL_1                       );
MAP_DRIVE_TO_SUBDRIVE(                      1,                                      1,                                      1,                                      1,                                      1,                                      1                                       );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series,             ec_get_moo_pdo_aw_j_series              );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series,              ec_get_remote_aw_j_series               );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series,        ec_get_follow_error_aw_j_series         );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   ec_get_error_string_pdo_aw_j_series,    ec_get_error_string_pdo_aw_j_series,    ec_get_error_string_pdo_aw_j_series,    ec_get_error_string_pdo_aw_j_series,    ec_get_error_string_pdo_aw_j_series,    ec_get_error_string_pdo_aw_j_series     );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series,            ec_set_ctrl_wrd_aw_j_series             );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series,            ec_get_stat_wrd_aw_j_series             );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series,          ec_get_actpos_wrd_aw_j_series           );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series,          ec_set_setpos_wrd_aw_j_series           );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series,          ec_get_actvel_wrd_aw_j_series           );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series,         ec_get_acttorq_wrd_aw_j_series          );
MAP_DRIVE_GET_CONTROL_EFFORT_WRD_FUNCTIONS( ec_get_control_effort_wrd_aw_j_series,  ec_get_control_effort_wrd_aw_j_series,  ec_get_control_effort_wrd_aw_j_series,  ec_get_control_effort_wrd_aw_j_series,  ec_get_control_effort_wrd_aw_j_series,  ec_get_control_effort_wrd_aw_j_series   );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        ec_set_settorq_wrd_aw_j_series,         ec_set_settorq_wrd_aw_j_series,         ec_set_settorq_wrd_aw_j_series,         ec_set_settorq_wrd_aw_j_series,         ec_set_settorq_wrd_aw_j_series,         ec_set_settorq_wrd_aw_j_series,         );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  ec_set_settorqoffset_wrd_aw_j_series,   ec_set_settorqoffset_wrd_aw_j_series,   ec_set_settorqoffset_wrd_aw_j_series,   ec_set_settorqoffset_wrd_aw_j_series,   ec_set_settorqoffset_wrd_aw_j_series,  ec_set_settorqoffset_wrd_aw_j_series     );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   ec_set_setveloffset_wrd_aw_j_series,    ec_set_setveloffset_wrd_aw_j_series,    ec_set_setveloffset_wrd_aw_j_series,    ec_set_setveloffset_wrd_aw_j_series,    ec_set_setveloffset_wrd_aw_j_series,   ec_set_setveloffset_wrd_aw_j_series      );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series              );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_RUN_HOMING(                       0,                                      0,                                      0,                                      0,                                      0,                                      0                                       );
MAP_DRIVE_GET_SECONDARY_NAME_FUNCTION(      ec_get_secondary_name_aw_j_series,      ec_get_secondary_name_aw_j_series,      ec_get_secondary_name_aw_j_series,      ec_get_secondary_name_aw_j_series,      ec_get_secondary_name_aw_j_series,      ec_get_secondary_name_aw_j_series       );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           ec_print_params_aw_j_series,            ec_print_params_aw_j_series,            ec_print_params_aw_j_series,            ec_print_params_aw_j_series,            ec_print_params_aw_j_series,            ec_print_params_aw_j_series             );
MAP_DRIVE_TYPE(                             DRIVE_TYPE_AW_J_40_LP,                  DRIVE_TYPE_AW_J_40_HP,                  DRIVE_TYPE_AW_J32,                      DRIVE_TYPE_AW_J25,                      DRIVE_TYPE_AW_J25,                      DRIVE_TYPE_AW_J20                       );

/**
 * POS_LIMIT in degrees
 * VEL LIMIT is rad/sec
 * TORQUE_LIMIT in % of rated torque
 */

/* DRIVE PARAMETERS */
MAP_DRIVE_SCALES(                           {166886,9549,3.414},         {166886,9549,2.54},           {166886,9549,4.3},           {166886,9549,4.3},            {166886,9549,3.414},         {166886,9549,16.07}          );

/* PLC IO CONFIG */



/* Outputs
 * 0 - OUT_TOOL_1 - command to gripper -> map to GBC IO
 * 1 - OUT_TOOL_2 - command to gripper -> map to GBC IO
 * 2 - SPARE - not used -> map to GBC IO
 * 3 - SPARE - not used -> map to GBC IO
 * 4 - SPARE - not used -> map to GBC IO
 * 5 - SPARE - not used -> map to GBC IO
 * 6 - SPARE - not used -> map to GBC IO
 * 7 - SPARE - not used -> map to GBC IO
 */

/* Inputs
 * 0 - IN_TOOL_1 - feedback from gripper -> map to GBC IO
 * 1 - IN_TOOL_2 - feedback from gripper -> map to GBC IO
 * 2 - 48V_SUPPLY_OK  - 48v supply to the arm -> map to GBC IO
 * 3 - BRAKE_CHOPPER_ERROR - error from the brake chopper -> map to GBC IO
 * 4 - BRAKE_AX_RELEASED - brake is released -> map to GBC IO
 * 5 - SPARE - not used -> map to GBC IO
 * 6 - SPARE - not used -> map to GBC IO
 * 7 - SPARE - not used -> map to GBC IO
 */


/*IO MAP*/
mapping_t map_iomap[50] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=7},     {}},

        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=0, .type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=1, .type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=2, .type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=3, .type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=4, .type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=5,.type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=6,.type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=7,.type=GBC_IO_TYPE_NORMAL},{}},

        {{.inout=MAP_OUT, .slave_num=MAP_AW_J20_CTRL_1, .byte_num=35, .datatype=ECT_UNSIGNED32},              {.inout=MAP_OUT, .datatype=ECT_UNSIGNED32, .ionum=0}, {}  },


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

        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=13, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=14, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_1, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=15, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=16, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=17, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=18, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=19, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=20, .type = GBC_IO_TYPE_SAFETY},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_OUTPUTS_OFFSET_BYTE_2, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=21, .type = GBC_IO_TYPE_SAFETY},      {}},

    //bit 0 is used by gbem to set the error ack
//        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=0, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=1, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=2, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=3, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=4, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=5, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=6, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_0, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=7, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=8, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=9, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=10, .type = GBC_IO_TYPE_SAFETY},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_BBH_SCU_1_EC_1, .byte_num=BBH_SCU_1_EC_FUNCTIONAL_INPUTS_OFFSET_BYTE_1, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=11, .type = GBC_IO_TYPE_SAFETY},     {}}
        };


uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);



/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_aw_j_series,       ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series         );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_aw_j_series,       ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series         );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_aw_j_series,     ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series       );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_aw_j_series,    ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series      );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_aw_j_series,        ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series          );


//@formatter:on

#endif //MACHINE_AW_ROBOT_L2
