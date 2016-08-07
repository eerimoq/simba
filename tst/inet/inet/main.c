/**
 * @file main.c
 * @version 5.0.0
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

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_aton, "test_aton" },
        { test_ntoa, "test_ntoa" },
        { NULL, NULL }
    };

    sys_start();
    inet_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
