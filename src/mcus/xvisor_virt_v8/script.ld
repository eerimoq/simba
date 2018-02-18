OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH("aarch64")
SEARCH_DIR(.)

/* Memory Spaces Definitions */
MEMORY
{
        rom (rx)    : ORIGIN = 0x00000000, LENGTH = 0x08000000 /* Flash, 128M */
        ram (rwx)   : ORIGIN = 0x40000000, LENGTH = 0x00080000 /* RAM,   512k */
}

INCLUDE "script.common.ld"
