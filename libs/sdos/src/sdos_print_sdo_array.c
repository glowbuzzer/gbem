/**
 ******************************************************************************
 * @file           :  sdos_print_sdo_array
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "sdos_print_sdo_array.h"
#include <stdio.h>
#include "user_message.h"

void sdos_print_sdo(ec_sdo *sdo) {
    printf("Index: 0x%04X, Subindex: 0x%02X, Length: %d, Datatype: %d, Value: %d",
           sdo->index, sdo->subindex, sdo->len, sdo->datatype, sdo->value);
}


void sdos_print_sdo_array(ec_sdo_array *array) {
    for (int i = 0; i < array->num_slaves; i++) {
        UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] Slave %d - number of SDOs [%d]", i, array->num_sdo[i]);
        if (array->num_sdo[i] == 0) {
//            UM_INFO(GBEM_UM_EN, "[JSON config]  No SDOs defined for this slave");
        } else {
            UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] %-8s %-8s %-6s %-10s %-10s", "Index", "Subindex", "Length",
                    "Datatype",
                    "Value");
            for (int j = 0; j < array->num_sdo[i]; j++) {
                ec_sdo *sdo = &array->sdo[i][j];
                UM_INFO(GBEM_UM_EN, "GBEM: [JSON config] 0x%04X   0x%02X     %-6d %-10d %-10d",
                        sdo->index, sdo->subindex, sdo->len, sdo->datatype, sdo->value);
            }
        }
    }
}

