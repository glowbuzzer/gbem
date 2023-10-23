/**
 ******************************************************************************
 * @file           :  controller
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_CONTROLLER_H
#define GBEM_CONTROLLER_H

#include <stdint.h>
#include "map.h"
#include "velocity_controller.h"
#include "torque_controller.h"


/** @brief  PID controller parameters
 *
 *  This struct contains the parameters for a PID controller.
 */
typedef struct {
    double kp; //Proportional gain
    double ki; //Integral gain
    double kd; //Derivative gain
    double max_integral; //Maximum integral windup
    double min_integral; //Minimum integral windup
    uint16_t sample_time; //Sample time (in milliseconds)
} pid_params_t;


typedef struct {
    double scale_position;
    double scale_velocity;
    double scale_torque;
} scale_t;

#endif //GBEM_CONTROLLER_H
