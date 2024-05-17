/**
 ******************************************************************************
 * @file           :  machine_aw_6dof_virtual.c
 * @brief          :  machine map for aw tube robot 6DOF with J3 (zero indexed) virtual
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
#if MACHINE_AW_6DOF_VIRTUAL == 1

#include "automationware.h"
#include "virtual_drive.h"
//@formatter:off

/* MACHINE INFO */
MAP_MACHINE_GET_SAFETY_STATE_FUNCTION(          ec_get_safety_state_aw_j_series);

/* FSOE MASTER INFO */
MAP_FSOE_MASTER_SLOT_CONFIG(                    );
MAP_FSOE_MASTER_SLOT_TO_SLAVE(                  );
MAP_FSOE_MASTER_SET_ERROR_ACK_STATE_FUNCTION(   NULL);

/* ESTOP DIN INFO */
map_pdo_object_t ctrl_estop_din_1 = {};
map_pdo_object_t ctrl_estop_din_2 = {};

/* SLAVES */
//                                         Slave 1
//                                         MAP_AW_J25_CTRL_1
//                                         J25 joint
MAP_NUM_DRIVES_ATTACHED(                   1,                                   );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(           ec_pdo_map_aw_j_series,              );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(             NULL,                                );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(          ec_apply_standard_sdos_aw_j_series,  );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(           ec_initial_pdo_aw_j_series,          );
MAP_SLAVE_CUSTOM_FMMU_SM_FUNCTIONS(        NULL,                                );
MAP_SLAVE_DC_TYPE(                         EC_DC_0,                             );
MAP_SLAVE_DC_CYCLE(                        4,                                   );
MAP_SLAVE_EXEC_FUNCTIONS(                  NULL,                                );
MAP_SLAVE_OPTIONAL(                        false,                               );



/* FSoE */
MAP_SLAVE_FSOE_SLAVE_TYPE(                 FSOE_SLAVE_TYPE_NONE,       );
MAP_SLAVE_FSOE_SLAVE_FUNCTION(             FSOE_SLAVE_FUNCTION_NONE,   );
MAP_SLAVE_FSOE_OFFSET_IN(                  0,                          );
MAP_SLAVE_FSOE_OFFSET_OUT(                 0,                          );
MAP_SLAVE_FSOE_GET_SLAVE_STATE_FUNCTIONS(  NULL,                       );
MAP_SLAVE_FSOE_GET_SLAVE_CON_ID_FUNCTIONS( NULL,                       );
MAP_SLAVE_FSOE_GET_MASTER_STATE_FUNCTIONS( NULL,                       );
MAP_FSOE_MASTER_CONTROL_FUNCTION(NULL);







/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {
        {.name = AW_EEP_NAME,           .eep_id = AW_EEP_ID,            .eep_man = AW_EEP_MAN,          .eep_rev = AW_EEP_REV       },

};

//"VIRTUAL"
//0,
//1,
//NULL,
//ec_get_moo_pdo_virtual,
//ec_get_remote_virtual,
//ec_get_follow_error_virtual,
//ec_get_error_string_pdo_virtual,
//NULL
//ec_set_ctrl_wrd_virtual,
//ec_get_stat_wrd_virtual,
//ec_get_actpos_wrd_virtual,
//ec_get_actvel_wrd_virtual,
//ec_get_acttorq_wrd_virtual,
//ec_set_setpos_wrd_virtual,
//ec_set_setvel_wrd_virtual,
//ec_set_settorq_wrd_virtual,
//ec_set_settorqoffset_wrd_virtual
//        ec_set_setveloffset_wrd_virtual,
//NULL,
//0,
//NULL,



/* DRIVES */
MAP_DRIVE_TO_NAME(                          "VIRTUAL J1",                         "VIRTUAL J2",                        "VIRTUAL J3",                         "VIRTUAL J4",                             "VIRTUAL J5",                         "VIRTUAL J6",                      "VIRTUAL J6"                          );
MAP_DRIVE_TO_SLAVE(                         0,                                    0,                                   MAP_AW_J32_CTRL_1,                    0,                                        0,                                    0,                                 0                                     );
MAP_DRIVE_TO_SUBDRIVE(                      1,                                    1,                                   1,                                    1,                                        1,                                    1,                                 1                                     );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_virtual,               ec_get_moo_pdo_virtual,              ec_get_moo_pdo_aw_j_series,           ec_get_moo_pdo_virtual,                   ec_get_moo_pdo_virtual,               ec_get_moo_pdo_virtual,            ec_get_moo_pdo_virtual                );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL,                                 NULL,                                NULL,                                 NULL,                                     NULL,                                 NULL,                              NULL                                  );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_virtual,                ec_get_remote_virtual,               ec_get_remote_aw_j_series,            ec_get_remote_virtual,                    ec_get_remote_virtual,                ec_get_remote_virtual,             ec_get_remote_virtual                 );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_virtual,          ec_get_follow_error_virtual,         ec_get_follow_error_aw_j_series,      ec_get_follow_error_virtual,              ec_get_follow_error_virtual,          ec_get_follow_error_virtual,       ec_get_follow_error_virtual           );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   ec_get_error_string_pdo_virtual,      ec_get_error_string_pdo_virtual,     ec_get_error_string_pdo_aw_j_series,  ec_get_error_string_pdo_virtual,          ec_get_error_string_pdo_virtual,      ec_get_error_string_pdo_virtual,   ec_get_error_string_pdo_virtual       );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   NULL,                                 NULL,                                NULL,                                 NULL,                                     NULL,                                 NULL,                              NULL                                  );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_virtual,              ec_set_ctrl_wrd_virtual,             ec_set_ctrl_wrd_aw_j_series,          ec_set_ctrl_wrd_virtual,                  ec_set_ctrl_wrd_virtual,              ec_set_ctrl_wrd_virtual,           ec_set_ctrl_wrd_virtual               );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_virtual,              ec_get_stat_wrd_virtual,             ec_get_stat_wrd_aw_j_series,          ec_get_stat_wrd_virtual,                  ec_get_stat_wrd_virtual,              ec_get_stat_wrd_virtual,           ec_get_stat_wrd_virtual               );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_virtual,            ec_get_actpos_wrd_virtual,           ec_get_actpos_wrd_aw_j_series,        ec_get_actpos_wrd_virtual,                ec_get_actpos_wrd_virtual,            ec_get_actpos_wrd_virtual,         ec_get_actpos_wrd_virtual             );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_virtual,            ec_set_setpos_wrd_virtual,           ec_set_setpos_wrd_aw_j_series,        ec_set_setpos_wrd_virtual,                ec_set_setpos_wrd_virtual,            ec_set_setpos_wrd_virtual,         ec_set_setpos_wrd_virtual             );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         ec_get_actvel_wrd_virtual,            ec_get_actvel_wrd_virtual,           ec_get_actvel_wrd_aw_j_series,        ec_get_actvel_wrd_virtual,                ec_get_actvel_wrd_virtual,            ec_get_actvel_wrd_virtual,         ec_get_actvel_wrd_virtual             );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        ec_get_acttorq_wrd_virtual,           ec_get_acttorq_wrd_virtual,          ec_get_acttorq_wrd_aw_j_series,       ec_get_acttorq_wrd_virtual,               ec_get_acttorq_wrd_virtual,           ec_get_acttorq_wrd_virtual,        ec_get_acttorq_wrd_virtual            );
MAP_DRIVE_GET_CONTROL_EFFORT_WRD_FUNCTIONS( ec_get_control_effort_wrd_virtual,    ec_get_control_effort_wrd_virtual,   ec_get_control_effort_wrd_aw_j_series,ec_get_control_effort_wrd_virtual,        ec_get_control_effort_wrd_virtual,    ec_get_control_effort_wrd_virtual, ec_get_control_effort_wrd_virtual     );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         ec_set_setvel_wrd_virtual,            ec_set_setvel_wrd_virtual,           NULL,                                 ec_set_setvel_wrd_virtual,                ec_set_setvel_wrd_virtual,            ec_set_setvel_wrd_virtual,         ec_set_setvel_wrd_virtual             );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        ec_set_settorq_wrd_virtual,           ec_set_settorq_wrd_virtual,          ec_set_settorq_wrd_aw_j_series,       ec_set_settorq_wrd_virtual,               ec_set_settorq_wrd_virtual,           ec_set_settorq_wrd_virtual,        ec_set_settorq_wrd_virtual            );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  ec_set_settorqoffset_wrd_virtual,     ec_set_settorqoffset_wrd_virtual,    ec_set_settorqoffset_wrd_aw_j_series, ec_set_settorqoffset_wrd_virtual,         ec_set_settorqoffset_wrd_virtual,     ec_set_settorqoffset_wrd_virtual,  ec_set_settorqoffset_wrd_virtual      );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   ec_set_setveloffset_wrd_virtual,      ec_set_setveloffset_wrd_virtual,     ec_set_setveloffset_wrd_aw_j_series,  ec_set_setveloffset_wrd_virtual,          ec_set_setveloffset_wrd_virtual,      ec_set_setveloffset_wrd_virtual,   ec_set_setveloffset_wrd_virtual       );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            ec_set_moo_pdo_virtual,               ec_set_moo_pdo_virtual,              ec_set_moo_pdo_aw_j_series,           ec_set_moo_pdo_virtual,                   ec_set_moo_pdo_virtual,               ec_set_moo_pdo_virtual,            ec_set_moo_pdo_virtual                );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                                 NULL,                                NULL,                                 NULL,                                     NULL,                                 NULL,                              NULL                                  );
MAP_DRIVE_RUN_HOMING(                       0,                                    0,                                   0,                                    0,                                        0,                                    0,                                 0                                      );
MAP_DRIVE_GET_SECONDARY_NAME_FUNCTION(      NULL,                                 NULL,                                ec_get_secondary_name_aw_j_series,    NULL,                                     NULL,                                 NULL,                              NULL                                  );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL,                                 NULL,                                ec_print_params_aw_j_series,          NULL,                                     NULL,                                 NULL,                              NULL                                  );
MAP_DRIVE_TYPE(                             DRIVE_TYPE_VIRTUAL,                   DRIVE_TYPE_VIRTUAL,                  DRIVE_TYPE_AW_J32,                    DRIVE_TYPE_VIRTUAL,                       DRIVE_TYPE_VIRTUAL,                   DRIVE_TYPE_VIRTUAL,                DRIVE_TYPE_VIRTUAL                    );
MAP_DRIVE_GET_LOG_FILE_FUNCTIONS(           NULL,                                 NULL,                                ec_get_log_file_aw_j_series,          NULL,                                     NULL,                                 NULL,                              NULL                                  );

/**
 * POS_LIMIT in degrees
 * VEL LIMIT is rad/sec
 * TORQUE_LIMIT in % of rated torque
 */

/* DRIVE PARAMETERS */
MAP_DRIVE_DIRECTION(                       1                );
MAP_DRIVE_SCALES(                          {166886,9549,4.3});

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
mapping_t map_iomap[0] = {
        };


uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);



/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_aw_j_series,       ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series,        ec_get_ctrl_wrd_rev_aw_j_series         );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_aw_j_series,       ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series,        ec_set_stat_wrd_rev_aw_j_series         );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_aw_j_series,     ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series,      ec_set_actpos_wrd_rev_aw_j_series       );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_aw_j_series,    ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series,     ec_get_setpos_word_rev_aw_j_series      );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_aw_j_series,        ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series,         ec_set_moo_pdo_rev_aw_j_series          );


//@formatter:on

#endif //MACHINE_AW_6DOF_VIRTUAL
