/**
 ******************************************************************************
 * @file           :  aw_j_series.c
 * @brief          :  common functions for all AutomationWare j-series drives
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "aw_j_series.h"
#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "user_message.h"
#include "ethercatsetget.h"
#include "cia402.h"


/**
 * @brief get the value of the remote bit from an AW-J-series drive
 * @param drive
 * @return true remote bit is set (ok) false not set
 */
bool ec_get_remote_aw_j_series(const uint16_t drive) {
    //remote bit not implemented on AW-J-series drives
    return true;
}

/**
 * @brief gets the mode of operation (MOO) for AW-J-series drives
 * @param drive_number
 * @return  moo value (8 for CSP)
 * @attention handles sub-drives
 */
int8_t ec_get_moo_pdo_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_input_int8(map_drive_to_slave[drive], AW_J_SERIES_MOODISP_PDO_INDEX);
}


/**
 * @brief gets the status of the follow error bit for AW-J-series drives
 * @param drive
 * @return
 */
bool ec_get_follow_error_aw_j_series(const uint16_t drive) {
    uint16_t drive_stat_wrd;

    drive_stat_wrd = ec_get_stat_wrd_aw_j_series(drive);

    if (BIT_CHECK(drive_stat_wrd, CIA_FOLLOW_ERROR_BIT_NUM)) {
        return true;
    }
    return false;
}


/**
 * @brief Perform first cycle PDO writes for AW-J-series drives
 * @param slave - slave number to send PDO writes
 * @retval E_SUCCESS - Normal
 * @attention
 */
gberror_t ec_initial_pdo_aw_j_series(const uint16_t slave) {

    ec_pdo_set_output_int8(slave, AW_J_SERIES_MOOSET_PDO_INDEX, map_drive_moo[map_slave_to_drive(slave)]);

    UM_INFO(GBEM_UM_EN,
            "GBEM: Setting MOO with PDO write for AW-J-Series drive slave [%u], drive, [%u], offset [%u], value [%u]",
            slave, map_drive_to_slave[slave],
            AW_J_SERIES_MOOSET_PDO_INDEX, map_drive_moo[map_slave_to_drive(slave)]);

    return E_SUCCESS;
}


/**
 * @brief get actpos for an AW-J-Series drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actpos_wrd_aw_j_series(const uint16_t drive) {

    return ec_pdo_get_input_int32(map_drive_to_slave[drive], AW_J_SERIES_ACTPOS_PDO_INDEX);

}

/**
 * @brief get actvel for an AW-J-Series drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_actvel_wrd_aw_j_series(const uint16_t drive) {

    return ec_pdo_get_input_int32(map_drive_to_slave[drive], AW_J_SERIES_ACTVEL_PDO_INDEX);

}

/**
 * @brief get acttorq for an AW-J-Series drive
 * @param drive
 * @return int32 position
 */
int32_t ec_get_acttorq_wrd_aw_j_series(const uint16_t drive) {

    return ec_pdo_get_input_int16(map_drive_to_slave[drive], AW_J_SERIES_ACTTORQ_PDO_INDEX);

}


/**
 * @brief get ctrlwrd for for an AW-J-series drive
 * @param drive
 * @return ctrlwrd
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
uint16_t ec_get_ctrl_wrd_rev_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_output_uint16_rev(map_drive_to_slave[drive], AW_J_SERIES_CONTROLWORD_PDO_INDEX);
}


/**
 * @brief set control word for AW-J-series drive
 * @param drive
 * @param ctrlwrd
 * @return gberror
 */
gberror_t ec_set_ctrl_wrd_aw_j_series(const uint16_t drive, const uint16_t ctrlwrd) {

    ec_pdo_set_output_uint16(map_drive_to_slave[drive], AW_J_SERIES_CONTROLWORD_PDO_INDEX, ctrlwrd);

    return E_SUCCESS;
}

/**
 * @brief set status word for AW-J-series drive
 * @param drive
 * @return status word

 */
uint16_t ec_get_stat_wrd_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_input_uint16(map_drive_to_slave[drive], AW_J_SERIES_STATUSWORD_PDO_INDEX);

}

/**
 * @brief set setpos for an AW-J-series drive
 * @param drive
 * @param setpos
 * @return gberror
 */
gberror_t ec_set_setpos_wrd_aw_j_series(const uint16_t drive, const int32_t setpos) {

    ec_pdo_set_output_int32(map_drive_to_slave[drive], AW_J_SERIES_SETPOS_PDO_INDEX, setpos);
    return E_SUCCESS;
}

/**
 * @brief set setvel for an AW-J-series drive
 * @param drive
 * @param setvel
 * @return gberror
 */
gberror_t ec_set_setvel_wrd_aw_j_series(const uint16_t drive, const int32_t setvel) {

    ec_pdo_set_output_int32(map_drive_to_slave[drive], AW_J_SERIES_SETVEL_PDO_INDEX, setvel);
    return E_SUCCESS;
}

/**
 * @brief set settorq for an AW-J-series drive
 * @param drive
 * @param settorq
 * @return gberror
 */
gberror_t ec_set_settorq_wrd_aw_j_series(const uint16_t drive, const int32_t settorq) {

    ec_pdo_set_output_int32(map_drive_to_slave[drive], AW_J_SERIES_SETTORQ_PDO_INDEX, settorq);
    return E_SUCCESS;
}


uint8_t *ec_get_error_string_sdo_aw_j_series(const uint16_t drive) {
    static uint8_t error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH];
    memset(&error_code_string[0], 0, sizeof(uint8_t) * MAX_DRIVE_ERROR_MSG_LENGTH);
    uint16_t drive_error_code = 0;

    char no_error_prefix[] = "AW-J-Series: no error on drive. Detailed error report";

    if (ec_sdo_read_uint16(map_drive_to_slave[drive], AW_J_SERIES_ERROR_CODE_SDO_INDEX,
                           AW_J_SERIES_ERROR_CODE_SDO_SUB_INDEX,
                           &drive_error_code, false)) {

        if (drive_error_code == 0) {

            snprintf((char *) error_code_string, MAX_DRIVE_ERROR_MSG_LENGTH,
                     "%s [%s]", no_error_prefix,
                     ec_get_detailled_error_report_sdo_aw_j_series(drive));
            return error_code_string;
        }

        char error_code_first[] = "AW-J-Series error code";
        char error_code_second[] = "Detailed error report";

        for (int i = 0; i < NUM_OF_AW_J_SERIES_ERROR_STRINGS; i++) {
            if (aw_j_series_error[i].error_id == drive_error_code) {

                snprintf((char *) error_code_string, MAX_DRIVE_ERROR_MSG_LENGTH,
                         "%s [%s] %s [%s]", error_code_first, aw_j_series_error[i].text_string, error_code_second,
                         ec_get_detailled_error_report_sdo_aw_j_series(drive));

                error_code_string[MAX_DRIVE_ERROR_MSG_LENGTH - 1] = '\0';
                return error_code_string;
            }
        }

        snprintf((char *) error_code_string, MAX_DRIVE_ERROR_MSG_LENGTH,
                 "AW-J-Series: error code returned by drive did not match any in the error table. Detailed error report [%s]",
                 ec_get_detailled_error_report_sdo_aw_j_series(drive));
        return error_code_string;
    }

    sprintf((char *) error_code_string,
            "AW-J-Series error code could not be read from drive");
    return error_code_string;
}

uint8_t *ec_get_detailled_error_report_sdo_aw_j_series(const uint16_t drive_number) {
    int os = 8;
    uint8_t octet_string[8];

    static uint8_t failed_error_code_read[50] = "AW-J-Series: Failed to read error code";
    static uint8_t failed_error_code_lookup[50] = "AW-J-Series: Error code does not match table";

    int rc = ec_SDOread(map_drive_to_slave[drive_number], AW_J_SERIES_ERROR_DESCRIPTION_SDO_INDEX,
                        AW_J_SERIES_ERROR_DESCRIPTION_SDO_SUB_INDEX, false, &os,
                        &octet_string, EC_TIMEOUTRXM);
    if (rc <= 0) {
        return failed_error_code_read;

    }

    for (int i = 0; i < NUM_OF_AW_J_SERIES_ERROR_REPORT_STRINGS; i++) {
        if (strcmp(aw_j_series_error_report[i].error_code, (char *) octet_string) != 0) {
            return (uint8_t *) aw_j_series_error_report[i].text_string;
        }
    }

    return failed_error_code_lookup;

}

/**
* @brief set status word for an AW-J-series drive
* @param drive
* @param statwrd
* @return gberror
* @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
*/
gberror_t ec_set_stat_wrd_rev_aw_j_series(const uint16_t drive, const uint16_t statwrd) {
    ec_pdo_set_input_uint16_rev(map_drive_to_slave[drive], AW_J_SERIES_STATUSWORD_PDO_INDEX, statwrd);
    return E_SUCCESS;
}


/**
 * @brief set actpos for an AW-J-Series drive
 * @param drive
 * @param actpos
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_actpos_wrd_rev_aw_j_series(const uint16_t drive, const int32_t actpos) {
    ec_pdo_set_input_int32_rev(map_drive_to_slave[drive], AW_J_SERIES_ACTPOS_PDO_INDEX, actpos);
    return E_SUCCESS;
}

/**
 * @brief get setpos for an AW-J-Series drive
 * @param drive
 * @return setpos
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
int32_t ec_get_setpos_word_rev_aw_j_series(const uint16_t drive) {
    return ec_pdo_get_output_int32_rev(map_drive_to_slave[drive], AW_J_SERIES_SETPOS_PDO_INDEX);

}


/**
 * @brief set moo for an AW-J-Series drive
 * @param drive
 * @return gberror
 * @warning REVERSE FUNCTION ("WRONG" WAY ROUND)
 */
gberror_t ec_set_moo_pdo_rev_aw_j_series(const uint16_t drive) {
    ec_pdo_set_input_int8_rev(map_drive_to_slave[drive], AW_J_SERIES_MOODISP_PDO_INDEX, CIA_MOO_CSP);
    return E_SUCCESS;
}


const aw_j_series_error_report_string_t aw_j_series_error_report[NUM_OF_AW_J_SERIES_ERROR_REPORT_STRINGS] = {
        {"NoFault",  "No fault"},
        {"PuOcA",    "Protection User-defined Over-current Phase A"},
        {"PuOcB",    "Protection User-defined Over-current Phase B"},
        {"PuOcC",    "Protection User-defined Over-current Phase C"},
        {"PuUv",     "Protection User-defined Under-voltage"},
        {"PuOv",     "Protection User-defined Over-voltage"},
        {"PhUdef",   "Protection Hardware Undefined"},
        {"PhFault",  "Protection Hardware Fault"},
        {"PhWtdg",   "Protection Hardware Watchdog"},
        {"PhDeadTA", "Protection Hardware Deadtime Phase A"},
        {"PhDeadTB", "Protection Hardware Deadtime Phase B"},
        {"PhDeadTC", "Protection Hardware Deadtime Phase C"},
        {"PhDeadTD", "Protection Hardware Deadtime Phase D"},
        {"PhOvUvOt", "Protection Hardware Over-voltage / Under-voltage / Over-temperature"},
        {"PhOc",     "Protection Hardware Overcurrent"},
        {"SfeDiIvd", "Safety Digital Input Invalid"},
        {"SfeFault", "Safety Fault"},
        {"BisErBit", "BiSS Error Bit active"},
        {"BisWnBit", "BiSS Warning Bit active"},
        {"BisAcBit", "BiSS Acknowledge Bit not received"},
        {"BisSrtBt", "BiSS Start Bit not received"},
        {"BisSloLo", "BiSS data (SLO) is permanently Low"},
        {"BisFrame", "BiSS Frame configuration error"},
        {"BisCrc",   "BiSS CRC error"},
        {"BisRegEr", "BiSS register communication error"},
        {"SsiFrame", "SSI Frame configuration error"},
        {"SsiDtaLo", "SSI Data is permanently Low"},
        {"SsiChksm", "SSI Checksum error"},
        {"R16WkMgF", "REM16 Weak Magnetic Field"},
        {"R16Cont",  "REM16 multiturn Counter error"},
        {"R16CorDc", "REM16 singleturn CORDIC error"},
        {"R16SpdOv", "REM16 multiturn Speed Overflow"},
        {"R16FtCfg", "REM16 Filter Configuration error"},
        {"R16FtSOF", "REM16 Filter Speed Overflow error"},
        {"R16UnCmd", "REM16 Unknown Command"},
        {"R16Chksm", "REM16 Checksum error"},
        {"R14Chksm", "REM14 Checksum error"},
        {"QeiLsTck", "QEI Lost Ticks"},
        {"QeiNoIdx", "QEI No Index"},
        {"QeiOpnWr", "QEI Open Wire"},
        {"HallSeq",  "Hall Sequence error"},
        {"AftFrame", "A-Format Frame error"},
        {"AftTmout", "A-Format Timeout"},
        {"AftCrc",   "A-Format CRC error"},
        {"AftBatt",  "A-Format BATT status"},
        {"AftMTErr", "A-Format MTERR status"},
        {"AftOvFlw", "A-Format OvFlow status"},
        {"AftOvSpd", "A-Format OVSPD status"},
        {"AftMemEr", "A-Format MEMERR status"},
        {"AftSTErr", "A-Format STERR status"},
        {"AftPSErr", "A-Format PSERR status"},
        {"AftBusyF", "A-Format BUSY status"},
        {"AftMemBs", "A-Format MemBusy status"},
        {"AftOvTmp", "A-Format OvTemp status"},
        {"AftIncEr", "A-Format IncErr status"},
        {"SnsrAngl", "Sensor Angle error"},
        {"SnsrCfg",  "Sensor Configuration error"},
        {"HwRsrcEr", "Hardware Resource Error"},
        {"IvldGpio", "Invalid Gpio configuration"},
        {"SnsrBatt", "Sensor battery error"},
        {"SkpCycls", "Serial encoder service is skipping cycles"},
        {"SwLimOut", "Software Limit Out"},
        {"BrkNtRls", "Brake release failed"},
        {"BkHiPull", "Pull brake voltage too high"},
        {"BkHiHold", "Hold brake voltage too high"},
        {"OpnTermA", "Open Terminal A"},
        {"OpnTermB", "Open Terminal B"},
        {"OpnTermC", "Open Terminal C"},
        {"OpnFetAH", "Open high FET at phase A"},
        {"OpnFetBH", "Open high FET at phase B"},
        {"OpnFetCH", "Open high FET at phase C"},
        {"OpnFetAL", "Open low FET at phase A"},
        {"OpnFetBL", "Open low FET at phase B"},
        {"OpnFetCL", "Open low FET at phase C"},
        {"IvldOfst", "Invalid offset"},
        {"IvldOpmd", "Invalid operational mode"},
        {"ZeroMxI",  "Max. current is zero or negative"},
        {"ZeroMxT",  "Max. torque is zero or negative"},
        {"ZeroMxMS", "Max. motor speed is zero or negative"},
        {"IvldPara", "Invalid Parameter"},
        {"SiVelPfx", "Invalid velocity scaling factor"},
        {"MxPwrLmt", "Maximum mechanical output power of the motor is reached"},
        {"CyclicHb", "Cyclic heartbeat"},
        {"OsCmdCol", "OS command collision"},
        {"I2tActv",  "I2t protection is active"},
        {"HmInvMth", "Unsupported or invalid homing method"},
        {"Intern01", "Internal fault 1"},
        {"Intern02", "Internal fault 2"},
        {"Intern03", "Internal fault 3"},
        {"VeFiFcLo", "Velocity filter cut-off frequency too low"},
        {"VeFiFcHi", "Velocity filter cut-off frequency too high"},
        {"PoFiFcLo", "Position filter cut-off frequency too low"},
        {"PoFiFcHi", "Position filter cut-off frequency too high"},
        {"VffFcLo",  "Velocity feed forward filter cut-off frequency too low"},
        {"VffFcHi",  "Velocity feed forward filter cut-off frequency too high"},
        {"NhFiFcHi", "Notch filter center frequency too high"},
        {"NhFiPara", "Invalid Notch filter parameters"},
        {"SynDifHi", "Sync difference too high"},
};

/*array mapping an AW-J-Series drive detailed error reports to a text string */
const aw_j_series_error_string_t aw_j_series_error[NUM_OF_AW_J_SERIES_ERROR_STRINGS] = {
        {0x2220, "Continuous over current (device internal)"},
        {0x2250, "Short circuit (device internal)"},
        {0x2350, "Load level fault (I2t, thermal state)"},
        {0x2351, "Load level warning (I2t, thermal state)"},
        {0x3130, "Phase failure"},
        {0x3131, "Phase failure L1"},
        {0x3132, "Phase failure L2"},
        {0x3133, "Phase failure L3"},
        {0x3210, "DC link over-voltage"},
        {0x3220, "DC link under-voltage"},
        {0x3331, "Field circuit interrupted"},
        {0x4210, "Excess temperature device"},
        {0x4310, "Excess temperature drive"},
        {0x5200, "Control"},
        {0x5300, "Operating unit"},
        {0x6010, "Software reset (watchdog)"},
        {0x6320, "Parameter error"},
        {0x7121, "Motor blocked"},
        {0x7300, "Sensor"},
        {0x7303, "Resolver 1 fault"},
        {0x7304, "Resolver 2 fault"},
        {0x7500, "Communication"},
        {0x8611, "Positioning controller (following error)"},
        {0x8612, "Positioning controller (reference limit)"},
        {0xF002, "Sub-synchronous run"},
        {0xFF00, "Manufacturer-specific"}
};