/**
 * @file linux/gnu/thrd_port.i
 * @version 3.0.0
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

#define THRD_PORT_STACK(name, size)             \
    uint32_t name[DIV_CEIL(sizeof(struct thrd_t) + (size), sizeof(uint32_t))] __attribute__ ((section (".simba_other_stacks")))

struct thrd_port_context_t {
    uint32_t a0;           /* Return addess from the swap function. */
    uint32_t a12;
    uint32_t a13;
    uint32_t a14;
    uint32_t a15;
};

struct thrd_port_t {
    struct thrd_port_context_t *context_p;
};

#endif
