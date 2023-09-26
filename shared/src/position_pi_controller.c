/**
 ******************************************************************************
 * @file           :  position_pi_controller
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "stdint.h"
#include "map.h"
#include "user_message.h"

//double Kp = 0.1;
//double Ki = 0.01;

//uint32_t loop_frequency = 250;

void position_pi_controller(int32_t demand_position, int32_t actual_position, int32_t demand_velocity,
                            int32_t *adjusted_velocity, uint64_t bus_cycle_count, uint32_t loop_frequency, double Kp,
                            double Ki, double *integral) {

//    double integral = 0;

    if (loop_frequency == 0) {
        UM_FATAL("GBEM: position_pi_controller loop_frequency is 0 which is invalid");
    }
    if (loop_frequency > MAP_CYCLE_TIME * 1000) {
        UM_FATAL("GBEM: position_pi_controller loop_frequency is greater than MAP_CYCLE_TIME*100 which is invalid");
    }

    if (bus_cycle_count * MAP_CYCLE_TIME % (1000 / loop_frequency) == 0) {
//code


        int32_t position_error = demand_position - actual_position;

        //clamp position error to VMAX

        //Calculate PI terms
        double p_term = position_error * Kp;
        *integral += Ki * position_error;

        //Calulate contol signal
        int32_t control_signal = (int32_t) p_term + (int32_t) *integral;

        *adjusted_velocity = control_signal + demand_velocity;

    }
}

/*
// Define integral clamping bounds
double integral_min = -MAX_INTEGRAL;
double integral_max = MAX_INTEGRAL;

// Inside the control loop
double p_term = position_error * Kp;
*integral += Ki * position_error;

// Apply integral clamping
if (*integral > integral_max) {
    *integral = integral_max;
} else if (*integral < integral_min) {
    *integral = integral_min;
}

int32_t control_signal = (int32_t)(p_term + *integral);
// Inside the control loop
double p_term = position_error * Kp;

// Conditional integration
if (condition_met) {
*integral += Ki * position_error;
}

int32_t control_signal = (int32_t)(p_term + *integral);


// Inside the control loop
double p_term = position_error * Kp;
double unsaturated_integral = *integral + Ki * position_error;

// Calculate the control signal without saturation
int32_t control_signal_unsaturated = (int32_t)(p_term + unsaturated_integral);

// Check for saturation
if (control_signal_unsaturated > MAX_CONTROL_SIGNAL || control_signal_unsaturated < MIN_CONTROL_SIGNAL) {
    // Apply anti-windup: adjust integral term
    *integral = *integral + (control_signal - control_signal_unsaturated);
}

int32_t control_signal = (int32_t)(p_term + *integral);


*/


//Conditional Clamping: You can apply clamping based on certain conditions. For example, you may choose to clamp the integral term only when the control signal is saturated:

//if (control_signal > control_max || control_signal < control_min) {
//if (*integral > integral_max) {
//*integral = integral_max;
//} else if (*integral < integral_min) {
//*integral = integral_min;
//}
//}


//Soft Clamping: In soft clamping, you gradually reduce the impact of the integral term as it approaches the bounds. This can help prevent abrupt changes in the control signal. Here's a basic implementation:
//if (*integral > integral_max) {
//*integral = integral_max - softening_factor;
//} else if (*integral < integral_min) {
//*integral = integral_min + softening_factor;
//}


//Hard Clamping: This is the simplest form of clamping. You set upper and lower bounds, and if the variable exceeds these bounds, it is forcibly set to the bound value. In a PI controller, you might clamp the integral term like this:

//if (*integral > integral_max) {
//*integral = integral_max;
//} else if (*integral < integral_min) {
//*integral = integral_min;
//}