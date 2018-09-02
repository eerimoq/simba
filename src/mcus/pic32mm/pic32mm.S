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
#define C0_EBASE                               $15, 1
#define C0_INTCTL                              $12, 1
#define INTCON                                 0xbf80f000

.section .reset, "ax", @progbits

/**
 * Reset entry point in boot flash.
 */
ASM_FUNC_BEGIN _reset, 2
        la  $t0, _start
        jalr $t0
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

        li      $t0, INTCON
        li      $t2, 0x00011000
        sw      $t2, 0($t0)

        li      $t2, 0x00000020
        mtc0    $t2, C0_INTCTL

        li      $t1, 0x00800000
        mtc0    $t1, C0_CAUSE

        li      $t0, 0x00000000
        mtc0    $t0, C0_STATUS

        la      $t0, main
        jalr    $t0
        nop

.L_start_loop:
        b       .L_start_loop
ASM_FUNC_END _start

/**
 * A vector entry.
 */
.macro VECTOR_ENTRY, number
        .type   vector_entry_\number, @function
        .align  3
vector_entry_\number:
        j       isr_interrupt
        nop
.endm

/**
 * The interrupt handler vector.
 */
.section .vector

VECTOR_ENTRY 0
VECTOR_ENTRY 1
VECTOR_ENTRY 2
VECTOR_ENTRY 3
VECTOR_ENTRY 4
VECTOR_ENTRY 5
VECTOR_ENTRY 6
VECTOR_ENTRY 7
VECTOR_ENTRY 8
VECTOR_ENTRY 9
VECTOR_ENTRY 10
VECTOR_ENTRY 11
VECTOR_ENTRY 12
VECTOR_ENTRY 13
VECTOR_ENTRY 14
VECTOR_ENTRY 15
VECTOR_ENTRY 16
VECTOR_ENTRY 17
VECTOR_ENTRY 18
VECTOR_ENTRY 19
VECTOR_ENTRY 20
VECTOR_ENTRY 21
VECTOR_ENTRY 22
VECTOR_ENTRY 23
VECTOR_ENTRY 24
VECTOR_ENTRY 25
VECTOR_ENTRY 26
VECTOR_ENTRY 27
VECTOR_ENTRY 28
VECTOR_ENTRY 29
VECTOR_ENTRY 30
VECTOR_ENTRY 31
VECTOR_ENTRY 32
VECTOR_ENTRY 33
VECTOR_ENTRY 34
VECTOR_ENTRY 35
VECTOR_ENTRY 36
VECTOR_ENTRY 37
VECTOR_ENTRY 38
VECTOR_ENTRY 39
VECTOR_ENTRY 40
VECTOR_ENTRY 41
VECTOR_ENTRY 42
VECTOR_ENTRY 43
VECTOR_ENTRY 44
VECTOR_ENTRY 45
VECTOR_ENTRY 46
VECTOR_ENTRY 47
VECTOR_ENTRY 48
VECTOR_ENTRY 49
VECTOR_ENTRY 50
VECTOR_ENTRY 51
VECTOR_ENTRY 52
VECTOR_ENTRY 53
VECTOR_ENTRY 54
VECTOR_ENTRY 55
VECTOR_ENTRY 56
VECTOR_ENTRY 57
VECTOR_ENTRY 58
VECTOR_ENTRY 59
VECTOR_ENTRY 60
VECTOR_ENTRY 61
VECTOR_ENTRY 62
VECTOR_ENTRY 63
VECTOR_ENTRY 64
VECTOR_ENTRY 65
VECTOR_ENTRY 66
VECTOR_ENTRY 67
VECTOR_ENTRY 68
VECTOR_ENTRY 69
VECTOR_ENTRY 70
VECTOR_ENTRY 71
VECTOR_ENTRY 72
VECTOR_ENTRY 73
VECTOR_ENTRY 74
VECTOR_ENTRY 75
VECTOR_ENTRY 76
VECTOR_ENTRY 77
VECTOR_ENTRY 78
VECTOR_ENTRY 79
VECTOR_ENTRY 80
VECTOR_ENTRY 81
VECTOR_ENTRY 82
VECTOR_ENTRY 83
VECTOR_ENTRY 84
VECTOR_ENTRY 85
VECTOR_ENTRY 86
VECTOR_ENTRY 87
VECTOR_ENTRY 88
VECTOR_ENTRY 89
VECTOR_ENTRY 90
VECTOR_ENTRY 91
VECTOR_ENTRY 92
VECTOR_ENTRY 93
VECTOR_ENTRY 94
VECTOR_ENTRY 95
VECTOR_ENTRY 96
VECTOR_ENTRY 97
VECTOR_ENTRY 98
VECTOR_ENTRY 99
VECTOR_ENTRY 100
VECTOR_ENTRY 101

ASM_FUNC_BEGIN isr_interrupt, 2
        /* Store registers on the stack to allow an interrupt handler
           written in C. */
        rdpgpr  $sp, $sp
        addiu   $sp, $sp, -76

        .set noat

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

        .set at

        addiu	$sp, $sp, 76
        wrpgpr  $sp, $sp

        eret
ASM_FUNC_END isr_interrupt
