
/**
 ******************************************************************************
 * @file           :  cyclic_message.c
 * @brief          :  functions to print cyclic messages
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "cyclic_message.h"
#include "user_message.h"
#include "ecrxtx.h"


/**
 * @brief cyclic messages are printed to console (or log file) by GBEM during cyclic execution with events (errors) occur
 * @param num_events
 * @param event_arr
 */
void print_cyclic_user_message(const uint32_t num_events, cyclic_event_t * event_arr){
    for (int i = 0; i < num_events; i++) {
        //reset message printed
        if (!event_arr[i].active){
            event_arr[i].message_printed=false;
        }
        //if the cyclic event is active AND we have not already printed a message then spit one out
        if(event_arr[i].active && !event_arr[i].message_printed) {
            switch (event_arr[i].type){
                case CYCLIC_MSG_INFO:
                    UM_INFO(GBEM_UM_EN, "GBEM: %s", event_arr[i].message);
                    break;
                case CYCLIC_MSG_WARN:
                    UM_WARN(GBEM_UM_EN, "GBEM: %s", event_arr[i].message);
                    break;
                case CYCLIC_MSG_ERROR:
                    UM_ERROR(GBEM_UM_EN, "GBEM: %s", event_arr[i].message);
                    break;
            }
            //record fact we have spat out an initial message
            event_arr[i].message_printed=true;
        } else if ((print_repeater_message) && event_arr[i].message_printed && event_arr[i].active){
            //if this is a cycle to spit out repeater messages AND
            switch (event_arr[i].type){
                case CYCLIC_MSG_INFO:
                    UM_INFO(GBEM_UM_EN, "GBEM: %s", event_arr[i].message);
                    break;
                case CYCLIC_MSG_WARN:
                    UM_WARN(GBEM_UM_EN, "GBEM: %s", event_arr[i].message);
                    break;
                case CYCLIC_MSG_ERROR:
                    UM_ERROR(GBEM_UM_EN, "GBEM: %s", event_arr[i].message);
                    break;
            }
        }
    }
}

