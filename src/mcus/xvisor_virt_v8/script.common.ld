OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH("aarch64")
SEARCH_DIR(.)

__rom_begin = ORIGIN(rom);
__rom_end = ORIGIN(rom) + LENGTH(rom);
__rom_size = LENGTH(rom);

__ram_begin = ORIGIN(ram);
__ram_end = ORIGIN(ram) + LENGTH(ram);
__ram_size = LENGTH(ram);

/* The stack used by the main thread. */
__main_stack_end = ORIGIN(ram) + LENGTH(ram) - 8;

/* Section Definitions */
SECTIONS
{
    .text :
    {
        . = ALIGN(8);
        __fixed_start__ = .;
        KEEP(*(.reset .reset.*))
        KEEP(*(.vectors .vectors.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(8);
        KEEP(*(.init))
        . = ALIGN(8);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(8);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(8);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(8);
        KEEP(*(.fini))

        . = ALIGN(8);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))
        *(.nvm.eeprom_soft.*)
        __fixed_end__ = .;            /* End of text section */
    } > rom

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(256);
    __text_end__ = .;

    .relocate : AT (__text_end__)
    {
        . = ALIGN(8);
        __relocate_start__ = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        *(.rodata .rodata* .gnu.linkonce.r.*)
        . = ALIGN(8);
        __relocate_end__ = .;
    } > ram

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        . = ALIGN(8);
        __bss_start__ = . ;
        __zero_start__ = . ;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = . ;
        __zero_end__ = . ;
    } > ram

    /* Main thread stack section. */
    .main_stack (NOLOAD) :
    {
        . = ALIGN(8);
    } > ram

    . = ALIGN(8);
    _end = . ;
}
