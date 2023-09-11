# status and control word bits (upper 16 bits of machine word)

## Status

### STATUS_WORD_GBEM_ALIVE_BIT_NUM (16)

Set in `main.c` on startup.

Unset in cleanup handler on exit (unless SEGFAULT for something nasty).

### STAUS_WORD_GBEM_BOOT_IN_PROGRESS_BIT_NUM         (17)

Set at start of boot process. `ec_step_1_init`

Unset at end of boot process. `ec_step_9_op`

### STATUS_WORD_GBEM_BOOTED_BIT_NUM                  (18)

Set at end of boot process. `ec_step_9_op`

Unset on reboot request or in cleanup handler on exit (unless SEGFAULT for something nasty).

### STATUS_WORD_GBEM_HOMING_NEEDED_BIT_NUM           (19)

Set in `ECBoot` if any drive has map_drive_run_homing set.

### STATUS_WORD_GBEM_WAITING_FOR_START_HOMING_BIT_NUM (20)

### STATUS_WORD_GBEM_HOMING_IN_PROGRESS_BIT_NUM      (21)

### STATUS_WORD_GBEM_HOMING_ERROR_BIT_NUM            (23)

### STATUS_WORD_GBEM_HOMING_ATTAINED_BIT_NUM         (24)

## Command

### CONTROL_WORD_GBEM_REBOOT_BIT_NUM                 (16)

Used by GBR/GBC to trigger a reboot of GBEM.

### CONTROL_WORD_GBEM_START_HOMING_BIT_NUM           (17)

Used by GBR/GBC to trigger a homing process.