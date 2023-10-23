/**
 ******************************************************************************
 * @file           :  velocity_controller
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_VELOCITY_CONTROLLER_H
#define GBEM_VELOCITY_CONTROLLER_H

#include <stdint.h>

double velocity_controller(uint64_t cycle_counter, uint16_t drive_num, double joint_set_position,
                           double joint_set_velocity, double joint_act_position);

void reset_velocity_controller(uint16_t drive_num);

#endif //GBEM_VELOCITY_CONTROLLER_H
