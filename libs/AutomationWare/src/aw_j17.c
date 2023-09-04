/**
 ******************************************************************************
 * @file           :  aw-j17.c
 * @brief          :  AutomationWare J17 control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "aw_j17.h"
#include "aw_j_series.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


/**
 * @brief perform SDOs writes during boot to configure an AW-J17 drive
 * @param slave
 * @return gberror_t
 * @retval E_SUCCESS all ok
 */
gberror_t ec_apply_standard_sdos_aw_j17(const uint16_t slave) {

    //Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
    //Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)


    if (nolimits) {
        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                                2147483647)) {
            return E_SDO_WRITE_FAILURE;
        }

        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                -2147483647)) {
            return E_SDO_WRITE_FAILURE;
        }
    } else {
        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                                map_drive_pos_limit[map_slave_to_drive(slave)])) {
            return E_SDO_WRITE_FAILURE;
        }

        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                map_drive_neg_limit[map_slave_to_drive(slave)])) {
            return E_SDO_WRITE_FAILURE;
        }
    }
    //Polarity	0x607E:0	USINT	8

    uint8_t polarity = 0;
    if (map_drive_direction[map_slave_to_drive(slave)] == 0) {
        polarity = 128;
    }

    if (!ec_sdo_write_int32(slave, AW_J_SERIES_POLARITY_SDO_INDEX, AW_J_SERIES_POLARITY_SDO_SUB_INDEX,
                            polarity)) {
        return E_SDO_WRITE_FAILURE;
    }


    //Max torque	0x6072:0	UINT	16	0	32767	3000		readwrite

    //The value is given in per thousandth of rated torque.

    //Max torque = (Tmax / rated torque) * 1000

    //E.g. if the rated torque from the datasheet is 200 Nm and the maximum torque is 600 Nm, then the value is 3000.

    //Default value is 3000 (300% of rated torque).

    //J17
    //    default max torque: 4016
    //    default motor rated torque: 620

//use percentage of default max torque in array
    uint16_t torque_limit = map_drive_torque_limit[map_slave_to_drive(slave)];

    if (torque_limit > 0) {

        torque_limit = (uint16_t) (((4016 * torque_limit) / 100));
        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_TORQUE_SDO_INDEX, AW_J_SERIES_MAX_TORQUE_SDO_SUB_INDEX,
                                torque_limit)) {
            return E_SDO_WRITE_FAILURE;
        }


    }




    //set bus cycle time
    //Communication cycle period	0x1006:0	DINT	32			100		readwrite
    if (!ec_sdo_write_int32(slave, AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_INDEX,
                            AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_SUB_INDEX, MAP_CYCLE_TIME * 100)) {
        return E_SDO_WRITE_FAILURE;
    }


    //todo
//set moo
//    0x6060:0
    if (!ec_sdo_write_int32(slave, 0x6060, 0, 8)) {
        return E_SDO_WRITE_FAILURE;
    }


    return E_SUCCESS;
}





