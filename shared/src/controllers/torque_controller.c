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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "controller.h"



//mechanical time constant is 10hz, the position loop runs at 50hz and the velocity loop at 250hz. Anything more and you are just amplifying noise.

// Constants for PID controllers
static pid_params_t pid_params_position[MAP_MAX_NUM_DRIVES] = {{3.0, 0.0002, 0.0, 2000.0, -2000.0, 20}};
//pid_params_t pid_params_position[MAP_MAX_NUM_DRIVES] = {{3.0, 0.0004, 0.0, 2000.0, -2000.0, 20}};

static pid_params_t pid_params_velocity[MAP_MAX_NUM_DRIVES] = {{30.0, 0.0, 0.000, 10.0, -10.0, 4}};
//pid_params_t pid_params_velocity[MAP_MAX_NUM_DRIVES] = {{10.0, 0.0, 0.000, 10.0, -10.0, 4}};


static scale_t scale = {
        .scale_position = 166886,
        .scale_velocity = 9549,
        .scale_torque = 32.68
};


// Global variables for PID controllers
static double position_integral[MAP_MAX_NUM_DRIVES] = {0};
static double position_prev_error[MAP_MAX_NUM_DRIVES] = {0};

static double velocity_integral[MAP_MAX_NUM_DRIVES] = {0};
static double velocity_prev_error[MAP_MAX_NUM_DRIVES] = {0};

static double position_command[MAP_MAX_NUM_DRIVES] = {0};
static double velocity_command[MAP_MAX_NUM_DRIVES] = {0};


void reset_torque_controller(uint16_t drive_num) {
    position_integral[drive_num] = 0;
    position_prev_error[drive_num] = 0;
    velocity_integral[drive_num] = 0;
    velocity_prev_error[drive_num] = 0;
    position_command[drive_num] = 0;
    velocity_command[drive_num] = 0;
}


// Function to update the PID controllers and set the torque command
double t_updatePositionController(uint64_t cycle_counter, uint16_t drive_num, double act_position,
                                  double target_position) {

    if (cycle_counter % (pid_params_position[drive_num].sample_time / MAP_CYCLE_TIME) == 0) {


        // Position PID control
        double position_error = target_position - act_position;
        position_integral[drive_num] += position_error * pid_params_position[drive_num].sample_time;

        // Limit the integral term
//        if (position_integral[drive_num] > pid_params_position[drive_num].max_integral) {
//            printf("limit integral max\n");
//            position_integral[drive_num] = pid_params_position[drive_num].max_integral;
//        } else if (position_integral[drive_num] < pid_params_position[drive_num].min_integral) {
//            printf("limit integral min\n");
//            position_integral[drive_num] = pid_params_position[drive_num].min_integral;
//        }

//        printf("position_integral: %f\n", position_integral[drive_num]);

        double position_derivative =
                (position_error - position_prev_error[drive_num]) / pid_params_position[drive_num].sample_time;
        position_command[drive_num] = pid_params_position[drive_num].kp * position_error +
                                      pid_params_position[drive_num].ki * position_integral[drive_num] +
                                      pid_params_position[drive_num].kd * position_derivative;

        // Update previous errors for next iteration
        position_prev_error[drive_num] = position_error;

    }
    return position_command[drive_num];
}

double
t_updateVelocityController(uint64_t cycle_counter, uint16_t drive_num, double act_velocity, double target_velocity) {


    if (cycle_counter % (pid_params_velocity[drive_num].sample_time / MAP_CYCLE_TIME) == 0) {
        // Velocity PID control

        double velocity_error = target_velocity - act_velocity;
        velocity_integral[drive_num] += velocity_error * pid_params_velocity[drive_num].sample_time;

//        // Limit the integral term
//        if (velocity_integral[drive_num] > pid_params_velocity[drive_num].max_integral) {
//            velocity_integral[drive_num] = pid_params_velocity[drive_num].max_integral;
//        } else if (velocity_integral[drive_num] < pid_params_velocity[drive_num].min_integral) {
//            velocity_integral[drive_num] = pid_params_velocity[drive_num].min_integral;
//        }

        double velocity_derivative =
                (velocity_error - velocity_prev_error[drive_num]) / pid_params_velocity[drive_num].sample_time;
        velocity_command[drive_num] = pid_params_velocity[drive_num].kp * velocity_error +
                                      pid_params_velocity[drive_num].ki * velocity_integral[drive_num] +
                                      pid_params_velocity[drive_num].kd * velocity_derivative;
        // Update previous errors for next iteration

        velocity_prev_error[drive_num] = velocity_error;

    }

    return velocity_command[drive_num];

}


double torque_controller(uint64_t cycle_counter, uint16_t drive_num, bool pos_vel_control, double joint_set_position,
                         double joint_set_velocity, double joint_set_torque, double joint_act_position,
                         double joint_act_velocity) {

    double joint_act_position_s = joint_act_position / scale.scale_position;
    double joint_set_position_s = joint_set_position / scale.scale_position;
    double joint_act_velocity_s = joint_act_velocity / scale.scale_velocity;
    double joint_set_velocity_s = joint_set_velocity / scale.scale_velocity;


    if (pos_vel_control == true) {

        double control_position = t_updatePositionController(cycle_counter, drive_num, joint_act_position_s,
                                                             joint_set_position_s);


        double control_velocity = t_updateVelocityController(cycle_counter, drive_num, joint_act_velocity_s,
                                                             joint_set_velocity_s);


        double torque_command = ((control_position + control_velocity) * scale.scale_torque) + joint_set_torque;

        if (joint_set_torque > 0.0001 || joint_set_torque < -0.0001) {
            printf(">>> joint_set_torque [%f] (from model)\n", joint_set_torque);
            printf(">>> torque_command [%f] (error from pos and vel)\n", torque_command * scale.scale_torque);
            printf(">>> overall torque [%f]\n", (torque_command * scale.scale_torque) + joint_set_torque);
            printf("joint_act_position_s: %f\n", joint_act_position_s);
            printf("joint_set_position_s: %f\n", joint_set_position_s);
            printf("joint_act_velocity_s: %f\n", joint_act_velocity_s);
            printf("joint_set_velocity_s: %f\n", joint_set_velocity_s);
        }

        return (torque_command);
    } else {
        return joint_set_torque;
    }

}