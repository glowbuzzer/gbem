
/**
 ******************************************************************************
 * @file           :  error_code.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "akd_error_code.h"

//typedef struct {
//    uint32_t numeric_code,
//            akd_error_type_t type,
//    char text_code [MAX_LEN_AKD_TEXT_CODE]
//    char description [MAX_LEN_AKD_ERROR_DESC]
//}akd_error_code_description_t;


const akd_error_code_description_t akd_error_code_description[NUM_OF_AKD_ERROR_STRINGS]= {
        {.numeric_code = 0x0000, .type=AKD_ERROR, .text_code="0", .description="AKD: Emergency error free"},
        {.numeric_code = 0x1080, .type=AKD_WARNING,  .text_code="-", .description="AKD: General Warning"},
        {.numeric_code = 0x1081, .type=AKD_ERROR,  .text_code="-", .description="AKD: General Error"},
        {.numeric_code = 0x3110, .type=AKD_ERROR, .text_code="F523", .description="AKD: DC Bus link over voltage FPGA"},
        {.numeric_code = 0x3120, .type=AKD_ERROR, .text_code="F247", .description="AKD: DC Bus link voltage exceed allowed thresholds"},
        {.numeric_code = 0x3130, .type=AKD_ERROR, .text_code="F503", .description="AKD: DC Bus link capacitor overload"},
        {.numeric_code = 0x3180, .type=AKD_WARNING, .text_code="n503", .description="AKD: Warning: DC Bus link capacitor overload"},
        {.numeric_code = 0x3210, .type=AKD_ERROR, .text_code="F501", .description="AKD: DC Bus link over-voltage"},
        {.numeric_code = 0x3220, .type=AKD_ERROR, .text_code="F502", .description="AKD: DC Bus Link under-voltage"},
        {.numeric_code = 0x3280, .type=AKD_WARNING, .text_code="n502", .description="AKD: Warning: DC Bus Link under-voltage"},
        {.numeric_code = 0x3281, .type=AKD_WARNING, .text_code="n521", .description="AKD: Warning: Dynamic Braking I2T"},
        {.numeric_code = 0x3282, .type=AKD_ERROR, .text_code="F519", .description="AKD: Regen short circuit"},
        {.numeric_code = 0x3283, .type=AKD_WARNING, .text_code="n501", .description="AKD: Warning: DC Bus link over-voltage"},
        {.numeric_code = 0x4210, .type=AKD_ERROR, .text_code="F234", .description="AKD: Excessive temperature, device (control board)"},
        {.numeric_code = 0x4310, .type=AKD_ERROR, .text_code="F235", .description="AKD: Excessive temperature, drive (heat sink)"},
        {.numeric_code = 0x4380, .type=AKD_ERROR, .text_code="F236", .description="AKD: Power temperature sensor 2 high"},
        {.numeric_code = 0x4381, .type=AKD_ERROR, .text_code="F237", .description="AKD: Power temperature sensor 3 high"},
        {.numeric_code = 0x4382, .type=AKD_ERROR, .text_code="F535", .description="AKD: Power board overtemperature"},
        {.numeric_code = 0x4390, .type=AKD_WARNING, .text_code="n234", .description="AKD: Warning: Control temperature sensor 1 high"},
        {.numeric_code = 0x4391, .type=AKD_WARNING, .text_code="n235", .description="AKD: Warning: Power temperature sensor 1 high"},
        {.numeric_code = 0x4392, .type=AKD_WARNING, .text_code="n236", .description="AKD: Warning: Power temperature sensor 2 high"},
        {.numeric_code = 0x4393, .type=AKD_WARNING, .text_code="n237", .description="AKD: Warning: Power temperature sensor 3 high"},
        {.numeric_code = 0x4394, .type=AKD_WARNING, .text_code="n240", .description="AKD: Warning: Control temperature sensor 1 low"},
        {.numeric_code = 0x4395, .type=AKD_WARNING, .text_code="n241", .description="AKD: Warning: Power temperature sensor 1 low"},
        {.numeric_code = 0x4396, .type=AKD_WARNING, .text_code="n242", .description="AKD: Warning: Power temperature sensor 2 low"},
        {.numeric_code = 0x4397, .type=AKD_WARNING, .text_code="n243", .description="AKD: Warning: Control temperature sensor 1 low"},
        {.numeric_code = 0x4398, .type=AKD_ERROR, .text_code="F240", .description="AKD: Control temperature sensor 1 low"},
        {.numeric_code = 0x4399, .type=AKD_ERROR, .text_code="F241", .description="AKD: Power temperature sensor 1 low"},
        {.numeric_code = 0x439A, .type=AKD_ERROR, .text_code="F242", .description="AKD: Power temperature sensor 2 low"},
        {.numeric_code = 0x439B, .type=AKD_ERROR, .text_code="F243", .description="AKD: Power temperature sensor 3 low"},
        {.numeric_code = 0x5113, .type=AKD_ERROR, .text_code="F512", .description="AKD: 5V0 under voltage"},
        {.numeric_code = 0x5114, .type=AKD_ERROR, .text_code="F505", .description="AKD: 1V2 under voltage"},
        {.numeric_code = 0x5115, .type=AKD_ERROR, .text_code="F507", .description="AKD: 2V5 under voltage"},
        {.numeric_code = 0x5116, .type=AKD_ERROR, .text_code="F509", .description="AKD: 3V3 under voltage"},
        {.numeric_code = 0x5117, .type=AKD_ERROR, .text_code="F514", .description="AKD: +12V0 under voltage"},
        {.numeric_code = 0x5118, .type=AKD_ERROR, .text_code="F516", .description="AKD: -12V0 under voltage"},
        {.numeric_code = 0x5119, .type=AKD_ERROR, .text_code="F518", .description="AKD: Analog 3V3 under voltage"},
        {.numeric_code = 0x5180, .type=AKD_ERROR, .text_code="F504", .description="AKD: 1V2 over voltage"},
        {.numeric_code = 0x5181, .type=AKD_ERROR, .text_code="F506", .description="AKD: 2V5 over voltage"},
        {.numeric_code = 0x5182, .type=AKD_ERROR, .text_code="F508", .description="AKD: 3V3 over voltage"},
        {.numeric_code = 0x5183, .type=AKD_ERROR, .text_code="F510", .description="AKD: 5V0 over voltage"},
        {.numeric_code = 0x5184, .type=AKD_ERROR, .text_code="F513", .description="AKD: +12V0 over voltage"},
        {.numeric_code = 0x5185, .type=AKD_ERROR, .text_code="F515", .description="AKD: -12V0 over voltage"},
        {.numeric_code = 0x5186, .type=AKD_ERROR, .text_code="F517", .description="AKD: Analog 3V3 over voltage"},
        {.numeric_code = 0x5530, .type=AKD_ERROR, .text_code="F105", .description="AKD: Hardware memory, non-volatile memory stamp invalid"},
        {.numeric_code = 0x5580, .type=AKD_ERROR, .text_code="F106", .description="AKD: Hardware memory, non-volatile memory data"},
        {.numeric_code = 0x5589, .type=AKD_ERROR, .text_code="F124", .description="AKD: Cogging compensation non volatile memory data error (CRC)"},
        {.numeric_code = 0x5590, .type=AKD_ERROR, .text_code="F204", .description="AKD: Control board EEPROM read failed"},
        {.numeric_code = 0x5591, .type=AKD_ERROR, .text_code="F205", .description="AKD: Control board EEPROM corrupted serial num stamp"},
        {.numeric_code = 0x5592, .type=AKD_ERROR, .text_code="F206", .description="AKD: Control board EEPROM corrupted serial num data"},
        {.numeric_code = 0x5593, .type=AKD_ERROR, .text_code="F207", .description="AKD: Control board EEPROM corrupted parameter stamp"},
        {.numeric_code = 0x5594, .type=AKD_ERROR, .text_code="F208", .description="AKD: Control board EEPROM corrupted parameter data"},
        {.numeric_code = 0x5595, .type=AKD_ERROR, .text_code="F219", .description="AKD: Control board EEPROM write failed"},
        {.numeric_code = 0x55A0, .type=AKD_ERROR, .text_code="F209", .description="AKD: Power board EEPROM read failed"},
        {.numeric_code = 0x55A1, .type=AKD_ERROR, .text_code="F210", .description="AKD: Power board EEPROM corrupted serial num stamp"},
        {.numeric_code = 0x55A2, .type=AKD_ERROR, .text_code="F212", .description="AKD: Power board EEPROM corrupted serial num data"},
        {.numeric_code = 0x55A3, .type=AKD_ERROR, .text_code="F213", .description="AKD: Power board EEPROM corrupted parameter stamp"},
        {.numeric_code = 0x55A4, .type=AKD_ERROR, .text_code="F214", .description="AKD: Power board EEPROM corrupted parameter data"},
        {.numeric_code = 0x55A5, .type=AKD_ERROR, .text_code="F230", .description="AKD: Power board EEPROM write failed"},
        {.numeric_code = 0x55A6, .type=AKD_ERROR, .text_code="F232", .description="AKD: Power board EEPROM invalid data"},
        {.numeric_code = 0x55B0, .type=AKD_ERROR, .text_code="F248", .description="AKD: Option board EEPROM corrupted"},
        {.numeric_code = 0x55B1, .type=AKD_ERROR, .text_code="F249", .description="AKD: Option board upstream checksum"},
        {.numeric_code = 0x55B2, .type=AKD_ERROR, .text_code="F250", .description="AKD: Option board upstream checksum"},
        {.numeric_code = 0x55B3, .type=AKD_ERROR, .text_code="F251", .description="AKD: Option board watchdog"},
        {.numeric_code = 0x55B8, .type=AKD_ERROR, .text_code="F252", .description="AKD: Firmware and option board FPGA types are not compatible"},
        {.numeric_code = 0x55B9, .type=AKD_ERROR, .text_code="F253", .description="AKD: Firmware and option boardFPGA versions are not compatible"},
        {.numeric_code = 0x55C0, .type=AKD_ERROR, .text_code="F621", .description="AKD: Control Board CRC fault"},
        {.numeric_code = 0x55C1, .type=AKD_ERROR, .text_code="F623", .description="AKD: Power Board CRC fault"},
        {.numeric_code = 0x55C2, .type=AKD_ERROR, .text_code="F624", .description="AKD: Power Board Watchdog fault"},
        {.numeric_code = 0x55C3, .type=AKD_ERROR, .text_code="F625", .description="AKD: Power Board Communication fault"},
        {.numeric_code = 0x55C4, .type=AKD_ERROR, .text_code="F626", .description="AKD: Power Board FPGA not configured"},
        {.numeric_code = 0x55C5, .type=AKD_ERROR, .text_code="F627", .description="AKD: Control Board Watchdog fault"},
        {.numeric_code = 0x55C6, .type=AKD_WARNING, .text_code="n103", .description="AKD: Warning: Resident FPGA"},
        {.numeric_code = 0x55C7, .type=AKD_WARNING, .text_code="n104", .description="AKD: Warning: Operational FPGA"},
        {.numeric_code = 0x6080, .type=AKD_ERROR, .text_code="F631", .description="AKD: Issue command timed out"},
        {.numeric_code = 0x6380, .type=AKD_ERROR, .text_code="F532", .description="AKD: Drive motor parameters setup incomplete"},
        {.numeric_code = 0x6381, .type=AKD_ERROR, .text_code="F120", .description="AKD: Failed to set default parameters"},
        {.numeric_code = 0x7180, .type=AKD_ERROR, .text_code="F301", .description="AKD: Motor overheat"},
        {.numeric_code = 0x7182, .type=AKD_ERROR, .text_code="F305", .description="AKD: Motor Brake open circuit"},
        {.numeric_code = 0x7183, .type=AKD_ERROR, .text_code="F306", .description="AKD: Motor Brake short circuit"},
        {.numeric_code = 0x7184, .type=AKD_ERROR, .text_code="F307", .description="AKD: Motor Brake applied during enable state"},
        {.numeric_code = 0x7185, .type=AKD_ERROR, .text_code="F436", .description="AKD: EnDAT overheated"},
        {.numeric_code = 0x7186, .type=AKD_WARNING, .text_code="n301", .description="AKD: Warning: Motor overheated"},
        {.numeric_code = 0x7187, .type=AKD_ERROR, .text_code="F308", .description="AKD: Voltage exceeds motor rating"},
        {.numeric_code = 0x7188, .type=AKD_ERROR, .text_code="F560", .description="AKD: Regen near capacity, could not prevent over voltage"},
        {.numeric_code = 0x7189, .type=AKD_ERROR, .text_code="F312", .description="AKD: Brake released when it should be applied"},
        {.numeric_code = 0x7305, .type=AKD_ERROR, .text_code="F417", .description="AKD: Broken wire in primary feedback"},
        {.numeric_code = 0x7380, .type=AKD_ERROR, .text_code="F402", .description="AKD: Feedback 1 Analog signal amplitudefault"},
        {.numeric_code = 0x7381, .type=AKD_ERROR, .text_code="F403", .description="AKD: Feedback 1 EnDat communication fault"},
        {.numeric_code = 0x7382, .type=AKD_ERROR, .text_code="F404", .description="AKD: Feedback 1 illegal hall state"},
        {.numeric_code = 0x7383, .type=AKD_ERROR, .text_code="F405", .description="AKD: Feedback 1 BiSS watchdog"},
        {.numeric_code = 0x7384, .type=AKD_ERROR, .text_code="F406", .description="AKD: Feedback 1 BiSS multi cycle"},
        {.numeric_code = 0x7385, .type=AKD_ERROR, .text_code="F407", .description="AKD: Feedback 1 BiSS sensor"},
        {.numeric_code = 0x7386, .type=AKD_ERROR, .text_code="F408", .description="AKD: Feedback 1 SFD configuration"},
        {.numeric_code = 0x7387, .type=AKD_ERROR, .text_code="F409", .description="AKD: Feedback 1 SFD UART overrun"},
        {.numeric_code = 0x7388, .type=AKD_ERROR, .text_code="F410", .description="AKD: Feedback 1 SFD UART frame"},
        {.numeric_code = 0x7389, .type=AKD_ERROR, .text_code="F412", .description="AKD: Feedback 1 SFD UART parity"},
        {.numeric_code = 0x738A, .type=AKD_ERROR, .text_code="F413", .description="AKD: Feedback 1 SFD transfer timeout"},
        {.numeric_code = 0x738C, .type=AKD_ERROR, .text_code="F415", .description="AKD: Feedback 1 SFD mult. corrupt position"},
        {.numeric_code = 0x738D, .type=AKD_ERROR, .text_code="F416", .description="AKD: Feedback 1 SFD Transfer incomplete"},
        {.numeric_code = 0x738E, .type=AKD_ERROR, .text_code="F418", .description="AKD: Feedback 1 power supply fault"},
        {.numeric_code = 0x738F, .type=AKD_ERROR, .text_code="F401", .description="AKD: Feedback 1 failed to set feedback"},
        {.numeric_code = 0x7390, .type=AKD_WARNING, .text_code="n414", .description="AKD: Warning: SFD single corrupted position"},
        {.numeric_code = 0x7391, .type=AKD_ERROR, .text_code="F419", .description="AKD: Encoder init procedure failed"},
        {.numeric_code = 0x7392, .type=AKD_ERROR, .text_code="F534", .description="AKD: Failed to read motor parameters from feedback device"},
        {.numeric_code = 0x7393, .type=AKD_ERROR, .text_code="F421", .description="AKD: SFD position sensor fault"},
        {.numeric_code = 0x7394, .type=AKD_ERROR, .text_code="F463", .description="AKD: Tamagawa encoder: overheat"},
        {.numeric_code = 0x7395, .type=AKD_WARNING, .text_code="n451", .description="AKD: Warning: Tamagawa encoder battery"},
        {.numeric_code = 0x7396, .type=AKD_WARNING, .text_code="n423", .description="AKD: Warning: Non volatile memory error, multiturn overflow"},
        {.numeric_code = 0x7397, .type=AKD_ERROR, .text_code="F471", .description="AKD: Operation in Position Mode with Halls Only feedback not allowed"},
        {.numeric_code = 0x7398, .type=AKD_ERROR, .text_code="F135", .description="AKD: Homing is needed"},
        {.numeric_code = 0x7399, .type=AKD_ERROR, .text_code="F468", .description="AKD: FB2.Source not set"},
        {.numeric_code = 0x739A, .type=AKD_ERROR, .text_code="F469", .description="AKD: FB1.ENCRES is not power of two"},
        {.numeric_code = 0x739B, .type=AKD_ERROR, .text_code="F423", .description="AKD: Non volatile memory error, multiturn overflow"},
        {.numeric_code = 0x739C, .type=AKD_ERROR, .text_code="F467", .description="AKD: Hiperface DSL fault"},
        {.numeric_code = 0x739D, .type=AKD_ERROR, .text_code="F452", .description="AKD: Multiturn overflow not supported with this feedback"},
        {.numeric_code = 0x739E, .type=AKD_ERROR, .text_code="F465", .description="AKD: Excessive shock detected by feedback device"},
        {.numeric_code = 0x73A0, .type=AKD_ERROR, .text_code="F453", .description="AKD: Tamagawa encoder: communication timeout"},
        {.numeric_code = 0x73A1, .type=AKD_ERROR, .text_code="F454", .description="AKD: Tamagawa encoder: communication transfer incomplete"},
        {.numeric_code = 0x73A2, .type=AKD_ERROR, .text_code="F456", .description="AKD: Tamagawa encoder: communication CRC"},
        {.numeric_code = 0x73A3, .type=AKD_ERROR, .text_code="F457", .description="AKD: Tamagawa encoder: communication start timeout"},
        {.numeric_code = 0x73A4, .type=AKD_ERROR, .text_code="F458", .description="AKD: Tamagawa encoder: communication UART overrun"},
        {.numeric_code = 0x73A5, .type=AKD_ERROR, .text_code="F459", .description="AKD: Tamagawa encoder: communication UART framing"},
        {.numeric_code = 0x73A6, .type=AKD_ERROR, .text_code="F460", .description="AKD: Tamagawa encoder: over speed"},
        {.numeric_code = 0x73A7, .type=AKD_ERROR, .text_code="F461", .description="AKD: Tamagawa encoder: contouring error"},
        {.numeric_code = 0x73A8, .type=AKD_ERROR, .text_code="F462", .description="AKD: Tamagawa encoder: counting overflow"},
        {.numeric_code = 0x73A9, .type=AKD_ERROR, .text_code="F464", .description="AKD: Tamagawa encoder: multiturn error"},
        {.numeric_code = 0x73AA, .type=AKD_ERROR, .text_code="F451", .description="AKD: Tamagawa encoder: battery"},
        {.numeric_code = 0x73B0, .type=AKD_ERROR, .text_code="F486", .description="AKD: Motor velocity exceeds emulated encoder maximum speed"},
        {.numeric_code = 0x73B8, .type=AKD_ERROR, .text_code="F420", .description="AKD: FB3 EnDat communication fault"},
        {.numeric_code = 0x73C0, .type=AKD_ERROR, .text_code="F473", .description="AKD: Wake and Shake. Insufficient movement"},
        {.numeric_code = 0x73C1, .type=AKD_ERROR, .text_code="F475", .description="AKD: Wake and Shake. Excessive movement"},
        {.numeric_code = 0x73C2, .type=AKD_ERROR, .text_code="F476", .description="AKD: Wake and Shake Fine-coarse delta too large"},
        {.numeric_code = 0x73C3, .type=AKD_ERROR, .text_code="F478", .description="AKD: Wake and Shake. Overspeed"},
        {.numeric_code = 0x73C4, .type=AKD_ERROR, .text_code="F479", .description="AKD: Wake and Shake. Loop angle delta too large"},
        {.numeric_code = 0x73C5, .type=AKD_ERROR, .text_code="F482", .description="AKD: Commutation not initialized"},
        {.numeric_code = 0x73C6, .type=AKD_ERROR, .text_code="F483", .description="AKD: Motor U phase missing"},
        {.numeric_code = 0x73C7, .type=AKD_ERROR, .text_code="F484", .description="AKD: Motor V phase missing"},
        {.numeric_code = 0x73C8, .type=AKD_ERROR, .text_code="F485", .description="AKD: Motor W phase missing"},
        {.numeric_code = 0x73C9, .type=AKD_WARNING, .text_code="n478", .description="AKD: Warning: Wake and Shake. Overspeed"},
        {.numeric_code = 0x73D0, .type=AKD_ERROR, .text_code="F487", .description="AKD: Wake and Shake. Validating positive movement failed"},
        {.numeric_code = 0x73D1, .type=AKD_ERROR, .text_code="F489", .description="AKD: Wake and Shake. Validating negative movement failed"},
        {.numeric_code = 0x73D2, .type=AKD_ERROR, .text_code="F490", .description="AKD: Wake and Shake. Validating commutation angle time out"},
        {.numeric_code = 0x73D3, .type=AKD_ERROR, .text_code="F491", .description="AKD: Wake and Shake. Validating commutation angle moved too far"},
        {.numeric_code = 0x73D4, .type=AKD_ERROR, .text_code="F492", .description="AKD: Wake and Shake. Validating commutation angle required more than MOTOR.ICONT"},
        {.numeric_code = 0x73D5, .type=AKD_ERROR, .text_code="F493", .description="AKD: Invalid commutation detected, motor accelerates in wrong direction"},
        {.numeric_code = 0x8130, .type=AKD_ERROR, .text_code="F129", .description="AKD: Life Guard Error or Heartbeat Error"},
        {.numeric_code = 0x8180, .type=AKD_WARNING, .text_code="n702", .description="AKD: Warning: Fieldbus communication lost"},
        {.numeric_code = 0x8280, .type=AKD_WARNING, .text_code="n601", .description="AKD: Warning: Modbus data rate is too high"},
        {.numeric_code = 0x8311, .type=AKD_ERROR, .text_code="F304", .description="AKD: Motor foldback"},
        {.numeric_code = 0x8331, .type=AKD_ERROR, .text_code="F524", .description="AKD: Drive foldback"},
        {.numeric_code = 0x8380, .type=AKD_WARNING, .text_code="n524", .description="AKD: Warning: Drive foldback"},
        {.numeric_code = 0x8381, .type=AKD_WARNING, .text_code="n304", .description="AKD: Warning: Motor foldback"},
        {.numeric_code = 0x8382, .type=AKD_WARNING, .text_code="n309", .description="AKD: Warning: Motor I2t load"},
        {.numeric_code = 0x8383, .type=AKD_WARNING, .text_code="n580", .description="AKD: Warning: Using derivate of position when using sensorless feedback type in position mode"},
        {.numeric_code = 0x8384, .type=AKD_WARNING, .text_code="n581", .description="AKD: Warning: Zero velocity when using induction sensorless feedback type in position mode"},
        {.numeric_code = 0x8385, .type=AKD_WARNING, .text_code="n495", .description="AKD: Warning: Failed to process recorder cogging compensation table"},
        {.numeric_code = 0x8480, .type=AKD_ERROR, .text_code="F302", .description="AKD: Over speed"},
        {.numeric_code = 0x8481, .type=AKD_ERROR, .text_code="F703", .description="AKD: Emergency timeout occurred while axis should disable"},
        {.numeric_code = 0x8482, .type=AKD_ERROR, .text_code="F480", .description="AKD: Fieldbus command velocity too high"},
        {.numeric_code = 0x8483, .type=AKD_ERROR, .text_code="F481", .description="AKD: Fieldbus command velocity too low"},
        {.numeric_code = 0x8582, .type=AKD_WARNING, .text_code="n107", .description="AKD: Warning: Positive software position limit is exceeded"},
        {.numeric_code = 0x8583, .type=AKD_WARNING, .text_code="n108", .description="AKD: Warning: Negative software position limit is exceeded"},
        {.numeric_code = 0x8611, .type=AKD_ERROR, .text_code="F439", .description="AKD: Following error (user)"},
        {.numeric_code = 0x8685, .type=AKD_ERROR, .text_code="F138", .description="AKD: Instability during autotune"},
        {.numeric_code = 0x8686, .type=AKD_WARNING, .text_code="n151", .description="AKD: Warning: Not enough distance to move; Motion Exception"},
        {.numeric_code = 0x8687, .type=AKD_WARNING, .text_code="n152", .description="AKD: Warning: Not enough distance to move; Following Motion Exception"},
        {.numeric_code = 0x8688, .type=AKD_WARNING, .text_code="n153", .description="AKD: Warning: Velocity Limit Violation, Exceeding Max Limit"},
        {.numeric_code = 0x8689, .type=AKD_WARNING, .text_code="n154", .description="AKD: Warning: Following Motion Failed; Check Motion Parameters"},
        {.numeric_code = 0x868A, .type=AKD_WARNING, .text_code="n156", .description="AKD: Warning: Target Position crossed due to Stop command"},
        {.numeric_code = 0x86A0, .type=AKD_WARNING, .text_code="n157", .description="AKD: Warning: Homing Index pulse not found"},
        {.numeric_code = 0x86A1, .type=AKD_WARNING, .text_code="n158", .description="AKD: Warning: Homing Reference Switch not found"},
        {.numeric_code = 0x86A2, .type=AKD_WARNING, .text_code="n159", .description="AKD: Warning: Failed to set motion task parameters"},
        {.numeric_code = 0x86A3, .type=AKD_WARNING, .text_code="n160", .description="AKD: Warning: Motion Task Activation Failed"},
        {.numeric_code = 0x86A4, .type=AKD_WARNING, .text_code="n161", .description="AKD: Warning: Homing Procedure Failed"},
        {.numeric_code = 0x86A5, .type=AKD_WARNING, .text_code="F139", .description="AKD: Target Position Over Short due to invalid Motion task activation"},
        {.numeric_code = 0x86A6, .type=AKD_WARNING, .text_code="n163", .description="AKD: Warning: MT.NUM exceeds limit"},
        {.numeric_code = 0x86A7, .type=AKD_WARNING, .text_code="n164", .description="AKD: Warning: Motion task is not initialized"},
        {.numeric_code = 0x86A8, .type=AKD_WARNING, .text_code="n165", .description="AKD: Warning: Motion task target position is out"},
        {.numeric_code = 0x86A9, .type=AKD_WARNING, .text_code="n167", .description="AKD: Warning: Software limit switch traversed"},
        {.numeric_code = 0x86AA, .type=AKD_WARNING, .text_code="n168", .description="AKD: Warning: Invalid bit combination in the motion task control word"},
        {.numeric_code = 0x86AB, .type=AKD_WARNING, .text_code="n169", .description="AKD: Warning: 1:1 profile cannot be triggered on the fly"},
        {.numeric_code = 0x86AC, .type=AKD_WARNING, .text_code="n170", .description="AKD: Warning: Customer profile table is not initialized"},
        {.numeric_code = 0x86AD, .type=AKD_WARNING, .text_code="n171", .description="AKD: Warning: Motion task activation is currently pending"},
        {.numeric_code = 0x86AE, .type=AKD_WARNING, .text_code="n135", .description="AKD: Warning: Homing is needed"},
        {.numeric_code = 0x86AF, .type=AKD_WARNING, .text_code="n174", .description="AKD: Warning: Homing maximum distance exceede"},
        {.numeric_code = 0x86B0, .type=AKD_ERROR, .text_code="F438", .description="AKD: Following error (numeric)"},
        {.numeric_code = 0x86B6, .type=AKD_WARNING, .text_code="n179", .description="AKD: Teaching of Cogging compensation stopped before finishing"},
        {.numeric_code = 0x86B7, .type=AKD_WARNING, .text_code="n180", .description="AKD: Cogging compensation not active. Axis needs to be homed first"},
        {.numeric_code = 0x8780, .type=AKD_ERROR, .text_code="F125", .description="AKD: Fieldbus synchronization lost"},
        {.numeric_code = 0x8781, .type=AKD_WARNING, .text_code="n125", .description="AKD: Warning: Fieldbus synchronization lost"},
        {.numeric_code = 0x8AF0, .type=AKD_WARNING, .text_code="n137", .description="AKD: Warning: Homing and feedback mismatch"},
        {.numeric_code = 0xFF00, .type=AKD_ERROR, .text_code="F701", .description="AKD: Fieldbus runtime"},
        {.numeric_code = 0xFF01, .type=AKD_ERROR, .text_code="F702", .description="AKD: Fieldbus communication lost"},
        {.numeric_code = 0xFF02, .type=AKD_ERROR, .text_code="F529", .description="AKD: Iu current offset limit exceeded"},
        {.numeric_code = 0xFF03, .type=AKD_ERROR, .text_code="F530", .description="AKD: Iv current offset limit exceeded"},
        {.numeric_code = 0xFF04, .type=AKD_ERROR, .text_code="F521", .description="AKD: Regen over power"},
        {.numeric_code = 0xFF07, .type=AKD_ERROR, .text_code="F525", .description="AKD: Output over current"},
        {.numeric_code = 0xFF08, .type=AKD_ERROR, .text_code="F526", .description="AKD: Current sensor short circuit"},
        {.numeric_code = 0xFF09, .type=AKD_ERROR, .text_code="F128", .description="AKD: MPOLES/FPOLES not an integer"},
        {.numeric_code = 0xFF0A, .type=AKD_ERROR, .text_code="F531", .description="AKD: Power stage fault"},
        {.numeric_code = 0xFF0B, .type=AKD_ERROR, .text_code="F602", .description="AKD: Safe torque off"},
        {.numeric_code = 0xFF0C, .type=AKD_ERROR, .text_code="F131", .description="AKD: Secondary feedback A/B line break"},
        {.numeric_code = 0xFF0D, .type=AKD_ERROR, .text_code="F130", .description="AKD: Secondary feedback supply over current"},
        {.numeric_code = 0xFF0E, .type=AKD_ERROR, .text_code="F134", .description="AKD: Secondary feedback illegal state"},
        {.numeric_code = 0xFF0F, .type=AKD_ERROR, .text_code="F245", .description="AKD: External fault"},
        {.numeric_code = 0xFF10, .type=AKD_ERROR, .text_code="F136", .description="AKD: Firmware and FPGA versions are not compatible"},
        {.numeric_code = 0xFF11, .type=AKD_ERROR, .text_code="F101", .description="AKD: Firmware type mismatch"},
        {.numeric_code = 0xFF12, .type=AKD_WARNING, .text_code="n439", .description="AKD: Warning: Following error (user)"},
        {.numeric_code = 0xFF13, .type=AKD_WARNING, .text_code="n438", .description="AKD: Warning: Following error (numeric)"},
        {.numeric_code = 0xFF14, .type=AKD_WARNING, .text_code="n102", .description="AKD: Warning: Operational FPGA is not a default FPGA"},
        {.numeric_code = 0xFF15, .type=AKD_WARNING, .text_code="n101", .description="AKD: Warning: The FPGA is a laboratory FPGA"},
        {.numeric_code = 0xFF16, .type=AKD_WARNING, .text_code="n602", .description="AKD: Warning: Safe torque off"},
        {.numeric_code = 0xFF17, .type=AKD_ERROR, .text_code="F132", .description="AKD: Secondary feedback Z line break"},
        {.numeric_code = 0xFF18, .type=AKD_WARNING, .text_code="n603", .description="AKD: Warning: OPMODE incompatible with CMDSOURCE"},
        {.numeric_code = 0xFF19, .type=AKD_WARNING, .text_code="n604", .description="AKD: Warning: EMUEMODE incompatible with DRV.HANDWHEELSRC"}
};