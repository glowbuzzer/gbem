/**
 ******************************************************************************
 * @file           :  velocity_controller
 * @brief          :  PID loop to control position in CSV mode
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "controller.h"

static pid_params_t pid_params_position[MAP_MAX_NUM_DRIVES] = {{2.0, 0.002, 0.0, 2000.0, -2000.0, 20}};

static scale_t scale = {
        .scale_position = 166886,
        .scale_velocity = 9549,
        .scale_torque = 32.68
};


// Global variables for PID controllers
static double position_integral[MAP_MAX_NUM_DRIVES] = {0};
static double position_prev_error[MAP_MAX_NUM_DRIVES] = {0};


static double position_command[MAP_MAX_NUM_DRIVES] = {0};
static double velocity_command[MAP_MAX_NUM_DRIVES] = {0};


void reset_velocity_controller(uint16_t drive_num) {
    position_integral[drive_num] = 0;
    position_prev_error[drive_num] = 0;
    position_command[drive_num] = 0;
    velocity_command[drive_num] = 0;
}


// Function to update the PID controllers and set the torque command
double v_updatePositionController(uint64_t cycle_counter, uint16_t drive_num, double act_position,
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

#define ALPHA 0.2


double velocity_controller(uint64_t cycle_counter, uint16_t drive_num, double joint_set_position,
                           double joint_set_velocity, double joint_act_position) {

    double joint_act_position_s = joint_act_position / scale.scale_position;
    double joint_set_position_s = joint_set_position / scale.scale_position;


//    static double filtered_velocity = 0.0;


    double control_position = v_updatePositionController(cycle_counter, drive_num, joint_act_position_s,
                                                         joint_set_position_s);


    double vel_command = (control_position * scale.scale_position) + joint_set_velocity;

//    filtered_velocity = (1.0 - ALPHA) * filtered_velocity + ALPHA * vel_command;


//    return (filtered_velocity);

    return (joint_set_velocity);

}


