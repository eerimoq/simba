/**
 * @file main.c
 * @version 4.1.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

extern void socket_stub_init(void);
extern void socket_stub_input(void *buf_p, size_t size);
extern void socket_stub_output(void *buf_p, size_t size);

static struct http_websocket_client_t foo;

#if defined(ARCH_LINUX)
static uint8_t buf[131072];
#else
static uint8_t buf[256];
#endif

static int test_connect(struct harness_t *harness_p)
{
    char *str_p;

    socket_stub_init();

    BTASSERT(http_websocket_client_init(&foo,
                                        "localhost",
                                        8090,
                                        "/") == 0);

    /* Prepare the socket input to the websocket for the connect. */
    str_p =
        "HTTP/1.1 101\r\n"
        "\r\n";
    socket_stub_input(str_p, strlen(str_p));

    BTASSERT(http_websocket_client_connect(&foo) == 0);

    /* Verify the output data. */
    str_p =
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Upgrade: WebSocket\r\n"
        "Connection: Upgrade\r\n"
        "Origin: SimbaWebSocketClient\r\n"
        "\r\n";
    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp((char *)buf, str_p) == 0);

    return (0);
}

static int test_read(struct harness_t *harness_p)
{
    /* Prepare socket input with 1 length byte. */
    buf[0] = 0x81; /* FIN & TEXT. */
    buf[1] = 0x83; /* MASK and 1 byte payload length. */
    buf[2] = 0x00; /* Masking key 0. */
    buf[3] = 0x00; /* Masking key 1. */
    buf[4] = 0x00; /* Masking key 2. */
    buf[5] = 0x00; /* Masking key 3. */
    buf[6] = 'f'; /* Payload 0. */
    buf[7] = 'o'; /* Payload 1. */
    buf[8] = 'o'; /* Payload 2. */
    socket_stub_input(buf, 9);

    BTASSERT(http_websocket_client_read(&foo, buf, 3) == 3);
    BTASSERT(buf[0] == 'f');
    BTASSERT(buf[1] == 'o');
    BTASSERT(buf[2] == 'o');

    /* Prepare socket input with 2 length byte. */
    buf[0] = 0x81; /* FIN & TEXT. */
    buf[1] = 0xfe; /* MASK and 2 bytes payload length. */
    buf[2] = 0x00; /* Payload length 0. */
    buf[3] = 0x03; /* Payload length 1. */
    buf[4] = 0x00; /* Masking key 0. */
    buf[5] = 0x00; /* Masking key 1. */
    buf[6] = 0x00; /* Masking key 2. */
    buf[7] = 0x00; /* Masking key 3. */
    buf[8] = 'b'; /* Payload 0. */
    buf[9] = 'a'; /* Payload 1. */
    buf[10] = 'r'; /* Payload 2. */
    socket_stub_input(buf, 11);

    BTASSERT(http_websocket_client_read(&foo, buf, 3) == 3);
    BTASSERT(buf[0] == 'b');
    BTASSERT(buf[1] == 'a');
    BTASSERT(buf[2] == 'r');

    /* Prepare socket input with 8 length byte. */
    buf[0] = 0x81; /* FIN & TEXT. */
    buf[1] = 0xff; /* MASK and 8 bytes payload length. */
    buf[2] = 0x00; /* Payload length 0. */
    buf[3] = 0x00; /* Payload length 1. */
    buf[4] = 0x00; /* Payload length 2. */
    buf[5] = 0x00; /* Payload length 3. */
    buf[6] = 0x00; /* Payload length 4. */
    buf[7] = 0x00; /* Payload length 5. */
    buf[8] = 0x00; /* Payload length 6. */
    buf[9] = 0x03; /* Payload length 7. */
    buf[10] = 0x00; /* Masking key 0. */
    buf[11] = 0x00; /* Masking key 1. */
    buf[12] = 0x00; /* Masking key 2. */
    buf[13] = 0x00; /* Masking key 3. */
    buf[14] = 'f'; /* Payload 0. */
    buf[15] = 'i'; /* Payload 1. */
    buf[16] = 'e'; /* Payload 2. */
    socket_stub_input(buf, 17);

    BTASSERT(http_websocket_client_read(&foo, buf, 3) == 3);
    BTASSERT(buf[0] == 'f');
    BTASSERT(buf[1] == 'i');
    BTASSERT(buf[2] == 'e');

    return (0);
}

static int test_write(struct harness_t *harness_p)
{
    buf[0] = 'f';
    buf[1] = 'o';
    buf[2] = 'o';

    BTASSERT(http_websocket_client_write(&foo,
                                         HTTP_TYPE_BINARY,
                                         buf,
                                         3) == 3);

    /* Verify the output data. */
    socket_stub_output(buf, 6 + 3);
    BTASSERT(buf[0] == 0x82); /* FIN & BINARY. */
    BTASSERT(buf[1] == 0x83); /* MASK and 3 bytes of payload. */
    BTASSERT(buf[2] == 0x00); /* Masking key 0. */
    BTASSERT(buf[3] == 0x00); /* Masking key 1. */
    BTASSERT(buf[4] == 0x00); /* Masking key 2. */
    BTASSERT(buf[5] == 0x00); /* Masking key 3. */
    BTASSERT(buf[6] == 'f'); /* Payload 0. */
    BTASSERT(buf[7] == 'o'); /* Payload 1. */
    BTASSERT(buf[8] == 'o'); /* Payload 2. */

#if defined(ARCH_LINUX)

    buf[0] = 'b';
    buf[1] = 'a';
    buf[2] = 'r';

    BTASSERT(http_websocket_client_write(&foo,
                                         HTTP_TYPE_BINARY,
                                         buf,
                                         126) == 126);

    /* Prepare socket input with 2 length byte. */
    socket_stub_output(buf, 8 + 126);
    BTASSERT(buf[0] == 0x82); /* FIN & BINARY. */
    BTASSERT(buf[1] == 0xfe); /* MASK and 2 bytes payload length. */
    BTASSERT(buf[2] == 0x00); /* Payload length 0. */
    BTASSERT(buf[3] == 0x7e); /* Payload length 1. */
    BTASSERT(buf[4] == 0x00); /* Masking key 0. */
    BTASSERT(buf[5] == 0x00); /* Masking key 1. */
    BTASSERT(buf[6] == 0x00); /* Masking key 2. */
    BTASSERT(buf[7] == 0x00); /* Masking key 3. */
    BTASSERT(buf[8] == 'b'); /* Payload 0. */
    BTASSERT(buf[9] == 'a'); /* Payload 1. */
    BTASSERT(buf[10] == 'r'); /* Payload 2. */

    buf[0] = 'f';
    buf[1] = 'i';
    buf[2] = 'e';

    BTASSERT(http_websocket_client_write(&foo,
                                         HTTP_TYPE_BINARY,
                                         buf,
                                         65536) == 65536);

    /* Prepare socket input with 8 length byte. */
    socket_stub_output(buf, 14 + 65536);
    BTASSERT(buf[0] == 0x82); /* FIN & BINARY. */
    BTASSERT(buf[1] == 0xff); /* MASK and 8 bytes payload length. */
    BTASSERT(buf[2] == 0x00); /* Payload length 0. */
    BTASSERT(buf[3] == 0x00); /* Payload length 1. */
    BTASSERT(buf[4] == 0x00); /* Payload length 2. */
    BTASSERT(buf[5] == 0x00); /* Payload length 3. */
    BTASSERT(buf[6] == 0x00); /* Payload length 4. */
    BTASSERT(buf[7] == 0x01); /* Payload length 5. */
    BTASSERT(buf[8] == 0x00); /* Payload length 6. */
    BTASSERT(buf[9] == 0x00); /* Payload length 7. */
    BTASSERT(buf[10] == 0x00); /* Masking key 0. */
    BTASSERT(buf[11] == 0x00); /* Masking key 1. */
    BTASSERT(buf[12] == 0x00); /* Masking key 2. */
    BTASSERT(buf[13] == 0x00); /* Masking key 3. */
    BTASSERT(buf[14] == 'f'); /* Payload 0. */
    BTASSERT(buf[15] == 'i'); /* Payload 1. */
    BTASSERT(buf[16] == 'e'); /* Payload 2. */

#endif

    return (0);
}

static int test_disconnect(struct harness_t *harness_p)
{
    BTASSERT(http_websocket_client_disconnect(&foo) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_connect, "test_connect" },
        { test_read, "test_read" },
        { test_write, "test_write" },
        { test_disconnect, "test_disconnect" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
