/*
 * ethercatsetget.c
 *
 *  Created on: 29 Aug 2020
 *      Author: David
 */

#include <stdint.h>
#include <stdbool.h>
#include "ethercat.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "std_defs_and_macros.h"

const char *ec_datatype_string[0x0037 + 2] = {
        [0x0000] = "ECT_UNDEFINED",
        [0x0001] = "ECT_BOOLEAN",
        [0x0002] = "ECT_INTEGER8",
        [0x0003]="ECT_INTEGER16",
        [0x0004]="ECT_INTEGER32",
        [0x0005]="ECT_UNSIGNED8",
        [0x0006]="ECT_UNSIGNED16",
        [0x0007]="ECT_UNSIGNED32",
        [0x0008]="ECT_REAL32",
        [0x0009]="ECT_VISIBLE_STRING",
        [0x000A]="ECT_OCTET_STRING",
        [0x000B]="ECT_UNICODE_STRING",
        [0x000C]="ECT_TIME_OF_DAY",
        [0x000D]="ECT_TIME_DIFFERENCE",
        [0x000F]="ECT_DOMAIN",
        [0x0010]="ECT_INTEGER24",
        [0x0011]="ECT_REAL64",
        [0x0015]="ECT_INTEGER64",
        [0x0016]="ECT_UNSIGNED24",
        [0x001B]="ECT_UNSIGNED64",
        [0x0030]="ECT_BIT1",
        [0x0031]="ECT_BIT2",
        [0x0032]="ECT_BIT3",
        [0x0033]="ECT_BIT4",
        [0x0034]="ECT_BIT5",
        [0x0035]="ECT_BIT6",
        [0x0036]="ECT_BIT7",
        [0x0037]="ECT_BIT8"
};


/**
 * @brief  Send unit8 to EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of byte in frame, test using slalveinfo
 * @param  value: Value of uint8
 * @retval None
*/
void ec_pdo_set_output_uint8(uint16_t slave_no, uint8_t byte_no, uint8_t value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    //warning
    *data_ptr = value;


}

/**
 * @brief  Send int8 to EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of byte in frame, test using slalveinfo
 * @param  value: Value of uint8
 * @retval None
*/
void ec_pdo_set_output_int8(uint16_t slave_no, uint8_t byte_no, int8_t value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    //warning
    *data_ptr = (value >> 0) & 0xFF;


}


/**
 * @brief  Send uint16 to EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of uint16 in frame, test using slaveinfo
 * @param  value: Value of uint16
 * @retval None
*/
void ec_pdo_set_output_uint16(uint16_t slave_no, uint8_t byte_no, uint16_t value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr = (value >> 8) & 0xFF;
}

/**
 * @brief  Send 1 int16 to EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of int16 in frame, test using salveinfo
 * @param  value: Value of int16
 * @retval None
*/
void ec_pdo_set_output_int16(uint16_t slave_no, uint8_t byte_no, int16_t value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr = (value >> 8) & 0xFF;
}


/**
 * @brief  Send uint32 to EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of uint32 in frame, test using slaveinfo
 * @param  value: Value of uint32
 * @retval None
*/
void ec_pdo_set_output_uint32(uint16_t slave_no, uint8_t byte_no, uint32_t value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Read value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr++ = (value >> 8) & 0xFF;
    *data_ptr++ = (value >> 16) & 0xFF;
    *data_ptr = (value >> 24) & 0xFF;
}


/**
 * @brief  Send int32 to EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of int32 in frame, test using slaveinfo
 * @param  value: Value of int32
 * @retval None
*/
void ec_pdo_set_output_int32(uint16_t slave_no, uint8_t byte_no, int32_t value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr++ = (value >> 8) & 0xFF;
    *data_ptr++ = (value >> 16) & 0xFF;
    *data_ptr = (value >> 24) & 0xFF;
}


/**
 * @brief  Send float to EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of float in frame, test using slaveinfo
 * @param  value: Value of float
 * @retval None
*/
void ec_pdo_set_output_float(uint16_t slave_no, uint8_t byte_no, float value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */

    volatile union {
        float val;
        uint8_t bytes[4];
    } cvt;

    cvt.val = value;


    *data_ptr++ = (cvt.bytes[0]);
    *data_ptr++ = (cvt.bytes[1]);
    *data_ptr++ = (cvt.bytes[2]);
    *data_ptr = (cvt.bytes[3]);

}

/**
 * @brief Get float from EtherCAT
 * @param slave_no: Number of slave
 * @param byte_no: Location of float in frame, test using slaveinfo
 * @return floatt value
 */
float ec_pdo_get_input_float(uint16_t slave_no, uint8_t byte_no) {
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    /* Read value byte by byte since all targets can't handle misaligned
     * addresses
     */

    union {
        float val;
        uint8_t bytes[4];
    } cvt;

    cvt.bytes[0] = *data_ptr++;
    cvt.bytes[1] += (*data_ptr++);
    cvt.bytes[2] += (*data_ptr++);
    cvt.bytes[3] += (*data_ptr);

    return cvt.val;
}


/**
 * @brief Get uint32 from EtherCAT
 * @param slave_no: Number of slave
 * @param byte_no: Location of int32 in frame, test using slaveinfo
 * @return uint32_t value
 */
uint32_t ec_pdo_get_input_uint32(uint16_t slave_no, uint8_t byte_no) {
    uint32_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    /* Read value byte by byte since all targets can't handle misaligned
     * addresses
     */
    return_value = *data_ptr++;
    return_value += (*data_ptr++ << 8);
    return_value += (*data_ptr++ << 16);
    return_value += (*data_ptr << 24);

    return return_value;
}


/**
 * @brief Get uint16 from EtherCAT
 * @param slave_no: Number of slave
 * @param byte_no: Location of uint16 in frame, test using slaveinfo
 * @return uint16_t value
 */
uint16_t ec_pdo_get_input_uint16(uint16_t slave_no, uint8_t byte_no) {
    uint16_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    /* Read value byte by byte since all targets can't handle misaligned
     * addresses
     */
    return_value = *data_ptr++;
    return_value += (*data_ptr << 8);


    return return_value;
}


/**
 * @brief Get int32 from EtherCAT
 * @param slave_no: Number of slave
 * @param byte_no: Location of uint32 in frame, test using slaveinfo
 * @return int32_t value
 */
int32_t ec_pdo_get_input_int32(uint16_t slave_no, uint8_t byte_no) {
    int32_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    /* Read value byte by byte since all targets can't handle misaligned
     * addresses
     */
    return_value = *data_ptr++;
    return_value += (*data_ptr++ << 8);
    return_value += (*data_ptr++ << 16);
    return_value += (*data_ptr << 24);

    return return_value;
}

/**
 * @brief Get int16 from EtherCAT
 * @param slave_no: Number of slave
 * @param byte_no: Location of int16 in frame, test using slaveinfo
 * @return int16_t value
 */
int16_t ec_pdo_get_input_int16(uint16_t slave_no, uint8_t byte_no) {
    int16_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    /* Read value byte by byte since all targets can't handle misaligned
     * addresses
     */
    return_value = *data_ptr++;
    return_value += (*data_ptr << 8);
    return return_value;
}


/**
 * @brief  Get uint8 from EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of byte in frame, test using slaveinfo
 * @retval uint8_t value
*/
uint8_t ec_pdo_get_input_uint8(uint16_t slave_no, uint8_t byte_no) {
    uint8_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    return_value = *data_ptr;
    return return_value;
}

/**
 * @brief  Get int8 from EtherCAT
 * @param  slave_no: Number of slave
 * @param  byte_no: Location of byte in frame, test using slaveinfo
 * @retval int8_t value
*/
int8_t ec_pdo_get_input_int8(uint16_t slave_no, uint8_t byte_no) {
    int8_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    return_value = (int8_t) *data_ptr;
    return return_value;
}


#define BIT(nr) (1UL << (nr))


/**
 * @brief Reads a bit from EtherCAT OUTPUT
 * @param slave_no
 * @param bit_no
 * @return
 *  @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */

bool cs_pdo_get_output_bit_rev(uint16 slave_no, uint8_t bit_no) {
    /* Get the the startbit position in slaves IO byte */
    uint8_t startbit = ec_slave[slave_no].Ostartbit;
    /* Mask bit and return boolean 0 or 1 */
    if (*ec_slave[slave_no].outputs & BIT (bit_no - 1 + startbit))
        return true;
    else
        return false;
}




/**
 * @brief Reads a bit from EtherCAT
 * @param slave_no
 * @param bit_no
 * @return
 */
//dghack why was this this way round??
bool ec_pdo_get_input_bit(uint16 slave_no, uint8_t bit_no) {
    /* Get the the startbit position in slaves IO bit */
    uint8_t startbit = ec_slave[slave_no].Istartbit;
    /* Mask bit and return boolean 0 or 1 */
    if (*ec_slave[slave_no].inputs & BIT (bit_no - 1 + startbit))
        return true;
    else
        return false;
}

/**
 * @brief Sets a bit on EtherCAT
 * @param slave_no
 * @param bit_no
 * @param value
 */
void ec_pdo_set_output_bit(uint16_t slave_no, uint8_t bit_no, bool value) {
    /* Get the the startbit position in slaves IO byte */
    uint8_t startbit = ec_slave[slave_no].Ostartbit;
    /* Set or Clear bit */
    if (value == false) {
        *ec_slave[slave_no].outputs &= ~(1 << (bit_no - 1 + startbit));
    } else {
        *ec_slave[slave_no].outputs |= (1 << (bit_no - 1 + startbit));
    }
}


/**
 * @brief sets a unit16 in the iomap INPUT
 * @param slave_no
 * @param byte_no
 * @param value
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
void ec_pdo_set_input_uint16_rev(uint16_t slave_no, uint8_t byte_no, uint16_t value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr = (value >> 8) & 0xFF;

}

/**
 * @brief sets a int8 in the iomap INPUT
 * @param slave_no
 * @param byte_no
 * @param value
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
void ec_pdo_set_input_int8_rev(uint16_t slave_no, uint8_t byte_no, int8_t value) {
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr = (value >> 0) & 0xFF;

}


/**
 * @brief sets a uint8 in the iomap INPUT
 * @param slave_no
 * @param byte_no
 * @param value
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
void ec_pdo_set_input_uint8(uint16_t slave_no, uint8_t byte_no, uint8_t value) {

    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr = (value >> 0) & 0xFF;
}


/**
 * @brief sets a int16 in the iomap INPUT
 * @param slave_no
 * @param byte_no
 * @param value
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
void ec_pdo_set_input_int16_rev(uint16_t slave_no, uint8_t byte_no, int16_t value) {

    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr = (value >> 8) & 0xFF;
}


/**
 * @brief sets a uint32 in the iomap INPUT
 * @param slave_no
 * @param byte_no
 * @param value
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
void ec_pdo_set_input_uint32_rev(uint16_t slave_no, uint8_t byte_no, uint32_t value) {

    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr++ = (value >> 8) & 0xFF;
    *data_ptr++ = (value >> 16) & 0xFF;
    *data_ptr = (value >> 24) & 0xFF;
}


/**
 * @brief sets a float in the iomap INPUT
 * @param slave_no
 * @param byte_no
 * @param value
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
void ec_pdo_set_input_float_rev(uint16_t slave_no, uint8_t byte_no, float value) {

    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;


    union {
        float val;
        uint8_t bytes[4];
    } cvt;

    cvt.val = value;

//    printf("%" PRIu8 "\n", cvt.bytes[0]);
//    printf("%" PRIu8 "\n", cvt.bytes[1]);
//    printf("%" PRIu8 "\n", cvt.bytes[2]);
//    printf("%" PRIu8 "\n", cvt.bytes[3]);


    *data_ptr++ = (cvt.bytes[0]);
    *data_ptr++ = (cvt.bytes[1]);
    *data_ptr++ = (cvt.bytes[2]);
    *data_ptr = (cvt.bytes[3]);


}


/**
 * @brief sets a imt32 in the iomap INPUT
 * @param slave_no
 * @param byte_no
 * @param value
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
void ec_pdo_set_input_int32_rev(uint16_t slave_no, uint8_t byte_no, int32_t value) {

    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;
    /* Write value byte by byte since all targets can't handle misaligned
  addresses
     */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr++ = (value >> 8) & 0xFF;
    *data_ptr++ = (value >> 16) & 0xFF;
    *data_ptr = (value >> 24) & 0xFF;
}


float ec_pdo_get_output_float_rev(uint16_t slave_no, uint8_t byte_no) {

    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    union {
        float val;
        uint8_t bytes[4];
    } cvt;

//original
//    cvt.bytes[0] = *data_ptr++;
//    cvt.bytes[1] += (*data_ptr++ << 8);
//    cvt.bytes[2] += (*data_ptr++ << 16);
//    cvt.bytes[3] += (*data_ptr++ << 24);


    cvt.bytes[0] = *data_ptr++;
    cvt.bytes[1] += (*data_ptr++);
    cvt.bytes[2] += (*data_ptr++);
    cvt.bytes[3] += (*data_ptr);


    return cvt.val;


}
/**
 * @brief reads a OUTPUT float from iomap
 * @param slave_no
 * @param byte_no
 * @return
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */





/**
 * @brief reads a OUTPUT unit16 from iomap
 * @param slave_no
 * @param byte_no
 * @return
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
uint16_t ec_pdo_get_output_uint16_rev(uint16_t slave_no, uint8_t byte_no) {
    uint32_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    /* Read value byte by byte since all targets can't handle misaligned
     * addresses
     */
    return_value = *data_ptr++;
    return_value += *data_ptr << 8;


    return return_value;
}


/**
 * @brief reads a OUTPUT unit32 from iomap
 * @param slave_no
 * @param byte_no
 * @return
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
uint32_t ec_pdo_get_output_uint32_rev(uint16_t slave_no, uint8_t byte_no) {
    uint32_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].outputs;
/* Move pointer to correct byte index*/
    data_ptr += byte_no;

/* Read value byte by byte since all targets can't handle misaligned
 * addresses
 */
    return_value = *data_ptr++;
    return_value += (*data_ptr++ << 8);
    return_value += (*data_ptr++ << 16);
    return_value += (*data_ptr << 24);


    return return_value;
}


/**
 * @brief reads an OUTPUT int32 from the iomap
 * @param slave_no
 * @param byte_no
 * @return
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
int32_t ec_pdo_get_output_int32_rev(uint16_t slave_no, uint8_t byte_no) {
    int32_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    /* Read value byte by byte since all targets can't handle misaligned
     * addresses
     */
    return_value = *data_ptr++;
    return_value += (*data_ptr++ << 8);
    return_value += (*data_ptr++ << 16);
    return_value += (*data_ptr << 24);

    return return_value;
}


/**
 * @brief reads an OUTPUT uint8 from the iomap
 * @param slave_no
 * @param byte_no
 * @return
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
uint8_t ec_pdo_get_output_uint8_rev(uint16_t slave_no, uint8_t byte_no) {
    uint8_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].outputs;
/* Move pointer to correct byte index*/
    data_ptr += byte_no;

/* Read value byte by byte since all targets can't handle misaligned
 * addresses
 */
    return_value = *data_ptr;

    return return_value;
}


/**
 * @brief reads an OUTPUT int8 from the iomap
 * @param slave_no
 * @param byte_no
 * @return
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
int8_t ec_pdo_get_output_int8_rev(uint16_t slave_no, uint8_t byte_no) {
    int8_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].outputs;
/* Move pointer to correct byte index*/
    data_ptr += byte_no;

/* Read value byte by byte since all targets can't handle misaligned
 * addresses
 */
    return_value = (int8_t) *data_ptr;

    return return_value;
}


/**
 * @brief reads an OUTPUT int16 from the iomap
 * @param slave_no
 * @param byte_no
 * @return
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
int16_t ec_pdo_get_output_int16_rev(uint16_t slave_no, uint8_t byte_no) {
    int16_t return_value;
    uint8_t *data_ptr;
    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct byte index*/
    data_ptr += byte_no;

    /* Read value byte by byte since all targets can't handle misaligned
     * addresses
     */
    return_value = *data_ptr++;
    return_value += (*data_ptr << 8);

    return return_value;
}


/**
 * @brief sets a bit in the iomap INPUT
 * @param slave_no
 * @param bit_no
 * @param value
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND
 */
void ec_pdo_set_input_bit_rev(uint16_t slave_no, uint8_t bit_no, bool value) {
    /* Get the the startbit position in slaves IO byte */
    uint8_t startbit = ec_slave[slave_no].Istartbit;
    /* Set or Clear bit */
    if (value == false)
        *ec_slave[slave_no].inputs &= ~(1 << (bit_no - 1 + startbit));
    else
        *ec_slave[slave_no].inputs |= (1 << (bit_no - 1 + startbit));
}


void ec_pdo_set_output_bit_from_byte_slave(uint16_t slave_no, ec_datatype datatype, uint8_t byte_no, uint8_t bit_no,
                                           bool val) {
    uint32_t temp;
    switch (datatype) {
        case ECT_UNSIGNED32:
            //read
            temp = ec_pdo_get_output_uint32_rev(slave_no, byte_no);
            if (val) {
                BIT_SET(temp, bit_no);
            } else {
                BIT_CLEAR(temp, bit_no);
            }
            ec_pdo_set_output_uint32(slave_no, byte_no, temp);
            break;
        case ECT_INTEGER32:

        case ECT_UNSIGNED16:
        case ECT_INTEGER16:

        case ECT_UNSIGNED8:

        case ECT_INTEGER8:

        default:
            break;
    }

}


bool ec_pdo_get_input_bit_from_byte_slave(uint16_t slave_no, ec_datatype datatype, uint8_t byte_no, uint8_t bit_no) {
    switch (datatype) {
        case ECT_UNSIGNED32:
            if (BIT_CHECK(ec_pdo_get_input_uint32(slave_no, byte_no), bit_no)) {
                return true;
            } else {
                return false;
            }
        case ECT_INTEGER32:
            if (BIT_CHECK(ec_pdo_get_input_int32(slave_no, byte_no), bit_no)) {
                return true;
            } else {
                return false;
            }

        case ECT_UNSIGNED16:
            if (BIT_CHECK(ec_pdo_get_input_uint16(slave_no, byte_no), bit_no)) {
                return true;
            } else {
                return false;
            }
        case ECT_INTEGER16:
            if (BIT_CHECK(ec_pdo_get_input_int16(slave_no, byte_no), bit_no)) {
                return true;
            } else {
                return false;
            }

        case ECT_UNSIGNED8:
            if (BIT_CHECK(ec_pdo_get_input_uint8(slave_no, byte_no), bit_no)) {
                return true;
            } else {
                return false;
            }

        case ECT_INTEGER8:
            if (BIT_CHECK(ec_pdo_get_input_int8(slave_no, byte_no), bit_no)) {
                return true;
            } else {
                return false;
            }
        default:
            break;
    }

}


bool ec_sdo_write_uint8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t Value, bool umError) {
    int os = sizeof(Value);
    if (ec_printSDO) {
        printf("\tSlave num: %u, ", Slave);
        printf("Index: %#08x, Sub-index: %#08x, ", Index, SubIndex);
        printf("Value: %#08x\n", Value);
        return true;
    } else {
        int rc = ec_SDOwrite(Slave, Index, SubIndex, false, os, &Value, EC_TIMEOUTRXM);
        if (rc <= 0) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: Could not write SDO. Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                     Index, SubIndex, Value, Slave);
            if (umError) {
                if (ec_iserror()) {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: EtherCAT error detected after SDO write to index:0x%04x - sub-index:0x%04x - value:%d. EtherCAT error [%s]",
                             Index,
                             SubIndex, Value, ec_elist2string());
                }
            }
            return false;
        }

    }
    return true;
}

bool ec_sdo_write_int8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int8_t Value, bool umError) {
    int os = sizeof(Value);

    if (ec_printSDO) {
        printf("\tSlave num: %u, ", Slave);
        printf("Index: %#08x, Sub-index: %#08x, ", Index, SubIndex);
        printf("Value: %#08x\n", Value);
        return true;

    } else {

        int rc = ec_SDOwrite(Slave, Index, SubIndex, false, os, &Value, EC_TIMEOUTRXM);
        if (rc <= 0) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: Could not write SDO. Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                     Index, SubIndex, Value, Slave);
            if (umError) {
                if (ec_iserror()) {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: EtherCAT error detected after SDO write to index:0x%08x - sub-index:0x%08x - value:%d. EtherCAT error [%s]",
                             Index,
                             SubIndex, Value, ec_elist2string());
                }
            }
            return false;

        }

        return true;
    }
}

bool ec_sdo_write_uint16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint16_t Value, bool umError) {
    int os = sizeof(Value);
    if (ec_printSDO) {
        printf("\tSlave num: %u, ", Slave);
        printf("Index: %#08x, Sub-index: %#08x, ", Index, SubIndex);
        printf("Value: %#08x\n", Value);

    } else {

        int rc = ec_SDOwrite(Slave, Index, SubIndex, false, os, &Value, EC_TIMEOUTRXM);
        if (rc <= 0) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: Could not write SDO. Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                     Index, SubIndex, Value, Slave);
            if (umError) {
                if (ec_iserror()) {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: EtherCAT error detected after SDO write to index:0x%04x - sub-index:0x%04x - value:%d. EtherCAT error [%s]",
                             Index,
                             SubIndex, Value, ec_elist2string());

                }
            }
            return false;
        }

    }
    return true;
}

bool ec_sdo_write_int16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int16_t Value, bool umError) {
    int os = sizeof(Value);
    if (ec_printSDO) {
        printf("\tSlave num: %u, ", Slave);
        printf("Index: %#08x, Sub-index: %#08x, ", Index, SubIndex);
        printf("Value: %#08x\n", Value);

    } else {

        int rc = ec_SDOwrite(Slave, Index, SubIndex, false, os, &Value, EC_TIMEOUTRXM);
        if (rc <= 0) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: Could not write SDO. Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                     Index, SubIndex, Value, Slave);
            if (umError) {
                if (ec_iserror()) {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: EtherCAT error detected after SDO write to index:0x%08x - sub-index:0x%08x - value:%d. EtherCAT error [%s]",
                             Index,
                             SubIndex, Value, ec_elist2string());
                }
            }
            return false;
        }
    }
    return true;
}

bool ec_sdo_write_uint32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint32_t Value, bool umError) {
    int os = sizeof(Value);
    if (ec_printSDO) {
        printf("\tSlave num: %u, ", Slave);
        printf("Index: %#08x, Sub-index: %#08x, ", Index, SubIndex);
        printf("Value: %#08x\n", Value);

    } else {

        int rc = ec_SDOwrite(Slave, Index, SubIndex, false, os, &Value, EC_TIMEOUTRXM);
        if (rc <= 0) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: Could not write SDO. Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                     Index, SubIndex, Value, Slave);
            if (umError) {
                if (ec_iserror()) {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: EtherCAT error detected after SDO write to index:0x%08x - sub-index:0x%08x - value:%d. EtherCAT error [%s]",
                             Index,
                             SubIndex, Value, ec_elist2string());
                }
            }
            return false;
        }

    }
    return true;
}

bool ec_sdo_write_int32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int32_t Value, bool umError) {
    int os = sizeof(Value);
    if (ec_printSDO) {
        printf("\tSlave num: %u, ", Slave);
        printf("Index: %#08x, Sub-index: %#08x, ", Index, SubIndex);
        printf("Value: %#08x\n", Value);

    } else {

        int rc = ec_SDOwrite(Slave, Index, SubIndex, false, os, &Value, EC_TIMEOUTRXM);
        if (rc <= 0) {
            UM_ERROR(GBEM_UM_EN,
                     "GBEM: Could not write SDO.Index:0x%08x - sub-index:0x%08x - value:%d - (on slave %u) aborting",
                     Index, SubIndex, Value, Slave);
            if (umError) {
                if (ec_iserror()) {
                    UM_ERROR(GBEM_UM_EN,
                             "GBEM: EtherCAT error detected after SDO write to index:0x%08x - sub-index:0x%08x - value:%d. EtherCAT error [%s]",
                             Index,
                             SubIndex, Value, ec_elist2string());

                }
            }
            return false;
        }

    }
    return true;
}


bool ec_sdo_read_int32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int32_t *return_val, bool umError) {

    int os = sizeof(int32_t);
    int rc = ec_SDOread(Slave, Index, SubIndex, false, &os, return_val, EC_TIMEOUTRXM);

    if (rc <= 0) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x (on slave:%u)", Index,
                 SubIndex, Slave);
        if (umError) {
            if (ec_iserror()) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: EtherCAT error detected after SDO read to index:0x%04x - sub-index:0x%04x. EtherCAT error [%s]",
                         Index,
                         SubIndex, ec_elist2string());

            }
        }
        return false;
    }
    return true;
}

__attribute__(( weak )) bool
ec_sdo_read_uint32(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint32_t *return_val, bool umError) {
    int os = sizeof(uint32_t);
    int rc = ec_SDOread(Slave, Index, SubIndex, false, &os, return_val, EC_TIMEOUTRXM);

    if (rc <= 0) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x (on slave:%u)", Index,
                 SubIndex, Slave);
        if (umError) {
            if (ec_iserror()) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: EtherCAT error detected after SDO read to index:0x%04x - sub-index:0x%04x. EtherCAT error [%s]",
                         Index,
                         SubIndex, ec_elist2string());

            }
        }
        return false;
    }
    return true;
}


bool ec_sdo_read_int16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int16_t *return_val, bool umError) {
    int os = sizeof(int16_t);
    int rc = ec_SDOread(Slave, Index, SubIndex, false, &os, return_val, EC_TIMEOUTRXM);

    if (rc <= 0) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x (on slave:%u)", Index,
                 SubIndex, Slave);
        if (umError) {
            if (ec_iserror()) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: EtherCAT error detected after SDO read to index:0x%04x - sub-index:0x%04x. EtherCAT error [%s]",
                         Index,
                         SubIndex, ec_elist2string());

            }
        }
        return false;
    }
    return true;

}

bool ec_sdo_read_uint16(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint16_t *return_val, bool umError) {
    int os = sizeof(uint16_t);
    int rc = ec_SDOread(Slave, Index, SubIndex, false, &os, return_val, EC_TIMEOUTRXM);

    if (rc <= 0) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x (on slave:%u)", Index,
                 SubIndex, Slave);
        if (umError) {
            if (ec_iserror()) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: EtherCAT error detected after SDO read to index:0x%04x - sub-index:0x%04x. EtherCAT error [%s]",
                         Index,
                         SubIndex, ec_elist2string());

            }
        }
        return false;
    }
    return true;

}

bool ec_sdo_read_int8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, int8_t *return_val, bool umError) {
    int os = sizeof(int8_t);
    int rc = ec_SDOread(Slave, Index, SubIndex, false, &os, return_val, EC_TIMEOUTRXM);

    if (rc <= 0) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x (on slave:%u)", Index,
                 SubIndex, Slave);
        if (umError) {
            if (ec_iserror()) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: EtherCAT error detected after SDO read to index:0x%04x - sub-index:0x%04x. EtherCAT error [%s]",
                         Index,
                         SubIndex, ec_elist2string());

            }
        }
        return false;
    }
    return true;

}

bool ec_sdo_read_uint8(uint16_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t *return_val, bool umError) {
    int os = sizeof(uint8_t);
    int rc = ec_SDOread(Slave, Index, SubIndex, false, &os, return_val, EC_TIMEOUTRXM);

    if (rc <= 0) {
        UM_ERROR(GBEM_UM_EN, "GBEM: Could not read SDO index:0x%08x - sub-index:0x%08x (on slave:%u)", Index,
                 SubIndex, Slave);
        if (umError) {
            if (ec_iserror()) {
                UM_ERROR(GBEM_UM_EN,
                         "GBEM: EtherCAT error detected after SDO read to index:0x%04x - sub-index:0x%04x. EtherCAT error [%s]",
                         Index,
                         SubIndex, ec_elist2string());

            }
        }
        return false;
    }
    return true;

}


