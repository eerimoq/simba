/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_ISOTP_H__
#define __DRIVERS_ISOTP_H__

#include "simba.h"

struct isotp_t {
    uint8_t *message_p;
    size_t size;
    int state;
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
 *
 * @return zero(0) or negative error code.
 */
int isotp_init(struct isotp_t *self_p,
               uint8_t *message_p,
               size_t size);

/**
 * Input a CAN frame into given ISO-TP object.
 *
 * @param[in] self_p Initialized ISO-TP object.
 * @param[in] buf_p Input data.
 * @param[in] size Data buffer length is bytes.
 *
 * @return Once a complete ISO-TP message has been received the size
 *         of the message is returned. Meanwhile, zero(0) or negative
 *         error code is returned. For an ISO-TP object that
 *         transmitts a message this function always returns zero(0)
 *         or negative error code.
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
 *         negative error code is returned. For an ISO-TP object that
 *         receives a message this function always returns zero(0) or
 *         negative error code.
 */
ssize_t isotp_output(struct isotp_t *self_p,
                     uint8_t *buf_p,
                     size_t *size_p);

#endif
