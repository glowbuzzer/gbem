/**
 ******************************************************************************
 * @file           :  machine_el6900_deadman.c
 * @brief          :  EL6900+EL1904+EL2904 with deadman safety PLC program
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
#if MACHINE_EL6900_DEADMAN == 1

#include "beckhoff.h"

//@formatter:off

/* SLAVES */
//                                          Slave 1                 Slave 2             Slave 3             Slave 4
//                                          MAP_EK1100_1            MAP_EL6900_1        MAP_EL1904_1        MAP_EL2904
//                                          Coupler                 Safety PLC          4 safe dig in       4 safe dig out
MAP_NUM_DRIVES_ATTACHED(                    0,                      0,                  0,                  0);
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(            NULL,                   NULL,               NULL,               NULL);
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(              NULL,                   NULL,               NULL,               NULL);
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(           NULL,                   NULL,               NULL,               NULL);
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(            NULL,                   NULL,               NULL,               NULL);
MAP_SLAVE_DC_TYPE(                          EC_DC_NONE,             EC_DC_NONE,         EC_DC_NONE,         EC_DC_0);
MAP_SLAVE_DC_CYCLE(                         0,                      0,                  0,                  4);
MAP_SLAVE_FSOE_MASTER(                      false,                  true,              false,              false);
MAP_SLAVE_FSOE_MASTER_TO_SLAVE_BYTES(       0,                      0,                  6,                  6);
MAP_SLAVE_FSOE_SLAVE_TO_MASTER_BYTES(       0,                      0,                  6,                  6);

MAP_FSOE_MASTER_CONTROL_FUNCTION(NULL);

/*This is a zero indexed array even though the slaves are 1 indexed */
/* This must be laid out in the order they appear in netscan */
const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES] = {
        {.name = EK1100_EEP_NAME, .eep_id = EK1100_EEP_ID, .eep_man = EK1100_EEP_MAN, .eep_rev = EK1100_EEP_REV},
        {.name = EL6900_EEP_NAME, .eep_id = EL6900_EEP_ID, .eep_man = EL6900_EEP_MAN, .eep_rev = EL6900_EEP_REV},
        {.name = EL1904_EEP_NAME, .eep_id = EL1904_EEP_ID, .eep_man = EL1904_EEP_MAN, .eep_rev = EL1904_EEP_REV},
        {.name = EL2904_EEP_NAME, .eep_id = EL2904_EEP_ID, .eep_man = EL2904_EEP_MAN, .eep_rev = EL2904_EEP_REV},
};



/* DRIVES */
MAP_DRIVE_TO_NAME();
MAP_DRIVE_TO_SLAVE();
MAP_DRIVE_TO_SUBDRIVE();
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS();
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS();
MAP_DRIVE_GET_REMOTE_FUNCTIONS();
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS();
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS();
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS();
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS();
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS();
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS();
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS();
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS();
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS();
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS();
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS();
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS();
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS();
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS();
MAP_DRIVE_HOMING_EXEC_FUNCTIONS();
MAP_DRIVE_RUN_HOMING();
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS();

/**
 * POS_LIMIT in degrees
 * VEL LIMIT is rad/sec
 * TORQUE_LIMIT in % of rated torque
 */

/* THESE ARE NOT USED ON THE L ROBOT CURRENTLY - SET DRIVE LIMITS IN OBLAC! NOT HERE */

/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT();
MAP_DRIVE_NEG_LIMIT();
MAP_DRIVE_DIRECTION();
MAP_DRIVE_TORQ_LIMIT();
MAP_DRIVE_VEL_LIMIT();
MAP_DRIVE_SCALES();

/* PLC IO CONFIG */



/*IO MAP*/
mapping_t map_iomap[16] = {
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},             {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6},      {}},
        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=7},      {}},

        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=0},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=1},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=2},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=3},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=4},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=5},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=6},     {}},
        {{.inout=MAP_OUT, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},            {.inout=MAP_OUT, .datatype=ECT_BOOLEAN, .ionum=7},     {}},
};

//mapping_t map_iomap[8] = {
//        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12, .bit_num=0, .datatype=ECT_BOOLEAN, .byte_slave=true},             {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=0},     {}},
//        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12,.bit_num=1, .datatype=ECT_BOOLEAN, .byte_slave=true},              { .inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=1},     {}},
//        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12,.bit_num=2, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=2},     {}},
//        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12,.bit_num=3, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=3},     {}},
//        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12,.bit_num=4, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=4},     {}},
//        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12,.bit_num=5, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=5},     {}},
//        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12,.bit_num=6, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=6},     {}},
//        {{.inout=MAP_IN, .slave_num=MAP_EL6900_1, .byte_num=12,.bit_num=7, .datatype=ECT_BOOLEAN, .byte_slave=true},              {.inout=MAP_IN, .datatype=ECT_BOOLEAN, .ionum=7},        {}},
//
//};


uint16_t map_num_rows_in_iomap = sizeof(map_iomap) / sizeof(map_iomap[0]);



/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS();
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS();
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS();
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS();
MAP_DRIVE_SET_MOO_REV_FUNCTIONS();


//@formatter:on




#endif //MACHINE_EL6900_DEADMAN
