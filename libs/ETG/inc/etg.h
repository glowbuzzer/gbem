/**
 ******************************************************************************
 * @file           :  etg.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_ETG_H
#define GB_MONOREPO_ETG_H
#include "gberror.h"
#include "stdint.h"

typedef struct {
    uint32_t id;
    char *name;
} etg_vendor_id_t;

#define ETG_NUM_VENDOR_IDS 100

extern etg_vendor_id_t etg_vendor_id_list[ETG_NUM_VENDOR_IDS];

char * etg_lookup_vendor_id (uint32_t id, etg_vendor_id_t *varray, uint32_t rows_in_varray, gberror_t *grc);

#endif //GB_MONOREPO_ETG_H
