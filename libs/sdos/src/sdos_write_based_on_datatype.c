/**
 ******************************************************************************
 * @file           :  sdos_write_based_on_datatype
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "sdos_write_based_on_datatype.h"
#include "user_message.h"

gberror_t
sdos_write_based_on_datatype(const uint16_t slave, const uint16_t index, const uint8_t subindex, const uint8_t datatype,
                             int len,
                             const ec_value value) {

    int os;
    int rc;

    switch (datatype) {

        case ECT_BOOLEAN:

            if (ec_sdo_write_uint8(slave, index, subindex, value.boolean, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
        case ECT_INTEGER8:
            if (ec_sdo_write_int8(slave, index, subindex, value.int8, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
        case ECT_INTEGER16:
            if (ec_sdo_write_int16(slave, index, subindex, value.int16, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
            break;
        case ECT_INTEGER32:
            if (ec_sdo_write_int32(slave, index, subindex, value.int32, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
        case ECT_UNSIGNED8:
            if (ec_sdo_write_uint8(slave, index, subindex, value.uint8, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
        case ECT_UNSIGNED16:
            if (ec_sdo_write_uint16(slave, index, subindex, value.uint16, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
        case ECT_UNSIGNED32:
            if (ec_sdo_write_uint32(slave, index, subindex, value.uint32, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
        case ECT_REAL32:
            if (ec_sdo_write_real32(slave, index, subindex, value.real32, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }

        case ECT_VISIBLE_STRING:
            os = len;
            rc = ec_SDOwrite(slave, index, subindex, false, os, &value.visible_string, EC_TIMEOUTRXM);
            if (rc <= 0) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: Could not write SDO. Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                         index, subindex, value, slave);
                return E_SDO_WRITE_FAILURE;
            }

            return E_SUCCESS;

        case ECT_OCTET_STRING:
            os = len;
            rc = ec_SDOwrite(slave, index, subindex, false, os, &value.octet_string, EC_TIMEOUTRXM);
            if (rc <= 0) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: Could not write SDO. Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                         index, subindex, value, slave);
                return E_SDO_WRITE_FAILURE;
            }

            return E_SUCCESS;

        case ECT_UNICODE_STRING:
            os = len;
            rc = ec_SDOwrite(slave, index, subindex, false, os, &value.unicode_string, EC_TIMEOUTRXM);
            if (rc <= 0) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: Could not write SDO. Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                         index, subindex, value, slave);
                return E_SDO_WRITE_FAILURE;
            }

            return E_SUCCESS;


        case ECT_TIME_OF_DAY:
            UM_FATAL("GBEM: Time of day not implemented");
        case ECT_TIME_DIFFERENCE:
            UM_FATAL("GBEM: Time difference not implemented");
        case ECT_DOMAIN:
            UM_FATAL("GBEM: Domain not implemented");
        case ECT_INTEGER24:
            UM_FATAL("GBEM: Integer24 not implemented");
        case ECT_REAL64:
            UM_FATAL("GBEM: Real64 not implemented");
        case ECT_INTEGER64:
            UM_FATAL("GBEM: Integer64 not implemented");

        case ECT_UNSIGNED24:
            UM_FATAL("GBEM: Unsigned24 not implemented");

        case ECT_UNSIGNED64:
            UM_FATAL("GBEM: Unsigned64 not implemented");
        case ECT_BIT1:
        case ECT_BIT2:
        case ECT_BIT3:
        case ECT_BIT4:
        case ECT_BIT5:
        case ECT_BIT6:
        case ECT_BIT7:
        case ECT_BIT8:
            if (ec_sdo_write_uint8(slave, index, subindex, value.bit, false)) {
                return E_SUCCESS;
            } else {
                return E_SDO_WRITE_FAILURE;
            }
        case ECT_STRING8:
        default:
            //todo
//            LL_FATAL("Unknown datatype: %d", datatype);
            break;

    }
    return E_SUCCESS;
}