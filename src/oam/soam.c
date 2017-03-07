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

#define SOAM_PACKET_SIZE_MIN                                6

#define SOAM_TYPE_STDOUT                             (1 << 4)
#define SOAM_TYPE_LOG_POINT                          (2 << 4)
#define SOAM_TYPE_COMMAND_REQUEST                    (3 << 4)
#define SOAM_TYPE_COMMAND_RESPONSE_DATA_PRINTF       (4 << 4)
#define SOAM_TYPE_COMMAND_RESPONSE_DATA_BINARY       (5 << 4)
#define SOAM_TYPE_COMMAND_RESPONSE                   (6 << 4)
#define SOAM_TYPE_DATABASE_ID_REQUEST                (7 << 4)
#define SOAM_TYPE_DATABASE_ID_RESPONSE               (8 << 4)
#define SOAM_TYPE_INVALID_TYPE                       (9 << 4)

#define SOAM_PACKET_FLAGS_CONSECUTIVE                (1 << 1)
#define SOAM_PACKET_FLAGS_LAST                       (1 << 0)

/* The generated SOAM database id. */
extern char soam_database_id[];

/**
 * Finalize and output current packet to the output channel.
 */
static ssize_t packet_output(struct soam_t *self_p)
{
    ssize_t size;
    size_t payload_crc_size;
    uint16_t crc;

    size = (self_p->tx.pos + 2);
    payload_crc_size = (size - 4);

    /* Write packet size and crc fields. */
    self_p->tx.buf_p[2] = (payload_crc_size >> 8);
    self_p->tx.buf_p[3] = payload_crc_size;

    crc = crc_ccitt(0xffff, &self_p->tx.buf_p[0], size - 2);

    self_p->tx.buf_p[size - 2] = (crc >> 8);
    self_p->tx.buf_p[size - 1] = crc;

    if (chan_write(self_p->tx.chout_p,
                   &self_p->tx.buf_p[0],
                   size) != size) {
        return (-1);
    }

    return (size - 6);
}

static ssize_t printf_or_binary_write(struct soam_t *self_p,
                                      const void *buf_p,
                                      size_t size,
                                      int type)
{
    ssize_t res;

    if (self_p->is_printf == 1) {
        res = soam_write_chunk(self_p, buf_p, size);
    } else {
        (void)soam_write_begin(self_p, type);
        (void)soam_write_chunk(self_p, buf_p, size);
        res = soam_write_end(self_p);
    }

    return (res);
}

static int printf_or_binary_control(struct soam_t *self_p,
                                    int operation,
                                    int type)
{
    int res;

    switch (operation) {

    case CHAN_CONTROL_PRINTF_BEGIN:
        res = soam_write_begin(self_p, type);
        self_p->is_printf = 1;
        break;

    case CHAN_CONTROL_PRINTF_END:
        self_p->is_printf = 0;
        res = soam_write_end(self_p);
        break;

    default:
        res = -1;
        break;
    }

    return (res);
}

static int stdout_control(void *chan_p,
                          int operation)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, stdout_chan);

    return (printf_or_binary_control(self_p, operation, SOAM_TYPE_STDOUT));
}

static ssize_t stdout_write(void *chan_p,
                            const void *buf_p,
                            size_t size)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, stdout_chan);

    return (printf_or_binary_write(self_p, buf_p, size, SOAM_TYPE_STDOUT));
}

static int log_control(void *chan_p,
                       int operation)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, log_chan);

    switch (operation) {

    case CHAN_CONTROL_LOG_BEGIN:
        return (soam_write_begin(self_p, SOAM_TYPE_LOG_POINT));

    case CHAN_CONTROL_LOG_END:
        return (soam_write_end(self_p));

    default:
        return (-1);
    }
}

static ssize_t log_write(void *chan_p,
                         const void *buf_p,
                         size_t size)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, log_chan);

    return (soam_write_chunk(self_p, buf_p, size));
}

static int command_control(void *chan_p,
                           int operation)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, command_chan);

    return (printf_or_binary_control(self_p,
                                     operation,
                                     SOAM_TYPE_COMMAND_RESPONSE_DATA_PRINTF));
}

static ssize_t command_write(void *chan_p,
                             const void *buf_p,
                             size_t size)
{
    struct soam_t *self_p;

    self_p = container_of(chan_p, struct soam_t, command_chan);

    return (printf_or_binary_write(self_p,
                                   buf_p,
                                   size,
                                   SOAM_TYPE_COMMAND_RESPONSE_DATA_BINARY));
}

int soam_init(struct soam_t *self_p,
              void *buf_p,
              size_t size,
              void *chout_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size >= 5, EINVAL);
    ASSERTN(chout_p != NULL, EINVAL);

    self_p->tx.buf_p = buf_p;
    self_p->tx.size = size;
    self_p->tx.chout_p = chout_p;
    self_p->tx.packet_index = 1;

    sem_init(&self_p->tx.sem, 0, 1);

    self_p->is_printf = 0;

    chan_init(&self_p->stdout_chan,
              chan_read_null,
              stdout_write,
              chan_size_null);
    chan_set_control_cb(&self_p->stdout_chan, stdout_control);

    chan_init(&self_p->log_chan,
              chan_read_null,
              log_write,
              chan_size_null);
    chan_set_control_cb(&self_p->log_chan, log_control);

    chan_init(&self_p->command_chan,
              chan_read_null,
              command_write,
              chan_size_null);
    chan_set_control_cb(&self_p->command_chan, command_control);

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
    uint8_t buf[4];

    if (size < SOAM_PACKET_SIZE_MIN) {
        return (-1);
    }

    payload_crc_size = ((buf_p[2] << 8) | buf_p[3]);

    if (payload_crc_size > (size - 4)) {
        return (-1);
    }

    input_crc = ((buf_p[payload_crc_size + 2] << 8)
                 | buf_p[payload_crc_size + 3]);
    crc = crc_ccitt(0xffff, buf_p, payload_crc_size + 2);

    /* std_printf(FSTR("crc: %04x\r\n"), crc); */

    if (crc != input_crc) {
        return (-1);
    }

    type = buf_p[0];

    switch (type) {

    case SOAM_TYPE_DATABASE_ID_REQUEST:
        if (soam_write(self_p,
                       SOAM_TYPE_DATABASE_ID_RESPONSE,
                       &soam_database_id[0],
                       32) != 32) {
            return (-1);
        }
        break;

    case SOAM_TYPE_COMMAND_REQUEST:
        res = fs_call((char *)&buf_p[4],
                      &self_p->command_chan,
                      &self_p->command_chan,
                      NULL);
        buf[0] = (res >> 24);
        buf[1] = (res >> 16);
        buf[2] = (res >> 8);
        buf[3] = res;

        if (soam_write(self_p, SOAM_TYPE_COMMAND_RESPONSE, &buf[0], 4) != 4) {
            return (-1);
        }

        break;

    default:
        soam_write(self_p, SOAM_TYPE_INVALID_TYPE, buf_p, size);

        return (-1);
    }

    return (0);
}

ssize_t soam_write_begin(struct soam_t *self_p,
                         int type)
{
    sem_take(&self_p->tx.sem, NULL);

    /* First packet initialization. */
    self_p->tx.buf_p[0] = type;
    self_p->tx.buf_p[1] = self_p->tx.packet_index++;
    self_p->tx.pos = 4;

    return (0);
}

ssize_t soam_write_chunk(struct soam_t *self_p,
                         const void *buf_p,
                         size_t size)
{
    size_t left;
    size_t n;
    const uint8_t *b_p;

    b_p = buf_p;
    left = size;

    while (left > 0) {
        /* Output if the transmission buffer is full. */
        if (self_p->tx.pos == self_p->tx.size - 2) {
            if (packet_output(self_p) <= 0) {
                self_p->tx.pos = -1;
                return (-1);
            }

            /* Next packet initialization. */
            self_p->tx.buf_p[0] |= SOAM_PACKET_FLAGS_CONSECUTIVE;
            self_p->tx.buf_p[1] = self_p->tx.packet_index++;
            self_p->tx.pos = 4;
        }

        /* Copy data to the transmission buffer. */
        n = (self_p->tx.size - self_p->tx.pos - 2);

        if (left < n) {
            n = left;
        }

        memcpy(&self_p->tx.buf_p[self_p->tx.pos], b_p, n);
        b_p += n;
        self_p->tx.pos += n;
        left -= n;
    }

    return (size);
}

ssize_t soam_write_end(struct soam_t *self_p)
{
    ssize_t size;

    /* Output last packet, if any. */
    if (self_p->tx.pos != -1) {
        self_p->tx.buf_p[0] |= SOAM_PACKET_FLAGS_LAST;
        size = packet_output(self_p);
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

    (void)soam_write_chunk(self_p, buf_p, size);

    return (soam_write_end(self_p));
}

void *soam_get_log_input_channel(struct soam_t *self_p)
{
    return (&self_p->log_chan);
}

void *soam_get_stdout_input_channel(struct soam_t *self_p)
{
    return (&self_p->stdout_chan);
}
