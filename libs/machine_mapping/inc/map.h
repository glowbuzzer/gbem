/**
 ******************************************************************************
 * @file           :  map.h
 * @brief          :  Slave, drive, plc, gbc mapping
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef INC_MAP_H_
#define INC_MAP_H_

#include <stdbool.h>
#include "ethercat.h"
#include "gberror.h"
#include "map_config.h"


/*
 *
 * Comments highlight where things to be:
 * MACHINE_NEW
 * MACHINE_SWAP
 * DRIVE_NEW
 *
 */


extern bool nolimits;

/** These macros build the function pointers for slave and drive startup and operational functions */

/* SLAVES */
#define MAP_NUM_DRIVES_ATTACHED(...) const uint8_t map_num_drives_attached[MAP_NUM_SLAVES] = {__VA_ARGS__};
#define MAP_SLAVE_PDO_MAPPING_FUNCTIONS(...) gberror_t (*map_slave_pdo_map_function_ptr[MAP_NUM_SLAVES])(uint16_t slave) = {__VA_ARGS__};
#define MAP_SLAVE_NVRAM_SDO_FUNCTIONS(...) gberror_t (*map_slave_nvram_sdo_function_ptr[MAP_NUM_SLAVES])(uint16_t slave) =  {__VA_ARGS__};
#define MAP_SLAVE_STANDARD_SDO_FUNCTIONS(...) gberror_t (*map_slave_standard_sdo_function_ptr[MAP_NUM_SLAVES])(uint16_t slave) =  {__VA_ARGS__};
#define MAP_SLAVE_INITIAL_PDO_FUNCTIONS(...)  gberror_t (*map_slave_initial_pdo_function_ptr[MAP_NUM_SLAVES])(uint16_t slave) =  {__VA_ARGS__};
#define MAP_SLAVE_DC_TYPE(...) const map_slave_dc_type_t map_dc_type[MAP_NUM_SLAVES] = {__VA_ARGS__};
#define MAP_SLAVE_DC_CYCLE(...) const int8_t map_dc_cycle[MAP_NUM_SLAVES] = {__VA_ARGS__};
#define MAP_SLAVE_EEP(...) const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES] = {__VA_ARGS__};

/* DRIVES */
#define MAP_DRIVE_TO_NAME(...) const char map_drive_to_name[MAP_NUM_DRIVES][MAX_DRIVE_NAME_LENGTH] = {__VA_ARGS__};
#define MAP_DRIVE_TO_SLAVE(...) const uint16_t map_drive_to_slave[MAP_NUM_DRIVES] = {__VA_ARGS__};
#define MAP_DRIVE_TO_SUBDRIVE(...) const uint16_t map_drive_to_subdrive[MAP_NUM_DRIVES] = {__VA_ARGS__};
#define MAP_DRIVE_GET_MOO_PDO_FUNCTIONS(...) int8_t (*map_drive_get_moo_pdo_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_GET_MOO_SDO_FUNCTIONS(...) int8_t (*map_drive_get_moo_sdo_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_GET_REMOTE_FUNCTIONS(...)  bool (*map_drive_get_remote_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};

#define MAP_DRIVE_GET_FOLLOW_ERROR_FUNCTIONS(...) bool (*map_drive_get_follow_error_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_GET_ERROR_STRING_PDO_FUNCTIONS(...)  uint8_t *(*map_drive_get_error_string_pdo_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_GET_ERROR_STRING_SDO_FUNCTIONS(...) uint8_t *(*map_drive_get_error_string_sdo_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_SET_CTRL_WRD_FUNCTIONS(...) gberror_t (*map_drive_set_ctrl_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive, uint16_t ctrlwrd) = {__VA_ARGS__};
#define MAP_DRIVE_GET_STAT_WRD_FUNCTIONS(...) uint16_t (*map_drive_get_stat_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_GET_ACTPOS_WRD_FUNCTIONS(...) int32_t (*map_drive_get_actpos_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_GET_ACTVEL_WRD_FUNCTIONS(...) int32_t (*map_drive_get_actvel_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_GET_ACTTORQ_WRD_FUNCTIONS(...) int16_t (*map_drive_get_acttorq_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};

#define MAP_DRIVE_SET_SETPOS_WRD_FUNCTIONS(...) gberror_t (*map_drive_set_setpos_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive, int32_t setpos) = {__VA_ARGS__};

#define MAP_DRIVE_HOMING_EXEC_FUNCTIONS(...) gberror_t (*map_drive_homing_exec_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};

#define MAP_DRIVE_POS_LIMIT(...) const int32_t map_drive_pos_limit[MAP_NUM_DRIVES] = {__VA_ARGS__};
#define MAP_DRIVE_NEG_LIMIT(...) const int32_t map_drive_neg_limit[MAP_NUM_DRIVES] = {__VA_ARGS__};
#define MAP_DRIVE_DIRECTION(...) const uint8_t map_drive_direction[MAP_NUM_DRIVES] = {__VA_ARGS__};
#define MAP_DRIVE_RUN_HOMING(...) const bool map_drive_run_homing[MAP_NUM_DRIVES] = {__VA_ARGS__};
#define MAP_DRIVE_PRINT_PARAMS_FUNCTIONS(...) gberror_t (*map_drive_print_params_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};

//REVERSE FUNCTIONS
#define MAP_DRIVE_GET_CTRL_WRD_REV_FUNCTIONS(...) uint16_t (*map_drive_get_ctrl_wrd_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_SET_STAT_WRD_REV_FUNCTIONS(...) gberror_t (*map_drive_set_stat_wrd_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive, uint16_t statwrd) = {__VA_ARGS__};
#define MAP_DRIVE_SET_ACTPOS_WRD_REV_FUNCTIONS(...) gberror_t (*map_drive_set_actpos_wrd_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive, int32_t actpos) = {__VA_ARGS__};
#define MAP_DRIVE_GET_SETPOS_WRD_REV_FUNCTIONS(...) int32_t (*map_drive_get_setpos_wrd_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};
#define MAP_DRIVE_SET_MOO_REV_FUNCTIONS(...) gberror_t (*map_drive_set_moo_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive) = {__VA_ARGS__};


typedef struct {
    /** Slave name from EEprom */
    char name[EC_MAXNAME + 1];
    /** Manufacturer from EEprom */
    uint32_t eep_man;
    /** ID from EEprom */
    uint32_t eep_id;
    /** revision from EEprom */
    uint32_t eep_rev;
} map_slave_map_t;

typedef enum {
    EC_DC_UNCONFIGURED,
    EC_DC_NONE,
    EC_DC_0,
    EC_DC_01,
} map_slave_dc_type_t;

/* for fixed PDO mapping */
typedef struct {
    uint8_t number_of_entries;
    uint16_t SM_assignment_index;
} map_SM_assignment_object_t;


/* for flexible PDO mapping */

typedef struct {
    uint8_t number_of_entries;
    uint32_t PDO_assignment_index;
} map_PDO_configuration_object_t;

typedef union {
    bool ect_boolean;
    uint8_t ect_unsigned8;
    int8_t ect_integer8;
    uint16_t ect_unsigned16;
    int16_t ect_integer16;
    uint32_t ect_unsigned32;
    int32_t ect_integer32;
    float ect_real32;
    double ect_real64;
    uint64_t ect_unsigned64;
    int64_t ect_integer64;
} ec_datatype_value_t;

typedef enum {
    MAP_UNDEFINED,
    MAP_IN,
    MAP_OUT,
} map_inout_t;

typedef struct {
    const map_inout_t inout;
    const uint16_t slave_num;
    const uint32_t byte_num;
    const uint8_t bit_num;
    const ec_datatype_t datatype;
    const double max_val; //this is used for float to int conversions
} map_pdo_object_t;

typedef struct {
    const map_inout_t inout;
    void *io;
    const ec_datatype_t datatype;
    const char linked_task_name[20];
    uint8_t private_linked_task_index;
} map_plc_object_t;

typedef struct {
    bool active;
    ec_datatype_value_t val;
} map_force_object_t;

typedef struct {
    const map_inout_t inout;
    const ec_datatype_t datatype;
    const uint16_t ionum;
} map_gbc_object_t;

/** struct for iomap */
typedef struct {
    map_pdo_object_t pdo;
    map_gbc_object_t gbc;
    map_plc_object_t plc;
    map_force_object_t force;
} mapping_t;

extern const map_slave_map_t ecm_slave_map[MAP_NUM_SLAVES];
extern map_machine_type_t map_machine_type;

//SLAVES
extern const uint8_t map_num_drives_attached[MAP_NUM_SLAVES];

extern gberror_t (*map_slave_pdo_map_function_ptr[MAP_NUM_SLAVES])(uint16_t slave);

extern gberror_t (*map_slave_nvram_sdo_function_ptr[MAP_NUM_SLAVES])(uint16_t slave);

extern gberror_t (*map_slave_standard_sdo_function_ptr[MAP_NUM_SLAVES])(uint16_t slave);

extern gberror_t (*map_slave_initial_pdo_function_ptr[MAP_NUM_SLAVES])(uint16_t slave);

extern const map_slave_dc_type_t map_dc_type[MAP_NUM_SLAVES];
extern const int8_t map_dc_cycle[MAP_NUM_SLAVES];


//DRIVES
extern const char map_drive_to_name[MAP_NUM_DRIVES][MAX_DRIVE_NAME_LENGTH];
extern const uint16_t map_drive_to_slave[MAP_NUM_DRIVES];
extern const uint16_t map_drive_to_subdrive[MAP_NUM_DRIVES];

extern int8_t (*map_drive_get_moo_pdo_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern int8_t (*map_drive_get_moo_sdo_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern bool (*map_drive_get_follow_error_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern bool (*map_drive_get_remote_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern uint8_t *(*map_drive_get_error_string_pdo_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern uint8_t *(*map_drive_get_error_string_sdo_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern gberror_t (*map_drive_set_ctrl_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive, uint16_t ctrlwrd);

extern uint16_t (*map_drive_get_stat_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern int32_t (*map_drive_get_actpos_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern int32_t (*map_drive_get_actvel_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern int16_t (*map_drive_get_acttorq_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern gberror_t (*map_drive_set_setpos_wrd_function_ptr[MAP_NUM_DRIVES])(uint16_t drive, int32_t setpos);

extern gberror_t (*map_drive_print_params_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern const bool map_drive_run_homing[MAP_NUM_DRIVES];

extern gberror_t (*map_drive_homing_exec_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern const int32_t map_drive_pos_limit[MAP_NUM_DRIVES];
extern const int32_t map_drive_neg_limit[MAP_NUM_DRIVES];
extern const uint8_t map_drive_direction[MAP_NUM_DRIVES];

//REVERSE FUNCTIONS
extern uint16_t (*map_drive_get_ctrl_wrd_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern gberror_t (*map_drive_set_stat_wrd_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive, uint16_t statwrd);

extern gberror_t (*map_drive_set_actpos_wrd_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive, int32_t actpos);

extern int32_t (*map_drive_get_setpos_wrd_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

extern gberror_t (*map_drive_set_moo_rev_function_ptr[MAP_NUM_DRIVES])(uint16_t drive);

//externs for iomap and its size
extern mapping_t map_iomap[];
extern uint16_t map_num_rows_in_iomap;

//standard functions
bool ec_is_warning(void);

bool ec_check_for_follow_error(gberror_t *gbc);

bool ec_check_for_internal_limit(gberror_t *gbc);

bool ec_check_remote(void);

//functions to look-up objects in the iomap
gberror_t map_get_gbc_iomap_row(map_inout_t inout, uint16_t ionum, ec_datatype_t datatype, mapping_t **row);

uint16_t map_get_gbc_iomap_slave(map_inout_t inout, uint16_t ionum, ec_datatype_t datatype);

uint32_t map_get_gbc_iomap_byte_num(map_inout_t inout, uint16_t ionum, ec_datatype_t datatype);

uint8_t map_get_gbc_iomap_bit_num(map_inout_t inout, uint16_t ionum, ec_datatype_t datatype);


//functions to check for validity of iomap objects
bool map_is_valid_gbc_datatype(ec_datatype_t type);

bool map_is_valid_pdo_datatype(ec_datatype_t type);

bool map_is_valid_plc_datatype(ec_datatype_t type);


extern map_pdo_object_t ctrl_estop_din;
extern map_pdo_object_t ctrl_estop_reset_din;

#endif /* INC_MAP_H_ */
