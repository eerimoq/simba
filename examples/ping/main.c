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

/* The ip address of the host to ping. */
#define REMOTE_HOST_IP 216.58.211.142

int main()
{
    int res;
    char remote_host_ip[] = STRINGIFY(REMOTE_HOST_IP);
    struct inet_ip_addr_t remote_host_ip_address;
    struct time_t round_trip_time, timeout;
    
    sys_start();

    if (inet_aton(remote_host_ip, &remote_host_ip_address) != 0) {
        std_printf(FSTR("Bad ip address '%s'.\r\n"), remote_host_ip);
        return (-1);
    }

    /* Ping the remote host. */
    timeout.seconds = 3;
    timeout.nanoseconds = 0;
    res = ping_host_by_ip_address(&remote_host_ip_address,
                                  &timeout,
                                  &round_trip_time);
    
    if (res != 0) {
        std_printf(FSTR("Failed to ping '%s'.\r\n"), remote_host_ip);
        return (-1);
    }

    std_printf(FSTR("Successfully pinged '%s' in %lu s %lu ns.\r\n"),
               round_trip_time.seconds,
               round_trip_time.nanoseconds);
    
    return (0);
}
