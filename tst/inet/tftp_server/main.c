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

extern void socket_stub_init(void);
extern void socket_stub_accept();
extern void socket_stub_input(int socket, void *buf_p, size_t size);
extern void socket_stub_output(void *buf_p, size_t size);
extern void socket_stub_wait_closed(void);
extern void socket_stub_close_connection(void);

static struct tftp_server_t server;
static THRD_STACK(listener_stack, 2048);

static int test_start(struct harness_t *harness_p)
{
    struct inet_addr_t addr;

    socket_stub_init();

    inet_aton("127.0.0.1", &addr.ip);
    addr.port = 69;

    BTASSERT(tftp_server_init(&server,
                              &addr,
                              50,
                              "tftp_server",
                              NULL,
                              listener_stack,
                              sizeof(listener_stack)) == 0);
    BTASSERT(tftp_server_start(&server) == 0);
    thrd_set_log_mask(server.thrd_p, LOG_UPTO(DEBUG));

    return (0);
}

static int test_read(struct harness_t *harness_p)
{
    struct fs_file_t file;
    uint8_t byte;
    uint8_t buf[516];
    int i;

    BTASSERT(fs_open(&file, "foo.txt", FS_WRITE | FS_CREAT | FS_TRUNC) == 0);

    byte = 0;

    for (i = 0; i < 1201; i++) {
        BTASSERT(fs_write(&file, &byte, 1) == 1);
        byte++;
    }

    BTASSERT(fs_close(&file) == 0);

    /* Input read request packet. */
    socket_stub_input(0, "\x00""\x01""foo.txt""\x00""octet""\x00", 16);

    /* Wait for the first data packet (bytes 0..511). */
    socket_stub_output(&buf[0], 516);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 3);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 1);

    byte = 0;

    for (i = 0; i < 512; i++) {
        BTASSERT(buf[4 + i] == byte);
        byte++;
    }

    /* Input bad acknowlegement packet. */
    socket_stub_input(1, "\x00""\x04""\x00""\x02", 4);

    /* Input acknowlegement packet. */
    socket_stub_input(1, "\x00""\x04""\x00""\x01", 4);

    /* Wait for the second data packet (bytes 512..1023). */
    socket_stub_output(&buf[0], 516);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 3);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 2);

    for (i = 0; i < 512; i++) {
        BTASSERT(buf[4 + i] == byte);
        byte++;
    }

    /* Input acknowlegement packet. */
    socket_stub_input(1, "\x00""\x04""\x00""\x02", 4);

    /* Wait for the second data packet (bytes 1024..1200). */
    socket_stub_output(&buf[0], 181);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 3);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 3);

    for (i = 0; i < 177; i++) {
        BTASSERT(buf[4 + i] == byte);
        byte++;
    }

    /* Input last acknowlegement packet. */
    socket_stub_input(1, "\x00""\x04""\x00""\x03", 4);

    thrd_sleep_ms(10);

    return (0);
}

static int test_write(struct harness_t *harness_p)
{
    struct fs_file_t file;
    uint8_t byte;
    uint8_t ref_byte;
    uint8_t buf[516];
    int i;

    /* Input write request packet. */
    socket_stub_input(0, "\x00""\x02""bar.txt""\x00""octet""\x00", 16);

    /* Wait for the first ack packet. */
    socket_stub_output(&buf[0], 4);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 4);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 0);

    /* Write the first data packet (bytes 0-511). */
    buf[0] = 0;
    buf[1] = 3;
    buf[2] = 0;
    buf[3] = 1;
    byte = 0;

    for (i = 0; i < 512; i++) {
        buf[4 + i] = byte;
        byte++;
    }

    socket_stub_input(2, &buf[0], 516);

    /* Wait for the second ack packet. */
    socket_stub_output(&buf[0], 4);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 4);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 1);

    /* Write the second data packet (bytes 512-1023). */
    buf[0] = 0;
    buf[1] = 3;
    buf[2] = 0;
    buf[3] = 2;
    byte = 0;

    for (i = 0; i < 512; i++) {
        buf[4 + i] = byte;
        byte++;
    }

    socket_stub_input(2, &buf[0], 516);

    /* Wait for the third ack packet. */
    socket_stub_output(&buf[0], 4);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 4);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 2);

    /* Write the third data packet (bytes 1024-1100) with wrong block
       number. */
    buf[0] = 0;
    buf[1] = 3;
    buf[2] = 0;
    buf[3] = 2;
    byte = 0;

    for (i = 0; i < 77; i++) {
        buf[4 + i] = byte;
        byte++;
    }

    socket_stub_input(2, &buf[0], 81);

    thrd_sleep_ms(10);

    /* Correct block number. */
    buf[3] = 3;
    socket_stub_input(2, &buf[0], 81);

    /* Wait for the third ack packet. */
    socket_stub_output(&buf[0], 4);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 4);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 3);

    thrd_sleep_ms(10);

    /* Verify the contents of the file created by the TFTP server. */
    BTASSERT(fs_open(&file, "bar.txt", FS_READ) == 0);

    byte = 0;

    for (i = 0; i < 1101; i++) {
        BTASSERT(fs_read(&file, &ref_byte, 1) == 1);
        BTASSERT(ref_byte == byte);
        byte++;
    }

    BTASSERT(fs_close(&file) == 0);

    return (0);
}

static int test_read_timeout(struct harness_t *harness_p)
{
    struct fs_file_t file;
    uint8_t byte;
    uint8_t buf[516];
    int i, j;

    BTASSERT(fs_open(&file, "foo.txt", FS_WRITE | FS_CREAT | FS_TRUNC) == 0);

    byte = 0;

    for (i = 0; i < 130; i++) {
        BTASSERT(fs_write(&file, &byte, 1) == 1);
        byte++;
    }

    BTASSERT(fs_close(&file) == 0);

    /* Input read request packet. */
    socket_stub_input(0, "\x00""\x01""foo.txt""\x00""octet""\x00", 16);

    /* Read the packet three times (first transmission + two
       retransmissions). */
    for (i = 0; i < 3; i++) {
        socket_stub_output(&buf[0], 134);
        BTASSERT(buf[0] == 0);
        BTASSERT(buf[1] == 3);
        BTASSERT(buf[2] == 0);
        BTASSERT(buf[3] == 1);

        byte = 0;

        for (j = 0; j < 130; j++) {
            BTASSERT(buf[4 + j] == byte);
            byte++;
        }
    }

    thrd_sleep_ms(10);

    return (0);
}

static int test_write_timeout(struct harness_t *harness_p)
{
    uint8_t buf[4];
    int i;

    /* Input write request packet. */
    socket_stub_input(0, "\x00""\x02""fie.txt""\x00""octet""\x00", 16);

    /* Wait for the first ack packet + 2 retransmissions. */
    for (i = 0; i < 3; i++) {
        socket_stub_output(&buf[0], 4);
        BTASSERT(buf[0] == 0);
        BTASSERT(buf[1] == 4);
        BTASSERT(buf[2] == 0);
        BTASSERT(buf[3] == 0);
    }

    thrd_sleep_ms(100);

    return (0);
}

static int test_bad_request(struct harness_t *harness_p)
{
    uint8_t buf[516];

    /* Input bad write request packets. */
    socket_stub_input(0, "\x00", 1);
    socket_stub_input(0, "\x00""\x07", 2);

    /* Filename missing. */
    socket_stub_input(0, "\x00""\x02", 2);

    socket_stub_output(&buf[0], 22);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 5);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 0);
    BTASSERT(strcmp("malformed request", (char *)&buf[4]) == 0);

    /* Mode missing. */
    socket_stub_input(0, "\x00""\x02""file", 6);

    socket_stub_output(&buf[0], 22);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 5);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 0);
    BTASSERT(strcmp("malformed request", (char *)&buf[4]) == 0);

    /* Mode missing #2. */
    socket_stub_input(0, "\x00""\x02""file""\x00", 7);

    socket_stub_output(&buf[0], 22);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 5);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 0);
    BTASSERT(strcmp("malformed request", (char *)&buf[4]) == 0);

    /* Bad mode. */
    socket_stub_input(0, "\x00""\x02""file""\x00""ooooo", 12);

    socket_stub_output(&buf[0], 31);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 5);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 0);
    BTASSERT(strcmp("only binary mode supported", (char *)&buf[4]) == 0);

    /* Read non-existing file. */
    socket_stub_input(0, "\x00""\x01""nonexisting""\x00""octet""\x00", 20);

    socket_stub_output(&buf[0], 19);
    BTASSERT(buf[0] == 0);
    BTASSERT(buf[1] == 5);
    BTASSERT(buf[2] == 0);
    BTASSERT(buf[3] == 1);
    BTASSERT(strcmp("file not found", (char *)&buf[4]) == 0);

    thrd_sleep_ms(10);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_start, "test_start" },
        { test_read, "test_read" },
        { test_write, "test_write" },
        { test_read_timeout, "test_read_timeout" },
        { test_write_timeout, "test_write_timeout" },
        { test_bad_request, "test_bad_request" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
