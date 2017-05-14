OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc:common)
SEARCH_DIR(.)
ENTRY(_start)

/* Memory Spaces Definitions */
MEMORY
{
        cflash (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00040000 /* Code flash, 256K */
        dflash (rx)  : ORIGIN = 0x00800000, LENGTH = 0x00010000 /* Data flash,  64K */
        ram (rwx)    : ORIGIN = 0x40000000, LENGTH = 0x00004000 /* SRAM,        16K */
}

INCLUDE "../spc5/spc5.ld"
