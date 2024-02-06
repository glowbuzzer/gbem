
/**
 ******************************************************************************
 * @file           :  ecrxtx.c
 * @brief          :  main cyclic functions for GBEM
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <signal.h>
#include "ethercat.h"
#include "control.h"
#include "log.h"
#include "dpm.h"
#include "ec_functions.h"
#include <unistd.h>
#include "linux_shm.h"
#include "user_message.h"
#include "std_defs_and_macros.h"
#include "plc_core.h"
#include "gberror.h"
#include "cyclic_message.h"
#include "stdlib.h"
#include "ecrxtx.h"
#include "map.h"
#include "ec_functions.h"
#include "main.h"
#include "status_control_word_bit_definitions.h"
#include "linux_shm.h"
#include "print_status.h"

bool homing_failed = false;
extern char proc_name[100];

//var to enable test mode in ec_rxtx
bool ec_rxtx_test_mode = false;

//these affect what gets called in the main cyclic thread and are set by the boot process
ec_rxtx_mode_t ec_rxtx_mode;

// Counters for 32 bit dc time function
int64 ref_dc_time = 0;
int64 last_dc_time_sample = 0;

int64 toff, gl_delta;
struct timeval tv, t1, t2;
volatile int wkc;
int expectedWKC;
uint8 currentgroup = 0;

/**
 * @brief this prints out the contents of ECT_REG_DCSYSDIFF on a specific slave
 * @param slave
 * @return
 *
 * DCSYSDIFF is the difference between the slaves local time and DC time
 *
 * 32 bits
 *
 *
 */
static gberror_t __attribute__((unused)) print_slave_dc_sysdiff(const uint16_t slave) {
    int tdiff = 0;
    int local_wc = ec_FPRD(ec_slave[slave].configadr, ECT_REG_DCSYSDIFF, sizeof(tdiff), &tdiff, EC_TIMEOUTRET);
    if (local_wc == 0) {
        return E_REGISTER_READ_FAILURE;
    }
    if (tdiff < 0) { tdiff = -(int32) ((uint32) tdiff & 0x7ffffff); }
    LL_INFO(GBEM_GEN_LOG_EN, "GBEM: DC Time difference on slave (%d):%d", slave, tdiff);
    return E_SUCCESS;
}


/**
 * @brief calculates the round trip time to exchange process data with slaves
 * @return
 */
static uint32_t __attribute__((unused)) fieldbus_roundtrip(void) {
    ec_timet start, end, diff;
    int wkc_roundtrip __attribute__((unused));
    uint32_t roundtrip_time = 0;

    start = osal_current_time();
    ec_send_processdata();
    wkc_roundtrip = ec_receive_processdata(EC_TIMEOUTRET);
    end = osal_current_time();
    osal_time_diff(&start, &end, &diff);
    roundtrip_time = diff.sec * 1000000 + diff.usec;

    return roundtrip_time;
}


/**
 * @brief prints a smorgasbord of dc timing data
 */
static void __attribute__((unused)) print_dc_timestamps(void) {
    LL_INFO(GBEM_GEN_LOG_EN, "GBEM: Process data cycle %12ld , Wck %3d, DCtime %12ld, dt %12ld\n",
            ecm_status.cycle_count,
            wkc,
            ec_DCtime, gl_delta);
}

/**
 * @brief call this each cycle when the dc time is 32 bit to stored the rollover in a global var
 */
static void __attribute__((unused)) keep_32_bit_dc_time() {
    ref_dc_time += (uint32) ec_DCtime - (uint32) last_dc_time_sample;
    last_dc_time_sample = ec_DCtime;
}

static void add_timespec(struct timespec *ts, int64 addtime) {
    int64 sec, nsec;
    nsec = addtime % NSEC_PER_SEC;
    sec = (addtime - nsec) / NSEC_PER_SEC;
    //    printf("addtime:%lld\n", addtime);
    //    printf("nsec:%lld\n", nsec);
    //    printf("sec:%lld\n", sec);
    //    printf("ts_sec(b):%ld\n", ts->tv_sec);
    //    printf("ts:nsec(b)%ld\n", ts->tv_nsec);
    ts->tv_sec += sec;
    ts->tv_nsec += nsec;
    if (ts->tv_nsec >= NSEC_PER_SEC) {
        nsec = ts->tv_nsec % NSEC_PER_SEC;
        ts->tv_sec += (ts->tv_nsec - nsec) / NSEC_PER_SEC;
        ts->tv_nsec = nsec;
    }
    //    printf("ts_sec(a):%ld\n", ts->tv_sec);
    //    printf("ts:nsec(a)%ld\n", ts->tv_nsec);
    //    printf("\n");
}


/**
 * @brief PI calculation to get GBC time synced to DC time
 * @param reftime
 * @param cycletime
 * @param offsettime
 */
static void ec_sync(int64 reftime, int64 cycletime, int64 *offsettime) {
    static int64 integral = 0;
    int64 delta;
    /* set sync point x ns later than DC sync*/
    delta = (reftime - MAP_GLOBAL_DC_SYNC_OFFSET) % cycletime;
    if (delta > (cycletime / 2)) { delta = delta - cycletime; }
    if (delta > 0) { integral++; }
    if (delta < 0) { integral--; }
    *offsettime = -(delta / 100) - (integral / 20);
    gl_delta = delta;

    //            if (gl_delta>500000){
    //            printf("large +ve delta:%12lld\n", gl_delta);
    //            }
    //    if (gl_delta<(-500000)){
    //        printf("large -ve delta:%12lld\n", gl_delta);
    //    }
}


__attribute__((weak)) void cyclicTest(void) {
    //this function is used by the test framework to link into the ec_rxtx task
}

bool print_repeater_message = false;


/**
 * @brief This is the thread that runs the main cyclic EtherCAT comms
 * @param argument is the process name we will send a cyclic signal to
 */
void ec_rxtx(void *argument) {
    bool first_run = true;
    char *proc __attribute__((unused)) = (char *) argument;
    struct timespec ts;
    struct timespec
    __attribute__((unused)) ts_wall;
    struct timespec tleft;
    int ht;
    int64 cycletime;
    int rc;
    static uint64_t bus_cycle_tick = 0;
    static uint64_t ms_tick = 0;
    struct timespec t_exec_start, t_exec_end;
    gberror_t grc = 0;

#define NUM_CYCLIC_EVENTS 5

    enum {
        CYCLIC_EVENT_OVERRUN,
        CYCLIC_EVENT_TIMEWARN,
        CYCLIC_EVENT_SEND_FAIL,
        CYCLIC_EVENT_NOT_OPMODE,
        CYCLIC_EVENT_GBC_NOT_CONNECTED
    };

    //@formatter:off
    cyclic_event_t __attribute__((unused)) ec_rxtx_event[NUM_CYCLIC_EVENTS] = {
            [CYCLIC_EVENT_OVERRUN] = {.message="The execution time has overrun!", .type=CYCLIC_MSG_ERROR},
            [CYCLIC_EVENT_TIMEWARN] = {.message="The execution time is over safe threshold", .type=CYCLIC_MSG_WARN},
            [CYCLIC_EVENT_SEND_FAIL] = {.message="Send process data over EtherCAT failed", .type=CYCLIC_MSG_ERROR},
            [CYCLIC_EVENT_NOT_OPMODE] = {.message="OPMODE is not set yet", .type=CYCLIC_MSG_INFO},
            [CYCLIC_EVENT_GBC_NOT_CONNECTED] = {.message="GBC is not connected", .type=CYCLIC_MSG_INFO},
    };
    //@formatter:on


    /* Allocated storage for statemachine struct and initialise */
    struct stateMachine *m;
    m = ctrl_statemachine_alloc();
    ctrl_init_statemachine(m);

#if ECRXTX_MEASURE_TIMING == 1
    struct timespec startTime, endTime, lastStartTime = {};
    uint32_t period_ns = 0, exec_ns = 0, latency_ns = 0;
    uint32_t latency_min_ns = 0;
    uint32_t latency_max_ns = 0;
    uint32_t period_min_ns = 0;
    uint32_t period_max_ns = 0;
    uint32_t exec_min_ns = 0;
    uint32_t exec_max_ns = 0;
#endif

    /* The EtherCAT frame handling is split into:
     * ec_send_processdata and ec_receive_processdata.
     * - ec_send_processdata sends the frame on the NIC and saves the frame on the stack for receive to fetch.
     * - ec_receive_processdata(EC_TIMEOUTRET) tries to fetch the frames on the stack.
     * We send an argument for how long we will try to fetch the frame.
     * ec_receive_processdata returns the working counter.
    */

    //record monotonic time
    rc = clock_gettime(CLOCK_MONOTONIC, &ts);
    if (rc != 0) {
        UM_FATAL("GBEM: gettime suffered a nasty error. (system call error message: %s). This is irrecoverable error.",
                 strerror(errno));
    }


#if USE_CLOCK_DIFFERENCE == 1
    //record wall clock time
    clock_gettime(CLOCK_REALTIME, &ts_wall);

    uint32_t clock_difference_sec = (uint32_t) ts_wall.tv_sec - (uint32_t) ts.tv_sec;

    UM_INFO(GBEM_UM_EN, "GBEM: Clock difference between CLOCK_MONOTONIC AND CLOCK_REALTIME, [%d] seconds",
            clock_difference_sec);
    UM_INFO(GBEM_UM_EN, "GBEM: This means we are [%u] years ahead of UNIX epoch (1st Jan 1970)",
            clock_difference_sec / 60 / 60 / 24 / 365);

    ts.tv_sec += (time_t) clock_difference_sec;

    int64 time_check_nsecs = ((int64) ts.tv_sec * (int64) NSEC_PER_SEC) + (int64) ts.tv_nsec;

    UM_INFO(GBEM_UM_EN, "GBEM: Local clock nsec from unix epoch :%" PRId64, time_check_nsecs);

    //need to use the _r version of the ctime call here
    char buf[80];
    ctime_r(&ts.tv_sec, buf);
    buf[strlen(buf) - 1] = '\0';
    UM_INFO(GBEM_UM_EN, "GBEM: Local time (calculated from monotonic) is [%s]", buf);
#endif

    ht = (ts.tv_nsec / 1000000) + 1; /* round to nearest ms */
    ts.tv_nsec = ht * 1000000;

    //this is new to the old code
    if (ts.tv_nsec >= NSEC_PER_SEC) {
        ts.tv_sec++;
        ts.tv_nsec -= NSEC_PER_SEC;
    }

    cycletime = ECM_CYCLE_TIME; /* cycletime in ns */
    toff = 0;

    bool __attribute__((unused)) print_i_am_alive_message = false;

    ec_rxtx_event[CYCLIC_EVENT_NOT_OPMODE].active = true;

    bool time_to_check_gbc = false;


    while (1) {
        bool reminder_message_flag = false;
        bool check_gbc_flag = false;
        bool gbem_i_am_alive_flag = false;

        //RT-sensitive
        for (int i = 0; i < MAP_CYCLE_TIME; i++) {
            ms_tick++;

            if ((ms_tick) % ECRXTX_REMINDER_MESSAGE_INTERVAL_MS == 0) {
                reminder_message_flag = true;
            }
            if ((ms_tick) % ECRXTX_GBC_CONNECTION_CHECK_INTERVAL_MS == 0) {
                check_gbc_flag = true;
            }
            if ((ms_tick) % ECRXTX_GBEM_I_AM_ALIVE_MESSAGE_INTERVAL_MS == 0) {
                gbem_i_am_alive_flag = true;
            }
        }

        print_i_am_alive_message = gbem_i_am_alive_flag ? true : false;
        print_repeater_message = reminder_message_flag ? true : false;
        time_to_check_gbc = check_gbc_flag ? true : false;


        bus_cycle_tick++;
        /* calculate next cycle start */
        add_timespec(&ts, cycletime + toff);


        //this copy does take say 4 useconds... can move into a separate thread but will need another mutex to protect exrxtx writing to ecmstatus
        if (sem_trywait(gbc_named_offline_mem_protection_semaphore) == 0) {
            memcpy(shmp->sm_offline_buf_in, &ecm_status, sizeof(ecm_status_t));
            sem_post(gbc_named_offline_mem_protection_semaphore);
        } else {
            //no need to warn here - we will miss a few cycles of offline mem exchange and that is fine
        }


        // if gbc is not connected AND we are not in test mode AND it is time to try an connect again to GBC
        if (!ecm_status.gbc_connected && !ec_rxtx_test_mode && time_to_check_gbc && ec_rxtx_mode == EC_RXTX_MODE_OP) {
            grc = establish_shared_mem_and_signal_con(&shmp, 1, 1);
            if (grc == E_SUCCESS) {
                UM_INFO(GBEM_UM_EN, "GBEM: Connection to shared memory >successfully< established ");
                memset(shmp->sm_buf_in, 0, sizeof(uint8_t) * SHM_BUF_SIZE);
                memset(shmp->sm_buf_out, 0, sizeof(uint8_t) * SHM_BUF_SIZE);
                ecm_status.gbc_connected = true;
            } else {
                UM_INFO(GBEM_UM_EN, "GBEM: Connection to shared memory  >failed<");
            }
        }

        /* wait to cycle start */
#if USE_CLOCK_DIFFERENCE == 1
        ts.tv_sec -= clock_difference_sec;
#endif
        int ns_rc = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, &tleft);
#if USE_CLOCK_DIFFERENCE == 1
        ts.tv_sec += clock_difference_sec;
#endif
        if (ecm_status.gbc_connected) {
            /* do the memcpy to shared mem */


            if (sem_trywait(gbc_named_mem_protection_semaphore) == 0) {
                memcpy(outA, shmp->sm_buf_out, SIZE_OF_GBC_PDO);
                memcpy(shmp->sm_buf_in, inA, SIZE_OF_GBC_PDO);
                // Release the semaphore when done
                sem_post(gbc_named_mem_protection_semaphore);
            } else {
                // UM_ERROR(GBEM_UM_EN, "GBEM: Shared mem connection busy");
                ecm_status.shared_mem_busy_count++;
            }


            int sem_post_rc = sem_post(gbc_named_trigger_semaphore);

            if (sem_post_rc == -1) {
                UM_FATAL("GBEM: Error posting to named semaphore [%s]. This error is irrecoverable. GBEM will exit",
                         strerror(errno));
                // Handle the error, which could include checking errno for specific details
            }
        }

#if ECRXTX_MEASURE_TIMING == 1
        clock_gettime(CLOCK_MONOTONIC, &startTime);
        latency_ns = DIFF_NS(ts, startTime);
        period_ns = DIFF_NS(lastStartTime, startTime);
        exec_ns = DIFF_NS(lastStartTime, endTime);
        lastStartTime = startTime;

        if (latency_ns > latency_max_ns) {
            latency_max_ns = latency_ns;
        }
        if (latency_ns < latency_min_ns) {
            latency_min_ns = latency_ns;
        }
        if (period_ns > period_max_ns) {
            period_max_ns = period_ns;
        }
        if (period_ns < period_min_ns) {
            period_min_ns = period_ns;
        }
        if (exec_ns > exec_max_ns) {
            exec_max_ns = exec_ns;
        }
        if (exec_ns < exec_min_ns) {
            exec_min_ns = exec_ns;
        }
#endif

        if (ns_rc != 0) {
            UM_FATAL(
                "GBEM: The nanosleep command suffered a nasty error. The system error message was: %s (%d). This error is irrecoverable. GBEM will exit",
                strerror(errno), ns_rc);
        }

        /* use this for a slave at start of chain that has a 32 bit dc time register */
        /* keep_32_bit_dc_time(); */

        //todo should this be here?
        copy_ec_slave_to_ecm_status();

        ecm_status.cycle_count++;

        //this is a weak function used to add test code to the cyclic process

        cyclicTest();

        // ec_rxtx_dorun is set after "Boot step 6 >success< (transition all slaves to SAFE OP state"
        if ((ec_rxtx_mode == EC_RXTX_MODE_DORUN) || (ec_rxtx_mode == EC_RXTX_MODE_OP) ||
            (ec_rxtx_mode == EC_RXTX_MODE_HOME)) {
            //wkc global is set here - it is checked in ec_check

            wkc = ec_receive_processdata(EC_TIMEOUTRET);

#if ECRXTX_MEASURE_TIMING == 1
            // output timing stats
            if (bus_cycle_tick%1000==0) {
                printf("period     %10u ... %10u\n",
                       period_min_ns, period_max_ns);
                printf("exec       %10u ... %10u\n",
                       exec_min_ns, exec_max_ns);
                printf("latency    %10u ... %10u\n",
                       latency_min_ns, latency_max_ns);
                if (bus_cycle_tick == 10000) {
                    printf("zero timing\n");
                period_max_ns = 0;
                period_min_ns = 0xffffffff;
                exec_max_ns = 0;
                exec_min_ns = 0xffffffff;
                latency_max_ns = 0;
                latency_min_ns = 0xffffffff;
            }
            }
#endif


            //ec_rxtx_mode == EC_RXTX_MODE_OP is set after "GBEM: Boot step 9 >success< (transition all slaves to OP state"
            if (ec_rxtx_mode == EC_RXTX_MODE_OP) {
                ec_rxtx_event[CYCLIC_EVENT_NOT_OPMODE].active = false;
                clock_gettime(CLOCK_MONOTONIC, &t_exec_start);
                if (ecm_status.gbc_connected) {
                    //if this is our first run through the cyclic loop we want to tell the main control program so that it can set initial PDOs
                    if (first_run) {
                        ctrl_main(m, first_run);
                        first_run = false;
                    } else {
                        ctrl_main(m, first_run);
                    }

                    //RT-sensitive
#if ENABLE_ALL_NON_CORE_FUNCTIONS == 1
#if ENABLE_PLC == 1
                    plc_task_exec();
#endif
#endif
                }

                //this fills out ec_slave struct with the current state

                ec_readstate();
            } else if (ec_rxtx_mode == EC_RXTX_MODE_HOME) {
                //in homing
                for (int i = 0; i < MAP_NUM_DRIVES; i++) {
                    if (map_drive_run_homing[i]) {
                        if (*map_drive_homing_exec_function_ptr[i] != NULL) {
                            map_drive_get_stat_wrd_function_ptr[i](i);
                        }
                    }
                }
            } else {
                //not in opmode or home
            }


            clock_gettime(CLOCK_MONOTONIC, &t_exec_end);


            rc = ec_send_overlap_processdata();

            if (rc == 0) {
                /* hmmm sending process data failed */
                ec_rxtx_event[CYCLIC_EVENT_SEND_FAIL].active = true;
            } else {
                ec_rxtx_event[CYCLIC_EVENT_SEND_FAIL].active = false;
            }

            if (ec_slave[0].hasdc) {
#if PRINT_DC_TIMESTAMPS == 1
                if (bus_cycle_tick % 5000 == 0) {
                    print_dc_timestamps();
                    //                //should call the last slave in the chain
                    //                print_slave_dc_sysdiff(MAP_EL7211_1);

                    printf("toff:%" PRIi64 "\n", toff);

                }
#endif

                /* calculate toff to get GBC time and DC synced */
                /*use this for a slave at start of chain that has a 32 bit dc time register */
                //                ec_sync(ref_dc_time, cycletime, &toff);
                if (ec_DCtime > 0) {
#if USE_CLOCK_DIFFERENCE == 1
                    ec_sync(ec_DCtime - (int64) (clock_difference_sec * 1000000000), cycletime, &toff);
#else
                    ec_sync(ec_DCtime, cycletime, &toff);
#endif

#ifdef ECRXTX_MEASURE_EXEC_TIME
                    if (ec_rxtx_mode == EC_RXTX_MODE_OP) {
                        //todo this UM printfs are redundant if cyclic messages are enabled - they are just for debugging

                        if (current_state == CIA_OPERATION_ENABLED) {
                            uint64_t exec_time_usec =
                                    (uint64_t) (((t_exec_end.tv_sec * NSEC_PER_SEC) + t_exec_end.tv_nsec) -
                                                ((t_exec_start.tv_sec * NSEC_PER_SEC) + t_exec_start.tv_nsec)) / 1000;


                            //                            if (exec_time_usec > 1000) {
                            //                                UM_INFO(GBEM_UM_EN, "GBEM: Execution time usecs [%]" PRIu64, exec_time_usec);
                            //                                UM_INFO(GBEM_UM_EN, "GBEM: Bus cycle tick [%]" PRIu64, bus_cycle_tick);
                            //                            }

                            /** Here we warn if the exec time (state machine gubbins plus plc jiggerypokery) is more than ur cycle time */

                            if ((uint32_t) exec_time_usec >
                                (uint32_t) (MAP_CYCLE_TIME * 1000 * ECRXTX_EXEC_TIME_ERROR_PERCENTAGE / 100)) {
                                ec_rxtx_event[CYCLIC_EVENT_OVERRUN].active = true;
                                UM_ERROR(GBEM_UM_EN, "GBEM: Execution time [%u] [%llu] (error)",
                                         (uint32_t) exec_time_usec, bus_cycle_tick);
                            } else if ((uint32_t) exec_time_usec >
                                       (uint32_t) (MAP_CYCLE_TIME * 1000 * ECRXTX_EXEC_TIME_WARNING_PERCENTAGE / 100)) {
                                ec_rxtx_event[CYCLIC_EVENT_TIMEWARN].active = true;
                                UM_WARN(GBEM_UM_EN, "GBEM: Execution time [%u] [%llu] (warning)",
                                        (uint32_t) exec_time_usec, bus_cycle_tick);
                            } else {
                                ec_rxtx_event[CYCLIC_EVENT_OVERRUN].active = false;
                                ec_rxtx_event[CYCLIC_EVENT_TIMEWARN].active = false;
                            }
                        }
#endif
                    }
                    //                ec_sync(ec_DCtime, cycletime, &toff);
#if ECRXTX_MEASURE_TIMING == 1
                    clock_gettime(CLOCK_MONOTONIC, &endTime);
#endif
                }
            }

            if (!ecm_status.gbc_connected) {
                ec_rxtx_event[CYCLIC_EVENT_GBC_NOT_CONNECTED].active = true;
            } else {
                ec_rxtx_event[CYCLIC_EVENT_GBC_NOT_CONNECTED].active = false;
            }
            //RT-sensitive
#if ENABLE_CYCLIC_MESSAGES == 1
            if (ms_tick > ECRXTX_DELAY_TO_START_MESSAGES_SEC * 1000) {
                print_cyclic_user_message(NUM_CYCLIC_EVENTS, ec_rxtx_event);
            }
            if (print_i_am_alive_message) {
                UM_INFO(GBEM_UM_EN, "GBEM: Is running. Current bus cycle count is [%llu]", bus_cycle_tick);
            }
#endif
        } else {
            //            UM_INFO(GBEM_UM_EN, "GBEM: No mode set for EC_RXTX. Waiting for mode to be set");

            osal_usleep(MAP_CYCLE_TIME * 1000);
        }
    }
}
