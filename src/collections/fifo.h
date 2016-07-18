/**
 * @file collections/fifo.h
 * @version 2.0.0
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

#ifndef __COLLECTIONS_FIFO_H__
#define __COLLECTIONS_FIFO_H__

#include "simba.h"

struct fifo_t {
    int rdpos;
    int wrpos;
    void *buf_p;
    int max;
};

/**
 * Define the fifo structure and functions for a given type.
 *
 * @rst
 * .. code-block:: c
 *
 *    FIFO_DEFINE_TEMPLATE(int);
 *
 *    int foo()
 *    {
 *        struct fifo_int_t fifo;
 *        int buf[4];
 *        int value;
 *
 *        fifo_init_int(&fifo, buf, membersof(buf));
 *
 *        // Put a value into the fifo.
 *        value = 10;
 *        fifo_put_int(&fifo, &value);
 *
 *        // Get the value from the fifo.
 *        fifo_get_int(&fifo, &value);
 *
 *        // Prints 'value = 10'.
 *        std_printf(FSTR("value=  %d\r\n", value));
 *    }
 * @endrst
 *
 * @param[in] type Type of the elements in the defined fifo.
 */
#define FIFO_DEFINE_TEMPLATE(type)                                      \
    struct fifo_ ## type ## _t {                                        \
        struct fifo_t fifo;                                             \
        type *buf_p;                                                    \
    };                                                                  \
                                                                        \
    static int fifo_init_ ## type(struct fifo_ ## type ## _t *self_p,   \
                                  type *buf_p,                          \
                                  int max)                              \
    {                                                                   \
        self_p->buf_p = buf_p;                                          \
                                                                        \
        return (fifo_init(&self_p->fifo, max));                         \
    }                                                                   \
                                                                        \
    static int fifo_put_ ## type(struct fifo_ ## type ## _t *self_p,    \
                                 type *value_p)                         \
    {                                                                   \
        int index;                                                      \
                                                                        \
        index = fifo_put(&self_p->fifo);                                \
                                                                        \
        if (index == -1) {                                              \
            return (-1);                                                \
        }                                                               \
                                                                        \
        self_p->buf_p[index] = *value_p;                                \
                                                                        \
        return (0);                                                     \
    }                                                                   \
                                                                        \
    static int fifo_get_ ## type(struct fifo_ ## type ## _t *self_p,    \
                                 type *value_p)                         \
    {                                                                   \
        int index;                                                      \
                                                                        \
        index = fifo_get(&self_p->fifo);                                \
                                                                        \
        if (index == -1) {                                              \
            return (-1);                                                \
        }                                                               \
                                                                        \
        *value_p = self_p->buf_p[index];                                \
                                                                        \
        return (0);                                                     \
    }

/**
 * Initialize given fifo.
 *
 * @param[in,out] self_p Fifo to initialize.
 * @param[in] max Maximum number of elements in the fifo.
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
 * Put an element in the fifo.
 *
 * @param[in] self_p Initialized fifo.
 *
 * @return Added element index in fifo, or -1 if there are no free
 *         positions.
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
 * Get the next element from the fifo.
 *
 * @param[in] self_p Initialized fifo.
 *
 * @return The fetched element index in fifo , or -1 if the fifo is
 *         empty.
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
