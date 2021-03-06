/**
 ******************************************************************************
 * @file           :  map_summary.h
 * @brief          :  produces a summary of the machine config
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_MAP_SUMMARY_H
#define GBEM_MAP_SUMMARY_H

#include "gberror.h"

int config_create_check_print(char * json_buffer, gberror_t *grc, bool print_output);


#endif //GBEM_MAP_SUMMARY_H
