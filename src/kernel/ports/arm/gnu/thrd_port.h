/**
 * @file arm/gnu/thrd_port.i
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#define THRD_PORT_STACK(name, size)                             \
    uint64_t name[DIV_CEIL(sizeof(struct thrd_t) + (size),      \
                           sizeof(uint64_t))] __attribute((aligned (8)))

struct thrd_port_context_t {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t pc;
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
