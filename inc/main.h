/**
 ******************************************************************************
 * @file           :  main.h
 * @brief          :  GBEM main
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_MAIN_H
#define GBEM_MAIN_H
#include <stdint.h>
#include "stdbool.h"
#include "shared.h"




extern char storage_dir_for_gbc_exchange[200];
extern char full_path_for_emstat_json_const[200];
extern char full_path_for_emstat_json_status[200];
extern char full_path_for_config_json[200];

extern struct shm_msg *shmp;

extern char eth_interface1[SIZE_OF_IF_NAME];
extern char eth_interface2[SIZE_OF_IF_NAME];


extern uint8_t IOmap[ECM_IO_MAP_SIZE];
extern volatile int wkc;
extern int expectedWKC;
extern uint8_t currentgroup;

extern uint16_t slave_to_write_nvram;



extern int gbc_pid;

void main_set_file_paths(void);


#endif //GBEM_MAIN_H
