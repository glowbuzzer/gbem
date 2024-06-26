/**
 ******************************************************************************
 * @file           :  virtual.h
 * @brief          :  virtual drive functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_VIRTUAL_H
#define GBEM_VIRTUAL_H

#include "map.h"
#include "gberror.h"


/** Drive functions */
gberror_t ec_set_ctrl_wrd_virtual(uint16_t drive, uint16_t ctrlwrd);

uint16_t ec_get_stat_wrd_virtual(uint16_t drive);

int32_t ec_get_actpos_wrd_virtual(uint16_t drive);

int32_t ec_get_actvel_wrd_virtual(uint16_t drive);

int32_t ec_get_acttorq_wrd_virtual(uint16_t drive);


gberror_t ec_set_setpos_wrd_virtual(uint16_t drive, int32_t setpos);

gberror_t ec_set_setvel_wrd_virtual(uint16_t drive, int32_t setvel);

gberror_t ec_set_settorq_wrd_virtual(uint16_t drive, int32_t settorq);

gberror_t ec_set_moo_pdo_virtual(const uint16_t drive, int8_t moo);


int32_t ec_get_control_effort_wrd_virtual(uint16_t drive);

bool ec_get_follow_error_virtual(uint16_t drive);

bool ec_get_remote_virtual(uint16_t drive);

int8_t ec_get_moo_pdo_virtual(uint16_t drive);

//ec_get_error_string_pdo_virtual
uint8_t *ec_get_error_string_pdo_virtual(uint16_t drive, bool *error);

gberror_t ec_set_settorqoffset_wrd_virtual(uint16_t drive, int32_t settorqoffset);

gberror_t ec_set_setveloffset_wrd_virtual(const uint16_t drive, const int32_t setveloffset);


#endif //GBEM_VIRTUAL_H
