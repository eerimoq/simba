OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips)
SEARCH_DIR(.)
ENTRY(_start)

/* Memory Spaces Definitions */
MEMORY
{
        flash (rx)  : ORIGIN = 0x1d000000, LENGTH = 0x00040000 /* Flash, 256K */
        ram (rwx)   : ORIGIN = 0x80000000, LENGTH = 0x00008000 /* RAM,  32K */
}

INCLUDE "../pic32mm/pic32mm.ld"
