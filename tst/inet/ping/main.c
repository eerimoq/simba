/**
 * @file main.c
 * @version 6.0.0
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

extern void socket_stub_input(void *buf_p, size_t size);
extern void socket_stub_output(void *buf_p, size_t size);

static int test_ping_host_by_ip_address(struct harness_t *harness_p)
{
    struct inet_ip_addr_t address;
    struct time_t round_trip_time;
    uint8_t request[8];
    uint8_t reply[8];

    /* Prepare the socket stub with the reply packet. */
    reply[0] = 0;
    reply[1] = 0;
    reply[2] = 255;
    reply[3] = 255;
    reply[4] = 0;
    reply[5] = 0;
    reply[6] = 0;
    reply[7] = 0;
    socket_stub_input(reply, sizeof(reply));

    address.number = 0x1;

    BTASSERT(ping_host_by_ip_address(&address, NULL, &round_trip_time) == 0);
    socket_stub_output(request, sizeof(request));
    BTASSERT(request[0] == 8);
    BTASSERT(request[1] == 0);
    BTASSERT(request[2] == 255);
    BTASSERT(request[3] == 255);
    BTASSERT(request[4] == 0);
    BTASSERT(request[5] == 0);
    BTASSERT(request[6] == 0);
    BTASSERT(request[7] == 0);

    std_printf(FSTR("round trip time: %lu s %lu ns\r\n"),
               round_trip_time.seconds,
               round_trip_time.nanoseconds);
    
    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_ping_host_by_ip_address, "test_ping_host_by_ip_address" },
        { NULL, NULL }
    };

    sys_start();
    ping_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
