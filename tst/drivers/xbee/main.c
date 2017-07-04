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

struct xbee_driver_t xbee;
struct chan_t transport;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(chan_init(&transport,
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);
    BTASSERT(xbee_init(&xbee, &transport) == 0);

    return (0);
}

static int test_write_escape(struct harness_t *harness_p)
{
    struct xbee_command_t command;
    uint8_t buf[7];

    /* Prepare a command where 0x11 will be escaped to 0x7d 0x31. */
    command.id = 0x23;
    command.data.buf[0] = 0x11;
    command.data.size = 1;

    BTASSERT(xbee_write(&xbee, &command) == 0);

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
    struct xbee_command_t command;
    uint8_t buf[7];

    /* Prepare a command reading the AT command "DL". */
    command.id = XBEE_COMMAND_ID_AT_COMMAND;
    command.data.buf[0] = 0x52;
    command.data.buf[1] = 'D';
    command.data.buf[2] = 'L';
    command.data.size = 3;

    BTASSERT(xbee_write(&xbee, &command) == 0);

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
    struct xbee_command_t command;
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

    BTASSERT(xbee_read(&xbee, &command) == 0);

    /* Validate the read command. */
    BTASSERTI(command.id, ==, 0x23);
    BTASSERTI(command.data.buf[0], ==, 0x11);
    BTASSERTI(command.data.size, ==, 1);

    return (0);
}

static int test_command_id_as_string(struct harness_t *harness_p)
{
    const char *actual_p;
    const char *expected_p;

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_TX_REQUEST_64_BIT_ADDRESS);
    expected_p = "TX (Transmit) Request: 64-bit address";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_TX_REQUEST_16_BIT_ADDRESS);
    expected_p = "TX (Transmit) Request: 16-bit address";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_AT_COMMAND);
    expected_p = "AT Command";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_AT_COMMAND_QUEUE_PARAMETER_VALUE);
    expected_p = "AT Command - Queue Parameter Value";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_RX_PACKET_64_BIT_ADDRESS);
    expected_p = "RX (Receive) Packet: 64-bit Address";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_RX_PACKET_16_BIT_ADDRESS);
    expected_p = "RX (Receive) Packet: 16-bit Address";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_AT_COMMAND_RESPONSE);
    expected_p = "AT Command Response";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_TX_STATUS);
    expected_p = "TX (Transmit) Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(XBEE_COMMAND_ID_MODEM_STATUS);
    expected_p = "Modem Status";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

    actual_p = xbee_command_id_as_string(0xff);
    expected_p = "Unknown Command";
    BTASSERTM(actual_p, expected_p, strlen(expected_p) + 1);

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
        { test_command_id_as_string, "test_command_id_as_string" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
