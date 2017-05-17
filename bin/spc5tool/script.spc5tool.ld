OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc:common)
SEARCH_DIR(.)

/* Memory Spaces Definitions */
MEMORY
{
        cflash (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00040000 /* Code flash, 256K */
        dflash (rx)  : ORIGIN = 0x00800000, LENGTH = 0x00010000 /* Data flash,  64K */
        ram (rwx)    : ORIGIN = 0x40000100, LENGTH = 0x00002000 /* SRAM,        8K */
}

__cflash_begin = ORIGIN(cflash);
__cflash_end = ORIGIN(cflash) + LENGTH(cflash);
__cflash_size = LENGTH(cflash);

__dflash_begin = ORIGIN(dflash);
__dflash_end = ORIGIN(dflash) + LENGTH(dflash);
__dflash_size = LENGTH(dflash);

__ram_begin = ORIGIN(ram);
__ram_end = ORIGIN(ram) + LENGTH(ram);
__ram_size = LENGTH(ram);

/* The stack used by the main thread. */
__main_stack_end = ORIGIN(ram) + LENGTH(ram) - 8;

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.startup .startup.*))
        KEEP(*(.vectors .vectors.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.rodata .rodata* .gnu.linkonce.r.*)

        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_begin = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_begin = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_begin = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        __fixed_end = .;            /* End of text section */
    } > ram

    . = ALIGN(256);
    __text_end = .;

    .relocate : AT (__text_end)
    {
        . = ALIGN(4);
        __relocate_begin = .;
        __relocate_end = .;
    } > ram

    .bss (NOLOAD) :
    {
          . = ALIGN(4);
          __bss_begin = . ;
          __zero_begin = . ;
          *(.bss .bss.*)
          *(COMMON)
          . = ALIGN(4);
          __bss_end = . ;
          __zero_end = . ;
    } > ram

    /* Main thread stack section. */
    .main_stack (NOLOAD) :
    {
        . = ALIGN(4);
    } > ram

    . = ALIGN(4);
    _end = . ;
}
