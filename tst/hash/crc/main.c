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

static int test_crc_32(struct harness_t *harness_p)
{
    char string[] = "The quick brown fox jumps over the lazy dog";

    BTASSERT(crc_32(0, string, strlen(string)) == 0x414fa339);

    return (0);
}

static int test_crc_ccitt(struct harness_t *harness_p)
{
    uint16_t crc;

    /* CCITT with 0xffff as initial value. */
    BTASSERT(crc_ccitt(0xffff, "", 0) == 0xffff);
    BTASSERT(crc_ccitt(0xffff, "1", 1) == 0xc782);
    BTASSERT(crc_ccitt(0xffff, "123456789", 9) == 0x29b1);
    crc = crc_ccitt(0xffff, "12345", 5);
    BTASSERT(crc_ccitt(crc, "6789", 4) == 0x29b1);

    return (0);
}

static int test_crc_xmodem(struct harness_t *harness_p)
{
    uint16_t crc;

    /* XModem with 0x0000 as initial value. */
    BTASSERT(crc_xmodem(0x0000, "", 0) == 0x0000);
    BTASSERT(crc_xmodem(0x0000, "1", 1) == 0x2672);
    BTASSERT(crc_xmodem(0x0000, "123456789", 9) == 0x31c3);
    crc = crc_xmodem(0x0000, "12345", 5);
    BTASSERT(crc_xmodem(crc, "6789", 4) == 0x31c3);

    return (0);
}

static int test_crc_7(struct harness_t *harness_p)
{
    /* CRC-7 calculation. */
    BTASSERT(crc_7("", 0) == 0x01);
    BTASSERT(crc_7("1", 1) == 0x45);
    BTASSERT(crc_7("123456789", 9) == 0xeb);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_crc_32, "test_crc_32" },
        { test_crc_ccitt, "test_crc_ccitt" },
        { test_crc_xmodem, "test_crc_xmodem" },
        { test_crc_7, "test_crc_7" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
