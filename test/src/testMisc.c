
/**
 ******************************************************************************
 * @file           :  testMisc.c
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

#include "etg1020_diag_test.h"
#include "cia402_test.h"

#include "ethercat_test.h"
#include "drive_error_test.h"

#define PRINT_TEST_JSON_BEFORE_TEST

bool ec_printSDO = false;
bool mydin1_test, mydin2_test, mydout1_test, mydout2_test, plc_din1;

void setUp(void) {
//unity

    internal_Cleardown();
}

void tearDown(void) {
//    unity
    // clean stuff up here
}


void test_miscReadPlatformFromIni(void) {
//    {PLATFORM_GENERIC_LINUX, PLATFORM_PI}

    os_platform_t test_platform_read;
    test_platform_read = read_platform_from_ini("test", PLATFORM_LINUX, GB_PROGRAM_GBEM);
    //test stuff
    TEST_ASSERT(test_platform_read == PLATFORM_LINUX);
}



void test_miscEcPushCircularSlaveErrorMessage(void) {

    ec_circular_slave_error_message_t test_message = {0};
    ec_push_circular_slave_error_message(&test_message,
                                         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg2");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg3");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg4");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg5");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg6");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg7");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg8");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg9");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message, "msg10");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);
    ec_push_circular_slave_error_message(&test_message,
                                         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//    printf("num slots: %u\n", test_message.num_slots_full);
//    printf("head: %u\n", test_message.head);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, test_message.head, "head wrong");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(10, test_message.num_slots_full, "num slots full wrong");

    uint8_t *message;

    message = ec_peek_circular_slave_error_message(&test_message, 0);
    TEST_ASSERT_EQUAL_STRING_MESSAGE(
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            message, "message 0 wrong");
//    printf("%s\n", message);

    message = ec_peek_circular_slave_error_message(&test_message, 1);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("msg2", message, "message 1 wrong");
//    printf("%s\n", message);

}


void test_miscCtrlCopySlaveErrorToEcmStatus(void) {

    ec_errort test_error;

    test_error.Slave = 1;
    test_error.ErrorCode = 3;
    ecx_pusherror(&ecx_context, &test_error);

    test_error.Slave = 2;
    test_error.ErrorCode = 4;
    ecx_pusherror(&ecx_context, &test_error);


    ctrl_copy_slave_error_to_ecm_status();
    ctrl_copy_slave_error_to_ecm_status();
    uint8_t *message;
    message = ec_peek_circular_slave_error_message(&ecm_status.slave_error_messages, 0);
//    printf("%s\n", message);

    message = ec_peek_circular_slave_error_message(&ecm_status.slave_error_messages, 1);
//    printf("%s\n", message);
}





void test_miscEcSlavesMatch(void) {

    ec_slavecount = MAP_NUM_SLAVES;
    for (int i = 0; i < MAP_NUM_SLAVES; i++) {
//        strcpy(ec_slave[i + 1].name, "Slave");

//        strcpy(ecm_slave_map[i].name, "Slave");
strcpy(ec_slave[i + 1].name, ecm_slave_map[i].name);

    }
//todo does not test when ECM_CHECK_EEP is defined
    TEST_ASSERT_MESSAGE(ec_slaves_match(), "Slave match fail");
}


void test_miscECCheck(void) {
//     ECCheck
}




int main(void) {

    UNITY_BEGIN();
    UNITY_EXEC_TIME_START();

    /* EtherCAT related */
    RUN_TEST(test_miscEthercatGetSet);
    RUN_TEST(test_miscEcSlavesMatch);

    //todo
    //test_misc_PDO functions

    RUN_TEST(test_miscReadPlatformFromIni);

    /* CIA related */
    RUN_TEST(test_miscCiaCtrlwrdToCommand);
    RUN_TEST(test_miscCiaStatwrdToState);
    RUN_TEST(test_miscCiaCommandToCtrlwrd);
    RUN_TEST(test_miscCiaStateToStatwrd);
    RUN_TEST(test_miscOmStyleCtrlwrds);
    RUN_TEST(test_miscOmStyleStatwrds);
    RUN_TEST(test_miscBeckhoffStyleCtrlwrds);
    RUN_TEST(test_miscBeckhoffStyleStatwrds);
    RUN_TEST(test_miscKollmorgenStyleCtrlwrds);
    RUN_TEST(test_miscKollmorgenStyleStatwrds);

    /* Circular queue related */
    RUN_TEST(test_miscEcPushCircularSlaveErrorMessage);
    RUN_TEST(test_miscCtrlCopySlaveErrorToEcmStatus);

    /* etg.1020 related */
    RUN_TEST(test_miscEtg1020_diag_decode_param_flag);
    RUN_TEST(test_miscEtg1020_diag_textid_decode);
    RUN_TEST(test_miscEtg1020_diag_flag_decode);
    RUN_TEST(test_miscEtg1020_diag_code_decode);
    RUN_TEST(test_miscEtg1020_diag_full_decode);
    RUN_TEST(test_miscEtg1020_diag_timestamp_decode);

    /* drive error message related */
//AZD
    /*drive related */
    RUN_TEST(test_miscAzdError);

    RUN_TEST(test_miscEc_get_error_code_azd);
//    AKD
    RUN_TEST(test_miscAkdError);

    //Beckhoff


    UNITY_EXEC_TIME_STOP();
    return UNITY_END();
}
