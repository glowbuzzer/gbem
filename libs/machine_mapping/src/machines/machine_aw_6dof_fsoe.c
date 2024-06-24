/**
 ******************************************************************************
 * @file           :  machine_aw_robot_l2.c
 * @brief          :  machine map for aw l2 sized robot with EK1100 + El1808 + EL2908
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
#if MACHINE_AW_6DOF == 1

#include "automationware.h"
#include "beckhoff.h"
#include "bbh.h"
//@formatter:off

/* MACHINE INFO */
MAP_MACHINE_GET_SAFETY_STATE_FUNCTION(          ?);

/* FSOE MASTER INFO */
MAP_FSOE_MASTER_SLOT_CONFIG(MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12, MDP_SLOT_TYPE_BBH_32_12 );
MAP_FSOE_MASTER_SLOT_TO_SLAVE(MAP_AW_J40LP_CTRL_1, MAP_AW_J40HP_CTRL_1, MAP_AW_J32_CTRL_1, MAP_AW_J25_CTRL_1, MAP_AW_J25_CTRL_2, MAP_AW_J20_CTRL_1 );     );
MAP_FSOE_MASTER_SET_ERROR_ACK_STATE_FUNCTION(ec_fsoe_set_error_ack_state_bbh_scu_1_ec);

//todo chnaged to 0 and 1
/* ESTOP DIN INFO */
map_pdo_object_t ctrl_estop_din_1 = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_EL1808_1, .bit_num=0};
map_pdo_object_t ctrl_estop_din_2 = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_EL1808_1, .bit_num=1};

/* SLAVES */
//                                         Slave 1                     Slave 2                   Slave 3                   Slave 4                         Slave 5                              Slave 6                              Slave 7                              Slave 8                              Slave 9                              Slave 10
//                                         MAP_EK1100_1                MAP_EL2808_1              MAP_EL1808_1              MAP                             MAP_AW_J40LP_CTRL_1                  MAP_AW_J40LP_CTRL_1                  MAP_AW_J32_CTRL_1                    MAP_AW_J25_CTRL_1                    MAP_AW_J25_CTRL_2                    MAP_AW_J20_CTRL_1
//                                         Coupler                     8 dig out                 8 dig in                  RS485                           J40LP joint                          J40HP joint                          J32 joint                            J25 joint                            J25 joint                            J20 joint
MAP_NUM_DRIVES_ATTACHED(                   0,                          0,                        0,                        0,                              1,                                   1,                                   1,                                   1,                                   1,                                   1                                   );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(           NULL,                       NULL,                     NULL,                     NULL,                           ec_pdo_map_aw_j_series,              ec_pdo_map_aw_j_series,              ec_pdo_map_aw_j_series,              ec_pdo_map_aw_j_series,              ec_pdo_map_aw_j_series,              ec_pdo_map_aw_j_series              );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(             NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(          NULL,                       NULL,                     NULL,                     ec_apply_standard_sdos_el6021,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series  );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(           NULL,                       NULL,                     NULL,                     NULL,                           ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series          );
MAP_SLAVE_CUSTOM_FMMU_SM_FUNCTIONS(        NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
MAP_SLAVE_DC_TYPE(                         EC_DC_NONE,                 EC_DC_NONE,               EC_DC_NONE,               EC_DC_NONE,                     EC_DC_0,                             EC_DC_0,                             EC_DC_0,                             EC_DC_0,                             EC_DC_0,                             EC_DC_0                             );
MAP_SLAVE_DC_CYCLE(                        0,                          0,                        0,                        0,                              4,                                   4,                                   4,                                   4,                                   4,                                   4                                   );
MAP_SLAVE_EXEC_FUNCTIONS(                  NULL,                       NULL,                     NULL,                     ec_slave_exec_el6021,           NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL,                               );
MAP_SLAVE_OPTIONAL(                        false,                      false,                    false,                    true,                           false,                               false,                               false,                               false,                               false,                               false, );



/* FSoE */
MAP_SLAVE_FSOE_SLAVE_TYPE(                 FSOE_SLAVE_TYPE_SCU_1_EC,   FSOE_SLAVE_TYPE_NONE,     FSOE_SLAVE_TYPE_NONE,     FSOE_SLAVE_TYPE_NONE,           FSOE_SLAVE_TYPE_SYNAPTICON,          FSOE_SLAVE_TYPE_SYNAPTICON,          FSOE_SLAVE_TYPE_SYNAPTICON,          FSOE_SLAVE_TYPE_SYNAPTICON,          FSOE_SLAVE_TYPE_SYNAPTICON,          FSOE_SLAVE_TYPE_SYNAPTICON          );
MAP_SLAVE_FSOE_SLAVE_FUNCTION(             FSOE_SLAVE_FUNCTION_MASTER, FSOE_SLAVE_FUNCTION_NONE, FSOE_SLAVE_FUNCTION_NONE, FSOE_SLAVE_FUNCTION_NONE,       FSOE_SLAVE_FUNCTION_SLAVE,            FSOE_SLAVE_FUNCTION_SLAVE,            FSOE_SLAVE_FUNCTION_SLAVE,            FSOE_SLAVE_FUNCTION_SLAVE,            FSOE_SLAVE_FUNCTION_SLAVE,            FSOE_SLAVE_FUNCTION_SLAVE            );
MAP_SLAVE_FSOE_OFFSET_IN(                  0,                          0,                        0,                        0,                              0,                                   0,                                   0,                                   0,                                   0,                                   0                                   );
MAP_SLAVE_FSOE_OFFSET_OUT(                 0,                          0,                        0,                        0,                              0,                                   0,                                   0,                                   0,                                   0,                                   0                                   );
MAP_SLAVE_FSOE_GET_SLAVE_STATE_FUNCTIONS(  NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
MAP_SLAVE_FSOE_GET_SLAVE_CON_ID_FUNCTIONS( NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
MAP_SLAVE_FSOE_GET_MASTER_STATE_FUNCTIONS( NULL,                       NULL,                     NULL,                     NULL,                           NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
MAP_FSOE_MASTER_CONTROL_FUNCTION(          ec_fsoe_get_master_state_bbh_scu_1_ec);



// /* SLAVES */
// //                                         Slave 1                     Slave 2                   Slave 3                   Slave 4                              Slave 5                              Slave 6                              Slave 7                              Slave 8                              Slave 9
// //                                         MAP_EK1100_1                MAP_EL2808_1              MAP_EL1808_1              MAP_AW_J40LP_CTRL_1                  MAP_AW_J40LP_CTRL_1                  MAP_AW_J32_CTRL_1                    MAP_AW_J25_CTRL_1                    MAP_AW_J25_CTRL_2                    MAP_AW_J20_CTRL_1
// //                                         Coupler                     8 dig out                 8 dig in                  J40LP joint                          J40HP joint                          J32 joint                            J25 joint                            J25 joint                            J20 joint
// MAP_NUM_DRIVES_ATTACHED(                   0,                          0,                        0,                        1,                                   1,                                   1,                                   1,                                   1,                                   1                                   );
// MAP_SLAVE_PDO_MAPPING_FUNCTIONS(           NULL,                       NULL,                     NULL,                     NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                ec_pdo_map_aw_j_series              );
// MAP_SLAVE_NVRAM_SDO_FUNCTIONS(             NULL,                       NULL,                     NULL,                     NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
// MAP_SLAVE_STANDARD_SDO_FUNCTIONS(          NULL,                       NULL,                     NULL,                     ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series,  ec_apply_standard_sdos_aw_j_series  );
// MAP_SLAVE_INITIAL_PDO_FUNCTIONS(           NULL,                       NULL,                     NULL,                     ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series,          ec_initial_pdo_aw_j_series          );
// MAP_SLAVE_CUSTOM_FMMU_SM_FUNCTIONS(        NULL,                       NULL,                     NULL,                     NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
// MAP_SLAVE_DC_TYPE(                         EC_DC_NONE,                 EC_DC_NONE,               EC_DC_NONE,               EC_DC_0,                             EC_DC_0,                             EC_DC_0,                             EC_DC_0,                             EC_DC_0,                             EC_DC_0                             );
// MAP_SLAVE_DC_CYCLE(                        0,                          0,                        0,                        4,                                   4,                                   4,                                   4,                                   4,                                   4                                   );
// MAP_SLAVE_EXEC_FUNCTIONS(                  NULL,                       NULL,                     NULL,                     NULL,                                NULL,                                NULL,                                NULL,                                NULL                                 NULL,                               );
//
//
//
// /* FSoE */
// MAP_SLAVE_FSOE_SLAVE_TYPE(                 FSOE_SLAVE_TYPE_NONE,       FSOE_SLAVE_TYPE_NONE,     FSOE_SLAVE_TYPE_NONE,     FSOE_SLAVE_TYPE_NONE,                FSOE_SLAVE_TYPE_NONE,                FSOE_SLAVE_TYPE_NONE,                FSOE_SLAVE_TYPE_NONE,                FSOE_SLAVE_TYPE_NONE,                FSOE_SLAVE_TYPE_NONE                );
// MAP_SLAVE_FSOE_SLAVE_FUNCTION(             FSOE_SLAVE_FUNCTION_NONE,   FSOE_SLAVE_FUNCTION_NONE, FSOE_SLAVE_FUNCTION_NONE, FSOE_SLAVE_FUNCTION_NONE,            FSOE_SLAVE_FUNCTION_NONE,            FSOE_SLAVE_FUNCTION_NONE,            FSOE_SLAVE_FUNCTION_NONE,            FSOE_SLAVE_FUNCTION_NONE,            FSOE_SLAVE_FUNCTION_NONE            );
// MAP_SLAVE_FSOE_OFFSET_IN(                  0,                          0,                        0,                        0,                                   0,                                   0,                                   0,                                   0,                                   0                                   );
// MAP_SLAVE_FSOE_OFFSET_OUT(                 0,                          0,                        0,                        0,                                   0,                                   0,                                   0,                                   0,                                   0                                   );
// MAP_SLAVE_FSOE_GET_SLAVE_STATE_FUNCTIONS(  NULL,                       NULL,                     NULL,                     NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
// MAP_SLAVE_FSOE_GET_SLAVE_CON_ID_FUNCTIONS( NULL,                       NULL,                     NULL,                     NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
// MAP_SLAVE_FSOE_GET_MASTER_STATE_FUNCTIONS( NULL,                       NULL,                     NULL,                     NULL,                                NULL,                                NULL,                                NULL,                                NULL,                                NULL                                );
// MAP_FSOE_MASTER_CONTROL_FUNCTION(NULL);




/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {
        {.name = EK1100_EEP_NAME,       .eep_id = EK1100_EEP_ID,        .eep_man = EK1100_EEP_MAN,      .eep_rev = EK1100_EEP_REV   },
        {.name = EL2808_EEP_NAME,       .eep_id = EL2808_EEP_ID,        .eep_man = EL2808_EEP_MAN,      .eep_rev = EL2808_EEP_REV   },
        {.name = EL1808_EEP_NAME,       .eep_id = EL1808_EEP_ID,        .eep_man = EL1808_EEP_MAN,      .eep_rev = EL1808_EEP_REV   },
        {.name = EL6021_EEP_NAME,       .eep_id = EL6021_EEP_ID,        .eep_man = EL6021_EEP_MAN,      .eep_rev = EL6021_EEP_REV   },
       {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,          .eep_rev = AW_EEP_REV       },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,          .eep_rev = AW_EEP_REV       },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,          .eep_rev = AW_EEP_REV       },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,          .eep_rev = AW_EEP_REV       },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,          .eep_rev = AW_EEP_REV       },
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,          .eep_rev = AW_EEP_REV       },
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
MAP_DRIVE_GET_LOG_FILE_FUNCTIONS(           ec_get_log_file_aw_j_series,            ec_get_log_file_aw_j_series,            ec_get_log_file_aw_j_series,            ec_get_log_file_aw_j_series,            ec_get_log_file_aw_j_series,            ec_get_log_file_aw_j_series             );

/**
 * POS_LIMIT in degrees
 * VEL LIMIT is rad/sec
 * TORQUE_LIMIT in % of rated torque
 */

/* DRIVE PARAMETERS */
MAP_DRIVE_DIRECTION(                        1,                                      1,                                      1,                                      1,                                      1,                                      1                                       );
MAP_DRIVE_SCALES(                           {166886,9549,3.414},         {166886,9549,2.54},           {166886,9549,4.3},           {166886,9549,4.3},            {166886,9549,3.414},         {166886,9549,16.07}          );

/* PLC IO CONFIG */

extern bool plc_in_1_SS1_CMD_SW;
extern bool plc_in_2_STOP_CMD_SW;
extern bool plc_in_3_ARM_48V_SUPPLY;
extern bool plc_in_4_RC_LIGHT_SIGNAL;
extern bool plc_in_5_BRAKE_CHOPPER_ERROR;
extern bool plc_in_6_IN_TOOL_1;
extern bool plc_in_7_IN_TOOL_2;
/*
 * Inputs
 * 1 SS1_CMD_SW - this triggers the quick stop
 * 2 STOP_CMD_SW - this triggers the quick stop too
 * 3 ARM_48V_SUPPLY
 * 4 RC_LIGHT_SIGNAL
 * 5 BRAKE_CHOPPER_ERROR
 * 6 IN_TOOL_1 - feedback from gripper
 * 7 IN_TOOL_2 - feedback from gripper
 */

extern bool plc_out_1_SW_HEARTBYTE_CH1;
extern bool plc_out_2_SW_HEARTBYTE_CH2;
extern bool plc_out_3_SS1_CMD_SW_FB;
extern bool plc_out_4_STOP_CMD_SW_FB;
extern bool plc_out_5_SPARE;
extern bool plc_out_6_OUT_TOOL_1;
extern bool plc_out_7_OUT_TOOL_2;

/*
 * Outputs
 * 1 SW_HEARTBYTE_CH1 - must be high to inform safety PLC that EEMC is working
 * 2 SW_HEARTBYTE_CH2 - must be high to inform safety PLC that EEMC is working
 * 3 SS1_CMD_SW_FB - must mirror the signal on SS1_CMD_SW
 * 4 STOP_CMD_SW_FB - must mirror the signal on STOP_CMD_SW
 * 5 SPARE
 * 6 OUT_TOOL_1 - command to gripper -> map to GBC IO
 * 7 OUT_TOOL_2 - command to gripper -> map to GBC IO
 */


/*IO MAP*/
mapping_t map_iomap[17] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_1_SS1_CMD_SW, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_2_STOP_CMD_SW, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_3_ARM_48V_SUPPLY, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_4_RC_LIGHT_SIGNAL, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_5_BRAKE_CHOPPER_ERROR, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6},     {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave = false},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=7},     {}},

        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave = false},    {},                                                    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_1_SW_HEARTBYTE_CH1, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave = false},    {},                                                    {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_2_SW_HEARTBYTE_CH2, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave = false},    {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_3_SS1_CMD_SW_FB, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave = false},    {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_4_STOP_CMD_SW_FB, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=0, .type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=1,.type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=2,.type=GBC_IO_TYPE_NORMAL},{}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave = false},    {.inout = MAP_OUT, .datatype = ECT_BOOLEAN, .ionum=3,.type=GBC_IO_TYPE_NORMAL},{}},

        {{.inout=MAP_OUT, .slave_num=MAP_AW_J20_CTRL_1, .byte_num=35, .datatype=ECT_UNSIGNED32},              {.inout=MAP_OUT, .datatype=ECT_UNSIGNED32, .ionum=0}, {}                                                                                                }
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
