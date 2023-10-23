/**
 ******************************************************************************
 * @file           :  reboot
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "status_control_word_bit_definitions.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "dpm.h"
#include "user_message.h"
#include "log.h"
#include <pthread.h>
#include "shared.h"
#include "ecrxtx.h"
#include "ec_functions.h"
#include <unistd.h>
#include "main.h"
#include "plc_core.h"


_Noreturn void ec_reboot(void *argument) {

    while (1) {

        if (BIT_CHECK(dpm_out->machine_word, CONTROL_WORD_GBEM_REBOOT_BIT_NUM)) {

            printf("dpm_out->machine_word: %d\n", dpm_out->machine_word);
            LL_FATAL("CONTROL_WORD_GBEM_REBOOT_BIT_NUM is set!");

        }

//            if (BIT_CHECK(dpm_out->machine_word, CONTROL_WORD_GBEM_REBOOT_BIT_NUM) &&
//                ecm_status.boot_state.boot_sucessful) {
//                LL_INFO(GBEM_GEN_LOG_EN, "GBEM: Reboot requested by high-level control");
//                ec_rxtx_mode = EC_RXTX_MODE_NONE;
//                ecm_status.boot_state.boot_sucessful = false;
//                int pthread_cancel_rc = 0;
//                pthread_cancel_rc = pthread_cancel(thread_ec_check);
//                if (pthread_cancel_rc != 0) {
//                    UM_ERROR(GBEM_UM_EN, "GBEM: Failed to cancel thread_ec_check");
//                } else {
//                    UM_INFO(GBEM_UM_EN, "GBEM: Cancelled thread_ec_check");
//                }
//                pthread_cancel_rc = pthread_cancel(thread_ec_rxtx);
//                if (pthread_cancel_rc != 0) {
//                    UM_ERROR(GBEM_UM_EN, "GBEM: Failed to cancel thread_ec_rxtx");
//                } else {
//                    UM_INFO(GBEM_UM_EN, "GBEM: Cancelled thread_ec_rxtx");
//                }
//                pthread_cancel_rc = pthread_cancel(thread_ec_error_message);
//                if (pthread_cancel_rc != 0) {
//                    UM_ERROR(GBEM_UM_EN, "GBEM: Failed to cancel thread_ec_error_message");
//                } else {
//                    UM_INFO(GBEM_UM_EN, "GBEM: Cancelled thread_ec_error_message");
//                }
//
//                //todo crit
//                //frees the state machine
////            ctrl_statemachinefree(m);
////            struct stateMachine *m;
////            m = ctrl_statemachine_alloc();
//
//
//                /* Kill any PLC tasks that are running */
//                for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {
//
//                    pthread_cancel_rc = pthread_cancel(plc_task_set.tasks[i].id);
//                    if (pthread_cancel_rc != 0) {
//                        UM_ERROR(GBEM_UM_EN, "GBEM: Failed to cancel thread_ec_error_message");
//                    } else {
//                        UM_INFO(GBEM_UM_EN, "GBEM: Cancelled thread_ec_error_message");
//                    }
//
//                }
//
//                //run main again
//                main(main_argc, main_argv);
//    }

        else {
//            sleep(2);
        }

    } //end while
}