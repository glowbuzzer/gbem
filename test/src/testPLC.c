
/**
 ******************************************************************************
 * @file           :  testPLC.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "unity.h"
#include "plc_core.h"
#include "gberror.h"
#include "stdlib.h"
#include "stdbool.h"
#include "shared.h"
#include "test.h"
#include "plc_stdlib.h"
#include "unistd.h"

bool ec_printSDO = false;



uint8_t inA[SIZE_OF_GBC_PDO];
uint8_t inB[SIZE_OF_GBC_PDO];
uint8_t outA[SIZE_OF_GBC_PDO];
uint8_t outB[SIZE_OF_GBC_PDO];


bool test1 = false;
bool test2 = false;
bool test3 = false;

//pthread_mutex_t plc_task_lock = PTHREAD_MUTEX_INITIALIZER;

void setUp(void) {
//unity

    internal_Cleardown();
}

void tearDown(void) {
//    unity
    // clean stuff up here
}

void *test_task1(void *argument) {

    int task_index = (int) ((intptr_t) argument & 0xFFFFFFFF);
//          sleep(1);
//     printf("task_index in test_task1 [%d]\n", task_index);

    while (1) {
        pthread_mutex_lock(&plc_task_lock);
        pthread_cond_wait(&plc_task_set.tasks[task_index].cond, &plc_task_lock);
//        printf("task1\n");
        test1 = true;
        pthread_mutex_unlock(&plc_task_lock);
        plc_task_set.tasks[task_index].exec.completed=true;
    }

}

void *test_task2(void *argument) {

    int task_index = (int) ((intptr_t) argument);

//    sleep(2);
//printf("task_index in test_task2 [%d]\n", task_index);

    while (1) {
        pthread_mutex_lock(&plc_task_lock);
        pthread_cond_wait(&plc_task_set.tasks[task_index].cond, &plc_task_lock);

//        printf("task2\n");
        test2 = true;
        pthread_mutex_unlock(&plc_task_lock);
        plc_task_set.tasks[task_index].exec.completed=true;
    }

}

void *test_task3(void *argument) {

    int task_index = (int) ((intptr_t) argument & 0xFFFFFFFF);
//        sleep(3);
//    printf("task_index in test_task3 [%d]\n", task_index);

    while (1) {
        pthread_mutex_lock(&plc_task_lock);
        pthread_cond_wait(&plc_task_set.tasks[task_index].cond, &plc_task_lock);


//        printf("task3\n");

        test3 = true;

        pthread_mutex_unlock(&plc_task_lock);
        plc_task_set.tasks[task_index].exec.completed=true;
    }
}

void *test_task4(void *argument) {
    PLC_TASK_START

    printf("task4\n");

    sleep(9);

    PLC_TASK_END

}


void test_plc_suspend_task (void){

    memset(&plc_task_set, 0, sizeof(plc_task_set_t));
    plc_register_tasks(test_task1, 12, 3, "Task 1 - main");

    TEST_ASSERT_EQUAL_INT(PLC_TASK_STATE_RUNNABLE, plc_task_set.tasks[0].state);

    plc_suspend_task("Task 1 - main");

    TEST_ASSERT_EQUAL_INT(PLC_TASK_STATE_SUSPENDED, plc_task_set.tasks[0].state);

    plc_enable_task("Task 1 - main");

    TEST_ASSERT_EQUAL_INT(PLC_TASK_STATE_RUNNABLE, plc_task_set.tasks[0].state);

}

void test_plc_init(void) {
    memset(&plc_task_set, 0, sizeof(plc_task_set_t));

    //we have broken apart the init function a bit here
    plc_register_tasks(test_task1, 12, 3, "Task 1 - main");
    plc_register_tasks(test_task2, 6, 1, "Task 2 - main");
    plc_register_tasks(test_task3, 4, 2, "Task 3 - main");
    //sort task array by priority
    qsort(plc_task_set.tasks, plc_task_set.num_tasks_defined, sizeof(plc_task_def_t), plc_task_set_comparator);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, plc_task_set.tasks[0].priority, "Priority of first task in array is not 1");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(3, plc_task_set.tasks[2].priority, "Priority of third task in array is not 3");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Task 3 - main", plc_task_set.tasks[1].name,
                                     "Name of second task in array mismatch");
//requires PLC_MAX_NUM_TASKS to be 3
    TEST_ASSERT_EQUAL_INT_MESSAGE(E_ARRAY_OVERFLOW, plc_register_tasks(test_task4, 12, 2, "Task 4 - main"),
                                  "Register task does not overflow");


}


//[0]plc_register_tasks(test_task2, 6, 1, "Task 2 - main");
//[1]plc_register_tasks(test_task3, 12, 2, "Task 3 - main");
//[2]plc_register_tasks(test_task1, 4, 3, "Task 1 - main");


void test_plc_task_exec(void) {
    memset(&plc_task_set, 0, sizeof(plc_task_set_t));

    //broken appart plc_task_exec function here
    plc_register_tasks(test_task1, 12, 3, "Task 1");
    plc_register_tasks(test_task2, 6, 1, "Task 2");
    plc_register_tasks(test_task3, 4, 2, "Task 3");
    //sort task array by priority
    qsort(plc_task_set.tasks, plc_task_set.num_tasks_defined, sizeof(plc_task_def_t), plc_task_set_comparator);

    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {
        int error = pthread_create(&plc_task_set.tasks[i].id, NULL, plc_task_set.tasks[i].task_fun,
                                   (void *) i);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
    }


//    if (pthread_mutex_init(&plc_task_lock, NULL) != 0) {
//        printf("\n mutex init failed\n");
//
//    }

    sleep(1);

    TEST_ASSERT_EQUAL_INT(false, test1);
    TEST_ASSERT_EQUAL_INT(false, test2);
    TEST_ASSERT_EQUAL_INT(false, test3);

    plc_task_exec();
    sleep(1);

    TEST_ASSERT_EQUAL_INT(false, test1);
    TEST_ASSERT_EQUAL_INT(false, test2);
    TEST_ASSERT_EQUAL_INT(true, test3);

    plc_task_exec();
    sleep(1);

    TEST_ASSERT_EQUAL_INT(false, test1);
    TEST_ASSERT_EQUAL_INT(true, test2);
    TEST_ASSERT_EQUAL_INT(true, test3);

    plc_task_exec();
    sleep(1);

    TEST_ASSERT_EQUAL_INT(true, test1);
    TEST_ASSERT_EQUAL_INT(true, test2);
    TEST_ASSERT_EQUAL_INT(true, test3);



//    pthread_join(plc_task_set.tasks[0].id, NULL);
//    pthread_join(plc_task_set.tasks[1].id, NULL);
//    pthread_join(plc_task_set.tasks[2].id, NULL);


}



bool mydin1_test, mydin2_test, mydout1_test, mydout2_test, plc_din1;

void test_plc_process_io(void) {

    memset(&plc_task_set, 0, sizeof(plc_task_set_t));
    plc_register_tasks(test_task1, 12, 3, "test_task1");

//    ec_pdo_set_input_bit_rev(MAP_EL1008_1, 1, true);
//    plc_process_iomap_in(0);
//    TEST_ASSERT(mydin1_test == true);
//
//    ec_pdo_set_input_bit_rev(MAP_EL1008_1, 1, false);
//    plc_process_iomap_in(0);
//    TEST_ASSERT(mydin1_test == false);

    TEST_ASSERT(cs_pdo_get_output_bit_rev(MAP_EL2008_1, 1) == false);
    mydout1_test=true;

    plc_process_iomap_out(0);
    TEST_ASSERT(cs_pdo_get_output_bit_rev(MAP_EL2008_1, 1) == true);






}


void test_plc_task_overrun(void){

    memset(&plc_task_set, 0, sizeof(plc_task_set_t));

    //broken appart plc_task_exec function here
    plc_register_tasks(test_task4, 8, 1, "Task 4");
    //sort task array by priority
    qsort(plc_task_set.tasks, plc_task_set.num_tasks_defined, sizeof(plc_task_def_t), plc_task_set_comparator);

    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {
        int error = pthread_create(&plc_task_set.tasks[i].id, NULL, plc_task_set.tasks[i].task_fun,
                                   (void *) i);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
    }


    plc_task_exec();
    sleep(2);
    plc_task_exec();
    sleep(2);
    plc_task_exec();
    sleep(2);
    plc_task_exec();
    sleep(2);
    plc_task_exec();

    TEST_ASSERT_EQUAL_INT(1, plc_task_has_overrun);

}



void test_plc_task_time_calc(void){


    memset(&plc_task_set, 0, sizeof(plc_task_set_t));

    //broken appart plc_task_exec function here
    plc_register_tasks(test_task4, 8, 1, "Task 4");
    //sort task array by priority
    qsort(plc_task_set.tasks, plc_task_set.num_tasks_defined, sizeof(plc_task_def_t), plc_task_set_comparator);

    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {
        int error = pthread_create(&plc_task_set.tasks[i].id, NULL, plc_task_set.tasks[i].task_fun,
                                   (void *) i);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
    }


    plc_task_exec();
    sleep(1);
    plc_task_exec();
    sleep(1);
    plc_task_exec();

    sleep(2);
//
//    printf("cycle time us - %u\n", plc_task_set.tasks[0].exec.last_cycle_time_us);
    TEST_ASSERT_UINT_WITHIN(10000, 1000000,plc_task_set.tasks[0].exec.last_cycle_time_us );

}

void test_ton(void) {
    bool x1 = true;
    static ton_t myton1 = {.pt=12};

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_INT(0, myton1.q);
        plcsl_ton(&myton1, x1);
        plc_bus_cycle_counter++;
        plc_ms_counter = plc_ms_counter + gbem_ctx.map_cycle_time;
    }
    TEST_ASSERT_EQUAL_INT(1, myton1.q);
    plc_ms_counter = plc_ms_counter + gbem_ctx.map_cycle_time;
    TEST_ASSERT_EQUAL_INT(1, myton1.q);
}


void test_rtrig(void) {
    static r_trig_t myrtrig1;
    bool x1 = false;
    plcsl_r_trig(&myrtrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myrtrig1.q);
    plcsl_r_trig(&myrtrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myrtrig1.q);
    x1 = true;
    plcsl_r_trig(&myrtrig1, x1);
    TEST_ASSERT_EQUAL_INT(1, myrtrig1.q);
    plcsl_r_trig(&myrtrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myrtrig1.q);
    x1 = false;
    plcsl_r_trig(&myrtrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myrtrig1.q);
    x1 = true;
    plcsl_r_trig(&myrtrig1, x1);
    TEST_ASSERT_EQUAL_INT(1, myrtrig1.q);
    plcsl_r_trig(&myrtrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myrtrig1.q);
}


void test_ftrig(void) {
    static f_trig_t myftrig1;
    bool x1 = false;
    plcsl_f_trig(&myftrig1, x1);
//    f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myftrig1.q);
    x1 = true;
    plcsl_f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myftrig1.q);
    x1 = true;
    plcsl_f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myftrig1.q);
    x1 = false;
    plcsl_f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(1, myftrig1.q);
    x1 = false;
    plcsl_f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myftrig1.q);
    x1 = true;
    plcsl_f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myftrig1.q);
    x1 = true;
    plcsl_f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myftrig1.q);
    x1 = false;
    plcsl_f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(1, myftrig1.q);
    x1 = false;
    plcsl_f_trig(&myftrig1, x1);
    TEST_ASSERT_EQUAL_INT(0, myftrig1.q);


}


void test_ctu(void) {
    static ctu_t myctu1 = {.pv=3};
    bool x1 = false;
    bool x2 = false;

    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);
    plcsl_ctu(&myctu1, x1, x2);
    plcsl_ctu(&myctu1, x1, x2);
    plcsl_ctu(&myctu1, x1, x2);
    plcsl_ctu(&myctu1, x1, x2);
    plcsl_ctu(&myctu1, x1, x2);
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);
    TEST_ASSERT_EQUAL_INT(0, myctu1.cv);

    //1
    x1 = true;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);
    x1 = false;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);
    TEST_ASSERT_EQUAL_INT(1, myctu1.cv);

    //2
    x1 = true;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);
    x1 = false;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);
    TEST_ASSERT_EQUAL_INT(2, myctu1.cv);

//3
    x1 = true;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);
    x1 = false;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);
    TEST_ASSERT_EQUAL_INT(3, myctu1.cv);

    //4
    x1 = true;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(1, myctu1.q);
    x1 = false;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(1, myctu1.q);
    TEST_ASSERT_EQUAL_INT(3, myctu1.cv);

    //5
    x1 = true;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(1, myctu1.q);
    x1 = false;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(1, myctu1.q);
    TEST_ASSERT_EQUAL_INT(3, myctu1.cv);


    x1 = false;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(1, myctu1.q);

    x1 = false;
    x2 = true;
    plcsl_ctu(&myctu1, x1, x2);
    TEST_ASSERT_EQUAL_INT(0, myctu1.cv);
    TEST_ASSERT_EQUAL_INT(0, myctu1.q);


}


void test_integrate(void) {

    static integrate_t myintegrate1 = {.out_max=0.055, .k=1};
    bool x1 = true;
    bool x2 = false;

    for (int i = 0; i < 10; i++) {
        plcsl_integrate(&myintegrate1, x1, 1.0, x2);
        plc_ms_counter = plc_ms_counter + gbem_ctx.map_cycle_time;
    }
    TEST_ASSERT_EQUAL_DOUBLE(0.036, myintegrate1.integral);

    x2 = true;
    plcsl_integrate(&myintegrate1, x1, 1.0, x2);
    plc_ms_counter = plc_ms_counter + gbem_ctx.map_cycle_time;
    TEST_ASSERT_EQUAL_DOUBLE(0, myintegrate1.integral);
    x2 = false;


    myintegrate1.out_max = 0.030;
    for (int i = 0; i < 10; i++) {

        plcsl_integrate(&myintegrate1, x1, 1.0, x2);
        plc_ms_counter = plc_ms_counter + gbem_ctx.map_cycle_time;
    }
    TEST_ASSERT_EQUAL_DOUBLE(0.030, myintegrate1.integral);

}

void test_clock_pulse(void) {
    static clock_pulse_t myclock_pulse1 = {.pt = 16};
    int n1 = 0;
    bool x2 = false;


    for (int i = 0; i < 2000; i++) {
        plc_ms_counter = plc_ms_counter + gbem_ctx.map_cycle_time;
        plcsl_clock_pulse(&myclock_pulse1, n1, x2);
        printf("%u\n", myclock_pulse1.q);
    }


}


void test_differentiate(void) {

    static differentiate_t mydifferentiate1 = {.k=1};
    bool x1 = true;
    bool x2 = false;

    for (int i = 0; i < 100; i++) {

        plcsl_differentiate(&mydifferentiate1, x1, i);
        plc_ms_counter = plc_ms_counter + gbem_ctx.map_cycle_time;
    }

    TEST_ASSERT_EQUAL_DOUBLE(250.0, mydifferentiate1.differential);
}


void test_tp(void) {
    bool x1 = false;

    static tp_t mytp1 = {.pt = 5};

    plcsl_tp(&mytp1, x1);
    x1 = true;
    plc_ms_counter = plc_ms_counter + 1;
    plcsl_tp(&mytp1, x1);
    TEST_ASSERT_EQUAL_INT(1, mytp1.q);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(1, mytp1.q);
        plcsl_tp(&mytp1, x1);

        plc_ms_counter = plc_ms_counter + 1;
    }
    TEST_ASSERT_EQUAL_UINT32(0, mytp1.q);
    TEST_ASSERT_EQUAL_UINT32(5, mytp1.et);

    x1 = false;
    plc_ms_counter = plc_ms_counter + 1;
    plcsl_tp(&mytp1, x1);
    TEST_ASSERT_EQUAL_UINT32(0, mytp1.et);

}


int main(void) {
    UNITY_BEGIN();
    UNITY_EXEC_TIME_START();

//    RUN_TEST(test_plc_init);
//    RUN_TEST(test_plc_suspend_task);
    RUN_TEST(test_plc_process_io);
//
//    RUN_TEST(test_plc_task_exec);
//    RUN_TEST(test_plc_task_overrun);

//    RUN_TEST(test_plc_task_time_calc);

//RUN_TEST(test_ton);
//RUN_TEST(test_rtrig);
//RUN_TEST(test_ctu);
//RUN_TEST(test_integrate);
//RUN_TEST(test_ftrig);
//RUN_TEST(test_differentiate);
//RUN_TEST(test_clock_pulse);
//RUN_TEST(test_tp);

    UNITY_EXEC_TIME_STOP();
    return UNITY_END();

}

