/**
 * @file main.c
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

static int test_aton(struct harness_t *harness_p)
{
    const char src[] = "1.2.3.4";
    const char bad_src_1[] = "1.2.g.4";
    const char bad_src_2[] = "1.2.3.";
    struct inet_ip_addr_t dst;

    BTASSERT(inet_aton(src, &dst) == 0);
    BTASSERT(dst.number == htonl(0x01020304));

    BTASSERT(inet_aton(bad_src_1, &dst) == -EINVAL);
    BTASSERT(inet_aton(bad_src_2, &dst) == -EINVAL);

    return (0);
}

static int test_ntoa(struct harness_t *harness_p)
{
    struct inet_ip_addr_t src;
    char dst[16];

    src.number = htonl(0x01020304);

    BTASSERT(inet_ntoa(&src, &dst[0]) == &dst[0]);
    BTASSERT(strcmp(dst, "1.2.3.4") == 0);

    return (0);
}

static int test_inet_checksum(struct harness_t *harness_p)
{
    char buf[8] = { 1, 5, 3, 9, 7, 13, 11, 17 };

    BTASSERT(inet_checksum(&buf, sizeof(buf)) == 0xe9d3);
    BTASSERT(inet_checksum(&buf, sizeof(buf) - 1) == 0xe9e4);

    /* One of the higher 16 bits in combined value in
       inet_checksum_end() is set. */
    buf[0] = 0xff;
    buf[1] = 0xff;
    buf[2] = 0xff;
    buf[3] = 0xff;
    buf[4] = 0x00;
    buf[5] = 0x01;
    BTASSERT(inet_checksum(&buf, 6) == 0xfffe);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_aton, "test_aton" },
        { test_ntoa, "test_ntoa" },
        { test_inet_checksum, "test_inet_checksum" },
        { NULL, NULL }
    };

    sys_start();
    inet_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
