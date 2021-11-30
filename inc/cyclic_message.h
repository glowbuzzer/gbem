/**
 ******************************************************************************
 * @file           :  cyclic_message.h
 * @brief          :  functions to print cyclic messages
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_CYCLIC_MESSAGE_H
#define GB_MONOREPO_CYCLIC_MESSAGE_H

#include "stdbool.h"
#include "stdint.h"

typedef enum{CYCLIC_MSG_INFO, CYCLIC_MSG_WARN, CYCLIC_MSG_ERROR} cyclic_message_type_t;

/** Cyclic messages are outputted periodically. This struct lays out the structure of these messages */
typedef struct{
    char * message;
    bool active;
    bool message_printed;
    cyclic_message_type_t type;
} cyclic_event_t;

void print_cyclic_user_message(uint32_t num_events, cyclic_event_t * event_arr);

#endif //GB_MONOREPO_CYCLIC_MESSAGE_H
