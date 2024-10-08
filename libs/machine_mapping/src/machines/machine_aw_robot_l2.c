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
#if MACHINE_AW_ROBOT_L2 == 1

#include "automationware.h"
#include "beckhoff.h"

//This config is for "L2" -  20kg payload
//J40LP -> J40HP -> J32 ->J25 -> J25 ->J20 - 1.4m

/* ESTOP (QUICK STOP) CONFIG */

//@formatter:off
map_pdo_object_t ctrl_estop_din_1 = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_EL1808_1, .bit_num=1};
map_pdo_object_t ctrl_estop_din_2 = {.datatype=ECT_BOOLEAN, .inout=MAP_IN, .slave_num=MAP_EL1808_1, .bit_num=2};

/* SLAVES */
//                                        Slave 1             Slave 2         Slave 3          Slave 4                           Slave 5                           Slave 6                         Slave 7                         Slave 8                         Slave 9
//                                        MAP_EK1100_1        MAP_EL2808_1    MAP_EL1808_1     MAP_AW_J40LP_CTRL_1               MAP_AW_J40LP_CTRL_1               MAP_AW_J32_CTRL_1               MAP_AW_J25_CTRL_1               MAP_AW_J25_CTRL_2               MAP_AW_J20_CTRL_1
//                                        Coupler             8 dig out       8 dig in         J40LP joint                       J40HP joint                       J32 joint                       J25 joint                       J25 joint                       J20 joint
MAP_NUM_DRIVES_ATTACHED(                  0,                  0,              0,               1,                                1,                                1,                              1,                              1,                              1                                   );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(          NULL,               NULL,           NULL,            NULL,                             NULL,                             NULL,                           NULL,                           NULL,                           ec_pdo_map_aw_j_series              );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(            NULL,               NULL,           NULL,            NULL,                             NULL,                             NULL,                           NULL,                           NULL,                           NULL                                );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(         NULL,               NULL,           NULL,            ec_apply_standard_sdos_aw_j40_lp, ec_apply_standard_sdos_aw_j40_hp, ec_apply_standard_sdos_aw_j32,  ec_apply_standard_sdos_aw_j25,  ec_apply_standard_sdos_aw_j25,  ec_apply_standard_sdos_aw_j20       );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(          NULL,               NULL,           NULL,            ec_initial_pdo_aw_j_series,       ec_initial_pdo_aw_j_series,       ec_initial_pdo_aw_j_series,     ec_initial_pdo_aw_j_series,     ec_initial_pdo_aw_j_series,     ec_initial_pdo_aw_j_series          );
MAP_SLAVE_DC_TYPE(                        EC_DC_NONE,         EC_DC_NONE,     EC_DC_NONE,      EC_DC_0,                          EC_DC_0,                          EC_DC_0,                        EC_DC_0,                        EC_DC_0,                        EC_DC_0                             );
MAP_SLAVE_DC_CYCLE(                       0,                  0,              0,               4,                                4,                                4,                              4,                              4,                              4                                   );

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {
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


/* DRIVES */
MAP_DRIVE_TO_NAME(                          "J1",                                   "J2",                                   "J3",                                   "J4",                                   "J5",                                   "J6"                                    );
MAP_DRIVE_TO_SLAVE(                         MAP_AW_J40LP_CTRL_1,                    MAP_AW_J40HP_CTRL_1,                    MAP_AW_J32_CTRL_1,                      MAP_AW_J25_CTRL_1,                      MAP_AW_J25_CTRL_2,                      MAP_AW_J20_CTRL_1                       );
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
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  ec_set_settorqoffset_wrd_aw_j_series,   ec_set_settorqoffset_wrd_aw_j_series,   ec_set_settorqoffset_wrd_aw_j_series,   ec_set_settorqoffset_wrd_aw_j_series,   ec_set_settorqoffset_wrd_aw_j_series,  ec_set_settorqoffset_wrd_aw_j_series    );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   ec_set_setveloffset_wrd_aw_j_series,    ec_set_setveloffset_wrd_aw_j_series,    ec_set_setveloffset_wrd_aw_j_series,    ec_set_setveloffset_wrd_aw_j_series,    ec_set_setveloffset_wrd_aw_j_series,   ec_set_setveloffset_wrd_aw_j_series     );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series,             ec_set_moo_pdo_aw_j_series              );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL,                                   NULL                                    );
MAP_DRIVE_RUN_HOMING(                       0,                                      0,                                      0,                                      0,                                      0,                                      0                                       );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           ec_print_params_aw_j_series,            ec_print_params_aw_j_series,            ec_print_params_aw_j_series,            ec_print_params_aw_j_series,            ec_print_params_aw_j_series,            ec_print_params_aw_j_series             );

/**
 * POS_LIMIT in degrees
 * VEL LIMIT is rad/sec
 * TORQUE_LIMIT in % of rated torque
 */

/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        90,                                     55,                                     135,                                     180,                                   -5,                                     300                                    );
MAP_DRIVE_NEG_LIMIT(                        -90,                                    -55,                                    45,                                     -180,                                   -175,                                   -300                                    );
MAP_DRIVE_TORQ_LIMIT(                       60,                                     60,                                     60,                                     60,                                     60,                                     60,                                     );
MAP_DRIVE_VEL_LIMIT(                        40,                                     40,                                     40,                                     40,                                     40,                                     40,                                     );
MAP_DRIVE_SCALES(                           {166886,9549,3.414},                    {166886,9549,2.54},                    {166886,9549,4.3},                      {166886,9549,4.3},                       {166886,9549,3.414},                    {166886,9549,16.07}                     );

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
 * 6 OUT_TOOL_1 - command to gripper
 * 7 OUT_TOOL_2 - command to gripper
 */


/*IO MAP*/
mapping_t map_iomap[15] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=1, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_1_SS1_CMD_SW, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=2, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_2_STOP_CMD_SW, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=3, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_3_ARM_48V_SUPPLY, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=4, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_4_RC_LIGHT_SIGNAL, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=5, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_5_BRAKE_CHOPPER_ERROR, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=6, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_6_IN_TOOL_1, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_IN, .slave_num=MAP_EL1808_1,.bit_num=7, .datatype=ECT_BOOLEAN},                           {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6},     {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .io=&plc_in_7_IN_TOOL_2, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=1, .datatype=ECT_BOOLEAN},                          {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_1_SW_HEARTBYTE_CH1, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=2, .datatype=ECT_BOOLEAN},                          {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_2_SW_HEARTBYTE_CH2, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=3, .datatype=ECT_BOOLEAN},                          {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_3_SS1_CMD_SW_FB, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=4, .datatype=ECT_BOOLEAN},                         {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_4_STOP_CMD_SW_FB, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=5, .datatype=ECT_BOOLEAN},                         {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_5_SPARE, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=6, .datatype=ECT_BOOLEAN},                         {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_6_OUT_TOOL_1, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL2808_1,.bit_num=7, .datatype=ECT_BOOLEAN},                         {},                                                   {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .io=&plc_out_7_OUT_TOOL_2, .linked_task_name="AwRobotTask1"}},
        {{.inout=MAP_OUT, .slave_num=MAP_AW_J20_CTRL_1, .byte_num=35, .datatype=ECT_UNSIGNED32},              {.inout=MAP_OUT, .datatype=ECT_UNSIGNED32, .ionum=0}, {}                                                                                                  }
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
