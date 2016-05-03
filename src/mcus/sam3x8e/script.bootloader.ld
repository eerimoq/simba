/**
 * Linker script for a bootloader with 64 kB flash and 96 kB SRAM.
 */

/* Memory Spaces Definitions */
MEMORY
{
        rom (rx)    : ORIGIN = 0x00080000, LENGTH = 0x00010000
        ram (rwx)   : ORIGIN = 0x20070000, LENGTH = 0x00018000
}

INCLUDE "script.common.ld"
