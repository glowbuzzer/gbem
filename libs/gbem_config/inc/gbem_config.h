/**
 ******************************************************************************
 * @file           :  gbem_config.h
 * @brief          :  source for GBEM config hash defines
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

/** >>>>> see also the defines in map.h */

#ifndef GBEM_GBEM_CONFIG_H
#define GBEM_GBEM_CONFIG_H

#include <net/if.h>
#include "gbem_config_autogen.h"

/*** *** GENERAL CONFIGURATIONS *** ***/
#define MAX_NUM_SLAVE_ERROR_MESSAGES                    10
#define ECRXTX_REMINDER_MESSAGE_INTERVAL_MS             10000
#define ECRXTX_GBC_CONNECTION_CHECK_INTERVAL_MS         20000
#define ECRXTX_GBEM_I_AM_ALIVE_MESSAGE_INTERVAL_MS      10000
#define ECRXTX_DELAY_TO_START_MESSAGES_SEC              30

/** Enables measure timing code which logs the timing of the main cyclic EtherCAT comms process to stdout (don't enable when not troubleshooting as it can upset realtimeness) */
#define ECRXTX_MEASURE_TIMING                           0

/** These defines define when a warning occurs in the main EtherCAT cyclic task when processing exceeds a given % of the bus cycle time */
#define ECRXTX_EXEC_TIME_ERROR_PERCENTAGE               80
#define ECRXTX_EXEC_TIME_WARNING_PERCENTAGE             60

/** Enables measure timing code which logs the timing of the main cyclic EtherCAT comms process to stdout (don't enable when not troubleshooting as it can upset realtimeness) */
#define ECRXTX_MEASURE_EXEC_TIME                        0

/** Enables printing of slave 1C32 info to stdout */
#define PRINT_1C32_INFO                                 0


/* defines whether redundant Ethernet ports are used. This requires the config to be defined outside of the command line options (NO_COMMAND_LINE_OPTIONS)
 * and the #if section in main to be completed with the parameters */
#define USE_REDUNDANT_ETHERNET_PORTS                    0

/** defines whether the getopt function is run to take command line arguments to GBEM or if the config is hash defined in main */
#define NO_COMMAND_LINE_OPTIONS                         0


#define LIMITS_INI_FILENAME "./limits.ini"
#define NUM_PARAMS_IN_LIMITS_INI 6
#define DEBUG_INI_FILENAME "./debug.ini"
#define NUM_PARAMS_IN_DEBUG_INI 3


/*** GBC CONFIGURATION ***/


/*** *** CTRL CONFIGURATION *** ***/

/** Number of cycles in which the heartbeat received from GBC must be within the one generated on GBEM */
#define CTRL_HEARTBEAT_TOLERANCE                        50


#define FSOE_STANDARD_OVERALL_SAFETY_STATE_BIT_NUM      0
#define FSOE_STANDARD_ERROR_STATE_BIT_NUM               1
#define FSOE_STANDARD_RESTART_ACK_STATE_BIT_NUM         2

#define FSOE_STANDARD_ERROR_ACK_CMD_BIT_NUM             0
#define FSOE_STANDARD_RESTART_ACK_CMD_BIT_NUM           1
#define FSOE_STANDARD_CONTROL_ALIVE_CMD_BIT_NUM         2


/** Threshold of cycles after requesting drives change state that the transition must be made */
//this has to be quite large as it needs to be > maximum time for the drives to stop
#define CTRL_DRIVE_CHANGE_STATE_TIMEOUT                 5000

///**CRITICAL! if this is defined then hardware estop is disabled! */
//#define DISABLE_ESTOP_CHECKING                          1
/**CRITICAL! if this is defined then heartbeat is disabled! */
#define DISABLE_HEARTBEAT_CHECKING                      0

/** This defines the size of the stack in bytes allocated to the main ecrxtx thread */
#define STACK64K                                        (64 * 1024)

/*** *** ETHERCAT DEFINES *** ***/

/** This defines the global cycle shift */
#define ECM_CYCLE_SHIFT                                 0U

/** ms to ns conversions for cycletime */
#define ECM_CYCLE_TIME                                  (MAP_CYCLE_TIME * 1000000U)

/** Timeout used when trying to re-establish connection to slave */
#define EC_TIMEOUTMON                                   500

/** this is the threshold below which we consider a slave to be in sync */
#define ECM_DC_SYNC_THRESHOLD 5000 //5 usec?

/*** *** SIZES & LENGTHS CONFIGURATION *** ***/

/* Defines for length of strings, buffers etc. */


/** Max length of string for PLC task names */
#define PLC_MAX_TASK_NAME_MAX_LENGTH                    20

/** Defines whether PLC functionality is enabled */
#define ENABLE_PLC                                      1


/** Defines whether all non-core functions are enable */
/** This will disable:
 * PLC
 * EtherCAT error checking thread (thread_ec_check - function ec_check)
 *
 */
#define ENABLE_ALL_NON_CORE_FUNCTIONS                  1

/** Defines whether cyclic messages are outputted to stdout - this will impact the real-time behaviour sometimes quite dramatically*/
#define ENABLE_CYCLIC_MESSAGES                          0

/** Max length of string for ec_state to string array */
#define MAX_EC_STATE_TO_STRING_INDEX                    0x20

/** This is the size of the buffer used to hold the JSON config object - this can become quite large if you have lots of slaves/drives */
#define SIZE_OF_CONFIG_SUMMARY_JSON_BUFFER              100000

/**defines max size of name of the Ethernet interface there - this is defined in net/if.h */
#define SIZE_OF_IF_NAME                                 IFNAMSIZ

/** defines max size of the SOEM iomap data structure - this is a critical define. It defines the size of the iomap - lots of slaves with lots of POO objects will make thsi large*/
#define ECM_IO_MAP_SIZE                                 2048


/** defines the use of the clock difference code in ecrtx - this calculates the difference between DC and local clock */
#define USE_CLOCK_DIFFERENCE                            0

/** defines the nice value for the GBEM process */
#define PROCESS_NICE_VALUE                              10


#endif //GBEM_GBEM_CONFIG_H
