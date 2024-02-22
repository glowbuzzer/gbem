/**
 ******************************************************************************
 * @file           :  main.c
 * @brief          :  GBEM main
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include "status_control_word_bit_definitions.h"
#include <sys/resource.h>
#include <sys/ioctl.h>    // SIOCGIFFLAGS
#include <errno.h>        // errno
#include <netinet/in.h>   // IPPROTO_IP
#include <net/if.h>       // IFF_*, ifreq
#include "reboot.h"
#include "std_headers.h"
#include "main.h"
#include "log.h"
#include "ethercat.h"
#include "shared.h"
#include "ethercatnetscan.h"
#include "ec_functions.h"
#include "dpm.h"
#include "user_message.h"
#include "map.h"
#include "std_defs_and_macros.h"
#include "control.h"
#include "map_summary.h"
#include "version.h"
#include "ecrxtx.h"
#include "shared_mem_types.h"
#include "limits_ini.h"
#include "debug_ini.h"
#include "machine_config_ini.h"
#include "std_utils.h"
#include "map_SDO_print.h"
#include "nvram.h"
#include "plc_core.h"
#include "gbem_config.h"
#include "linux_shm.h"
#include "optional_slaves.h"
#include "gbem_ctx.h"


#define BOOL_STRING(b) ((b) ? "true" : "false")


/**global var storing the name of the nic read from command lines args */
// char eth_interface1[SIZE_OF_IF_NAME] = {0};
// char eth_interface2[SIZE_OF_IF_NAME] = {0};


/** array for SOEM iomap - this is the key slave comms data storage */
uint8_t IOmap[ECM_IO_MAP_SIZE];

//** this global is used to spit out a summary of the slaves PDO writes - it must be false during normal running else SDO writes will end up on stdout not on the slaves!
bool ec_printSDO = false;

/** global struct containing the status of the ec master */
ecm_status_t ecm_status;

pthread_cond_t cond_ec_rxtx = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_ec_rxtx = PTHREAD_MUTEX_INITIALIZER;


uint16_t slave_to_write_nvram = 0;

/** dpm storage - overlaid with dpm structs*/
uint8_t inA[SIZE_OF_GBC_PDO];
uint8_t inB[SIZE_OF_GBC_PDO];
uint8_t outA[SIZE_OF_GBC_PDO];
uint8_t outB[SIZE_OF_GBC_PDO];


int log_run_level = LOG_LEVEL;

struct shm_msg *shmp;

sem_t *gbc_named_trigger_semaphore;
sem_t *gbc_named_mem_protection_semaphore;
sem_t *gbc_named_offline_mem_protection_semaphore;


/** Define a mutex to synchronize access to console output*/
pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t thread_ec_rxtx;
pthread_t thread_ec_check;
pthread_t thread_ec_reboot;
pthread_t thread_ec_error_message;

/* function forward declarations */

static void main_getopt_usage(void);


/**
 * @brief checks if linux ethernet interface is up
 * @param ifname
 * @return true link up
 *
 * @warning not entirely sure that this portable
 */
bool check_ethernet_link(char *ifname) {
    int socId = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (socId < 0) {
        UM_ERROR(GBEM_UM_EN, "Socket failed. Errno = %d\n", errno);
    }

    struct ifreq if_req;
    (void) strncpy(if_req.ifr_name, ifname, sizeof(if_req.ifr_name) - 1);
    int rv = ioctl(socId, SIOCGIFFLAGS, &if_req);
    close(socId);

    if (rv == -1) {
        UM_ERROR(GBEM_UM_EN, "Ioctl failed. Errno = %d\n", errno);
    }

    return (if_req.ifr_flags & IFF_UP) && (if_req.ifr_flags & IFF_RUNNING);
}

void initialise_ec_circular_slave_error_message(ec_circular_slave_error_message_t *queue) {
    queue->head = 0;
    queue->tail = -1;
    queue->num_slots_full = 0;
}


/**
 * @brief prints usage information for gbem command line
 */
static void main_getopt_usage(void) {
    ec_adaptert *adapter = NULL;
    printf("Usage of GBEM:\n");

    printf("\t-h | -n | -d | -w SLAVENAME | -n | -m | -c -i IFNAME [-p PROCESSNAME]\n\n");
    printf(
        "\t-n | --netscan: run the GBEM netscan program to scan the EtherCAT network including reading the slave EEPROM summary\n");
    printf(
        "\t-m | --netscanwithpdo: run the GBEM netscan program to scan the EtherCAT network after writing PDO re-map SDOs \n");
    printf("\t-d | --confcheck: check and print out GBEM configuration and exit\n");
    printf("\t-c | --cyclic: run the GBEM cyclic program (for normal operation)\n");
    printf("\t-w | --write: write SDOs for a specified slave and the trigger a write to slave's NVRAM\n");
    printf("\t-i | --if: interface to use - this is a NIC interface, e.g. eth0 (mandatory)\n");
    printf("\t-v | --version: show the version of GBEM\n");
    printf("\t-h | --help: GBEM usage information\n");
    printf("\nExample #1: GBEM -n -i eth0 -pGBC-linux = run netscan on eth0\n");
    printf("\nExample #2: GBEM --netscan --if lan1 --process GBC-linux = run cyclic program on lan1\n");
    printf("\nAvailable adapters:\n");


    //todo
    // add write slave firmware
    // add read and write slave eeprom

    adapter = ec_find_adapters();
    while (adapter != NULL) {
        printf("\tDescription : %s, Device name to use: %s\n", adapter->desc, adapter->name);
        adapter = adapter->next;
    }
}


//Catch nasty signals and try and cleanup before exit
void cleanup(int sig) {
    //todo crit - debug remove
    printf("Performing cleanup\n");

    //set status word to zeros to clear things like GBEM_ALIVE bit
    dpm_in->machine_word = 0;

    //todo crit - memset shared mem?
    memset(shmp, 0, sizeof(struct shm_msg));
    /* request INIT state for all slaves */
    if (ecm_status.slavecount > 0) {
        ec_slave[0].state = EC_STATE_INIT;
        ec_writestate(0);
        ec_close();
    }
    exit(1);
}

// called from UM_FATAL
void gb_fatal_cleanup(void) {
    //todo crit
    printf("GBEM: UM_FATAL cleanup routine\n");
    cleanup(0);
}

int main_argc = 0;
char **main_argv = NULL;

int main(int argc, char *argv[]) {
    int __attribute__((unused)) len = 0;
    FILE __attribute__((unused)) *fp;

    main_argc = argc;
    main_argv = argv;

    signal(SIGTERM, cleanup);
    signal(SIGINT, cleanup);
    signal(SIGSEGV, cleanup);
    signal(SIGABRT, cleanup);

    gberror_t grc = E_GENERAL_FAILURE;


    // set STATUS_WORD_GBEM_ALIVE_BIT_NUM bit in status word
    BIT_SET(dpm_in->machine_word, STATUS_WORD_GBEM_ALIVE_BIT_NUM);

    //key boolean indicating if GBC has a shared mem connection to GBEM
    ecm_status.gbc_connected = false;

    //reset key vars in case this is a reboot
    ecm_status.shared_mem_busy_count = 0;
    ecm_status.ec_check_found_error = false;
    ecm_status.cyclic_state = ECM_PRE_BOOT;

    initialise_ec_circular_slave_error_message(&ecm_status.slave_error_messages);


    //solves missing output in debugger log output (a gdb thing)
    setbuf(stdout, 0);

    /* These set where the user message output will be sent. Uncomment the one you want.
     * This is just for user messages UM_* not logging LL_ which, if enabled, always goes to stdout
    */
    logger_set_stdout();
    //    logger_set_log_file("gbem.log", GBEM_UM_EN);
    //    logger_set_syslog("Glowbuzzer");


    //temp just for testing
    //    config_check_and_print(config_summary_json_buffer, &grc);
    //    exit(0);


    //if any drive has the STATUS_WORD_GBEM_HOMING_NEEDED_BIT_NUM set then set the status word: STATUS_WORD_GBEM_HOMING_NEEDED_BIT_NUM
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (map_drive_run_homing[i] == true) {
            BIT_SET(dpm_in->machine_word, STATUS_WORD_GBEM_HOMING_NEEDED_BIT_NUM);
            break;
        }
    }


    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                     Starting GB EtherCAT Master                    ***");
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");


    int which = PRIO_PROCESS; // You can also use PRIO_PGRP or PRIO_USER
    __pid_t pid = getpid(); // Get the process ID of the current process

    int nice_value = PROCESS_NICE_VALUE; // Adjust this value as needed, where lower values mean higher priority

    if (setpriority(which, (id_t) pid, nice_value) == 0) {
        UM_INFO(GBEM_UM_EN, "GBEM: Process priority set to [%d]", nice_value);
    } else {
        UM_FATAL("GBEM: Failed to set process priority with setpriority [%s]", strerror(errno));
    }


    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - overall size [%u]", sizeof(ecm_status_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - ecm_boot_state_t size [%u]", sizeof(ecm_boot_state_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - ecm_net_scan_state_t size [%u]", sizeof(ecm_net_scan_state_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - ecm_active_program_t size [%u]", sizeof(ecm_active_program_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - ecm_status_map_t size [%u]", sizeof(ecm_status_map_t)*EC_MAXSLAVE);
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - cycle_count size [%u]", sizeof(uint64_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - drive_count size [%u]", sizeof(uint8_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - gbc_connected size [%u]", sizeof(bool));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - ec_circular_slave_error_message_t size [%u]",
    //         sizeof(ec_circular_slave_error_message_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - ec_check_found_error size [%u]", sizeof(bool));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - slavecount size [%u]", sizeof(uint8_t));
    //
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - ecm_status_drive_t size [%u]",
    //         sizeof(ecm_status_drive_t)*MAP_MAX_NUM_DRIVES);
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - ec_circular_slave_error_message_t size [%u]",
    //         sizeof(ec_circular_slave_error_message_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - machine_state size [%u]", sizeof(cia_state_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - commanded_machine_state size [%u]", sizeof(cia_state_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - shared_mem_busy_count size [%u]", sizeof(uint64_t));
    // UM_INFO(GBEM_UM_EN, "GBEM: ecm_status - fso size [%u]", sizeof(ecm_status_fsoe_t));


    if (sizeof(ecm_status_t) > SHM_OFFLINE_BUF_SIZE) {
        UM_FATAL(
            "GBEM: The size of the ecm_status data is larger than allocated in the shared memory buffer. SHM_OFFLINE_BUF_SIZE vs sizeof(ecm_status_t)")
        ;
    }


    /* NEW_MACHINE - add new print for machine here */
    /* handy defines to output to console what machine type has been configured */
#if MACHINE_MINI == 1
    map_machine_type = MAP_MACHINE_MINI;
#endif
#if MACHINE_CARTESIAN == 1
    map_machine_type = MAP_MACHINE_CARTESIAN;
#endif
#if MACHINE_DRIVE_SPINNERS == 1
    map_machine_type = MAP_MACHINE_DRIVE_SPINNERS;
#endif
#if MACHINE_DRIVE_SPINNERS_VIRTUAL == 1
    map_machine_type = MAP_MACHINE_DRIVE_SPINNERS_VIRTUAL;
#endif

#if MACHINE_IGUS == 1
    map_machine_type = MAP_MACHINE_IGUS;
#endif
#if MACHINE_STAUBLI == 1
    map_machine_type = MAP_MACHINE_STAUBLI;
#endif
#if MACHINE_SINGLE_AKD == 1
    map_machine_type = MAP_MACHINE_SINGLE_AKD;
#endif
#if MACHINE_CONVEYORS == 1
    map_machine_type = MAP_MACHINE_CONVEYORS;
#endif
#if MACHINE_EL7211_TEST == 1
    map_machine_type = MAP_MACHINE_EL7211_TEST;
#endif
#if MACHINE_TEST == 1
    map_machine_type = MAP_MACHINE_TEST;
#endif

#if MACHINE_JVL_MIS_TEST == 1
    map_machine_type = MAP_MACHINE_JVL_MIS_TEST;
#endif

#if MACHINE_G5_TEST == 1
    map_machine_type = MAP_MACHINE_G5_TEST;
#endif

#if MACHINE_N5_TEST == 1
    map_machine_type = MAP_MACHINE_N5_TEST;
#endif

#if MACHINE_EL7031_TEST == 1
    map_machine_type = MAP_MACHINE_EL7031_TEST;
#endif

#if MACHINE_EL7041_TEST == 1
    map_machine_type = MAP_MACHINE_EL7041_TEST;
#endif

#if MACHINE_EL2522_TEST == 1
    map_machine_type = MAP_MACHINE_EL2522_TEST;
#endif

#if MACHINE_EL7037_TEST == 1
    map_machine_type = MAP_MACHINE_EL7037_TEST;
#endif

#if MACHINE_STARTER_KIT_1 == 1
    map_machine_type = MAP_MACHINE_STARTER_KIT_1;
#endif

#if MACHINE_STARTER_KIT_2 == 1
    map_machine_type = MAP_MACHINE_STARTER_KIT_2;
#endif

#if MACHINE_ASDA_A2_TEST == 1
    map_machine_type = MAP_MACHINE_ASDA_A2_TEST;
#endif

#if MACHINE_SMC3_TEST == 1
    map_machine_type = MAP_MACHINE_SMC3_TEST;
#endif

#if MACHINE_AX5101_TEST == 1
    map_machine_type = MAP_MACHINE_AX5101_TEST;
#endif

#if MACHINE_AW_J17 == 1
    map_machine_type = MAP_MACHINE_AW_J17;
#endif
#if MACHINE_AW_ROBOT_S == 1
    map_machine_type = MAP_MACHINE_AW_ROBOT_S;
#endif

#if MACHINE_AW_ROBOT_L2 == 1
    map_machine_type = MAP_MACHINE_AW_ROBOT_L2;
#endif
#if MACHINE_AW_ROBOT_L == 1
    map_machine_type = MAP_MACHINE_AW_ROBOT_L;
#endif
#if MACHINE_VIRTUAL == 1
    map_machine_type = MAP_MACHINE_VIRTUAL;
#endif
#if MACHINE_J17_PLUS_VIRTUAL == 1
    map_machine_type = MAP_MACHINE_J17_PLUS_VIRTUAL;
#endif
#if MACHINE_AW_STEWART == 1
    map_machine_type = MAP_MACHINE_AW_STEWART;
#endif
#if MACHINE_EL6900_DEADMAN == 1
    map_machine_type = MAP_MACHINE_EL6900_DEADMAN;
#endif
#if MACHINE_EL6900_J25 == 1
    map_machine_type = MAP_MACHINE_EL6900_J25;
#endif
#if MACHINE_BBH_J25 == 1
    map_machine_type = MAP_MACHINE_BBH_J25;
#endif

#if MACHINE_AW_6DOF == 1
    map_machine_type = MAP_MACHINE_AW_6DOF;
#endif
#if MACHINE_AW_6DOF_FSOE == 1
    map_machine_type = MAP_MACHINE_AW_6DOF_FSOE;
#endif


    if (map_machine_type < MAP_NUM_MACHINES) {
        UM_INFO(GBEM_UM_EN, "GBEM: This code has been compiled for [%s]", map_machine_type_strings[map_machine_type]);
    } else {
        UM_ERROR(GBEM_UM_EN,
                 "GBEM: There is an issue with map_machine_type_strings, please add your machine to the array");
    }


    /* This section just outputs the state of debug #defines */
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                           Debug info.                              ***");
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");

#if ENABLE_LOGGING == 1
    UM_INFO(GBEM_UM_EN, "GBEM: Debug logging is globally enabled (ENABLE_LOGGING)");
    UM_INFO(GBEM_UM_EN, "GBEM: Debug log level (LOG_LEVEL) is [%u]", LOG_LEVEL);
#else
    UM_INFO(GBEM_UM_EN, "GBEM: Debug logging is NOT globally enabled (ENABLE_LOGGING)");
#endif

#if GBEM_GEN_LOG_EN == 1
    UM_INFO(GBEM_UM_EN, "GBEM: General GBEM debug logging is enabled (GBEM_GEN_LOG_EN)");
#else
    UM_INFO(GBEM_UM_EN, "GBEM: General GBEM debug logging is NOT enabled (GBEM_GEN_LOG_EN)");
#endif

#if GBEM_MISSING_FUN_LOG_EN == 1
    UM_INFO(GBEM_UM_EN, "GBEM: Missing function pointer logging for GBEM is enabled (GBEM_MISSING_FUN_LOG_EN)");
#endif

#if GBEM_SM_LOG_EN == 1
    UM_INFO(GBEM_UM_EN, "GBEM: State machine logging is enabled for GBEM. This produces voluminous console output! (SM_LOG_EN)");
#endif

#if NO_COMMAND_LINE_OPTIONS == 1
    strcpy(eth_interface1, "eth0");
    strcpy(eth_interface2, "eth1");
    ecm_status.active_program = ECM_CYCLIC_PROG;
    goto skip_command_line;
#endif

    char ch;
    int index = 0;

    int duplicate_arg = 0;
    int help = 0;
    struct option options[] = {
        {"help", no_argument, NULL, 'h'},
        {"netscan", no_argument, NULL, 'n'},
        {"netscanwithpdo", no_argument, NULL, 'm'},
        {"cyclic", no_argument, NULL, 'c'},
        {"write", required_argument, NULL, 'w'},
        {"confcheck", no_argument, NULL, 'd'},
        {"if", required_argument, NULL, 'i'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    while (((ch = getopt_long(argc, argv, "hnmcwv:di:", options, &index)) != -1) && (ch != 255)) {
        switch (ch) {
            case 'h':
                main_getopt_usage();
                help = 1;
                duplicate_arg++;
                break;
            case 'n':
                ecm_status.active_program = ECM_NET_SCAN_PROG;
                duplicate_arg++;
                break;
            case 'm':
                ecm_status.active_program = ECM_NET_SCAN_PDO_PROG;
                duplicate_arg++;
                break;
            case 'd':
                ecm_status.active_program = ECM_PRINT_CONFIG_PROG;
                duplicate_arg++;
                break;
            case 'c':
                ecm_status.active_program = ECM_CYCLIC_PROG;
                duplicate_arg++;
                break;
            case 'w':
                ecm_status.active_program = ECM_WRITE_NVRAM_PROG;

                if (optarg != NULL) {
                    slave_to_write_nvram = strtoimax(optarg, NULL, 10);
                } else {
                    printf("Please specify a slave number (to which to write the NVRAM) -w[number]\n");
                }
                break;
            case 'i':
                if (optarg != NULL) {
                    memcpy(gbem_ctx.eth_interface1, optarg, strlen(optarg));
                } else {
                    printf("Please specify an interface with -i[name]");
                }
                break;

            case 'v':
                UM_INFO(GBEM_UM_EN, "GBEM: Version is [%s]", GIT_TAG);
                exit(EXIT_SUCCESS);
                break;


            case '?':
                main_getopt_usage();
                return EXIT_FAILURE;
            default:
                main_getopt_usage();
                return EXIT_FAILURE;
        }
    }


    if (optind == 1) {
        printf("GBEM was called without any command line arguments\n");
        main_getopt_usage();
        return EXIT_FAILURE;
    }

    if (gbem_ctx.eth_interface1[0] == '\0' && !help) {
        printf("GBEM was called without a networking interface specified\n");
        main_getopt_usage();
        return EXIT_FAILURE;
    }

    if (help) {
        return EXIT_SUCCESS;
    }

    if (duplicate_arg > 1) {
        printf("The program arguments h | n | c | d are exclusive more than one has been specified\n");
        main_getopt_usage();
        return EXIT_FAILURE;
    }

    //label used if we have #defined NO_COMMAND_LINE_OPTIONS and dont want to pull GBEM options off the command line and wish to define them in the code
skip_command_line:
    __attribute__((unused));

    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                         Pre-boot sequence                          ***");
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");

    UM_INFO(GBEM_UM_EN, "GBEM: Software project name [%s]", PROJECT_NAME);
    UM_INFO(GBEM_UM_EN, "GBEM: Software project version [%s]", PROJECT_VER);
    UM_INFO(GBEM_UM_EN, "GBEM: Release Git Tag [%s]", GIT_TAG);

    //set the number of slaves in the map to be the (maximum) number of slaves defined in the machine map (this may be reduced when optional slaves are removed)
    map_num_slaves = MAP_NUM_SLAVES;

    // test();
    // exit(0);

    UM_INFO(GBEM_UM_EN, "GBEM: We are running with the [%s] program on interface [%s]",
            ecm_active_program_names[ecm_status.active_program], gbem_ctx.eth_interface1);

    if (check_ethernet_link(gbem_ctx.eth_interface1)) {
        UM_INFO(GBEM_UM_EN, "GBEM: Ethernet interface (1) link status is ok");
    } else {
        UM_ERROR(GBEM_UM_EN,
                 "GBEM: Ethernet interface link status problem. This could be: cable not connected, invalid interface, invalid interface configuration, EtherCAT slaves unpowered etc.")
        ;
        UM_ERROR(GBEM_UM_EN, "GBEM: This is a >serious< issue and GBEM is unlike to run successfully");
    }


    //print maximum number of slaves configured in EC_MAXSLAVE
    UM_INFO(GBEM_UM_EN, "GBEM: Maximum number of slaves configured in EC_MAXSLAVE is [%d]", EC_MAXSLAVE);


    //RT-Sensitive
#if ENABLE_ALL_NON_CORE_FUNCTIONS == 1
#if ENABLE_PLC == 1
    grc = plc_init();
    if (grc != E_SUCCESS) {
        UM_FATAL(
            "GBEM: We were unable to initialise the PLC, we can't run with a defective PLC configuration. Error [%s]",
            gb_strerror(grc));
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: PLC initialised successfully");
    }
#endif
#endif


    if ((ecm_status.active_program == ECM_NET_SCAN_PROG) || (ecm_status.active_program == ECM_NET_SCAN_PDO_PROG) || (
            ecm_status.active_program == ECM_PRINT_CONFIG_PROG) ||
        (ecm_status.active_program == ECM_WRITE_NVRAM_PROG)) {
        goto program_switch;
    }

    char *username_buf;
    username_buf = (char *) malloc(11 * sizeof(char));
    memset(username_buf, 0, 11 * sizeof(char));
    if (getlogin_r(username_buf, 10) != 0) {
        strncpy(username_buf, "<unknown>", 10);
    }
    UM_INFO(GBEM_UM_EN, "GBEM: We are running as user [%s]", username_buf);

    grc = establish_shared_mem_and_signal_con(&shmp, 1, 1);
    if (grc != E_SUCCESS) {
        ecm_status.gbc_connected = false;
        UM_ERROR(GBEM_UM_EN,
                 "GBEM: Connection to shared memory could not be established - we will continue without a connection to GBC")
        ;
    } else {
        ecm_status.gbc_connected = true;
        UM_INFO(GBEM_UM_EN, "GBEM: We have a connection to shared memory >successfully< established ");
        LL_INFO(GBEM_GEN_LOG_EN,
                "GBEM: Shared memory address [%p] (this is a virtual address so will not match across processes)",
                shmp);
        memset(shmp->sm_buf_in, 0, sizeof(uint8_t) * SHM_BUF_SIZE);
        memset(shmp->sm_buf_out, 0, sizeof(uint8_t) * SHM_BUF_SIZE);
    }

    UM_INFO(GBEM_UM_EN,
            "GBEM: Number of row in IO map [%u]", map_num_rows_in_iomap);

    if (!check_limits_ini_exists()) {
        UM_WARN(GBEM_UM_EN,
                "GBEM: Limits ini file [%s] not found, we will run without soft limits (position velocity torque etc.) configured on the drives",
                LIMITS_INI_FILENAME);
    } else {
        UM_INFO(GBEM_UM_EN,
                "GBEM: Limits ini file [%s] found, we will now parse the limits from the file", LIMITS_INI_FILENAME);
        uint8_t limits_number_of_joints_in_ini = get_limits_ini_sections();

        if (limits_number_of_joints_in_ini != MAP_NUM_DRIVES) {
            UM_FATAL(
                "GBEM: The number of joints in the limits file [%s] does not match the number of joints in the machine config [%d]. This is a fatal error and GBEM will exit",
                LIMITS_INI_FILENAME, MAP_NUM_DRIVES);
        } else {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: The number of joints in the limits file [%s] matches the number of joints in the machine config [%d]",
                    LIMITS_INI_FILENAME, MAP_NUM_DRIVES);
        }

        uint8_t parse_count = 0;

        gberror_t limits_read_rc = read_limits_ini(&parse_count);

        if (parse_count != NUM_PARAMS_IN_LIMITS_INI * MAP_NUM_DRIVES) {
            UM_FATAL(
                "GBEM: The limits ini file does not contain the correct number of parameters - please fix the limits ini file [%s]",
                LIMITS_INI_FILENAME);
        } else {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: The limits ini file contains the correct number of parameters [%d] - we will continue",
                    parse_count);
        }


        for (int i = 0; i < MAP_NUM_DRIVES; i++) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: Limits for joint [%d] are: position_limit_max [%d] position_limit_min [%d] velocity_limit [%d] torque_limit [%d] using max_motor_speed [%d] and max_motor_torque [%d]",
                    i, map_machine_limits[i].position_limit_max, map_machine_limits[i].position_limit_min,
                    map_machine_limits[i].velocity_limit, map_machine_limits[i].torque_limit,
                    map_machine_limits[i].max_motor_speed, map_machine_limits[i].max_motor_torque);
        }
        if (limits_read_rc == E_SUCCESS) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: Limits loaded - we will apply these soft limits (position velocity torque etc.) to the drives")
            ;
            UM_INFO(GBEM_UM_EN,
                    "GBEM: !IMPORTANT! - on some drives once limits are written to the drive they are persisted in NVRAM")
            ;
        } else {
            UM_FATAL("GBEM: Limits parsing failed - please fix the limits file [%s]", LIMITS_INI_FILENAME);
        }
    }


    if (!check_debug_ini_exists()) {
        UM_WARN(GBEM_UM_EN,
                "GBEM: Debug ini file [%s] not found, no debug options will be applied",
                DEBUG_INI_FILENAME);
    } else {
        UM_INFO(GBEM_UM_EN,
                "GBEM: Debug ini file [%s] found, we will now parse the file", DEBUG_INI_FILENAME);

        uint8_t number_of_items_found = 0;

        gberror_t debug_read_rc = read_debug_ini(&number_of_items_found);


        if (number_of_items_found != NUM_PARAMS_IN_DEBUG_INI) {
            UM_FATAL(
                "GBEM: The debug ini file does not contain the correct number of parameters [%u] - please fix the debug ini file [%s]",
                number_of_items_found, DEBUG_INI_FILENAME);
        } else {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: The debug file contains the correct number of parameters [%d] - we will continue",
                    number_of_items_found);
        }


        UM_INFO(GBEM_UM_EN,
                "GBEM: Debug config is: disable_drive_follow_error_check [%s], disable_drive_follow_error_check [%s], disable_drive_follow_error_check [%s]",
                BOOL_STRING(debug_settings.disable_drive_follow_error_check),
                BOOL_STRING(debug_settings.disable_drive_follow_error_check),
                BOOL_STRING(debug_settings.disable_drive_follow_error_check));

        if (debug_read_rc == E_SUCCESS) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: Debug settings loaded successfully")
            ;
        } else {
            UM_FATAL("GBEM: Debug settings parsing failed - please fix the debug ini file [%s]", DEBUG_INI_FILENAME);
        }
    }


    if (!check_machine_config_ini_exists()) {
        UM_WARN(GBEM_UM_EN,
                "GBEM: Machine config ini file [%s] not found, no optional slaves will be enabled",
                MACHINE_CONFIG_INI_FILENAME);
    } else {
        UM_INFO(GBEM_UM_EN,
                "GBEM: Machine config ini file [%s] found, we will now parse the file", MACHINE_CONFIG_INI_FILENAME);

        uint8_t number_of_items_found = 0;

        gberror_t machine_config_read_rc = read_machine_config_ini(&number_of_items_found);


        if (number_of_items_found != NUM_PARAMS_IN_MACHINE_CONFIG_INI) {
            UM_FATAL(
                "GBEM: The machine config ini file does not contain the correct number of parameters [%u] - please fix the machine config ini file [%s]",
                number_of_items_found, MACHINE_CONFIG_INI_FILENAME);
        } else {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: The machine config contains the correct number of parameters [%d] - we will continue",
                    number_of_items_found);
        }


        UM_INFO(GBEM_UM_EN,
                "GBEM: Machine config is: enable_optional_slave_1 [%s], enable_optional_slave_2 [%s], enable_optional_slave_3 [%s], enable_optional_slave_4 [%s], enable_optional_slave_5 [%s], enable_optional_slave_6 [%s], enable_optional_slave_7 [%s], enable_optional_slave_8 [%s], enable_optional_slave_9 [%s], enable_optional_slave_10 [%s],",
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_1),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_2),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_3),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_4),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_5),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_6),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_7),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_8),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_9),
                BOOL_STRING(machine_config_optional_slaves.enable_optional_slave_10)
        );

        if (machine_config_read_rc == E_SUCCESS) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: Machine config (optional slaves) settings loaded successfully")
            ;
        } else {
            UM_FATAL("GBEM: Machine config settings parsing failed - please fix the machine config ini file [%s]",
                     DEBUG_INI_FILENAME);
        }
    }

    //we must disable slaves even if we dont have a config ini file as the file "turns on" slaves


    UM_INFO(GBEM_UM_EN, "GBEM: Starting process to disable optional slaves");
    os_disable_slaves(map_slave_optional);
    UM_INFO(GBEM_UM_EN, "GBEM: Completed process to disable optional slaves");


#if DISABLE_ESTOP_CHECKING == 1
    UM_WARN(GBEM_UM_EN, "GBEM: Warning DISABLE_ESTOP_CHECKING is defined! This will override the hardware estop");
#endif
#if DISABLE_HEARTBEAT_CHECKING == 1
    UM_WARN(GBEM_UM_EN, "Warning DISABLE_HEARTBEAT_CHECKING is defined! This will disable heartbeat check");
#endif

    int *task_param = (int *) malloc(sizeof(int));
    *task_param = 0;

    int rc;

    //label for jump for netscan program
program_switch:

    switch
    (ecm_status
        .
        active_program
    ) {
        case ECM_CYCLIC_PROG:
            rc = osal_thread_create(&thread_ec_reboot, STACK64K * 2, &ec_reboot, (void *) task_param);
            if (rc != 1) {
                UM_FATAL(
                    "GBEM: An error occurred whilst creating the pthread (ec_reboot) and GBEM will exit. This error message implies that a Linux system call (pthread_create) has failed. This could be because the system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process would be exceeded. Neither of these should occur normally. Something bad has happened deep down")
                ;
            }


        //Create RT thread for cyclic task, inside this function the priority and scheduler params are set
            rc = osal_thread_create_rt(&thread_ec_rxtx, STACK64K * 2, &ec_rxtx, (void *) task_param);
            if (rc != 1) {
                UM_FATAL(
                    "GBEM: An error occurred whilst creating the pthread (ec_rxtx which is the main cyclic process thread) and GBEM will exit. This error message implies that a Linux system call (pthread_create) has failed. This could be because the system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process would be exceeded. Neither of these should occur normally. Something bad has happened deep down")
                ;
            }
        //wait to let the thread_ec_rxtx startup messages spit out ungarbled
            sleep(2);

        //Run EC boot sequence
            ECBoot((void *) task_param);
            break;
        case ECM_NET_SCAN_PROG:
            ECNetscan(false);
            break;
        case ECM_NET_SCAN_PDO_PROG:
            ECNetscan(true);
            break;
        case ECM_PRINT_CONFIG_PROG:
            grc = config_print();
            if (grc == E_SUCCESS) {
                return EXIT_SUCCESS;
            } else {
                UM_FATAL(
                    "GBEM: We can't print the complete config as the process failed. This implies that there is something nasty in either the machine config itself or the process to create it")
                ;
            }
        case ECM_WRITE_NVRAM_PROG:
            //this function will EXIT
            nvram_step_1();
            break;
        default:
            UM_FATAL(
                "GBEM: An error occurred when we were initialising, we can't figure out what GBEM sub-program you want to run so must exit")
            ;
    }

    //todo crit - is this right?
    if
    (ecm_status
     .
     active_program
     !=
     ECM_NET_SCAN_PROG
    ) {
        pthread_join(thread_ec_reboot, NULL);
    }
    return
            0;
}
