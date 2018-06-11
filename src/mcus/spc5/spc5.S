/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#include "config.h"
#include "config_default.h"
#include "kernel/asm.h"

#define INTC_IACKR                       0xfff48010
#define INTC_EOIR                        0xfff48018

/* Enable critical and machine check interrupts. */
#define MSR                              0x00021000

#define INTERRUPT_STACK (interrupt_stack + CONFIG_SYSTEM_INTERRUPT_STACK_SIZE - 84)

.section .startup

#if CONFIG_SPC5_BOOT_ENTRY_RCHW == 1

        .globl   _rchw
        .type     _rchw,@object
        .align    2
_rchw:
        .long     0x015a0000
        .long     _start

#endif

/**
 * Entry point of the application. Initializes the CPU and calls the
 * application main function.
 */
ASM_FUNC_BEGIN _start, 2
        se_bl     _ecc_init
#if CONFIG_SPC5_RELOCATE_INIT == 1
        se_bl     _relocate_init
#endif

#if CONFIG_SYSTEM_INTERRUPTS == 1
        /* Configure the MSR register. */
        e_lis     %r1, MSR@h
        e_or2i    %r1, MSR@l
        mtmsr     %r1
#endif

        /* Stack pointer initialization. */
        e_lis     %r1, __main_stack_end@h
        e_or2i    %r1, __main_stack_end@l

        e_bl      spc5_init

#if CONFIG_SYSTEM_INTERRUPTS == 1
        se_bl     _interrupt_init
#endif

        /* Call the application main function! */
        e_bl      main

.L_start_loop:
        se_b .L_start_loop
ASM_FUNC_END _start

/**
 * SRAM ECC initialization by writing zero to all addresses. This
 * implicitly clears the zero segment.
 */
ASM_FUNC_LOCAL_BEGIN _ecc_init, 2
#    if CONFIG_SPC5_RAM_CLEAR_ALL == 1
        e_lis     %r3, __ram_begin@h
        e_or2i    %r3, __ram_begin@l
#    else
        e_lis     %r3, __bss_begin@h
        e_or2i    %r3, __bss_begin@l
#    endif
        e_lis     %r4, __ram_end@h
        e_or2i    %r4, __ram_end@l

        not       %r5, %r3
        add       %r5, %r4, %r5
        e_srwi    %r5, %r5, 2
        mtctr     %r5
        se_li     %r5, 0

.L_ecc_init_loop:
        e_stw     %r5, 0(%r3)
        e_addi    %r3, %r3, 4
        e_bdnz    .L_ecc_init_loop

        se_blr
ASM_FUNC_END _ecc_init

#if CONFIG_SPC5_RELOCATE_INIT == 1

/**
 * Initialize the relocate segment.
 */
ASM_FUNC_LOCAL_BEGIN _relocate_init, 2
        e_lis     %r3, __text_end@h
        e_or2i    %r3, __text_end@l
        e_lis     %r4, __relocate_begin@h
        e_or2i    %r4, __relocate_begin@l
        e_lis     %r5, __relocate_end@h
        e_or2i    %r5, __relocate_end@l

        /* Skip if the relocate segment empty. */
        cmplw     %r4, %r5
        e_bge     .L__relocate_init_end

        not       %r6, %r4
        add       %r6, %r6, %r5
        e_addi    %r6, %r6, 4
        e_srwi    %r6, %r6, 2
        mtctr     %r6
        e_li      %r6, 0

.L_relocate_init_loop:
        lwzx      %r8, %r6, %r3
        stwx      %r8, %r6, %r4
        e_addi    %r6, %r6, 4
        e_bdnz    .L_relocate_init_loop

        se_blr
ASM_FUNC_END _relocate_init

#endif

#if CONFIG_SYSTEM_INTERRUPTS == 1

/**
 * Setup interrput handling.
 */
ASM_FUNC_LOCAL_BEGIN _interrupt_init, 2
        /* Interrupt vector prefix register. The base address of the
           interrupt vector.

           Each interrupt has an associated interrupt vector address,
           obtained by concatenating the IVPR and IVOR values
           (IVPR[32–47] || IVORn[48–59] || 0b0000).
        */
        e_lis     %r3, ivor@h
        e_or2i    %r3, ivor@l
        mtivpr    %r3

        se_blr
ASM_FUNC_END _interrupt_init
        
/**
 * External interrupt handler.
 */
.section .text

ASM_FUNC_WEAK_BEGIN isr_external_input, 2
#if CONFIG_SYSTEM_INTERRUPT_STACK_SIZE == 0
        /* Store registers on the stack to allow an interrupt handler
           written in C. */
        e_stwu    %r1, -76(%r1)
#else
        /* Store r2 on the thread stack and move the stack pointer to
           r2, later saved on the interrupt stack. */
        e_stwu    %r1, -8(%r1)
        se_stw    %r2, 4(%r1)
        se_mr     %r2, %r1
        e_add16i  %r2, %r2, 8
        e_lis     %r1, INTERRUPT_STACK@h
        e_or2i    %r1, INTERRUPT_STACK@l
        e_stw     %r2, 0(%r1)
#endif
        e_stw     %r0, 64(%r1)
        mfsrr0    %r0
        e_stw     %r0, 80(%r1)
        se_stw    %r2, 60(%r1)
        se_stw    %r3, 56(%r1)
        se_stw    %r4, 52(%r1)
        se_stw    %r5, 48(%r1)
        se_stw    %r6, 44(%r1)
        se_stw    %r7, 40(%r1)
        e_stw     %r8, 36(%r1)
        e_stw     %r9, 32(%r1)
        e_stw     %r10, 28(%r1)
        e_stw     %r11, 24(%r1)
        e_stw     %r12, 20(%r1)
        mfmsr     %r2
        se_stw    %r2, 16(%r1)
        se_mflr   %r2
        se_stw    %r2, 12(%r1)
        mfcr      %r2
        se_stw    %r2, 8(%r1)
        se_mfctr  %r2
        e_stw     %r2, 68(%r1)
        mfxer     %r2
        e_stw     %r2, 72(%r1)

        /* Read the ISR address from the IACKR register. */
        e_lis     %r3, INTC_IACKR@ha
        e_lwz     %r3, INTC_IACKR@l(%r3) /* Address in the external interrupt
                                            vector. */
        e_lwz     %r4, 0x0(%r3) /* Load the ISR address. */

        /* Call the ISR. */
        se_mtctr  %r4
        se_bctrl

        mbar

        /* Signal end of interrupt to the hardware. */
        e_lis     %r3, INTC_EOIR@ha
        e_stw     %r3, INTC_EOIR@l(%r3)

        /* Load registers from the stack. */
        e_lwz     %r2, 72(%r1)
        mtxer     %r2
        e_lwz     %r2, 68(%r1)
        se_mtctr  %r2
        se_lwz    %r2, 8(%r1)
        mtcr      %r2
        se_lwz    %r2, 12(%r1)
        se_mtlr   %r2
        se_lwz    %r2, 16(%r1)
        mtmsr     %r2
        e_lwz     %r12, 20(%r1)
        e_lwz     %r11, 24(%r1)
        e_lwz     %r10, 28(%r1)
        e_lwz     %r9, 32(%r1)
        e_lwz     %r8, 36(%r1)
        se_lwz    %r7, 40(%r1)
        se_lwz    %r6, 44(%r1)
        se_lwz    %r5, 48(%r1)
        se_lwz    %r4, 52(%r1)
        se_lwz    %r3, 56(%r1)
        se_lwz    %r2, 60(%r1)
        e_lwz     %r0, 64(%r1)
#if CONFIG_SYSTEM_INTERRUPT_STACK_SIZE == 0
        e_add16i  %r1, %r1, 76
#else
        /* Restore the thread stack pointer. */
        se_mr     %r1, %r2
        e_sub16i  %r1, %r1, 8
        se_lwz    %r2, 4(%r1)
        e_add16i  %r1, %r1, 8
#endif

        se_rfi
ASM_FUNC_END isr_external_input

.section .ivor

/**
 * Interrupt vector. Must be aligned to 4096 bytes.
 */
ASM_OBJ_BEGIN ivor, 12
        .align    4
        e_b       isr_critical_input
        .align    4
        e_b       isr_machine_check
        .align    4
        e_b       isr_data_storage
        .align    4
        e_b       isr_instruction_storage
        .align    4
        e_b       isr_external_input
        .align    4
        e_b       isr_alignment
        .align    4
        e_b       isr_program
        .align    4
        e_b       isr_reserved
        .align    4
        e_b       isr_system_call
        .align    4
        e_b       isr_auxiliary_processor_unavailable
        .align    4
        e_b       isr_decrementer
        .align    4
        e_b       isr_fixed_interval_timer_interrupt
        .align    4
        e_b       isr_watchdog_timer_interrupt
        .align    4
        e_b       isr_data_tlb_error
        .align    4
        e_b       isr_instruction_tlb_error
        .align    4
        e_b       isr_debug
ASM_OBJ_END ivor

/**
 * Interrupt debug strings.
 */
.section .data

        .type     isr_critical_input_string,@object
isr_critical_input_string:
        .string   "isr_critical_input"

        .type     isr_machine_check_string,@object
isr_machine_check_string:
        .string   "isr_machine_check"

        .type     isr_data_storage_string,@object
isr_data_storage_string:
        .string   "isr_data_storage"

        .type     isr_instruction_storage_string,@object
isr_instruction_storage_string:
        .string   "isr_instruction_storage"

        .type     isr_alignment_string,@object
isr_alignment_string:
        .string   "isr_alignment"

        .type     isr_program_string,@object
isr_program_string:
        .string   "isr_program"

        .type     isr_reserved_string,@object
isr_reserved_string:
        .string   "isr_reserved"

        .type     isr_system_call_string,@object
isr_system_call_string:
        .string   "isr_system_call"

        .type     isr_auxiliary_processor_unavailable_string,@object
isr_auxiliary_processor_unavailable_string:
        .string   "isr_auxiliary_processor_unavailable"

        .type     isr_decrementer_string,@object
isr_decrementer_string:
        .string   "isr_decrementer"

        .type     isr_fixed_interval_timer_interrupt_string,@object
isr_fixed_interval_timer_interrupt_string:
        .string   "isr_fixed_interval_timer_interrupt"

        .type     isr_watchdog_timer_interrupt_string,@object
isr_watchdog_timer_interrupt_string:
        .string   "isr_watchdog_timer_interrupt"

        .type     isr_data_tlb_error_string,@object
isr_data_tlb_error_string:
        .string   "isr_data_tlb_error"

        .type     isr_instruction_tlb_error_string,@object
isr_instruction_tlb_error_string:
        .string   "isr_instruction_tlb_error"

        .type     isr_debug_string,@object
isr_debug_string:
        .string   "isr_debug"

/**
 * Interrupt handlers.
 *
 * Assembler functions equivalent to the C code below, but saving SRR0
 * as return address instead of LR.
 *
 * void isr_<name>(void)
 * {
 *     sys_panic("<name>")
 * }
 */
.section .ivor

ASM_FUNC_WEAK_BEGIN isr_critical_input, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_critical_input_string@h
        e_add16i  %r3, %r3, isr_critical_input_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_critical_input

ASM_FUNC_WEAK_BEGIN isr_machine_check, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_machine_check_string@h
        e_add16i  %r3, %r3, isr_machine_check_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_machine_check

ASM_FUNC_WEAK_BEGIN isr_data_storage, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_data_storage_string@h
        e_add16i  %r3, %r3, isr_data_storage_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_data_storage

ASM_FUNC_WEAK_BEGIN isr_instruction_storage, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_instruction_storage_string@h
        e_add16i  %r3, %r3, isr_instruction_storage_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_instruction_storage

ASM_FUNC_WEAK_BEGIN isr_alignment, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_alignment_string@h
        e_add16i  %r3, %r3, isr_alignment_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_alignment

ASM_FUNC_WEAK_BEGIN isr_program, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_program_string@h
        e_add16i  %r3, %r3, isr_program_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_program

ASM_FUNC_WEAK_BEGIN isr_reserved, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_reserved_string@h
        e_add16i  %r3, %r3, isr_reserved_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_reserved

ASM_FUNC_WEAK_BEGIN isr_system_call, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_system_call_string@h
        e_add16i  %r3, %r3, isr_system_call_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_system_call

ASM_FUNC_WEAK_BEGIN isr_auxiliary_processor_unavailable, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_auxiliary_processor_unavailable_string@h
        e_add16i  %r3, %r3, isr_auxiliary_processor_unavailable_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_auxiliary_processor_unavailable

ASM_FUNC_WEAK_BEGIN isr_decrementer, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_decrementer_string@h
        e_add16i  %r3, %r3, isr_decrementer_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_decrementer

ASM_FUNC_WEAK_BEGIN isr_fixed_interval_timer_interrupt, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_fixed_interval_timer_interrupt_string@h
        e_add16i  %r3, %r3, isr_fixed_interval_timer_interrupt_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_fixed_interval_timer_interrupt

ASM_FUNC_WEAK_BEGIN isr_watchdog_timer_interrupt, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_watchdog_timer_interrupt_string@h
        e_add16i  %r3, %r3, isr_watchdog_timer_interrupt_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_watchdog_timer_interrupt

ASM_FUNC_WEAK_BEGIN isr_data_tlb_error, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_data_tlb_error_string@h
        e_add16i  %r3, %r3, isr_data_tlb_error_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_data_tlb_error

ASM_FUNC_WEAK_BEGIN isr_instruction_tlb_error, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_instruction_tlb_error_string@h
        e_add16i  %r3, %r3, isr_instruction_tlb_error_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_instruction_tlb_error

ASM_FUNC_WEAK_BEGIN isr_debug, 4
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_debug_string@h
        e_add16i  %r3, %r3, isr_debug_string@l
        e_bl      sys_panic
ASM_FUNC_END isr_debug

#endif
