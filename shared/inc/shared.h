/**
 ******************************************************************************
 * @file           :  shared.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef GBEM_SHARED
#define GBEM_SHARED

#include "std_headers.h"
#include "cia402.h"
#include "gbem_config.h"

#ifdef GB_APP_LINUX

#include <pthread.h>
#include "map.h"
#include "ecm_status.h"

#endif


////these are really defined in ethercatmain.h and map.h
#ifndef GB_APP_LINUX
#define EC_MAXNAME 40
#define MAX_NUM_SLAVE_ERROR_MESSAGES 10
#define  MAX_DRIVE_ERROR_MSG_LENGTH 200
#define EC_MAXSLAVE 200
#endif

#ifdef GB_APP_LINUX
extern pthread_t thread_ec_rxtx;
extern pthread_t thread_ec_check;
extern pthread_t thread_ec_emstat;
extern pthread_t thread_ec_reboot;
extern pthread_t thread_ec_drive_error_message;

#endif


#endif
