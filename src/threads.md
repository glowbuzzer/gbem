# List of threads

## `thread_ec_check` - function is: `ec_check`

Checks for lost slaves

Copies slave state to shared memory (pops from error list)

## `thread_ec_error_message` - function is:`ec_read_error_messages`

When machine state is FAULT/FAULT_REACTION_ACTIVE reads error messages from drives (SDO)

## `thread_ec_reboot` - function is: `ec_reboot`

Sits waiting for `CONTROL_WORD_GBEM_REBOOT_BIT_NUM` to be set in status word.

Then kills threads and call main.

## `thread_ec_rxtx`  function is: `ec_rxtx`

Real-time thread where all the action takes place.

## PLC threads - created one per PLC task

Runs PLC tasks