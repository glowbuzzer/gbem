
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
#include "map_config.h"

#if MACHINE_AW_6DOF ==  0

bool plc_din1, plc_din2, plc_din3, plc_din4, plc_din5, plc_din6, plc_din7, plc_din8;

bool plc_dout1, plc_dout2, plc_dout3, plc_dout4, plc_dout5, plc_dout6, plc_dout7, plc_dout8;


_Noreturn void *plc_mytask1(void *argument) {

    //init stuff


    PLC_TASK_START

        static int counter = 0;

//        printf("mytask1\n");

//        if (plc_din1) {
//            printf("Button 1 pressed\n");
//        }
//        if (plc_din2) {
//            printf("Button 2 pressed\n");
//        }

        counter++;

//        if (counter % 2 == 0) {
//            printf("LED 1 on\n");
//            plc_dout1 = 0;
//        } else {
//            printf("LED 1 off\n");
//            plc_dout1 = 1;
//        }


//plc_signalled_error=false;

//        plc_task_set.tasks[task_index].exec.completed=true;

    PLC_TASK_END


}

#endif