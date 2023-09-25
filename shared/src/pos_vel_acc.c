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
#include "pos_vel_acc.h"


int32_t get_demand_position(uint16_t drive) {
    return dpm_out->joint_set_position[drive];
}

int32_t get_demand_velocity(uint16_t drive) {

    return dpm_out->joint_set_velocity[drive];
}


int32_t get_demand_acceleration(uint16_t drive) {

    return dpm_out->joint_set_torque[drive];

}

int32_t get_actual_position(uint16_t drive) {
    return dpm_in->joint_actual_position[drive];
}

int32_t get_actual_velocity(uint16_t drive) {
    return dpm_in->joint_actual_velocity[drive];
}

int32_t get_actual_torque(uint16_t drive) {
    return dpm_in->joint_actual_torque[drive];
}


