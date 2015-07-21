/**
 * @file kernel/fifo.h
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

#ifndef __KERNEL_FIFO_H__
#define __KERNEL_FIFO_H__

#include "simba.h"

struct fifo_t {
    int rdpos;
    int wrpos;
    int max;
};

static inline int fifo_init(struct fifo_t *fifo_p,
                            int max)
{
    fifo_p->rdpos = 0;
    fifo_p->wrpos = 0;
    fifo_p->max = max;

    return (0);
}

static inline int fifo_put(struct fifo_t *fifo_p)
{
    int i;
    int wrpos = ((fifo_p->wrpos + 1) % fifo_p->max);

    if (fifo_p->rdpos == wrpos) {
        return (-1);
    }

    i = fifo_p->wrpos;
    fifo_p->wrpos = wrpos;

    return (i);
}

static inline int fifo_get(struct fifo_t *fifo_p)
{
    int i;

    if (fifo_p->rdpos == fifo_p->wrpos) {
        return (-1);
    }

    i = fifo_p->rdpos;
    fifo_p->rdpos++;
    fifo_p->rdpos %= fifo_p->max;

    return (i);
}

#endif
