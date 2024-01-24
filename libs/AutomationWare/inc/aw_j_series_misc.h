/**
 ******************************************************************************
 * @file           :  aw_j_series_misc
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_AW_J_SERIES_MISC_H
#define GBEM_AW_J_SERIES_MISC_H

#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"

gberror_t ec_print_params_aw_j_series(uint16_t drive);

gberror_t ec_get_secondary_name_aw_j_series(uint16_t slave, char *secondary_name);


#endif //GBEM_AW_J_SERIES_MISC_H
