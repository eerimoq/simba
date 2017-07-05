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

int xbee_print_frame(void *chan_p, struct xbee_frame_t *frame_p)
{
    ASSERTN(chan_p != NULL, EINVAL);
    ASSERTN(frame_p != NULL, EINVAL);

    int res;
    size_t i;
    const char *delim_p;

    res = 0;

    /* Print the frame header. */
    std_fprintf(chan_p,
                OSTR("%s("),
                xbee_frame_type_as_string(frame_p->type));

    /* Print the frame data. */
    switch (frame_p->type) {

    case XBEE_FRAME_TYPE_AT_COMMAND:
        std_fprintf(chan_p,
                    OSTR("frame_id=0x%02x, at_command='%c%c'"),
                    frame_p->data.buf[0],
                    frame_p->data.buf[1],
                    frame_p->data.buf[2]);

        delim_p = ", parameter=0x";

        for (i = 0; i < frame_p->data.size - 3; i++, delim_p = "") {
            std_fprintf(chan_p,
                        OSTR("%s%02x"),
                        delim_p,
                        frame_p->data.buf[3 + i]);
        }
        break;

    case XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS:
        std_fprintf(chan_p,
                    OSTR("address=0x%02x%02x%02x%02x%02x%02x%02x%02x, "
                         "rssi=-%u dBm, "
                         "options=[adddress_broadcast=%u, pan_broadcast=%u]"),
                    frame_p->data.buf[0],
                    frame_p->data.buf[1],
                    frame_p->data.buf[2],
                    frame_p->data.buf[3],
                    frame_p->data.buf[4],
                    frame_p->data.buf[5],
                    frame_p->data.buf[6],
                    frame_p->data.buf[7],
                    frame_p->data.buf[8],
                    ((frame_p->data.buf[9] >> 1) & 0x1),
                    ((frame_p->data.buf[9] >> 2) & 0x1));

        delim_p = ", data=0x";

        for (i = 0; i < frame_p->data.size - 10; i++, delim_p = "") {
            std_fprintf(chan_p,
                        OSTR("%s%02x"),
                        delim_p,
                        frame_p->data.buf[10 + i]);
        }
        break;

    case XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS:
        std_fprintf(chan_p,
                    OSTR("address=0x%02x%02x, "
                         "rssi=-%u dBm, "
                         "options=[adddress_broadcast=%u, pan_broadcast=%u]"),
                    frame_p->data.buf[0],
                    frame_p->data.buf[1],
                    frame_p->data.buf[2],
                    ((frame_p->data.buf[3] >> 1) & 0x1),
                    ((frame_p->data.buf[3] >> 2) & 0x1));

        delim_p = ", data=0x";

        for (i = 0; i < frame_p->data.size - 4; i++, delim_p = "") {
            std_fprintf(chan_p,
                        OSTR("%s%02x"),
                        delim_p,
                        frame_p->data.buf[4 + i]);
        }
        break;

    case XBEE_FRAME_TYPE_AT_COMMAND_RESPONSE:
        std_fprintf(chan_p,
                    OSTR("frame_id=0x%02x, at_command='%c%c', "
                         "status='%s'"),
                    frame_p->data.buf[0],
                    frame_p->data.buf[1],
                    frame_p->data.buf[2],
                    xbee_at_command_response_status_as_string(
                        frame_p->data.buf[3]));

        delim_p = ", data=0x";

        for (i = 0; i < frame_p->data.size - 4; i++, delim_p = "") {
            std_fprintf(chan_p,
                        OSTR("%s%02x"),
                        delim_p,
                        frame_p->data.buf[4 + i]);
        }
        break;

    case XBEE_FRAME_TYPE_TX_STATUS:
        std_fprintf(chan_p,
                    OSTR("frame_id=0x%02x, status='%s'"),
                    frame_p->data.buf[0],
                    xbee_tx_status_as_string(frame_p->data.buf[1]));
        break;

    case XBEE_FRAME_TYPE_MODEM_STATUS:
        std_fprintf(chan_p,
                    OSTR("status='%s'"),
                    xbee_modem_status_as_string(frame_p->data.buf[0]));
        break;

    default:
        std_fprintf(chan_p, OSTR("\r\n'"));
        std_hexdump(chan_p, &frame_p->data.buf[0], frame_p->data.size);
        res = -EINVAL;
        break;
    }

    /* Print the frame footer. */
    std_fprintf(chan_p, OSTR(")\r\n"));

    return (res);
}

const char *xbee_frame_type_as_string(uint8_t frame_type)
{
    switch (frame_type) {

    case XBEE_FRAME_TYPE_TX_REQUEST_64_BIT_ADDRESS:
        return "TX Request: 64-bit address";

    case XBEE_FRAME_TYPE_TX_REQUEST_16_BIT_ADDRESS:
        return "TX Request: 16-bit address";

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
        return "RX Packet: 64-bit Address";

    case XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS:
        return "RX Packet: 16-bit Address";

    case XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS_IO:
        return "RX Packet: 64-bit Address I/O";

    case XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS_IO:
        return "RX Packet: 16-bit Address I/O";

    case XBEE_FRAME_TYPE_AT_COMMAND_RESPONSE:
        return "AT Command Response";

    case XBEE_FRAME_TYPE_TX_STATUS:
        return "TX Status";

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

const char *xbee_tx_status_as_string(uint8_t tx_status)
{
    switch (tx_status) {

    case 0x00:
        return "Standard";

    case 0x01:
        return "No ACK received";

    case 0x02:
        return "CCA failure";

    case 0x03:
        return "Transmission was purged because a coordinator tried "
            "to send to an end device, but it timed out waiting for "
            "a poll from the end device that never occurred";

    case 0x21:
        return "Network ACK failure";

    case 0x22:
        return "Transmission failed because an end device was not "
            "joined to the network";

    case 0x31:
        return "Internal error";

    case 0x32:
        return "Transmission failed due to resource depletion (for "
            "example, out of buffers, especially for indirect "
            "messages from coordinator)";

    case 0x74:
        return "The payload in the frame was larger than allowed";

    default:
        return "Unknown Status";
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

const char *xbee_at_command_response_status_as_string(uint8_t response_status)
{
    switch (response_status) {

    case 0x00:
        return "OK";

    case 0x01:
        return "ERROR";

    case 0x02:
        return "Invalid command";

    case 0x03:
        return "Invalid parameter";

    default:
        return "Unknown Command Status";
    }
}
