
/**
 ******************************************************************************
 * @file           :  etg.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "etg.h"


etg_vendor_id_t etg_vendor_id_list[ETG_NUM_VENDOR_IDS] = {
        {0x0,        "Invalid vendor ID"},
        {0x00000001, "EtherCAT Technology Group"},
        {0x00000002, "Beckhoff Automation"},
        {0x00000004, "HMS Technology Center"},
        {0x00000007, "Janz Tec "},
        {0x00000014, "KEB Automation"},
        {0x00000016, "KEBA Industrial Automation"},
        {0x00000017, "esd electronics"},
        {0x0000001B, "HMS Industrial Networks"},
        {0x0000001D, "Festo "},
        {0x00000021, "WAGO Kontakttechnik"},
        {0x00000024, "Bosch Rexroth"},
        {0x00000034, "port industrial automation"},
        {0x0000003B, "Lenze"},
        {0x00000044, "Hilscher GmbH"},
        {0x00000059, "SEW-EURODRIVE"},
        {0x0000006A, "KOLLMORGEN"},
        {0x00000083, "OMRON Corporation"},
        {0x00000084, "Phoenix Contact"},
        {0x0000009A, "Elmo Motion Control"},
        {0x0000009C, "Hans Turck"},
        {0x000000AB, "Copley Controls"},
        {0x000000AD, "Pepperl+Fuchs"},
        {0x000000B7, "ABB Oy Drives"},
        {0x000000BD, "ADVANCED Motion Controls"},
        {0x000000DE, "Dave Engineering"},
        {0x000000FB, "maxon motor"},
        {0x00000114, "SMC Corporation"},
        {0x000001C7, "Eaton Industries"},
        {0x000001DD, "Delta Electronics"},
        {0x00000247, "Harmonic Drive"},
        {0x0000026C, "Nanotec Electronic"},
        {0x00000286, "TRINAMIC Motion Control"},
        {0x00000321, "CODESYS"},
        {0x000003DB, "Baldor"},
        {0x00000404, "Applied Motion Products"},
        {0x000004D8, "Microchip Technology"},
        {0x0000050C, "ABB"},
        {0x0000050E, "zub machine control"},
        {0x00000540, "ACS Motion Control"},
        {0x00000541, "KEYENCE Corporation"},
        {0x00000567, "ABB"},
        {0x00000569, "KUNBUS"},
        {0x00000584, "Stäubli Favergesv"},
        {0x00000599, "Galil Motion Control"},
        {0x0000062A, "Delta Tau Data System"},
        {0x0000066F, "Panasonic Corporation"},
        {0x00000691, "FANUC CORPORATION"},
        {0x000006F6, "IntervalZero"},
        {0x01000089, "Parker Hannifin"},
        {0x01000056, "SICK AG"},
        {0x010000E8, "Balluff"},
        {0x0800005A, "Schneider Electric"},
        {0x000006B0, "Panasonic Production Engineering"},
        {0x000006D0, "Hitachi Europe"},
        {0x0000CEBA, "KEBA AG"},
        {0x00000117, "JVL Industri Elektronik"},
        {0x000001F9, "National Instruments Corporation"},
        {0x000006B8, "Axxon Computer Corporation"},
        {0x00000A07, "Glowbuzzer"},
        {0x000002BE, "ORIENTAL MOTOR"},
        {0x00000927, "Banner Engineering Corporation"},
        {0x000002DE, "Trio Motion Technology"},
        {0x000022D2, "Synapticon GmbH"},
        {0x00000AFC, "AutomationWare S.r.l."},
        {0x000001B9, "Sanyo Denki Co., Ltd."},
        {0x0000017F, "R.T.A. S.r.l."},
        {0x000002DE, "Trio Motion Technology Ltd."}

};


char *etg_lookup_vendor_id(uint32_t id, etg_vendor_id_t *varray, uint32_t rows_in_varray, gberror_t *grc) {
    for (int i = 0; i < rows_in_varray; i++) {
        if (varray[i].id == id) {
            *grc = E_SUCCESS;
            return varray[i].name;
        }
    }
    *grc = E_LOOKUP_FAILED;
    return varray[0].name;
}

