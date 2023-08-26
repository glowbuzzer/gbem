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

    for (int i = 0; i < MAP_NUM_DRIVES; i++) {
        if (map_drive_to_slave[i] == slave) {

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
                                        map_drive_pos_limit[i])) {
                    return E_SDO_WRITE_FAILURE;
                }

                if (!ec_sdo_write_int32(slave, AW_J_SERIES_MIN_POSITION_LIMIT_SDO_INDEX,
                                        AW_J_SERIES_MIN_POSITION_LIMIT_SDO_SUB_INDEX,
                                        map_drive_neg_limit[i])) {
                    return E_SDO_WRITE_FAILURE;
                }
            }
            //Polarity	0x607E:0	USINT	8

            uint8_t polarity = 0;
            if (map_drive_direction[i] == 0) {
                polarity = 128;
            }

            if (!ec_sdo_write_int32(slave, AW_J_SERIES_POLARITY_SDO_INDEX, AW_J_SERIES_POLARITY_SDO_SUB_INDEX,
                                    polarity)) {
                return E_SDO_WRITE_FAILURE;
            }
        }
    }
//    ec_print_key_sdos_aw_j17(slave);
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



//Max torque	0x6072:0	UINT	16	0	32767	3000		readwrite

//Defines the absolute position limits for the 0x607A Target position and the 0x6064 Position actual value. Every new target and actual position are checked against these limits. The limit positions shall be always relative to the machine home position. Limits are disabled by setting them to default values, min. limit to -2147483648 and max. limit to 2147483647. The units are user-defined (increments of the position encoder by default; see 0x6092 Feed constant to change it).

//Min position limit	0x607D:1	DINT	32			2147483648	Inc	readwrite	Receive PDO (Outputs)
//Max position limit	0x607D:2	DINT	32			2147483647	Inc	readwrite	Receive PDO (Outputs)


    return E_SUCCESS;
}





