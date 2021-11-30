/**
 ******************************************************************************
 * @file           :  my_drive.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_MY_DRIVE_H
#define GB_MONOREPO_MY_DRIVE_H

#include "my_drive.h"
#include "map.h"
#include "gberror.h"


map_SM_assignment_object_t map_SM2_mydrive;
map_SM_assignment_object_t map_SM3_mydrive;
uint16_t map_SM2_index_of_assigned_PDO_mydrive[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_mydrive[ECM_MAX_PDO_MAPPING_ENTRIES];


/** Slave functions */
gberror_t ec_initial_pdo_mydrive(uint16_t slave);
gberror_t ec_apply_standard_sdos_mydrive(uint16_t slave);
gberror_t ec_pdo_map_mydrive(uint16_t slave);
int8_t ec_get_moo_pdo_mydrive(uint16_t drive);
uint8_t * ec_get_error_string_pdo_mydrive(uint16_t drive);
gberror_t ec_nvram_sdos_mydrive(uint16_t slave);
gberror_t ec_write_nvram_mydrive(uint16_t slave);

/** Drive functions */
gberror_t ec_set_ctrl_wrd_mydrive(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_mydrive(uint16_t drive);
int32_t ec_get_actpos_wrd_mydrive(uint16_t drive);
gberror_t ec_set_setpos_wrd_mydrive(uint16_t drive, int32_t setpos);
bool ec_get_remote_mydrive(uint16_t drive);
bool ec_get_follow_error_mydrive(uint16_t drive);


#define MYDRIVE_MAN 0x1a
#define MYDRIVE_EEP_NAME "MY DRIVE NAME"
#define MYDRIVE_EEP_MAN MYDRIVE_MAN
#define MYDRIVE_EEP_REV 0x1
#define MYDRIVE_EEP_ID 0x1

#define NUM_OF_MYDRIVE_ERROR_STRINGS 42





/* These define the position of objects in the PDO - number of bytes from start of input or output */
//need to decide on these
//static const uint16_t AZD_SETPOS_DRIVE_1_INDEX = 2;

#define MYDRIVE_SETPOS_PDO_INDEX        2

#define MYDRIVE_ACTPOS_PDO_INDEX        2

#define MYDRIVE_CONTROLWORD_PDO_INDEX   0

#define MYDRIVE_STATUSWORD_PDO_INDEX    0


#define MYDRIVE_MOOSET_PDO_INDEX        6


#define MYDRIVE_MOODISP_PDO_INDEX       6

#define MYDRIVE_ERROR_CODE_PDO_INDEX    17


#define MYDRIVE_MIN_LIMIT_SDO_INDEX     (0x607d)
#define MYDRIVE_MIN_LIMIT_SDO_SUB_INDEX (0x1)

#define MYDRIVE_MAX_LIMIT_SDO_INDEX     (0x607d)
#define MYDRIVE_MAX_LIMIT_SDO_SUB_INDEX (0x2)


#define MYDRIVE_DIRECTION_SDO_INDEX       0x41C2
#define MYDRIVE_DIRECTION_SDO_SUB_INDEX   0x1


#define MYDRIVE_WRITE_CONFIG_SDO_INDEX      0x40C6
#define MYDRIVE_WRITE_CONFIG_SDO_SUB_INDEX  0x1
#define MYDRIVE_WRITE_CONFIG_SDO_VALUE      0x1


#endif //GB_MONOREPO_MY_DRIVE_H
