/**
 ******************************************************************************
 * @file           :  limits_ini
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef LIMITS_INI_H
#define LIMITS_INI_H


gberror_t read_limits_ini(uint8_t *number_of_items_found);

bool check_limits_ini_exists(void);

uint8_t get_limits_ini_sections(void);

#endif //LIMITS_INI_H
