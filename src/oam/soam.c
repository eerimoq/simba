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

#include "simba.h"

static ssize_t command_write(void *chan_p,
                             const void *buf_p,
                             size_t size)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, command.chan);

    if (soam_write_begin(self_p, SOAM_TYPE_COMMAND_RESPONSE_DATA) != 0) {
        return (-1);
    }

    (void)soam_write_chunk(self_p, self_p->command.id_p, 2);
    (void)soam_write_chunk(self_p, buf_p, size);

    if (soam_write_end(self_p) != size + 2) {
        size = -1;
    }

    return (size);
}

static ssize_t log_write(void *chan_p,
                         const void *buf_p,
                         size_t size)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, log.chan);

    return (soam_write_chunk(self_p, buf_p, size));
}

static int log_control(void *chan_p,
                       int operation)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, log.chan);

    switch (operation) {

    case CHAN_CONTROL_PACKET_BEGIN:
        return (soam_write_begin(self_p, SOAM_TYPE_LOG_POINT));

    case CHAN_CONTROL_PACKET_END:
        return (soam_write_end(self_p));

    default:
        return (-1);
    }
}

int soam_init(struct soam_t *self_p,
              void *buf_p,
              size_t size,
              void *chout_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(buf_p != NULL, -EINVAL);
    ASSERTN(size >= 5, -EINVAL);
    ASSERTN(chout_p != NULL, -EINVAL);

    self_p->tx.buf_p = buf_p;
    self_p->tx.size = size;
    self_p->tx.chout_p = chout_p;

    sem_init(&self_p->tx.sem, 0, 1);

    chan_init(&self_p->command.chan,
              chan_read_null,
              command_write,
              chan_size_null);

    chan_init(&self_p->log.chan,
              chan_read_null,
              log_write,
              chan_size_null);
    chan_set_control_cb(&self_p->log.chan, log_control);

    return (0);
}

int soam_input(struct soam_t *self_p,
               uint8_t *buf_p,
               size_t size)
{
    uint8_t type;
    size_t payload_crc_size;
    uint16_t crc;
    uint16_t input_crc;
    int32_t res;
    uint8_t buf[6];

    if (size < SOAM_PACKET_SIZE_MIN) {
        return (-1);
    }

    payload_crc_size = ((buf_p[1] << 8) | buf_p[2]);

    if (payload_crc_size > (size - 3)) {
        return (-1);
    }

    input_crc = ((buf_p[payload_crc_size + 1] << 8)
                 | buf_p[payload_crc_size + 2]);
    crc = crc_ccitt(0xffff, buf_p, payload_crc_size + 1);

    std_printf(FSTR("crc: %04x\r\n"), crc);

    if (crc != input_crc) {
        return (-1);
    }

    type = buf_p[0];

    self_p->command.id_p = &buf_p[3];
    buf[0] = self_p->command.id_p[0];
    buf[1] = self_p->command.id_p[1];

    switch (type) {

    case SOAM_TYPE_COMMAND_REQUEST:
        res = fs_call((char *)self_p->command.id_p,
                      &self_p->command.chan,
                      &self_p->command.chan,
                      NULL);
        buf[2] = (res >> 24);
        buf[3] = (res >> 16);
        buf[4] = (res >> 8);
        buf[5] = res;

        if (soam_write(self_p, SOAM_TYPE_COMMAND_RESPONSE, &buf[0], 6) != 6) {
            return (-1);
        }

        break;

    default:
        return (-1);
    }

    return (0);
}

ssize_t soam_write_begin(struct soam_t *self_p,
                         int type)
{
    self_p->tx.buf_p[0] = type;
    self_p->tx.pos = 3;

    return (sem_take(&self_p->tx.sem, NULL));
}

ssize_t soam_write_chunk(struct soam_t *self_p,
                         const void *buf_p,
                         size_t size)
{
    if ((size > (self_p->tx.size - self_p->tx.pos - 2))
        || (self_p->tx.pos == -1)) {
        self_p->tx.pos = -1;

        return (-1);
    }

    memcpy(&self_p->tx.buf_p[self_p->tx.pos], buf_p, size);
    self_p->tx.pos += size;

    return (size);
}

ssize_t soam_write_end(struct soam_t *self_p)
{
    ssize_t size;
    size_t data_crc_size;
    uint16_t crc;

    if (self_p->tx.pos != -1) {
        size = (self_p->tx.pos + 2);
        data_crc_size = (size - 3);

        /* Write packet size and crc fields. */
        self_p->tx.buf_p[1] = (data_crc_size >> 8);
        self_p->tx.buf_p[2] = data_crc_size;

        crc = crc_ccitt(0xffff, &self_p->tx.buf_p[0], size - 2);

        self_p->tx.buf_p[size - 2] = (crc >> 8);
        self_p->tx.buf_p[size - 1] = crc;

        if (chan_write(self_p->tx.chout_p,
                       &self_p->tx.buf_p[0],
                       size) != size) {
            size = -1;
        } else {
            size -= 5;
        }
    } else {
        size = -1;
    }

    sem_give(&self_p->tx.sem, 1);

    return (size);
}

ssize_t soam_write(struct soam_t *self_p,
                   int type,
                   const void *buf_p,
                   size_t size)
{
    if (soam_write_begin(self_p, type) != 0) {
        return (-1);
    }

    (void)soam_write_chunk(self_p ,buf_p, size);

    return (soam_write_end(self_p));
}

void *soam_get_log_input_channel(struct soam_t *self_p)
{
    return (&self_p->log.chan);
}
