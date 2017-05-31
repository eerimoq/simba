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

#define TYPE_SINGLE_FRAME                          0
#define TYPE_FIRST_FRAME                           1
#define TYPE_CONSECUTIVE_FRAME                     2
#define TYPE_FLOW_CONTROL_FRAME                    3

enum state_t {
    state_idle_t = 0,

    /* RX states. */
    state_first_frame_received_t,
    state_flow_control_frame_sent_t,

    /* TX states. */
    state_first_frame_sent_t,
    state_flow_control_frame_received_t
};

static ssize_t handle_input_idle(struct isotp_t *self_p,
                                 const uint8_t *buf_p,
                                 size_t size)
{
    int res;
    int type;

    type = (buf_p[0] >> 4);

    switch (type) {

    case TYPE_SINGLE_FRAME:
        size = (buf_p[0] & 0x0f);

        if ((size > 7) || (size == 0)) {
            res = -1;
            break;
        }

        if (size > self_p->size) {
            res = -1;
            break;
        }

        memcpy(self_p->message_p, &buf_p[1], size);
        res = size;
        break;

    case TYPE_FIRST_FRAME:
        size = (((buf_p[0] & 0x0f) << 8) | buf_p[1]);

        if (size < 8) {
            res = -1;
            break;
        }

        if (size > self_p->size) {
            res = -1;
            break;
        }

        memcpy(self_p->message_p, &buf_p[2], 6);
        self_p->message.size = size;
        self_p->message.offset = 6;
        self_p->message.next_index = 1;

        if (self_p->flags & ISOTP_FLAGS_NO_FLOW_CONTROL) {
            self_p->state = state_flow_control_frame_sent_t;
        } else {
            self_p->state = state_first_frame_received_t;
        }

        res = 0;
        break;

    default:
        res = -1;
        break;
    }

    return (res);
}

static ssize_t handle_input_first_frame_sent(struct isotp_t *self_p,
                                             const uint8_t *buf_p,
                                             size_t size)
{
    int type;

    type = (buf_p[0] >> 4);

    if (type != TYPE_FLOW_CONTROL_FRAME) {
        return (-1);
    }

    if (size < 3) {
        return (-1);
    }

    self_p->state = state_flow_control_frame_received_t;

    return (0);
}

static ssize_t handle_input_flow_control_sent(struct isotp_t *self_p,
                                              const uint8_t *buf_p,
                                              size_t size)
{
    int index;
    ssize_t res;
    int type;

    type = (buf_p[0] >> 4);

    if (type != TYPE_CONSECUTIVE_FRAME) {
        return (-1);
    }

    index = (buf_p[0] & 0x0f);

    if (index != self_p->message.next_index) {
        return (-1);
    }

    memcpy(&self_p->message_p[self_p->message.offset], &buf_p[1], size - 1);
    self_p->message.offset += (size - 1);
    self_p->message.next_index++;
    self_p->message.next_index %= 16;

    if (self_p->message.offset >= self_p->message.size) {
        res = self_p->message.size;
        self_p->state = state_idle_t;
    } else {
        res = 0;
    }

    return (res);
}

static ssize_t handle_input_flow_control_received(struct isotp_t *self_p,
                                                  const uint8_t *buf_p,
                                                  size_t size)
{
    int type;

    type = (buf_p[0] >> 4);

    if (type != TYPE_FLOW_CONTROL_FRAME) {
        return (-1);
    }

    return (0);
}

static ssize_t handle_output_idle(struct isotp_t *self_p,
                                  uint8_t *output_p,
                                  size_t *output_size_p)
{
    int res;

    res = 0;

    if (self_p->size < 8) {
        output_p[0] = ((TYPE_SINGLE_FRAME << 4) | self_p->size);
        memcpy(&output_p[1], self_p->message_p, self_p->size);
        *output_size_p = (self_p->size + 1);
        res = self_p->size;
    } else {
        output_p[0] = ((TYPE_FIRST_FRAME << 4) | (self_p->size >> 8));
        output_p[1] = self_p->size;
        memcpy(&output_p[2], self_p->message_p, 6);
        *output_size_p = 8;
        self_p->message.offset = 6;
        self_p->message.next_index = 1;

        if (self_p->flags & ISOTP_FLAGS_NO_FLOW_CONTROL) {
            self_p->state = state_flow_control_frame_received_t;
        } else {
            self_p->state = state_first_frame_sent_t;
        }
    }

    return (res);
}

static ssize_t handle_output_first_frame_received(struct isotp_t *self_p,
                                                  uint8_t *buf_p,
                                                  size_t *size_p)
{
    buf_p[0] = (TYPE_FLOW_CONTROL_FRAME << 4);
    buf_p[1] = 0;
    buf_p[2] = 0;
    *size_p = 3;
    self_p->state = state_flow_control_frame_sent_t;

    return (0);
}

static ssize_t handle_output_flow_control_received(struct isotp_t *self_p,
                                                   uint8_t *buf_p,
                                                   size_t *size_p)
{
    int res;
    size_t size;

    res = 0;

    buf_p[0] = ((TYPE_CONSECUTIVE_FRAME << 4) | self_p->message.next_index);
    size = MIN(self_p->size - self_p->message.offset, 7);
    memcpy(&buf_p[1], &self_p->message_p[self_p->message.offset], size);
    *size_p = (size + 1);
    self_p->message.offset += size;
    self_p->message.next_index++;
    self_p->message.next_index %= 16;

    if (self_p->message.offset >= self_p->size) {
        self_p->state = state_idle_t;
        res = self_p->size;
    }

    return (res);
}

int isotp_init(struct isotp_t *self_p,
               uint8_t *buf_p,
               size_t size,
               int flags)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    self_p->message_p = buf_p;
    self_p->size = size;
    self_p->state = state_idle_t;
    self_p->flags = flags;

    return (0);
}

ssize_t isotp_input(struct isotp_t *self_p,
                    const uint8_t *buf_p,
                    size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    ssize_t res;

    switch (self_p->state) {

    case state_idle_t:
        res = handle_input_idle(self_p, buf_p, size);
        break;

    case state_first_frame_sent_t:
        res = handle_input_first_frame_sent(self_p, buf_p, size);
        break;

    case state_flow_control_frame_sent_t:
        res = handle_input_flow_control_sent(self_p, buf_p, size);
        break;

    case state_flow_control_frame_received_t:
        res = handle_input_flow_control_received(self_p, buf_p, size);
        break;

    default:
        res = -1;
        break;
    }

    return (res);
}

ssize_t isotp_output(struct isotp_t *self_p,
                     uint8_t *buf_p,
                     size_t *size_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    int res;

    *size_p = 0;

    switch (self_p->state) {

    case state_idle_t:
        res = handle_output_idle(self_p, buf_p, size_p);
        break;

    case state_first_frame_received_t:
        res = handle_output_first_frame_received(self_p, buf_p, size_p);
        break;

    case state_flow_control_frame_received_t:
        res = handle_output_flow_control_received(self_p, buf_p, size_p);
        break;

    case state_first_frame_sent_t:
    case state_flow_control_frame_sent_t:
        res = 0;
        break;

    default:
        res = -1;
        break;
    }

    return (res);
}
