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

struct xbee_client_t xbee;
struct event_t event;
struct chan_t transport;
static uint8_t frame_rxbuf[256];
static THRD_STACK(client_stack, 512);

static struct queue_t queue;
static uint8_t queue_buf[256];

static int test_init(void)
{
    BTASSERT(xbee_client_module_init() == 0);
    BTASSERT(xbee_client_module_init() == 0);

    BTASSERT(event_init(&event) == 0);
    BTASSERT(queue_init(&queue, &queue_buf[0], sizeof(queue_buf)) == 0);

    BTASSERT(chan_init(&transport,
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);
    BTASSERT(xbee_client_init(&xbee,
                              &transport,
                              &transport,
                              &frame_rxbuf[0],
                              sizeof(frame_rxbuf),
                              0) == 0);

    thrd_spawn(xbee_client_main,
               &xbee,
               0,
               &client_stack[0],
               sizeof(client_stack));

    return (0);
}

static int test_rx_packet_16_bits_address(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    struct xbee_client_address_t sender;
    uint8_t buf[XBEE_DATA_MAX];
    ssize_t size;

    /* Prepare input frame. */
    frame.type = 0x81;
    frame.data.size = 7;
    memcpy(&frame.data.buf[0], "\x12\x34\x37\x05""foo", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    event_write(&event, &mask, sizeof(mask));

    /* Read the frame from the client. */
    size = xbee_client_read_from(&xbee,
                                 &buf[0],
                                 sizeof(buf),
                                 &sender);

    BTASSERTI(size, ==, 3);
    BTASSERTI(sender.type, ==, xbee_client_address_type_16_bits_t);
    BTASSERTM(&sender.buf[0], "\x12\x34", 2);
    BTASSERTM(&buf[0], "foo", size);

    return (0);
}

static int test_rx_packet_64_bits_address(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    struct xbee_client_address_t sender;
    uint8_t buf[XBEE_DATA_MAX];
    ssize_t size;

    /* Prepare input frame. */
    frame.type = 0x80;
    frame.data.size = 16;
    memcpy(&frame.data.buf[0],
           "\x88\x77\x66\x55\x44\x33\x22\x11"
           "\x00\x01""foobar", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    event_write(&event, &mask, sizeof(mask));

    /* Read the frame from the client. */
    size = xbee_client_read_from(&xbee,
                                 &buf[0],
                                 sizeof(buf),
                                 &sender);

    BTASSERTI(size, ==, 6);
    BTASSERTI(sender.type, ==, xbee_client_address_type_64_bits_t);
    BTASSERTM(&sender.buf[0], "\x88\x77\x66\x55\x44\x33\x22\x11", 8);
    BTASSERTM(&buf[0], "foobar", size);

    return (0);
}

static int test_rx_packet_16_bits_address_short(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    struct xbee_client_address_t sender;
    uint8_t buf[2];
    ssize_t size;

    /* Prepare input frame. */
    frame.type = 0x81;
    frame.data.size = 7;
    memcpy(&frame.data.buf[0], "\x12\x34\x37\x05""bar", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    event_write(&event, &mask, sizeof(mask));

    /* Read the frame from the client. */
    size = xbee_client_read_from(&xbee,
                                 &buf[0],
                                 sizeof(buf),
                                 &sender);

    BTASSERTI(size, ==, 2);
    BTASSERTI(sender.type, ==, xbee_client_address_type_16_bits_t);
    BTASSERTM(&sender.buf[0], "\x12\x34", 2);
    BTASSERTM(&buf[0], "ba", size);

    return (0);
}

static int test_rx_packet_64_bits_address_short(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    struct xbee_client_address_t sender;
    uint8_t buf[3];
    ssize_t size;

    /* Prepare input frame. */
    frame.type = 0x80;
    frame.data.size = 16;
    memcpy(&frame.data.buf[0],
           "\x88\x77\x66\x55\x44\x33\x22\x11"
           "\x00\x01""hello", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    event_write(&event, &mask, sizeof(mask));

    /* Read the frame from the client. */
    size = xbee_client_read_from(&xbee,
                                 &buf[0],
                                 sizeof(buf),
                                 &sender);

    BTASSERTI(size, ==, 3);
    BTASSERTI(sender.type, ==, xbee_client_address_type_64_bits_t);
    BTASSERTM(&sender.buf[0], "\x88\x77\x66\x55\x44\x33\x22\x11", 8);
    BTASSERTM(&buf[0], "hel", size);

    return (0);
}

static int test_rx_packet_16_bits_address_bad_size(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    struct time_t timeout;

    /* Prepare input frame. */
    frame.type = 0x81;
    frame.data.size = 3;
    memcpy(&frame.data.buf[0], "\x12\x34\x37", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    event_write(&event, &mask, sizeof(mask));

    /* Packet discarded in the client. */
    timeout.seconds = 0;
    timeout.nanoseconds = 1000000;
    BTASSERT(chan_poll(&xbee, &timeout) == NULL);

    return (0);
}

static int test_rx_packet_64_bits_address_bad_size(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    struct time_t timeout;

    /* Prepare input frame. */
    frame.type = 0x80;
    frame.data.size = 9;
    memcpy(&frame.data.buf[0],
           "\x88\x77\x66\x55\x44\x33\x22\x11"
           "\x00", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    event_write(&event, &mask, sizeof(mask));

    /* Packet discarded in the client. */
    timeout.seconds = 0;
    timeout.nanoseconds = 1000000;
    BTASSERT(chan_poll(&xbee, &timeout) == NULL);

    return (0);
}

static int test_rx_packet_16_bits_address_no_data(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    struct xbee_client_address_t sender;
    uint8_t buf[1];
    ssize_t size;

    /* Prepare input frame. */
    frame.type = 0x81;
    frame.data.size = 4;
    memcpy(&frame.data.buf[0], "\x12\x34\x37\x05", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    event_write(&event, &mask, sizeof(mask));

    /* Read the frame from the client. */
    size = xbee_client_read_from(&xbee,
                                 &buf[0],
                                 sizeof(buf),
                                 &sender);

    BTASSERTI(size, ==, 0);
    BTASSERTI(sender.type, ==, xbee_client_address_type_16_bits_t);
    BTASSERTM(&sender.buf[0], "\x12\x34", 2);

    return (0);
}

static int test_rx_packet_64_bits_address_no_data(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    struct xbee_client_address_t sender;
    uint8_t buf[1];
    ssize_t size;

    /* Prepare input frame. */
    frame.type = 0x80;
    frame.data.size = 10;
    memcpy(&frame.data.buf[0],
           "\x88\x77\x66\x55\x44\x33\x22\x11"
           "\x00\x01", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    event_write(&event, &mask, sizeof(mask));

    /* Read the frame from the client. */
    size = xbee_client_read_from(&xbee,
                                 &buf[0],
                                 sizeof(buf),
                                 &sender);

    BTASSERTI(size, ==, 0);
    BTASSERTI(sender.type, ==, xbee_client_address_type_64_bits_t);
    BTASSERTM(&sender.buf[0], "\x88\x77\x66\x55\x44\x33\x22\x11", 8);

    return (0);
}

static int test_tx_packet_16_bits_address(void)
{
    struct xbee_frame_t frame;
    struct xbee_client_address_t receiver;
    ssize_t size;
    uint32_t mask;

    /* Prepare the TX Status frame. */
    frame.type = 0x89;
    frame.data.size = 2;
    memcpy(&frame.data.buf[0], "\x01\x00", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Perform the write. */
    receiver.type = xbee_client_address_type_16_bits_t;
    receiver.buf[0] = 0x56;
    receiver.buf[1] = 0x78;

    size = xbee_client_write_to(&xbee,
                                "hello",
                                5,
                                0,
                                &receiver);
    BTASSERTI(size, ==, 5);

    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_TX_REQUEST_16_BIT_ADDRESS);
    BTASSERTI(frame.data.size, ==, 9);
    BTASSERTM(&frame.data.buf[0], "\x01\x56\x78\x00hello", frame.data.size);

    return (0);
}

static int test_tx_packet_64_bits_address_no_ack(void)
{
    struct xbee_frame_t frame;
    struct xbee_client_address_t receiver;
    ssize_t size;

    /* Perform the write. */
    receiver.type = xbee_client_address_type_64_bits_t;
    receiver.buf[0] = 0x56;
    receiver.buf[1] = 0x78;
    receiver.buf[2] = 0x9a;
    receiver.buf[3] = 0xbc;
    receiver.buf[4] = 0xde;
    receiver.buf[5] = 0xf0;
    receiver.buf[6] = 0x12;
    receiver.buf[7] = 0x34;

    size = xbee_client_write_to(&xbee,
                                "hello 64",
                                8,
                                XBEE_CLIENT_NO_ACK,
                                &receiver);
    BTASSERTI(size, ==, 8);

    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_TX_REQUEST_64_BIT_ADDRESS);
    BTASSERTI(frame.data.size, ==, 18);
    BTASSERTM(&frame.data.buf[0],
              "\x00\x56\x78\x9a\xbc\xde\xf0\x12\x34\x00hello 64",
              frame.data.size);

    return (0);
}

static int test_tx_packet_timeout(void)
{
    struct xbee_frame_t frame;
    struct xbee_client_address_t receiver;
    ssize_t size;

    /* Perform the write. */
    receiver.type = xbee_client_address_type_16_bits_t;
    receiver.buf[0] = 0x56;
    receiver.buf[1] = 0x78;

    size = xbee_client_write_to(&xbee,
                                "hello",
                                5,
                                0,
                                &receiver);
    BTASSERTI(size, ==, -ETIMEDOUT);

    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_TX_REQUEST_16_BIT_ADDRESS);
    BTASSERTI(frame.data.size, ==, 9);
    BTASSERTM(&frame.data.buf[0], "\x02\x56\x78\x00hello", frame.data.size);

    return (0);
}

static int test_pin_set_mode(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 4;
    memcpy(&frame.data.buf[0], "\x03""D0\x00", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Perform the AT command. */
    BTASSERTI(xbee_client_pin_set_mode(&xbee,
                                       0,
                                       XBEE_CLIENT_PIN_OUTPUT), ==, 0);

    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 4);
    BTASSERTM(&frame.data.buf[0], "\x03""D0\x04", frame.data.size);

    return (0);
}

static int test_pin_write_high(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 4;
    memcpy(&frame.data.buf[0], "\x04""D0\x00", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Perform the AT command. */
    BTASSERTI(xbee_client_pin_write(&xbee, 0, 1), ==, 0);

    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 4);
    BTASSERTM(&frame.data.buf[0], "\x04""D0\x05", frame.data.size);

    return (0);
}

static int test_pin_toggle(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;

    /* Prepare the two AT Command response frames. */

    /* Toggle low. */
    frame.type = 0x88;
    frame.data.size = 4;
    memcpy(&frame.data.buf[0], "\x05""D0\x00", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Toggle high. */
    frame.data.buf[0] = 6;
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Toggle twise. */
    BTASSERTI(xbee_client_pin_toggle(&xbee, 0), ==, 0);
    BTASSERTI(xbee_client_pin_toggle(&xbee, 0), ==, 0);

    /* Verify the two written frames. */
    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 4);
    BTASSERTM(&frame.data.buf[0], "\x05""D0\x04", frame.data.size);

    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 4);
    BTASSERTM(&frame.data.buf[0], "\x06""D0\x05", frame.data.size);

    return (0);
}

static int test_at_command_read_u8(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    uint8_t parameter;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 5;
    memcpy(&frame.data.buf[0], "\x07""DL\x00\xbe", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Execute the AT command. */
    parameter = 0;
    BTASSERTI(xbee_client_at_command_read_u8(&xbee,
                                             "DL",
                                             &parameter), ==, 0);
    BTASSERTI(parameter, ==, 0xbe);

    /* Verify the written frame. */
    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 3);
    BTASSERTM(&frame.data.buf[0], "\x07""DL", frame.data.size);

    return (0);
}

static int test_at_command_read_u16(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    uint16_t parameter;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 6;
    memcpy(&frame.data.buf[0], "\x08""DL\x00\xca\xfe", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Execute the AT command. */
    parameter = 0;
    BTASSERTI(xbee_client_at_command_read_u16(&xbee,
                                              "DL",
                                              &parameter), ==, 0);
    BTASSERTI(parameter, ==, 0xcafe);

    /* Verify the written frame. */
    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 3);
    BTASSERTM(&frame.data.buf[0], "\x08""DL", frame.data.size);

    return (0);
}

static int test_at_command_read_u32(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;
    uint32_t parameter;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 8;
    memcpy(&frame.data.buf[0],
           "\x09""DL\x00\xbe\xef\xba\xbe",
           frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Execute the AT command. */
    parameter = 0;
    BTASSERTI(xbee_client_at_command_read_u32(&xbee,
                                              "DL",
                                              &parameter), ==, 0);
    BTASSERTI(parameter, ==, 0xbeefbabe);

    /* Verify the written frame. */
    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 3);
    BTASSERTM(&frame.data.buf[0], "\x09""DL", frame.data.size);

    return (0);
}

static int test_at_command_write_u8(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 4;
    memcpy(&frame.data.buf[0], "\x0a""DL\x00", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Execute the AT command. */
    BTASSERTI(xbee_client_at_command_write_u8(&xbee, "DL", 0x0b), ==, 0);

    /* Verify the written frame. */
    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 4);
    BTASSERTM(&frame.data.buf[0], "\x0a""DL\x0b", frame.data.size);

    return (0);
}

static int test_at_command_write_u16(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 4;
    memcpy(&frame.data.buf[0], "\x0b""DL\x00", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Execute the AT command. */
    BTASSERTI(xbee_client_at_command_write_u16(&xbee,
                                               "IV",
                                               0x0b01), ==, 0);

    /* Verify the written frame. */
    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 5);
    BTASSERTM(&frame.data.buf[0], "\x0b""IV\x0b\x01", frame.data.size);

    return (0);
}

static int test_at_command_write_u32(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 4;
    memcpy(&frame.data.buf[0], "\x0c""DL\x00", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Execute the AT command. */
    BTASSERTI(xbee_client_at_command_write_u32(&xbee,
                                               "IP",
                                               0x12345678), ==, 0);

    /* Verify the written frame. */
    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 7);
    BTASSERTM(&frame.data.buf[0],
              "\x0c""IP\x12\x34\x56\x78",
              frame.data.size);

    return (0);
}

static int test_at_command_error(void)
{
    struct xbee_frame_t frame;
    uint32_t mask;

    /* Prepare the AT Command response frame. */
    frame.type = 0x88;
    frame.data.size = 4;
    memcpy(&frame.data.buf[0], "\x0d""DL\x01", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Execute the AT command. */
    BTASSERTI(xbee_client_at_command_write_u32(&xbee,
                                               "IP",
                                               0x12345678), ==, -EPROTO);

    /* Verify the written frame. */
    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_AT_COMMAND);
    BTASSERTI(frame.data.size, ==, 7);
    BTASSERTM(&frame.data.buf[0],
              "\x0d""IP\x12\x34\x56\x78",
              frame.data.size);

    return (0);
}

static int test_tx_packet_negative_response(void)
{
    struct xbee_frame_t frame;
    struct xbee_client_address_t receiver;
    uint32_t mask;

    /* Prepare the TX Status frame. */
    frame.type = 0x89;
    frame.data.size = 2;
    memcpy(&frame.data.buf[0], "\x0e\x01", frame.data.size);
    harness_mock_write("xbee_read(): return (frame)",
                       &frame,
                       sizeof(frame));
    mask = 1;
    harness_mock_write("xbee_write(): event mask", &mask, sizeof(mask));

    /* Perform the write. */
    receiver.type = xbee_client_address_type_16_bits_t;
    receiver.buf[0] = 0x56;
    receiver.buf[1] = 0x78;

    BTASSERTI(xbee_client_write_to(&xbee,
                                   "hello",
                                   5,
                                   0,
                                   &receiver), ==, -EPROTO);

    harness_mock_read("xbee_write(frame)", &frame, sizeof(frame));
    BTASSERTI(frame.type, ==, XBEE_FRAME_TYPE_TX_REQUEST_16_BIT_ADDRESS);
    BTASSERTI(frame.data.size, ==, 9);
    BTASSERTM(&frame.data.buf[0], "\x0e\x56\x78\x00hello", frame.data.size);

    return (0);
}

static int test_print_address(void)
{
    struct xbee_client_address_t address;

    /* Invalid address. */
    address.type = xbee_client_address_type_invalid_t;

    BTASSERT(xbee_client_print_address(&queue, &address) == 0);
    BTASSERTI(harness_expect(&queue,
                             "Address: invalid\r\n",
                             NULL), ==, 18);

    /* 16 bits address. */
    address.type = xbee_client_address_type_16_bits_t;
    address.buf[0] = 0x01;
    address.buf[1] = 0x02;

    BTASSERT(xbee_client_print_address(&queue, &address) == 0);
    BTASSERTI(harness_expect(&queue, "Address: 0x0102\r\n", NULL), ==, 17);

    /* 64 bits address. */
    address.type = xbee_client_address_type_64_bits_t;
    address.buf[0] = 0x07;
    address.buf[1] = 0x06;
    address.buf[2] = 0x05;
    address.buf[3] = 0x04;
    address.buf[4] = 0x03;
    address.buf[5] = 0x02;
    address.buf[6] = 0x01;
    address.buf[7] = 0x00;

    BTASSERT(xbee_client_print_address(&queue, &address) == 0);
    BTASSERTI(harness_expect(&queue,
                             "Address: 0x0706050403020100\r\n",
                             NULL), ==, 29);

    /* Bad enum value. */
    address.type = -1;

    BTASSERT(xbee_client_print_address(&queue, &address) == 0);
    BTASSERTI(harness_expect(&queue,
                             "Address: unknown\r\n",
                             NULL), ==, 18);

    return (0);
}

int STUB(xbee_module_init)()
{
    return (0);
}

int STUB(xbee_init)(struct xbee_driver_t *self_p,
                    void *chin_p,
                    void *chout_p)
{
    self_p->transport.chin_p = chin_p;
    self_p->transport.chout_p = chout_p;

    return (0);
}

int STUB(xbee_read)(struct xbee_driver_t *self_p,
                    struct xbee_frame_t *frame_p)
{
    uint32_t mask;

    mask = 1;
    event_read(&event, &mask, sizeof(mask));

    harness_mock_read("xbee_read(): return (frame)",
                      frame_p,
                      sizeof(*frame_p));

    return (0);
}

int STUB(xbee_write)(struct xbee_driver_t *self_p,
                     const struct xbee_frame_t *frame_p)
{
    uint32_t mask;

    harness_mock_write("xbee_write(frame)",
                       frame_p,
                       sizeof(*frame_p));

    if (harness_mock_try_read("xbee_write(): event mask",
                              &mask,
                              sizeof(mask)) == sizeof(mask)) {
        /* Signal that the response packet is available. */
        event_write(&event, &mask, sizeof(mask));
    }

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_init, "test_init" },
        { test_rx_packet_16_bits_address, "test_rx_packet_16_bits_address" },
        { test_rx_packet_64_bits_address, "test_rx_packet_64_bits_address" },
        {
            test_rx_packet_16_bits_address_short,
            "test_rx_packet_16_bits_address_short"
        },
        {
            test_rx_packet_64_bits_address_short,
            "test_rx_packet_64_bits_address_short"
        },
        {
            test_rx_packet_16_bits_address_bad_size,
            "test_rx_packet_16_bits_address_bad_size"
        },
        {
            test_rx_packet_64_bits_address_bad_size,
            "test_rx_packet_64_bits_address_bad_size"
        },
        {
            test_rx_packet_16_bits_address_no_data,
            "test_rx_packet_16_bits_address_no_data"
        },
        {
            test_rx_packet_64_bits_address_no_data,
            "test_rx_packet_64_bits_address_no_data"
        },
        { test_tx_packet_16_bits_address, "test_tx_packet_16_bits_address" },
        {
            test_tx_packet_64_bits_address_no_ack,
            "test_tx_packet_64_bits_address_no_ack"
        },
        { test_tx_packet_timeout, "test_tx_packet_timeout" },
        { test_pin_set_mode, "test_pin_set_mode" },
        { test_pin_write_high, "test_pin_write_high" },
        { test_pin_toggle, "test_pin_toggle" },
        { test_at_command_read_u8, "test_at_command_read_u8" },
        { test_at_command_read_u16, "test_at_command_read_u16" },
        { test_at_command_read_u32, "test_at_command_read_u32" },
        { test_at_command_write_u8, "test_at_command_write_u8" },
        { test_at_command_write_u16, "test_at_command_write_u16" },
        { test_at_command_write_u32, "test_at_command_write_u32" },
        { test_at_command_error, "test_at_command_error" },
        {
            test_tx_packet_negative_response,
            "test_tx_packet_negative_response"
        },
        { test_print_address, "test_print_address" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
