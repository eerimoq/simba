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
 * Read and unescape given number of frame bytes from the transport
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
            return (-EPROTO);
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
                return (-EPROTO);
            }
        }

        buf_p[i] = byte;
    }

    return (size);
}

/**
 * Escape and write given number of frame bytes to the transport
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

static ssize_t base_chan_read(void *base_p,
                              void *buf_p,
                              size_t size)
{
    struct xbee_driver_t *self_p;
    struct xbee_frame_t *frame_p;

    ASSERTN(size == sizeof(*frame_p), EINVAL);

    self_p = base_p;
    frame_p = buf_p;

    return (xbee_read(self_p, frame_p));
}

static ssize_t base_chan_write(void *base_p,
                               const void *buf_p,
                               size_t size)
{
    struct xbee_driver_t *self_p;
    const struct xbee_frame_t *frame_p;

    ASSERTN(size == sizeof(*frame_p), EINVAL);

    self_p = base_p;
    frame_p = buf_p;

    return (xbee_write(self_p, frame_p));
}

static size_t base_chan_size(void *base_p)
{
    struct xbee_driver_t *self_p;

    self_p = base_p;

    return (chan_size(self_p->transport_p));
}

int xbee_module_init(void)
{
    return (0);
}

int xbee_init(struct xbee_driver_t *self_p,
              void *transport_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(transport_p != NULL, EINVAL);

    chan_init(&self_p->base,
              base_chan_read,
              base_chan_write,
              base_chan_size);

    self_p->transport_p = transport_p;

    return (0);
}

int xbee_read(struct xbee_driver_t *self_p,
              struct xbee_frame_t *frame_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(frame_p != NULL, EINVAL);

    ssize_t res;
    uint8_t size[2];
    size_t i;
    uint8_t crc;

    /* Find a frame delimiter. */
    res = read_frame_delimiter(self_p);

    if (res != 0) {
        return (res);
    }

    /* Read the size. */
    res = read_bytes(self_p, &size[0], sizeof(size));

    if (res != sizeof(size)) {
        return (res);
    }

    frame_p->data.size = ((size[0] << 8) | size[1]);

    if (frame_p->data.size < 1) {
        return (-EPROTO);
    }

    /* Read the frame id. */
    res = read_bytes(self_p, &frame_p->type, sizeof(frame_p->type));

    if (res != sizeof(frame_p->type)) {
        return (res);
    }

    /* The frame id is not part of the data. */
    frame_p->data.size--;

    if (frame_p->data.size > sizeof(frame_p->data.buf)) {
        return (-ENOMEM);
    }

    /* Read the data. */
    res = read_bytes(self_p,
                     &frame_p->data.buf[0],
                     frame_p->data.size);

    if (res != frame_p->data.size) {
        return (res);
    }

    /* Read the CRC. */
    res = chan_read(self_p->transport_p, &crc, sizeof(crc));

    if (res != sizeof(crc)) {
        return (-EIO);
    }

    /* Calculate the CRC. */
    crc += frame_p->type;

    for (i = 0; i < frame_p->data.size; i++) {
        crc += frame_p->data.buf[i];
    }

    if (crc != 0xff) {
        return (-EPROTO);
    }

    return (0);
}

int xbee_write(struct xbee_driver_t *self_p,
               const struct xbee_frame_t *frame_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(frame_p != NULL, EINVAL);

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
    header[0] = (frame_p->data.size >> 8);
    header[1] = (frame_p->data.size + 1);
    header[2] = frame_p->type;

    res = write_bytes(self_p, &header[0], sizeof(header));

    if (res != sizeof(header)) {
        return (res);
    }

    /* Write the data. */
    res = write_bytes(self_p,
                      &frame_p->data.buf[0],
                      frame_p->data.size);

    if (res != frame_p->data.size) {
        return (res);
    }

    /* Calculate the CRC. */
    crc = frame_p->type;

    for (i = 0; i < frame_p->data.size; i++) {
        crc += frame_p->data.buf[i];
    }

    crc ^= 0xff;

    /* Write the checksum. */
    res = chan_write(self_p->transport_p, &crc, sizeof(crc));

    if (res != sizeof(crc)) {
        return (-EIO);
    }

    return (0);
}

const char *xbee_frame_type_as_string(uint8_t frame_type)
{
    switch (frame_type) {

    case XBEE_FRAME_TYPE_TX_REQUEST_64_BIT_ADDRESS:
        return "TX (Transmit) Request: 64-bit address";

    case XBEE_FRAME_TYPE_TX_REQUEST_16_BIT_ADDRESS:
        return "TX (Transmit) Request: 16-bit address";

    case XBEE_FRAME_TYPE_AT_COMMAND:
        return "AT Command";

    case XBEE_FRAME_TYPE_AT_COMMAND_QUEUE_PARAMETER_VALUE:
        return "AT Command - Queue Parameter Value";

    case XBEE_FRAME_TYPE_ZIGBEE_TRANSMIT_REQUEST:
        return "ZigBee Transmit Request";

    case XBEE_FRAME_TYPE_EXPLICIT_ADDRESSING_ZIGBEE_COMMAND_FRAME:
        return "Explicit Addressing ZigBee Command Frame";

    case XBEE_FRAME_TYPE_REMOTE_COMMAND_REQUEST:
        return "Remote Command Request";

    case XBEE_FRAME_TYPE_CREATE_SOURCE_ROUTE:
        return "Create Source Route";

    case XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS:
        return "RX (Receive) Packet: 64-bit Address";

    case XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS:
        return "RX (Receive) Packet: 16-bit Address";

    case XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS_IO:
        return "RX Packet: 64-bit Address I/O";

    case XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS_IO:
        return "RX Packet: 16-bit Address I/O";

    case XBEE_FRAME_TYPE_AT_COMMAND_RESPONSE:
        return "AT Command Response";

    case XBEE_FRAME_TYPE_TX_STATUS:
        return "TX (Transmit) Status";

    case XBEE_FRAME_TYPE_MODEM_STATUS:
        return "Modem Status";

    case XBEE_FRAME_TYPE_ZIGBEE_TRANSMIT_STATUS:
        return "ZigBee Transmit Status";

    case XBEE_FRAME_TYPE_ZIGBEE_RECEIVE_PACKET_AO_0:
        return "ZigBee Receive Packet (AO=0)";

    case XBEE_FRAME_TYPE_ZIGBEE_EXPLICIT_RX_INDICATOR_AO_1:
        return "ZigBee Explicit Rx Indicator (AO=1)";

    case XBEE_FRAME_TYPE_ZIGBEE_IO_DATA_SAMPLE_RX_INDICATOR:
        return "ZigBee I/O Data Sample Rx Indicator";

    case XBEE_FRAME_TYPE_XBEE_SENSOR_READ_INDICATOR_AO_0:
        return "XBee Sensor Read Indicator (AO=0)";

    case XBEE_FRAME_TYPE_NODE_IDENTIFICATION_INDICATOR_AO_0:
        return "Node Identification Indicator (AO=0)";

    case XBEE_FRAME_TYPE_REMOTE_COMMAND_RESPONSE:
        return "Remote Command Response";

    case XBEE_FRAME_TYPE_EXTENDED_MODEM_STATUS:
        return "Extended Modem Status";

    case XBEE_FRAME_TYPE_OVER_THE_AIR_FIRMWARE_UPDATE_STATUS:
        return "Over-the-Air Firmware Update Status";

    case XBEE_FRAME_TYPE_ROUTE_RECORD_INDICATOR:
        return "Route Record Indicator";

    case XBEE_FRAME_TYPE_MANY_TO_ONE_ROUTE_REQUEST_INDICATOR:
        return "Many-to-One Route Request Indicator";

    default:
        return "Unknown Command";
    }
}

const char *xbee_modem_status_as_string(uint8_t modem_status)
{
    switch (modem_status) {

    case 0x00:
        return "Hardware reset";

    case 0x01:
        return "Watchdog timer reset";

    case 0x02:
        return "End device successfully associated with a coordinator";

    case 0x03:
        return "End device disassociated from coordinator or coordinator "
            "failed to form a new network";

    case 0x06:
        return "Coordinator formed a new network";

    case 0x0d:
        return "Input voltage on the XBee-PRO device is too high, which "
            "prevents transmissions";

    default:
        return "Unknown Modem Status";
    }
}

int xbee_print_frame(void *chan_p, struct xbee_frame_t *frame_p)
{
    ASSERTN(chan_p != NULL, EINVAL);
    ASSERTN(frame_p != NULL, EINVAL);

    int res;

    res = 0;

    switch (frame_p->type) {

    case XBEE_FRAME_TYPE_MODEM_STATUS:
        std_fprintf(chan_p,
                    OSTR("%s(status='%s')\r\n"),
                    xbee_frame_type_as_string(frame_p->type),
                    xbee_modem_status_as_string(frame_p->data.buf[0]));
        break;

    case XBEE_FRAME_TYPE_AT_COMMAND:
        std_fprintf(chan_p,
                    OSTR("%s(frame_id=0x%02x, at_command='%c%c')\r\n"),
                    xbee_frame_type_as_string(frame_p->type),
                    frame_p->data.buf[0],
                    frame_p->data.buf[1],
                    frame_p->data.buf[2]);
        break;

    default:
        std_fprintf(chan_p,
                    OSTR("Unknown frame type 0x%02x.\r\n"),
                    frame_p->type);
        res = -EINVAL;
        break;
    }

    return (res);
}
