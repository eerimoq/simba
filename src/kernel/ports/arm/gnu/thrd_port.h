/**
 * @file kernel/ports/arm/gnu/thrd_port.h
 * @version 5.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
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
