/**
 ******************************************************************************
 * @file           :  test.h
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBEM_TEST_H
#define GBEM_TEST_H

#include <stdio.h>


#include "std_defs_and_macros.h"
#include "dpm.h"
#include "control.h"
#include "ethercatsetget.h"
#include "map.h"
#include "azdxa-ked.h"
#include "log.h"
#include "status.h"
#include "fault_cause.h"
#include "unity.h"
#include "ec_functions.h"
extern struct stateMachine *m;

void internal_Cleardown(void);



//public fns
void internal_advanceStateIncrementHeartbeat(int number_of_cycles);
void internal_advanceStateIncrementHeartbeatSetRemote(int number_of_cycles);
void internal_advanceState(int number_of_cycle);
void internal_stateMachineClearEstopAndResetFault(void);
void internal_stateMachineAdvanceToOperationEnabled(void);
void internal_setAllDriveStatus(cia_state_t state);


#endif //GBEM_TEST_H
