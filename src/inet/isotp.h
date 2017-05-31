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

#ifndef __INET_ISOTP_H__
#define __INET_ISOTP_H__

#include "simba.h"

#define ISOTP_FLAGS_NO_FLOW_CONTROL            (1 << 0)

struct isotp_t {
    uint8_t *message_p;
    size_t size;
    int state;
    int flags;
    struct {
        size_t size;
        size_t offset;
        int next_index;
    } message;
};

/**
 * Initialize given ISO-TP object. An object can _either_ be used to
 * transmit or receive an ISO-TP message. Once `isotp_input()` or
 * `isotp_output()` returns a positive value the message transmission
 * is completed.
 *
 * An object can only be used to transmit one message. Initialize a
 * new object to transmit another message.
 *
 * @param[in] self_p Driver object to initialize.
 * @param[in] message_p ISO-TP message to transmit, or a reception
 *                      buffer for an incoming message.
 * @param[in] size Size of the message buffer in bytes.
 * @param[in] flags Configuration flags.
 *
 * @return zero(0) or negative error code.
 */
int isotp_init(struct isotp_t *self_p,
               uint8_t *message_p,
               size_t size,
               int flags);

/**
 * Input a CAN frame into given ISO-TP object. Always call
 * isotp_output() after this function returns zero(0) to check if
 * there are frames to transmit.
 *
 * @param[in] self_p Initialized ISO-TP object.
 * @param[in] buf_p Input data.
 * @param[in] size Data buffer length is bytes.
 *
 * @return Once a complete ISO-TP message has been received the size
 *         of the message is returned. Meanwhile, zero(0) is returned
 *         if the frame was expected. A negative error code is
 *         returned if the frame was unexpected or invalid.
 *
 *         For an ISO-TP object that transmits a message this function
 *         always returns zero(0) or negative error code.
 */
ssize_t isotp_input(struct isotp_t *self_p,
                    const uint8_t *buf_p,
                    size_t size);

/**
 * Check if there is data to be transmitted. The caller must transmit
 * all frames this function creates.
 *
 * @param[in] self_p Initialized ISO-TP object.
 * @param[out] buf_p Output data to be transmitted to the peer. The
 *                   size of this buffer must be at least eight bytes.
 * @param[out] size_p Number of bytes to be transmitted.
 *
 * @return Once a complete ISO-TP message has been transmitted the
 *         size of the message is returned. Meanwhile, zero(0) or
 *         negative error code is returned.
 *
 *         For an ISO-TP object that receives a message this function
 *         always returns zero(0) or negative error code.
 */
ssize_t isotp_output(struct isotp_t *self_p,
                     uint8_t *buf_p,
                     size_t *size_p);

#endif
