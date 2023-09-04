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


//global var holding machine type (set in main.c)
map_machine_type_t map_machine_type = MAP_MACHINE_UNKNOWN;


/**
 * @brief This checks the warning bit on ALL drives
 * @param void
 * @return true any one drive has the warning bit set, false warning bit not set on any drive
 * @attention based on cia402
 *
 */
bool ec_is_warning(void) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        uint16_t drive_stat_wrd;
        if (*map_drive_get_stat_wrd_function_ptr[i] != NULL) {
            drive_stat_wrd = map_drive_get_stat_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "Missing function pointer for map_drive_get_stat_wrd on drive [%u]", i);
            //we can't check all drives for a warning so return true that there is one - safest behaviour
            return true;
        }
        if (BIT_CHECK(drive_stat_wrd, CIA_WARNING_BIT_NUM)) {
            return true;
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
bool ec_check_for_follow_error(gberror_t *grc) {
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (*map_drive_get_follow_error_function_ptr[i] != NULL) {
            if (map_drive_get_follow_error_function_ptr[i](i)) {
                LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                         "Missing function pointer for map_drive_get_follow_error on drive [%u]", i);
                *grc = E_SUCCESS;
                return true;
            }
        } else {

            //we can't check all drives for a follow error so return true that there is one - safest behaviour
            *grc = E_NO_FUNCTION_FOUND;
            return true;
        }
    }
    *grc = E_SUCCESS;
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
bool ec_check_for_internal_limit(gberror_t *gbc) {


    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        uint16_t drive_stat_wrd;
        if (*map_drive_get_stat_wrd_function_ptr[i] != NULL) {
            drive_stat_wrd = map_drive_get_stat_wrd_function_ptr[i](i);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN, "Missing function pointer for map_drive_get_stat_wrd on drive [%u]", i);
            //we can't check all drives for an internal limit so return true that there is one - safest behaviour
            *gbc = E_NO_FUNCTION_FOUND;
            return true;
        }
        if (BIT_CHECK(drive_stat_wrd, CIA_INTERNAL_LIMIT_BIT_NUM)) {
            *gbc = E_SUCCESS;
            return true;
        }
    }
    *gbc = E_SUCCESS;
    return false;
}


/**
 * @brief This checks the remote bit on ALL drives
 * @param void
 * @return true any one drive has the remote bit set, false remote bit not set on any drive
 * @attention based on cia402
 *
 */
bool ec_check_remote(void) {

    int j = 0;
    for (int i = 0; i < MAP_NUM_DRIVES; i++) {

        bool remote_ok = false;
        if (*map_drive_get_remote_function_ptr[i] != NULL) {
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
map_get_gbc_iomap_row(const map_inout_t inout, const uint16_t ionum, const ec_datatype_t datatype, mapping_t **row) {
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
uint16_t map_get_gbc_iomap_slave(const map_inout_t inout, const uint16_t ionum, const ec_datatype_t datatype) {
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
uint32_t map_get_gbc_iomap_byte_num(const map_inout_t inout, const uint16_t ionum, const ec_datatype_t datatype) {
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


uint8_t map_get_gbc_iomap_bit_num(const map_inout_t inout, const uint16_t ionum, const ec_datatype_t datatype) {
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
bool map_is_valid_gbc_datatype(const ec_datatype_t type) {

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
bool map_is_valid_pdo_datatype(const ec_datatype_t type) {

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
bool map_is_valid_plc_datatype(const ec_datatype_t type) {
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

