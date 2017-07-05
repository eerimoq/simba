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

struct xbee_driver_t xbee;
struct chan_t transport;

static struct queue_t queue;
static uint8_t queue_buf[256];

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(queue_init(&queue, &queue_buf[0], sizeof(queue_buf)) == 0);
    BTASSERT(chan_init(&transport,
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);
    BTASSERT(xbee_init(&xbee, &transport) == 0);

    return (0);
}

static int test_write_escape(struct harness_t *harness_p)
{
    struct xbee_frame_t frame;
    uint8_t buf[7];

    /* Prepare a frame where 0x11 will be escaped to 0x7d 0x31. */
    frame.type = 0x23;
    frame.data.buf[0] = 0x11;
    frame.data.size = 1;

    BTASSERT(xbee_write(&xbee, &frame) == 0);

    /* Validate the written frame. */
    harness_mock_read("chan_write(buf_p)", &buf[0], 1);
    harness_mock_read("chan_write(buf_p)", &buf[1], 1);
    harness_mock_read("chan_write(buf_p)", &buf[2], 1);
    harness_mock_read("chan_write(buf_p)", &buf[3], 1);
    harness_mock_read("chan_write(buf_p)", &buf[4], 2);
    harness_mock_read("chan_write(buf_p)", &buf[6], 1);

    BTASSERTM(&buf[0], "\x7e\x00\x02\x23\x7d\x31\xcb", 7);

    return (0);
}

static int test_write_at(struct harness_t *harness_p)
{
    struct xbee_frame_t frame;
    uint8_t buf[7];

    /* Prepare a frame reading the AT frame "DL". */
    frame.type = XBEE_FRAME_TYPE_AT_COMMAND;
    frame.data.buf[0] = 0x52;
    frame.data.buf[1] = 'D';
    frame.data.buf[2] = 'L';
    frame.data.size = 3;

    BTASSERT(xbee_write(&xbee, &frame) == 0);

    /* Validate the written frame. */
    harness_mock_read("chan_write(buf_p)", &buf[0], 1);
    harness_mock_read("chan_write(buf_p)", &buf[1], 1);
    harness_mock_read("chan_write(buf_p)", &buf[2], 1);
    harness_mock_read("chan_write(buf_p)", &buf[3], 1);
    harness_mock_read("chan_write(buf_p)", &buf[4], 1);
    harness_mock_read("chan_write(buf_p)", &buf[5], 1);
    harness_mock_read("chan_write(buf_p)", &buf[6], 1);
    harness_mock_read("chan_write(buf_p)", &buf[7], 1);

    BTASSERTM(&buf[0], "\x7e\x00\x04\x08\x52\x44\x4c\x15", 8);

    return (0);
}

static int test_read_unescape(struct harness_t *harness_p)
{
    struct xbee_frame_t frame;
    uint8_t buf[] = {
        0x7e, 0x00, 0x02, 0x23, 0x7d, 0x31, 0xcb
    };

    /* Prepare a frame from the XBee module where 0x7d 0x31 will be
       unescaped to 0x11. */
    harness_mock_write("chan_read(): return (buf_p)", &buf[0], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[1], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[2], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[3], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[4], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[5], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[6], 1);

    BTASSERT(xbee_read(&xbee, &frame) == 0);

    /* Validate the read frame. */
    BTASSERTI(frame.type, ==, 0x23);
    BTASSERTI(frame.data.buf[0], ==, 0x11);
    BTASSERTI(frame.data.size, ==, 1);

    return (0);
}

static int test_channel_write_tx_request(struct harness_t *harness_p)
{
    struct xbee_frame_t frame;
    uint8_t buf[14];

    /* Prepare a frame. */
    frame.type = XBEE_FRAME_TYPE_TX_REQUEST_16_BIT_ADDRESS;
    frame.data.buf[0] = 0x01;
    frame.data.buf[1] = 0x50;
    frame.data.buf[2] = 0x01;
    frame.data.buf[3] = 0x00;
    frame.data.buf[4] = 0x48;
    frame.data.buf[5] = 0x65;
    frame.data.buf[6] = 0x6c;
    frame.data.buf[7] = 0x6c;
    frame.data.buf[8] = 0x6f;
    frame.data.size = 9;

    BTASSERT(chan_write(&xbee, &frame, sizeof(frame)) == 0);

    /* Validate the written frame. */
    harness_mock_read("chan_write(buf_p)", &buf[0],  1);
    harness_mock_read("chan_write(buf_p)", &buf[1],  1);
    harness_mock_read("chan_write(buf_p)", &buf[2],  1);
    harness_mock_read("chan_write(buf_p)", &buf[3],  1);
    harness_mock_read("chan_write(buf_p)", &buf[4],  1);
    harness_mock_read("chan_write(buf_p)", &buf[5],  1);
    harness_mock_read("chan_write(buf_p)", &buf[6],  1);
    harness_mock_read("chan_write(buf_p)", &buf[7],  1);
    harness_mock_read("chan_write(buf_p)", &buf[8],  1);
    harness_mock_read("chan_write(buf_p)", &buf[9],  1);
    harness_mock_read("chan_write(buf_p)", &buf[10], 1);
    harness_mock_read("chan_write(buf_p)", &buf[11], 1);
    harness_mock_read("chan_write(buf_p)", &buf[12], 1);
    harness_mock_read("chan_write(buf_p)", &buf[13], 1);

    BTASSERTM(&buf[0],
              "\x7e\x00\x0a\x01\x01\x50\x01\x00\x48\x65\x6c\x6c\x6f\xb8",
              14);

    return (0);
}

static int test_channel_read_unescape(struct harness_t *harness_p)
{
    struct xbee_frame_t frame;
    uint8_t buf[] = {
        0x7e, 0x00, 0x02, 0x23, 0x7d, 0x31, 0xcb
    };

    /* Prepare a frame from the XBee module where 0x7d 0x31 will be
       unescaped to 0x11. */
    harness_mock_write("chan_read(): return (buf_p)", &buf[0], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[1], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[2], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[3], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[4], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[5], 1);
    harness_mock_write("chan_read(): return (buf_p)", &buf[6], 1);

    BTASSERT(chan_read(&xbee, &frame, sizeof(frame)) == 0);

    /* Validate the read frame. */
    BTASSERTI(frame.type, ==, 0x23);
    BTASSERTI(frame.data.buf[0], ==, 0x11);
    BTASSERTI(frame.data.size, ==, 1);

    return (0);
}

static int test_frame_type_as_string(struct harness_t *harness_p)
{
    const char *actual_p;
    const char *expected_p;

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_TX_REQUEST_64_BIT_ADDRESS);
    expected_p = "TX (Transmit) Request: 64-bit address";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_TX_REQUEST_16_BIT_ADDRESS);
    expected_p = "TX (Transmit) Request: 16-bit address";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_AT_COMMAND);
    expected_p = "AT Command";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_AT_COMMAND_QUEUE_PARAMETER_VALUE);
    expected_p = "AT Command - Queue Parameter Value";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_ZIGBEE_TRANSMIT_REQUEST);
    expected_p = "ZigBee Transmit Request";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_EXPLICIT_ADDRESSING_ZIGBEE_COMMAND_FRAME);
    expected_p = "Explicit Addressing ZigBee Command Frame";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_REMOTE_COMMAND_REQUEST);
    expected_p = "Remote Command Request";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_CREATE_SOURCE_ROUTE);
    expected_p = "Create Source Route";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS);
    expected_p = "RX Packet: 64-bit Address";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS);
    expected_p = "RX Packet: 16-bit Address";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS_IO);
    expected_p = "RX Packet: 64-bit Address I/O";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS_IO);
    expected_p = "RX Packet: 16-bit Address I/O";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_AT_COMMAND_RESPONSE);
    expected_p = "AT Command Response";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_TX_STATUS);
    expected_p = "TX (Transmit) Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_MODEM_STATUS);
    expected_p = "Modem Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(0xff);
    expected_p = "Unknown Command";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_ZIGBEE_TRANSMIT_STATUS);
    expected_p = "ZigBee Transmit Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_ZIGBEE_RECEIVE_PACKET_AO_0);
    expected_p = "ZigBee Receive Packet (AO=0)";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_ZIGBEE_EXPLICIT_RX_INDICATOR_AO_1);
    expected_p = "ZigBee Explicit Rx Indicator (AO=1)";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_ZIGBEE_IO_DATA_SAMPLE_RX_INDICATOR);
    expected_p = "ZigBee I/O Data Sample Rx Indicator";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_XBEE_SENSOR_READ_INDICATOR_AO_0);
    expected_p = "XBee Sensor Read Indicator (AO=0)";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_NODE_IDENTIFICATION_INDICATOR_AO_0);
    expected_p = "Node Identification Indicator (AO=0)";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_REMOTE_COMMAND_RESPONSE);
    expected_p = "Remote Command Response";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_EXTENDED_MODEM_STATUS);
    expected_p = "Extended Modem Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_OVER_THE_AIR_FIRMWARE_UPDATE_STATUS);
    expected_p = "Over-the-Air Firmware Update Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_ROUTE_RECORD_INDICATOR);
    expected_p = "Route Record Indicator";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_frame_type_as_string(XBEE_FRAME_TYPE_MANY_TO_ONE_ROUTE_REQUEST_INDICATOR);
    expected_p = "Many-to-One Route Request Indicator";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    return (0);
}

static int test_modem_status_as_string(struct harness_t *harness_p)
{
    const char *actual_p;
    const char *expected_p;

    actual_p = xbee_modem_status_as_string(0x00);
    expected_p = "Hardware reset";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_modem_status_as_string(0x01);
    expected_p = "Watchdog timer reset";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_modem_status_as_string(0x02);
    expected_p = "End device successfully associated with a coordinator";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_modem_status_as_string(0x03);
    expected_p = "End device disassociated from coordinator or coordinator "
        "failed to form a new network";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_modem_status_as_string(0x06);
    expected_p = "Coordinator formed a new network";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_modem_status_as_string(0x0d);
    expected_p = "Input voltage on the XBee-PRO device is too high, which "
        "prevents transmissions";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_modem_status_as_string(0xff);
    expected_p = "Unknown Modem Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    return (0);
}

static int test_at_command_response_status_as_string(struct harness_t *harness_p)
{
    const char *actual_p;
    const char *expected_p;

    actual_p = xbee_at_command_response_status_as_string(0x00);
    expected_p = "OK";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_at_command_response_status_as_string(0x01);
    expected_p = "ERROR";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_at_command_response_status_as_string(0x02);
    expected_p = "Invalid command";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_at_command_response_status_as_string(0x03);
    expected_p = "Invalid parameter";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_at_command_response_status_as_string(0x04);
    expected_p = "Unknown Command Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    return (0);
}

static int test_frame_as_string(struct harness_t *harness_p)
{
    struct xbee_frame_t frame;

    /* Modem status. */
    frame.type = XBEE_FRAME_TYPE_MODEM_STATUS;
    frame.data.buf[0] = 0x00;
    frame.data.size = 1;

    BTASSERT(xbee_print_frame(&queue, &frame) == 0);
    BTASSERTI(harness_expect(&queue,
                             "Modem Status(status='Hardware reset')\r\n",
                             NULL), ==, 39);

    /* AT Command - parameter read. */
    frame.type = XBEE_FRAME_TYPE_AT_COMMAND;
    frame.data.buf[0] = 0x01;
    frame.data.buf[1] = 'D';
    frame.data.buf[2] = 'L';
    frame.data.size = 3;

    BTASSERT(xbee_print_frame(&queue, &frame) == 0);
    BTASSERTI(harness_expect(&queue,
                             "AT Command(frame_id=0x01, at_command='DL')\r\n",
                             NULL), ==, 44);

    /* AT Command - parameter write. */
    frame.type = XBEE_FRAME_TYPE_AT_COMMAND;
    frame.data.buf[0] = 0x02;
    frame.data.buf[1] = 'D';
    frame.data.buf[2] = 'L';
    frame.data.buf[3] = 0x01;
    frame.data.buf[4] = 0x02;
    frame.data.buf[5] = 0x03;
    frame.data.buf[6] = 0x04;
    frame.data.size = 7;

    BTASSERT(xbee_print_frame(&queue, &frame) == 0);
    BTASSERTI(harness_expect(
                  &queue,
                  "AT Command(frame_id=0x02, at_command='DL', "
                  "parameter=0x01020304)\r\n",
                  NULL), ==, 66);

    /* AT Command Response - parameter read. */
    frame.type = XBEE_FRAME_TYPE_AT_COMMAND_RESPONSE;
    frame.data.buf[0] = 0x03;
    frame.data.buf[1] = 'D';
    frame.data.buf[2] = 'L';
    frame.data.buf[3] = 0x00;
    frame.data.buf[4] = 0x04;
    frame.data.buf[5] = 0x03;
    frame.data.buf[6] = 0x02;
    frame.data.buf[7] = 0x01;
    frame.data.size = 8;

    BTASSERT(xbee_print_frame(&queue, &frame) == 0);
    BTASSERTI(harness_expect(
                  &queue,
                  "AT Command Response(frame_id=0x03, at_command='DL', "
                  "status='OK', data=0x04030201)\r\n",
                  NULL), ==, 83);

    /* AT Command Response - parameter write. */
    frame.type = XBEE_FRAME_TYPE_AT_COMMAND_RESPONSE;
    frame.data.buf[0] = 0x03;
    frame.data.buf[1] = 'D';
    frame.data.buf[2] = 'L';
    frame.data.buf[3] = 0x00;
    frame.data.size = 4;

    BTASSERT(xbee_print_frame(&queue, &frame) == 0);
    BTASSERTI(harness_expect(
                  &queue,
                  "AT Command Response(frame_id=0x03, at_command='DL', "
                  "status='OK')\r\n",
                  NULL), ==, 66);

    /* RX Packet: 64-bit Address frame. */
    frame.type = XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS;
    frame.data.buf[0] = 0x12;
    frame.data.buf[1] = 0x34;
    frame.data.buf[2] = 0x56;
    frame.data.buf[3] = 0x78;
    frame.data.buf[4] = 0x9a;
    frame.data.buf[5] = 0xbc;
    frame.data.buf[6] = 0xde;
    frame.data.buf[7] = 0xf0;
    frame.data.buf[8] = 0x28;
    frame.data.buf[9] = 0x04;
    frame.data.buf[10] = 0x01;
    frame.data.buf[11] = 0x02;
    frame.data.buf[12] = 0x03;
    frame.data.size = 13;

    BTASSERT(xbee_print_frame(&queue, &frame) == 0);
    BTASSERTI(harness_expect(
                  &queue,
                  "RX Packet: 64-bit Address(address=0x123456789abcdef0, "
                  "rssi=40, options=[adddress_broadcast=0, "
                  "pan_broadcast=1], data=0x010203)\r\n",
                  NULL), ==, 128);

    /* RX Packet: 16-bit Address frame. */
    frame.type = XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS;
    frame.data.buf[0] = 0x12;
    frame.data.buf[1] = 0x34;
    frame.data.buf[2] = 0x28;
    frame.data.buf[3] = 0x02;
    frame.data.buf[4] = 0x01;
    frame.data.buf[5] = 0x02;
    frame.data.buf[6] = 0x03;
    frame.data.size = 7;

    BTASSERT(xbee_print_frame(&queue, &frame) == 0);
    BTASSERTI(harness_expect(
                  &queue,
                  "RX Packet: 16-bit Address(address=0x1234, rssi=40, "
                  "options=[adddress_broadcast=1, pan_broadcast=0], "
                  "data=0x010203)\r\n",
                  NULL), ==, 116);

    /* Unknown frame type. */
    frame.type = 0xff;
    frame.data.buf[0] = 'f';
    frame.data.buf[1] = 'o';
    frame.data.buf[2] = 'o';
    frame.data.size = 3;

    BTASSERT(xbee_print_frame(&queue, &frame) == -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "Unknown Command(\r\n"
                             "'00000000: 66 6f 6f                  "
                             "                      'foo'\r\n"
                             ")\r\n",
                             NULL), ==, 87);

    return (0);
}

ssize_t STUB(chan_read)(void *self_p,
                        void *buf_p,
                        size_t size)
{
    ssize_t res;

    if (harness_mock_read("chan_read(): return (res)",
                          &res,
                          sizeof(res)) == -1) {
        res = size;
    }

    harness_mock_read("chan_read(): return (buf_p)",
                      buf_p,
                      res);

    return (res);
}

ssize_t STUB(chan_write)(void *self_p,
                         const void *buf_p,
                         size_t size)
{
    ssize_t res;

    if (harness_mock_read("chan_write(): return (res)",
                          &res,
                          sizeof(res)) == -1) {
        res = size;
    }

    harness_mock_write("chan_write(buf_p)", buf_p, size);

    return (res);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_write_escape, "test_write_escape" },
        { test_write_at, "test_write_at" },
        { test_read_unescape, "test_read_unescape" },
        { test_channel_write_tx_request, "test_channel_write_tx_request" },
        { test_channel_read_unescape, "test_channel_read_unescape" },
        { test_frame_type_as_string, "test_frame_type_as_string" },
        { test_modem_status_as_string, "test_modem_status_as_string" },
        {
            test_at_command_response_status_as_string,
            "test_at_command_response_status_as_string"
        },
        { test_frame_as_string, "test_frame_as_string" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
