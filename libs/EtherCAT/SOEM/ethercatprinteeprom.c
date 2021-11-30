
/**
 ******************************************************************************
 * @file           :  ethercatprinteeprom.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ethercat.h"

#define MINBUF 128
#define MAXBUF 524288

uint8 ebuf[MAXBUF];


void calc_crc(uint8 *crc, uint8 b)
{
    int j;
    *crc ^= b;
    for(j = 0; j <= 7 ; j++ )
    {
        if(*crc & 0x80)
            *crc = (*crc << 1) ^ 0x07;
        else
            *crc = (*crc << 1);
    }
}

uint16 SIIcrc(uint8 *buf)
{
    int i;
    uint8 crc;

    crc = 0xff;
    for( i = 0 ; i <= 13 ; i++ )
    {
        calc_crc(&crc , *(buf++));
    }
    return (uint16)crc;
}


int eeprom_read(int slave, int start, int length)
{
    int i, ainc = 4;
    uint16 estat, aiadr;
    uint32 b4;
    uint64 b8;
    uint8 eepctl;

    if((ec_slavecount >= slave) && (slave > 0) && ((start + length) <= MAXBUF))
    {
        aiadr = 1 - slave;
        eepctl = 2;
        ec_APWR(aiadr, ECT_REG_EEPCFG, sizeof(eepctl), &eepctl , EC_TIMEOUTRET); /* force Eeprom from PDI */
        eepctl = 0;
        ec_APWR(aiadr, ECT_REG_EEPCFG, sizeof(eepctl), &eepctl , EC_TIMEOUTRET); /* set Eeprom to master */

        estat = 0x0000;
        aiadr = 1 - slave;
        ec_APRD(aiadr, ECT_REG_EEPSTAT, sizeof(estat), &estat, EC_TIMEOUTRET); /* read eeprom status */
        estat = etohs(estat);
        if (estat & EC_ESTAT_R64)
        {
            ainc = 8;
            for (i = start ; i < (start + length) ; i+=ainc)
            {
                b8 = ec_readeepromAP(aiadr, i >> 1 , EC_TIMEOUTEEP);
                ebuf[i] = b8 & 0xFF;
                ebuf[i+1] = (b8 >> 8) & 0xFF;
                ebuf[i+2] = (b8 >> 16) & 0xFF;
                ebuf[i+3] = (b8 >> 24) & 0xFF;
                ebuf[i+4] = (b8 >> 32) & 0xFF;
                ebuf[i+5] = (b8 >> 40) & 0xFF;
                ebuf[i+6] = (b8 >> 48) & 0xFF;
                ebuf[i+7] = (b8 >> 56) & 0xFF;
            }
        }
        else
        {
            for (i = start ; i < (start + length) ; i+=ainc)
            {
                b4 = ec_readeepromAP(aiadr, i >> 1 , EC_TIMEOUTEEP) & 0xFFFFFFFF;
                ebuf[i] = b4 & 0xFF;
                ebuf[i+1] = (b4 >> 8) & 0xFF;
                ebuf[i+2] = (b4 >> 16) & 0xFF;
                ebuf[i+3] = (b4 >> 24) & 0xFF;
            }
        }

        return 1;
    }

    return 0;
}


void eeprom_print(int slave) {
    int wkc;
    ec_timet tstart,tend, tdif;


    int w, rc = 0, estart, esize;
    uint16 *wbuf;

    w = 0x0000;
    wkc = ec_BRD(0x0000, ECT_REG_TYPE, sizeof(w), &w, EC_TIMEOUTSAFE);      /* detect number of slaves */
    if (wkc > 0) {
        ec_slavecount = wkc;

        printf("%d slaves found.\n", ec_slavecount);
        if ((ec_slavecount >= slave) && (slave > 0)) {

                tstart = osal_current_time();
                eeprom_read(slave, 0x0000, MINBUF); // read first 128 bytes

                wbuf = (uint16 * ) & ebuf[0];
                printf("Slave %d data\n", slave);
                printf(" PDI Control      : %4.4X\n", *(wbuf + 0x00));
                printf(" PDI Config       : %4.4X\n", *(wbuf + 0x01));
                printf(" Config Alias     : %4.4X\n", *(wbuf + 0x04));
                printf(" Checksum         : %4.4X\n", *(wbuf + 0x07));
                printf("   calculated     : %4.4X\n", SIIcrc(&ebuf[0]));
                printf(" Vendor ID        : %8.8X\n", *(uint32 * )(wbuf + 0x08));
                printf(" Product Code     : %8.8X\n", *(uint32 * )(wbuf + 0x0A));
                printf(" Revision Number  : %8.8X\n", *(uint32 * )(wbuf + 0x0C));
                printf(" Serial Number    : %8.8X\n", *(uint32 * )(wbuf + 0x0E));
                printf(" Mailbox Protocol : %4.4X\n", *(wbuf + 0x1C));
                esize = (*(wbuf + 0x3E) + 1) * 128;
                if (esize > MAXBUF) esize = MAXBUF;
                printf(" Size             : %4.4X = %d bytes\n", *(wbuf + 0x3E), esize);
                printf(" Version          : %4.4X\n", *(wbuf + 0x3F));


        } else {
            printf("Slave number outside range.\n");
        }
    } else {
        printf("No slaves found!\n");
    }
}