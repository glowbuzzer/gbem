/**
 ******************************************************************************
 * @file           :  gbem_ctx
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#ifndef GBEM_CTX_H
#define GBEM_CTX_H

#include "std_headers.h"
#include "gbem_config.h"
#include "ecm_status.h"
#include "dpm.h"
#include "linux_shm.h"
#include "control.h"

typedef struct {
    char eth_interface1[SIZE_OF_IF_NAME]; //done
    char eth_interface2[SIZE_OF_IF_NAME]; //done
    uint8_t IOmap[ECM_IO_MAP_SIZE];
    bool ec_printSDO;
    ecm_status_t ecm_status;
    uint16_t slave_to_write_nvram;
    uint8_t inA[SIZE_OF_GBC_PDO];
    uint8_t inB[SIZE_OF_GBC_PDO];
    uint8_t outA[SIZE_OF_GBC_PDO];
    uint8_t outB[SIZE_OF_GBC_PDO];
    struct shm_msg *shmp;
    sem_t *gbc_named_trigger_semaphore;
    sem_t *gbc_named_mem_protection_semaphore;
    sem_t *gbc_named_offline_mem_protection_semaphore;
    pthread_mutex_t console_mutex;
    pthread_t thread_ec_rxtx;
    pthread_t thread_ec_check;
    pthread_t thread_ec_reboot;
    pthread_t thread_ec_error_message;
    debug_settings_t debug_settings;
    event_data_t event_data;
    uint32_t gbem_heartbeat;
    ec_sdo_array ar;
    int32_t ecm_cycle_shift; //ECM_CYCLE_SHIFT
    int32_t map_cycle_time; //MAP_CYCLE_TIME
    bool no_limits; //NO_LIMITS (software pos limits)
    char *gbem_version;
} gbem_ctx_t;

extern gbem_ctx_t gbem_ctx;

#endif //GBEM_CTX_H
