
/**
 ******************************************************************************
 * @file           :  cia402_test.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "std_defs_and_macros.h"
#include "unity.h"
#include "cia402.h"

void test_miscOmStyleCtrlwrds(void) {
    uint16_t test_ctrlwrd;

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-1");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-2");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "SWITCH_ON-1");

    test_ctrlwrd = 0;
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-2");
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-3");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-4");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 1);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-2");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-3");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_OPERATION ||
                        cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "DISABLE_OPERATION-1 (SWITCH_ON)");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_ENABLE_OPERATION, "ENABLE_OPERATION-1");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 7);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-2");
    BIT_SET(test_ctrlwrd, 1);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-3");
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-4");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-5");

}

void test_miscOmStyleStatwrds(void) {

    uint16_t test_statwrd;

    test_statwrd = 0;
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_NOT_READY_TO_SWITCH_ON, "NOT_READY_TO_SWITCH_ON-1");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 5);
    BIT_SET(test_statwrd, 6);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCH_ON_DISABLED, "SWITCH_ON_DISABLED-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCH_ON_DISABLED, "SWITCH_ON_DISABLED-2");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_READY_TO_SWITCH_ON, "READY_TO_SWITCH_ON-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_READY_TO_SWITCH_ON, "READY_TO_SWITCH_ON-2");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCHED_ON, "SWITCHED_ON-1");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_OPERATION_ENABLED, "OPERATION_ENABLED-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_OPERATION_ENABLED, "OPERATION_ENABLED-2");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 3);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT, "FAULT-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT, "FAULT-2");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    BIT_SET(test_statwrd, 3);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT_REACTION_ACTIVE, "FAULT_REACTION_ACTIVE-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT_REACTION_ACTIVE, "FAULT_REACTION_ACTIVE-2");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_QUICK_STOP_ACTIVE, "QUICK_STOP_ACTIVE-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_QUICK_STOP_ACTIVE, "QUICK_STOP_ACTIVE-2");
}

void test_miscBeckhoffStyleCtrlwrds(void) {
    uint16_t test_ctrlwrd;

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-1");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-2");
    BIT_SET(test_ctrlwrd, 4);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-3");
    BIT_SET(test_ctrlwrd, 5);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-4");
    BIT_SET(test_ctrlwrd, 6);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-5");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "SWITCH_ON-1");
    BIT_SET(test_ctrlwrd, 4);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "SWITCH_ON-2");
    BIT_SET(test_ctrlwrd, 5);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "SWITCH_ON-3");
    BIT_SET(test_ctrlwrd, 6);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "SWITCH_ON-4");


    test_ctrlwrd = 0;
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-2");
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-3");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-4");
    BIT_SET(test_ctrlwrd, 4);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-5");
    BIT_SET(test_ctrlwrd, 5);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-6");
    BIT_SET(test_ctrlwrd, 6);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-7");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 1);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-2");
    BIT_SET(test_ctrlwrd, 4);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-3");
    BIT_SET(test_ctrlwrd, 5);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-4");
    BIT_SET(test_ctrlwrd, 6);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-4");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_OPERATION ||
                        cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "DISABLE_OPERATION-1 (SWITCH_ON)");


    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_ENABLE_OPERATION, "ENABLE_OPERATION-1");
    BIT_SET(test_ctrlwrd, 4);
//    volatile cia_commands_t test_command =    cia_ctrlwrd_to_command(test_ctrlwrd);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_ENABLE_OPERATION, "ENABLE_OPERATION-2");
    BIT_SET(test_ctrlwrd, 5);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_ENABLE_OPERATION, "ENABLE_OPERATION-3");
    BIT_SET(test_ctrlwrd, 6);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_ENABLE_OPERATION, "ENABLE_OPERATION-4");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 7);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-2");
    BIT_SET(test_ctrlwrd, 1);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-3");
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-4");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-5");
    BIT_SET(test_ctrlwrd, 4);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-6");
    BIT_SET(test_ctrlwrd, 5);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-7");
    BIT_SET(test_ctrlwrd, 6);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-8");


}



void test_miscBeckhoffStyleStatwrds(void) {

    uint16_t test_statwrd;

    test_statwrd = 0;
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_NOT_READY_TO_SWITCH_ON, "NOT_READY_TO_SWITCH_ON-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_NOT_READY_TO_SWITCH_ON, "NOT_READY_TO_SWITCH_ON-2");
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_NOT_READY_TO_SWITCH_ON, "NOT_READY_TO_SWITCH_ON-3");
    BIT_SET(test_statwrd, 7);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_NOT_READY_TO_SWITCH_ON, "NOT_READY_TO_SWITCH_ON-4");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 6);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCH_ON_DISABLED, "SWITCH_ON_DISABLED-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCH_ON_DISABLED, "SWITCH_ON_DISABLED-2");
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCH_ON_DISABLED, "SWITCH_ON_DISABLED-2");
    BIT_SET(test_statwrd, 7);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCH_ON_DISABLED, "SWITCH_ON_DISABLED-2");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_READY_TO_SWITCH_ON, "READY_TO_SWITCH_ON-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_READY_TO_SWITCH_ON, "READY_TO_SWITCH_ON-2");
    BIT_SET(test_statwrd, 7);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_READY_TO_SWITCH_ON, "READY_TO_SWITCH_ON-3");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCHED_ON, "SWITCHED_ON-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCHED_ON, "SWITCHED_ON-2");
    BIT_SET(test_statwrd, 7);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCHED_ON, "SWITCHED_ON-3");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_OPERATION_ENABLED, "OPERATION_ENABLED-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_OPERATION_ENABLED, "OPERATION_ENABLED-2");
    BIT_SET(test_statwrd, 7);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_OPERATION_ENABLED, "OPERATION_ENABLED-2");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 3);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT, "FAULT-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT, "FAULT-2");
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT, "FAULT-3");
    BIT_SET(test_statwrd, 7);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT, "FAULT-4");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    BIT_SET(test_statwrd, 3);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT_REACTION_ACTIVE, "FAULT_REACTION_ACTIVE-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT_REACTION_ACTIVE, "FAULT_REACTION_ACTIVE-2");
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT_REACTION_ACTIVE, "FAULT_REACTION_ACTIVE-3");
    BIT_SET(test_statwrd, 7);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT_REACTION_ACTIVE, "FAULT_REACTION_ACTIVE-4");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_QUICK_STOP_ACTIVE, "QUICK_STOP_ACTIVE-1");
    BIT_SET(test_statwrd, 4);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_QUICK_STOP_ACTIVE, "QUICK_STOP_ACTIVE-2");
    BIT_SET(test_statwrd, 7);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_QUICK_STOP_ACTIVE, "QUICK_STOP_ACTIVE-3");

}



void test_miscKollmorgenStyleCtrlwrds(void) {
    uint16_t test_ctrlwrd;

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-1");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SHUTDOWN, "SHUTDOWN-2");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "SWITCH_ON-1");

    test_ctrlwrd = 0;
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-2");
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-3");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE-4");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 1);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-2");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_QUICK_STOP, "QUICK_STOP-3");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_DISABLE_OPERATION ||
                        cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_SWITCH_ON, "DISABLE_OPERATION-1 (SWITCH_ON)");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 0);
    BIT_SET(test_ctrlwrd, 1);
    BIT_SET(test_ctrlwrd, 2);
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_ENABLE_OPERATION, "ENABLE_OPERATION-1");

    test_ctrlwrd = 0;
    BIT_SET(test_ctrlwrd, 7);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-1");
    BIT_SET(test_ctrlwrd, 0);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-2");
    BIT_SET(test_ctrlwrd, 1);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-3");
    BIT_SET(test_ctrlwrd, 2);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-4");
    BIT_SET(test_ctrlwrd, 3);
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(test_ctrlwrd) == CIA_FAULT_RESET, "FAULT_RESET-5");
}



void test_miscKollmorgenStyleStatwrds(void) {

    uint16_t test_statwrd;

    test_statwrd = 0;
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_NOT_READY_TO_SWITCH_ON, "NOT_READY_TO_SWITCH_ON-1");
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_NOT_READY_TO_SWITCH_ON, "NOT_READY_TO_SWITCH_ON-2");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 6);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCH_ON_DISABLED, "SWITCH_ON_DISABLED-1");
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCH_ON_DISABLED, "SWITCH_ON_DISABLED-2");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_READY_TO_SWITCH_ON, "READY_TO_SWITCH_ON-1");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_SWITCHED_ON, "SWITCHED_ON-1");


    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_OPERATION_ENABLED, "OPERATION_ENABLED-1");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 3);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT, "FAULT-1");
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT, "FAULT-2");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    BIT_SET(test_statwrd, 3);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT_REACTION_ACTIVE, "FAULT_REACTION_ACTIVE-1");
    BIT_SET(test_statwrd, 5);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_FAULT_REACTION_ACTIVE, "FAULT_REACTION_ACTIVE-2");

    test_statwrd = 0;
    BIT_SET(test_statwrd, 0);
    BIT_SET(test_statwrd, 1);
    BIT_SET(test_statwrd, 2);
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(test_statwrd) == CIA_QUICK_STOP_ACTIVE, "QUICK_STOP_ACTIVE-1");
}


void test_miscCiaCtrlwrdToCommand(void) {

//    cia_commands_t cia_ctrlwrd_to_command(uint16_t controlWord);

    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(CIA_SHUTDOWN_CTRLWRD) == CIA_SHUTDOWN, "SHUTDOWN");
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(CIA_SWITCH_ON_CTRLWRD) == CIA_SWITCH_ON, "SWITCH_ON");
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(CIA_DISABLE_VOLTAGE_CTRLWRD) == CIA_DISABLE_VOLTAGE, "DISABLE_VOLTAGE");
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(CIA_QUICK_STOP_CTRLWRD) == CIA_QUICK_STOP, "QUICK_STOP");
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(CIA_DISABLE_OPERATION_CTRLWRD) == CIA_DISABLE_OPERATION ||
                        cia_ctrlwrd_to_command(CIA_DISABLE_OPERATION_CTRLWRD) == CIA_SWITCH_ON, "DISABLE_OPERATION");
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(CIA_ENABLE_OPERATION_CTRLWRD) == CIA_ENABLE_OPERATION,
                        "ENABLE_OPERATION");
    TEST_ASSERT_MESSAGE(cia_ctrlwrd_to_command(CIA_FAULT_RESET_CTRLWRD) == CIA_FAULT_RESET, "FAULT_RESET");

}


void test_miscCiaStatwrdToState(void) {
//    cia_state_t cia_statwrd_to_state(uint16_t statusWord);

    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(CIA_NOT_READY_TO_SWITCH_ON_STATWRD) == CIA_NOT_READY_TO_SWITCH_ON,
                        "NOT_READY_TO_SWITCH_ON");
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(CIA_SWITCH_ON_DISABLED_STATWRD) == CIA_SWITCH_ON_DISABLED,
                        "SWITCH_ON_DISABLED");
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(CIA_READY_TO_SWITCH_ON_STATWRD) == CIA_READY_TO_SWITCH_ON,
                        "READY_TO_SWITCH_ON");
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(CIA_SWITCHED_ON_STATWRD) == CIA_SWITCHED_ON, "SWITCHED_ON");
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(CIA_OPERATION_ENABLED_STATWRD) == CIA_OPERATION_ENABLED,
                        "OPERATION_ENABLED");
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(CIA_QUICK_STOP_ACTIVE_STATWRD) == CIA_QUICK_STOP_ACTIVE,
                        "QUICK_STOP_ACTIVE");
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(CIA_FAULT_REACTION_ACTIVE_STATWRD) == CIA_FAULT_REACTION_ACTIVE,
                        "FAULT_REACTION_ACTIVE");
    TEST_ASSERT_MESSAGE(cia_statwrd_to_state(CIA_FAULT_STATWRD) == CIA_FAULT, "CIA_FAULT");
}



void test_miscCiaCommandToCtrlwrd(void) {
    TEST_ASSERT_MESSAGE(cia_command_to_ctrlwrd(CIA_SHUTDOWN) == CIA_SHUTDOWN_CTRLWRD, "SHUTDOWN");
    TEST_ASSERT_MESSAGE(cia_command_to_ctrlwrd(CIA_SWITCH_ON) == CIA_SWITCH_ON_CTRLWRD, "SWITCH_ON");
    TEST_ASSERT_MESSAGE(cia_command_to_ctrlwrd(CIA_DISABLE_VOLTAGE) == CIA_DISABLE_VOLTAGE_CTRLWRD, "DISABLE_VOLTAGE");
    TEST_ASSERT_MESSAGE(cia_command_to_ctrlwrd(CIA_QUICK_STOP) == CIA_QUICK_STOP_CTRLWRD, "QUICK_STOP");
    TEST_ASSERT_MESSAGE(cia_command_to_ctrlwrd(CIA_DISABLE_OPERATION) == CIA_DISABLE_OPERATION_CTRLWRD,
                        "DISABLE_OPERATION");
    TEST_ASSERT_MESSAGE(cia_command_to_ctrlwrd(CIA_ENABLE_OPERATION) == CIA_ENABLE_OPERATION_CTRLWRD,
                        "ENABLE_OPERATION");
    TEST_ASSERT_MESSAGE(cia_command_to_ctrlwrd(CIA_FAULT_RESET) == CIA_FAULT_RESET_CTRLWRD, "FAULT_RESET");

}

void test_miscCiaStateToStatwrd(void) {

    TEST_ASSERT_MESSAGE(cia_state_to_statwrd(CIA_NOT_READY_TO_SWITCH_ON) == CIA_NOT_READY_TO_SWITCH_ON_STATWRD,
                        "NOT_READY_TO_SWITCH_ON");
    TEST_ASSERT_MESSAGE(cia_state_to_statwrd(CIA_SWITCH_ON_DISABLED) == CIA_SWITCH_ON_DISABLED_STATWRD,
                        "SWITCH_ON_DISABLED");
    TEST_ASSERT_MESSAGE(cia_state_to_statwrd(CIA_READY_TO_SWITCH_ON) == CIA_READY_TO_SWITCH_ON_STATWRD,
                        "READY_TO_SWITCH_ON");
    TEST_ASSERT_MESSAGE(cia_state_to_statwrd(CIA_SWITCHED_ON) == CIA_SWITCHED_ON_STATWRD, "SWITCHED_ON");
    TEST_ASSERT_MESSAGE(
            cia_state_to_statwrd(CIA_OPERATION_ENABLED) == CIA_OPERATION_ENABLED_STATWRD, "OPERATION_ENABLED");
    TEST_ASSERT_MESSAGE(cia_state_to_statwrd(CIA_QUICK_STOP_ACTIVE) == CIA_QUICK_STOP_ACTIVE_STATWRD,
                        "QUICK_STOP_ACTIVE");
    TEST_ASSERT_MESSAGE(
            cia_state_to_statwrd(CIA_FAULT_REACTION_ACTIVE) == CIA_FAULT_REACTION_ACTIVE_STATWRD,
            "FAULT_REACTION_ACTIVE");
    TEST_ASSERT_MESSAGE(cia_state_to_statwrd(CIA_FAULT) == CIA_FAULT_STATWRD, "FAULT");

}