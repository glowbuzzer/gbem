/**
 ******************************************************************************
 * @file           :  p
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "p.h"

static const double Kp = 0.1;

int32_t calculateVelocityCommand(int32_t setPos, int32_t actPos) {
    int32_t error = setPos - actPos;
    double velocityCommand = Kp * (double) error;
    return (int32_t) velocityCommand;
}