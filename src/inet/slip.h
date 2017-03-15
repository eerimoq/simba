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

struct slip_t {
    struct {
        int is_escaped;
        uint8_t *buf_p;
        size_t size;
        size_t pos;
    } rx;
    struct chan_t chout;
    void *chout_p;
};

/**
 * Initialize given slip object.
 *
 * @param[in] self_p Object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int slip_init(struct slip_t *self_p,
              void *buf_p,
              size_t size,
              void *chout_p);

/**
 * Input a data byte into the slip parser.
 *
 * @param[in] self_p Slip object.
 * @param[in] data Data byte to input.
 *
 * @return Once a complete SLIP frame has been received the size of
 *         the frame is returned. Meanwhile, zero(0) or negative error
 *         code is returned.
 */
ssize_t slip_input(struct slip_t *self_p,
                   uint8_t data);

/**
 * Get the output channel for given slip object.
 *
 * @param[in] self_p Slip object.
 *
 * @return Output channel.
 */
void *slip_get_output_channel(struct slip_t *self_p);
