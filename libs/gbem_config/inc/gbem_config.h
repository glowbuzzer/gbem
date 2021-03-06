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
#define ECRXTX_EXEC_TIME_WARNING_PERCENTAGE             50




/* defines whether redundant Ethernet ports are used. This requires the config to be defined outside of the command line options (NO_COMMAND_LINE_OPTIONS)
 * and the #if section in main to be completed with the parameters */
#define USE_REDUNDANT_ETHERNET_PORTS                    0

/** defines whether the getopt function is run to take command line arguments to GBEM or if the config is hash defined in main */
#define NO_COMMAND_LINE_OPTIONS                         0


/*** GBC CONFIGURATION ***/


/** defines the size of the PDO buffers IN/OUT (overlaid with the dpm structs) */
#define SIZE_OF_GBC_PDO                                 200

/*** *** EMSTAT CONFIGURATION *** ***/

/**time in seconds between writing of the JSON status updates to the filesystem */
#define JSON_STATUS_UPDATE_PERIOD_SECS                  2
/** Delay in starting to output json status and const data cyclically */
#define DELAY_TO_START_JSON_OUTPUT_SECS                 10


/*** *** CTRL CONFIGURATION *** ***/

/** Number of cycles in which the heartbeat received from GBC must be within the one generated on GBEM */
#define CTRL_HEARTBEAT_TOLERANCE                        50



///** Bit number in machine controlword that triggers a transition to fault state */
//#define CTRL_MACHINE_CTRL_WRD_REQUEST_FAULT_BIT_NUM     16

/**Bit number in machine controlword that triggers a reset of drives and statemachine */
#define CTRL_MACHINE_CTRL_WRD_REQUEST_RESET_BIT_NUM     ???

/** Threshold of cycles after requesting drives change state that the transition must be made */
//this has to be quite large as it needs to be > maximum time for the drives to stop
#define CTRL_DRIVE_CHANGE_STATE_TIMEOUT                 100

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1
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

/** Max length of string for gbc process name */
#define GBC_PROCESS_NAME_MAX_LENGTH                     100

/** Max length of string for PLC task names */
#define PLC_MAX_TASK_NAME_MAX_LENGTH                    20

/** Max length of string for ec_state to string array */
#define MAX_EC_STATE_TO_STRING_INDEX                    0x20

/** This is the size of the buffer used to hold the JSON config object - this can become quite large if you have lots of slaves/drives */
#define SIZE_OF_CONFIG_SUMMARY_JSON_BUFFER              100000

/**defines max size of name of the Ethernet interface there - this is defined in net/if.h */
#define SIZE_OF_IF_NAME                                 IFNAMSIZ

/** defines max size of the SOEM iomap data structure - this is a critical define. It defines the size of the iomap - lots of slaves with lots of POO objects will make thsi large*/
#define ECM_IO_MAP_SIZE                                 2048


/*** *** MBED DEFINES *** ***/

#define EC_CHECK_TASK_PRIO                               3


#endif //GBEM_GBEM_CONFIG_H
