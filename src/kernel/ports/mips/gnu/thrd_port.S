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

#include "kernel/asm.h"

/**
 * Perform a thread context switch.
 *
 * @param[in] a0 Thread to swap in.
 * @param[in] a1 Thread to swap out.
 */
ASM_FUNC_BEGIN thrd_port_swap, 4
        /* Store registers on the stack. */
        addiu  $sp, $sp, -44
        sw     $s0, 0($sp)
        sw     $s1, 4($sp)
        sw     $s2, 8($sp)
        sw     $s3, 12($sp)
        sw     $s4, 16($sp)
        sw     $s5, 20($sp)
        sw     $s6, 24($sp)
        sw     $s7, 28($sp)
        sw     $gp, 32($sp)
        sw     $s8, 36($sp)
        sw     $ra, 40($sp)

        /* Save 'out_p' stack pointer. */
        sw     $sp, 8($a1)

        /* Restore 'in_p' stack pointer. */
        lw     $sp, 8($a0)

        /* Load registers from the stack. */
        lw     $s0, 0($sp)
        lw     $s1, 4($sp)
        lw     $s2, 8($sp)
        lw     $s3, 12($sp)
        lw     $s4, 16($sp)
        lw     $s5, 20($sp)
        lw     $s6, 24($sp)
        lw     $s7, 28($sp)
        lw     $gp, 32($sp)
        lw     $s8, 36($sp)
        lw     $ra, 40($sp)

        jraddiusp 44
        nop
ASM_FUNC_END thrd_port_swap

/**
 * Thread entry function wrapper.
 */
ASM_FUNC_BEGIN thrd_port_main, 4
        /* Enable interrupts. */
        ei

        /* Call thread main function with argument. */
        move   $a0, $s1
        jrc    $s0

        /* Call termination function. */
        jal    terminate
ASM_FUNC_END thrd_port_main
