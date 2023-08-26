/**
 ******************************************************************************
 * @file           :  testEthercat.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "test.h"
#include "shared.h"
#include <unistd.h>
#include "ec_functions.h"
#include "ecrxtx.h"


bool mydin1_test, mydin2_test, mydout1_test, mydout2_test, plc_din1;

#define RUN_CYCLIC_TEST(name) do {\
    fun_ptr = name;\
    strcpy(test_result.test_name, #name);\
    test_result.ran_to_completion = false;\
    test_result.premature_termination = false;\
    while (!test_result.ran_to_completion && !test_result.premature_termination){\
        sleep(1);\
    }\
    RUN_TEST(pass_fail);\
} while(0)

bool ec_printSDO = false;

typedef struct {
    bool premature_termination;
    bool ran_to_completion;
    char test_name[100];
} test_result_t;

void nop(void) {

}

volatile test_result_t test_result = {.ran_to_completion=false, .premature_termination=false, .test_name="nop"};

void (*fun_ptr)(void) = &nop;

void cyclicTest(void) {
    if (!test_result.premature_termination && !test_result.ran_to_completion) {
        (*fun_ptr)();
    }
    dpm_out->heartbeat = dpm_in->heartbeat;
}

void setUp(void) {

}

void tearDown(void) {
    // clean stuff up here
}

void test_boot(void) {
    TEST_ASSERT_MESSAGE(ec_step_1_init() == true, "Step 1: Init Ethernet interface");
    TEST_ASSERT_MESSAGE(ec_step_2_config() == true, "Step 2: Configure slaves");
    TEST_ASSERT_MESSAGE(ec_step_3_preop() == true, "Step 3: Move slaves to PRE_OP");
    TEST_ASSERT_MESSAGE(ec_step_5_error_check() == true, "Step 4: EtherCAT error check");
    TEST_ASSERT_MESSAGE(ec_step_6_safeop() == true, "Step 5: Move slaves to SAFE-OP");
    TEST_ASSERT_MESSAGE(ecm_status.boot_state.pdo_remap_done && ecm_status.boot_state.apply_standard_sdos_done,
                        "Step 6: hooks complete");
    TEST_ASSERT_MESSAGE(ec_step_7_wkc_check() == true, "Step 7: Run WKC check");
    TEST_ASSERT_MESSAGE(ec_step_8_slaves_match() == true, "Step 8: Check slaves match");
    TEST_ASSERT_MESSAGE(ec_step_9_op() == true, "Step 9: Move slaves to OP");
}

void test_move(void) {
    static int32_t initial_pos[MAP_NUM_DRIVES];
    static bool first_run = true;
    static int execution_count = 0;
    const int num_cycles_to_abort = 1000;
    execution_count++;
    static bool test_pass = false;


    if (first_run) {
        for (int i = 0; i < MAP_NUM_DRIVES; i++) {
            initial_pos[i] = dpm_in->joint_actual_position[i];
//            printf("init pos: %d", initial_pos[i]);
        }

        first_run = false;
    }
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        dpm_out->joint_set_position[i]++;
    }

    int j = 0;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if ((dpm_in->joint_actual_position[i] - initial_pos[i]) > 500) {
            j++;
        }
//        printf("j:%u, init pos:%d, act pos:%d\n", i, dpm_in->joint_actual_position[i], initial_pos[i]);
//        printf("j:%u, pos:%d\n", i, (dpm_in->joint_actual_position[i]-initial_pos[i]));
    }
    if (j == MAP_NUM_DRIVES) {
        test_pass = true;
    }
    if (execution_count > num_cycles_to_abort) {
        test_result.premature_termination = true;
    } else if (test_pass) {
        test_result.ran_to_completion = true;
    }
}

void test_op_enabled(void) {
    static int execution_count = 0;
    const int num_cycles_to_abort = 100;
    static bool sync_pos = true;
    int32_t initial_pos;
    execution_count++;
//    printf("exec count: %d\n", execution_count);
//    printf("Machine status word: %s\n", cia_state_names[cia_statwrd_to_state(dpm_in->machine_word)]);

    /*force statemachine up to Operation Enabled */
    if (dpm_in->machine_word == CIA_FAULT_STATWRD) {
        dpm_out->machine_word = CIA_FAULT_RESET_CTRLWRD;
    }


    if (dpm_in->machine_word == CIA_SWITCH_ON_DISABLED_STATWRD) {
        dpm_out->machine_word = CIA_SHUTDOWN_CTRLWRD;
        if (sync_pos) {
            for (int i = 0; i < MAP_NUM_DRIVES; i++) {
                initial_pos = dpm_in->joint_actual_position[i];
                dpm_out->joint_set_position[i] = initial_pos;
                sync_pos = false;
            }
        }
    }
    if (dpm_in->machine_word == CIA_READY_TO_SWITCH_ON_STATWRD) {
        dpm_out->machine_word = CIA_SWITCH_ON_CTRLWRD;
    }

    if (dpm_in->machine_word == CIA_SWITCHED_ON_STATWRD) {
        dpm_out->machine_word = CIA_ENABLE_OPERATION_CTRLWRD;
    }

    if (execution_count > num_cycles_to_abort) {
        test_result.premature_termination = true;
    } else if (dpm_in->machine_word == CIA_OPERATION_ENABLED_STATWRD) {
        test_result.ran_to_completion = true;
    }
}

void pass_fail() {
    printf("%s: ", test_result.test_name);
    TEST_ASSERT_MESSAGE(test_result.ran_to_completion, "Did not complete in specified number of cycles");
}

int main(void) {

#define GBEM_UM_EN

//        log_run_level =0;
    printf("*** THIS IS HARDWARE IN LOOP TESTING ***\n");
    printf("***    Connect EtheCAT hardware      ***\n");

//>>>test shared mem if we want to
//    establish_shared_mem_con(GBC_PROCESS_NAME);

    int rc;
    int *task_param = (int *) malloc(sizeof(int));
    *task_param = 0;
    //Create RT thread for cyclic task, inside this function the priority and scheduler params are set
    rc = osal_thread_create_rt(&thread_ec_rxtx, STACK64K * 2, &ec_rxtx, (void *) task_param);
    if (rc != 1) {
        LL_FATAL("GBEM: error creating pthread (ec_rxtx)");
    }

    //RT-sensitive
    rc = osal_thread_create_rt(&thread_ec_check, STACK64K * 2, &ec_check, NULL);
    if (rc != 1) {
        LL_FATAL(
                "GBEM: An error occurred whilst creating the pthread (ec_heck which is the thread used to check slave statuses) and GBEM will exit. This error message implies that a Linux system call (pthread_create) has failed. This could be because the system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process would be exceeded. Neither of these should occur normally. Something bad has happend deep down");
    }


    //wait to let the thread_ec_rxtx so startup messages spit out ungarbled
    sleep(2);

    //    ECBoot((void *) task_param);

    m = ctrl_statemachine_alloc();
    ctrl_init_statemachine(m);

    UNITY_BEGIN();
    RUN_TEST(test_boot);
    ec_rxtx_test_mode = true;
    RUN_CYCLIC_TEST(test_op_enabled);
    ec_rxtx_test_mode = false;
    ec_rxtx_test_mode = true;
    RUN_CYCLIC_TEST(test_move);
    ec_rxtx_test_mode = false;
//sample gl_delta
//read slave time diff
//read ecat error

    return UNITY_END();

}
