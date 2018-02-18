/**
 * @section License
 *
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
 * @param[in] x30 Return address.
 * @param[in] sp Stack address.
 * @param[in] x0 Thread to swap in.
 * @param[in] x1 Thread to swap out.
 */
ASM_FUNC_BEGIN thrd_port_swap
        sub	sp, sp, 256

        /* Store out-thread's context. */
        stp     x0, x1, [sp]
        stp	x2, x3, [sp, 16]
        stp	x4, x5, [sp, 32]
        stp	x6, x7, [sp, 48]
        stp	x8, x9, [sp, 64]
        stp	x10, x11, [sp, 80]
        stp	x12, x13, [sp, 96]
        stp	x14, x15, [sp, 112]
        stp	x16, x17, [sp, 128]
        stp	x18, x19, [sp, 144]
        stp	x20, x21, [sp, 160]
        stp	x22, x23, [sp, 176]
        stp	x24, x25, [sp, 192]
        stp	x26, x27, [sp, 208]
        stp	x28, x29, [sp, 224]
        str	x30, [sp, 240]

        /* Swap stack pointers. */
        mov	x2, sp
        str	x2, [x1, 32]
        ldr	x0, [x0, 32]
        mov	sp, x0

        /* Load in-thread's context. */
        ldr	x30, [sp, 240]
        ldp	x28, x29, [sp, 224]
        ldp	x26, x27, [sp, 208]
        ldp	x24, x25, [sp, 192]
        ldp	x22, x23, [sp, 176]
        ldp	x20, x21, [sp, 160]
        ldp	x18, x19, [sp, 144]
        ldp	x16, x17, [sp, 128]
        ldp	x14, x15, [sp, 112]
        ldp	x12, x13, [sp, 96]
        ldp	x10, x11, [sp, 80]
        ldp	x8, x9, [sp, 64]
        ldp	x6, x7, [sp, 48]
        ldp	x4, x5, [sp, 32]
        ldp	x2, x3, [sp, 16]
        ldp	x0, x1, [sp]

        add	sp, sp, 256

        ret

ASM_FUNC_END thrd_port_swap

/**
 * Thread entry function wrapper.
 */
ASM_FUNC_BEGIN thrd_port_main
        msr	daifclr, (1 << 1)
        blr     x1
        bl      terminate
        nop

ASM_FUNC_END thrd_port_main
