/**
 * @file linux/gnu/thrd_port.i
 * @version 0.2.0
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

#include <pthread.h>

#define THRD_PORT_STACK(name, size) char name[sizeof(struct thrd_t) + (size)]

struct thrd_port_t {
    pthread_t thrd;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    void *(*main)(void *arg);
    void *arg;
};

#endif
