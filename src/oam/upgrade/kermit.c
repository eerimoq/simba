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

#define START_OF_HEADING 0x01
#define END_OF_TEXT      0x03

#define SPACE            ' '

/* Packet types. */
#define PACKET_TYPE_SEND         'S'
#define PACKET_TYPE_DATA         'D'
#define PACKET_TYPE_ACK          'Y'
#define PACKET_TYPE_BREAK        'B'

/* Configuration. */
#define PACKET_LENGTH_MAX            94
#define TIMEOUT                       1
#define PAD_COUNT                     0
#define PAD_VALUE                   '\0'
#define END_OF_LINE                 '\r'
#define QUOTE_CONTROL                '#'
#define REFUSE                       'N'
#define CLOCK_CHECK_TYPE             '1'
#define LONG_PACKETS                  2
#define NO_WINDOWS                    0
#define LONG_PACKETS_LENGTH_MAX_MSB   1
#define LONG_PACKETS_LENGTH_MAX_LSB  94

struct upgrade_kermit_t {
    void *chin_p;
    void *chout_p;
    struct {
        uint8_t buf[256];
        size_t size;
    } input;
};

static struct upgrade_kermit_t module;
static struct fs_command_t cmd_application_kermit_load;

/**
 * Encode given binary value to a printable ascii value.
 */
static inline uint8_t encode(int value)
{
    return ((uint8_t)value + SPACE);
}

/**
 * Decode given printable ascii value to a binary value.
 */
static inline int decode(uint8_t value)
{
    return ((int)(value - SPACE));
}

/**
 * Unescape a data byte.
 */
static uint8_t unescape(uint8_t value)
{
    if ((value & 0x60) == 0x40) {
        return (value & 0xbf);
    } else if ((value & 0x7f) == 0x3f) {
        return (value | 0x40);
    } else {
        return (value);
    }
}

/**
 * Calculate the checksum of given buffer.
 */
static int checksum(uint8_t *buf_p, size_t length)
{
    int checksum = 0;

    while (length > 0) {
        checksum += *buf_p++;
        length--;
    }

    return ((checksum + ((checksum >> 6) & 0x03)) & 0x3f);
}

static int write_ack(int sequence_number)
{
    uint8_t response[6];

    /* Build the response packet. */
    response[0] = START_OF_HEADING;
    response[1] = encode(3);
    response[2] = encode(sequence_number);
    response[3] = PACKET_TYPE_ACK;
    response[4] = encode(checksum(&response[1], 3));
    response[5] = END_OF_LINE;

    chan_write(module.chout_p, response, sizeof(response));

    return (0);
}

/**
 * Handle a send packet.
 */
static int handle_send(int sequence_number)
{
    uint8_t response[20];

    /* Build the response packet. */
    response[0] = START_OF_HEADING;
    response[1] = encode(16);
    response[2] = encode(sequence_number);
    response[3] = PACKET_TYPE_ACK;
    response[4] = encode(PACKET_LENGTH_MAX);
    response[5] = encode(TIMEOUT);
    response[6] = encode(PAD_COUNT);
    response[7] = encode(PAD_VALUE);
    response[8] = encode(END_OF_LINE);
    response[9] = QUOTE_CONTROL;
    response[10] = REFUSE;
    response[11] = CLOCK_CHECK_TYPE;
    response[12] = REFUSE;
    response[13] = encode(LONG_PACKETS);
    response[14] = encode(NO_WINDOWS);
    response[15] = encode(LONG_PACKETS_LENGTH_MAX_MSB);
    response[16] = encode(LONG_PACKETS_LENGTH_MAX_LSB);
    response[17] = encode(checksum(&response[1], 16));
    response[18] = END_OF_LINE;

    chan_write(module.chout_p, response, 19);

    return (0);
}

/**
 * Handle a data packet.
 */
static int handle_data(int sequence_number)
{
    size_t i, j;

    /* Decode the buffer. */
    for (i = 0, j = 0; i < module.input.size; i++, j++) {
        if (module.input.buf[i] == QUOTE_CONTROL) {
            i++;
            module.input.buf[j] = unescape(module.input.buf[i]);
        } else {
            module.input.buf[j] = module.input.buf[i];
        }
    }

    if (upgrade_binary_upload(module.input.buf, j) != 0) {
        return (-1);
    }

    return (write_ack(sequence_number));
}

/**
 * Handle other packets.
 */
static int handle_default(int sequence_number)
{
    return (write_ack(sequence_number));
}

/**
 * Read a packet from the input channel and process it.
 */
static int handle_packet(void)
{
    uint8_t value;
    int length, length_msb, length_lsb;
    int sequence_number;
    int type;
    int actual_checksum, expected_checksum;
    int res;

    /* Wait for a packet. */
    while (1) {
        chan_read(module.chin_p, &value, sizeof(value));

        if (value == START_OF_HEADING) {
            break;
        } else if (value == END_OF_TEXT) {
            return (-1);
        }
    }

    actual_checksum = 0;

    /* Length. */
    chan_read(module.chin_p, &value, sizeof(value));
    length = decode(value);
    actual_checksum += value;

    /* Sequence number. */
    chan_read(module.chin_p, &value, sizeof(value));
    sequence_number = decode(value);
    actual_checksum += value;
    length--;

    /* Type. */
    chan_read(module.chin_p, &value, sizeof(value));
    type = value;
    actual_checksum += value;
    length--;

    /* Extended length. */
    if (length == -2) {
        chan_read(module.chin_p, &value, sizeof(value));
        actual_checksum += value;
        length_msb = decode(value);

        chan_read(module.chin_p, &value, sizeof(value));
        actual_checksum += value;
        length_lsb = decode(value);

        length = (95 * length_msb + length_lsb);

        chan_read(module.chin_p, &value, sizeof(value));
        /* Should compare the checksums here. */
        actual_checksum += value;
    }

    /* Read the packet. */
    module.input.size = 0;

    while (length > 1) {
        chan_read(module.chin_p, &value, sizeof(value));
        actual_checksum += value;
        length--;
        module.input.buf[module.input.size++] = value;
    }

    actual_checksum += ((actual_checksum >> 6) & 0x03);
    actual_checksum &= 0x3f;
    actual_checksum = encode(actual_checksum);

    /* Read the checksum. */
    chan_read(module.chin_p, &value, sizeof(value));
    expected_checksum = value;

    /* Comapare the actual checksum to the expected checksum. */
    if (actual_checksum != expected_checksum) {
        std_printf(FSTR("error: actual_checksum = 0x%02x, "
                        "expected_checksum = 0x%02x\r\n"),
                   actual_checksum,
                   expected_checksum);

        return (-1);
    }

    /* Read end character. */
    chan_read(module.chin_p, &value, sizeof(value));

    if (value != END_OF_LINE) {
        std_printf(FSTR("error: bad end character %d\r\n"), value);

        return (-1);
    }

    switch (type) {

    case PACKET_TYPE_SEND:
        res = handle_send(sequence_number);
        break;

    case PACKET_TYPE_DATA:
        res = handle_data(sequence_number);
        break;

    case PACKET_TYPE_BREAK:
        write_ack(sequence_number);
        std_fprintf(module.chout_p,
                    FSTR("File transfer completed successfully.\r\n"));
        res = 1;
        break;

    default:
        res = handle_default(sequence_number);
        break;
    }

    return (res);
}

/**
 * Shell command that receives a file over the Kermit file transfer
 * protocol and writes it to flash.
 */
static int cmd_application_kermit_load_cb(int argc,
                                          const char *argv[],
                                          void *out_p,
                                          void *in_p,
                                          void *arg_p,
                                          void *call_arg_p)
{
    return (upgrade_kermit_load_file());
}

int upgrade_kermit_init(void *chin_p,
                        void *chout_p)
{
    module.chin_p = chin_p;
    module.chout_p = chout_p;

    fs_command_init(&cmd_application_kermit_load,
                    CSTR("/oam/upgrade/kermit/upload"),
                    cmd_application_kermit_load_cb,
                    NULL);
    fs_command_register(&cmd_application_kermit_load);

    return (0);
}

int upgrade_kermit_load_file()
{
    int res;

    std_printf(FSTR("Ready to receive a file over the Kermit file "
                    "transfer protocol.\r\n"
                    "\r\n"
                    "In kermit; press '\\+c' and use the kermit command "
                    "'send' to start the file transfer. Type 'c' and press "
                    "Enter after the file transfer to return to the "
                    "bootloader shell.\r\n"));

    if (upgrade_binary_upload_begin() != 0) {
        return (-1);
    }

    while (1) {
        res = handle_packet();

        if (res == 1) {
            std_printf(FSTR("software download successful\r\n"));
            res = 0;
            break;
        } else if (res < 0) {
            std_printf(FSTR("error: software download failed\r\n"));
            break;
        }
    }

    if (upgrade_binary_upload_end() != 0) {
        return (-1);
    }

    return (res);
}
