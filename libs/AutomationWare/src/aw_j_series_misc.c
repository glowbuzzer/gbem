/**
 ******************************************************************************
 * @file           :  aw
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "aw_j_series_misc.h"
#include "ethercatsetget.h"
#include "aw_j_series.h"

/**
 * @brief print key default SDOs for AW-J-Series drive
 * @param slave
 */



gberror_t ec_print_params_aw_j_series(const uint16_t drive) {

    int32_t max_pos = 0;
    ec_sdo_read_int32(map_drive_to_slave[drive], AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX,
                      AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                      &max_pos);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Max. pos [%d] on drive [%d]", max_pos, drive);

    int32_t min_pos = 0;
    ec_sdo_read_int32(map_drive_to_slave[drive], AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                      AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                      &min_pos);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Min. pos  [%d] on drive [%d]", min_pos, drive);

    uint16_t max_torque = 0;
    ec_sdo_read_uint16(map_drive_to_slave[drive], AW_J_SERIES_MAX_TORQUE_SDO_INDEX,
                       AW_J_SERIES_MAX_TORQUE_SDO_SUB_INDEX, &max_torque);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Max torque [%d] on drive [%d]", max_torque, drive);

    uint32_t motor_rated_torque = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_INDEX,
                       AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_SUB_INDEX,
                       &motor_rated_torque);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Motor rated torque  [%d] on drive [%d]", motor_rated_torque,
            drive);

    uint32_t motor_revolutions = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_INDEX,
                       AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_SUB_INDEX,
                       &motor_revolutions);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Motor revolutions [%d] on drive [%d]", motor_revolutions, drive);

    uint32_t shaft_revolutions = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_INDEX,
                       AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_SUB_INDEX,
                       &shaft_revolutions);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Shaft revolutions [%d] on drive [%d]", shaft_revolutions, drive);

    int os = 50;
    uint8_t octet_string[50];

    int rc = ec_SDOread(map_drive_to_slave[drive], AW_J_SERIES_MANUFACTURER_SOFTWARE_VERSION_SDO_INDEX,
                        AW_J_SERIES_MANUFACTURER_SOFTWARE_VERSION_SDO_SUB_INDEX, false, &os,
                        &octet_string, EC_TIMEOUTRXM);
    if (rc <= 0) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Could not read SDO index:0x%04x - sub-index:0x%04x (on slave:%u)",
                 AW_J_SERIES_MANUFACTURER_SOFTWARE_VERSION_SDO_INDEX,
                 AW_J_SERIES_MANUFACTURER_SOFTWARE_VERSION_SDO_SUB_INDEX,
                 map_drive_to_slave[drive]);
        return E_SDO_READ_FAILURE;
    }
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Manufacturer software version [%s] on drive [%d]", octet_string,
            drive);


    uint16_t drive_error_code = 0;
    ec_sdo_read_uint16(map_drive_to_slave[drive], AW_J_SERIES_ERROR_CODE_SDO_INDEX,
                       AW_J_SERIES_ERROR_CODE_SDO_SUB_INDEX,
                       &drive_error_code);

    UM_INFO(GBEM_UM_EN, "GBEM:  AW-J-Series - drive error code [%d]", drive_error_code);

//default max pos: 2147483647
//    default min pos: -2147483648
//    default max torque: 4016
//    default motor rated torque: 620
//    default motor revolutions: 51
//    default shaft revolutions: 1

// 20 bit encoder =
//    DEG_TO_INC: DINT := 2912

    return E_SUCCESS;
}


