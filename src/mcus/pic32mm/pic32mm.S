/*
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

#include "config.h"
#include "config_default.h"
#include "kernel/asm.h"

#define C0_STATUS                              $12
#define C0_CAUSE                               $13
#define C0_CAUSE_IV                            0x00800000
#define C0_EBASE                               $15, 1
#define C0_INTCTL                              $12, 1
#define C0_INTCTL_VS_1                         0x00000020

.section .reset, "ax", @progbits

/**
 * Reset entry point in boot flash.
 */
ASM_FUNC_BEGIN _reset, 2
        la      $t0, _start
        jalr    $t0
        nop
ASM_FUNC_END _reset

.section .startup

/**
 * Entry point of the application. Initializes the CPU and calls the
 * application main function.
 */
ASM_FUNC_BEGIN _start, 2
        /* Stack pointer initialization. */
        la      $sp, __main_stack_end

        jal     pic32mm_relocate_init
        jal     pic32mm_bss_init

        la      $t1, __ebase
        mtc0    $t1, C0_EBASE

        li      $t2, C0_INTCTL_VS_1
        mtc0    $t2, C0_INTCTL

        li      $t1, C0_CAUSE_IV
        mtc0    $t1, C0_CAUSE

        mtc0    $zero, C0_STATUS

        la      $t0, main
        jalr    $t0
        nop

.L_start_loop:
        b       .L_start_loop
ASM_FUNC_END _start

/**
 * The interrupt handler at address __ebase + 0x200.
 */
.section .vector

ASM_FUNC_BEGIN isr_interrupt, 2
        /* Store registers on the stack to allow an interrupt handler
           written in C. */
        rdpgpr  $sp, $sp
        addiu   $sp, $sp, -76

        .set    noat

        sw      $at, 0($sp)
        sw      $v0, 4($sp)
        sw      $v1, 8($sp)
        sw      $a0, 12($sp)
        sw      $a1, 16($sp)
        sw      $a2, 20($sp)
        sw      $a3, 24($sp)
        sw      $t0, 28($sp)
        sw      $t1, 32($sp)
        sw      $t2, 36($sp)
        sw      $t3, 40($sp)
        sw      $t4, 44($sp)
        sw      $t5, 48($sp)
        sw      $t6, 52($sp)
        sw      $t7, 56($sp)
        sw      $t8, 60($sp)
        sw      $t9, 64($sp)
        sw      $fp, 68($sp)
        sw      $ra, 72($sp)

        /* Use vector index to find the interrupt service routine. */
        li      $a0, 0xbf80f020
        lw      $a0, 0($a0)
        andi    $a0, $a0, 0xff
        la      $a1, vector_table
        lwxs    $a1, $a0($a1)

        /* Call the ISR. */
        jal      $a1

        /* Load registers from the stack. */
        lw      $at, 0($sp)
        lw      $v0, 4($sp)
        lw      $v1, 8($sp)
        lw      $a0, 12($sp)
        lw      $a1, 16($sp)
        lw      $a2, 20($sp)
        lw      $a3, 24($sp)
        lw      $t0, 28($sp)
        lw      $t1, 32($sp)
        lw      $t2, 36($sp)
        lw      $t3, 40($sp)
        lw      $t4, 44($sp)
        lw      $t5, 48($sp)
        lw      $t6, 52($sp)
        lw      $t7, 56($sp)
        lw      $t8, 60($sp)
        lw      $t9, 64($sp)
        lw      $fp, 68($sp)
        lw      $ra, 72($sp)

        .set    at

        addiu	$sp, $sp, 76
        wrpgpr  $sp, $sp

        eret
ASM_FUNC_END isr_interrupt
