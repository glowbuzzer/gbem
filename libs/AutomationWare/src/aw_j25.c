/**
 ******************************************************************************
 * @file           :  aw_j25.c
 * @brief          :  AutomationWare J25 control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "aw_j25.h"
#include "aw_j_series.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


/**
 * @brief perform SDOs writes during boot to configure an AW-J17 drive
 * @param slave
 * @return gberror_t
 * @retval E_SUCCESS all ok
 */
gberror_t ec_apply_standard_sdos_aw_j25(const uint16_t slave) {

    UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Applying standard SDOs to slave [%u]", slave);

    //nolimits is a global variable set by running gbem with a command line option - x and is used when the drive is beyond the normal limits

    //Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
    //Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)

    if (nolimits) {
        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                                2147483647, true)) {
            return E_SDO_WRITE_FAILURE;
        }

        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                -2147483647, true)) {
            return E_SDO_WRITE_FAILURE;
        }
    } else {

        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MAX_POSITION_LIMIT_SDO_SUB_INDEX,
                                (int32_t) ((double) AW_J_SERIES_ENCODER_COUNTS_PER_REV *
                                           ((double) map_drive_pos_limit[map_slave_to_drive(slave)] / (double) 360)),
                                true)) {
            return E_SDO_WRITE_FAILURE;
        }
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Max position limit [%d] on drive [%d]",
                (int32_t) ((double) AW_J_SERIES_ENCODER_COUNTS_PER_REV *
                           ((double) map_drive_pos_limit[map_slave_to_drive(slave)] / (double) 360)),
                map_slave_to_drive(slave));

        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                                AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                (int32_t) ((double) AW_J_SERIES_ENCODER_COUNTS_PER_REV *
                                           ((double) map_drive_neg_limit[map_slave_to_drive(slave)] / (double) 360)),
                                true)) {
            return E_SDO_WRITE_FAILURE;
        }
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Min position limit [%d] on drive [%d]",
                (int32_t) ((double) AW_J_SERIES_ENCODER_COUNTS_PER_REV *
                           ((double) map_drive_neg_limit[map_slave_to_drive(slave)] / (double) 360)),
                map_slave_to_drive(slave));
    }
    //Polarity	0x607E:0	USINT	8

    uint8_t polarity = 0;
    if (map_drive_direction[map_slave_to_drive(slave)] == 0) {
        polarity = 128;
    }

    if (!ec_sdo_write_int32(slave, AW_J_SERIES_POLARITY_SDO_INDEX, AW_J_SERIES_POLARITY_SDO_SUB_INDEX,
                            polarity, true)) {
        return E_SDO_WRITE_FAILURE;
    }


    //Max torque	0x6072:0	UINT	16	0	32767	3000		readwrite

    //The value is given in per thousandth of rated torque.

    //Max torque = (Tmax / rated torque) * 1000

    //E.g. if the rated torque from the datasheet is 200 Nm and the maximum torque is 600 Nm, then the value is 3000.

    //Default value is 3000 (300% of rated torque).

    //J25

#define AW_J25_MAX_TORQUE 3991


    //use percentage of default max torque in array
    uint16_t torque_limit = map_drive_torque_limit[map_slave_to_drive(slave)];

    if (torque_limit > 0) {

        torque_limit = (uint16_t) ((((double) AW_J25_MAX_TORQUE * (double) torque_limit) / (double) 100));
        UM_INFO(GBEM_UM_EN, "GBEM: AW-J-Series - Max torque limit [%d] on drive [%d]",
                torque_limit,
                map_slave_to_drive(slave));
        if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_TORQUE_SDO_INDEX, AW_J_SERIES_MAX_TORQUE_SDO_SUB_INDEX,
                                torque_limit, true)) {
            return E_SDO_WRITE_FAILURE;
        }

    }

    //    Max motor speed	0x6080:0	UDINT	32	0		1000	rpm	readwrite
//BUT  AW-J-Series - SI unit velocity [0.001 * RPM] on drive [0]
#define AW_J25_MAX_SPEED 2500000

//for testing 5 degrees per second  so roughly 2500000 * 5/360 = 43722

    if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_MOTOR_SPEED_SDO_INDEX, AW_J_SERIES_MAX_MOTOR_SPEED_SDO_SUB_INDEX,
                            80000, true)) {

        return E_SDO_WRITE_FAILURE;

    }


    //set bus cycle time
    //Communication cycle period	0x1006:0	DINT	32			100		readwrite
    if (!ec_sdo_write_int32(slave, AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_INDEX,
                            AW_J_SERIES_COMMUNICATION_CYCLE_PERIOD_SDO_SUB_INDEX, MAP_CYCLE_TIME * 100, true)) {
        return E_SDO_WRITE_FAILURE;
    }

    return E_SUCCESS;
}





