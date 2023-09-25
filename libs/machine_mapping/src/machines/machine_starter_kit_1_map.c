
/**
 ******************************************************************************
 * @file           :  machine_starter_kit_1_map.c
 * @brief          :  machine map for starter kit 1 (single AZD-KED)
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "map.h"

#if MACHINE_STARTER_KIT_1 == 1

#include "azd-ked.h"



//@formatter:off
/* SLAVES */
//                                          Slave 1
//                                          MAP_AZD_1
//                                          1 axis stepper
MAP_NUM_DRIVES_ATTACHED(                    1                                       );
MAP_SLAVE_PDO_MAPPING_FUNCTIONS(            ec_pdo_map_azd_ked                      );
MAP_SLAVE_NVRAM_SDO_FUNCTIONS(              ec_nvram_sdos_azd_ked                   );
MAP_SLAVE_STANDARD_SDO_FUNCTIONS(           NULL                                    );
MAP_SLAVE_INITIAL_PDO_FUNCTIONS(            ec_initial_pdo_azd_ked                  );
MAP_SLAVE_DC_TYPE(                          EC_DC_0                                 );
MAP_SLAVE_DC_CYCLE(                         4                                       );

/* DRIVES */
MAP_DRIVE_TO_NAME(                          "AZD1"                                  );
MAP_DRIVE_TO_SLAVE(                         MAP_AZD_1                               );
MAP_DRIVE_TO_SUBDRIVE(                      1                                       );
MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(            ec_get_moo_pdo_azd_ked                  );
MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(            NULL                                    );
MAP_DRIVE_GET_REMOTE_FUNCTIONS(             ec_get_remote_azd_ked                   );
MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(       ec_get_follow_error_azd_ked             );
MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(   ec_get_error_string_pdo_azd_ked         );
MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(   NULL,                                   );
MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(           ec_set_ctrl_wrd_azd_ked,                );
MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(           ec_get_stat_wrd_azd_ked,                );
MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(         ec_get_actpos_wrd_azd_ked,              );
MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(         ec_set_setpos_wrd_azd_ked,              );
MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(         NULL                                    );
MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(        NULL                                    );
MAP_DRIVE_SET_SETVEL_WRD_FUNCTIONS(         NULL                                    );
MAP_DRIVE_SET_SETTORQ_WRD_FUNCTIONS(        NULL                                    );
MAP_DRIVE_MOO(                              CIA_MOO_CSP                             );
MAP_DRIVE_HOMING_EXEC_FUNCTIONS(            NULL                                    );
MAP_DRIVE_RUN_HOMING(                       0                                       );
MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(           NULL                                    );


/* DRIVE PARAMETERS */
MAP_DRIVE_POS_LIMIT(                        999,                                    );
MAP_DRIVE_NEG_LIMIT(                        -999,                                   );
MAP_DRIVE_DIRECTION(                        1,                                      );

/*IO MAP */
mapping_t map_iomap[0] = {
};
uint16_t map_num_rows_in_iomap = sizeof (map_iomap)/ sizeof(map_iomap[0]);

/*This is a zero indexed array even though the slaves are 1 indexed
 * This must be laid out in the order they appear in netscan
 * It is used to check the slaves match in the boot process */
const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES] = {
        {
                .name = AZD_KED_EEP_NAME,
                .eep_id = AZD_KED_EEP_ID,
                .eep_man = AZD_KED_EEP_MAN,
                .eep_rev = AZD_KED_EEP_REV }, };



/* REVERSE FUNCTIONS - USED FOR UNIT TESTING */
MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(       ec_get_ctrl_wrd_rev_azd_ked     );
MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(       ec_set_stat_wrd_rev_azd_ked     );
MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(     ec_set_actpos_wrd_rev_azd_ked   );
MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(     ec_get_setpos_word_rev_azd_ked  );
MAP_DRIVE_SET_MOO_REV_FUNCTIONS(            ec_set_moo_pdo_rev_azd_ked      );

#endif

