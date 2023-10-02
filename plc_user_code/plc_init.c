
/**
 ******************************************************************************
 * @file           :  plc_init.c
 * @brief          :  user code - all PLC tasks that need to executed need to be registered here
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "plc_init.h"
#include "plc_core.h"
//#include "plc_task_1.h"
#include "plc_aw_robot_task_1.h"

/** Used to register new PLC tasks - user to add their tasks here */
void plc_register_tasks_user(void) {

//    plc_register_tasks(plc_mytask1, 5000, 1, "Task1");

    plc_register_tasks(plc_aw_robot_task_1, 500, 1, "AwRobotTask1");


}





