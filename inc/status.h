/**
 ******************************************************************************
 * @file           :  status.h
 * @brief          :  functions to produce a JSON status file cyclically
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_STATUS_H
#define GBEM_STATUS_H

#include <stdbool.h>
#include "shared.h"

_Noreturn void ec_emstat(void *argument);

bool write_const_json();
bool write_status_json();

int status_data_to_json(char *dest, ecm_status_t *data);
int const_data_to_json(char *dest, ecm_status_t *data);

#endif //GBEM_STATUS_H
