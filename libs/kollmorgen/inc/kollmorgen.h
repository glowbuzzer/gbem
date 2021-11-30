/**
 ******************************************************************************
 * @file           :  kollmorgen.h
 * @brief          :  kollmorgen drive functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef MONOREPO_KOLLMORGEN_H
#define MONOREPO_KOLLMORGEN_H

#include "map.h"

map_SM_assignment_object_t map_SM2_akd;
map_SM_assignment_object_t map_SM3_akd;
uint16_t map_SM2_index_of_assigned_PDO_akd[ECM_MAX_PDO_MAPPING_ENTRIES];
uint16_t map_SM3_index_of_assigned_PDO_akd[ECM_MAX_PDO_MAPPING_ENTRIES];


bool ec_get_remote_akd(uint16_t drive);
gberror_t ec_standard_sdos_akd(uint16_t slave);
gberror_t ec_pdo_map_akd(uint16_t slave);
int8_t ec_get_moo_sdo_akd(uint16_t drive);
uint8_t * ec_get_error_string_sdo_akd(uint16_t drive);
gberror_t ec_write_nvram_akd(uint16_t slave);
gberror_t ec_nvram_sdos_akd(uint16_t slave);


gberror_t ec_set_ctrl_wrd_akd(uint16_t drive, uint16_t ctrlwrd);
uint16_t ec_get_stat_wrd_akd(uint16_t drive);
uint16_t ec_get_ctrl_wrd_rev_akd(uint16_t drive);
gberror_t ec_set_stat_wrd_rev_akd(uint16_t drive, uint16_t statwrd);
gberror_t ec_set_actpos_wrd_rev_akd(uint16_t drive, int32_t actpos);
int32_t ec_get_setpos_word_rev_akd(uint16_t drive);
int32_t ec_get_actpos_wrd_akd(uint16_t drive);
gberror_t ec_set_setpos_wrd_akd(uint16_t drive, int32_t setpos);
bool ec_get_follow_error_akd(uint16_t drive);

bool print_akd_sync_params(const uint16_t slave);

#define KOLLMORGEN_MAN 0x0000006A
#define AKD_EEP_NAME "AKD"
#define AKD_EEP_MAN KOLLMORGEN_MAN
#define AKD_EEP_REV 0x00000002
#define AKD_EEP_ID 0x00414B44



#define AKD_SETPOS_PDO_INDEX                    0 //4 bytes
#define AKD_CONTROLWORD_PDO_INDEX               4 //2 bytes

#define AKD_STATUSWORD_PDO_INDEX                0 //2bytes
#define AKD_ACTPOS_PDO_INDEX                    2 //4 bytes
#define AKD_ACTTORQ_PDO_INDEX                   6 //2 bytes
#define AKD_FOLLOWERROR_ACTVAL_PDO_INDEX        14 //4bytes


/* AKD drive extended statusword bit numbers */
#define AKD_STATUSWORD_STO_BIT_NUM      	    8

#define AKD_FOLLOW_ERROR_TOLLERANCE             10000

#define AKD_MIN_LIMIT_SDO_INDEX     (0x607d)
#define AKD_MIN_LIMIT_SDO_SUB_INDEX (0x1)

#define AKD_MAX_LIMIT_SDO_INDEX     (0x607d)
#define AKD_MAX_LIMIT_SDO_SUB_INDEX (0x2)

#define AKD_WRITE_CONFIG_SDO_INDEX      0x1010
#define AKD_WRITE_CONFIG_SDO_SUB_INDEX  0x1
#define AKD_WRITE_CONFIG_SDO_VALUE      (0x65766173)


#define AKD_DIRECTION_SDO_INDEX       0x352a
#define AKD_DIRECTION_SDO_SUB_INDEX   0x0


#define AKD_ERROR_CODE_SDO_INDEX 0x2001
#define AKD_WARNING_CODE_SDO_INDEX 0x2000


//interpolation time index
#define AKD_INTERPOLATION_TIME_SDO_INDEX (0x60C2)
#define AKD_INTERPOLATION_TIME_SDO_SUB_INDEX (2)
#define AKD_INTERPOLATION_TIME_SDO_VALUE (-3)

//interpolation time units

#define AKD_INTERPOLATION_TIME_PERIOD_SDO_INDEX (0x60C2)
#define AKD_INTERPOLATION_TIME_PERIOD_SDO_SUB_INDEX (1)

//moo

#define AKD_MOO_SET_SDO_INDEX (0x6060)
#define AKD_MOO_SET_SDO_SUB_INDEX (0)

#define AKD_MOO_GET_SDO_INDEX (0x6061)
#define AKD_MOO_GET_SDO_SUB_INDEX (0)


/*
 * The cycle time to be used in the drive for the cyclical setpoints and actual values can either
be stored in the FBUS.SAMPLEPERIOD parameter in the amplifier or configured in the startup phase. This happens via SDO mailbox access to objects 60C2 subindex 1 and 2.
Subindex 2, known as the interpolation time index, defines the power of ten of the time value
(e.g. -3 means 10-3 or milliseconds) while subindex 1, known as interpolation time units,
gives the number of units (e.g. 4 means 4 units).
You can run a 2 ms cycle using various combinations. For example,
Index = -3, Units = 2 or
Index = -4, Units = 20 etc.
The FBUS.SAMPLEPERIOD parameter is counted in multiples of 62.5us microseconds
within the de
 */


//flexible pdo mapping possible options

/*
 * mfr status bytes
 * brake control
 * moo disp
 * moo set
 *
 */




/*
 * so...
 * mooset and disp must be via sdo unless we do fancy pdo remapping
 * error code either sdo read or work out these co error emergency messages
 *
 *
 *
 */




//0 is limit turned off
//this is sdo
//must be saved and the drive restarted



#endif //MONOREPO_KOLLMORGEN_H
