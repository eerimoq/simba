/**
 * @section License
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef __INET_SSL_H__
#define __INET_SSL_H__

#include "simba.h"

struct ssl_t {
    struct chan_t base;
    struct socket_t socket;
};

/**
 * Initialize the ssl module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int ssl_module_init(void);

/**
 * Initialize given SSL instance.
 *
 * @param[out] self_p Ssl to initialize.
 *
 * @return zero(0) or negative error code.
 */
int ssl_init(struct ssl_t *self_p);

/**
 * Close given ssl. No data transfers are allowed on after the
 * ssl has been closed.
 *
 * @param[in] self_p Ssl to close.
 *
 * @return zero(0) or negative error code.
 */
int ssl_start(struct ssl_t *self_p);

/**
 * Close given ssl. No data transfers are allowed on after the
 * ssl has been closed.
 *
 * @param[in] self_p Ssl to close.
 *
 * @return zero(0) or negative error code.
 */
int ssl_stop(struct ssl_t *self_p);

/**
 * Write data to given SSL instalce.
 *
 * @param[in] self_p SSL instance.
 * @param[in] buf_p Buffer to send.
 * @param[in] size Numer of bytes to send.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t ssl_write(struct ssl_t *self_p,
                  const void *buf_p,
                  size_t size);

/**
 * Read data from given SSL instance.
 *
 * @param[in] self_p Ssl.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t ssl_read(struct ssl_t *self_p,
                 void *buf_p,
                 size_t size);

/**
 * Get the number of input bytes currently stored in the SSL
 * instance. May return less bytes than number of bytes stored in the
 * channel.
 *
 * @param[in] self_p SSL instance.
 *
 * @return Number of input bytes in the SSL instance.
 */
ssize_t ssl_size(struct ssl_t *self_p);

#endif
