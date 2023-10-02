# PLC functionality

## Enable

`#define ENABLE_PLC                                      1`

`plc_user_code/plc_init.c`

In this file you create tasks. E.g.

`plc_register_tasks(plc_mytask1, 12, 1, "Task1");`

'plc_mytask1' is the function name

`12` is cycle time

`1` is priority

`"Task1"` is name

## plc functions

Need `PLC_TASK_START` macro at start of function

Need `PLC_TASK_END` macro at end of function

## iomappping

to map in machine map
create matching vars in plc task code

## plc standard functions





