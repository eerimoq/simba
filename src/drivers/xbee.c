/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

/* These have to be escaped. */
#define FRAME_DELIMITER                       0x7e
#define ESCAPE                                0x7d
#define XON                                   0x11
#define XOFF                                  0x13

static ssize_t write_bytes(struct xbee_driver_t *self_p,
                           const uint8_t *buf_p,
                           size_t size)
{
    ssize_t res;
    size_t i;
    uint8_t chunk[2];
    size_t chunk_size;

    for (i = 0; i < size; i++) {
        switch (buf_p[i]) {

        case FRAME_DELIMITER:
        case ESCAPE:
        case XON:
        case XOFF:
            chunk[0] = ESCAPE;
            chunk[1] = buf_p[i];
            chunk[1] ^= 0x20;
            chunk_size = 2;
            break;

        default:
            chunk[0] = buf_p[i];
            chunk_size = 1;
            break;
        }

        /* Write the bytes the the XBee module. */
        res = chan_write(self_p->transport_p, &chunk[0], chunk_size);
        
        if (res != chunk_size) {
            return (-EIO);
        }
    }
    
    return (size);
}

int xbee_module_init(void)
{
    return (0);
}

int xbee_init(struct xbee_driver_t *self_p,
              void *transport_p)
{
    self_p->transport_p = transport_p;

    return (0);
}

int xbee_read(struct xbee_driver_t *self_p,
              struct xbee_command_t *command_p)
{
    return (-ENOSYS);
}

int xbee_write(struct xbee_driver_t *self_p,
               const struct xbee_command_t *command_p)
{
    ssize_t res;
    uint8_t header[4];
    size_t i;
    uint8_t crc;
    
    /* Write the header. */
    header[0] = FRAME_DELIMITER;
    header[1] = (command_p->data.size >> 8);
    header[2] = command_p->data.size;
    header[3] = command_p->id;
    
    res = write_bytes(self_p, &header[0], sizeof(header));
    
    if (res != sizeof(header)) {
        return (-EIO);
    }

    /* Write the data. */
    res = write_bytes(self_p,
                      &command_p->data.buf[0],
                      command_p->data.size);
    
    if (res != command_p->data.size) {
        return (-EIO);
    }

    /* Calculate the CRC. */
    crc = 0;

    for (i = 0; i < command_p->data.size; i++) {
        crc += command_p->data.buf[i];
    }

    crc ^= 0xff;
    
    /* Write the checksum. */
    res = write_bytes(self_p, &crc, sizeof(crc));
    
    if (res != sizeof(crc)) {
        return (-EIO);
    }

    return (0);
}
