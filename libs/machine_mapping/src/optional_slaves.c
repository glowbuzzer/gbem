/**
 ******************************************************************************
 * @file           :  optional_slaves
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "std_headers.h"
#include "map.h"
#include "gberror.h"
#include "optional_slaves.h"
#include <stdio.h>
#include "gbem_config.h"
#include "user_message.h"

// typedef enum {
//  MAP_ARRAY_TYPE_FUNCTION_PRT,
//  MAP_ARRAY_TYPE_UINT8_T,
// } map_array_types_t;


/**
 * \brief
 * \param array
 * \param element_size size of each element in the array
 * \param size num elements in the array
 * \param index index of the element to be deleted
 * \return
 */
gberror_t os_delete_and_shift(void *array, size_t element_size, size_t size, size_t index) {
 if (index >= size) {
  // Index out of bounds, handle the error as needed
  return E_TOO_LONG;
 }

 // Calculate the byte offset of the element to be deleted
 size_t offset = index * element_size;

 // Shift the remaining elements left
 void *start = (char *) array + offset;
 void *end = (char *) array + ((size - 1) * element_size);


 for (size_t i = 0; i < (end - start); ++i) {
  // memcpy(start + i, start + i + element_size, element_size);
  memmove(start + i, start + i + element_size, (element_size));
 }

 memset(end, 0, element_size);

 return E_SUCCESS;
}

// Function to delete and shift rows based on slave_num
// void delete_and_shift_iomap(mapping_t *array, int *size, int targetSlaveNum) {
//  int newSize = 0;
//
//  // Iterate through the array
//  for (int i = 0; i < *size; ++i) {
//   // Check if the current row has the target slave_num
//   if (array[i].pdo.slave_num != targetSlaveNum) {
//    // If not, copy the row to the new array
//    array[newSize] = array[i];
//    newSize++;
//   }
//   // If the slave_num matches, skip the row (delete)
//  }
//
//  // Update the size of the array
//  *size = newSize;
// }


void test(void) {
 // Delete the element at index 2 from the array
 os_delete_and_shift((void *) map_slave_standard_sdo_function_ptr, sizeof(gberror_t (*)(uint16_t)), 5, 2);
 for (int i = 0; i < 5; i++) {
  printf("[%d] ", map_num_drives_attached[i]);
 }
 printf("\n");

 // Delete the element at index 2 from the array
 os_delete_and_shift((void *) map_num_drives_attached, sizeof(uint8_t), 5, 2);

 for (int i = 0; i < 5; i++) {
  printf("[%d] ", map_num_drives_attached[i]);
 }
 printf("\n");
}


void os_get_array_of_optional_slaves(machine_config_optional_slaves_t config, bool *optional_slave) {
 if (config.enable_optional_slave_1 == true) {
  optional_slave[0] = true;
 }
 if (config.enable_optional_slave_2 == true) {
  optional_slave[1] = true;
 }
 if (config.enable_optional_slave_3 == true) {
  optional_slave[2] = true;
 }
 if (config.enable_optional_slave_4 == true) {
  optional_slave[3] = true;
 }
 if (config.enable_optional_slave_5 == true) {
  optional_slave[4] = true;
 }
 if (config.enable_optional_slave_6 == true) {
  optional_slave[5] = true;
 }
 if (config.enable_optional_slave_7 == true) {
  optional_slave[6] = true;
 }
 if (config.enable_optional_slave_8 == true) {
  optional_slave[7] = true;
 }
 if (config.enable_optional_slave_9 == true) {
  optional_slave[8] = true;
 }
 if (config.enable_optional_slave_10 == true) {
  optional_slave[9] = true;
 }
}


gberror_t os_disable_slaves(bool *which_slaves_are_optional) {
 uint8_t num_optional_slaves = 0;
 uint8_t num_slaves_to_be_enabled_from_config = 0;

 for (uint8_t i = 0; i < MAP_NUM_SLAVES; i++) {
  if (which_slaves_are_optional[i] == true) {
   num_optional_slaves++;
  }
 }

 //this contains the array of slaves that must be enabled - 0 -10
 bool enable_slaves_from_config[MAX_NUM_OPTIONAL_SLAVES] = {false};

 os_get_array_of_optional_slaves(machine_config_optional_slaves, enable_slaves_from_config);

 for (uint8_t i = 0; i < MAX_NUM_OPTIONAL_SLAVES; i++) {
  if (enable_slaves_from_config[i] == true) {
   num_slaves_to_be_enabled_from_config++;
  }
 }
 if (num_optional_slaves < num_slaves_to_be_enabled_from_config) {
  UM_FATAL(
   "GBEM: The number of optional slaves in the machine config ini  file [%u] is greater than the number of optional slaves allowed in the machine [%u]",
   num_slaves_to_be_enabled_from_config, num_optional_slaves);
 }

 UM_INFO(GBEM_UM_EN, "GBEM: Number of optional slaves that need to be >enabled< is [%u]",
         num_slaves_to_be_enabled_from_config);
 UM_INFO(GBEM_UM_EN, "GBEM: Number of optional slaves that need to be >disabled< is [%u]",
         num_optional_slaves - num_slaves_to_be_enabled_from_config);


 uint8_t count_of_deleted = 0;
 uint8_t count_of_config = 0;
 for (uint8_t i = 0; i < MAP_NUM_SLAVES; i++) {
  if (which_slaves_are_optional[i] == true) {
   UM_INFO(GBEM_UM_EN, "GBEM: Slave [%u] [%s] is optional for [%s]", i+1, ecm_slave_map[i].name,
           map_machine_type_strings[map_machine_type]);

   if (enable_slaves_from_config[count_of_config] == false) {
    UM_INFO(GBEM_UM_EN,
            "GBEM: Slave [%u] [%s] is >disabled< in the machine config ini file [%s] for [%s] (or no machine config ini has been provided)",
            i+1,
            ecm_slave_map[i].name,
            MACHINE_CONFIG_INI_FILENAME,
            map_machine_type_strings[map_machine_type]);

    // MAP_NUM_DRIVES_ATTACHED
    os_delete_and_shift(map_num_drives_attached, sizeof(uint8_t), map_num_slaves, i - count_of_deleted);
    // MAP_SLAVE_PDO_MAPPING_FUNCTIONS
    os_delete_and_shift(map_slave_pdo_map_function_ptr, sizeof(gberror_t (*)(uint16_t)), map_num_slaves,
                        i - count_of_deleted);
    // MAP_SLAVE_NVRAM_SDO_FUNCTIONS
    os_delete_and_shift(map_slave_nvram_sdo_function_ptr, sizeof(gberror_t (*)(uint16_t)), map_num_slaves,
                        i - count_of_deleted);
    // MAP_SLAVE_STANDARD_SDO_FUNCTIONS
    os_delete_and_shift(map_slave_standard_sdo_function_ptr, sizeof(gberror_t (*)(uint16_t)), map_num_slaves,
                        i - count_of_deleted);
    // MAP_SLAVE_CUSTOM_FMMU_SM_FUNCTIONS
    os_delete_and_shift(map_slave_custom_fmmu_sm_function_ptr, sizeof(gberror_t (*)(uint16_t)), map_num_slaves,
                        i - count_of_deleted);
    // MAP_SLAVE_DC_TYPE
    os_delete_and_shift(map_dc_type, sizeof(map_slave_dc_type_t), map_num_slaves, i - count_of_deleted);
    // MAP_SLAVE_DC_CYCLE
    os_delete_and_shift(map_dc_cycle, sizeof(int8_t), map_num_slaves, i - count_of_deleted);
    // MAP_SLAVE_EXEC_FUNCTIONS
    os_delete_and_shift(map_slave_exec_function_ptr, sizeof(gberror_t (*)(uint16_t)), map_num_slaves,
                        i - count_of_deleted);

    // MAP_SLAVE_OPTIONAL
    os_delete_and_shift(map_slave_optional, sizeof(bool), map_num_slaves, i - count_of_deleted);


    // MAP_SLAVE_FSOE_SLAVE_TYPE
    os_delete_and_shift(map_slave_fsoe_slave_type, sizeof(enum FSOE_SLAVE_TYPE), map_num_slaves, i - count_of_deleted);
    // MAP_SLAVE_FSOE_SLAVE_FUNCTION
    os_delete_and_shift(map_slave_fsoe_function, sizeof(map_slave_fsoe_function_t), map_num_slaves,
                        i - count_of_deleted);

    // MAP_SLAVE_FSOE_OFFSET_IN
    os_delete_and_shift(map_slave_fsoe_offset_in, sizeof(uint16_t), map_num_slaves, i - count_of_deleted);
    // MAP_SLAVE_FSOE_OFFSET_OUT
    os_delete_and_shift(map_slave_fsoe_offset_out, sizeof(uint16_t), map_num_slaves, i - count_of_deleted);
    // MAP_SLAVE_FSOE_GET_SLAVE_STATE_FUNCTIONS
    os_delete_and_shift(map_slave_fsoe_get_slave_state_function_ptr,
                        sizeof(gberror_t (*)(uint16_t slave, uint32_t *state,
                                             enum FSOE_SLAVE_HIGH_LEVEL_STATE *high_level_state)), map_num_slaves,
                        i - count_of_deleted);
    // MAP_SLAVE_FSOE_GET_SLAVE_CON_ID_FUNCTIONS
    os_delete_and_shift(map_slave_fsoe_get_slave_con_id_function_ptr,
                        sizeof(gberror_t (*)(uint16_t slave, uint32_t *con_id)), map_num_slaves,
                        i - count_of_deleted);

    // MAP_SLAVE_FSOE_GET_MASTER_STATE_FUNCTIONS
    os_delete_and_shift(map_slave_fsoe_get_master_state_function_ptr,
                        sizeof(gberror_t (*)(uint16_t slave, enum FSOE_MASTER_HIGH_LEVEL_STATE *high_level_state,
                                             uint32_t *error_code)), map_num_slaves,
                        i - count_of_deleted);

    os_delete_and_shift(ecm_slave_map, sizeof(map_slave_map_t), map_num_slaves, i - count_of_deleted);


    count_of_deleted++;
    map_num_slaves--;
   } else {
    UM_INFO(GBEM_UM_EN, "GBEM: Slave [%u] [%s] is >enabled< in the machine config ini file [%s] for [%s]", i+1,
            ecm_slave_map[i].name,
            MACHINE_CONFIG_INI_FILENAME,
            map_machine_type_strings[map_machine_type]);
   }
   count_of_config++;
  }
 }

 for (uint16_t drive = 0; drive < MAP_NUM_DRIVES; drive++) {
  map_drive_to_slave[drive] = map_drive_to_slave[drive] - count_of_deleted;
 }


 UM_INFO(GBEM_UM_EN, "GBEM: Number of slaves disabled [%u]", count_of_deleted);
 UM_INFO(GBEM_UM_EN, "GBEM: Number of slaves remaining enabled [%u] (originally [%u])", map_num_slaves, MAP_NUM_SLAVES);


 //process io map


 return E_SUCCESS;
}
