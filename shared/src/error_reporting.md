# Boot stage errors

During boot, we check for SDO write errors during boot and pop them with `ec_elist2string` and output them as a user
messages.

# Post boot errors

After boot, once we are up and running we have two error threads running:

`ec_read_error_message`
Created at end of boot
Guarded so runs only XXXX
Reads drive errors with SDO read functions and ??

> this could be split so slave errors are read by a thread and drive errors are read by another thread.


`ec_check`
Created at end of boot

Guarded by `EC_RXTX_MODE_OP` `EC_RXTX_MODE_HOMING`
Focussed on lost slaves and restarting slaves

Reads error messages from slaves and pops them with `ec_elist2string` in function `ctrl_copy_slave_error_to_ecm_status`.

We have:

`event_data.slave_reported_error = EcatError;` in `control.c` which triggers and error if the global error flag is set (
any slave
reporting and error)

The popping of the errors clears this flag

# Initial PDO writes

Performed on first run of state machine - when state is set to `EC_RXTX_MODE_OP` at end of boot.

The ec_check thread will not have been created then but will pick up the errors when it does start.

So no ec_iserror() calls are needed in drive functions.

