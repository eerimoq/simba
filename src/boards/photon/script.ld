OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

/* Memory Spaces Definitions */
MEMORY
{
        rom (rx)    : ORIGIN = 0x08020000, LENGTH = 0x00080000 /* Flash, 512K */
        ram (rwx)   : ORIGIN = 0x20000000, LENGTH = 0x00020000 /* sram, 128K */
}

INCLUDE "script.common.ld"
