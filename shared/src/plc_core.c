
/**
 ******************************************************************************
 * @file           :  plc_core.c
 * @brief          :  Core PLC functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "gberror.h"
#include "ethercatsetget.h"
#include "string.h"
#include "user_message.h"
#include "stdlib.h"
#include "stdio.h"
#include "plc_core.h"
#include <time.h>
#include "std_defs_and_macros.h"
#include "dpm.h"
#include "plc_init.h"
#include <pthread.h>
#include "log.h"
#include "iomap_processing.h"


plc_task_set_t plc_task_set;
uint64_t plc_bus_cycle_counter = 0;
uint64_t plc_ms_counter = 0;
bool plc_signalled_error = 0;
pthread_mutex_t plc_task_lock = PTHREAD_MUTEX_INITIALIZER;
bool plc_task_has_overrun = false;


void plc_force_plc_in(const uint8_t task_index) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        //test if we are processing the task this cycle
        if (map_iomap[i].plc.private_linked_task_index == task_index) {
            if (map_iomap[i].plc.inout == MAP_IN) {
                //we have a plc entry
                if (map_iomap[i].force.active)
                    iomap_set_plc_in_union(map_iomap[i].plc.datatype, map_iomap[i].plc.io, map_iomap[i].force.val);
            }
        }
    }
}

void plc_process_iomap_out(const uint8_t task_index) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        /* look for rows matching
          * PDO  | GBC  | PLC
          * out  | none | out - pdo(out)=plc(out)
          * none | in   | out - gbc(in) = plc(out)
          * out  | out  | out - pdo(out)=plc(out)
          */
        //test if we are processing the task this cycle
        if (map_iomap[i].plc.private_linked_task_index == task_index) {
            if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_IN &&
                map_iomap[i].plc.inout == MAP_OUT) {
                //gbc(in) = plc(out)
                switch (map_iomap[i].gbc.datatype) {
                    case ECT_BOOLEAN:
                        if (*((bool *) map_iomap[i].plc.io)) {
                            BIT_SET(dpm_in->digital, map_iomap[i].gbc.ionum);
                        } else {
                            BIT_CLEAR(dpm_in->digital, map_iomap[i].gbc.ionum);
                        }
                        break;
                    case ECT_INTEGER32:
                        dpm_out->integer32[map_iomap[i].gbc.ionum] = *((int32_t *) map_iomap[i].plc.io);
                        break;
                    case ECT_UNSIGNED32:
                        dpm_out->unsigned32[map_iomap[i].gbc.ionum] = *((uint32_t *) map_iomap[i].plc.io);
                        break;
                    case ECT_REAL32:
                        dpm_out->analog[map_iomap[i].gbc.ionum] = *((float *) map_iomap[i].plc.io);
                        break;
                    default:
                        break;
                }
            }
            if ((map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
                 map_iomap[i].plc.inout == MAP_OUT) ||
                (map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_OUT &&
                 map_iomap[i].plc.inout == MAP_OUT)) {
                // pdo(out)=plc(out)
                switch (map_iomap[i].plc.datatype) {
                    case ECT_BOOLEAN:
                        iomap_set_pdo_out_bool(map_iomap[i].pdo.byte_slave,
                                               map_iomap[i].pdo.slave_num,
                                               map_iomap[i].pdo.byte_num,
                                               map_iomap[i].pdo.bit_num,
                                               *((bool *) map_iomap[i].plc.io));
                        break;
                    case ECT_INTEGER32:
                        iomap_set_pdo_out_int32(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                map_iomap[i].pdo.byte_num, *((int32_t *) map_iomap[i].plc.io),
                                                (float) map_iomap[i].pdo.max_val);
                        break;
                    case ECT_UNSIGNED32:
                        iomap_set_pdo_out_uint32(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                 map_iomap[i].pdo.byte_num, *((uint32_t *) map_iomap[i].plc.io),
                                                 (float) map_iomap[i].pdo.max_val);
                        break;
                    case ECT_REAL32:
                        iomap_set_pdo_out_float(map_iomap[i].pdo.datatype, map_iomap[i].pdo.slave_num,
                                                map_iomap[i].pdo.byte_num, *((float *) map_iomap[i].plc.io),
                                                (float) map_iomap[i].pdo.max_val);
                        break;
                    default:
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid iomap datatype in row [%u]", i);
                        break;
                } //switch
            }//ifs
        } //for loop
    }
}

void plc_process_iomap_in(const uint8_t task_index) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        /* look for rows matching
          * PDO  | GBC  | PLC
          * in   | none | in  - plc(in) = pdo(in)
          * none | out  | in  - plc(in) = gbc(out)
          * in   | in   | in - plc(in) = pdo(in)
          */
        //test if we are processing the task this cycle
        if (map_iomap[i].plc.private_linked_task_index == task_index) {

            if ((map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
                 map_iomap[i].plc.inout == MAP_IN) ||
                (map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_IN &&
                 map_iomap[i].plc.inout == MAP_IN)) {
                //plc(in) = pdo(in)
                switch (map_iomap[i].plc.datatype) {
                    case ECT_BOOLEAN:
                        *((bool *) map_iomap[i].plc.io) = iomap_get_pdo_in_bool(map_iomap[i].pdo.byte_slave,
                                                                                map_iomap[i].pdo.slave_num,
                                                                                map_iomap[i].pdo.byte_num,
                                                                                map_iomap[i].pdo.bit_num);
                        break;
                    case ECT_INTEGER32:
                        *((int32_t *) map_iomap[i].plc.io) = iomap_get_pdo_in_int32(map_iomap[i].pdo.datatype,
                                                                                    map_iomap[i].pdo.slave_num,
                                                                                    map_iomap[i].pdo.byte_num);
                        break;

                    case ECT_UNSIGNED32:
                        *((uint32_t *) map_iomap[i].plc.io) = iomap_get_pdo_in_uint32(map_iomap[i].pdo.datatype,
                                                                                      map_iomap[i].pdo.slave_num,
                                                                                      map_iomap[i].pdo.byte_num);
                        break;
                    case ECT_REAL32:
                        *((float *) map_iomap[i].plc.io) = iomap_get_pdo_in_float(map_iomap[i].pdo.datatype,
                                                                                  map_iomap[i].pdo.slave_num,
                                                                                  map_iomap[i].pdo.byte_num,
                                                                                  (float) map_iomap[i].pdo.max_val);
                        break;
                    default:
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid iomap datatype in row [%u]", i);
                        break;
                }
            }
            if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_OUT &&
                map_iomap[i].plc.inout == MAP_IN) {
                //plc(in) = gbc(out)

                switch (map_iomap[i].plc.datatype) {
                    case ECT_BOOLEAN:
                        if (BIT_CHECK(dpm_out->digital, map_iomap[i].gbc.ionum)) {
                            *((bool *) map_iomap[i].plc.io) = true;
                        } else {
                            *((bool *) map_iomap[i].plc.io) = false;
                        }
                        break;
                    case ECT_INTEGER32:
                        *((int32_t *) map_iomap[i].plc.io) = dpm_in->integer32[map_iomap[i].gbc.ionum];
                        break;
                    case ECT_UNSIGNED32:
                        *((uint32_t *) map_iomap[i].plc.io) = dpm_in->unsigned32[map_iomap[i].gbc.ionum];
                        break;
                    case ECT_REAL32:
                        *((float *) map_iomap[i].plc.io) = dpm_in->analog[map_iomap[i].gbc.ionum];
                        break;
                    default:
                        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid iomap datatype in row [%u]", i);
                        break;

                }//if var is linked to this task
            }


        } //if task index matches
    } //for loop
}

/**
 * @brief register a PLC task with the PLC core
 * @param function a function for the task (actually a pthread)
 * @param cycle_time time interval at which the task will be called
 * @param priority priority with which the task will be called
 * @param name name of the task
 * @return E_SUCCESS or other gb error code
 */
gberror_t plc_register_tasks(void *function, const uint32_t cycle_time, const uint8_t priority, const char *name) {

    if (plc_task_set.num_tasks_defined >= PLC_MAX_NUM_TASKS) {
        return E_ARRAY_OVERFLOW;
    }
    strcpy(plc_task_set.tasks[plc_task_set.num_tasks_defined].name, name);
    plc_task_set.tasks[plc_task_set.num_tasks_defined].task_fun = function;
    plc_task_set.tasks[plc_task_set.num_tasks_defined].cycle_time = cycle_time;
    plc_task_set.tasks[plc_task_set.num_tasks_defined].priority = priority;
    plc_task_set.tasks[plc_task_set.num_tasks_defined].exec.completed = true;
    plc_task_set.tasks[plc_task_set.num_tasks_defined].state = PLC_TASK_STATE_RUNNABLE;
    pthread_cond_init(&plc_task_set.tasks[plc_task_set.num_tasks_defined].cond, NULL);

    plc_task_set.num_tasks_defined++;
    return E_SUCCESS;
}


void plc_task_prefix(const int task_index, struct timespec *start_time) {

    pthread_mutex_lock(&plc_task_lock);
    pthread_cond_wait(&plc_task_set.tasks[task_index].cond, &plc_task_lock);
    plc_task_set.tasks[task_index].exec.completed = false;
    clock_gettime(CLOCK_MONOTONIC, start_time);

    plc_process_iomap_in(task_index);
#if PLC_ENABLE_FORCING == 1
    plc_force_plc_in(task_index);
#endif
}

void plc_task_suffix(const int task_index, struct timespec *start_time) {
    struct timespec task_end_time;

    plc_process_iomap_out(task_index);
    pthread_mutex_unlock(&plc_task_lock);
    clock_gettime(CLOCK_MONOTONIC, &task_end_time);
    plc_task_set.tasks[task_index].exec.last_cycle_time_us = DIFF_NS(*start_time, task_end_time) / 1000;

    if (plc_task_set.tasks[task_index].exec.last_cycle_time_us >
        plc_task_set.tasks[task_index].exec.max_cycle_time_us) {
        plc_task_set.tasks[task_index].exec.max_cycle_time_us = plc_task_set.tasks[task_index].exec.last_cycle_time_us;
    }

    plc_task_set.tasks[task_index].exec.completed = true;
}


/**
 * @brief executes (released semaphore) all plc tasks based on their registered interval and priority
 */
void plc_task_exec(void) {

    plc_bus_cycle_counter = plc_bus_cycle_counter + MAP_CYCLE_TIME;
    static uint64_t ms_counter = 0;

    plc_ms_counter++;

    bool trigger_task[PLC_MAX_NUM_TASKS] = {0};
    for (int i = 0; i < MAP_CYCLE_TIME; i++) {
        ms_counter++;
        for (int j = 0; j < plc_task_set.num_tasks_defined; j++) {
            if (ms_counter % plc_task_set.tasks[j].cycle_time == 0) {
                if (plc_task_set.tasks[j].state == PLC_TASK_STATE_RUNNABLE) {
                    trigger_task[j] = true;
                }
            }
        }
    }
//    printf("Bus cycle count [%llu]\n", plc_bus_cycle_counter);

    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {
        if (trigger_task[i]) {
            if (!plc_task_set.tasks[i].exec.completed) {
                UM_ERROR(GBEM_UM_EN, "GBEM: PLC task has overrun task [%d]", i);
                plc_task_has_overrun = true;
                //todo decide what to do here
            }
            pthread_cond_signal(&plc_task_set.tasks[i].cond);
        }
    }


}

/**
 * @brief suspends a task based on its name
 * @param name
 */
void plc_suspend_task(const char *name) {
    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {
        if (!strcmp(plc_task_set.tasks[i].name, name)) {
            plc_task_set.tasks[i].state = PLC_TASK_STATE_SUSPENDED;
        }
    }
}

/**
 * @brief enables a task based on its name
 * @param name
 */
void plc_enable_task(const char *name) {
    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {
        if (!strcmp(plc_task_set.tasks[i].name, name)) {
            plc_task_set.tasks[i].state = PLC_TASK_STATE_RUNNABLE;
        }
    }
}


/**
 * @brief qsort comparator function to sort array of plc_task_def_t by priority
 * @param v1
 * @param v2
 * @return
 */
int plc_task_set_comparator(const void *v1, const void *v2) {
    const plc_task_def_t *p1 = (plc_task_def_t *) v1;
    const plc_task_def_t *p2 = (plc_task_def_t *) v2;
    if (p1->priority < p2->priority)
        return -1;
    else if (p1->priority > p2->priority)
        return +1;
    else
        return 0;
}


void plc_saved_retained_vars(void) {
//must be saved one per task
//    FILE * filew= fopen("output", "wb");
//    if (filew != NULL) {
//        fwrite(&object, sizeof(struct date), 1, filew);
//        fclose(filew);
//    }
//mmap ??

}

void plc_load_retained_vars(void) {

//    struct date object2 = {0};
//    FILE * filer= fopen("output", "rb");
//    if (filer != NULL) {
//        fread(&object2, sizeof(struct date), 1, filer);
//        fclose(filer);
//    }
//)
}

uint8_t plc_task_name_to_task_index(const char *name) {
    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {
        if (!strcmp(plc_task_set.tasks[i].name, name)) {
            return (uint8_t) i;
        }
    }
    return 0;
}

/**
 * @brief initialise the PLC, must be called before cyclic task is executed
 */
gberror_t plc_init(void) {

    //call user code function that actually contains the task definitions
    plc_register_tasks_user();

    //sort task array by priority
    qsort(plc_task_set.tasks, plc_task_set.num_tasks_defined, sizeof(plc_task_def_t), plc_task_set_comparator);

    for (int i = 0; i < map_num_rows_in_iomap; i++) {

        if (map_iomap[i].plc.linked_task_name[0] != '\0') {
            map_iomap[i].plc.private_linked_task_index = plc_task_name_to_task_index(map_iomap[i].plc.linked_task_name);
        }
    }
    //make sure the threads themselves know their position in the array by passing in the void * argument to the task
    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {

        int error = pthread_create(&plc_task_set.tasks[i].id, NULL, plc_task_set.tasks[i].task_fun,
                                   (void *) i);
        if (error != 0) {
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Thread can't be created : [%s]", strerror(error));
            return E_THREAD_CREATE_FAIL;
        }
    }
    return E_SUCCESS;
}