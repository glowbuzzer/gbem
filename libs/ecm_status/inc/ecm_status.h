/**
 ******************************************************************************
 * @file           :  ecm_status.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_ECM_STATUS_H
#define GBEM_ECM_STATUS_H

#include "gbem_config.h"
#include "cia402.h"

/* enums for the state of the different programs that can be run */
typedef enum {
    ECM_PRE_BOOT, ECM_BOOT_FINISHED, ECM_CYCLIC_RUNNING, ECM_ERROR
} ecm_cyclic_state_t;
typedef enum {
    ECM_NO_PROG, ECM_NET_SCAN_PROG, ECM_CYCLIC_PROG, ECM_PRINT_CONFIG_PROG, ECM_WRITE_NVRAM_PROG, ECM_NET_SCAN_PDO_PROG
} ecm_active_program_t;
typedef enum {
    ECM_NET_SCAN_PRE_START, ECM_NET_SCAN_START, ECM_NET_SCAN_ERROR, ECM_NET_SCAN_NO_SLAVES_FOUND, ECM_NET_SCAN_FINISHED
} ecm_net_scan_state_t;

/** used in ecm_status to track the progress of the main boot*/
typedef struct {
    bool init_done;
    bool slaves_found;
    bool all_slaves_pre_op;
    bool error_check_ok;
    bool all_slaves_safe_op;
    bool pdo_remap_done;
    bool apply_standard_sdos_done;
    bool wkc_check_ok;
    bool slaves_match_ok;
    bool all_slaves_op;
    bool boot_sucessful;
} ecm_boot_state_t;


/** struct used in ecm_status to hold a copy of key slave state info*/
typedef struct {
    char name[EC_MAXNAME + 1]; //const
    uint16_t Obits; //const
    uint16_t Ibits; //const
    uint16_t Obytes; //const
    uint16_t Ibytes; //const
    bool hasdc; //const
    //this is the ethercat state
    uint16_t state;
    uint16_t configadr; //const
    uint16_t ALstatuscode;
} ecm_status_map_t;


typedef struct {
    uint8_t slave_error_message[MAX_NUM_SLAVE_ERROR_MESSAGES][EC_MAXERRORNAME];
    int head;
    uint8_t num_slots_full;
} ec_circular_slave_error_message_t;

/** nested in ecm_status and holds drive status info */
typedef struct {
    char error_message[MAX_DRIVE_ERROR_MSG_LENGTH];
    cia_commands_t command;
    cia_state_t state;
    char name[MAX_DRIVE_NAME_LENGTH];
} ecm_status_drive_t;


/** This struct holds the config and status of whole machine
 *  It is used by the status writing program
 *  Status runs on another core so it doesnt have access to everything
 *  The other core does have access to dpm so there should be no overlap
*/
typedef struct {
    ecm_cyclic_state_t cyclic_state;
    ecm_boot_state_t boot_state;
    ecm_net_scan_state_t net_scan_state;
    ecm_active_program_t active_program;
    //ec_map is the struct copies into from soem slave struct array
    ecm_status_map_t map[EC_MAXSLAVE]; //this is key bits of the slave state struct
    uint64_t cycle_count;
    uint8_t drive_count; //const
    uint8_t gbc_max_float_in_count; //const
    uint8_t gbc_max_float_out_count; //const
    uint8_t gbc_max_digital_in_count; //const
    uint8_t gbc_max_digital_out_count; //const
    uint8_t gbc_max_int32_in_count; //const
    uint8_t gbc_max_int32_out_count; //const
    uint8_t gbc_max_uint32_in_count; //const
    uint8_t gbc_max_uint32_out_count; //const

    uint8_t plc_float_in_count; //const
    uint8_t plc_float_out_count; //const
    uint8_t plc_digital_in_count; //const
    uint8_t plc_digital_out_count; //const
    uint8_t plc_int32_in_count; //const
    uint8_t plc_int32_out_count; //const
    uint8_t plc_uint32_in_count; //const
    uint8_t plc_uint32_out_count; //const
    bool gbc_connected;
    ec_circular_slave_error_message_t slave_error_messages;
    bool ec_check_found_error;
    uint8_t slavecount;
    ecm_status_drive_t drives[MAP_MAX_NUM_DRIVES];
    cia_state_t machine_state;
    uint64_t shared_mem_busy_count;
} ecm_status_t;

//#ifdef GB_APP_LINUX
extern ecm_status_t ecm_status;
//#endif

extern const char *ecm_cyclic_state_names[];
extern const char *ecm_active_program_names[];
extern const char *ecm_net_scan_names[];


#endif //GBEM_ECM_STATUS_H
