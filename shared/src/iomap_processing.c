
/**
 ******************************************************************************
 * @file           :  iomap_processing.c
 * @brief          :  functions for interacting with the iomap
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "iomap_processing.h"

#include <user_message.h>

#include "float_int_conv.h"
#include "ethercatsetget.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "dpm.h"

bool iomap_get_pdo_in_bool(bool byte_slave, const uint16_t slave_num, const uint32_t byte_num,
                           const uint8_t bit_num) {
    if (!byte_slave) {
        return (ec_pdo_get_input_bit(slave_num, bit_num));
    } else {
        return (ec_pdo_get_input_bit_from_byte_slave(slave_num, byte_num, bit_num));
    }
}


/**
 * @brief gets a PDO value of type defined by pdo_type and casts it to a int32_t and returns it
 * @param pdo_type
 * @param slave_num
 * @param byte_num
 * @return value read from PDO
 */
int32_t iomap_get_pdo_in_int32(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num) {
    switch (pdo_type) {
        case ECT_REAL32:
            return ((int32_t) ec_pdo_get_input_float(slave_num, byte_num));
        case ECT_UNSIGNED8:
            return ((int32_t) ec_pdo_get_input_uint8(slave_num, byte_num));
        case ECT_INTEGER8:
            return ((int32_t) ec_pdo_get_input_int8(slave_num, byte_num));
        case ECT_UNSIGNED16:
            return ((int32_t) ec_pdo_get_input_uint16(slave_num, byte_num));
        case ECT_INTEGER16:
            return ((int32_t) ec_pdo_get_input_int16(slave_num, byte_num));
        case ECT_UNSIGNED32:
            return ((int32_t) ec_pdo_get_input_uint32(slave_num, byte_num));
        case ECT_INTEGER32:
            return ((int32_t) ec_pdo_get_input_int32(slave_num, byte_num));
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
            return 0;
    }
}

/**
 * @brief gets a PDO value of type defined by pdo_type and casts it to a uint32_t and returns it
 * @param pdo_type
 * @param slave_num
 * @param byte_num
 * @return value read from PDO
 */
uint32_t iomap_get_pdo_in_uint32(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num) {
    switch (pdo_type) {
        case ECT_REAL32:
            return ((uint32_t) ec_pdo_get_input_float(slave_num, byte_num));
        case ECT_UNSIGNED8:
            return ((uint32_t) ec_pdo_get_input_uint8(slave_num, byte_num));
        case ECT_INTEGER8:
            return ((uint32_t) ec_pdo_get_input_int8(slave_num, byte_num));
        case ECT_UNSIGNED16:
            return ((uint32_t) ec_pdo_get_input_uint16(slave_num, byte_num));
        case ECT_INTEGER16:
            return ((uint32_t) ec_pdo_get_input_int16(slave_num, byte_num));
        case ECT_UNSIGNED32:
            return ((uint32_t) ec_pdo_get_input_uint32(slave_num, byte_num));
        case ECT_INTEGER32:
            return ((uint32_t) ec_pdo_get_input_int32(slave_num, byte_num));
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
            return 0;
    }
}

/**
 * @brief gets a PDO value of type defined by pdo_type and converts it to a float (with max val) and returns it
 * @param pdo_type
 * @param slave_num
 * @param byte_num
 * @return value from PDO (converted to float)
 * @warning doesn't handle bool to float...
 */

float iomap_get_pdo_in_float(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num,
                             const float max_val) {
    switch (pdo_type) {
        case ECT_REAL32:
            return (ec_pdo_get_input_float(slave_num, byte_num));
        case ECT_UNSIGNED8:
            return (uint8_to_float(max_val, ec_pdo_get_input_uint8(slave_num, byte_num)));
        case ECT_INTEGER8:
            return (int8_to_float(max_val, ec_pdo_get_input_int8(slave_num, byte_num)));
        case ECT_UNSIGNED16:
            return (uint16_to_float(max_val, ec_pdo_get_input_uint16(slave_num, byte_num)));
        case ECT_INTEGER16:
            return (int16_to_float(max_val, ec_pdo_get_input_int16(slave_num, byte_num)));
        case ECT_UNSIGNED32:
            return (uint32_to_float(max_val, ec_pdo_get_input_uint32(slave_num, byte_num)));
        case ECT_INTEGER32:
            return (int32_to_float(max_val, ec_pdo_get_input_int32(slave_num, byte_num)));
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
            return 0;
    }
}


void iomap_set_pdo_out_bool(bool byte_slave, const uint16_t slave_num, const uint32_t byte_num,
                            const uint8_t bit_num, const bool val) {
    if (!byte_slave) {
        ec_pdo_set_output_bit(slave_num, bit_num, val);
        //        printf("set bit on slave_num [%u] bit_num [%u] to val [%d]\n", slave_num, bit_num, val);
    } else {
        //        printf("set bit on slave_num [%u] byte_num [%u] bit_num [%u] to val [%d]\n", slave_num, byte_num, bit_num, val);
        ec_pdo_set_output_bit_from_byte_slave(slave_num, byte_num, bit_num, val);
    }
}

/**
 * @brief Sets a pdo out (type defined by the PDO) using a float provided as a parameter (val)
 * @param pdo_type datatype of the PDO
 * @param slave_num
 * @param byte_num
 * @param max_val
 * @param val value to be set
 */
void iomap_set_pdo_out_float(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num,
                             const float val, const float max_val) {
    switch (pdo_type) {
        case ECT_REAL32:
            ec_pdo_set_output_float(slave_num, byte_num, val);
            break;
        case ECT_UNSIGNED8:
            ec_pdo_set_output_uint8(slave_num, byte_num, float_to_uint8((float) max_val, val));
            break;
        case ECT_INTEGER8:
            ec_pdo_set_output_int8(slave_num, byte_num, float_to_int8((float) max_val, val));
            break;
        case ECT_UNSIGNED16:
            ec_pdo_set_output_uint16(slave_num, byte_num, float_to_uint16((float) max_val, val));
            break;
        case ECT_INTEGER16:
            ec_pdo_set_output_int16(slave_num, byte_num, float_to_int16((float) max_val, val));
            break;
        case ECT_UNSIGNED32:
            ec_pdo_set_output_uint32(slave_num, byte_num, float_to_uint32((float) max_val, val));
            break;
        case ECT_INTEGER32:
            ec_pdo_set_output_int32(slave_num, byte_num, float_to_int32((float) max_val, val));
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
    }
}


void iomap_set_pdo_out_int32(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num,
                             const int32_t val, const float max_val) {
    switch (pdo_type) {
        case ECT_INTEGER8:
            ec_pdo_set_output_int8(slave_num, byte_num, (int8_t) val);
            break;
        case ECT_UNSIGNED8:
            ec_pdo_set_output_uint8(slave_num, byte_num, (uint8_t) val);
            break;
        case ECT_INTEGER16:
            ec_pdo_set_output_int16(slave_num, byte_num, (int16_t) val);
            break;
        case ECT_UNSIGNED16:
            ec_pdo_set_output_uint16(slave_num, byte_num, (uint16_t) val);
            break;
        case ECT_INTEGER32:
            ec_pdo_set_output_int32(slave_num, byte_num, (int32_t) val);
            break;
        case ECT_UNSIGNED32:
            ec_pdo_set_output_uint32(slave_num, byte_num, (uint32_t) val);
            break;
        case ECT_REAL32:
            ec_pdo_set_output_float(slave_num, byte_num, int32_to_float(max_val, val));
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
    }
}


void iomap_set_pdo_out_uint32(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num,
                              const uint32_t val, const float max_val) {
    switch (pdo_type) {
        case ECT_INTEGER8:
            ec_pdo_set_output_int8(slave_num, byte_num, (int8_t) val);
            break;
        case ECT_UNSIGNED8:
            ec_pdo_set_output_uint8(slave_num, byte_num, (uint8_t) val);
            break;
        case ECT_INTEGER16:
            ec_pdo_set_output_int16(slave_num, byte_num, (int16_t) val);
            break;
        case ECT_UNSIGNED16:
            ec_pdo_set_output_uint16(slave_num, byte_num, (uint16_t) val);
            break;
        case ECT_INTEGER32:
            ec_pdo_set_output_int32(slave_num, byte_num, (int32_t) val);
            break;
        case ECT_UNSIGNED32:
            ec_pdo_set_output_uint32(slave_num, byte_num, (uint32_t) val);
            break;
        case ECT_REAL32:
            ec_pdo_set_output_float(slave_num, byte_num, uint32_to_float(max_val, val));
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
    }
}


void iomap_set_gbc_in_union(const uint16_t io_num, const ec_datatype gbc_type, const ec_datatype_value_t val) {
    switch (gbc_type) {
        case ECT_BOOLEAN:
            if (val.ect_boolean) {
                BIT_SET(dpm_in->digital[0], io_num);
            } else {
                BIT_CLEAR(dpm_in->digital[0], io_num);
            }
            break;
        case ECT_INTEGER32:
            dpm_in->integer32[io_num] = val.ect_integer32;
            break;
        case ECT_UNSIGNED32:
            dpm_in->unsigned32[io_num] = val.ect_unsigned32;
            break;
        case ECT_REAL32:
            dpm_in->analog[io_num] = val.ect_real32;
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
            break;
    }
}

void iomap_set_plc_in_union(const ec_datatype plc_type, void *io, const ec_datatype_value_t val) {
    switch (plc_type) {
        case ECT_BOOLEAN:
            *((bool *) io) = val.ect_boolean;
        case ECT_INTEGER8:
            *((int8_t *) io) = val.ect_integer8;
            break;
        case ECT_UNSIGNED8:
            *((uint8_t *) io) = val.ect_unsigned8;
            break;
        case ECT_INTEGER16:
            *((int16_t *) io) = val.ect_integer16;
            break;
        case ECT_UNSIGNED16:
            *((uint16_t *) io) = val.ect_unsigned16;
            break;
        case ECT_INTEGER32:
            *((int32_t *) io) = val.ect_integer32;
            break;
        case ECT_UNSIGNED32:
            *((uint32_t *) io) = val.ect_unsigned32;
            break;
        case ECT_REAL32:
            *((float *) io) = val.ect_real32;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
    }
}

void iomap_set_pdo_out_union(const uint16_t slave_num, const uint32_t byte_num, const ec_datatype pdo_type,
                             const ec_datatype_value_t val) {
    switch (pdo_type) {
        case ECT_BOOLEAN:
            ec_pdo_set_output_bit(slave_num, byte_num, val.ect_boolean);
        case ECT_INTEGER8:
            ec_pdo_set_output_int8(slave_num, byte_num, val.ect_integer8);
            break;
        case ECT_UNSIGNED8:
            ec_pdo_set_output_uint8(slave_num, byte_num, val.ect_unsigned8);
            break;
        case ECT_INTEGER16:
            ec_pdo_set_output_int16(slave_num, byte_num, val.ect_integer16);
            break;
        case ECT_UNSIGNED16:
            ec_pdo_set_output_uint16(slave_num, byte_num, val.ect_unsigned16);
            break;
        case ECT_INTEGER32:
            ec_pdo_set_output_int32(slave_num, byte_num, val.ect_integer32);
            break;
        case ECT_UNSIGNED32:
            ec_pdo_set_output_uint32(slave_num, byte_num, val.ect_unsigned32);
            break;
        case ECT_REAL32:
            ec_pdo_set_output_float(slave_num, byte_num, val.ect_real32);
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
    }
}

/**
 * @brief sets a gbc digital (specified by a gbc_io_id) with a value read from a PDO using the PDOs datatype
 * @param pdo_type
 * @param slave_num
 * @param bit_byte_num
 * @param gbc_io_id
 * @warning the PDO datatype must be a BOOLEAN
 */

void iomap_set_gbc_digital_in_from_pdo(bool byte_slave, const uint16_t slave_num, const uint32_t byte_num,
                                       uint8_t bit_num, const uint16_t gbc_io_id, const map_gbc_io_type_t type) {
    if (!byte_slave) {
        if (ec_pdo_get_input_bit(slave_num, bit_num)) {
            if (type == GBC_IO_TYPE_NORMAL) {
                BIT_SET(dpm_in->digital[0], gbc_io_id);
            } else if (type == GBC_IO_TYPE_SAFETY) {
                BIT_SET(dpm_in->safetyDigital[0], gbc_io_id);
            } else {
                UM_FATAL("GBEM: An external IO has been mapped to a digital in");
            }
        } else {
            if (type == GBC_IO_TYPE_NORMAL) {
                BIT_CLEAR(dpm_in->digital[0], gbc_io_id);
            } else if (type == GBC_IO_TYPE_SAFETY) {
                BIT_CLEAR(dpm_in->safetyDigital[0], gbc_io_id);
            } else {
                UM_FATAL("GBEM: An external IO has been mapped to a digital in");
            }
        }
    } else {
        if (ec_pdo_get_input_bit_from_byte_slave(slave_num, byte_num, bit_num)) {
            if (type == GBC_IO_TYPE_NORMAL) {
                BIT_SET(dpm_in->digital[0], gbc_io_id);
            } else if (type == GBC_IO_TYPE_SAFETY) {
                BIT_SET(dpm_in->safetyDigital[0], gbc_io_id);
            } else {
                UM_FATAL("GBEM: An external IO has been mapped to a digital in");
            }
        } else {
            if (type == GBC_IO_TYPE_NORMAL) {
                BIT_CLEAR(dpm_in->digital[0], gbc_io_id);
            } else if (type == GBC_IO_TYPE_SAFETY) {
                BIT_CLEAR(dpm_in->safetyDigital[0], gbc_io_id);
            } else {
                UM_FATAL("GBEM: An external IO has been mapped to a digital in");
            }
        }
    }
}

/**
 * @brief sets a gbc int32 (specified by a gbc_io_id) with a value read from a PDO using the PDOs datatype
 * @param pdo_type
 * @param slave_num
 * @param bit_byte_num
 * @param gbc_io_id
 * @param max_val
 */
void iomap_set_gbc_in_int32_from_pdo(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num,
                                     const uint16_t gbc_io_id, const float max_val) {
    switch (pdo_type) {
        case ECT_INTEGER8:
            dpm_in->integer32[gbc_io_id] = (int32_t) ec_pdo_get_input_int8(slave_num, byte_num);
            break;
        case ECT_UNSIGNED8:
            dpm_in->integer32[gbc_io_id] = (int32_t) ec_pdo_get_input_uint8(slave_num, byte_num);
            break;
        case ECT_INTEGER16:
            dpm_in->integer32[gbc_io_id] = (int32_t) ec_pdo_get_input_int16(slave_num, byte_num);
            break;
        case ECT_UNSIGNED16:
            dpm_in->integer32[gbc_io_id] = (int32_t) ec_pdo_get_input_uint16(slave_num, byte_num);
            break;
        case ECT_INTEGER32:
            dpm_in->integer32[gbc_io_id] = (int32_t) ec_pdo_get_input_int32(slave_num, byte_num);
            break;
        case ECT_UNSIGNED32:
            dpm_in->integer32[gbc_io_id] = (int32_t) ec_pdo_get_input_uint32(slave_num, byte_num);
            break;
        case ECT_REAL32:
            dpm_in->integer32[gbc_io_id] = float_to_int32(max_val, ec_pdo_get_input_float(slave_num, byte_num));
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
    } //end case
}

/**
 * @brief sets a gbc uint32 (specified by a gbc_io_id) with a value read from a PDO using the PDOs datatype
 * @param pdo_type
 * @param slave_num
 * @param bit_byte_num
 * @param gbc_io_id
 * @param max_val
 */
void
iomap_set_gbc_uint32_in_from_pdo(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num,
                                 const uint16_t gbc_io_id, const float max_val) {
    switch (pdo_type) {
        case ECT_INTEGER8:
            dpm_in->unsigned32[gbc_io_id] = (uint32_t) ec_pdo_get_input_int8(slave_num, byte_num);
            break;
        case ECT_UNSIGNED8:
            dpm_in->unsigned32[gbc_io_id] = (uint32_t) ec_pdo_get_input_uint8(slave_num, byte_num);
            break;
        case ECT_INTEGER16:
            dpm_in->unsigned32[gbc_io_id] = (uint32_t) ec_pdo_get_input_int16(slave_num, byte_num);
            break;
        case ECT_UNSIGNED16:
            dpm_in->unsigned32[gbc_io_id] = (uint32_t) ec_pdo_get_input_uint16(slave_num, byte_num);
            break;
        case ECT_INTEGER32:
            dpm_in->unsigned32[gbc_io_id] = (uint32_t) ec_pdo_get_input_int32(slave_num, byte_num);
            break;
        case ECT_UNSIGNED32:
            dpm_in->unsigned32[gbc_io_id] = (uint32_t) ec_pdo_get_input_uint32(slave_num, byte_num);
            break;
        case ECT_REAL32:
            dpm_in->unsigned32[gbc_io_id] = float_to_uint32(max_val, ec_pdo_get_input_float(slave_num, byte_num));
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
    } //end case
}


/**
 * sets a gbc float (analog) (specified by a gbc_io_id) with a value read from a PDO using the PDOs datatype
 * @param pdo_type
 * @param slave_num
 * @param bit_byte_num
 * @param gbc_io_id
 * @param max_val
 */
void iomap_set_gbc_float_in_from_pdo(const ec_datatype pdo_type, const uint16_t slave_num, const uint32_t byte_num,
                                     const uint16_t gbc_io_id,
                                     const float max_val) {
    switch (pdo_type) {
        case ECT_REAL32:
            dpm_in->analog[gbc_io_id] = ec_pdo_get_input_float(slave_num, byte_num);
            break;
        case ECT_UNSIGNED8:
            dpm_in->analog[gbc_io_id] = uint8_to_float((float) max_val, ec_pdo_get_input_uint8(slave_num, byte_num));
            break;
        case ECT_INTEGER8:
            dpm_in->analog[gbc_io_id] = int8_to_float((float) max_val, ec_pdo_get_input_int8(slave_num, byte_num));
            break;
        case ECT_UNSIGNED16:
            dpm_in->analog[gbc_io_id] = uint16_to_float((float) max_val, ec_pdo_get_input_uint16(slave_num, byte_num));
            break;
        case ECT_INTEGER16:
            dpm_in->analog[gbc_io_id] = int16_to_float((float) max_val, ec_pdo_get_input_int16(slave_num, byte_num));
            break;
        case ECT_UNSIGNED32:
            dpm_in->analog[gbc_io_id] = uint32_to_float((float) max_val, ec_pdo_get_input_uint32(slave_num, byte_num));
            break;
        case ECT_INTEGER32:
            dpm_in->analog[gbc_io_id] = int32_to_float((float) max_val, ec_pdo_get_input_int32(slave_num, byte_num));
            break;
        default:
            LL_ERROR(GBEM_GEN_LOG_EN, "GBEM: Invalid datatype in iomap");
    }
}

//todo crit fix unions with byte slaves
