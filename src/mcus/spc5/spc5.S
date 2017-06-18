/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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
        .globl    _start
        .type     _start,@function
        .align    2
_start:
        se_bl     _ecc_init
        se_bl     _relocate_init

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

.L_start_end:
        se_b .L_start_end

/**
 * SRAM ECC initialization by writing zero to all addresses. This
 * implicitly clears the zero segment.
 */
        .type     _ecc_init,@function
_ecc_init:
#if CONFIG_SPC5_RAM_CLEAR_ALL == 1
        e_lis     %r3, __ram_begin@h
        e_or2i    %r3, __ram_begin@l
#else
        e_lis     %r3, __bss_begin@h
        e_or2i    %r3, __bss_begin@l
#endif
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

/**
 * Initialize the relocate segment.
 */
        .type     _relocate_init,@function
_relocate_init:
        e_lis     %r3, __text_end@h
        e_or2i    %r3, __text_end@l
        e_lis     %r4, __relocate_begin@h
        e_or2i    %r4, __relocate_begin@l
        e_lis     %r5, __relocate_end@h
        e_or2i    %r5, __relocate_end@l

        /* Skip if the relocate segment empty. */
        cmplw     %r4, %r5
        e_bge     .L_relocate_init_end

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

.L_relocate_init_end:
        se_blr

#if CONFIG_SYSTEM_INTERRUPTS == 1

/**
 * Setup interrput handling.
 */
        .type     _interrupt_init,@function
_interrupt_init:
        /* Interrupt vector prefix register. The base address of the
           interrupt vector. */
        e_lis     %r3, ivor@h
        e_or2i    %r3, ivor@l
        mtivpr    %r3

        se_blr

/**
 * External interrupt handler.
 */
.section .text

        .type     isr_external_input,@function
        .weak     isr_external_input
isr_external_input:
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

.section .ivor

/**
 * Interrupt vector. Must be aligned to 4096 bytes.
 */
        .globl    ivor
        .type     ivor,@object
        .align    12                              /* 2 ^ 12 = 4096 */
ivor:
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
.section .text

        .globl    isr_critical_input
        .type     isr_critical_input,@function
        .weak     isr_critical_input
        .align    4
isr_critical_input:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_critical_input_string@h
        e_add16i  %r3, %r3, isr_critical_input_string@l
        e_bl      sys_panic

        .globl    isr_machine_check
        .type     isr_machine_check,@function
        .weak     isr_machine_check
        .align    4
isr_machine_check:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_machine_check_string@h
        e_add16i  %r3, %r3, isr_machine_check_string@l
        e_bl      sys_panic

        .globl    isr_data_storage
        .type     isr_data_storage,@function
        .weak     isr_data_storage
        .align    4
isr_data_storage:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_data_storage_string@h
        e_add16i  %r3, %r3, isr_data_storage_string@l
        e_bl      sys_panic

        .globl    isr_instruction_storage
        .type     isr_instruction_storage,@function
        .weak     isr_instruction_storage
        .align    4
isr_instruction_storage:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_instruction_storage_string@h
        e_add16i  %r3, %r3, isr_instruction_storage_string@l
        e_bl      sys_panic

        .globl    isr_alignment
        .type     isr_alignment,@function
        .weak     isr_alignment
        .align    4
isr_alignment:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_alignment_string@h
        e_add16i  %r3, %r3, isr_alignment_string@l
        e_bl      sys_panic

        .globl    isr_program
        .type     isr_program,@function
        .weak     isr_program
        .align    4
isr_program:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_program_string@h
        e_add16i  %r3, %r3, isr_program_string@l
        e_bl      sys_panic

        .globl    isr_reserved
        .type     isr_reserved,@function
        .weak     isr_reserved
        .align    4
isr_reserved:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_reserved_string@h
        e_add16i  %r3, %r3, isr_reserved_string@l
        e_bl      sys_panic

        .globl    isr_system_call
        .type     isr_system_call,@function
        .weak     isr_system_call
        .align    4
isr_system_call:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_system_call_string@h
        e_add16i  %r3, %r3, isr_system_call_string@l
        e_bl      sys_panic

        .globl    isr_auxiliary_processor_unavailable
        .type     isr_auxiliary_processor_unavailable,@function
        .weak     isr_auxiliary_processor_unavailable
        .align    4
isr_auxiliary_processor_unavailable:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_auxiliary_processor_unavailable_string@h
        e_add16i  %r3, %r3, isr_auxiliary_processor_unavailable_string@l
        e_bl      sys_panic

        .globl    isr_decrementer
        .type     isr_decrementer,@function
        .weak     isr_decrementer
        .align    4
isr_decrementer:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_decrementer_string@h
        e_add16i  %r3, %r3, isr_decrementer_string@l
        e_bl      sys_panic

        .globl    isr_fixed_interval_timer_interrupt
        .type     isr_fixed_interval_timer_interrupt,@function
        .weak     isr_fixed_interval_timer_interrupt
        .align    4
isr_fixed_interval_timer_interrupt:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_fixed_interval_timer_interrupt_string@h
        e_add16i  %r3, %r3, isr_fixed_interval_timer_interrupt_string@l
        e_bl      sys_panic

        .globl    isr_watchdog_timer_interrupt
        .type     isr_watchdog_timer_interrupt,@function
        .weak     isr_watchdog_timer_interrupt
        .align    4
isr_watchdog_timer_interrupt:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_watchdog_timer_interrupt_string@h
        e_add16i  %r3, %r3, isr_watchdog_timer_interrupt_string@l
        e_bl      sys_panic

        .globl    isr_data_tlb_error
        .type     isr_data_tlb_error,@function
        .weak     isr_data_tlb_error
        .align    4
isr_data_tlb_error:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_data_tlb_error_string@h
        e_add16i  %r3, %r3, isr_data_tlb_error_string@l
        e_bl      sys_panic

        .globl    isr_instruction_tlb_error
        .type     isr_instruction_tlb_error,@function
        .weak     isr_instruction_tlb_error
        .align    4
isr_instruction_tlb_error:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_instruction_tlb_error_string@h
        e_add16i  %r3, %r3, isr_instruction_tlb_error_string@l
        e_bl      sys_panic

        .globl    isr_debug
        .type     isr_debug,@function
        .weak     isr_debug
        .align    4
isr_debug:
        e_stwu    %r1, -8(%r1)
        mfsrr0    %r0
        se_stw    %r0, 12(%r1)
        e_lis     %r3, isr_debug_string@h
        e_add16i  %r3, %r3, isr_debug_string@l
        e_bl      sys_panic

#endif
