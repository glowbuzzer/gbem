
/**
 ******************************************************************************
 * @file           :  map_summary.c
 * @brief          :  produces a summary of the machine config
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "std_defs_and_macros.h"
#include "ethercatsetget.h"
#include "dpm.h"
#include "shared.h"
#include "map_summary.h"
#include "json_maker.h"
#include "log.h"
#include "version.h"
#include "user_message.h"
#include "plc_core.h"
#include "status.h"
#include "json_print.h"
#include "map_SDO_print.h"
#include "ec_functions.h"
#include "main.h"
#include "gbem_config.h"
#include "etg.h"
#include "map.h"

#define KRED     "\x1b[31m"
#define KGREEN   "\x1b[32m"
#define KYELLOW  "\x1b[33m"
#define KBLUE    "\x1b[34m"
#define KMAGENTA "\x1b[35m"
#define KCYAN    "\x1b[36m"
#define KRESET   "\x1b[0m"

static gberror_t check_slave_function_ptrs(const uint16_t num_slaves) {
    //we can't check slaves as all function pointers optional
    for (int i = 0; i < num_slaves; i++) {
        if(*map_slave_initial_pdo_function_ptr[i] == NULL){

        }
    }
//could print out yes / no
    //todo-crit
    return E_SUCCESS;
}



//MAP_DRIVE_GET_MOO_PDO_FUNCTIONS - MANDATORY
//MAP_DRIVE_GET_MOO_SDO_FUNCTIONS - MANDATORY
//MAP_DRIVE_GET_REMOTE_FUNCTIONS - MANDATORY
//MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS - MANDATORY
//MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS - MANDATORY
//MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS - MANDATORY
//MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS - MANDATORY
//MAP_DRIVE_GET_STAT_WRD_FUNCTIONS - MANDATORY
//MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS - MANDATORY
//MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS - MANDATORY
//MAP_DRIVE_HOMING_EXEC_FUNCTIONS - OPTIONAL
//MAP_DRIVE_PRINT_PARAMS_FUNCTIONS - OPTIONAL



static gberror_t check_drive_function_ptrs(const uint16_t num_drives) {
    for (int i = 0; i < num_drives; i++) {
        if (*map_drive_set_ctrl_wrd_function_ptr[i] == NULL) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Missing set control word function pointer on drive [%u]", i);
            return E_INVALID_MAP;
        }
        if (*map_drive_get_stat_wrd_function_ptr[i] == NULL) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Missing get status word function pointer on drive [%u]", i);
            return E_INVALID_MAP;
        }
        if (*map_drive_get_actpos_wrd_function_ptr[i] == NULL) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Missing get actpos word function pointer on drive [%u]", i);
            return E_INVALID_MAP;
        }
        if (*map_drive_set_setpos_wrd_function_ptr[i] == NULL) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Missing set setpos word function pointer on drive [%u]", i);
            return E_INVALID_MAP;
        }
        if (*map_drive_get_remote_function_ptr[i] == NULL) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Missing get remote word function pointer on drive [%u]", i);
            return E_INVALID_MAP;
        }
        if (*map_drive_get_moo_pdo_function_ptr[i] == NULL && *map_drive_get_moo_sdo_function_ptr[i] == NULL) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: Neither a get modes of operation with SDO or POD function pointer on drive [%u]", i);
            return E_INVALID_MAP;
        }
        if (*map_drive_get_error_string_sdo_function_ptr[i] == NULL &&
            *map_drive_get_error_string_pdo_function_ptr[i] == NULL) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Neither a get error with SDO or POD function pointer on drive [%u]", i);
            return E_INVALID_MAP;
        }
        if (*map_drive_get_follow_error_function_ptr[i] == NULL) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Missing get follow error function pointer on drive [%u]", i);
            return E_INVALID_MAP;
        }
    }
    return E_SUCCESS;
}


/**
 * @brief Examines the general machine config - checks, prints summary and creates JSON config summary
 * @param json_dest
 * @param grc
 * @return
 */
static char *config_process_general(char *json_dest, gberror_t *grc) {
    bool config_error = false;
    json_dest = json_objOpen(json_dest, "general");


    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                Step 1 - Examining general configuration                  ***");
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");


    UM_INFO(GBEM_UM_EN, "GBEM: ECRXTX_REMINDER_MESSAGE_INTERVAL_MS [%u]", ECRXTX_REMINDER_MESSAGE_INTERVAL_MS);
    UM_INFO(GBEM_UM_EN, "GBEM: ECRXTX_GBC_CONNECTION_CHECK_INTERVAL_MS [%u]", ECRXTX_GBC_CONNECTION_CHECK_INTERVAL_MS);

    UM_INFO(GBEM_UM_EN, "GBEM: MAX_NUM_SLAVE_ERROR_MESSAGES [%u]", MAX_NUM_SLAVE_ERROR_MESSAGES);
    UM_INFO(GBEM_UM_EN, "GBEM: JSON_STATUS_UPDATE_PERIOD_SECS [%u]", JSON_STATUS_UPDATE_PERIOD_SECS);
    UM_INFO(GBEM_UM_EN, "GBEM: MAX_NUMBER_MAP_ENTRIES_IN_IOMAP [%u]", MAX_NUMBER_MAP_ENTRIES_IN_IOMAP);

#if FLAVOUR == PLATFORM_PI
        UM_INFO(GBEM_UM_EN, "GBEM: FLAVOUR is [GENERIC_LINUX]");
#elif FLAVOUR == PLATFORM_LINUX
        UM_INFO(GBEM_UM_EN, "GBEM: FLAVOUR is [GENERIC_LINUX]");
#else
        UM_ERROR(GBEM_UM_EN, "GBEM: No FLAVOUR has been set");
        config_error = true;
#endif

    UM_INFO(GBEM_UM_EN, "GBEM: Software version [%s]", PROJECT_VER);
    json_dest = json_str(json_dest, "gbem_version", PROJECT_VER);

    UM_INFO(GBEM_UM_EN, "GBEM: Machine map in use [%s]", map_machine_type_strings[map_machine_type]);
    json_dest = json_str(json_dest, "machine_map_name", map_machine_type_strings[map_machine_type]);

    UM_INFO(GBEM_UM_EN,
            "GBEM: MAP_CYCLE_TIME [%u ms] (This is the cycle time for the fieldbus cyclic communication and defines the frequency at which GBC is signalled)",
            MAP_CYCLE_TIME);

    json_dest = json_uint(json_dest, "map_cycle_time", MAP_CYCLE_TIME);

    if (!(MAP_CYCLE_TIME == 1 || MAP_CYCLE_TIME == 2 || MAP_CYCLE_TIME == 4 || MAP_CYCLE_TIME == 8)) {
        UM_WARN(GBEM_UM_EN,
                "GBEM: Strange cycle time is set. Usually this is 1, 2, 4 or 8 ms. Maybe you know what you are up to...");
    }

    UM_INFO(GBEM_UM_EN,
            "GBEM: MAP_GLOBAL_DC_SYNC_OFFSET [%u ns] (This is the global offset applied to all EtherCAT DC clocks)",
            MAP_GLOBAL_DC_SYNC_OFFSET);
    json_dest = json_uint(json_dest, "map_global_dc_sync_offset", MAP_GLOBAL_DC_SYNC_OFFSET);

    if (MAP_GLOBAL_DC_SYNC_OFFSET > 1000000) {
        UM_WARN(GBEM_UM_EN, "GBEM: Large global sync offset is set (>1,000,000). Maybe you know what you are up to...");
    }

    UM_INFO(GBEM_UM_EN,
            "GBEM: GBC_PROCESS_NAME [%s] (This is the hardcoded default process name we try and connect over shared memory with unless one is specified with a -p command line option)",
            GBC_PROCESS_NAME);
    json_dest = json_str(json_dest, "gbc_process_name", GBC_PROCESS_NAME);
    if (strlen(GBC_PROCESS_NAME) < 1) {
        UM_WARN(GBEM_UM_EN, "GBEM: GBC_PROCESS_NAME is an empty string\n");
    }

    UM_INFO(GBEM_UM_EN,
            "GBEM: ECM_IO_MAP_SIZE [%u] (This is the size in bytes of the in-memory buffer that holds the data r/w to the EtherCAT network, it must be larger than all the pdo data from all the slaves on teh network)",
            ECM_IO_MAP_SIZE);

    json_dest = json_uint(json_dest, "ecm_io_map_size", ECM_IO_MAP_SIZE);

    UM_INFO(GBEM_UM_EN, "GBEM: SIZE_OF_GBC_PDO [%u] (This is the size of the shared memory object shared with GBC)",
            SIZE_OF_GBC_PDO);
    json_dest = json_uint(json_dest, "size_of_gbc_pdo", SIZE_OF_GBC_PDO);

    UM_INFO(GBEM_UM_EN,
            "GBEM: MAP_MAX_NUM_DRIVES [%u] (This is the maximum number of drives that can be configured on GBEM)",
            MAP_MAX_NUM_DRIVES);
    json_dest = json_uint(json_dest, "map_max_num_drives", MAP_MAX_NUM_DRIVES);

    UM_INFO(GBEM_UM_EN,
            "GBEM: CTRL_HEARTBEAT_TOLERANCE [%u] (This is the number of cycles before which the lost heartbeat error control kicks in)",
            CTRL_HEARTBEAT_TOLERANCE);
    json_dest = json_uint(json_dest, "ctrl_heartbeat_tolerance", CTRL_HEARTBEAT_TOLERANCE);

    UM_INFO(GBEM_UM_EN,
            "GBEM: CTRL_DRIVE_CHANGE_STATE_TIMEOUT [%u] (This is the number of cycles a drive must respond to a command before state change error control kicks in - it must be longer than drive stopping time)",
            CTRL_DRIVE_CHANGE_STATE_TIMEOUT);
    json_dest = json_uint(json_dest, "ctrl_drive_change_state_timeout", CTRL_DRIVE_CHANGE_STATE_TIMEOUT);

    UM_INFO(GBEM_UM_EN,
            "GBEM: MAX_DRIVE_NAME_LENGTH [%u] (This is the maximum length of a drive name supported by the system)",
            MAX_DRIVE_NAME_LENGTH);
    json_dest = json_uint(json_dest, "max_drive_name_length", MAX_DRIVE_NAME_LENGTH);

    UM_INFO(GBEM_UM_EN,
            "GBEM: Size of dpm in struct [%u] (This is the size of the struct overlaid on the shared memory)",
            (uint32_t) sizeof(dpm_in_t));
    json_dest = json_uint(json_dest, "size_of_dpm_in", (uint32_t) sizeof(dpm_in_t));

    UM_INFO(GBEM_UM_EN,
            "GBEM: Size of dpm out struct [%u] (This is the size of the struct overlaid on the shared memory)",
            (uint32_t) sizeof(dpm_out_t));
    json_dest = json_uint(json_dest, "size_of_dpm_out", (uint32_t) sizeof(dpm_out_t));


    if ((SIZE_OF_GBC_PDO != (uint32_t) sizeof(dpm_in_t)) || (SIZE_OF_GBC_PDO != (uint32_t) sizeof(dpm_out_t))) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Sizes of PDO do not match! (SIZE_OF_GBC_PDO or dpm_in/out structs");
        config_error = true;
    }

    UM_INFO(GBEM_UM_EN,
            "GBEM: MAX_DRIVE_ERROR_MSG_LENGTH [%u] (This is the maximum length in bytes of a drive error message)",
            MAX_DRIVE_ERROR_MSG_LENGTH);

    json_dest = json_uint(json_dest, "max_drive_error_msg_length", MAX_DRIVE_ERROR_MSG_LENGTH);
    UM_INFO(GBEM_UM_EN,
            "GBEM: emstat (ENABLE_EMSTAT) is %s", ENABLE_EMSTAT == 1 ? "enabled" : "disabled");

    json_dest = json_bool(json_dest, "emstat_enabled", ENABLE_EMSTAT == 1 ? true : false);


    if (config_error) {
        *grc = E_INVALID_CONFIG;
    } else {
        *grc = E_SUCCESS;
    }

    json_dest = json_objClose(json_dest);
    return json_dest;
}

/**
 * @brief Examines the IO configuration - checks, prints summary and creates JSON config summary
 * @param json_dest
 * @param grc
 * @return
 */
static char *config_process_io(char *json_dest, gberror_t *grc) {
    bool config_error = false;
    json_dest = json_objOpen(json_dest, "io");

    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                Step 3 - Examining IO configuration                       ***");
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");

#if DISABLE_ESTOP_CHECKING == 1
    UM_WARN(GBEM_UM_EN, "GBEM: ESTOP checking is disabled!");
#else
    UM_INFO(GBEM_UM_EN, "GBEM: ESTOP checking is >not< disabled");
#endif

#if DISABLE_HEARTBEAT_CHECKING == 1
    UM_WARN(GBEM_UM_EN, "GBEM: Heartbeat checking is disabled!");
#else
    UM_INFO(GBEM_UM_EN, "GBEM: Heartbeat checking is >not< disabled");
#endif

//USE_ESTOP_RESET is a define to control whether GBEM processes the reset signal for estop or whether the estop state is provided by the electricals

#if DISABLE_ESTOP_CHECKING != 1
if (ctrl_estop_din.slave_num>1){
    UM_INFO(GBEM_UM_EN, "GBEM: ESTOP is on slave [%u] and bit number [%u]", ctrl_estop_din.slave_num, ctrl_estop_din.bit_num);
} else{
    UM_ERROR(GBEM_UM_EN, "GBEM: No ESTOP signal is defined. This is an error");
    config_error = true;
}

#ifdef USE_ESTOP_RESET
    if (ctrl_estop_reset_din.slave_num>1){

        UM_INFO(GBEM_UM_EN, "GBEM: ESTOP Reset is on slave [%u] and bit number [%u]", ctrl_estop_reset_din.slave_num, ctrl_estop_reset_din.bit_num);
        //    UM_INFO(GBEM_UM_EN,
        //            "GBEM: Estop on Digital In: %d, and requires a reset on Digital In: %d (USE_ESTOP_RESET is #defined)\n",
    }else{
        UM_ERROR(GBEM_UM_EN, "GBEM: No ESTOP reset signal is defined. This is an error when USE_ESTOP_RESET is defined");
        config_error = true;
    }
#else
    UM_INFO(GBEM_UM_EN, "GBEM: Estop on Digital In: %d, and does NOT require a reset signal (USE_ESTOP_RESET is NOT #defined)");
#endif
#endif
    //todo add to json estop info


    UM_INFO(GBEM_UM_EN, "GBEM: Max number of GBC digital ins [%u] (DPM_NUM_DIGITAL)",
            DPM_NUM_DIGITALS);
    json_dest = json_uint(json_dest, "max_gbc_din", DPM_NUM_DIGITALS);

    UM_INFO(GBEM_UM_EN, "GBEM: Max number of GBC digital outs [%u] (MAP_GBC_NUM_DIGITAL_OUT)",
            DPM_NUM_DIGITALS);
    json_dest = json_uint(json_dest, "max_gbc_dout", DPM_NUM_DIGITALS);
    UM_INFO(GBEM_UM_EN, "GBEM: Max number of GBC integer32 ins [%u] (MAP_GBC_NUM_INTEGER32_IN)",
            DPM_NUM_INT32S);
    json_dest = json_uint(json_dest, "max_gbc_i32_in", DPM_NUM_INT32S);
    UM_INFO(GBEM_UM_EN, "GBEM: Max number of GBC integer32 outs [%u] (MAP_GBC_NUM_INTEGER32_OUT)",
            DPM_NUM_INT32S);
    json_dest = json_uint(json_dest,"max_gbc_i32_out", DPM_NUM_INT32S);
    UM_INFO(GBEM_UM_EN, "GBEM: Max number of GBC unsigned32 ins [%u] {MAP_GBC_NUM_UNSIGNED32_IN)",
            DPM_NUM_UINT32S);
    json_dest = json_uint(json_dest, "max_gbc_u32_in", DPM_NUM_UINT32S);
    UM_INFO(GBEM_UM_EN, "GBEM: Max number of GBC unsigned outs [%u] (MAP_GBC_NUM_UNSIGNED32_OUT)",
            DPM_NUM_UINT32S);
    json_dest = json_uint(json_dest, "max_gbc_u32_out", DPM_NUM_UINT32S);
    UM_INFO(GBEM_UM_EN, "GBEM: Max number of GBC analogs(float) ins [%u] (MAP_GBC_NUM_FLOAT_IN)",
            DPM_NUM_ANALOGS);
    json_dest = json_uint(json_dest, "max_gbc_float_in", DPM_NUM_ANALOGS);
    UM_INFO(GBEM_UM_EN, "GBEM: Max number of GBC analogs(float) outs  [%u] (MAP_GBC_NUM_FLOAT_OUT)",
            DPM_NUM_ANALOGS);
    json_dest = json_uint(json_dest, "max_gbc_float_out", DPM_NUM_ANALOGS);


    UM_INFO(GBEM_UM_EN, "GBEM: Number of rows in io map [%u]", map_num_rows_in_iomap);
    json_dest = json_uint(json_dest, "num_rows_in_iomap", map_num_rows_in_iomap);


    UM_INFO(GBEM_UM_EN, "GBEM: Starting iomap checking");

    int tests_run=0;
    int tests_passed=0;
//check for missing plc info
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].plc.inout == MAP_IN || map_iomap[i].plc.inout == MAP_OUT) {
            tests_run++;
            if (map_iomap[i].plc.io == NULL) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: iomap is missing a PLC variable. Problem on row [%u]",
                         i);
            }else{
                tests_passed++;
            }

            tests_run++;
            if (map_iomap[i].plc.linked_task_name[0] == '\0') {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: iomap is missing a PLC task. Problem on row [%u]",
                         i);
            }else{
                tests_passed++;
            }
            tests_run++;
            if (!map_is_valid_plc_datatype(map_iomap[i].gbc.datatype)) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: iomap contains an invalid PLC type (not all ec_datatypes are supported in the PLC write functions). Problem on row [%u]",
                         i);
            }else{
                tests_passed++;
            }
        }
    }

    //check for missing gbc info
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].gbc.inout == MAP_IN || map_iomap[i].gbc.inout == MAP_OUT) {
            tests_run++;
            if (!map_is_valid_gbc_datatype(map_iomap[i].gbc.datatype)) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: iomap contains an invalid GBC type (not all ec_datatypes are supported in the DPM). Problem on row [%u]",
                         i);
            }
            else{
                tests_passed++;
            }
        }

        if (map_iomap[i].gbc.datatype==ECT_REAL32){
            tests_run++;
            //we have a gbc analog (i.e. real32 float) this means we need a max value if the corresponding max val
            if(map_iomap[i].pdo.datatype!=ECT_REAL32){

                if (map_iomap[i].pdo.max_val==0){
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: iomap is missing a max value on the PDO data type as we going to convert an int to a float. Problem on row [%u]",
                             i);

                }else{
                    tests_passed++;
                }
            }else{
                tests_passed++;
            }
        }
    }

    //check for missing pdo info
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].pdo.inout == MAP_IN || map_iomap[i].pdo.inout == MAP_OUT) {
            tests_run++;
            if (map_iomap[i].pdo.slave_num == 0) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: iomap is missing a PDO slave number. Problem on row [%u]", i);
            }else{
                tests_passed++;
            }

            tests_run++;
            if (!map_is_valid_pdo_datatype(map_iomap[i].pdo.datatype)) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: iomap contains an invalid PDO type (not all ec_datatypes are supported in PDO write functions). Problem on row [%u]",
                         i);
            }else{
                tests_passed++;
            }
        }
    }

    //check for invalid inout rows
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_IN &&
            map_iomap[i].plc.inout == MAP_OUT) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#1) iomap has a row with a PDO(in) and GBC(in) and PLC(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 1
             * PDO  | GBC  | PLC
             * in   | in   | out
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_OUT &&
            map_iomap[i].plc.inout == MAP_IN) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#3) iomap has a row with a PDO(in) and GBC(out) and PLC(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 3
             * PDO  | GBC  | PLC
             * in   | out  | in
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_OUT &&
            map_iomap[i].plc.inout == MAP_OUT) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#4) iomap has a row with a PDO(in) and GBC(out) and PLC(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 4
             * PDO  | GBC  | PLC
             * in   | out  | out
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_OUT &&
            map_iomap[i].plc.inout == MAP_UNDEFINED) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#5) iomap has a row with a PDO(in) and GBC(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 5
             * PDO  | GBC  | PLC
             * in   | out  | none
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
            map_iomap[i].plc.inout == MAP_OUT) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#7) iomap has a row with a PDO(in) and PLC(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 7
             * PDO  | GBC  | PLC
             * in   | none | out
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
            map_iomap[i].plc.inout == MAP_UNDEFINED) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#8) iomap has a row with just a PDO(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 8
             * PDO  | GBC  | PLC
             * in   | none | none
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_IN &&
            map_iomap[i].plc.inout == MAP_IN) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#9) iomap has a row with a PDO(out) and GBC(in) and PLC(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 9
             * PDO  | GBC  | PLC
             * out  | in  | in
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_IN &&
            map_iomap[i].plc.inout == MAP_OUT) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#10) iomap has a row with a PDO(out) and GBC(in) and PLC(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 10
             * PDO  | GBC  | PLC
             * out  | in   | out
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_IN &&
            map_iomap[i].plc.inout == MAP_UNDEFINED) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#11) iomap has a row with a PDO(out) and GBC(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 11
             * PDO  | GBC  | PLC
             * out  | in   | none
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_OUT &&
            map_iomap[i].plc.inout == MAP_IN) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#12) iomap has a row with a PDO(out) and GBC(out) and PLC(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 12
             * PDO  | GBC  | PLC
             * out  | out  | in
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
            map_iomap[i].plc.inout == MAP_IN) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#15) iomap has a row with a PDO(out) and PLC(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 15
             * PDO  | GBC  | PLC
             * out  | none | in
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
            map_iomap[i].plc.inout == MAP_UNDEFINED) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#17) iomap has a row with just a PDO(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 17
             * PDO  | GBC  | PLC
             * out  | none | none
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_IN &&
            map_iomap[i].plc.inout == MAP_IN) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#18) iomap has a row with a GBC(in) and PLC(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 18
             * PDO  | GBC  | PLC
             * none | in   | in
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_IN &&
            map_iomap[i].plc.inout == MAP_UNDEFINED) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#20) iomap has a row with just a GBC(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 20
             * PDO  | GBC  | PLC
             * none | in   | none
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_OUT &&
            map_iomap[i].plc.inout == MAP_OUT) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#22) iomap has a row with a GBC(out) and PLC(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 22
             * PDO  | GBC  | PLC
             * none | out  | out
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_OUT &&
            map_iomap[i].plc.inout == MAP_UNDEFINED) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#23) iomap has a row with just a GBC(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 23
             * PDO  | GBC  | PLC
             * none | out  | none
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
            map_iomap[i].plc.inout == MAP_IN) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#24) iomap has a row with just a PLC(in) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 24
             * PDO  | GBC  | PLC
             * none | none | in
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
            map_iomap[i].plc.inout == MAP_OUT) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#25) iomap has a row with just a PLC(out) defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 25
             * PDO  | GBC  | PLC
             * none | none | out
             */
        }else{
            tests_passed++;
        }

        tests_run++;
        if (map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
            map_iomap[i].plc.inout == MAP_UNDEFINED) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: (#26) iomap has a row with a nothing defined. This doesn't make sense. Problem is on row [%u]",
                     i);
            /* Row 26
             * PDO  | GBC  | PLC
             * none | none | none
             */
        }
        else{
            tests_passed++;
        }
    } //end of massive for loop

    if (tests_run!=tests_passed){
        config_error=true;
    }

    UM_INFO(GBEM_UM_EN, "GBEM: End of iomap checking [%d] tests were run and [%d] tests passed, %s", tests_run, tests_passed, config_error ? ">ISSUES WERE FOUND<" : "issues were not found");

    json_dest = json_arrOpen(json_dest, "iomap");

    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        json_dest = json_objOpen(json_dest, NULL);
        json_dest = json_str(json_dest, "pdo_inout", map_iomap[i].pdo.inout == MAP_IN ? "IN" : "OUT");
        json_dest = json_uint(json_dest, "pdo_slave_num", map_iomap[i].pdo.slave_num);
        json_dest = json_uint(json_dest, "pdo_bit_num", map_iomap[i].pdo.bit_num);
        json_dest = json_uint(json_dest, "pdo_byte_num", map_iomap[i].pdo.byte_num);
        json_dest = json_str(json_dest, "pdo_type",  ec_datatype_string[map_iomap[i].pdo.datatype]);
        json_dest = json_double(json_dest, "pdo_bit_byte_num", map_iomap[i].pdo.max_val);

        json_dest = json_str(json_dest, "gbc_inout", map_iomap[i].gbc.inout == MAP_IN ? "IN" : "OUT");
        json_dest = json_str(json_dest, "gbc_type",  ec_datatype_string[map_iomap[i].gbc.datatype]);
        json_dest = json_uint(json_dest, "gbc_ionum", map_iomap[i].gbc.ionum);

        json_dest = json_str(json_dest, "plc_inout", map_iomap[i].plc.inout == MAP_IN ? "IN" : "OUT");
        json_dest = json_str(json_dest, "plc_type",  ec_datatype_string[map_iomap[i].plc.datatype]);
        json_dest = json_str(json_dest, "plc_type",  map_iomap[i].plc.linked_task_name);

        json_dest= json_objClose(json_dest);
    }
    json_dest = json_arrClose(json_dest);


        UM_INFO(GBEM_UM_EN, "GBEM: Summary of iomap:");
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        /* copy from PDO(in) to GBC(in) and PLC (in) #0
        * PDO | GBC | PLC
        * in  | in  | in
        */
        if ((map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_IN &&
             map_iomap[i].plc.inout == MAP_IN)) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: GBC [%s.%u] (%s) and PLC [%s@0x%p] (%s) in task (%s) are linked to a PDO [%s.%u.%u.%u] [%s] (%s)",
                    map_iomap[i].gbc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].gbc.ionum,
                    ec_datatype_string[map_iomap[i].gbc.datatype], map_iomap[i].plc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].plc.io,
                    ec_datatype_string[map_iomap[i].plc.datatype],
                    map_iomap[i].plc.linked_task_name, map_iomap[i].pdo.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].pdo.slave_num,
                    map_iomap[i].pdo.byte_num,map_iomap[i].pdo.bit_num,
                    ecm_slave_map[map_iomap[i].pdo.slave_num - 1].name,
                    ec_datatype_string[map_iomap[i].pdo.datatype]);
        }
        /* copy from PDO(in) to GBC(in) #2
        * PDO | GBC  | PLC
        * in  | in   | none
        */
        if ((map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_IN &&
             map_iomap[i].plc.inout == MAP_UNDEFINED)) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: GBC [%s.%u] (%s) is linked to a PDO [%s.%u.%u.%u] [%s] (%s)",
                    map_iomap[i].gbc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].gbc.ionum,
                    ec_datatype_string[map_iomap[i].gbc.datatype],
                    map_iomap[i].pdo.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].pdo.slave_num,
                    map_iomap[i].pdo.byte_num,map_iomap[i].pdo.bit_num,
                    ecm_slave_map[map_iomap[i].pdo.slave_num - 1].name,
                    ec_datatype_string[map_iomap[i].pdo.datatype]);


        }
        /* copy from PDO(in) to PLC(in) #6
        * PDO | GBC  | PLC
        * in  | none | in
        */
        if ((map_iomap[i].pdo.inout == MAP_IN && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
             map_iomap[i].plc.inout == MAP_IN)) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: PLC [%s@0x%p] (%s) in task (%s) is linked to a PDO [%s.%u.%u.%u] [%s] (%s)",
                    map_iomap[i].plc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].plc.io,
                    ec_datatype_string[map_iomap[i].plc.datatype],
                    map_iomap[i].plc.linked_task_name, map_iomap[i].pdo.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].pdo.slave_num,
                    map_iomap[i].pdo.byte_num, map_iomap[i].pdo.bit_num,
                    ecm_slave_map[map_iomap[i].pdo.slave_num - 1].name,
                    ec_datatype_string[map_iomap[i].pdo.datatype]);
        }

        /* copy from GBC(out) OR PLC(out) to PDO(out) #13
        * PDO | GBC | PLC
        * out | out | out
        */
        if ((map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_OUT &&
             map_iomap[i].plc.inout == MAP_OUT)) {

            UM_INFO(GBEM_UM_EN,
                    "GBEM: GBC [%s.%u] (%s) and PLC [%s@0x%p] (%s) in task (%s) are linked to a PDO [%s.%u.%u.%u] [%s] (%s)",
                    map_iomap[i].gbc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].gbc.ionum,
                    ec_datatype_string[map_iomap[i].gbc.datatype], map_iomap[i].plc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].plc.io,
                    ec_datatype_string[map_iomap[i].plc.datatype],
                    map_iomap[i].plc.linked_task_name, map_iomap[i].pdo.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].pdo.slave_num,
                    map_iomap[i].pdo.byte_num,map_iomap[i].pdo.bit_num,
                    ecm_slave_map[map_iomap[i].pdo.slave_num - 1].name,
                    ec_datatype_string[map_iomap[i].pdo.datatype]);

        }


        /* copy from GBC(out) PDO(out) #14
        * PDO | GBC | PLC
        * out | out | none
        */
        if ((map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_OUT &&
             map_iomap[i].plc.inout == MAP_UNDEFINED)) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: GBC [%s.%u] (%s) is linked to a PDO [%s.%u.%u.%u] [%s] (%s)",
                    map_iomap[i].gbc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].gbc.ionum,
                    ec_datatype_string[map_iomap[i].gbc.datatype], map_iomap[i].pdo.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].pdo.slave_num,
                    map_iomap[i].pdo.byte_num,map_iomap[i].pdo.bit_num,
                    ecm_slave_map[map_iomap[i].pdo.slave_num - 1].name,
                    ec_datatype_string[map_iomap[i].pdo.datatype]);

        }
        /* copy from PLC(out) to PDO(out) #16
        * PDO | GBC  | PLC
        * out | none | out
        */
        if ((map_iomap[i].pdo.inout == MAP_OUT && map_iomap[i].gbc.inout == MAP_UNDEFINED &&
             map_iomap[i].plc.inout == MAP_OUT)) {

            UM_INFO(GBEM_UM_EN,
                    "GBEM: PLC [%s@0x%p] (%s) in task (%s) is linked to a PDO [%s.%u.%u.%u] [%s] (%s)",
                    map_iomap[i].plc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].plc.io,
                    ec_datatype_string[map_iomap[i].plc.datatype],
                    map_iomap[i].plc.linked_task_name, map_iomap[i].pdo.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].pdo.slave_num,
                    map_iomap[i].pdo.byte_num,map_iomap[i].pdo.bit_num,
                    ecm_slave_map[map_iomap[i].pdo.slave_num - 1].name,
                    ec_datatype_string[map_iomap[i].pdo.datatype]);

        }
        /* copy from PLC(out) to GBC(in) #19
        * PDO  | GBC | PLC
        * none | in  | out
        */

        if ((map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_IN &&
             map_iomap[i].plc.inout == MAP_OUT)) {
            UM_INFO(GBEM_UM_EN,
                    "GBEM: GBC [%s.%u] (%s) and PLC [%s@0x%p] (%s) in task (%s) are linked",
                    map_iomap[i].gbc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].gbc.ionum,
                    ec_datatype_string[map_iomap[i].gbc.datatype], map_iomap[i].plc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].plc.io,
                    ec_datatype_string[map_iomap[i].plc.datatype],
                    map_iomap[i].plc.linked_task_name);
        }

        /* copy from GBC(out) to PLC(in) #21
        * PDO  | GBC | PLC
        * none | out  | in
        */
        if ((map_iomap[i].pdo.inout == MAP_UNDEFINED && map_iomap[i].gbc.inout == MAP_OUT &&
             map_iomap[i].plc.inout == MAP_IN)) {

            UM_INFO(GBEM_UM_EN,
                    "GBEM: GBC [%s.%u] (%s) and PLC [%s@0x%p] (%s) in task (%s) are linked",
                    map_iomap[i].gbc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].gbc.ionum,
                    ec_datatype_string[map_iomap[i].gbc.datatype], map_iomap[i].plc.inout == MAP_IN ? "IN" : "OUT", map_iomap[i].plc.io,
                    ec_datatype_string[map_iomap[i].plc.datatype],
                    map_iomap[i].plc.linked_task_name);

        }

    } //end of massive for loop

    if (config_error) {
        *grc = E_INVALID_CONFIG;
    } else {
        *grc = E_SUCCESS;
    }
    json_dest = json_objClose(json_dest);
    return json_dest;
}


/**
 * @brief Examines the slave config - checks, prints summary and creates JSON config summary
 * @param json_dest
 * @param grc
 * @return
 */
static char *config_process_slaves(char *json_dest, gberror_t *grc) {
    bool config_error = false;
    json_dest = json_objOpen(json_dest, "slaves");

    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                Step 4 - Examining slave configuration                    ***");
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");

    //#define ECM_CHECK_EEP_MAN
//#define ECM_CHECK_EEP_REV
//#define ECM_CHECK_EEP_ID

#ifdef ECM_CHECK_EEP_MAN
    UM_WARN(GBEM_UM_EN, "GBEM: Slave  manufacturer checking (#define ECM_CHECK_EEP_MAN) is enabled. This means that all slaves must have their EEP_MFR completed in the ecm_slave_map array");
#endif


#ifdef ECM_CHECK_EEP_ID
    UM_WARN(GBEM_UM_EN, "GBEM: Slave ID checking (#define ECM_CHECK_EEP_ID) is enabled. This means that all slaves must have their EEP_ID completed in the ecm_slave_map array");
#endif

#ifdef ECM_CHECK_EEP_REV
    UM_WARN(GBEM_UM_EN, "GBEM: Slave revision checking (#define ECM_CHECK_EEP_REV) is enabled. This means that all slaves must have their EEP_REV completed in the ecm_slave_map array");
#endif


    UM_INFO(GBEM_UM_EN, "GBEM: Number of slaves defined [%u]", MAP_NUM_SLAVES);

    json_dest = json_uint(json_dest, "number_of_slaves_defined", MAP_NUM_SLAVES);

    if (GET_ARRAY_LEN(map_dc_type) != MAP_NUM_SLAVES) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Not all slaves have a DC type (check map_dc_type[])");
        config_error = true;
    }

    if (check_slave_function_ptrs(MAP_NUM_SLAVES)!=E_SUCCESS){
        UM_ERROR(GBEM_UM_EN, "GBEM: Error checking slave function pointers");
        config_error = true;
    }

    bool dc_on_any_slave = false;
    json_dest = json_arrOpen(json_dest, "slaves");
    for (int i = 1; i < MAP_NUM_SLAVES + 1; i++) {

        if (strlen(ecm_slave_map[i - 1].name) == 0) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Slave number [%u] is missing its name in the ecm_slave_map array", i);
            config_error = true;
        }
#ifdef ECM_CHECK_EEP_ID
            else if (ecm_slave_map[i-1].eep_id==0){
                UM_ERROR(GBEM_UM_EN, "GBEM: Slave number: %u is missing its EEP ID in (ecm_slave_map) - as full slave checking is enabled (#define ECM_CHECK_EEP) GBEM will not boot until these are complete in the array ecm_slave_map");
                          config_error = true;
            }
#endif
#ifdef ECM_CHECK_EEP_REV
            else if (ecm_slave_map[i-1].eep_rev==0){
            UM_ERROR(GBEM_UM_EN, "GBEM: Slave number: %u is missing its EEP REVISION in (ecm_slave_map) - as full slave checking is enabled (#define ECM_CHECK_EEP) GBEM will not boot until these are complete in the array ecm_slave_map");
            config_error = true;
            }
#endif
#ifdef ECM_CHECK_EEP_MAN
            else if (ecm_slave_map[i-1].eep_man==0){
            UM_ERROR(GBEM_UM_EN, "GBEM: Slave number: %u is missing its EEP MANUFACTURER in (ecm_slave_map) - as full slave checking is enabled (#define ECM_CHECK_EEP) GBEM will not boot until these are complete in the array ecm_slave_map");
            config_error = true;
            }
#endif
        else {
            json_dest = json_objOpen(json_dest, NULL);
            UM_INFO(GBEM_UM_EN, "GBEM: Slave number [%u]", i);
            json_dest = json_uint(json_dest, "slave_number", (uint32_t) i);
            UM_INFO(GBEM_UM_EN, "GBEM: \tName [%s]", ecm_slave_map[i - 1].name);
            json_dest = json_str(json_dest, "slave_name", ecm_slave_map[i - 1].name);

            gberror_t etg_lookup_vendor_id_grc = E_GENERAL_FAILURE;
            UM_INFO(GBEM_UM_EN, "GBEM: \tEEP ID [%#.8x], EEP MANUFACTURER [%#.8x] (%s), EEP REVISION [%#.8x]",
                    ecm_slave_map[i - 1].eep_id, ecm_slave_map[i - 1].eep_man,
                    etg_lookup_vendor_id(ecm_slave_map[i - 1].eep_man, etg_vendor_id_list, ETG_NUM_VENDOR_IDS,
                                         &etg_lookup_vendor_id_grc),
                    ecm_slave_map[i - 1].eep_rev);

            UM_INFO(GBEM_UM_EN, "GBEM: \tNumber of drives attached to this slave is [%u]",
                    map_num_drives_attached[i - 1]);
            json_dest = json_uint(json_dest, "num_drives_attached_to_slave", map_num_drives_attached[i - 1]);
        }

        switch (map_dc_type[i - 1]) {
            case EC_DC_UNCONFIGURED:
                UM_INFO(GBEM_UM_EN, "GBEM: Slave has un-configured DC settings - this is a problem");
                config_error = true;
            case EC_DC_NONE:
                UM_INFO(GBEM_UM_EN, "GBEM: \tSlave does not have DC enabled");
                json_dest = json_str(json_dest, "slave_dc_type", "none");
                break;
            case EC_DC_0:
                UM_INFO(GBEM_UM_EN, "GBEM: \tSlave has DC0 enabled");
                dc_on_any_slave = true;
                json_dest = json_str(json_dest, "slave_dc_type", "DC0");
                break;
            case EC_DC_01:
                UM_INFO(GBEM_UM_EN, "GBEM: \tSlave has DC01 enabled");
                dc_on_any_slave = true;
                json_dest = json_str(json_dest, "slave_dc_type", "DC01");
                break;
            default:
                LL_FATAL("GBEM: DC case missing type");
        }
        json_dest = json_uint(json_dest, "slave_dc_cycle", (unsigned int) map_dc_cycle[i-1]);
        UM_INFO(GBEM_UM_EN, "GBEM: \tSlave has DC cycle time of [%u]",map_dc_cycle[i-1] );
        //if the dc cycle time is more than say 12 ms then flag a warning
        if (map_dc_cycle[i-1] > 12){
            UM_WARN(GBEM_UM_EN, "GBEM: Slave has what looks like an invalid DC cycle time [%u]",map_dc_cycle[i-1]);
            config_error = true;
        }


        json_dest = json_objClose(json_dest);
    }
    json_dest = json_arrClose(json_dest);

    if (!dc_on_any_slave) {
        UM_WARN(GBEM_UM_EN, "GBEM: There is no slave with a DC configuration, slaves will not be DC synchronised");
    }

#ifndef ECM_CHECK_EEP
    UM_INFO(GBEM_UM_EN,
            "GBEM: As ECM_CHECK_EEP is not defined, missing EEP_ID, EEP MANUFACTURER and EEP REVISION info. in the ecm_slave_map array is NOT important");
#endif

    if (config_error) {
        *grc = E_INVALID_CONFIG;
    } else {
        *grc = E_SUCCESS;
    }

    json_dest = json_objClose(json_dest);
    return json_dest;
}

/**
 * @brief Examines the drive config - checks, prints summary and creates JSON config summary
 * @param json_dest
 * @param grc
 * @return
 */
static char *config_process_drives(char *json_dest, gberror_t *grc) {
    bool config_error = false;
    json_dest = json_objOpen(json_dest, "drives");

    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                Step 2 - Examining drive configuration                    ***");
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");

    if (MAP_NUM_DRIVES == 0) {
        UM_WARN(GBEM_UM_EN, "GBEM: No drives are configured in the map");
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: Number of drives configured [%u]", MAP_NUM_DRIVES);
    }
    json_dest = json_uint(json_dest, "drive_total)num", MAP_NUM_DRIVES);

    if (GET_ARRAY_LEN(map_drive_to_name) != MAP_NUM_DRIVES) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Incorrect number of drive names in map_drive_names[] (is %u should be %u)",
                 MAP_NUM_DRIVES,
                 (uint32_t) GET_ARRAY_LEN(map_drive_to_name));
        config_error = true;
    }
    json_dest = json_arrOpen(json_dest, "drive_data");
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        json_dest = json_objOpen(json_dest, NULL);
        if (map_drive_to_name[i] == NULL) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Drive[%u] name is NULL please add a map_drive_names entry", i);
            config_error = true;
        } else if (strlen(map_drive_to_name[i]) == 0) {
            UM_ERROR(GBEM_UM_EN, "GBEM: Drive[%u] name is an empty string please add a map_drive_names entry", i);
            config_error = true;
        } else {
            UM_INFO(GBEM_UM_EN, "GBEM: Drive [%u]", i);
            json_dest = json_uint(json_dest, "drive_num", (uint32_t ) i);
            UM_INFO(GBEM_UM_EN, "GBEM: \tName [%s]", map_drive_to_name[i]);
            json_dest = json_str(json_dest, "drive_name", map_drive_to_name[i]);
            UM_INFO(GBEM_UM_EN, "GBEM: \tDrive is on slave [%u] (%s)", map_drive_to_slave[i],
                    map_drive_to_name[i]);
            json_dest = json_uint(json_dest, "drive_slave_num", map_drive_to_slave[i] );
            json_dest = json_str(json_dest, "drive_slave_name", map_drive_to_name[i]);
            UM_INFO(GBEM_UM_EN, "GBEM: \tSub-drive number [%u]", map_drive_to_subdrive[i]);
            json_dest = json_uint(json_dest, "drive_sub_drive_num",  map_drive_to_subdrive[i]);
            UM_INFO(GBEM_UM_EN, "GBEM: \tPositive limit [%d], Negative limit [%d]", map_drive_pos_limit[i],
                    map_drive_neg_limit[i]);
            json_dest = json_int(json_dest, "drive_pos_limit",  map_drive_pos_limit[i]);
            json_dest = json_int(json_dest, "drive_sub_drive_num",  map_drive_neg_limit[i]);

        }
        json_dest = json_objClose(json_dest);
    }
    json_dest = json_arrClose(json_dest);

    if (check_drive_function_ptrs(MAP_NUM_DRIVES) != E_SUCCESS) {
        UM_ERROR(GBEM_UM_EN, "GBEM: One or more drives has problems with its function pointers");
        config_error = true;
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: All mandatory function pointers are assigned to functions for drives");
    }

    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
//        printf("Drive[%u] error code index = %#04x\n", map_drives_error_code[i]
    }

    if (config_error) {
        *grc = E_INVALID_CONFIG;
    } else {
        *grc = E_SUCCESS;
    }

    json_dest = json_objClose(json_dest);
    return json_dest;
}

/**
 * @brief Examines the PLC config - checks, prints summary and creates JSON config summary
 * @param json_dest
 * @param grc
 * @return
 */
char *config_process_plc(char *json_dest, gberror_t *grc) {

    bool config_error = false;

    json_dest = json_objOpen(json_dest, "plc");

    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                  Step 5 - Examining PLC configuration                    ***");
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");


    UM_INFO(GBEM_UM_EN, "GBEM: Number of PLC tasks defined [%u]", plc_task_set.num_tasks_defined);

    json_dest= json_uint(json_dest, "plc_num_tasks", plc_task_set.num_tasks_defined);

    for (int i = 0; i < plc_task_set.num_tasks_defined; i++) {

        UM_INFO(GBEM_UM_EN, "GBEM: Task [%s] has priority [%u] and a cycle time of [%ums] and is index [%u]",
                 plc_task_set.tasks[i].name, plc_task_set.tasks[i].priority, plc_task_set.tasks[i].cycle_time, i);

    }


    if (config_error) {
        *grc = E_INVALID_CONFIG;
    } else {
        *grc = E_SUCCESS;
    }

    json_dest = json_objClose(json_dest);
    return json_dest;
}



/**
 * @brief this creates the status and const json files and prints them to stdout
 *
 * This is a bit of a funny function because the information to properly create the status JSON is not present until the machine is up and running
 *
 * It is really just for testing
 */
void config_print_json_const_status(void){
    char const_json_temp[3000] = {0};
    char status_json_temp[3000] = {0};

    //create json
    status_data_to_json(status_json_temp, &ecm_status);

    const_data_to_json(const_json_temp, &ecm_status);

    UM_INFO(GBEM_UM_EN, "GBEM: ***                              STATUS JSON                                 ***");
    json_print(status_json_temp);
    UM_INFO(GBEM_UM_EN, "GBEM: ***                              CONST JSON                                  ***");
    json_print(const_json_temp);
    main_set_file_paths();
    write_status_json();
    write_const_json();
}


/**
 * @brief creates a json buffer of the GBEM config, checks the config and optionally outputs the checking process results and config to stdout
 * @param json_buffer
 * @param grc
 * @param print_output
 * @return
 */
int config_create_check_print(char *json_buffer, gberror_t *grc, bool print_output) {

    gberror_t local_grc = 0;
    bool map_error = false;
    char *json_dest = json_buffer;

    //if we have be asked to run without printing the output disable by setting the user message (um) global flag
    if (!print_output){
        um_disable_logging = true;
    }

    json_dest = json_objOpen(json_dest, NULL);

    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***            Check and print config for [%20s]             ***",
            map_machine_type_strings[map_machine_type]);
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");

    UM_INFO(GBEM_UM_EN, "GBEM: This process examines the >compiled< configuration checking for misconfiguration");
    UM_INFO(GBEM_UM_EN, "GBEM: It also outputs as summary of key configuration information");


    json_dest = config_process_general(json_dest, &local_grc);

    if (local_grc == E_SUCCESS) {
        UM_INFO(GBEM_UM_EN, "GBEM: RESULT: Step 1 >>success<< no errors were found in the general configuration");
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: RESULT: Step 1 >>failure<< errors were found in the general configuration");
        map_error = true;
    }

    json_dest = config_process_drives(json_dest, &local_grc);

    if (local_grc == E_SUCCESS) {
        UM_INFO(GBEM_UM_EN, "GBEM: RESULT: Step 2 >>success<< no errors were found in the drive configuration");
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: RESULT: Step 2 >>failure<< errors were found in the drive configuration");
        map_error = true;
    }

    json_dest = config_process_io(json_dest, &local_grc);

    if (local_grc == E_SUCCESS) {
        UM_INFO(GBEM_UM_EN, "GBEM: RESULT: Step 3 >>success<< no errors were found in the IO configuration");
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: RESULT: Step 3 >>failure<< errors were found in the IO configuration");
        map_error = true;
    }

    json_dest = config_process_slaves(json_dest, &local_grc);

    if (local_grc == E_SUCCESS) {
        UM_INFO(GBEM_UM_EN, "GBEM: RESULT: Step 4 >>success<< no errors were found in the slave configuration");
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: RESULT: Step 4 >>failure<< errors were found in the slave configuration");
        map_error = true;
    }

    json_dest = config_process_plc(json_dest, &local_grc);

    if (local_grc == E_SUCCESS) {
        UM_INFO(GBEM_UM_EN, "GBEM: RESULT: Step 5 >>success<< no errors were found in the PLC configuration");
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: RESULT: Step 5 >>failure<< errors were found in the PLC configuration");
        map_error = true;
    }

    if (!map_error) {
        UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
        UM_INFO(GBEM_UM_EN, "GBEM: ***   Overall >>success<< no errors were found in the machine configuration  ***");
        UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    } else {
        UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
        UM_INFO(GBEM_UM_EN, "GBEM: ***    Overall >>failure<< errors were found in the machine configuration    ***");
        UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    }


    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***                     Print config JSON file contents                      ***");
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");

    //this ouputs a summary of the config json so we can have a look see at it
    json_print(json_buffer);

    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***      Print SDOs used to re-map slaves and perform initial slave config   ***");
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");

    map_print_sdos();

    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");
    UM_INFO(GBEM_UM_EN, "GBEM: ***   Print const and status JSON (will be incomplete as EtherCAT isn't up)  ***");
    UM_INFO(GBEM_UM_EN, "GBEM: ********************************************************************************");

    init_ecm_status();
    config_print_json_const_status();



//set the return code
    if (map_error) {
        *grc = E_GENERAL_FAILURE;
    } else {
        *grc = E_SUCCESS;
    }

    json_dest = json_objClose(json_dest);

    json_dest = json_end(json_dest);

    //re-enabled um logging
    if (!print_output){
        um_disable_logging = false;
    }
    //return size of json created
    return (json_dest - json_buffer);

}
