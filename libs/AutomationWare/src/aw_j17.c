/**
 ******************************************************************************
 * @file           :  aw_j17.c
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
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


typedef struct {
    ec_datatype datatype;
    uint16_t index;
    uint8_t subindex;
    ec_datatype_value_t value;
} standard_sdo_t;


/**
 * @brief perform SDOs writes during boot to configure an AW-J17 drive
 * @param slave
 * @return gberror_t
 * @retval E_SUCCESS all ok
 */
gberror_t ec_apply_standard_sdos_aw_j17(const uint16_t slave) {


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

    //J17
    //    default max torque: 4016
    //    default motor rated torque: 620

    //use percentage of default max torque in array
    uint16_t torque_limit = map_drive_torque_limit[map_slave_to_drive(slave)];
#define AW_J17_MAX_TORQUE 4016


    if (torque_limit > 0) {

        torque_limit = (uint16_t) ((((double) AW_J17_MAX_TORQUE * (double) torque_limit) / (double) 100));
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

//by default max motor speed = 3000000 = 3000 rpm = 50 rps but this is before the 51 ratio gear box so is 0.98 rps
//not sure gearbox plays a roll here as it is scaled by feed constant
//for testing 5 degrees per second  so roughly 3000000 * 5/360 = 41666
//for testing 10 degrees per second  so roughly 3000000 * 10/360 = 83332

//for testing 20 degrees per second  so roughly 3000000 * 20/360 = 166664



    if (!ec_sdo_write_int32(slave, AW_J_SERIES_MAX_MOTOR_SPEED_SDO_INDEX, AW_J_SERIES_MAX_MOTOR_SPEED_SDO_SUB_INDEX,
                            300000, true)) {

        return E_SDO_WRITE_FAILURE;

    }


    return E_SUCCESS;
}





