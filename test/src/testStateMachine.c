
/**
 ******************************************************************************
 * @file           :  testStateMachine.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "test.h"
bool mydin1_test, mydin2_test, mydout1_test, mydout2_test, plc_din1;
bool ec_printSDO = false;

void setUp(void) {
//unity
    internal_Cleardown();
}

void tearDown(void) {
//    unity
    // clean stuff up here
}

/**
 * @brief internal test function to check everything has reached a particular state
 * @param state
 * tests:
 * current_state (state machine state)
 * dpm_in->machine_word (state reported back in the dpm)
 *
 */

void internal_testState(cia_state_t state) {
    if (current_state != state) {
        TEST_PRINTF("FAIL: current_state!=%s (this is the state machine state)", cia_state_names[state]);
        TEST_FAIL();

    }
    if (dpm_in->machine_word != cia_state_to_statwrd(state)) {
        TEST_PRINTF("FAIL: dpm_in->machine_word!=%s", cia_state_names[state]);
        TEST_FAIL();
    }
}



void test_stateMachineDriveCommandsAndState(void) {
    internal_stateMachineClearEstopAndResetFault();
    internal_stateMachineAdvanceToOperationEnabled();
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(CIA_ENABLE_OPERATION_CTRLWRD, map_drive_get_ctrl_wrd_rev_function_ptr[i](i), "Drive control word mismatch on drive");
        TEST_ASSERT_MESSAGE(ecm_status.drives[i].command == CIA_ENABLE_OPERATION,
                            "ecm_status.drives[i].command mismatch");
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(CIA_OPERATION_ENABLED,
                                         cia_statwrd_to_state(map_drive_get_stat_wrd_function_ptr[i](i)),
                                         "Drive status word mismatch");
        TEST_ASSERT_MESSAGE(ecm_status.drives[i].state == CIA_OPERATION_ENABLED, "ecm_status.drives[i].state mismatch");
    }
}

void test_stateMachineMooDispInevent_data(void) {
    TEST_MESSAGE("Start");
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        TEST_ASSERT_MESSAGE(event_data.moo_disp[i] == CIA_MOO_CSP, "moo_disp in event_data is not CIA_MOO_CSP");
    }
}


void test_StateMachineClearEstopAndResetFault(void) {

    TEST_MESSAGE("Start");
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(event_data.estop == true,
                        "estop in event_data is not true even though we are in an estop state");
    //test that we end up in FAULT_REACTION_ACTIVE because estop is true
    internal_testState(CIA_FAULT_REACTION_ACTIVE);
    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->fault_history_word, FAULT_CAUSE_SAFETY_BIT_NUM),
                        "We had an estop event but fault cause does not record it");
    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_SAFETY_BIT_NUM),
                        "estop bit is not set in dpm_in->active_fault_word ");


    mapping_t *row = 0;

//    map_get_gbc_iomap_row(MAP_IN, CTRL_ESTOP_DIN, ECT_BOOLEAN, &row);

    ec_pdo_set_input_bit_rev(ctrl_estop_din.slave_num,ctrl_estop_din.bit_num,
                             true);

//    map_get_gbc_iomap_row(MAP_IN, CTRL_ESTOP_RESET_DIN, ECT_BOOLEAN, &row);


ec_pdo_set_input_bit_rev(ctrl_estop_reset_din.slave_num, ctrl_estop_reset_din.bit_num,
                             true);

//
//    ec_pdo_set_input_bit_rev(map_gbc_in_out.din[CTRL_ESTOP_DIN].slave_num, map_gbc_in_out.din[CTRL_ESTOP_DIN].bit_byte_num,
//                          true);
//    ec_pdo_set_input_bit_rev(map_gbc_in_out.din[CTRL_ESTOP_RESET_DIN].slave_num,
//                          map_gbc_in_out.din[CTRL_ESTOP_RESET_DIN].bit_byte_num,
//                          true);
    dpm_out->machine_word = CIA_FAULT_RESET_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);

    TEST_ASSERT_MESSAGE(event_data.estop == false,
                        "estop in event_data has not been set to false (estop should be cleared)");

    TEST_ASSERT_MESSAGE(!BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_SAFETY_BIT_NUM),
                        "estop bit is still set in dpm_in->active_fault_word even though we have cleared estop");

    internal_setAllDriveStatus(CIA_SWITCH_ON_DISABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(2);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_FAULT_RESET) ||
                        ctrl_check_all_drives_commands_sim(CIA_DISABLE_VOLTAGE),
                        "All drives were not commanded with FAULT_RESET or DISABLED_VOLTAGE");
    internal_testState(CIA_SWITCH_ON_DISABLED);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED),
                        "All drives were not in state SWITCH_ON_DISABLED");
}


void test_StateMachineAdvanceToOperationEnabled(void) {
    TEST_MESSAGE("Start");
    internal_stateMachineClearEstopAndResetFault();

    //machine should start in SWITCH_ON_DISABLED

    internal_setAllDriveStatus(CIA_SWITCH_ON_DISABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_DISABLE_VOLTAGE),
                        "All drives were not commanded with DISABLE_VOLTAGE");
    internal_testState(CIA_SWITCH_ON_DISABLED);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED),
                        "All drives were not in state SWITCH_ON_DISABLED");


    //command machine to SHUTDOWN
    dpm_out->machine_word = CIA_SHUTDOWN_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    //drives should advance to READY_TO_SWITCH_ON
    internal_setAllDriveStatus(CIA_READY_TO_SWITCH_ON);

    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_SHUTDOWN),
                        "All drives were not commanded with SHUTDOWN");
    internal_testState(CIA_READY_TO_SWITCH_ON);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON),
                        "All drives were not in state READY_TO_SWITCH_ON");

//command machine to SWITCH_ON
    dpm_out->machine_word = CIA_SWITCH_ON_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    internal_setAllDriveStatus(CIA_SWITCHED_ON);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_SWITCH_ON),
                        "All drives were not commanded with SWITCH_ON");
    internal_testState(CIA_SWITCHED_ON);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_SWITCHED_ON), "All drives were not in state SWITCHED_ON");


//command machine to OPERATION_ENABLED
    dpm_out->machine_word = CIA_ENABLE_OPERATION_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    internal_setAllDriveStatus(CIA_OPERATION_ENABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_ENABLE_OPERATION),
                        "All drives were not commanded with ENABLE_OPERATION");
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_OPERATION_ENABLED),
                        "All drives were not in state OPERATION_ENABLED");
    internal_testState(CIA_OPERATION_ENABLED);
}

void test_StateMachineRegressToSwitchOnDisabled(void) {
    TEST_MESSAGE("Start");
    internal_stateMachineClearEstopAndResetFault();
    internal_stateMachineAdvanceToOperationEnabled();

    dpm_out->machine_word = CIA_DISABLE_VOLTAGE_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    internal_setAllDriveStatus(CIA_SWITCH_ON_DISABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_DISABLE_VOLTAGE),
                        "All drives were not commands with DISABLE_VOLTAGE");
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED),
                        "All drives were not in state SWITCH_ON_DISABLED");

    internal_testState(CIA_SWITCH_ON_DISABLED);

}


void internal_OperationEnabledToFaultWithDriveError(void) {
    internal_stateMachineClearEstopAndResetFault();
    internal_stateMachineAdvanceToOperationEnabled();
    map_drive_set_stat_wrd_rev_function_ptr[0](0, CIA_FAULT_REACTION_ACTIVE_STATWRD);
//    ec_pdo_set_input_uint16_sim(map_drives_statusword[0].slave_num, map_drives_statusword[0].byte_num,
//                             CIA_FAULT_REACTION_ACTIVE_STATWRD);


    internal_advanceStateIncrementHeartbeatSetRemote(1);

}

void test_StateMachineOperationEnabledToFaultWithDriveError(void) {
    TEST_MESSAGE("Start");
    internal_stateMachineClearEstopAndResetFault();
    internal_stateMachineAdvanceToOperationEnabled();


    //set the first drive's status word to fault reaction active
    map_drive_set_stat_wrd_rev_function_ptr[0](0, CIA_FAULT_REACTION_ACTIVE_STATWRD);
//    ec_pdo_set_input_uint16_sim(map_drives_statusword[0].slave_num, map_drives_statusword[0].byte_num,
//                             CIA_FAULT_REACTION_ACTIVE_STATWRD);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    internal_testState(CIA_FAULT_REACTION_ACTIVE);
}


void test_StateMachineRecoverFromDriveFault(void) {
    TEST_MESSAGE("Start");

    internal_stateMachineClearEstopAndResetFault();
    internal_stateMachineAdvanceToOperationEnabled();
    internal_OperationEnabledToFaultWithDriveError();
    internal_setAllDriveStatus(CIA_SWITCH_ON_DISABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(1);

    dpm_out->machine_word = CIA_FAULT_RESET_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(dpm_in->machine_word == CIA_SWITCH_ON_DISABLED_STATWRD,
                        "Did not transition to SWITCH_ON_DISABLED after fault");

//should be what here?
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_QUICK_STOP) ||
                        ctrl_check_all_drives_commands_sim(CIA_DISABLE_VOLTAGE),
                        "All drives were not commanded with QUICK_STOP");

    dpm_out->machine_word = CIA_SHUTDOWN_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    //drives should advance to READY_TO_SWITCH_ON

    internal_setAllDriveStatus(CIA_READY_TO_SWITCH_ON);

    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_SHUTDOWN),
                        "All drives were not commanded with SHUTDOWN");
    internal_testState(CIA_READY_TO_SWITCH_ON);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON),
                        "All drives were not in state READY_TO_SWITCH_ON");
}

void test_StateMachineHeartbeatLost(void) {
    TEST_MESSAGE("Start");

    internal_advanceState(100);

    TEST_ASSERT_MESSAGE(event_data.heartbeat_lost == true, "event_data.heartbeat_lost!=true");

    TEST_ASSERT_MESSAGE(current_state == CIA_FAULT_REACTION_ACTIVE, "current_state!=CIA_FAULT_REACTION_ACTIVE");
    internal_testState(CIA_FAULT_REACTION_ACTIVE);

    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_HEARTBEAT_LOST_BIT_NUM),
                        "We had an heartbeat lost event but dpm_in->active_fault_word is not recording it");

}

void test_StateMachineHeartbeatRecover(void) {
    TEST_MESSAGE("Start");
    internal_advanceState(100);

    TEST_ASSERT_MESSAGE(event_data.heartbeat_lost == true, "event_data.heartbeat_lost!=true");

    TEST_ASSERT_MESSAGE(current_state == CIA_FAULT_REACTION_ACTIVE, "current_state!=CIA_FAULT_REACTION_ACTIVE");
    internal_testState(CIA_FAULT_REACTION_ACTIVE);

    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_HEARTBEAT_LOST_BIT_NUM),
                        "We had an heartbeat lost event but dpm_in->active_fault_word is not recording it");

    internal_advanceStateIncrementHeartbeatSetRemote(20);

    TEST_ASSERT_MESSAGE(!BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_HEARTBEAT_LOST_BIT_NUM),
                        "We restored heartbeat but dpm_in->active_fault_word is still showing heartbeat lost");

    TEST_ASSERT_MESSAGE(event_data.heartbeat_lost == false, "event_data.heartbeat_lost!=true");


}


void test_StateMachineDelayedTransition(void) {
    TEST_MESSAGE("Start");
    internal_stateMachineClearEstopAndResetFault();

    internal_setAllDriveStatus(CIA_SWITCH_ON_DISABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_DISABLE_VOLTAGE),
                        "All drives were not commanded with DISABLE_VOLTAGE");
    internal_testState(CIA_SWITCH_ON_DISABLED);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED),
                        "All drives were not in state SWITCH_ON_DISABLED");


    ctrl_state_change_timeout = 5;

    //command machine to SHUTDOWN
    dpm_out->machine_word = CIA_SHUTDOWN_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    //drives should advance to READY_TO_SWITCH_ON
    internal_setAllDriveStatus(CIA_READY_TO_SWITCH_ON);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_commands_sim(CIA_SHUTDOWN),
                        "All drives were not commanded with SHUTDOWN");
    internal_testState(CIA_READY_TO_SWITCH_ON);
    TEST_ASSERT_MESSAGE(ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON),
                        "All drives were not in state READY_TO_SWITCH_ON");

    dpm_out->machine_word = CIA_SWITCH_ON_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(10);

    internal_testState(CIA_FAULT_REACTION_ACTIVE);

    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_DRIVE_STATE_CHANGE_TIMEOUT_BIT_NUM),
                        "We have an drive state change timeout but dpm_in->active_fault_word is not recording it");


    ctrl_state_change_timeout = CTRL_DRIVE_CHANGE_STATE_TIMEOUT;


}

void test_StateMachineFollowError(void) {
    TEST_MESSAGE("Start");
    uint16_t test_drive_status_word = 0;
    BIT_SET(test_drive_status_word, CIA_FOLLOW_ERROR_BIT_NUM);

    map_drive_set_stat_wrd_rev_function_ptr[0](0, test_drive_status_word);
//    ec_pdo_set_input_uint16_sim(map_drives_statusword[0].slave_num, map_drives_statusword[0].byte_num,
//                             test_drive_status_word);

    internal_advanceStateIncrementHeartbeatSetRemote(2);

    printf("active fault[%u]\n", dpm_in->active_fault_word);

    TEST_ASSERT_MESSAGE(event_data.follow_error == true, "event_data.follow_error!=true");

    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_DRIVE_FOLLOW_ERROR_BIT_NUM),
                        "We simulated an follow error but dpm_in active fault does not record it");
}

void test_StateMachineInternalLimit(void) {
    TEST_MESSAGE("Start");
    uint16_t test_drive_status_word = 0;
    BIT_SET(test_drive_status_word, CIA_INTERNAL_LIMIT_BIT_NUM);

    map_drive_set_stat_wrd_rev_function_ptr[0](0, test_drive_status_word);
//    ec_pdo_set_input_uint16_sim(map_drives_statusword[0].slave_num, map_drives_statusword[0].byte_num,
//                             test_drive_status_word);

    internal_advanceStateIncrementHeartbeatSetRemote(1);

    TEST_ASSERT_MESSAGE(event_data.internal_limit == true, "event_data.internal_limit!=true");
    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_LIMIT_REACHED_BIT_NUM),
                        "We simulated an limit error  but active fault cause does not record it");

}

void test_StateMachineMachineRequestError(void) {
    TEST_MESSAGE("Start");

    internal_stateMachineClearEstopAndResetFault();
    internal_stateMachineAdvanceToOperationEnabled();
    BIT_SET(dpm_out->machine_word, CTRL_MACHINE_CTRL_WRD_REQUEST_FAULT_BIT_NUM);
    internal_advanceStateIncrementHeartbeatSetRemote(1);

    internal_testState(CIA_FAULT_REACTION_ACTIVE);
    TEST_ASSERT_MESSAGE(event_data.machine_request_error == true, "event_data.machine_request_error!=true");
    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->fault_history_word, FAULT_CAUSE_GBC_FAULT_REQUEST_BIT_NUM),
                        "We simulated a gbc error request  but fault cause does not record it");
    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_GBC_FAULT_REQUEST_BIT_NUM),
                        "We simulated an gbc error request but active fault cause does not record it");
}

void test_StateMachineEccheckError(void) {

    TEST_MESSAGE("Start");

    ecm_status.ec_check_found_error = true;
    internal_advanceStateIncrementHeartbeatSetRemote(2);

    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_ECAT_BIT_NUM),
                        "We simulated an ecat error  but active fault cause does not record it");

}

void test_StateMachineSlaveReportedError(void) {
    TEST_MESSAGE("Start");
    EcatError = true;
    internal_advanceState(1);
    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_ECAT_SLAVE_ERROR_BIT_NUM),
                        "We simulated an ecat error  but active fault cause does not record it");
}

void testStateMachineRemoteLostOnDrive(void) {
    TEST_MESSAGE("Start");

    uint16_t test_drive_status_word = 0;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(event_data.remote_ok == true, "event_data.remote_ok!=true");
    //clear the remote bit on the first drive
    BIT_CLEAR(test_drive_status_word, CIA_REMOTE_BIT_NUM);
    map_drive_set_stat_wrd_rev_function_ptr[0](0, test_drive_status_word);
//    ec_pdo_set_input_uint16_sim(map_drives_statusword[0].slave_num, map_drives_statusword[0].byte_num,
//                             test_drive_status_word);

    internal_advanceStateIncrementHeartbeat(1);

    TEST_ASSERT_MESSAGE(event_data.remote_ok != true, "event_data.remote_ok==true");
    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_DRIVE_NO_REMOTE_BIT_NUM),
                        "We simulated a remote lost but active fault cause does not record it");

}

void testStateMachineWarningOnDrive(void) {
    TEST_MESSAGE("Start");

    uint16_t test_drive_status_word = 0;
    BIT_SET(test_drive_status_word, CIA_WARNING_BIT_NUM);

    map_drive_set_stat_wrd_rev_function_ptr[0](0, test_drive_status_word);
//    ec_pdo_set_input_uint16_sim(map_drives_statusword[0].slave_num, map_drives_statusword[0].byte_num,
//                             test_drive_status_word);

    internal_advanceStateIncrementHeartbeatSetRemote(1);

    TEST_ASSERT_MESSAGE(event_data.any_drive_has_alarm == true, "event_data.any_drive_has_alart!=true");

    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->active_fault_word, FAULT_CAUSE_DRIVE_ALARM_BIT_NUM),
                        "We simulated a drive warning  but active fault cause does not record it");
}



void test_initEcm_status(void) {
    TEST_MESSAGE("Start");
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    init_ecm_status();

    //todo-crit

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(MAP_NUM_DRIVES, ecm_status.drive_count, "MAP_NUM_DRIVES");

}

void test_StateMachinetest_quick_stop(void) {
    TEST_MESSAGE("Start");
    internal_stateMachineClearEstopAndResetFault();
    internal_stateMachineAdvanceToOperationEnabled();
    dpm_out->machine_word = CIA_QUICK_STOP_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);

    internal_setAllDriveStatus(CIA_QUICK_STOP_ACTIVE);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    internal_testState(CIA_QUICK_STOP_ACTIVE);

    internal_setAllDriveStatus(CIA_SWITCH_ON_DISABLED);

    internal_advanceStateIncrementHeartbeatSetRemote(5);

    internal_testState(CIA_SWITCH_ON_DISABLED);
}

void test_StateMachineActSetPos(void) {
    TEST_MESSAGE("Start");

    //set a setpos in dpm for joint 0
    dpm_out->joint_set_position[0] = 999;
    //run a cucle
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    //test that the drive itself had the setpos from dpm applied
    TEST_ASSERT_EQUAL_INT32_MESSAGE(999, map_drive_get_setpos_wrd_rev_function_ptr[0](0), "setPos");

    //now sim that the drive reported an actpos
    map_drive_set_actpos_wrd_rev_function_ptr[0](0, 777);

    //run a cylce
    internal_advanceStateIncrementHeartbeatSetRemote(1);

    //test that dpm is reporting the actpos from the drive
    TEST_ASSERT_EQUAL_INT32_MESSAGE(777, dpm_in->joint_actual_position[0], "actPos");

}


void test_StateMachineCycleCount(void) {
    TEST_MESSAGE("Start");
    internal_advanceStateIncrementHeartbeatSetRemote(3);
    //we run a cycle in the test setup code so the test must have one added
        TEST_ASSERT_EQUAL_UINT64_MESSAGE(4, ecm_status.cycle_count, "ecm_status.cycle_count");
}


int8_t ec_get_moo_sdo_akd(uint16_t drive){
    return 8;
}


int main(void) {
    UNITY_BEGIN();
    UNITY_EXEC_TIME_START();
    RUN_TEST(test_StateMachineAdvanceToOperationEnabled);
    RUN_TEST(test_stateMachineDriveCommandsAndState);
    RUN_TEST(test_StateMachineRecoverFromDriveFault);
    RUN_TEST(test_StateMachineOperationEnabledToFaultWithDriveError);
    RUN_TEST(test_initEcm_status);
    RUN_TEST(test_stateMachineMooDispInevent_data);
    RUN_TEST(test_StateMachineHeartbeatLost);
    RUN_TEST(test_StateMachineClearEstopAndResetFault);
    RUN_TEST(test_StateMachineDelayedTransition);
    RUN_TEST(test_StateMachineFollowError);
    RUN_TEST(test_StateMachineInternalLimit);
    RUN_TEST(test_StateMachineMachineRequestError);
    RUN_TEST(test_StateMachineEccheckError);
    RUN_TEST(testStateMachineRemoteLostOnDrive);
    RUN_TEST(testStateMachineWarningOnDrive);
    RUN_TEST(test_StateMachineSlaveReportedError);
    RUN_TEST(test_StateMachineHeartbeatRecover);
    RUN_TEST(test_StateMachinetest_quick_stop);
    RUN_TEST(test_StateMachineCycleCount);
    RUN_TEST(test_StateMachineActSetPos);
    UNITY_EXEC_TIME_STOP();
    return UNITY_END();

}
