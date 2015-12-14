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

/**
 * Initialize given FIFO.
 *
 * @param[in,out] self_p FIFO to initialize.
 * @param[in] max Maximum number of elements in the FIFO.
 *
 * @return zero(0) or negative error code.
 */
static inline int fifo_init(struct fifo_t *self_p,
                            int max)
{
    self_p->rdpos = 0;
    self_p->wrpos = 0;
    self_p->max = max;

    return (0);
}

/**
 * Put element to the FIFO.
 *
 * @param[in] self_p Initialized FIFO.
 *
 * @return Added element index in FIFO, or -1 if there are no free
 *         positions in the FIFO.
 */
static inline int fifo_put(struct fifo_t *self_p)
{
    int i;
    int wrpos = ((self_p->wrpos + 1) % self_p->max);

    if (self_p->rdpos == wrpos) {
        return (-1);
    }

    i = self_p->wrpos;
    self_p->wrpos = wrpos;

    return (i);
}

/**
 * Get element to the FIFO.
 *
 * @param[in] self_p Initialized FIFO.
 *
 * @return Fetched element index in FIFO , or -1 if the FIFO is empty.
 */
static inline int fifo_get(struct fifo_t *self_p)
{
    int i;

    if (self_p->rdpos == self_p->wrpos) {
        return (-1);
    }

    i = self_p->rdpos;
    self_p->rdpos++;
    self_p->rdpos %= self_p->max;

    return (i);
}

#endif
