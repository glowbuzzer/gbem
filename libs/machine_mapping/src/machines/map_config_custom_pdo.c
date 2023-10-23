/**
 ******************************************************************************
 * @file           :  map_config_custom_pdo
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "ethercattype.h"

typedef enum {
    RXPDO = 0,
    TXPDO = 1
} map_custom_pdo_rxtx;

typedef struct {
    map_custom_pdo_rxtx rxtx;
    ec_datatype datatype;
    uint16_t index;
    uint8_t subindex;
} map_custom_pdo_t;


map_custom_pdo_t aw_custom[10] = {
        {.rxtx=RXPDO, .datatype=ECT_INTEGER32}
};



//PDO mapping
//RxPDO – device receives this process data
//0x1600:00
//10 slots
//0x1601:00
//10 slots
//0x1602:00
//10 slots
//        TxPDO – device sends this process data
//0x1A00:00
//10 slots
//0x1a01
//10 slots
//0x1A02:00
//10 slots

