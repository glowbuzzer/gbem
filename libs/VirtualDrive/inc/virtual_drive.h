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

gberror_t ec_set_setpos_wrd_virtual(uint16_t drive, int32_t setpos);

bool ec_get_remote_aw_j_virtual(uint16_t drive);

bool ec_get_follow_error_virtual(uint16_t drive);

bool ec_get_remote_virtual(const uint16_t drive);

#endif //GBEM_VIRTUAL_H
