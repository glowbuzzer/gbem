/**
 ******************************************************************************
 * @file           :  ec_functions.h
 * @brief          :  functions for EtherCAT
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef INC_EC_FUNCTIONS_H_
#define INC_EC_FUNCTIONS_H_

#include <stdbool.h>
#include <stdint.h>
#include "gberror.h"


gberror_t ec_ack_errors(void);

gberror_t ec_slaves_match(void);

void ECBoot(void *argument);

void copy_ec_slave_to_ecm_status(void);

_Noreturn void ec_check(void *argument);

void ecm_netscan(char *ifname1, bool apply_pdo_mapping_before_netscan);

void init_ecm_status(void);

//extern const char *ec_state_to_string[];

bool ec_step_1_init(void);

bool ec_step_2_config(void);

bool ec_step_3_preop(void);

bool ec_step_5_error_check(void);

bool ec_step_6_safeop(void);

bool ec_step_7_wkc_check(void);

bool ec_step_8_slaves_match(void);

bool ec_step_9_op(void);

gberror_t print_1c32(uint16_t slave);


typedef enum {
    DC_CLOCK_ERROR,
    DC_CLOCK_NONE,
    DC_CLOCK_32,
    DC_CLOCK_64,
} dc_clock_type_t;


dc_clock_type_t check_dc_clock_type(const uint16_t slave);

#endif /* INC_EC_FUNCTIONS_H_ */
