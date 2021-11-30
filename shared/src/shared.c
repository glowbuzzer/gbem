/**
 ******************************************************************************
 * @file           :  shared.c
 * @brief          :  fuck knows what this is doing here
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


/* enums for the state of the different programs that can be run */
//typedef enum {ECM_PRE_BOOT, ECM_BOOT_FINISHED, ECM_CYCLIC_RUNNING, ECM_ERROR} ecm_cyclic_state_t ;
//typedef enum {ECM_NO_PROG, ECM_NET_SCAN_PROG ,ECM_CYCLIC_PROG, ECM_PRINT_CONFIG_PROG, ECM_WRITE_NVRAM_PROG, ECM_NET_SCAN_PDO_PROG} ecm_active_program_t ;
//typedef enum {ECM_NET_SCAN_PRE_START,ECM_NET_SCAN_START, ECM_NET_SCAN_ERROR, ECM_NET_SCAN_NO_SLAVES_FOUND, ECM_NET_SCAN_FINISHED} ecm_net_scan_state_t;


//these must line up with the above typedefs



const char* ecm_cyclic_state_names[] = {"Pre-boot", "Boot finished", "Cyclic running", "Error"};
const char* ecm_active_program_names[6] = {"None", "Net scan", "Cyclic", "Print config", "Write NVRAM", "Netscan with PDO map"};
const char* ecm_net_scan_names[] = {"Pre-start", "Started", "Error", "No slaves found", "Finished"};

