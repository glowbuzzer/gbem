/**
 ******************************************************************************
 * @file           :  torque_controller
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_TORQUE_CONTROLLER_H
#define GBEM_TORQUE_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

double torque_controller(uint64_t ms_counter, uint16_t drive_num, bool pos_vel_control, double joint_set_position,
                         double joint_set_velocity, double joint_set_torque, double joint_act_position,
                         double joint_act_velocity);


void reset_torque_controller(uint16_t drive_num);

#endif //GBEM_TORQUE_CONTROLLER_H
