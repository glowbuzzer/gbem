/**
 ******************************************************************************
 * @file           :  plc_core.h
 * @brief          :  Core PLC functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_CYCLIC_FUNCTIONS_H
#define GBEM_CYCLIC_FUNCTIONS_H
#include "gberror.h"
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "gbem_config.h"



extern bool plc_task_has_overrun;


void plc_task_prefix(int task_index,  struct timespec * start_time );
void plc_task_suffix(int task_index,  struct timespec * end_time );


#define PLC_TASK_START  struct timespec task_start_time; \
int task_index =  (int) ((uintptr_t) argument & 0xFFFFFFFF);\
while (1) { \
plc_task_prefix(task_index, &task_start_time); \


#define PLC_TASK_END  plc_task_suffix(task_index, &task_start_time); \
}\



extern bool plc_signalled_error;
extern pthread_mutex_t plc_task_lock;

//typdef for function pointer
typedef gberror_t (*task_fun_t)(void);


typedef enum {PLC_TASK_STATE_RUNNABLE, PLC_TASK_STATE_SUSPENDED} plc_task_state_t;

typedef struct {
    uint32_t last_cycle_time_us;
    uint32_t max_cycle_time_us;
    bool completed;
}plc_task_exec_t;

/** main struct for defining PLC tasks */
typedef struct {
    char name[PLC_MAX_TASK_NAME_MAX_LENGTH];
    uint8_t cycle_time;
//    task_fun_t task_fun;
    void * task_fun;
    uint8_t priority;
    plc_task_exec_t exec;
    pthread_cond_t cond;
    pthread_t id;
    plc_task_state_t state;
}plc_task_def_t;


#define PLC_MAX_NUM_TASKS 3
typedef struct {
    plc_task_def_t tasks[PLC_MAX_NUM_TASKS];
    plc_task_exec_t task_exec[PLC_MAX_NUM_TASKS];
    uint8_t num_tasks_defined;
}plc_task_set_t;


extern uint64_t plc_bus_cycle_counter;
extern uint64_t plc_ms_counter;
extern plc_task_set_t plc_task_set;


void plc_process_iomap_in(uint8_t task_index);
void plc_process_iomap_out(uint8_t task_index);


gberror_t plc_init(void);
gberror_t plc_register_tasks (void * function, uint8_t cycle_time, uint8_t priority, const char *name);
int plc_task_set_comparator(const void *v1, const void *v2);
void plc_task_exec(void);
void plc_suspend_task (const char * name);
void plc_enable_task (const char * name);
uint8_t plc_task_name_to_task_index(const char* name);

#endif //GBEM_CYCLIC_FUNCTIONS_H
