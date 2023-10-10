
/**
 ******************************************************************************
 * @file           :  plc_task_1.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */



#include "plc_task_1.h"
#include "plc_core.h"
#include "time.h"
#include "std_defs_and_macros.h"
#include "user_message.h"

bool plc_in_1_SS1_CMD_SW;
bool plc_in_2_STOP_CMD_SW;
bool plc_in_3_ARM_48V_SUPPLY;
bool plc_in_4_RC_LIGHT_SIGNAL;
bool plc_in_5_BRAKE_CHOPPER_ERROR;
bool plc_in_6_IN_TOOL_1;
bool plc_in_7_IN_TOOL_2;

/*
 * Inputs
 * 1 SS1_CMD_SW - this triggers the quick stop
 * 2 STOP_CMD_SW - this triggers the quick stop too
 * 3 ARM_48V_SUPPLY
 * 4 RC_LIGHT_SIGNAL
 * 5 BRAKE_CHOPPER_ERROR
 * 6 IN_TOOL_1 - feedback from gripper
 * 7 IN_TOOL_2 - feedback from gripper
 */

bool plc_out_1_SW_HEARTBYTE_CH1;
bool plc_out_2_SW_HEARTBYTE_CH2;
bool plc_out_3_SS1_CMD_SW_FB;
bool plc_out_4_STOP_CMD_SW_FB;
bool plc_out_5_SPARE;
bool plc_out_6_OUT_TOOL_1;
bool plc_out_7_OUT_TOOL_2;

_Noreturn void *plc_aw_robot_task_1(void *argument) {

    PLC_TASK_START

//        printf("plc_aw_robot_task_1\n");

        //These signals have to be set HIGH during the normal operation to inform the “S-PLC” that the “EEMC” is working
        plc_out_1_SW_HEARTBYTE_CH1 = true;
        plc_out_2_SW_HEARTBYTE_CH2 = true;

        //This signal has to be set to replicate the status of the IN1. It is the feedback signal to the “S-PLC” needed to verify that the “EEMC” has received and managed the SS 1-t command via IN1/In2
        plc_out_3_SS1_CMD_SW_FB = plc_in_1_SS1_CMD_SW;
        plc_out_4_STOP_CMD_SW_FB = plc_in_2_STOP_CMD_SW;

        //handle plc_in_5_BRAKE_CHOPPER_ERROR in front end or in plc?
//        if (plc_in_5_BRAKE_CHOPPER_ERROR) {
//            plc_signalled_error = true;
//        } else {
//            plc_signalled_error = false;
//        }

        if (!plc_in_3_ARM_48V_SUPPLY) {
            //?
        }





//        plc_task_set.tasks[task_index].exec.completed=true;

    PLC_TASK_END


}
