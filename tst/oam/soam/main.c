/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#define SSTR1(string) "%d\x1f""%s\x1f\x80\x01"
#define SSTR2(string) "\x80\x02"
#define SSTR3(string) "/\x80\x03"

static struct soam_t soam;
static uint8_t txbuf[128];
static uint8_t buf[128];

static struct queue_t chout;
static struct fs_command_t cmd_foo;

static int cmd_foo_cb(int argc,
                      const char *argv[],
                      void *chout_p,
                      void *chin_p,
                      void *arg_p,
                      void *call_arg_p)
{
    std_fprintf(chout_p, SSTR2("foo\r\n"));

    return (-1);
}

static int test_init(struct harness_t *harness_p)
{
    void *chin_p;

    BTASSERT(fs_command_init(&cmd_foo, SSTR3("/foo"), cmd_foo_cb, NULL) == 0);
    BTASSERT(fs_command_register(&cmd_foo) == 0);

    BTASSERT(queue_init(&chout, &buf[0], sizeof(buf)) == 0);
    
    BTASSERT(soam_init(&soam,
                       &txbuf[0],
                       sizeof(txbuf),
                       &chout) == 0);

    chin_p = soam_get_log_input_channel(&soam);

    BTASSERT(chin_p != NULL);
    BTASSERT(log_set_default_handler_output_channel(chin_p) == 0);
    
    return (0);
}

static int test_log(struct harness_t *harness_p)
{
    uint8_t buf[64];
    size_t size;
    size_t offset;
    uint16_t crc;
    
    log_object_print(NULL,
                     LOG_INFO,
                     SSTR1("one: %d, erik: %s\r\n"),
                     1,
                     "erik");

    /* Read the SOAM packet written to the output channel from the log
       function. */
    BTASSERT(chan_read(&chout, &buf[0], 3) == 3);
    BTASSERT(buf[0] == 4);

    size = ((buf[1] << 8) | buf[2]);
    BTASSERT(size >= 32);
    offset = 3;

    /* Skip the log timestamp since its value is unknown. */
    BTASSERT(chan_read(&chout, &buf[offset], size - 31) == size - 31);
    offset += (size - 31);
    BTASSERT(chan_read(&chout, &buf[offset], 31) == 31);
    
    BTASSERT(memcmp(&buf[offset],
                    ":info:main:default: "
                    "1\x1f"
                    "erik\x1f"
                    "\x80\x01", 29) == 0);

    crc = ((buf[size + 1] << 8) | buf[size + 2]);
    BTASSERT(crc_ccitt(0xffff, &buf[0], size + 1) == crc);

    /* Another log point. */
    log_object_print(NULL, LOG_ERROR, SSTR2("foo\r\n"));

    /* Read the SOAM packet written to the output channel from the log
       function. */
    BTASSERT(chan_read(&chout, &buf[0], 3) == 3);
    BTASSERT(buf[0] == 4);

    size = ((buf[1] << 8) | buf[2]);
    BTASSERT(size >= 26);
    offset = 3;

    /* Skip the log timestamp since its value is unknown. */
    BTASSERT(chan_read(&chout, &buf[offset], size - 25) == size - 25);
    offset += (size - 25);
    BTASSERT(chan_read(&chout, &buf[offset], 25) == 25);
    
    BTASSERT(memcmp(&buf[offset], ":error:main:default: \x80\x02", 23) == 0);

    crc = ((buf[size + 1] << 8) | buf[size + 2]);
    BTASSERT(crc_ccitt(0xffff, &buf[0], size + 1) == crc);

    return (0);
}

static int test_command(struct harness_t *harness_p)
{
    uint8_t buf[64];
    size_t size;
    uint16_t crc;

    BTASSERT(soam_input(&soam,
                        (uint8_t *)"\x01"
                        "\x00\x05"
                        "\x80\x03\x00"
                        "\x99\xfc",
                        8) == 0);

    /* Read the type and size fields of the data response. */
    BTASSERT(chan_read(&chout, &buf[0], 3) == 3);
    BTASSERT(buf[0] == 2);

    size = ((buf[1] << 8) | buf[2]);
    BTASSERT(size == 6);

    /* Read the command id and payload. */
    BTASSERT(chan_read(&chout, &buf[3], size) == size);
    BTASSERT(buf[3] == 0x80);
    BTASSERT(buf[4] == 0x03);
    BTASSERT(buf[5] == 0x80);
    BTASSERT(buf[6] == 0x02);
    
    crc = ((buf[size + 1] << 8) | buf[size + 2]);
    BTASSERT(crc_ccitt(0xffff, &buf[0], size + 1) == crc);

    /* Read the command response packet (with code). */
    BTASSERT(chan_read(&chout, &buf[0], 3) == 3);
    BTASSERT(buf[0] == 3);

    size = ((buf[1] << 8) | buf[2]);
    BTASSERT(size == 8);

    BTASSERT(chan_read(&chout, &buf[3], size) == size);
    BTASSERT(buf[3] == 0x80);
    BTASSERT(buf[4] == 0x03);
    BTASSERT(buf[5] == 0xff);
    BTASSERT(buf[6] == 0xff);
    BTASSERT(buf[7] == 0xff);
    BTASSERT(buf[8] == 0xff);
    
    crc = ((buf[size + 1] << 8) | buf[size + 2]);
    BTASSERT(crc_ccitt(0xffff, &buf[0], size + 1) == crc);
    
    return (0);
}

static int test_bad_input(struct harness_t *harness_p)
{
    /* Short packet. */
    BTASSERT(soam_input(&soam, (uint8_t *)"1234", 4) == -1);

    /* CRC error. */
    BTASSERT(soam_input(&soam,
                        (uint8_t *)"\x01"
                        "\x00\x05"
                        "\x80\x03\x00"
                        "\xa5\xc1",
                        8) == -1);

    /* Bad type in the packet. */
    BTASSERT(soam_input(&soam,
                        (uint8_t *)"\x00"
                        "\x00\x05"
                        "\x80\x03\x00"
                        "\xdc\x5c",
                        8) == -1);

    /* Bad length in the packet. */
    BTASSERT(soam_input(&soam,
                        (uint8_t *)"\x01"
                        "\x00\x06"
                        "\x80\x03\x00"
                        "\x02\x20",
                        8) == -1);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_log, "test_log" },
        { test_command, "test_command" },
        { test_bad_input, "test_bad_input" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
