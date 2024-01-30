/**
 ******************************************************************************
 * @file           :  debug_ini
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef DEBUG_INI_H
#define DEBUG_INI_H


gberror_t read_debug_ini(uint8_t *number_of_items_found);

bool check_debug_ini_exists(void);

uint8_t get_debug_ini_sections(void);

#endif //DEBUG_INI_H
