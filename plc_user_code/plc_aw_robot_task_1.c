
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
#include "log.h"

bool plc_din1, plc_din2, plc_din3, plc_dout1, plc_dout2, plc_dout3, mydin1_test, mydout1_test;


_Noreturn void *plc_mytask1(void *argument) {

    //init stuff


    PLC_TASK_START

        static int counter = 0;

        printf("mytask1\n");

        if (plc_din1) {
            printf("Button 1 pressed\n");
        }
        if (plc_din2) {
            printf("Button 2 pressed\n");
        }

        counter++;

        if (counter % 2 == 0) {
            printf("LED 1 on\n");
            plc_dout1 = 0;
        } else {
            printf("LED 1 off\n");
            plc_dout1 = 1;
        }


//plc_signalled_error=false;

//        plc_task_set.tasks[task_index].exec.completed=true;

    PLC_TASK_END


}
