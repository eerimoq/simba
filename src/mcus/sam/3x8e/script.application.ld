/**
 * Linker script for an application with 448 kB flash and 96 kB
 * SRAM. To be called from the bootloader.
 */

/* Memory Spaces Definitions */
MEMORY
{
        rom (rx)    : ORIGIN = 0x00090000, LENGTH = 0x00070000
        ram (rwx)   : ORIGIN = 0x20070000, LENGTH = 0x00018000
}

INCLUDE "script.common.ld"
