
/**
 ******************************************************************************
 * @file           :  machine_single_akd.c
 * @brief          :  machine map for single AKD drive
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */





#include "map.h"


#if MACHINE_SINGLE_AKD == 1
#include "kollmorgen.h"


//@formatter:off

/* SLAVES */
//                                          Slave 1
//                                          MAP_AKD_1
//                                          single servor
MAP_NUM_DRIVES_ATTACHED(                    2                                   );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(            ec_pdo_map_akd                      );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(              ec_nvram_sdos_akd                   );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(           ec_standard_sdos_akd                );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(            NULL                                );
MAP_SLAVE_DC_TYPE(                          EC_DC_0                             );
MAP_SLAVE_DC_CYCLE(                         4                                   );


/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AKD1"                              );
MAP_DRIVE_TO_SLAVE(                         MAP_AKD_1                           );
MAP_DRIVE_TO_SUBDRIVE(                      1                                   );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            NULL                                );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            ec_get_moo_sdo_akd                  );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_akd                   );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_akd,            );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   NULL                                );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   ec_get_error_string_sdo_akd         );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_akd                 );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_akd                 );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_akd               );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_akd               );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL                                );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL                                );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL,                               );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL,                               );
MAP_DRIVE_SET_SETTORQOFFSET_WRD_FUNCTIONS(  NULL,                               );
MAP_DRIVE_SET_SETVELOFFSET_WRD_FUNCTIONS(   NULL,                               );
MAP_DRIVE_MOO_SET_PDO_FUNCTIONS(            NULL,                               );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL,                               );
MAP_DRIVE_RUN_HOMING(                       0,                                  );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL                                );


/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        999,                                );
MAP_DRIVE_NEG_LIMIT(                        -999,                               );
MAP_DRIVE_TORQ_LIMIT(                       0,                              );
MAP_DRIVE_VEL_LIMIT(                        0,                              );
MAP_DRIVE_SCALES(                           {0},                            );

/*IO MAP*/
mapping_t map_iomap[0] = {

};
uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

//This is a zero indexed array even though the slaves are 1 indexed
/* This must be laid out in the order they appear in the netscan */
const map_slave_map_t ecm_slave_map[EC_MAXSLAVE] = {

        {
                .name = AKD_EEP_NAME,
                .eep_id = AKD_EEP_ID,
                .eep_man = AKD_EEP_MAN,
                .eep_rev = AKD_EEP_REV},};


/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_akd             );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_akd             );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_akd           );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_akd          );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(         NULL              );

#endif


