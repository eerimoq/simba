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

int test_sha1(struct harness_t *harness_p)
{
    struct sha1_t foo;
    uint8_t hash[20];
    int i;
    struct {
        char *name_p;
        char *input_p;
        char *hash_p;
    } testdata[] = {

        {
            .name_p = "Empty",
            .input_p = "",
            .hash_p =
            "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55"
            "\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09"
        },

        {
            .name_p = "Abc",
            .input_p = "abc",
            .hash_p =
            "\xa9\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e"
            "\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x9d"
        },

        {
            .name_p = "Dog",
            .input_p = "The quick brown fox jumps over the lazy dog",
            .hash_p =
            "\x2f\xd4\xe1\xc6\x7a\x2d\x28\xfc\xed\x84"
            "\x9e\xe1\xbb\x76\xe7\x39\x1b\x93\xeb\x12"
        },

        {
            .name_p = "Cog",
            .input_p = "The quick brown fox jumps over the lazy cog",
            .hash_p =
            "\xde\x9f\x2c\x7f\xd2\x5e\x1b\x3a\xfa\xd3"
            "\xe8\x5a\x0b\xd1\x7d\x9b\x10\x0d\xb4\xb3"
        },

        {
            .name_p = "60",
            .input_p =
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            .hash_p =
            "\x13\xd9\x56\x03\x3d\x9a\xf4\x49\xbf\xe2"
            "\xc4\xef\x78\xc1\x7c\x20\x46\x9c\x4b\xf1"
        },

        {
            .name_p = "Long",
            .input_p =
            "abcdefghbcdefghicdefghijdefghijkefghijklfgh"
            "ijklmghijklmnhijklmnoijklmnopjklmnopqklmnop"
            "qrlmnopqrsmnopqrstnopqrstu",
            .hash_p =
            "\xa4\x9b\x24\x46\xa0\x2c\x64\x5b\xf4\x19"
            "\xf9\x95\xb6\x70\x91\x25\x3a\x04\xa2\x59"
        }
    };

    /* Test vectors. */
    for (i = 0; i < membersof(testdata); i++) {
        std_printf(FSTR("%s\r\n"), testdata[i].name_p);

        BTASSERT(sha1_init(&foo) == 0);
        BTASSERT(sha1_update(&foo,
                                  testdata[i].input_p,
                                  strlen(testdata[i].input_p)) == 0);
        BTASSERT(sha1_digest(&foo, hash) == 0);
        
        BTASSERT(memcmp(hash, testdata[i].hash_p, 20) == 0);
    }

    /* Multiple updates. */
    BTASSERT(sha1_init(&foo) == 0);

    for (i = 0; i < 400; i++) {
        BTASSERT(sha1_update(&foo, "1", 1) == 0);
    }

    BTASSERT(sha1_digest(&foo, hash) == 0);

    BTASSERT(memcmp(hash,
                    "\x7f\xdd\xab\x82\x28\xdf\x0f\x39\x77\xed"
                    "\xf7\x3b\xb8\x06\x86\x11\x59\xad\x89\xd1",
                    20) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_sha1, "test_sha1" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
