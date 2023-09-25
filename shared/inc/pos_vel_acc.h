/**
 ******************************************************************************
 * @file           :  pos_vel_acc
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_POS_VEL_ACC_H
#define GBEM_POS_VEL_ACC_H

#include "stdint.h"
#include "map.h"
#include "dpm.h"


int32_t get_demand_position(uint16_t drive);

int32_t get_demand_velocity(uint16_t drive);

int32_t get_demand_acceleration(uint16_t drive);

int32_t get_actual_position(uint16_t drive);

int32_t get_actual_velocity(uint16_t drive);

int32_t get_actual_torque(uint16_t drive);


void persist_last_positions_and_velocities(void);


#endif //GBEM_POS_VEL_ACC_H
