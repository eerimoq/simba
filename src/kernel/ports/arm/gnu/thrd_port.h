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

#ifndef __KERNEL_THRD_PORT_H__
#define __KERNEL_THRD_PORT_H__

#if CONFIG_PREEMPTIVE_SCHEDULER == 1
#    error "This port does not support a preemptive scheduler."
#endif

#define THRD_PORT_STACK(name, size)                             \
    uint64_t name[DIV_CEIL(sizeof(struct thrd_t) + (size),      \
                           sizeof(uint64_t))] __attribute((aligned (8)))

#define THRD_PORT_CONTEXT_STORE_ISR
#define THRD_PORT_CONTEXT_LOAD_ISR

struct thrd_port_context_t {
    /* Context stored by the software. */
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    /* uint32_t lr_ex; */

    /* /\* Context stored by the hardware when it enters an
       interrupt. *\/ */
    /* uint32_t r0; */
    /* uint32_t r1; */
    /* uint32_t r2; */
    /* uint32_t r3; */
    /* uint32_t r12; */
    /* uint32_t lr; */
    uint32_t pc;
    /* uint32_t psr; */
};

struct thrd_port_t {
    struct {
        uint32_t start;
        struct {
            uint32_t start;
            uint32_t time;
        } period;
    } cpu;
    struct thrd_port_context_t *context_p;
};

#endif
