/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

static int test_to_bin(void)
{
    uint8_t encoded[8];

    BTASSERT(hex_to_bin(&encoded[0], "0123456789abcdef", 16) == 8);
    BTASSERT(memcmp(&encoded[0], "\x01\x23\x45\x67\x89\xab\xcd\xef", 8) == 0);

    BTASSERT(hex_to_bin(&encoded[0], "0123456789ABCDEF", 16) == 8);
    BTASSERT(memcmp(&encoded[0], "\x01\x23\x45\x67\x89\xab\xcd\xef", 8) == 0);

    return (0);
}

static int test_to_bin_non_hex_character(void)
{
    uint8_t encoded[1];

    BTASSERT(hex_to_bin(&encoded[0], "/0", 2) == -EINVAL);
    BTASSERT(hex_to_bin(&encoded[0], "9:", 2) == -EINVAL);
    BTASSERT(hex_to_bin(&encoded[0], "`a", 2) == -EINVAL);
    BTASSERT(hex_to_bin(&encoded[0], "fg", 2) == -EINVAL);
    BTASSERT(hex_to_bin(&encoded[0], "@A", 2) == -EINVAL);
    BTASSERT(hex_to_bin(&encoded[0], "FG", 2) == -EINVAL);

    return (0);
}

static int test_from_bin(void)
{
    char decoded[17];

    BTASSERT(hex_from_bin(&decoded[0],
                          "\x01\x23\x45\x67\x89\xab\xcd\xef",
                          8) == 16);
    BTASSERT(strcmp(&decoded[0], "0123456789abcdef") == 0);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_to_bin, "test_to_bin" },
        { test_to_bin_non_hex_character, "test_to_bin_non_hex_character" },
        { test_from_bin, "test_from_bin" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
