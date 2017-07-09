OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc:common)
SEARCH_DIR(.)
ENTRY(_start)

/* Memory Spaces Definitions */
MEMORY
{
        cflash (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00020000 /* Code flash, 128K */
        dflash (rx)  : ORIGIN = 0x00800000, LENGTH = 0x00010000 /* Data flash,  64K */
        ram (rwx)    : ORIGIN = 0x40000000, LENGTH = 0x00003000 /* SRAM,        12K */
}

INCLUDE "../spc5/spc5.ld"
