/**
 ******************************************************************************
 * @file           :  sdos_read_based_on_datatype
 * @brief          :  read the sdo value based on the datatype
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "gberror.h"
#include <stdint.h>
#include "ethercattype.h"
#include "ethercatsetget.h"
#include "log.h"
#include "user_message.h"

gberror_t
sdos_read_based_on_datatype(const uint16_t slave, const uint16_t index, const uint8_t subindex, const uint8_t datatype,
                            int len,
                            ec_value *value) {

    int rc = E_GENERAL_FAILURE;

    switch (datatype) {

        case ECT_BOOLEAN:


            if (ec_sdo_read_uint8(slave, index, subindex, &value->boolean, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }
        case ECT_INTEGER8:

            if (ec_sdo_read_int8(slave, index, subindex, &value->int8, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }

        case ECT_INTEGER16:

            if (ec_sdo_read_int16(slave, index, subindex, &value->int16, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }

        case ECT_INTEGER32:

//            value->int32 = 42;
//            return E_SUCCESS;
            if (ec_sdo_read_int32(slave, index, subindex, &value->int32, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }

        case ECT_UNSIGNED8:

            if (ec_sdo_read_uint8(slave, index, subindex, &value->uint8, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }

        case ECT_UNSIGNED16:

            if (ec_sdo_read_uint16(slave, index, subindex, &value->uint16, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }

        case ECT_UNSIGNED32:

            if (ec_sdo_read_uint32(slave, index, subindex, &value->uint32, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }

        case ECT_REAL32:

            if (ec_sdo_read_real32(slave, index, subindex, &value->real32, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }

        case ECT_VISIBLE_STRING:
            rc = ec_SDOread(slave, index, subindex, false, &len, &value->visible_string, EC_TIMEOUTRXM);
            value->visible_string[len + 1] = '\0';
            if (rc <= 0) {
                UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x - len: %d (on slave:%u)",
                         index,
                         subindex, len, slave);
                return E_SDO_READ_FAILURE;
            } else {
                return E_SUCCESS;
            }
        case ECT_OCTET_STRING:
            rc = ec_SDOread(slave, index, subindex, false, &len, &value->octet_string, EC_TIMEOUTRXM);

            if (rc <= 0) {
                UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x - len: %d (on slave:%u)",
                         index,
                         subindex, len, slave);
                return E_SDO_READ_FAILURE;
            } else {
                return E_SUCCESS;
            }
        case ECT_UNICODE_STRING:
            rc = ec_SDOread(slave, index, subindex, false, &len, &value->unicode_string, EC_TIMEOUTRXM);

            if (rc <= 0) {
                UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x - len: %d (on slave:%u)",
                         index,
                         subindex, len, slave);
                return E_SDO_READ_FAILURE;
            } else {
                return E_SUCCESS;
            }
        case ECT_TIME_OF_DAY:
            UM_FATAL("GBEM: Time of day datatype not supported");
            break;
        case ECT_TIME_DIFFERENCE:
            UM_FATAL("GBEM: Time difference datatype not supported");
            break;
        case ECT_DOMAIN:
            UM_FATAL("GBEM: Domain datatype not supported");
            break;
        case ECT_INTEGER24:
            UM_FATAL("GBEM: Integer 24 datatype not supported");
            break;
        case ECT_REAL64:
            UM_FATAL("GBEM: Real 64 datatype not supported");
            break;
        case ECT_INTEGER64:
            UM_FATAL("GBEM: Integer 64 datatype not supported");
            break;
        case ECT_UNSIGNED24:
            UM_FATAL("GBEM: Unsigned 24 datatype not supported");
            break;
        case ECT_UNSIGNED64:
            UM_FATAL("GBEM: Unsigned 64 datatype not supported");
            break;
        case ECT_BIT1:
        case ECT_BIT2:
        case ECT_BIT3:
        case ECT_BIT4:
        case ECT_BIT5:
        case ECT_BIT6:
        case ECT_BIT7:
        case ECT_BIT8:
            if (ec_sdo_read_uint8(slave, index, subindex, &value->bit, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_READ_FAILURE;
            }
        case ECT_STRING8:
            break;
        default:
            UM_FATAL("GBEM: Unknown datatype: %d", datatype);

    }
    return E_SUCCESS;
}