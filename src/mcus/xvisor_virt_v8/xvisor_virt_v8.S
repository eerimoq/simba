/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#include "kernel/asm.h"

.section .reset

.extern main
.extern c_esr_none
.extern xvisor_virt_v8_init

/**
 * Context handling for exception handlers implemented in C.
 *
 * @param[in] esr Exception service routine.
 * @param[in] c_esr C exception service routine to call.
 */
.macro C_ESR_WRAPPER, esr, c_esr
        .globl  \esr
        .weak   \esr
        .type   \esr,@function
\esr:
        /* Store the context. */
        sub     sp, sp, 256

        stp     x0, x1, [sp]
        stp     x2, x3, [sp, 16]
        stp     x4, x5, [sp, 32]
        stp     x6, x7, [sp, 48]
        stp     x8, x9, [sp, 64]
        stp     x10, x11, [sp, 80]
        stp     x12, x13, [sp, 96]
        stp     x14, x15, [sp, 112]
        stp     x16, x17, [sp, 128]
        stp     x18, x19, [sp, 144]
        stp     x20, x21, [sp, 160]
        stp     x22, x23, [sp, 176]
        stp     x24, x25, [sp, 192]
        stp     x26, x27, [sp, 208]
        stp     x28, x29, [sp, 224]
        str     x30, [sp, 240]

        /* Call the C exception service routine. */
        bl      \c_esr

        /* Load the stored context. */
        ldr     x30, [sp, 240]
        ldp     x28, x29, [sp, 224]
        ldp     x26, x27, [sp, 208]
        ldp     x24, x25, [sp, 192]
        ldp     x22, x23, [sp, 176]
        ldp     x20, x21, [sp, 160]
        ldp     x18, x19, [sp, 144]
        ldp     x16, x17, [sp, 128]
        ldp     x14, x15, [sp, 112]
        ldp     x12, x13, [sp, 96]
        ldp     x10, x11, [sp, 80]
        ldp     x8, x9, [sp, 64]
        ldp     x6, x7, [sp, 48]
        ldp     x4, x5, [sp, 32]
        ldp     x2, x3, [sp, 16]
        ldp     x0, x1, [sp]

        add     sp, sp, 256

        /* Return from the exception. */
        eret
.endm

/**
 * An entry in the exception table, just branching to given exception
 * service routine.
 *
 * @param[in] esr Exception service routine.
 */
.macro VECTOR_TABLE_ENTRY, esr
        .align  7
        b       \esr
.endm

/**
 * The application entry point.
 */
ASM_FUNC_BEGIN app_entry
        ldr     x0, main_stack_end
        mov     sp, x0

        /* Data relocation. */
        ldr     x1, relocate_start
        ldr     x2, relocate_end
        cmp     x1, x2
        b.cs    .app_entry_zero

        mvn     x0, x1
        add     x0, x0, x2
        add     x0, x0, 4
        and     x0, x0, 0xfffffffffffffffc
        ldr     x3, text_end
        mov     x2, 0

.app_entry_relocate_loop:
        ldr     w4, [x2, x3]
        str     w4, [x2, x1]
        add     x2, x2, 4
        cmp     x2, x0
        b.ne    .app_entry_relocate_loop

        /* Zero. */
.app_entry_zero:
        ldr     x1, zero_start
        ldr     x2, zero_end
        cmp     x1, x2
        b.cs    .app_entry_main

        add     x2, x2, 4

.app_entry_zero_loop:
        str     wzr, [x1, 0]
        add     x1, x1, 4
        cmp     x1, x2
        b.ne    .app_entry_zero_loop

.app_entry_main:
        adr     x0, vector_table
        msr     vbar_el1, x0
        bl      xvisor_virt_v8_init
        bl      main

.app_entry_end_loop:
        b       .app_entry_end_loop
        nop

ASM_FUNC_END app_entry

/* Linker script definitions. */
main_stack_end:
        .dword  __main_stack_end

text_end:
        .dword  __text_end__

relocate_start:
        .dword  __relocate_start__

relocate_end:
        .dword  __relocate_end__

zero_start:
        .dword  __zero_start__

zero_end:
        .dword  __zero_end__

/**
 * The vector table with one entry for each exception.
 */
        .align  12
vector_table:
        /* Current EL with SP0. */
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none
        /* Current EL with SPx. */
        VECTOR_TABLE_ENTRY esr_sync
        VECTOR_TABLE_ENTRY esr_irq
        VECTOR_TABLE_ENTRY esr_fiq
        VECTOR_TABLE_ENTRY esr_serror
        /* Lower EL using AArch64. */
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none
        /* Lower EL using AArch32. */
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none
        VECTOR_TABLE_ENTRY esr_none

/* C exception service routine wrappers. Use ESR() in the C code to
   override. */
C_ESR_WRAPPER esr_sync, c_esr_sync

C_ESR_WRAPPER esr_irq, c_esr_irq

C_ESR_WRAPPER esr_fiq, c_esr_fiq

C_ESR_WRAPPER esr_serror, c_esr_serror

C_ESR_WRAPPER esr_none, c_esr_none
