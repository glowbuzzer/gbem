
/**
 ******************************************************************************
 * @file           :  test.c
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


ecm_status_t ecm_status;
char storage_dir_for_gbc_exchange[200];
char full_path_for_emstat_json_const[200];
char full_path_for_emstat_json_status[200];

char proc_name[100];




uint8_t inA[SIZE_OF_GBC_PDO];
uint8_t inB[SIZE_OF_GBC_PDO];
uint8_t outA[SIZE_OF_GBC_PDO];
uint8_t outB[SIZE_OF_GBC_PDO];

volatile int wkc;
int expectedWKC;
//uint8 currentgroup = 0;

/** array for SOEM iomap */
uint8_t IOmap[ECM_IO_MAP_SIZE];

/**global var storing the name of the nic read from command lines args */
char eth_interface1[SIZE_OF_IF_NAME] = "eth0";

struct stateMachine *m;

//bool ec_rxtx_test_mode = true;

void internal_advanceStateIncrementHeartbeat(int number_of_cycles) {
    for (int i=0; i<number_of_cycles;i++) {
        ctrl_main(m, false);
        dpm_out->heartbeat++;

    }
}



void internal_advanceStateIncrementHeartbeatSetRemote(int number_of_cycles) {

    //set remote bit in status word for all drives

    for (int i = 0; i < MAP_NUM_DRIVES; i++) {



        uint16_t test_drive_status_word=0;

        if (*map_drive_get_stat_wrd_function_ptr[i] != NULL) {
            test_drive_status_word =  map_drive_get_stat_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_get_stat_wrd on drive [%u]", i);
        }

        BIT_SET(test_drive_status_word, CIA_REMOTE_BIT_NUM);

        if (*map_drive_set_stat_wrd_rev_function_ptr[i] != NULL) {
            map_drive_set_stat_wrd_rev_function_ptr[i](i, test_drive_status_word);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_set_stat_wrd on drive [%u]", i);
        }


    }

for (int i=0; i<number_of_cycles;i++) {
    ctrl_main(m, false);
    dpm_in->heartbeat++;
    dpm_out->heartbeat=dpm_in->heartbeat;
//    printf("*** State Change ***\n");

//printf("State change cycle count: %u\n", ctrl_state_change_cycle_count);
}
}

void internal_advanceState(int number_of_cycles) {
    for (int i=0; i<number_of_cycles;i++) {
    ctrl_main(m, false);
//    printf("*** State Change ***\n");
        }
}


/**
 * @brief this is a simulation function to set a drive status (usually the drive does this itself)
 * @param state
 */
void internal_setAllDriveStatus(cia_state_t state) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {

        if (*map_drive_set_stat_wrd_rev_function_ptr[i] != NULL) {
            map_drive_set_stat_wrd_rev_function_ptr[i](i, cia_state_to_statwrd(state));
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "GBEM: Missing function pointer for map_drive_set_stat_wrd on drive [%u]", i);
        }

    }
}






/**
 * @brief INTERNAL function to advance to operation enabled
 * @warning does not execute any tests
 * @warning assumes estop etc. is ok
 */
void internal_stateMachineAdvanceToOperationEnabled(void) {
    internal_setAllDriveStatus(CIA_SWITCH_ON_DISABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    //set command machine to SHUTDOWN
    dpm_out->machine_word = CIA_SHUTDOWN_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    //drives should advance to READY_TO_SWITCH_ON
    internal_setAllDriveStatus(CIA_READY_TO_SWITCH_ON);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
//command machine to SWITCH_ON
    dpm_out->machine_word = CIA_SWITCH_ON_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    internal_setAllDriveStatus(CIA_SWITCHED_ON);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
//command machine to OPERATION_ENABLED
    dpm_out->machine_word = CIA_ENABLE_OPERATION_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    internal_setAllDriveStatus(CIA_OPERATION_ENABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    TEST_MESSAGE("AdvanceToOperationEnabled completed");
}



void internal_stateMachineClearEstopAndResetFault(void) {
    internal_advanceStateIncrementHeartbeatSetRemote(1);

    mapping_t *row = 0;

//    map_get_gbc_iomap_row(MAP_IN, CTRL_ESTOP_DIN, ECT_BOOLEAN, &row);

    ec_pdo_set_input_bit_rev(ctrl_estop_din.slave_num,ctrl_estop_din.bit_num,
                             true);


    ec_pdo_set_input_bit_rev(ctrl_estop_reset_din.slave_num, ctrl_estop_reset_din.bit_num,
                             true);

    dpm_out->machine_word = CIA_FAULT_RESET_CTRLWRD;
    internal_advanceStateIncrementHeartbeatSetRemote(1);
    internal_setAllDriveStatus(CIA_SWITCH_ON_DISABLED);
    internal_advanceStateIncrementHeartbeatSetRemote(2);
    TEST_MESSAGE("ClearEstopAndResetFault completed");
}


void internal_Cleardown(void) {
    // set stuff up here

//    log_run_level =0;

    gbem_heartbeat = 0;
    current_state = CIA_NOT_READY_TO_SWITCH_ON;
    m = ctrl_statemachine_alloc();
    ctrl_init_statemachine(m);

    memset(&event_data, 0, sizeof(event_data_t));
    memset(&ecm_status, 0, sizeof(ecm_status_t));
    memset(IOmap, 0, ECM_IO_MAP_SIZE);

    memset(dpm_in, 0, sizeof(dpm_in_t));
    memset(dpm_out, 0, sizeof(dpm_out_t));

    ec_slave[1].outputs = &IOmap[0];
    ec_slave[1].inputs = &IOmap[1000];
    ec_slave[2].outputs = &IOmap[100];
    ec_slave[2].inputs = &IOmap[1100];
    ec_slave[3].outputs = &IOmap[200];
    ec_slave[3].inputs = &IOmap[1200];
    ec_slave[4].outputs = &IOmap[300];
    ec_slave[4].inputs = &IOmap[1300];
    ec_slave[5].outputs = &IOmap[400];
    ec_slave[5].inputs = &IOmap[1400];
    ec_slave[6].outputs = &IOmap[500];
    ec_slave[6].inputs = &IOmap[1500];
    ec_slave[7].outputs = &IOmap[600];
    ec_slave[7].inputs = &IOmap[1600];
    ec_slave[8].outputs = &IOmap[700];
    ec_slave[8].inputs = &IOmap[1700];
    ec_slave[9].outputs = &IOmap[800];
    ec_slave[9].inputs = &IOmap[1800];


    ec_slave[1].Istartbit = 0;
    ec_slave[1].Ostartbit = 0;
    ec_slave[2].Istartbit = 0;
    ec_slave[2].Ostartbit = 0;
    ec_slave[3].Istartbit = 0;
    ec_slave[3].Ostartbit = 0;
    ec_slave[4].Istartbit = 0;
    ec_slave[4].Ostartbit = 0;
    ec_slave[5].Istartbit = 0;
    ec_slave[5].Ostartbit = 0;
    ec_slave[6].Istartbit = 0;
    ec_slave[6].Ostartbit = 0;
    ec_slave[7].Istartbit = 0;
    ec_slave[7].Ostartbit = 0;

    ec_slave[8].Istartbit = 0;
    ec_slave[8].Ostartbit = 0;

    ctrl_main(m, true);


    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_set_moo_rev_function_ptr[i] != NULL) {
            map_drive_set_moo_rev_function_ptr[i](i);
        }
//
//        ec_pdo_set_input_int8_sim(map_drives_moo_get_azd[i].slave_num, map_drives_moo_get_azd[i].byte_num,
//                               CIA_MOO_CSP);
    }


}


