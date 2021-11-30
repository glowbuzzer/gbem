
/**
 ******************************************************************************
 * @file           :  testIo.c
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

bool ec_printSDO = false;
bool mydin1_test, mydin2_test, mydout1_test, mydout2_test, plc_din1;
bool slave_to_write_nvram;


void setUp(void) {

internal_Cleardown();
}

void tearDown(void) {
    // clean stuff up here
}

void test_ioDigitalOut(void) {
    TEST_MESSAGE("Start");
    BIT_SET(dpm_out->digital, 1);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(!cs_pdo_get_output_bit_rev(map_get_gbc_iomap_slave(MAP_OUT, 1, ECT_BOOLEAN), map_get_gbc_iomap_bit_num(MAP_OUT, 1, ECT_BOOLEAN)), "Digital output bit is set and should not be as we are not in OPERATION_ENABLED");
    internal_stateMachineClearEstopAndResetFault();
    internal_stateMachineAdvanceToOperationEnabled();

    TEST_ASSERT_MESSAGE(cs_pdo_get_output_bit_rev(map_get_gbc_iomap_slave(MAP_OUT, 1, ECT_BOOLEAN), map_get_gbc_iomap_bit_num(MAP_OUT, 1, ECT_BOOLEAN)), "Digital output bit is not set");

}

void test_ioDigitalIn(void) {
    TEST_MESSAGE("Start");
    //for some reason with mini map dis start at 1
    TEST_ASSERT_MESSAGE(dpm_in->digital == 0, "Digital ins are not all zero before start of test");
    ec_pdo_set_input_bit_rev(map_get_gbc_iomap_slave(MAP_IN, 1, ECT_BOOLEAN), map_get_gbc_iomap_bit_num(MAP_IN, 1, ECT_BOOLEAN), true);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(BIT_CHECK(dpm_in->digital, 1), "Digital in not set");
    ec_pdo_set_input_bit_rev(map_get_gbc_iomap_slave(MAP_IN, 1, ECT_BOOLEAN), map_get_gbc_iomap_bit_num(MAP_IN, 1, ECT_BOOLEAN), false);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(!BIT_CHECK(dpm_in->digital, 1), "Digital in still set");
}


void test_ioIntegerIn(void) {
    TEST_MESSAGE("Start");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, dpm_in->integer32[0], "Integer in [0] is not zero before start of test");
    ec_pdo_set_input_int16_rev(map_get_gbc_iomap_slave(MAP_IN, 0, ECT_INTEGER32), map_get_gbc_iomap_byte_num(MAP_IN, 0, ECT_INTEGER32), 32767);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(32767, dpm_in->integer32[0], "Integer in [0] not set");
    ec_pdo_set_input_int16_rev(map_get_gbc_iomap_slave(MAP_IN, 0, ECT_INTEGER32), map_get_gbc_iomap_byte_num(MAP_IN, 0, ECT_INTEGER32), -32766);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-32766, dpm_in->integer32[0], "Integer in [0] not set (-ve)");

    //1200/32766 * 50 = 1.8311106

    ec_pdo_set_input_int16_rev(map_get_gbc_iomap_slave(MAP_IN, 1, ECT_REAL32), map_get_gbc_iomap_byte_num(MAP_IN, 1, ECT_REAL32), 1200);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(1.8311106, dpm_in->analog[1], "Analog in [1] wrong");



}

void test_ioIntegerOut(void) {
    TEST_MESSAGE("Start");
    dpm_out->integer32[0] = 0;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, ec_pdo_get_output_int16_rev(map_get_gbc_iomap_slave(MAP_OUT, 0, ECT_INTEGER32),
                                                                   map_get_gbc_iomap_byte_num(MAP_OUT, 0, ECT_INTEGER32)), "Integer out [0] not zero");
    dpm_out->integer32[0] = 32767;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_INT16_MESSAGE(32767, ec_pdo_get_output_int16_rev(map_get_gbc_iomap_slave(MAP_OUT, 0, ECT_INTEGER32),
                                                                       map_get_gbc_iomap_byte_num(MAP_OUT, 0, ECT_INTEGER32)), "Integer out [0]");
    dpm_out->integer32[0] = -32767;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_INT16_MESSAGE(-32767, ec_pdo_get_output_int16_rev(map_get_gbc_iomap_slave(MAP_OUT, 0, ECT_INTEGER32),
                                                                        map_get_gbc_iomap_byte_num(MAP_OUT, 0, ECT_INTEGER32)), "Integer out [0]");



}


void test_ioAnalogIn(void) {
    TEST_MESSAGE("Start");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0, dpm_in->analog[0], "Analog in [0] is not zero before start of test");
    ec_pdo_set_input_float_rev(map_get_gbc_iomap_slave(MAP_IN, 0, ECT_REAL32), map_get_gbc_iomap_byte_num(MAP_IN, 0, ECT_REAL32), 33.2);

//    volatile float dg = ec_pdo_get_input_float(map_gbc_in_out.floatin[0].slave_num, map_gbc_in_out.floatin[0].bit_byte_num);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(33.2, dpm_in->analog[0], "Analog in [0] not set");
    ec_pdo_set_input_float_rev(map_get_gbc_iomap_slave(MAP_IN, 0, ECT_REAL32), map_get_gbc_iomap_byte_num(MAP_IN, 0, ECT_REAL32), -32.7);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(-32.7, dpm_in->analog[0], "Analog in [0] not set");
}

void test_ioAnalogOut(void) {
    TEST_MESSAGE("Start");
    dpm_out->analog[0] = 0;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_MESSAGE(
            ec_pdo_get_output_float_rev(map_get_gbc_iomap_slave(MAP_OUT, 0, ECT_REAL32), map_get_gbc_iomap_byte_num(MAP_OUT, 0, ECT_REAL32)) == 0, "Float out [0] not zero");
    dpm_out->analog[0] = 32767.2;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(32767.2, ec_pdo_get_output_float_rev(map_get_gbc_iomap_slave(MAP_OUT, 0, ECT_REAL32),
                                                                         map_get_gbc_iomap_byte_num(MAP_OUT, 0, ECT_REAL32)), "Float out [0]");
    dpm_out->analog[0] = -32767.4;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(-32767.4, ec_pdo_get_output_float_rev(map_get_gbc_iomap_slave(MAP_OUT, 0, ECT_REAL32),
                                                                          map_get_gbc_iomap_byte_num(MAP_OUT, 0, ECT_REAL32)), "Float out [0]");

}



void test_ioSetPos(void){
    TEST_MESSAGE("Start");
    dpm_out->joint_set_position[0]=314159;
    dpm_out->joint_set_position[1]=271828;
    dpm_out->joint_set_position[2]=768;
    dpm_out->joint_set_position[3]=-9999;
    internal_advanceState(1);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-9999, map_drive_get_setpos_wrd_rev_function_ptr[3](3), "setpos drive 3 incorrect");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(314159, map_drive_get_setpos_wrd_rev_function_ptr[0](0), "setpos drive 0 incorrect");


    TEST_ASSERT_EQUAL_INT32_MESSAGE(768, map_drive_get_setpos_wrd_rev_function_ptr[2](2), "setpos drive 2 incorrect");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(271828, map_drive_get_setpos_wrd_rev_function_ptr[1](1), "setpos drive 1 incorrect");
}

//map_drive_get_setpos_wrd_sim_function_ptr[0](0)
//map_drive_set_actpos_wrd_sim_function_ptr
void test_ioActPos(void){

    TEST_MESSAGE("Start");
    map_drive_set_actpos_wrd_rev_function_ptr[0](0, 314159 );
    map_drive_set_actpos_wrd_rev_function_ptr[1](1, 271828 );
    internal_advanceState(1);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(314159, dpm_in->joint_actual_position[0], "actpos drive 0 incorrect");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(271828, dpm_in->joint_actual_position[1], "actpos drive 1 incorrect");
}

int8_t ec_get_moo_sdo_akd(uint16_t drive){
    return 8;
}


int main(void) {
    UNITY_BEGIN();
    UNITY_EXEC_TIME_START();
    RUN_TEST(test_ioDigitalIn);
    RUN_TEST(test_ioDigitalOut);
    RUN_TEST(test_ioAnalogIn);
    RUN_TEST(test_ioAnalogOut);
    RUN_TEST(test_ioIntegerIn);
    RUN_TEST(test_ioIntegerOut);
    RUN_TEST(test_ioSetPos);
    RUN_TEST(test_ioActPos);


    UNITY_EXEC_TIME_STOP();

    //test drive error functions

    return UNITY_END();

}
