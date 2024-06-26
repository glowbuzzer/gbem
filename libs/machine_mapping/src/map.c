/**
 ******************************************************************************
 * @file           : map.c
 * @brief          : Slave, drive, plc, gbc mapping
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


/* Things that follow the cia402 standard are handled in control.c things that dont have to be dealt with here */

#include "map.h"

#include "cia402.h"
#include "log.h"
#include <string.h>
#include "std_defs_and_macros.h"
#include "user_message.h"
#include "gbem_config.h"


//global var holding machine type (set in main.c)
map_machine_type_t map_machine_type = MAP_MACHINE_UNKNOWN;

map_machine_limits_t map_machine_limits[MAP_NUM_DRIVES];

uint8_t map_num_slaves = 0;


int8_t map_drive_moo[MAP_NUM_DRIVES];

machine_config_optional_slaves_t machine_config_optional_slaves = {0};

/**
 * \brief   get the size of the datatype in bits
 * \param datatype
 * \return
 */
uint32_t map_get_datatype_size_bits(ec_datatype datatype) {
    switch (datatype) {
        case ECT_BOOLEAN:
            return 1;
        case ECT_INTEGER8:
            return 8;
        case ECT_INTEGER16:
            return 16;
        case ECT_INTEGER24:
            return 24;
        case ECT_INTEGER32:
            return 32;
        case ECT_INTEGER64:
            return 64;
        case ECT_UNSIGNED8:
            return 8;
        case ECT_UNSIGNED16:
            return 16;
        case ECT_UNSIGNED24:
            return 24;
        case ECT_UNSIGNED32:
            return 32;
        case ECT_UNSIGNED64:
            return 64;
        case ECT_REAL32:
            return 32;
        case ECT_REAL64:
            return 64;
            //todo crit finish these sizes
        case ECT_VISIBLE_STRING:
            return 8;
        case ECT_OCTET_STRING:
            return 0;
        case ECT_UNICODE_STRING:
            return 0;
        case ECT_TIME_OF_DAY:
            return 0;
        case ECT_TIME_DIFFERENCE:
            return 0;
        case ECT_DOMAIN:
            return 0;
        case ECT_BIT1:
            return 1;
        case ECT_BIT2:
            return 2;
        case ECT_BIT3:
            return 3;
        case ECT_BIT4:
            return 4;
        case ECT_BIT5:
            return 5;
        case ECT_BIT6:
            return 6;
        case ECT_BIT7:
            return 7;
        case ECT_BIT8:
            return 8;
        case ECT_STRING8:
            return 64;
        default:
            LL_FATAL(
                    "GBEM: missing ec_datatype to size definition - this can occur when custom pdo map num entries don't match the num of elements in the array");
    }
    return 0;
}

enum FSOE_SLAVE_HIGH_LEVEL_STATE map_fsoe_command_to_state(uint8_t command) {
    switch (command) {
        case 0x00:
            return FSOE_SLAVE_HIGH_LEVEL_STATE_NONE;
        case 0x36:
            return FSOE_SLAVE_HIGH_LEVEL_STATE_PROCESS_DATA;
        case 0x2A:
            return FSOE_SLAVE_HIGH_LEVEL_STATE_RESET;
        case 0x4E:
            return FSOE_SLAVE_HIGH_LEVEL_STATE_SESSION;
        case 0x64:
            return FSOE_SLAVE_HIGH_LEVEL_STATE_CONNECTION;
        case 0x52:
            return FSOE_SLAVE_HIGH_LEVEL_STATE_PARAMETER;
        case 0x08:
            return FSOE_SLAVE_HIGH_LEVEL_STATE_FAILSAFEDATA;
        default:
            return FSOE_SLAVE_HIGH_LEVEL_STATE_UNKNOWN;
    }
}


uint32_t map_fsoe_master_get_overall_slot_size_out(void) {
    uint32_t size = 0;
    for (int slot = 0; slot < MAP_NUM_FSOE_MASTER_SLOTS; slot++) {
        switch (map_slave_fsoe_master_slot_config[slot]) {
            case MDP_SLOT_TYPE_NONE:
                UM_FATAL(
                        "GBEM: No slot type set for slot [%u] - check MAP_NUM_FSOE_MASTER_SLOTS matches the number of slots (FSoE slaves)",
                        slot);
                break;
            case MDP_SLOT_TYPE_BBH_32_12:
                size += 12;
                break;
            default:
                UM_FATAL("GBEM: Unknown slot type set for slot [%u]", slot);
        }
    }
    return size;
}

uint32_t map_fsoe_master_get_overall_slot_size_in(void) {
    uint32_t size = 0;
    for (int slot = 0; slot < MAP_NUM_FSOE_MASTER_SLOTS; slot++) {
        switch (map_slave_fsoe_master_slot_config[slot]) {
            case MDP_SLOT_TYPE_NONE:
                UM_FATAL(
                        "GBEM: No slot type set for slot [%u] - check MAP_NUM_FSOE_MASTER_SLOTS matches the number of slots (FSoE slaves)",
                        slot);
                break;
            case MDP_SLOT_TYPE_BBH_32_12:
                size += 32;
                break;
            default:
                UM_FATAL("GBEM: Unknown slot type set for slot [%u]", slot);
        }
    }
    return size;
}


uint32_t map_fsoe_get_slot_size_master_out(uint16_t slave) {
    uint32_t size = 0;
    for (int slot = 0; slot < MAP_NUM_FSOE_MASTER_SLOTS; slot++) {
        if (map_fsoe_master_slot_to_slave[slot] == slave) {
            switch (map_slave_fsoe_master_slot_config[slot]) {
                case MDP_SLOT_TYPE_NONE:
                    UM_FATAL(
                            "GBEM: No slot type set for slot [%u] - check MAP_NUM_FSOE_MASTER_SLOTS matches the number of slots (FSoE slaves)",
                            slot);
                    break;
                case MDP_SLOT_TYPE_BBH_32_12:
                    size = 12;
                    break;
                default:
                    UM_FATAL("GBEM: Unknown slot type set for slot [%u]", slot);
            }
        }
    }

    return size;
}

uint32_t map_fsoe_get_slot_size_master_in(uint16_t slave) {
    uint32_t size = 0;

    for (int slot = 0; slot < MAP_NUM_FSOE_MASTER_SLOTS; slot++) {
        if (map_fsoe_master_slot_to_slave[slot] == slave) {
            switch (map_slave_fsoe_master_slot_config[slot]) {
                case MDP_SLOT_TYPE_NONE:
                    UM_FATAL(
                            "GBEM: No slot type set for slot [%u] - check MAP_NUM_FSOE_MASTER_SLOTS matches the number of slots (FSoE slaves)",
                            slot);
                    break;
                case MDP_SLOT_TYPE_BBH_32_12:
                    size = 32;
                    break;
                default:
                    UM_FATAL("GBEM: Unknown slot type set for slot [%u]", slot);
            }
        }
    }

    return size;
}


uint32_t map_fsoe_get_slot_size_slave_out(uint16_t slave) {
    uint32_t size = 0;

    for (int slot = 0; slot < MAP_NUM_FSOE_MASTER_SLOTS; slot++) {
        if (map_fsoe_master_slot_to_slave[slot] == slave) {
            switch (map_slave_fsoe_master_slot_config[slot]) {
                case MDP_SLOT_TYPE_NONE:
                    UM_FATAL(
                            "GBEM: No slot type set for slot [%u] - check MAP_NUM_FSOE_MASTER_SLOTS matches the number of slots (FSoE slaves)",
                            slot);
                    break;
                case MDP_SLOT_TYPE_BBH_32_12:
                    size = 32;
                    break;
                default:
                    UM_FATAL("GBEM: Unknown slot type set for slot [%u]", slot);
            }
        }
    }

    return size;
}

uint32_t map_fsoe_get_slot_size_slave_in(uint16_t slave) {
    uint32_t size = 0;

    for (int slot = 0; slot < MAP_NUM_FSOE_MASTER_SLOTS; slot++) {
        if (map_fsoe_master_slot_to_slave[slot] == slave) {
            switch (map_slave_fsoe_master_slot_config[slot]) {
                case MDP_SLOT_TYPE_NONE:
                    UM_FATAL("GBEM: No slot type set for slot [%u]", slot);
                    break;
                case MDP_SLOT_TYPE_BBH_32_12:
                    size = 12;
                    break;
                default:
                    UM_FATAL("GBEM: Unknown slot type set for slot [%u]", slot);
            }
        }
    }

    return size;
}


uint32_t map_fsoe_get_slot_size_out(uint16_t slot) {
    uint32_t size = 0;

    switch (map_slave_fsoe_master_slot_config[slot]) {
        case MDP_SLOT_TYPE_NONE:
            UM_FATAL(
                    "GBEM: No slot type set for slot [%u] - check MAP_NUM_FSOE_MASTER_SLOTS matches the number of slots (FSoE slaves)",
                    slot);
            break;
        case MDP_SLOT_TYPE_BBH_32_12:
            size = 32;
            break;
        default:
            UM_FATAL("GBEM: Unknown slot type set for slot [%u]", slot);
    }


    return size;
}

uint32_t map_fsoe_get_slot_size_in(uint16_t slot) {
    uint32_t size = 0;
    switch (map_slave_fsoe_master_slot_config[slot]) {
        case MDP_SLOT_TYPE_NONE:
            UM_FATAL("GBEM: No slot type set for slot [%u]", slot);
            break;
        case MDP_SLOT_TYPE_BBH_32_12:
            size = 12;
            break;
        default:
            UM_FATAL("GBEM: Unknown slot type set for slot [%u]", slot);
    }


    return size;
}


/**
 * @brief This checks the warning bit on ALL drives
 * @param bool enable_check
 * @return true any one drive has the warning bit set, false warning bit not set on any drive
 * @attention based on cia402
 *
 */
bool ec_is_warning(bool enable_check) {
    if (!enable_check) {
        return false;
    }

    // Static array to store the results of the last N cycles
    static bool lastResults[NUM_CYCLES_TO_EVALUATE_FAULTS_OVER][MAP_NUM_DRIVES] = {false};

    bool warning_active[MAP_NUM_DRIVES];

    // Initializing all elements to true using a loop
    for (int i = 0; i < MAP_NUM_DRIVES; ++i) {
        warning_active[i] = true;
    }


    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        uint16_t drive_stat_wrd;
        if (map_drive_get_stat_wrd_function_ptr[i] != NULL) {
            drive_stat_wrd = map_drive_get_stat_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "Missing function pointer for map_drive_get_stat_wrd on drive [%u]", i);
            //we can't check all drives for a warning so return true that there is one - safest behaviour
            return true;
        }
        for (int j = 0; j < NUM_CYCLES_TO_EVALUATE_FAULTS_OVER - 1; ++j) {
            lastResults[j][i] = lastResults[j + 1][i];
        }

        if (BIT_CHECK(drive_stat_wrd, CIA_WARNING_BIT_NUM)) {
            lastResults[NUM_CYCLES_TO_EVALUATE_FAULTS_OVER - 1][i] = true;
        } else {
            lastResults[NUM_CYCLES_TO_EVALUATE_FAULTS_OVER - 1][i] = false;
        }
    }


    //evaluate
    for (int drive = 0; drive < MAP_NUM_DRIVES; drive++) {
        ecm_status.drives[drive].active_warning = true;
        for (int i = 0; i < NUM_CYCLES_TO_EVALUATE_FAULTS_OVER; ++i) {
            if (!lastResults[i][drive]) {
                warning_active[drive] = false;
                ecm_status.drives[drive].active_warning = false;
                break;
            }
        }
    }


    for (int i = 0; i < MAP_NUM_DRIVES; ++i) {
        if (warning_active[i]) {
            return true; // If any value is true, return true
        }
    }

    return false;
}

/**
 * @brief This checks the follow error bit on ALL drives
 * @param[out] gberror_t *grc error code
 * @return true any one drive has the follow error bit set, false follow error bit not set on any drive
 * @attention based on cia402
 *
 */
bool ec_check_for_follow_error(gberror_t *grc, bool enable_check) {
    if (!enable_check) {
        *grc = E_SUCCESS;
        return false;
    }
    // Static array to store the results of the last N cycles
    static bool lastResults[NUM_CYCLES_TO_EVALUATE_FAULTS_OVER][MAP_NUM_DRIVES] = {false};


    bool follow_error_active[MAP_NUM_DRIVES];

    // Initializing all elements to true using a loop
    for (int i = 0; i < MAP_NUM_DRIVES; ++i) {
        follow_error_active[i] = true;
    }


    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        bool follow_error = true;
        if (map_drive_get_follow_error_function_ptr[i] != NULL) {
            follow_error = map_drive_get_follow_error_function_ptr[i](i);
        } else {
            //we can't check all drives for a follow error so return true that there is one - safest behaviour
            *grc = E_NO_FUNCTION_FOUND;
            return true;
        }
        for (int j = 0; j < NUM_CYCLES_TO_EVALUATE_FAULTS_OVER - 1; ++j) {
            lastResults[j][i] = lastResults[j + 1][i];
        }
        lastResults[NUM_CYCLES_TO_EVALUATE_FAULTS_OVER - 1][i] = follow_error;
    }


    //evaluate
    for (int drive = 0; drive < MAP_NUM_DRIVES; drive++) {
        ecm_status.drives[drive].active_follow_error = true;
        for (int i = 0; i < NUM_CYCLES_TO_EVALUATE_FAULTS_OVER; ++i) {
            if (!lastResults[i][drive]) {
                follow_error_active[drive] = false;
                ecm_status.drives[drive].active_follow_error = false;
                break;
            }
        }
    }

    *grc = E_SUCCESS;

    for (int i = 0; i < MAP_NUM_DRIVES; ++i) {
        if (follow_error_active[i]) {
            return true; // If any value is true, return true
        }
    }

    return false;
}


/**
 * @brief This checks the internal limit bit on ALL drives
 * @param void
 * @return true any one drive has the internal limit bit set, false internal limit bit not set on any drive
 * @attention based on cia402
 * @warning if a new drive type is added to the frameworrk that doesnt support CIA 402 limits in the status word then this will need to be extended to be like remote
 *
 */
bool ec_check_for_internal_limit(gberror_t *grc, bool enable_check) {
    if (!enable_check) {
        *grc = E_SUCCESS;
        return false;
    }

    // Static array to store the results of the last N cycles
    static bool lastResults[NUM_CYCLES_TO_EVALUATE_FAULTS_OVER][MAP_NUM_DRIVES] = {false};

    bool internal_limit_active[MAP_NUM_DRIVES];

    // Initializing all elements to true using a loop
    for (int i = 0; i < MAP_NUM_DRIVES; ++i) {
        internal_limit_active[i] = true;
    }

    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        uint16_t drive_stat_wrd;
        if (map_drive_get_stat_wrd_function_ptr[i] != NULL) {
            drive_stat_wrd = map_drive_get_stat_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "Missing function pointer for map_drive_get_stat_wrd on drive [%u]", i);
            //we can't check all drives for an internal limit so return true that there is one - safest behaviour
            *grc = E_NO_FUNCTION_FOUND;
            return true;
        }
        for (int j = 0; j < NUM_CYCLES_TO_EVALUATE_FAULTS_OVER - 1; ++j) {
            lastResults[j][i] = lastResults[j + 1][i];
        }

        if (BIT_CHECK(drive_stat_wrd, CIA_INTERNAL_LIMIT_BIT_NUM)) {
            lastResults[NUM_CYCLES_TO_EVALUATE_FAULTS_OVER - 1][i] = true;
        } else {
            lastResults[NUM_CYCLES_TO_EVALUATE_FAULTS_OVER - 1][i] = false;
        }
    }

    //evaluate
    for (int drive = 0; drive < MAP_NUM_DRIVES; drive++) {
        ecm_status.drives[drive].active_internal_limit = true;
        for (int i = 0; i < NUM_CYCLES_TO_EVALUATE_FAULTS_OVER; ++i) {
            if (!lastResults[i][drive]) {
                internal_limit_active[drive] = false;
                ecm_status.drives[drive].active_internal_limit = false;
                break;
            }
        }
    }

    *grc = E_SUCCESS;

    for (int i = 0; i < MAP_NUM_DRIVES; ++i) {
        if (internal_limit_active[i]) {
            return true; // If any value is true, return true
        }
    }

    return false;
}


/**
 * @brief This checks the remote bit on ALL drives
 * @param void
 * @return true any one drive has the remote bit set, false remote bit not set on any drive
 * @attention based on cia402
 *
 */
bool ec_check_remote(bool enable_check) {
    if (!enable_check) {
        return false;
    }

    int j = 0;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        bool remote_ok = false;
        if (map_drive_get_remote_function_ptr[i] != NULL) {
            remote_ok = map_drive_get_remote_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "Missing function pointer for map_drive_get_remote on drive [%u]", i);
            return false;
        }

        if (remote_ok) {
            j++;
        }
    }
    if (j == MAP_NUM_DRIVES) {
        return true;
    } else {
        return false;
    }
}


/**
 * @brief returns a pointer to a row in the iomap based on gbc params
 * @param inout
 * @param ionum
 * @param datatype
 * @param row
 * @return
 */
gberror_t
map_get_gbc_iomap_row(const map_inout_t inout, const uint16_t ionum, const ec_datatype datatype, mapping_t **row) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].gbc.inout == inout) {
            if (map_iomap[i].gbc.ionum == ionum) {
                if (map_iomap[i].gbc.datatype == datatype) {
                    *row = &map_iomap[i];
                    return E_SUCCESS;
                }
            }
        }
    }
    return E_ENTRY_NOT_FOUND;
}

/**
 * @brief gets a slave number from the iomap based on gbc io_num, gbc datatype and IN / OUT
 * @param inout
 * @param ionum
 * @param datatype
 * @return
 */
uint16_t map_get_gbc_iomap_slave(const map_inout_t inout, const uint16_t ionum, const ec_datatype datatype) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].gbc.inout == inout) {
            if (map_iomap[i].gbc.ionum == ionum) {
                if (map_iomap[i].gbc.datatype == datatype) {
                    return map_iomap[i].pdo.slave_num;
                }
            }
        }
    }
    LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Matching iomap entry not found");
    return 0;
}

/**
 * @brief gets a iomap bit byte number from the iomap based on gbc io_num, gbc datatype and IN / OUT
 * @param inout
 * @param ionum
 * @param datatype
 * @return
 */
uint32_t map_get_gbc_iomap_byte_num(const map_inout_t inout, const uint16_t ionum, const ec_datatype datatype) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].gbc.inout == inout) {
            if (map_iomap[i].gbc.ionum == ionum) {
                if (map_iomap[i].gbc.datatype == datatype) {
                    return map_iomap[i].pdo.byte_num;
                }
            }
        }
    }
    LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Matching iomap entry not found");
    return 0;
}


uint8_t map_get_gbc_iomap_bit_num(const map_inout_t inout, const uint16_t ionum, const ec_datatype datatype) {
    for (int i = 0; i < map_num_rows_in_iomap; i++) {
        if (map_iomap[i].gbc.inout == inout) {
            if (map_iomap[i].gbc.ionum == ionum) {
                if (map_iomap[i].gbc.datatype == datatype) {
                    return map_iomap[i].pdo.bit_num;
                }
            }
        }
    }
    LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Matching iomap entry not found");
    return 0;
}


/**
 * @brief checks if a ec_datatype is supported in the DPM structure
 * @param type
 * @return
 */
bool map_is_valid_gbc_datatype(const ec_datatype type) {
    switch (type) {
        case ECT_BOOLEAN:
            return true;
        case ECT_INTEGER32:
            return true;
        case ECT_UNSIGNED32:
            return true;
        case ECT_REAL32:
            return true;
        default:
            return false;
    }
}

/**
 * @brief Checks if a ec_datatype is supported in the PDO read/write functions
 * @param type
 * @return
 */
bool map_is_valid_pdo_datatype(const ec_datatype type) {
    switch (type) {
        case ECT_BOOLEAN:
            return true;
        case ECT_INTEGER32:
            return true;
        case ECT_UNSIGNED32:
            return true;
        case ECT_INTEGER16:
            return true;
        case ECT_UNSIGNED16:
            return true;
        case ECT_INTEGER8:
            return true;
        case ECT_UNSIGNED8:
            return true;
        case ECT_REAL32:
            return true;
        default:
            return false;
    }
}

uint16_t map_slave_to_drive(uint16_t slave) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (map_drive_to_slave[i] == slave) {
            return i;
        }
    }
    LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Matching drive not found");
    return 0;
}


/**
 * @brief Checks if a ec_datatype is supported in the PLC read/write functions
 * @param type
 * @return
 */
bool map_is_valid_plc_datatype(const ec_datatype type) {
    switch (type) {
        case ECT_BOOLEAN:
            return true;
        case ECT_INTEGER32:
            return true;
        case ECT_UNSIGNED32:
            return true;
        case ECT_INTEGER16:
            return true;
        case ECT_UNSIGNED16:
            return true;
        case ECT_INTEGER8:
            return true;
        case ECT_UNSIGNED8:
            return true;
        case ECT_REAL32:
            return true;
        default:
            return false;
    }
}

