
/**
 ******************************************************************************
 * @file           :  EL7041.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "EL7041.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"

//SM2
//0x1602, stepper motor control (STM)


//0x1605, pos control compact


//SM3
//0x1a03 stepper motor status (STM)


//reducedCurrent 0x8010, 0x02
//motorCoilResistance 0x8010, 0x04
//reversed 0x8012, 0x09

//set pos control mode -

//ecrt_slave_config_sdo8(dev->slaveConfig, 0x8012, 0x01, 3);


//- before enabling, if the error status bit (PDO Item #9 {0x6010, 0x04, 1}) is set
//set the faultReset control bit (PDO Item #1 {0x7010, 0x02, 1}) for around 100ms
//
//- ensure the readyToEnable status bit (PDO Item #6 {0x6010, 0x01, 1}) is on
//if there is no error set the enable control bit (PDO Item #0 {0x7010, 0x01, 1})
//wait until the ready status bit (PDO Item #7 {0x6010, 0x02, 1}) is on
//
//
//Once enabled you need to update the target position (PDO Item #5 {0x7010, 0x11, 32})


//Lastly, if you get the warning or error status bits set you can look up what the error or warning is via the 0xA010 SDO diagnostic bits (0xA010:0x01 - 0xA010:0x11). Note: these are only available via SDO access, so what I do is have a state machine that looks them up if the warning or error status bits are set.



//0x8010:01 Max current - value 0x1388 = 5000 = 5A

//0x8010:03 "Nominal voltage - value 0xc350 = 50000 = 50v

//Microstepping is set to 1/64 and cannot be changed.

//0x8012:05 - speed range = default 2000 (1)

//0x8012:08 feeback type - internal counter / encoderr

//0x8011:07 "Ka factor (curr.)"

/*
    Ready to enable
    Enabled
    Warn
    Error

                                RTE     Enabled     Error
    Not ready to switch on      0       0           x
    Switch on disabled          0       0           x
    Ready to switch on          1       0           x
    Switched on                 1       1           x
    Operation enabled
    Quick stop active
    Fault reaction active       x       x           1
    Fault


 */

/*
 * Enable
 * Reset
 * emergency stop
 *
 *                              En      Re      ES
 * Switch on                    1       x       x
 * Enable volatge               1       x       x
 * Quick stop                   x       x       1
 * Enable op                    1       x       x
 * Fault reset                  x       1       x
 *
 *
 *
 *
 */



int32_t ec_get_actpos_wrd_el7041(const uint16_t drive) {
//    return ec_pdo_get_input_int32(map_drive_to_slave[drive], JVL_MIS_ACTPOS_PDO_INDEX);

    return E_SUCCESS;
}

gberror_t ec_set_ctrl_wrd_el7041(const uint16_t drive, const uint16_t ctrlwrd) {

//    switch (ctrlwrd){
//        case 1 :
//            ec_pdo_set_output_
//
//    }
//
//    ec_pdo_set_output_uint16(map_drive_to_slave[drive], JVL_MIS_CONTROLWORD_PDO_INDEX, ctrlwrd);
    return E_SUCCESS;
}

uint16_t ec_get_stat_wrd_el7041(const uint16_t drive) {
//    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], JVL_MIS_STATUSWORD_PDO_INDEX);
    return E_SUCCESS;
}

gberror_t ec_set_setpos_wrd_el7041(const uint16_t drive, const int32_t setpos) {
//    ec_pdo_set_output_int32(map_drive_to_slave[drive], JVL_MIS_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}
