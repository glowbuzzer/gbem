/**
 ******************************************************************************
 * @file           :  ec_functions.c
 * @brief          :  functions for EtherCAT:
 *                      * boot
 *                      * slave check
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "ec_functions.h"
#include "shared.h"
#include "main.h"
#include "log.h"
#include "ethercatsetget.h"
#include "ethercatnetscan.h"
#include "log.h"
#include "user_message.h"
#include "gberror.h"
#include "dpm.h"
#include "ecrxtx.h"
#include "kollmorgen.h"
#include "jvl_mis.h"

#ifdef GB_APP_LINUX

#include "status.h"
#include <unistd.h>

#else
#include "FreeRTOS.h"
#endif

/** global holding dc delta */
extern int64 gl_delta;

static bool __attribute__((unused)) check_slave_dc_sysdiff_less_than_threshold(const uint16_t slave) {
    int tdiff = 0;

    int local_wc = ec_FPRD(ec_slave[slave].configadr, ECT_REG_DCSYSDIFF, sizeof(tdiff), &tdiff, EC_TIMEOUTRET);

    printf("tdfiff [%d] for slave [%u, 0x%4.4x, %s]\n", tdiff, slave, ec_slave[slave].configadr, ec_slave[slave].name);


    if (local_wc == 0) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: error reading EtherCAT register");
    }

    if (tdiff < 0) {
        tdiff = -(int32) ((uint32) tdiff & 0x7ffffff);
    } else {
        //think this is right if this reads 0 then we don't have sync???
        return false;
    }

    if (tdiff < ECM_DC_SYNC_THRESHOLD) {
        return true;
    } else {
        return false;
    }
}


/**
 * @brief runs network scanning program
 * @param apply_pdo_mapping_before_netscan
 */
void ECNetscan(const bool apply_pdo_mapping_before_netscan) {

#ifdef GB_APP_LINUX
    ecm_netscan(eth_interface1, apply_pdo_mapping_before_netscan);
#else
    ecm_netscan();
#endif

    //copy the contents of the ec_slave struct to the ecm_status struct (shared mem to gbc-m4 for gui)
    copy_ec_slave_to_ecm_status();
    ecm_status.net_scan_state = ECM_NET_SCAN_FINISHED;
    UM_INFO(GBEM_UM_EN, "GBEM: net scan finished");

#ifndef GB_APP_LINUX
    vTaskDelete( NULL);
#else
    exit(EXIT_SUCCESS);
#endif
}


gberror_t ec_ack_errors(void) {

    ec_group[currentgroup].docheckstate = FALSE;
    ec_readstate();

    bool ec_state_error = false;

    for (int i = 1; i <= ec_slavecount; i++) {

        ec_group[currentgroup].docheckstate = TRUE;
        printf("%d - %u\n", i, ec_slave[i].state);

        if (ec_slave[i].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR)) {
            UM_WARN(GBEM_UM_EN, "GBEM: Slave [%d] is in SAFE_OP + ERROR, attempting error ack", i);
            ec_slave[i].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
            ec_writestate(i);
            ec_state_error = true;
        }

        if (ec_slave[i].state == (EC_STATE_PRE_OP + EC_STATE_ERROR)) {
            UM_WARN(GBEM_UM_EN, "GBEM: Slave [%d] is in PRE_OP + ERROR, attempting error ack", i);
            ec_slave[i].state = (EC_STATE_PRE_OP + EC_STATE_ACK);
            ec_writestate(i);
            ec_state_error = true;
        }

        if (ec_slave[1].state == EC_STATE_NONE) {
            UM_WARN(GBEM_UM_EN, "GBEM: Slave [%d] is in STATE NONE", i);
            ec_state_error = true;
        }

    }


    return ec_state_error;

}

/**
 * @brief Task to check state of EtherCAT slaves and try and recover them
 * @param argument (not used)
 * @warning if this is triggered the cyclic task is stopped from running
 */
void ec_check(void *argument) {
    (void) argument;
    int slave;
//    int test_count=0;
    while (1) {
//        test_count++;

//        if (test_count==200){
//            printf("SETTING WKC TO ZERO!\n");
//            wkc =0;
//        }


        if (ec_rxtx_mode == EC_RXTX_MODE_OP || ec_rxtx_mode == EC_RXTX_MODE_HOME) {
            if (((wkc < expectedWKC) || ec_group[currentgroup].docheckstate)) {
                UM_WARN(GBEM_UM_EN, "GBEM: ec_check found a possible EtherCAT error. Checking states...");
                ecm_status.cyclic_state = ECM_ERROR;
                //notify the cyclic task we have an issue do this with a global var
                ecm_status.ec_check_found_error = true;
                //update slave states in ecm_status
                for (int i = 1; i <= ec_slavecount; i++) {
                    ecm_status.map[i].state = ec_slave[i].state;
                    //                if(ec_slave[i].state != EC_STATE_SAFE_OP)
                    //                {
                    //                    UM_WARN(GBEM_UM_EN, "GBEM: Slave [%d], State [%2x], StatusCode [%4x : %s]",
                    //                           i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    //                }
                }

                /* one or more slaves are not responding */
                ec_group[currentgroup].docheckstate = FALSE;
                ec_readstate();

                for (slave = 1; slave <= ec_slavecount; slave++) {
                    if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL)) {
                        ec_group[currentgroup].docheckstate = TRUE;
                        if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR)) {
                            UM_ERROR(GBEM_UM_EN, "GBEM: Slave [%d] is in SAFE_OP + ERROR, attempting ack", slave);

                            ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                            ec_writestate(slave);
                        } else if (ec_slave[slave].state == EC_STATE_SAFE_OP) {
                            UM_WARN(GBEM_UM_EN, "GBEM: Slave [%d] is in SAFE_OP, changing to OPERATIONAL", slave);
                            ec_slave[slave].state = EC_STATE_OPERATIONAL;
                            ec_writestate(slave);
                        } else if (ec_slave[slave].state > EC_STATE_NONE) {
                            if (ec_reconfig_slave(slave, EC_TIMEOUTMON)) {
                                ec_slave[slave].islost = FALSE;
                                UM_WARN(GBEM_UM_EN, "GBEM: Slave [%d] reconfigured", slave);
                            }
                        } else if (!ec_slave[slave].islost) {
                            /* re-check state */
                            ec_statecheck(slave, EC_STATE_OPERATIONAL,
                                          EC_TIMEOUTRET);
                            if (ec_slave[slave].state == EC_STATE_NONE) {
                                ec_slave[slave].islost = TRUE;

                                UM_ERROR(GBEM_UM_EN, "GBEM: Slave [%d] lost", slave);

                            }
                        }
                    }
                    if (ec_slave[slave].islost) {
                        if (ec_slave[slave].state == EC_STATE_NONE) {
                            if (ec_recover_slave(slave, EC_TIMEOUTMON)) {
                                ec_slave[slave].islost = FALSE;
                                UM_INFO(GBEM_UM_EN, "GBEM: Slave [%d] recovered", slave);
                            }
                        } else {
                            ec_slave[slave].islost = FALSE;
                            UM_INFO(GBEM_UM_EN, "GBEM: Slave [%d] found", slave);
                        }
                    }
                }
                if (!ec_group[currentgroup].docheckstate) {
                    UM_INFO(GBEM_UM_EN, "GBEM: all slaves are in OPERATIONAL state");
                }
            } else {
                ecm_status.cyclic_state = ECM_CYCLIC_RUNNING;
                ecm_status.ec_check_found_error = false;
            }
        }
        //delay before we tick round and run the error check again
#ifndef GB_APP_LINUX
        vTaskDelay(50);
#else
        //500ms
        usleep(500000);
#endif
    }
}

/**
 * @brief copy the content of ec_slave struct to ecm_status in shared memory for display in the gui
 * also copies ec_slavecount
 */
void copy_ec_slave_to_ecm_status(void) {
    ecm_status.slavecount = ec_slavecount;
    for (int i = 1; i <= ec_slavecount; i++) {
        ecm_status.map[i].hasdc = ec_slave[i].hasdc;
        ecm_status.map[i].configadr = ec_slave[i].configadr;
        ecm_status.map[i].Ibits = ec_slave[i].Ibits;
        ecm_status.map[i].Obits = ec_slave[i].Obits;
        ecm_status.map[i].Ibytes = ec_slave[i].Ibytes;
        ecm_status.map[i].Ibytes = ec_slave[i].Ibytes;
        ecm_status.map[i].state = ec_slave[i].state;
        memcpy(&ecm_status.map[i].name, ec_slave[i].name, EC_MAXNAME + 1);
        ecm_status.map[i].ALstatuscode = ec_slave[i].ALstatuscode;
    }
}

/**
 * @brief function called by the PREOP->SAFEOP hooks ec_slave[n].PO2SOconfig = custom_slave_config
 * @param slave
 * @return
 */
static int custom_slave_config(uint16 slave) {
    static bool pdo_map_failure = false;
    static bool apply_standard_sdos_failure = false;

    UM_INFO(GBEM_UM_EN, "GBEM: Running custom_slave_config for slave [%u] (PREOP->SAFEOP hook)", slave);

    //if the function pointer for the current slave is not null call it
//    printf("slave for fp %u", slave);
    if (*map_slave_pdo_map_function_ptr[slave - 1] != NULL) {
        if ((*map_slave_pdo_map_function_ptr[slave - 1])(slave) == E_SUCCESS) {
            UM_INFO(GBEM_UM_EN, "GBEM: PDO mapping succeeded for slave [%u]", slave);
        } else {
            UM_ERROR(GBEM_UM_EN, "GBEM: PDO mapping failed for slave [%u]", slave);
            pdo_map_failure = true;
        }
    }

    if (*map_slave_standard_sdo_function_ptr[slave - 1] != NULL) {
        if ((*map_slave_standard_sdo_function_ptr[slave - 1])(slave) == E_SUCCESS) {
            UM_INFO(GBEM_UM_EN, "GBEM: applying standard SDOs succeeded for slave [%u]", slave);
        } else {
            UM_ERROR(GBEM_UM_EN, "GBEM: applying standard SDOs failed for slave [%u]", slave);
            apply_standard_sdos_failure = true;
        }
    }

    if (slave == MAP_NUM_SLAVES) {
        //we are on the last slave
        if (!pdo_map_failure) {
            UM_INFO(GBEM_UM_EN, "GBEM: Boot step 4.1 <success> (PDO mapping)");
            ecm_status.boot_state.pdo_remap_done = true;
        } else {
            UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 4.1 <failure> (PDO mapping failed for one or more slaves)");
            ecm_status.boot_state.pdo_remap_done = false;
        }

        if (!apply_standard_sdos_failure) {
            UM_INFO(GBEM_UM_EN, "GBEM: Boot step 4.2 <success> (applying standard SDOs)");
            ecm_status.boot_state.apply_standard_sdos_done = true;
        } else {
            UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 4.2 <failure> applying standard SDOs failed for one or more slaves");
            ecm_status.boot_state.apply_standard_sdos_done = false;
        }
    }

    bool applied_dc = false;
    /* apply DC config */

    if (map_dc_type[slave - 1] == EC_DC_0) {
//        ec_dcsync0(slave, TRUE, (uint32_t)map_dc_cycle[slave -1] * 1000000, ECM_CYCLE_SHIFT);
        ec_dcsync0(slave, TRUE, ECM_CYCLE_TIME, ECM_CYCLE_SHIFT);
        UM_INFO(GBEM_UM_EN, "GBEM: Applying DC 0 in PO->SO hook for slave [%d]", slave);
        applied_dc = true;
    }
    if (map_dc_type[slave - 1] == EC_DC_01) {
        ec_dcsync01(slave, TRUE, ECM_CYCLE_TIME, ECM_CYCLE_TIME, ECM_CYCLE_SHIFT);

//        ec_dcsync01(slave, TRUE, (uint32_t)map_dc_cycle[slave -1] * 1000000,(uint32_t)map_dc_cycle[slave -1] * 1000000, ECM_CYCLE_SHIFT);
        UM_INFO(GBEM_UM_EN, "GBEM: Applying DC 01 in PO->SO hook for slave [%d]", slave);
        applied_dc = true;
    }
    if (applied_dc == false) {
        UM_INFO(GBEM_UM_EN, "GBEM: No DC applied in PO->SO hook for slave [%d]", slave);
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: DC >was< applied in PO->SO hook for slave [%d]", slave);
    }

    //rc is not used
    return 0;
}

/**
 * @brief print out local dc config (set after DC config is called)
 * @param slave
 * @return gberror
 */
static gberror_t print_slave_dc_local_config(const uint16_t slave) {
    UM_INFO(GBEM_UM_EN, "GBEM: Slave [%d], DCactive [%d], DCnext [%d], DCprevious [%d], DCcycle [%d], DCshift [%d]",
            slave, ec_slave[slave].DCactive, ec_slave[slave].DCnext, ec_slave[slave].DCprevious,
            ec_slave[slave].DCcycle, ec_slave[slave].DCshift);
    return E_SUCCESS;
}

dc_clock_type_t check_dc_clock_type(const uint16_t slave) {

    int local_wc;

    uint16_t reg_0910, reg_0912, reg_0914, reg_0916;

    bool dc_word_present[4] = {false};

    local_wc = ec_FPRD(ec_slave[slave].configadr, 0x0910, sizeof(reg_0910), &reg_0910, EC_TIMEOUTRET);
    if (local_wc == 0) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: error reading EtherCAT register 0x0910");
        return DC_CLOCK_ERROR;
    } else {
        if (reg_0910 != 0) {
            dc_word_present[0] = true;
        }
//        printf("0x910 [%u]\n", reg_0910);
    }

    local_wc = ec_FPRD(ec_slave[slave].configadr, 0x0912, sizeof(reg_0912), &reg_0912, EC_TIMEOUTRET);
    if (local_wc == 0) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: error reading EtherCAT register 0x0912");
        return DC_CLOCK_ERROR;
    } else {
        if (reg_0912 != 0) {
            dc_word_present[1] = true;
        }
//        printf("0x912 [%u]\n", reg_0912);
    }

    local_wc = ec_FPRD(ec_slave[slave].configadr, 0x0914, sizeof(reg_0914), &reg_0914, EC_TIMEOUTRET);
    if (local_wc == 0) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: error reading EtherCAT register 0x0914");
        return DC_CLOCK_ERROR;
    } else {
        if (reg_0914 != 0) {
            dc_word_present[2] = true;
        }
//        printf("0x914 [%u]\n", reg_0914);
    }

    local_wc = ec_FPRD(ec_slave[slave].configadr, 0x0916, sizeof(reg_0916), &reg_0916, EC_TIMEOUTRET);
    if (local_wc == 0) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: error reading EtherCAT register 0x0916");
        return DC_CLOCK_ERROR;
    } else {
        if (reg_0916 != 0) {
            dc_word_present[3] = true;
        }
//        printf("0x916 [%u]\n", reg_0916);
    }


    if ((dc_word_present[0] || dc_word_present[1]) && (dc_word_present[2] || dc_word_present[3])) {
//        printf("DC clock 64\n");
        return DC_CLOCK_64;

    }

    if ((dc_word_present[0] || dc_word_present[1]) && (!dc_word_present[2] && !dc_word_present[3])) {
//        printf("DC clock 32\n");
        return DC_CLOCK_32;

    }
//    printf("DC clock none\n");
    return DC_CLOCK_NONE;


}

/**
 * @brief disable the Sync Manager for a specific slave
 * @param slave
 * @param sync_manager
 * @return
 */
__attribute__((unused)) static bool disable_sync_managers  (const uint16_t slave, const uint8_t sync_manager)  {
    ec_slave[slave].SM[sync_manager].StartAddr = 0;
    return true;
}

/**
 * @brief reads the 0x1c32 Sync Manager (SM) standard parameter objects for a slave
 * @param slave - number of slave
 * @return gberror
 */
gberror_t print_1c32(const uint16_t slave) {
    bool read_error = false;
    uint16_t uib16;
    uint32_t uib32;
    static const char *const ec_sync_mode_strings[] = {"Free Run", "Synchron with SM 2 Event",
                                                       "DC-Mode - Synchron with SYNC0 Event",
                                                       "DC-Mode - Synchron with SYNC1 Event"};
// Sync mode
// 0 Free Run
//1 Synchron with SM 2 Event
//2 DC-Mode - Synchron with SYNC0 Event
//3 DC-Mode - Synchron with SYNC1 Even

//0x1c32 = SM2
//0x1c33 - SM3


// read 1C32:01 unit16 for sync mode
    if (ec_sdo_read_uint16(slave, 0x1c32, 0x1, &uib16)) {
        if (uib16 <= 2) {
            UM_INFO(GBEM_GEN_LOG_EN, "GBEM: Sync mode read from slave [%d] (0x1c32:1) is [%s]", slave,
                    ec_sync_mode_strings[uib16]);
        }
    }else{
        read_error=true;
    }


//    read 0x1c32:02 uint32 for cycle time

    if (ec_sdo_read_uint32(slave, 0x1c32, 0x2, &uib32)) {
        UM_INFO(GBEM_UM_EN, "GBEM: Cycle time read from slave(%d) (0x1c32:2) is: [%d]", slave, uib32);
    }
    else{
        read_error=true;
    }



// read 1C32:03 unit32 for shift time
    if (ec_sdo_read_uint32(slave, 0x1c32, 0x3, &uib32)) {
        UM_INFO(GBEM_UM_EN, "GBEM: Shift time read from slave [%d] (0x1c32:3) is [%d]", slave, uib32);
    }
    else{
        read_error=true;
    }


    // read 1C32:06 unit32 for calc and copy time
    if (ec_sdo_read_uint32(slave, 0x1c32, 0x6, &uib32)) {
        UM_INFO(GBEM_UM_EN, "GBEM: Calc and copy time read from slave [%d] (0x1c32:6) is [%d]", slave, uib32);
    }
    else{
        read_error=true;
    }


    if (read_error){
        return E_REGISTER_READ_FAILURE;
    }

    return E_SUCCESS;
}

/*
 * i->p (init to pre-op_
 * p->s (pre-op to safe-op) and the reverse s->p
 * s->o (safe-op to op) and the reverse o->s
 */


/**
 * @brief Initialise EtherCAT bind socket to ifname
 * @return
 */
bool ec_step_1_init(void) {
#ifdef GB_APP_LINUX
/* if we want to use the redundant EtherCAT interface we need two interfaces here is where we enable this */
#if USE_REDUNDANT_ETHERNET_PORTS == 1
    if (ec_init_redundant(eth_interface1, eth_interface2)){
#else
    if (ec_init(eth_interface1)) {
#endif

#else
        if (ec_init(NULL)) {
#endif
        UM_INFO(GBEM_UM_EN, "GBEM: Boot step 1 >success< (initialisation and Ethernet IF config)");
        return true;
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 1 >failure<  (initialisation and Ethernet IF config)");
        return false;
    }
}


/**
 * @brief runs slave config it and then assigns PO2SO hook functions
 * @return
 */
bool ec_step_2_config(void) {

    if (ec_config_init(FALSE)) {

        if (ec_slavecount == 0) {
            UM_ERROR(GBEM_UM_EN, "GBEM: We failed to find any slaves on the EtherCAT network");
            return false;
        }

        UM_INFO(GBEM_UM_EN, "GBEM: We found [%d] slaves on the EtherCAT network", ec_slavecount);

        if (ec_slavecount!=MAP_NUM_SLAVES){
            UM_FATAL("GBEM: The number of slaves found on the EtherCAT network [%d] does not match the number configured in the machine map (MAP_NUM_SLAVES) [%d]", ec_slavecount, MAP_NUM_SLAVES);
        }




        /* Here we loop over all slaves in the map and assign a function (custom_slave_config) to their transition from PreOp to SafeOp */

/* The PreOP to SafeOP configuration hook must be done per slave and should be set before calling
 * the configuration and mapping of process data, e.g. the call to ec_config_map. Setting the configuration
 * hook ensure that the custom configuration will be applied when calling recover and re-configuration
 * of a slave.
 */
        for (int i = 1; i <= MAP_NUM_SLAVES; i++) {
            ec_slave[i].PO2SOconfig = custom_slave_config;
        }
/*copy the contents of the ec_slave struct to shared mem (the ecm_status struct is used by the UI and needs basic info e.g. slave names */
        copy_ec_slave_to_ecm_status();

        UM_INFO(GBEM_UM_EN, "GBEM: Boot step 2 >success< (search for and initialise EtherCAT slaves)");
        return true;

    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 2 >failure< (search for an init slaves)");
        UM_ERROR(GBEM_UM_EN, "GBEM: No slaves were found on the EtherCAT network");
        return false;
    }
}

/**
 * @brief transitions all slaves to PRE OP
 * @return
 */
bool ec_step_3_preop(void) {
    ec_slave[0].state = EC_STATE_PRE_OP;
    ec_writestate(0);

#ifdef GB_APP_LINUX
    usleep(200);
#else
    vTaskDelay(100);
#endif

    /* '0' here addresses all slaves */
    if (ec_statecheck(0, EC_STATE_PRE_OP, 8 * EC_TIMEOUTSTATE) == EC_STATE_PRE_OP) {
        UM_INFO(GBEM_UM_EN, "GBEM: Boot step 3 >success< (transition all slaves to PRE OP state)");

        if (ec_configdc()) {
            UM_INFO(GBEM_UM_EN, "GBEM: DC capable slaves have been found");
        } else {
            UM_INFO(GBEM_UM_EN, "GBEM: No DC capable slaves found");
        }

        /* Next we will create an IOmap and configure the SyncManager's and FMMU's to link the EtherCAT master and the slaves. */
        int usedmem = ec_config_map(&IOmap);
        /* The above step also requests a transition of the slaves to SAFE OP so the next request is a bit superfluous*/
        if (usedmem >= ECM_IO_MAP_SIZE) {
            UM_FATAL("GBEM: The size of slave io map is greater than the maximum we allow (increase ECM_IO_MAP_SIZE)");
        }
        return true;
#ifdef GB_APP_LINUX
//            usleep(500000);
        sleep(3);
#else
        vTaskDelay(1000);
#endif

    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 3 >failure< (transition all slaves to PRE OP state)");

        return false;
    }
}

/**
 * @brief check for any EtherCAT error
 * @return
 */
bool ec_step_5_error_check(void) {

    int ec_boot_ecaterror_count = 0;
    while (EcatError) {
        ec_boot_ecaterror_count++;
        UM_ERROR(GBEM_UM_EN, "GBEM: EtherCAT error detected [%s]", ec_elist2string());
#ifdef GB_APP_LINUX
        usleep(500000);
#else
        vTaskDelay(5000);
#endif
        if (ec_boot_ecaterror_count > 20) {
            break;
        }
    }
    if ((ec_boot_ecaterror_count > 20)) {
        UM_INFO(GBEM_UM_EN,
                "GBEM: Excessive number of EtherCAT slave error messages (>%d) detected during boot (so not all have been outputted)",
                20);
    }
    if (ec_boot_ecaterror_count > 0) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 5 >failure< (Check for any slaves reporting errors)");
        return false;
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Boot step 5 >success< (Check for any slaves reporting errors)");
        return true;
    }
}

/**
 * @brief transistions all slaves to SAFE OP, sets ec_rxtx_dorun to start cyclic exchange of messages
 * @return
 */
bool ec_step_6_safeop(void) {

    ec_slave[0].state = EC_STATE_SAFE_OP;
    ec_writestate(0);

    /* '0' here addresses all slaves */
    if (ec_statecheck(0, EC_STATE_SAFE_OP, (EC_TIMEOUTSTATE * 8)) == EC_STATE_SAFE_OP) {
        UM_INFO(GBEM_UM_EN, "GBEM: Boot step 6 >success< (transition all slaves to SAFE OP state)");

        /*do run triggers the ec_rxtx thread to start sending and receiving process data */
        ec_rxtx_mode = EC_RXTX_MODE_DORUN;
        return true;
        /* To enter state OP we need to send valid data to outputs (at least once?) */
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 6 >failure< (transition all slaves to SAFE OP state)");
        ec_readstate();
        for (int i = 1; i <= ec_slavecount; i++) {
            if (ec_slave[i].state != EC_STATE_SAFE_OP) {
                UM_INFO(GBEM_UM_EN, "GBEM: slave not in safe op is Slave [%d] State [%s] StatusCode [%4x : %s]", i, ec_state_to_string[ec_slave[i].state], ec_slave[i].ALstatuscode,
                        ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
            }
        }

        return false;
    }
}

/**
 * @brief checks EtherCAT workcounter matches expected
 * @return
 */
bool ec_step_7_wkc_check(void) {
#ifdef GB_APP_LINUX
    sleep(1);
#else
    //        vTaskDelay(3000);
#endif


    int temp_wkc = ec_receive_processdata(EC_TIMEOUTRET);
    expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
    UM_INFO(GBEM_UM_EN, "GBEM: We have a wkc of [%d], and an expectedWKC of [%d] (these are the EtherCAT working counters",
            temp_wkc,
            expectedWKC);

    if (temp_wkc != expectedWKC) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 7 >failure< (check working counters match)");
        return false;
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Boot step 7 >success< (check working counters match)");

        /* Read all slave states in ec_slave */
        //todo not sure why we do this?
        ec_readstate();
        return true;
    }
}



/**
 * @brief Runs slave EEP check
 * @return
 */
bool ec_step_8_slaves_match(void) {
    /* now we want to make sure the slaves are the ones we have hard coded in our map and occur in the right order */
    gberror_t grc = E_GENERAL_FAILURE;

    grc=ec_slaves_match();

    if (grc==E_SUCCESS) {
        UM_INFO(GBEM_UM_EN, "GBEM: Boot step 8 >success< (check all slaves match the configuration)");
        return true;
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 8 >failure< (check all slaves match the configuration)");
        UM_ERROR(GBEM_UM_EN,
                 "GBEM: Not all slaves match the configuration (this is the array of slave names manufacturer ids etc. in the machine config");
        UM_ERROR(GBEM_UM_EN,
                 "GBEM: Error from slave match [%s]", gb_strerror(grc));

        return false;
    }
}

/**
 * @brief transitions all slaves to OPERATIONAL
 * @return
 */
bool ec_step_9_op(void) {
    ec_slave[0].state = EC_STATE_OPERATIONAL;
    /* request OP state for all slaves */
    ec_writestate(0);

    /* wait for all slaves to reach OP state */
    /* this is a blocking call with a timeout */
    int chk = 40;
    do {
        ec_statecheck(0, EC_STATE_OPERATIONAL, 50000); // 50 ms wait for state check
    } while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));


    if (ec_statecheck(0, EC_STATE_OPERATIONAL, (5 * EC_TIMEOUTSTATE)) == EC_STATE_OPERATIONAL) {
        UM_INFO(GBEM_UM_EN, "GBEM: Boot step 9 >success< (transition all slaves to OP state)");
#ifdef GB_APP_LINUX
        sleep(1);
#else
        //            vTaskDelay(1000);
#endif


#ifdef GB_APP_LINUX
        sleep(2);
#else
        //            vTaskDelay(2000);
#endif

        return true;
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: Boot step 9 >failure< (transition all slaves to OP state)");
        return false;
    }
}


/**
 * @brief function to boot EtherCAT network
 * @param argument
 */
void ECBoot(void *argument) {
    int rc;
    bool ec_boot_proceed = false;



//placeholder in case we want to run different modes in ECM_CYCLIC_PROG
    if (*((int *) argument) == 0) {
        ecm_status.active_program = ECM_CYCLIC_PROG;
    } else if (*((int *) argument) == 1) {
        UM_FATAL("argument invalid");
    }

    /******************** this is the start of the boot phase ********************/
    /* this is a goto label that we return to if boot fails */
    boot_start_goto_label:
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                    Start of cyclic boot process                    ***");
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
    ecm_status.cyclic_state = ECM_PRE_BOOT;
#ifdef GB_APP_LINUX
    sleep(2);
#else
    vTaskDelay(2000);
#endif

    /* first let's try and initialise the network adapter */
    /* we need to call this even with a single hardcoded NIC as it assigns a load of pointers */
    ecm_status.boot_state.init_done = ec_step_1_init();
    ec_boot_proceed = ecm_status.boot_state.init_done;
    /* now let's try and Enumerate and init all slaves.*/
    if (ec_boot_proceed) {
        ecm_status.boot_state.slaves_found = ec_step_2_config();
        ec_boot_proceed = ecm_status.boot_state.slaves_found;
    }


    /* now we ask the slaves to reach PRE_OP state and wait while they do this*/
    if (ec_boot_proceed) {
        retry_step3:
        ecm_status.boot_state.all_slaves_pre_op = ec_step_3_preop();
        ec_boot_proceed = ecm_status.boot_state.all_slaves_pre_op;
    }

    //if we failed to get into pre-op we can try acknowledging errors on the slaves
    int error_ack_count = 0;
    bool slave_in_error_state = false;
    if (!ec_boot_proceed && ecm_status.boot_state.slaves_found) {
        while (1) {
            slave_in_error_state = ec_ack_errors();
            if (!slave_in_error_state) {
                break;
            }
            sleep(1);
            error_ack_count++;
            if (error_ack_count > 5) {
                UM_ERROR(GBEM_UM_EN, "GBEM: Slaves were in a error state and we tried acknowledging the errors but the remained in the error state");
                break;
            }
        }

        if (!slave_in_error_state) {
            goto retry_step3;
        }
    }



    //here the hooks kick in
//    ecm_status.boot_state.pdo_remap_done  && ecm_status.boot_state.apply_standard_sdos_done are set in the state change hook
    ec_boot_proceed =
            ec_boot_proceed && ecm_status.boot_state.pdo_remap_done &&
            ecm_status.boot_state.apply_standard_sdos_done;

    /* now check for any ethercat errors */
    if (ec_boot_proceed) {
        ecm_status.boot_state.error_check_ok = ec_step_5_error_check();
        ec_boot_proceed = ecm_status.boot_state.error_check_ok;
    }
    /* now we ask the slaves to reach SAFE_OP state and wait while they do this*/
    if (ec_boot_proceed) {
        ecm_status.boot_state.all_slaves_safe_op = ec_step_6_safeop();
        ec_boot_proceed = ecm_status.boot_state.all_slaves_safe_op;
    }

    gberror_t gbrc;
    sleep(5);
    if (ec_boot_proceed) {


    }
    if (ec_boot_proceed) {
        ecm_status.boot_state.wkc_check_ok = ec_step_7_wkc_check();
        ec_boot_proceed = ecm_status.boot_state.wkc_check_ok;
    }
    if (ec_boot_proceed) {
        ecm_status.boot_state.slaves_match_ok = ec_step_8_slaves_match();
        ec_boot_proceed = ecm_status.boot_state.slaves_match_ok;
    }
    if (ec_boot_proceed) {
        ecm_status.boot_state.all_slaves_op = ec_step_9_op();
        ec_boot_proceed = ecm_status.boot_state.all_slaves_op;
    }


//    bool have_sync = false;
//
//    for (int i = 1; i <= MAP_NUM_SLAVES; i++) {
//
//        check_slave_dc_sysdiff_less_than_threshold(i);
    //
//        if (map_dc_type[i-1]!=EC_DC_NONE){
//            while (!check_slave_dc_sysdiff_less_than_threshold(i)){
//                printf("getting sync for slave [%u]\n", i);
//                sleep(1);
//            }
//
//        }
//    }

    sleep(10);

    if (ec_boot_proceed) {

        dc_clock_type_t dc_clock_type;
        /* think we can now print the slave's dc * 1c32 config and check DC clock support */
        for (int i = 1; i <= MAP_NUM_SLAVES; i++) {

            dc_clock_type = check_dc_clock_type(i);
            switch (dc_clock_type) {
                case DC_CLOCK_ERROR:
                    UM_ERROR(GBEM_UM_EN, "GBEM: Could not read DC clock type from slave [%u]", i);
                    break;
                case DC_CLOCK_NONE:
                    UM_INFO(GBEM_UM_EN, "GBEM: Slave [%u] does not support DC clock", i);
                    break;
                case DC_CLOCK_32:
                    UM_WARN(GBEM_UM_EN, "GBEM: Slave [%u] supports a 32 bit DC clock", i);
                    break;
                case DC_CLOCK_64:
                    UM_INFO(GBEM_UM_EN, "GBEM: Slave [%u] supports a 64 bit DC clock", i);
                    break;
                default:
                    UM_ERROR(GBEM_UM_EN, "GBEM: Error in establishing DC clock type for slave [%u]", i);
            }

            print_slave_dc_local_config(i);
            gbrc = print_1c32(i);
            if (gbrc != E_SUCCESS) {
                UM_WARN(GBEM_UM_EN, "GBEM: Could not print 1c32 information for slave [%u]. Error [%s]", i,
                        gb_strerror(gbrc));
                UM_WARN(GBEM_UM_EN, "GBEM: If you have debug logging enabled, you will see some failed SDO read messages in the proceeding console lines");
            }
        }
    }
    /* !! Once we are in operational mode it is ESSENTIAL that we keep sending process data to avoid the slave's watchdog (WD) being activated
     * I think this occurs on the SM not receiving data within 100ms
     */

    /******************** this is the end of the boot phase ********************/

//let's take stock of what happened in the boot process
    if (ec_boot_proceed) {
        UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
        UM_INFO(GBEM_UM_EN, "GBEM: ***                  The boot process was successful                   ***");
        UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
        ecm_status.boot_state.boot_sucessful = true;

        //This calls a function that can be defined for each drive to print out useful config information
        for (int i = 0; i < MAP_NUM_DRIVES; i++) {
            if (*map_drive_print_params_function_ptr[i] != NULL) {
                map_drive_print_params_function_ptr[i](i);
            } else {
                LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                         "GBEM: Missing function pointer for map_drive_print_params on drive [%u]", i);
            }
        }

        bool any_drive_needs_homing = false;
        for (int i = 0; i < MAP_NUM_DRIVES; i++) {
            if (map_drive_run_homing[i]) {
                any_drive_needs_homing = true;
            }
        }

        if (any_drive_needs_homing) {
            ec_rxtx_mode == EC_RXTX_MODE_HOME;
        } else {

            ec_rxtx_mode = EC_RXTX_MODE_OP;
        }
        ecm_status.cyclic_state = ECM_BOOT_FINISHED;
    } else {
//We have had an issue during the boot phase
        sleep(5);


        ecm_status.boot_state.boot_sucessful = false;
        UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
        UM_ERROR(GBEM_UM_EN, "GBEM: ***     The boot process failed - we will start again at Step 1        ***");
        UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");

        /* jump back to the start and try whole boot process again */
        goto boot_start_goto_label;
    }

    //fills out ecm_status with things like MAP_NUM_DRIVES
    init_ecm_status();

#ifndef GB_APP_LINUX
    //    xReturned = xTaskCreate(ec_heck, "ec_check_task", configMINIMAL_STACK_SIZE, NULL, EC_CHECK_TASK_PRIO, NULL);
//    if (xReturned != pdPASS) {
//        UM_FATAL("GBEM: freertos could not create the ec_check task, this is very bad indeed");
//    }

#else

    rc = osal_thread_create_rt(&thread_ec_check, STACK64K * 2, &ec_check, NULL);
    if (rc != 1) {
        UM_FATAL(
                "GBEM: An error occurred whilst creating the pthread (ec_check which is the thread used to check slave statuses) and GBEM will exit. This error message implies that a Linux system call (pthread_create) has failed. This could be because the system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process would be exceeded. Neither of these should occur normally. Something bad has happened deep down");
    }
#endif


#ifdef GB_APP_LINUX
#if ENABLE_EMSTAT == 1
    rc = osal_thread_create_rt(&thread_ec_emstat, STACK64K * 2, &ec_emstat, NULL);
    if (rc != 1) {
        UM_FATAL(
                "GBEM: An error occurred whilst creating the pthread (ec_emstat which is the thread to create JSON status messages) and GBEM will exit. This error message implies that a Linux system call (pthread_create) has failed. This could be because the system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process would be exceeded. Neither of these should occur normally. Something bad has happened deep down");
    }
#endif
#else
    vTaskDelete(NULL);

#endif

//this is needed ONLY if the display / ecmstat is disabled
//    sleep(10000);

}





/**
 * @brief runs through the slaves defined in the map and checks their name (and) man, id, ver against the EEprom and checks order
 * @return true: slaves on network matches the map (good), false slaves don't map the network (bad)
 */
gberror_t ec_slaves_match(void) {

    if (ec_slavecount < MAP_NUM_SLAVES) {

        return E_NOT_ENOUGH_SLAVES;
    }
    /* Verify slave by slave that it is correct*/
    /* strcmp returns 0 if strings are identical */


    /* see these #defs at the top of this file
     * #define ECM_CHECK_EEP_MAN
     * #define ECM_CHECK_EEP_REV
     * #define ECM_CHECK_EEP_ID
     * */

    for (int i = 0; i < MAP_NUM_SLAVES; i++) {

        if (strcmp(ec_slave[i + 1].name, ecm_slave_map[i].name)) {
            return E_SLAVE_NAME_MATCH_FAILURE;
        }
#ifdef ECM_CHECK_EEP_MAN
        if (ecm_slave_map[i].eep_man==0){
            LL_WARN(GBEM_GEN_LOG_EN, "GBEM: ECM_CHECK_EEP_MAN is defined but slave [%u] has a 0 manufacturer");
        }
        if (ec_slave[i + 1].eep_man != ecm_slave_map[i].eep_man) {
            return E_SLAVE_EEP_MAN_MATCH_FAILURE;
        }
#endif

#ifdef ECM_CHECK_EEP_ID
        if (ecm_slave_map[i].eep_id==0){
            LL_WARN(GBEM_GEN_LOG_EN, "GBEM: ECM_CHECK_EEP_ID is defined but slave [%u] has a 0 id");
        }

        if (ec_slave[i + 1].eep_id != ecm_slave_map[i].eep_id) {
            return E_SLAVE_EEP_ID_MATCH_FAILURE;
        }
#endif
#ifdef ECM_CHECK_EEP_REV
        if (ecm_slave_map[i].eep_rev==0){
            LL_WARN(GBEM_GEN_LOG_EN, "GBEM: ECM_CHECK_EEP_REV is defined but slave [%u] has a 0 revision id");
        }
        if (ec_slave[i + 1].eep_rev != ecm_slave_map[i].eep_rev) {
            return E_SLAVE_EEP_REV_MATCH_FAILURE;
        }
#endif
    }
    return E_SUCCESS;
}

///** array mapping ec state to text descriptions*/
//const char *ec_state_to_string[] = {
//        [0x0] = "EC State: None",
//        [0x01] = "EC State: Init ",
//        [0x02] = "EC State: Pre-op",
//        [0x03] = "EC State: Boot",
//        [0x04] = "EC State: Safe-op",
//        [0x08] = "EC State: Operational",
//        [0x10] = "EC State: Error Ack",
//        [0x11] = "EC State: Init error active",
//        [0x12] = "EC State: Pre-op error active",
//        [0x14] = "EC State: Safe-op error active",
//        [0x18] = "EC State: Op error active",
//        [0x20] = "EC State: ?"
//};

//typedef enum
//{
//    /** No valid state. */
//    EC_STATE_NONE           = 0x00,
//    /** Init state*/
//    EC_STATE_INIT           = 0x01,
//    /** Pre-operational. */
//    EC_STATE_PRE_OP         = 0x02,
//    /** Boot state*/
//    EC_STATE_BOOT           = 0x03,
//    /** Safe-operational. */
//    EC_STATE_SAFE_OP        = 0x04,
//    /** Operational */
//    EC_STATE_OPERATIONAL    = 0x08,
//    /** Error or ACK error */
//    EC_STATE_ACK            = 0x10,
//    EC_STATE_ERROR          = 0x10
//} ec_state;




/**
 * @brief initialise some elements in the ecm_status struct to the hash defined values
 */
void init_ecm_status(void) {
    ecm_status.drive_count = MAP_NUM_DRIVES;
    ecm_status.gbc_max_float_in_count = DPM_NUM_ANALOGS;
    ecm_status.gbc_max_float_out_count = DPM_NUM_ANALOGS;
    ecm_status.gbc_max_int32_in_count = DPM_NUM_INT32S;
    ecm_status.gbc_max_int32_out_count = DPM_NUM_INT32S;
    ecm_status.gbc_max_uint32_in_count = DPM_NUM_UINT32S;
    ecm_status.gbc_max_uint32_out_count = DPM_NUM_UINT32S;
    ecm_status.gbc_max_digital_in_count = DPM_NUM_DIGITALS;
    ecm_status.gbc_max_digital_out_count = DPM_NUM_DIGITALS;


    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (map_drive_to_name[i] == NULL) {
            UM_FATAL(
                    "GBEM: map_drive_names [%u] char* array is empty - initialise the array in the machine configuration",
                    i);
        } else {
            strcpy(ecm_status.drives[i].name, map_drive_to_name[i]);
        }
    }
}

