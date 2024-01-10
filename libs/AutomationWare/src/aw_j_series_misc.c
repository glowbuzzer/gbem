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
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Printing Key Parameter SDOs for drive [%d]", drive);

    int rc = 0;

    int assigned_name_os = 50;
    uint8_t assigned_name[50];

    rc = ec_SDOread(map_drive_to_slave[drive], AW_J_SERIES_ASSIGNED_NAME_SDO_INDEX,
                    AW_J_SERIES_ASSIGNED_NAME_SDO_SUB_INDEX, false, &assigned_name_os,
                    &assigned_name, EC_TIMEOUTRXM);
    if (rc <= 0) {
        LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Could not read SDO index:0x%04x - sub-index:0x%04x (on slave:%u)",
                 AW_J_SERIES_ASSIGNED_NAME_SDO_INDEX,
                 AW_J_SERIES_ASSIGNED_NAME_SDO_SUB_INDEX,
                 map_drive_to_slave[drive]);
        return E_SDO_READ_FAILURE;
    }
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Assigned name [%s] on drive [%d]", assigned_name, drive);


    uint32_t enc1_res = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_RESOLUTION_ENC1_SDO_INDEX,
                       AW_J_SERIES_RESOLUTION_ENC1_SDO_SUB_INDEX,
                       &enc1_res, true);

    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Encoder 1 resolution [%d] on drive [%d]", enc1_res, drive);


    uint32_t enc2_res = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_RESOLUTION_ENC2_SDO_INDEX,
                       AW_J_SERIES_RESOLUTION_ENC2_SDO_SUB_INDEX,
                       &enc2_res, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Encoder 2 resolution [%d] on drive [%d]", enc2_res, drive);


    //SI unit velocity	0x60A9:0	UDINT	32
    uint32_t si_unit_velocity = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_SI_UNIT_VELOCITY_SDO_INDEX,
                       AW_J_SERIES_SI_UNIT_VELOCITY_SDO_SUB_INDEX,
                       &si_unit_velocity, true);

    if (si_unit_velocity == 11814656) {
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity [RPM] on drive [%d]", drive);
    } else if (si_unit_velocity == 4290004736) {
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity [0.1 * RPM] on drive [%d]", drive);
    } else if (si_unit_velocity == 4273227520) {
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity [0.01 * RPM] on drive [%d]", drive);
    } else if (si_unit_velocity == 4256450304) {
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity [0.001 * RPM] on drive [%d]", drive);
    } else if (si_unit_velocity == 11797248) {
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity [RPS] on drive [%d]", drive);
    } else if (si_unit_velocity == 4289987328) {
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity [0.1 * RPS] on drive [%d]", drive);
    } else if (si_unit_velocity == 4273210112) {
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity [0.01 * RPS] on drive [%d]", drive);
    } else if (si_unit_velocity == 4256432896) {
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity [0.001 * RPS] on drive [%d]", drive);
    } else {
        UM_ERROR(GBEM_UM_EN, "GBEM: AW-J-Series - SI unit velocity not recognised on drive [%d]", drive);
    }


    //    Max motor speed	0x6080:0	UDINT	32	0		1000	rpm	readwrite
    //Think need to divide by 51 to get shaft speed
    uint32_t max_motor_speed = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_MAX_MOTOR_SPEED_SDO_INDEX,
                       AW_J_SERIES_MAX_MOTOR_SPEED_SDO_SUB_INDEX,
                       &max_motor_speed, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Max motor speed [%d] on drive [%d]", max_motor_speed, drive);


    //not sure why this does not exist in the object dictionary
    //    uint32_t feed_constant = 0;
    //    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_FEED_CONSTANT_SDO_INDEX,
    //                       AW_J_SERIES_FEED_CONSTANT_SDO_SUB_INDEX,
    //                       &feed_constant, true);
    //
    //    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Feed constant [%d] on drive [%d]", feed_constant, drive);


    int32_t max_pos = 0;
    ec_sdo_read_int32(map_drive_to_slave[drive], AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX,
                      AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                      &max_pos, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Max. pos [%d] on drive [%d]", max_pos, drive);

    int32_t min_pos = 0;
    ec_sdo_read_int32(map_drive_to_slave[drive], AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                      AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                      &min_pos, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Min. pos  [%d] on drive [%d]", min_pos, drive);

    uint16_t max_torque = 0;
    ec_sdo_read_uint16(map_drive_to_slave[drive], AW_J_SERIES_MAX_TORQUE_SDO_INDEX,
                       AW_J_SERIES_MAX_TORQUE_SDO_SUB_INDEX, &max_torque, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Max torque [%d] on drive [%d]", max_torque, drive);

    uint32_t motor_rated_torque = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_INDEX,
                       AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_SUB_INDEX,
                       &motor_rated_torque, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Motor rated torque  [%d] on drive [%d]", motor_rated_torque,
            drive);

    uint32_t motor_revolutions = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_INDEX,
                       AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_SUB_INDEX,
                       &motor_revolutions, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Motor revolutions [%d] on drive [%d]", motor_revolutions, drive);

    uint32_t shaft_revolutions = 0;
    ec_sdo_read_uint32(map_drive_to_slave[drive], AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_INDEX,
                       AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_SUB_INDEX,
                       &shaft_revolutions, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Shaft revolutions [%d] on drive [%d]", shaft_revolutions, drive);

    int os = 50;
    uint8_t octet_string[50];

    rc = ec_SDOread(map_drive_to_slave[drive], AW_J_SERIES_MANUFACTURER_SOFTWARE_VERSION_SDO_INDEX,
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
                       &drive_error_code, true);

    UM_INFO(GBEM_UM_EN, "GBEM:  AW-J-Series - drive error code [%d]", drive_error_code);


    int32_t torque_constant = 0;
    ec_sdo_read_int32(map_drive_to_slave[drive], AW_J_SERIES_TORQUE_CONSTANT_SDO_INDEX,
                      AW_J_SERIES_TORQUE_CONSTANT_SDO_SUB_INDEX,
                      &torque_constant, true);
    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Torque constant [%d] on drive [%d]", torque_constant, drive);


    //default max pos: 2147483647
    //    default min pos: -2147483648
    //    default max torque: 4016
    //    default motor rated torque: 620
    //    default motor revolutions: 51
    //    default shaft revolutions: 1

    // 20 bit encoder =
    //    DEG_TO_INC: DINT := 2912


    //aw-j17
    //    [INFO     ] GBEM: AW-J-Series - SI unit velocity [0.001 * RPM] on drive [0]
    //    [INFO     ] GBEM: AW-J-Series - Max motor speed [3000000] on drive [0]
    //    [INFO     ] GBEM: AW-J-Series - Max. pos [99999999] on drive [0]
    //    [INFO     ] GBEM: AW-J-Series - Min. pos  [-99999999] on drive [0]
    //    [INFO     ] GBEM: AW-J-Series - Max torque [240] on drive [0]
    //    [INFO     ] GBEM: AW-J-Series - Motor rated torque  [620] on drive [0]
    //    [INFO     ] GBEM: AW-J-Series - Motor revolutions [51] on drive [0]
    //    [INFO     ] GBEM: AW-J-Series - Shaft revolutions [1] on drive [0]
    //    [INFO     ] GBEM: AW-J-Series - Manufacturer software version [v5.0.9] on drive [0]


    return E_SUCCESS;
}


