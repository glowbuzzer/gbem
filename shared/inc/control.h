/**
 ******************************************************************************
 * @file           :  control.h
 * @brief          :  main control functions - runs drive state machine
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "stateMachine.h"

#include "cia402.h"
#include "shared.h"


/** sm event data (passed in with each call to execute the state machine ) */
typedef struct {
    bool follow_error;
    bool machine_request_error; //bit 6 of machine controlword
    bool estop;
    bool heartbeat_lost;
    bool any_drive_has_alarm;
    bool internal_limit;
    bool remote_ok;
    bool slave_reported_error;
    bool plc_signalled_error;
    bool homing_failed;
    int8_t moo_disp[MAP_NUM_DRIVES];
    bool ec_check_error;
    uint32_t fault_cause;
} event_data_t;



struct stateMachine* ctrl_statemachine_alloc(void);
void ctrl_statemachinefree (struct stateMachine *smachine);
void ctrl_init_statemachine(struct stateMachine *smachine);
void ctrl_main(struct stateMachine *m, bool first_run);


/* sm helper functions */
void ctrl_change_all_drives_states(uint16_t controlword);
bool cia_is_fault_condition(struct event *event);
bool ctrl_check_all_drives_state(cia_state_t state);
bool ctrl_check_any_drives_state(cia_state_t state);
bool ctrl_check_all_drives_commands_sim(cia_commands_t command);
void ec_push_circular_slave_error_message(ec_circular_slave_error_message_t *c, uint8_t *slave_error_message);
uint8_t *ec_peek_circular_slave_error_message(ec_circular_slave_error_message_t *c, uint8_t slave_error_message_number);
void ctrl_copy_slave_error_to_ecm_status(void);

extern event_data_t event_data;
extern uint32_t gbem_heartbeat ;
extern bool estop;
extern cia_state_t current_state;
extern uint32_t ctrl_state_change_cycle_count;
extern uint32_t ctrl_state_change_timeout;


void ctrl_process_iomap_out(bool zero);
void ctrl_process_iomap_in(void);



#endif /* INC_CONTROL_H_ */
