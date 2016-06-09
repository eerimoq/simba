OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

/* Memory Spaces Definitions */
MEMORY
{
        rom (rx)    : ORIGIN = 0x08000000, LENGTH = 0x00020000 /* Flash, 128K */
        ram (rwx)   : ORIGIN = 0x20000000, LENGTH = 0x00002000 /* sram, 8K */
}

INCLUDE "script.common.ld"
