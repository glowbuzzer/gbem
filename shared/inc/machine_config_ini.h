/**
 ******************************************************************************
 * @file           :  machine_config_ini
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef MACHINE_CONFIG_INI_H
#define MACHINE_CONFIG_INI_H


gberror_t read_machine_config_ini(uint8_t *number_of_items_found);

bool check_machine_config_ini_exists(void);

uint8_t get_machine_config_ini_sections(void);

#endif //MACHINE_CONFIG_INI_H
