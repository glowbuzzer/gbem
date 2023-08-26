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
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"

#include "aw_j_series.h"

/**
 * @brief print key default SDOs for AW-J-Series drive
 * @param slave
 */

gberror_t ec_print_key_sdos_aw_j_series(const uint16_t slave) {

    int32_t max_pos = 0;
    ec_sdo_read_int32(slave, AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX, AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                      &max_pos);
    printf("default max pos: %d\n", max_pos);

    int32_t min_pos = 0;
    ec_sdo_read_int32(slave, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                      &min_pos);
    printf("default min pos: %d\n", min_pos);

    uint16_t max_torque = 0;
    ec_sdo_read_uint16(slave, AW_J_SERIES_MAX_TORQUE_SDO_INDEX, AW_J_SERIES_MAX_TORQUE_SDO_SUB_INDEX, &max_torque);
    printf("default max torque: %d\n", max_torque);

    uint32_t motor_rated_torque = 0;
    ec_sdo_read_uint32(slave, AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_INDEX, AW_J_SERIES_MOTOR_RATED_TORQUE_SDO_SUB_INDEX,
                       &motor_rated_torque);
    printf("default motor rated torque: %d\n", motor_rated_torque);

    uint32_t motor_revolutions = 0;
    ec_sdo_read_uint32(slave, AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_INDEX, AW_J_SERIES_MOTOR_REVOLUTIONS_SDO_SUB_INDEX,
                       &motor_revolutions);
    printf("default motor revolutions: %d\n", motor_revolutions);

    uint32_t shaft_revolutions = 0;
    ec_sdo_read_uint32(slave, AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_INDEX, AW_J_SERIES_SHAFT_REVOLUTIONS_SDO_SUB_INDEX,
                       &shaft_revolutions);
    printf("default shaft revolutions: %d\n", shaft_revolutions);



//    default max pos: 2147483647
//    default min pos: -2147483648
//    default max torque: 4016
//    default motor rated torque: 620
//    default motor revolutions: 51
//    default shaft revolutions: 1

    return E_SUCCESS;
}


