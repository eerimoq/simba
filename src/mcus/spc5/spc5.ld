/**
 * SPC5 common linker script definitions.
 */

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
        __fixed_begin = .;
        KEEP(*(.startup .startup.*))
        . = ALIGN(2048);
        KEEP(*(.vector))
        . = ALIGN(4096);
        KEEP(*(.ivor))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.sdata .sdata.*);

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

        __fixed_end = .;            /* End of text section */
    } > cflash

    . = ALIGN(4);
    __text_end = .;

    /* Non-volatile memory. One DFlash block for each nvm block. */
    .nvm.eeprom_soft :
    {
        *(.nvm.eeprom_soft.block_0);
        . = 0x4000;

        *(.nvm.eeprom_soft.block_1);
        . = 0x8000;
    } > dflash

    .relocate : AT (__text_end)
    {
        . = ALIGN(4);
        __relocate_begin = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        . = ALIGN(4);
        __relocate_end = .;
    } > ram

    .bss (NOLOAD) :
    {
          . = ALIGN(4);
          __bss_begin = .;
          __zero_begin = .;
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
