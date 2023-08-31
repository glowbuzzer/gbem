/**
 ******************************************************************************
 * @file           :  aw-j-series.h
 * @brief          :  common defines and prototypes for all AutomationWare j-series drives
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_AW_J_SERIES_H
#define GBEM_AW_J_SERIES_H

#include "map.h"
#include "gberror.h"
#include "automationware.h"


/** Slave functions */

gberror_t ec_initial_pdo_aw_j_series(uint16_t slave);

int8_t ec_get_moo_pdo_aw_j_series(uint16_t drive);


/** Error functions */
uint8_t *ec_get_error_string_sdo_aw_j_series(uint16_t drive);

uint8_t *ec_get_detailled_error_report_sdo_aw_j_series(uint16_t drive_number);

/** Drive functions */
gberror_t ec_set_ctrl_wrd_aw_j_series(uint16_t drive, uint16_t ctrlwrd);

uint16_t ec_get_stat_wrd_aw_j_series(uint16_t drive);

int32_t ec_get_actpos_wrd_aw_j_series(uint16_t drive);

int32_t ec_get_actvel_wrd_aw_j_series(uint16_t drive);

int16_t ec_get_acttorq_wrd_aw_j_series(uint16_t drive);

gberror_t ec_set_setpos_wrd_aw_j_series(uint16_t drive, int32_t setpos);

bool ec_get_remote_aw_j_series(uint16_t drive);

bool ec_get_follow_error_aw_j_series(uint16_t drive);

uint16_t ec_get_ctrl_wrd_rev_aw_j_series(uint16_t drive);

gberror_t ec_set_stat_wrd_rev_aw_j_series(uint16_t drive, uint16_t statwrd);

gberror_t ec_set_actpos_wrd_rev_aw_j_series(uint16_t drive, int32_t actpos);

int32_t ec_get_setpos_word_rev_aw_j_series(uint16_t drive);

gberror_t ec_set_moo_pdo_rev_aw_j_series(uint16_t drive);


//Number of error strings and error report strings for the AW J series drives
#define NUM_OF_AW_J_SERIES_ERROR_STRINGS 26
#define NUM_OF_AW_J_SERIES_ERROR_REPORT_STRINGS 98


//PDO indexes for the AW J series drives - these are most easily found from a netscan of the drive
//These apply to the standard PDO mapping
#define AW_J_SERIES_SETPOS_PDO_INDEX        5
#define AW_J_SERIES_ACTPOS_PDO_INDEX        3
#define AW_J_SERIES_CONTROLWORD_PDO_INDEX   0
#define AW_J_SERIES_STATUSWORD_PDO_INDEX    0
#define AW_J_SERIES_MOOSET_PDO_INDEX        2
#define AW_J_SERIES_MOODISP_PDO_INDEX       2
#define AW_J_SERIES_ACTVEL_PDO_INDEX        7
#define AW_J_SERIES_ACTTORQ_PDO_INDEX       11


//SDO indexes for the AW J series drives

//Communication cycle period	0x1006:0	DINT	32			100		readwrite
#define AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_INDEX      0x1006
#define AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_SUB_INDEX  0x0

//Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
//Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)
#define AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX      0x607D
#define AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX  0x1
#define AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX      0x607D
#define AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX  0x2

//Error code	0x603F:0	UINT	16			0		readonly (default)
#define AW_J_SERIES_ERROR_CODE_SDO_INDEX      0x603F
#define AW_J_SERIES_ERROR_CODE_SDO_SUB_INDEX  0x0

//Max torque	0x6072:0	UINT	16	0	32767	3000		readwrite
#define AW_J_SERIES_MAX_TORQUE_SDO_INDEX      0x6072
#define AW_J_SERIES_MAX_TORQUE_SDO_SUB_INDEX  0x0

//Motor rated torque	0x6076:0	UDINT	32	1		1	mNm	readwrite
#define AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_INDEX      0x6076
#define AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_SUB_INDEX  0x0

//Motor revolutions	0x6091:1	UDINT	32			1		readwrite
#define AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_INDEX      0x6091
#define AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_SUB_INDEX  0x1

//Shaft revolutions	0x6091:2	UDINT	32			1		readwrite
#define AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_INDEX      0x6091
#define AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_SUB_INDEX  0x2

//Polarity	0x607E:0	USINT	8 (None	0, Position polarity flipped	128, Velocity polarity flipped	64, Both flipped	192		readwrite
#define AW_J_SERIES_POLARITY_SDO_INDEX      0x607E
#define AW_J_SERIES_POLARITY_SDO_SUB_INDEX  0x0

//Description	0x203F:1	STRING(8)	64			0		readonly (default)
#define AW_J_SERIES_ERROR_DESCRIPTION_SDO_INDEX      0x203F
#define AW_J_SERIES_ERROR_DESCRIPTION_SDO_SUB_INDEX  0x1


//Typedefs for the error strings and error report strings for the AW J series drives
typedef struct {
    uint16_t error_id;
    char text_string[100];
} aw_j_series_error_string_t;

typedef struct {
    char error_code[8];
    char text_string[100];
} aw_j_series_error_report_string_t;

extern const aw_j_series_error_string_t aw_j_series_error[NUM_OF_AW_J_SERIES_ERROR_STRINGS];
extern const aw_j_series_error_report_string_t aw_j_series_error_report[NUM_OF_AW_J_SERIES_ERROR_REPORT_STRINGS];


#endif //GBEM_AW_J_SERIES_H
