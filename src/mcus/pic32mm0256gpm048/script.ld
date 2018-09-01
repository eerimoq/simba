OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips)
SEARCH_DIR(.)
ENTRY(_start)

/* Memory Spaces Definitions */
MEMORY
{
        pflash (rx) :   ORIGIN = 0x1d000000, LENGTH = 0x00040000 /* Program Flash, 256K */
        bflash (rx) :   ORIGIN = 0x1fc00000, LENGTH = 0x00001700 /* Boot Flash, 5.75K */
        confbits (rx) : ORIGIN = 0x1fc017c0, LENGTH = 0x00000040 /* Confbits, 256 */
        ram (rwx)   :   ORIGIN = 0x80000000, LENGTH = 0x00008000 /* RAM,  32K */
}

INCLUDE "../pic32mm/pic32mm.ld"
