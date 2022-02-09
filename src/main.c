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

#include <sys/ioctl.h>    // SIOCGIFFLAGS
#include <errno.h>        // errno
#include <netinet/in.h>   // IPPROTO_IP
#include <net/if.h>       // IFF_*, ifreq



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
//#include "startup.h"
#include "map_summary.h"
#include "version.h"
#include "ecrxtx.h"

#include "std_utils.h"
#include "map_SDO_print.h"
#include "nvram.h"
#include "plc_core.h"
#include "gbem_config.h"
#include "linux_shm.h"


/** storage for gbc process name */
char proc_name[GBC_PROCESS_NAME_MAX_LENGTH] = {0};

/** storage for directories and paths for gbc json exchange files */
char storage_dir_for_gbc_exchange[200];
char full_path_for_emstat_json_const[200];
char full_path_for_emstat_json_status[200];
char full_path_for_config_json[200];

/**global var storing the name of the nic read from command lines args */
char eth_interface1[SIZE_OF_IF_NAME] = {0};
char eth_interface2[SIZE_OF_IF_NAME] = {0};


//todo-crit put in ec
/** global variable to signal between tasks when ec_check finds and error */
//volatile int ec_check_found_error;

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
int gbc_pid =0;


/* function forward declarations */

static void main_getopt_usage(void);

/**
 * @brief function needed by freertos to handle the custom signal we added - in this case this will not be called
 * @param signum
 */
void __attribute__((unused)) gb_handle_signal(int signum) {
    (void) signum;
    LL_FATAL("GBEM: gb_handle signal called, this should never happen");
}



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
    (void) strncpy(if_req.ifr_name, ifname, sizeof(if_req.ifr_name));
    int rv = ioctl(socId, SIOCGIFFLAGS, &if_req);
    close(socId);

    if (rv == -1) {
        UM_ERROR(GBEM_UM_EN, "Ioctl failed. Errno = %d\n", errno);
    }

    return (if_req.ifr_flags & IFF_UP) && (if_req.ifr_flags & IFF_RUNNING);
}


/**
 * @brief prints usage information for gbem command line
 */
static void main_getopt_usage(void) {
    ec_adaptert *adapter = NULL;
    printf("Usage of GBEM:\n");

    printf("\t-h | -n | -d | -wSLAVENAME | -n | -m | -c -iIFNAME [-pPROCESSNAME]\n\n");
    printf("\t-n | --netscan: run the GBEM netscan program to scan the EtherCAT network including reading the slave EEPROM summary\n");
    printf("\t-m | --netscanwithpdo: run the GBEM netscan program to scan the EtherCAT network after writing PDO re-map SDOs \n");
    printf("\t-d | --confcheck: check and print out GBEM configuration and exit\n");
    printf("\t-c | --cyclic: run the GBEM cyclic program (for normal operation)\n");
    printf("\t-w | --write: write SDOs for a specified slave and the trigger a write to slave's NVRAM\n");
    printf("\t-i | --if: interface to use - this is a NIC interface, e.g. eth0 (mandatory)\n");
    printf("\t-p | --process: the process name to send signals to (this is optional, default is: [%s])\n",
           GBC_PROCESS_NAME);
    printf("\t-h | --help: GBEM usage information\n");
    printf("\nExample #1: GBEM -n -ieth0 -pGBC-linux = run netscan on eth0\n");
    printf("\nExample #2: GBEM --netscan --iflan1 --processGBC-linux = run cyclic program on lan1\n");
    printf("\nIf no process name name (-p or --process) is specified the default [%s] will be used\n",
           GBC_PROCESS_NAME);
    printf("\nAvailable adapters:\n");

    //todo-crit
//    add write slave firmware
// add read and write slave eeprom

    adapter = ec_find_adapters();
    while (adapter != NULL) {
        printf("\tDescription : %s, Device name to use: %s\n", adapter->desc, adapter->name);
        adapter = adapter->next;
    }
}

/**
 * @brief based on the value of platform variable sets the file paths for emstat & config json
 */
void main_set_file_paths(void) {
//    if (os_platform == PLATFORM_PI) {
        strcpy(storage_dir_for_gbc_exchange, FILE_STORAGE_DIRECTORY);
//    } else {
//        strcpy(storage_dir_for_gbc_exchange, STORAGE_DIR_GENERIC_LINUX);
//    }

    //strcpy the base paths
    strcpy(full_path_for_emstat_json_const, storage_dir_for_gbc_exchange);
    strcpy(full_path_for_emstat_json_status, storage_dir_for_gbc_exchange);
    strcpy(full_path_for_config_json, storage_dir_for_gbc_exchange);

    //concatenate the filenames onto the base paths
    strcat(full_path_for_emstat_json_const, JSON_CONST_FILENAME);
    strcat(full_path_for_emstat_json_status, JSON_STATUS_FILENAME);
    strcat(full_path_for_config_json, JSON_CONFIG_FILENAME);

}


int main(int argc, char *argv[]) {
    int len = 0;
    FILE *fp;

    gberror_t grc = E_GENERAL_FAILURE;

    //this is the stack based buffer to hold the json config summary
    char config_summary_json_buffer[SIZE_OF_CONFIG_SUMMARY_JSON_BUFFER];

    //key boolean indicating if GBC has a shared mem connection to GBEM
    ecm_status.gbc_connected = false;

    ecm_status.ec_check_found_error = false;

    //solves missing output in debugger log output (a gdb thing)
    setbuf(stdout, 0);

    /* These set where the user message output will be sent. Uncomment the one you want.
     * This is just for user messages UM_* not logging LL_ which, if enabled, always goes to stdout
    */
    logger_set_stdout();
    //    logger_set_log_file("gbem.log", GBEM_UM_EN);
    //    logger_set_syslog("Glowbuzzer");


    //temp just for testing
//    len = config_check_and_print(config_summary_json_buffer, &grc);
//    fp = fopen("test.json", "w+");
//    if (fp) {
//        fwrite(config_summary_json_buffer, sizeof(char), (size_t) len, fp);
//    }
//    fclose(fp);
//    exit(0);


    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                     Starting GB EtherCAT Master                    ***");
    UM_INFO(GBEM_UM_EN, "GBEM: **************************************************************************");


    /* This section just outputs the state of debug #defines */
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
    strcpy(proc_name, GBC_PROCESS_NAME);
    goto skip_command_line;
#endif

    char ch;
    int index = 0;
    int proc_name_arg = 0;
    int duplicate_arg = 0;
    int help = 0;
    struct option options[] = {
            {"help",           no_argument,       NULL, 'h'},
            {"netscan",        no_argument,       NULL, 'n'},
            {"netscanwithpdo", no_argument,       NULL, 'm'},
            {"cyclic",         no_argument,       NULL, 'c'},
            {"write",          required_argument, NULL, 'w'},
            {"confcheck",      no_argument,       NULL, 'd'},
            {"if",             required_argument, NULL, 'i'},
            {"process",        required_argument, NULL, 'p'},
            {0, 0, 0,                                   0}
    };

    while (((ch = getopt_long(argc, argv, "hnmcw:di:p:", options, &index)) != -1) && (ch != 255)) {
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
                    memcpy(eth_interface1, optarg, strlen(optarg));
                } else {
                    printf("Please specify an interface with -i[name]");
                }
                break;
            case 'p':
                if (optarg != NULL) {
                    memcpy(proc_name, optarg, strlen(optarg));
                    proc_name_arg = 1;
                } else {
                    printf("Please specify an process name with -p[name]");
                }
                break;

            case '?':
                main_getopt_usage();
                return EXIT_FAILURE;
            default:
                main_getopt_usage();
                return EXIT_FAILURE;
        }
    }

    if (!proc_name_arg) {
        if (GBC_PROCESS_NAME != NULL) {
            if (strlen(GBC_PROCESS_NAME) < GBC_PROCESS_NAME_MAX_LENGTH - 1) {
                memcpy(proc_name, GBC_PROCESS_NAME, strlen(GBC_PROCESS_NAME));
            } else {
                UM_FATAL("GBEM: GBC_PROCESS_NAME is longer than [%u]", GBC_PROCESS_NAME - 1);
            }
        } else {
            UM_FATAL("GBEM: Invalid GBC_PROCESS_NAME is defined");
        }
    }

    if (optind == 1) {
        printf("GBEM was called without any command line arguments\n");
        main_getopt_usage();
        return EXIT_FAILURE;
    }

    if (eth_interface1[0] == '\0' && !help) {
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

    UM_INFO(GBEM_UM_EN, "GBEM: We are running with the [%s] program on interface [%s]",
            ecm_active_program_names[ecm_status.active_program], eth_interface1);

    if (check_ethernet_link(eth_interface1)) {
        UM_INFO(GBEM_UM_EN, "GBEM: Ethernet interface (1) link status is ok");
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: Ethernet interface link status problem. This could be: cable not connected, invalid interface, invalid interface configuration, EtherCAT slaves unpowered etc.");
        UM_ERROR(GBEM_UM_EN, "GBEM: This is a >serious< issue and GBEM is unlike to run successfully");
    }

    grc = plc_init();
    if (grc != E_SUCCESS) {
        UM_FATAL(
                "GBEM: We were unable to initialise the PLC, we can't run with a defective PLC configuration. Error [%s]",
                gb_strerror(grc));
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: PLC initialised successfully");
    }

    if ((ecm_status.active_program == ECM_NET_SCAN_PROG) || (ecm_status.active_program == ECM_NET_SCAN_PDO_PROG) || (ecm_status.active_program == ECM_PRINT_CONFIG_PROG) ||
        (ecm_status.active_program == ECM_WRITE_NVRAM_PROG)) {
        goto program_switch;
    }

    if (SIGNAL_TO_SEND > 31) {
        UM_FATAL(
                "GBEM: We have a signal number defined (with SIGNAL_TO_SEND) to a number greater than 31. This is outside the range of normal Linux signals. We must exit");
    }
    char *str = strdup(sys_siglist[SIGNAL_TO_SEND]);
    if (str) {
        upcase(str);
        UM_INFO(GBEM_UM_EN,
                "GBEM: We are using Linux signal [SIG %s] (we are sending this out to GBC to advance its cycle)", str);
        free(str);
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: Error matching the signal number [%u], to the standard Linux signals",
                 SIGNAL_TO_SEND);
    }

#ifdef GB_APP_LINUX
    char *username_buf;
    username_buf = (char *) malloc(11 * sizeof(char));
    memset(username_buf, 0, 11 * sizeof(char));
    if (getlogin_r(username_buf, 10) != 0) {
        strncpy(username_buf, "<unknown>", 10);
    }
    UM_INFO(GBEM_UM_EN, "GBEM: We are running as user [%s]", username_buf);
#endif


//    char gb_inifile_name[] = "gbem.ini";
//    /* set global var platform to be the platform value read from the ini file */
//    os_platform = read_platform_from_ini(&gb_inifile_name[0], PLATFORM_PI, GB_PROGRAM_GBEM);

/* */
    grc = establish_shared_mem_and_signal_con(&shmp, proc_name, true, &gbc_pid, 1);
    if (grc != E_SUCCESS) {
        ecm_status.gbc_connected = false;
        UM_ERROR(GBEM_UM_EN,
                 "GBEM: Connection to shared memory & GBC process could not be established - we will continue without a connection to GBC");
    } else {
        ecm_status.gbc_connected = true;
        UM_INFO(GBEM_UM_EN, "GBEM: We have a connection to shared memory & GBC process >successfully< established ");
        LL_INFO(GBEM_GEN_LOG_EN, "GBEM: Shared memory address [%p] (this is a virtual address so will not match across processes)", shmp);
        memset(shmp->sm_buf_in, 0, sizeof (uint8_t) * SHM_BUF_SIZE);
        memset(shmp->sm_buf_out, 0, sizeof (uint8_t) * SHM_BUF_SIZE);
    }


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

#if MACHINE_SK1 == 1
    map_machine_type = MAP_MACHINE_SK1;
#endif

#if MACHINE_SK2 == 1
    map_machine_type = MAP_MACHINE_SK2;
#endif

#if MACHINE_ASDA_A2_TEST == 1
    map_machine_type = MAP_MACHINE_ASDA_A2;
#endif

#if MACHINE_SMC3_TEST == 1
    map_machine_type = MAP_MACHINE_SMC3;
#endif

#if MACHINE_AX5101_TEST == 1
    map_machine_type = MAP_MACHINE_AX5101_TEST;
#endif


    if (map_machine_type < MAP_NUM_MACHINES) {
        UM_INFO(GBEM_UM_EN, "GBEM: This code has been compiled for [%s]", map_machine_type_strings[map_machine_type]);

    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: There is an issue with map_machine_type_strings, please add your machine to the array");
    }

#if DISABLE_ESTOP_CHECKING == 1
    UM_WARN(GBEM_UM_EN, "Warning DISABLE_ESTOP_CHECKING is defined! This will override the hardware estop");
#endif
#if DISABLE_HEARTBEAT_CHECKING == 1
    UM_WARN(GBEM_UM_EN, "Warning DISABLE_HEARTBEAT_CHECKING is defined! This will disable heartbeat check");
#endif

    /* sets the paths for file based exchange with GBC */
    main_set_file_paths();

    /* outputs the paths that are being used */
    UM_INFO(GBEM_UM_EN, "GBEM: The path we are using for emstat const file [%s]", full_path_for_emstat_json_const);
    UM_INFO(GBEM_UM_EN, "GBEM: The path we are using for emstat status file [%s]", full_path_for_emstat_json_status);
    UM_INFO(GBEM_UM_EN, "GBEM: The path we are using for config file [%s]", full_path_for_config_json);

    /* The directory for file based exchange exists */
    DIR *dir = opendir(storage_dir_for_gbc_exchange);
    if (dir) {
        /* Directory exists. */
        closedir(dir);
        UM_INFO(GBEM_UM_EN, "GBEM: [%s] directory exists for file based GBC exchange (JSON files)", storage_dir_for_gbc_exchange);
    } else if (ENOENT == errno) {
        /* Directory does not exist. */
        UM_FATAL(
                "GBEM: No %s directory exists - please create this directory (maybe a ramdisk) for storing JSON files for GBC exchange. GBEM can't run without this directory so will exit",
                storage_dir_for_gbc_exchange);
    } else {
        /* opendir() failed for some other reason. */
        UM_FATAL(
                "GBEM: We could not check if a [%s] directory exists. This implies the Linux system call (opendir) failed. Error message from system call: >>%s<<",
                storage_dir_for_gbc_exchange, strerror(errno));
    }

    //check the config, create the json config summary, and optionally print it - in this case don't print. If you want a config summary printed at start-up change to true
//    len = config_create_check_print(&config_summary_json_buffer[0], &grc, true);
//
//
//    if (grc == E_SUCCESS) {
//        UM_INFO(GBEM_UM_EN, "GBEM: Size of config json [%d]", len);
//    } else {
//        UM_FATAL(
//                "GBEM: Config checking (and config JSON creation) >failed< .This implies that there is something nasty in either the machine config itself or the process to create it");
//    }

    //write  config json to file
    fp = fopen(full_path_for_config_json, "w+");
    if (fp) {
        fwrite(config_summary_json_buffer, sizeof(char), (size_t) len, fp);
        fclose(fp);
    } else{
        UM_FATAL("GBEM: Could not open a file to output the config JSON (file name is %s)", full_path_for_config_json);
    }


    int *task_param = (int *) malloc(sizeof(int));
    *task_param = 0;

    int rc;

    //label for jump for netscan program
    program_switch:

    switch (ecm_status.active_program) {
        case ECM_CYCLIC_PROG:
            //Create RT thread for cyclic task, inside this function the priority and scheduler params are set
            rc = osal_thread_create_rt(&thread_ec_rxtx, STACK64K * 2, &ec_rxtx, (void *) proc_name);
            if (rc != 1) {
                UM_FATAL(
                        "GBEM: An error occurred whilst creating the pthread (ec_rxtx which is the main cyclic process thread) and GBEM will exit. This error message implies that a Linux system call (pthread_create) has failed. This could be because the system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process would be exceeded. Neither of these should occur normally. Something bad has happened deep down");
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
            UM_INFO(GBEM_UM_EN,
                    "GBEM: Here, we print out the SDO configuration that will be applied to configure the PDOs and SDOs to configure the slaves");

            map_print_sdos();
            config_create_check_print(config_summary_json_buffer, &grc, true);
            if (grc == E_SUCCESS) {
                return EXIT_SUCCESS;
            } else {
                UM_FATAL(
                        "GBEM: We can't print the complete config as the process failed. This implies that there is something nasty in either the machine config itself or the process to create it");

            }
        case ECM_WRITE_NVRAM_PROG:
            //this function will EXIT
            nvram_step_1();
            break;
        default:
            UM_FATAL("GBEM: An error occurred when we were initialising, we can't figure out what GBEM sub-program you want to run so must exit");
    }

    if (ecm_status.active_program != ECM_NET_SCAN_PROG) {
        //join the main to the thread to stop it exiting until the thread has finished - pthread jiggerypokery
        pthread_join(thread_ec_emstat, NULL);

        //if emstat isn't running join ec_rxtx by uncommenting the following line
//        pthread_join(thread_ec_rxtx, NULL);
    }
    return 0;
}