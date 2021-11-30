
/**
 ******************************************************************************
 * @file           :  smc3.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

/* PDO MAPPING */
//SM2 - 0x1c12
//0x1602
#define SMC3_CONTROLWORD_PDO_INDEX           0
#define SMC3_SETPOS_PDO_INDEX                2
#define SMC3_MOOSET_PDO_INDEX                6



//SM3 - 0x1c13
//0x1a02
#define SMC3_STATUSWORD_PDO_INDEX            0
#define SMC3_ACTPOS_PDO_INDEX                2
#define SMC3_MOOGET_PDO_INDEX                6


//0x60c2 - interpolation



//0x2023:0 uint32 - Stall Guard Turns - value = 0x2e8 (1000)


//0x2022:0 uint32 - Stall Guard Offset - value = 0


//0x2000:0 uint32 - KvFactor - value = 0x1E (30)


//0x8010:7 uint32  - Motor steps per rev - value = 0xc8 (200)


//0x8010:8 uint32 - Encoder increment per rev - value = 0


//0x8010:10 uint32 - Steps - value = 0x100 (256)


//0x8010:11 uint32 - Desired current = 0xc8 (200) (2A)


//0x8010:12 uint32 - Value of current in standstill = 0x32 (50) (500mA)


