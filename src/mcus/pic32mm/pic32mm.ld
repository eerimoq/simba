/**
 * PIC32MM common linker script definitions.
 */

__pflash_begin = ORIGIN(pflash);
__pflash_end = ORIGIN(pflash) + LENGTH(pflash);
__pflash_size = LENGTH(pflash);

__bflash_begin = ORIGIN(bflash);
__bflash_end = ORIGIN(bflash) + LENGTH(bflash);
__bflash_size = LENGTH(bflash);

__ram_begin = ORIGIN(ram);
__ram_end = ORIGIN(ram) + LENGTH(ram);
__ram_size = LENGTH(ram);

/* The stack used by the main thread. */
__main_stack_end = ORIGIN(ram) + LENGTH(ram) - 8;

SECTIONS
{
    .reset :
    {
        KEEP(*(.reset .reset.*))
    } > bflash

    .text :
    {
        __fixed_begin = .;
        __ebase = .;
        . = ALIGN(0x180);
        KEEP(*(.general_exception))
        . = ALIGN(0x200);
        KEEP(*(.vector))
        KEEP(*(.startup .startup.*))
        . = ALIGN(4096);
        KEEP(*(.interrupt))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.sdata .sdata.*);
        *(.MIPS.abiflags)

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
    } > pflash

    /* Non-volatile memory. One DPflash block for each nvm block. */
    .nvm.eeprom_soft :
    {
        *(.nvm.eeprom_soft.block_0);
        . = 0x4000;

        *(.nvm.eeprom_soft.block_1);
        . = 0x8000;
    } > pflash

    . = ALIGN(4);
    __text_end = .;

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
          *(.bss)
          *(.bss.*)
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
