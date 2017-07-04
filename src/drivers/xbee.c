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

/**
 * Read data from the transport channel until a frame delimiter is
 * found.
 */
static int read_frame_delimiter(struct xbee_driver_t *self_p)
{
    int res;
    uint8_t byte;

    while (1) {
        res = chan_read(self_p->transport_p, &byte, sizeof(byte));

        if (res != sizeof(byte)) {
            res = -EIO;
            break;
        }

        if (byte == FRAME_DELIMITER) {
            res = 0;
            break;
        }
    }

    return (res);
}

/**
 * Read and unescape given number of command bytes from the transport
 * channel.
 */
static ssize_t read_bytes(struct xbee_driver_t *self_p,
                          uint8_t *buf_p,
                          size_t size)
{
    ssize_t res;
    size_t i;
    uint8_t byte;

    for (i = 0; i < size; i++) {
        res = chan_read(self_p->transport_p, &byte, sizeof(byte));

        if (res != sizeof(byte)) {
            return (-EIO);
        }

        /* Handle frame delimiters and escaped bytes. */
        if (byte == FRAME_DELIMITER) {
            return (-EIO);
        } else if (byte == ESCAPE) {
            res = chan_read(self_p->transport_p, &byte, sizeof(byte));

            if (res != sizeof(byte)) {
                return (-EIO);
            }

            byte ^= 0x20;

            /* The unescaped byte must be on of the following,
               otherwise it's a protocol error. */
            if ((byte != FRAME_DELIMITER)
                && (byte != ESCAPE)
                && (byte != XON)
                && (byte != XOFF)) {
                return (-EIO);
            }
        }

        buf_p[i] = byte;
    }

    return (size);
}

/**
 * Escape and write given number of command bytes to the transport
 * channel.
 */
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
    ssize_t res;
    uint8_t size[2];
    size_t i;
    uint8_t crc;

    /* Find a frame delimiter. */
    res = read_frame_delimiter(self_p);

    if (res != 0) {
        return (-EIO);
    }

    /* Read the size. */
    res = read_bytes(self_p, &size[0], sizeof(size));

    if (res != sizeof(size)) {
        return (-EIO);
    }

    command_p->data.size = ((size[0] << 8) | size[1]);

    if (command_p->data.size < 1) {
        return (-EIO);
    }

    /* Read the command id. */
    res = read_bytes(self_p, &command_p->id, sizeof(command_p->id));

    if (res != sizeof(command_p->id)) {
        return (-EIO);
    }

    /* The command id is not part of the data. */
    command_p->data.size--;

    if (command_p->data.size > sizeof(command_p->data.buf)) {
        return (-EIO);
    }

    /* Read the data. */
    res = read_bytes(self_p,
                     &command_p->data.buf[0],
                     command_p->data.size);

    if (res != command_p->data.size) {
        return (-EIO);
    }

    /* Read the CRC. */
    res = chan_read(self_p->transport_p, &crc, sizeof(crc));

    if (res != sizeof(crc)) {
        return (-EIO);
    }

    /* Calculate the CRC. */
    crc += command_p->id;

    for (i = 0; i < command_p->data.size; i++) {
        crc += command_p->data.buf[i];
    }

    return (crc != 0xff);
}

int xbee_write(struct xbee_driver_t *self_p,
               const struct xbee_command_t *command_p)
{
    ssize_t res;
    uint8_t header[3];
    size_t i;
    uint8_t crc;

    /* Write the frame delimiter. */
    header[0] = FRAME_DELIMITER;

    res = chan_write(self_p->transport_p, &header[0], sizeof(header[0]));

    if (res != sizeof(header[0])) {
        return (-EIO);
    }

    /* Write the header. */
    header[0] = (command_p->data.size >> 8);
    header[1] = (command_p->data.size + 1);
    header[2] = command_p->id;

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
    crc = command_p->id;

    for (i = 0; i < command_p->data.size; i++) {
        crc += command_p->data.buf[i];
    }

    crc ^= 0xff;

    /* Write the checksum. */
    res = chan_write(self_p->transport_p, &crc, sizeof(crc));

    if (res != sizeof(crc)) {
        return (-EIO);
    }

    return (0);
}

const char *xbee_command_id_as_string(uint8_t command_id)
{
    switch (command_id) {

    case XBEE_COMMAND_ID_TX_REQUEST_64_BIT_ADDRESS:
        return "TX (Transmit) Request: 64-bit address";

    case XBEE_COMMAND_ID_TX_REQUEST_16_BIT_ADDRESS:
        return "TX (Transmit) Request: 16-bit address";

    case XBEE_COMMAND_ID_AT_COMMAND:
        return "AT Command";

    case XBEE_COMMAND_ID_AT_COMMAND_QUEUE_PARAMETER_VALUE:
        return "AT Command - Queue Parameter Value";

    case XBEE_COMMAND_ID_RX_PACKET_64_BIT_ADDRESS:
        return "RX (Receive) Packet: 64-bit Address";

    case XBEE_COMMAND_ID_RX_PACKET_16_BIT_ADDRESS:
        return "RX (Receive) Packet: 16-bit Address";

    case XBEE_COMMAND_ID_AT_COMMAND_RESPONSE:
        return "AT Command Response";

    case XBEE_COMMAND_ID_TX_STATUS:
        return "TX (Transmit) Status";

    case XBEE_COMMAND_ID_MODEM_STATUS:
        return "Modem Status";

    default:
        return "Unknown Command";
    }
}
