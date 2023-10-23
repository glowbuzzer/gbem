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

    gberror_t grc = ec_apply_standard_sdos_aw_j_series(slave);

    if (grc != E_SUCCESS) {
        return grc;
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


    return E_SUCCESS;
}





