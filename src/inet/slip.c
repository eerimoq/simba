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

#include "simba.h"

/** Protocol definitions. */
#define SLIP_END          0xc0
#define SLIP_ESC          0xdb
#define SLIP_ESC_END      0xdc
#define SLIP_ESC_ESC      0xdd

static ssize_t packet_write(void *chan_p,
                            const void *buf_p,
                            size_t size)
{
    struct slip_t *self_p;
    int i;
    uint8_t data;
    const uint8_t *b_p;

    self_p = container_of(chan_p, struct slip_t, chout);
    b_p = buf_p;

    data = SLIP_END;
    chan_write(self_p->chout_p, &data, 1);

    for (i = 0; i < size; i++) {
        data = b_p[i];

        switch (data) {

        case SLIP_END:
            data = SLIP_ESC;
            chan_write(self_p->chout_p, &data, 1);
            data = SLIP_ESC_END;
            break;

        case SLIP_ESC:
            data = SLIP_ESC;
            chan_write(self_p->chout_p, &data, 1);
            data = SLIP_ESC_ESC;
            break;
        }

        chan_write(self_p->chout_p, &data, 1);
    }

    data = SLIP_END;
    chan_write(self_p->chout_p, &data, 1);

    return (size);
}

static int packet_append(struct slip_t *self_p,
                         uint8_t data)
{
    /* Truncate long packets. */
    if (self_p->rx.pos == self_p->rx.size) {
        return (-1);
    }

    /* Write the data to the buffer. */
    self_p->rx.buf_p[self_p->rx.pos] = data;
    self_p->rx.pos++;

    return (0);
}

int slip_init(struct slip_t *self_p,
              void *buf_p,
              size_t size,
              void *chout_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);
    ASSERTN(chout_p != NULL, EINVAL);

    self_p->rx.is_escaped = 0;
    self_p->rx.buf_p = buf_p;
    self_p->rx.size = size;
    self_p->rx.pos = 0;
    self_p->chout_p = chout_p;

    chan_init(&self_p->chout,
              chan_read_null,
              packet_write,
              chan_size_null);

    return (0);
}

ssize_t slip_input(struct slip_t *self_p,
                   uint8_t data)
{
    ASSERTN(self_p != NULL, EINVAL);

    int res;

    res = 0;

    if (self_p->rx.is_escaped == 0) {
        if (data == SLIP_END) {
            if (self_p->rx.pos > 0) {
                res = self_p->rx.pos;
                self_p->rx.pos = 0;
            }
        } else if (data == SLIP_ESC) {
            self_p->rx.is_escaped = 1;
        } else {
            res = packet_append(self_p, data);
        }
    } else {
        if (data == SLIP_ESC_END) {
            res = packet_append(self_p, SLIP_END);
        } else if (data == SLIP_ESC_ESC) {
            res = packet_append(self_p, SLIP_ESC);
        } else {
            /* Protocol error. Discard current frame. */
            self_p->rx.pos = 0;
            res = -1;
        }

        self_p->rx.is_escaped = 0;
    }

    return (res);
}

void *slip_get_output_channel(struct slip_t *self_p)
{
    ASSERTNRN(self_p != NULL, EINVAL);

    return (&self_p->chout);
}
