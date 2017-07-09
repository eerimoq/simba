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

#define THRD_PORT_STACK(name, size)             \
    uint32_t name[DIV_CEIL(sizeof(struct thrd_t) + (size), sizeof(uint32_t))]

void thrd_port_set_main_thrd(struct thrd_t *thrd_p);

void thrd_port_set_main_thrd_stack_top(void *top_p);

const void *thrd_port_get_bottom_of_stack_current_thread(void);

struct thrd_port_context_t {
    uint32_t a0;           /* Return addess from the swap function. */
    uint32_t ps;
};

struct thrd_port_t {
    struct thrd_port_context_t *context_p;
};

extern void thrd_port_swap(struct thrd_t *in_p,
                           struct thrd_t *out_p);

#endif
