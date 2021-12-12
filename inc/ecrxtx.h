/**
 ******************************************************************************
 * @file           :  ecrxtx.h
 * @brief          :  main cyclic functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_ECRXTX_H
#define GBEM_ECRXTX_H


void ec_rxtx(void *argument);



extern bool print_repeater_message;
extern bool ec_rxtx_test_mode;

typedef enum {EC_RXTX_MODE_NONE, EC_RXTX_MODE_DORUN, EC_RXTX_MODE_OP, EC_RXTX_MODE_HOME} ec_rxtx_mode_t;

//these affect what gets called in the main cyclic thread and are set by the boot process

extern bool homing_failed;
extern ec_rxtx_mode_t ec_rxtx_mode;


#endif //GBEM_ECRXTX_H
