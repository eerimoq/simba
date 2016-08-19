/**
 * @file main.c
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

/* The ip address of the host to connect to. */
#define REMOTE_HOST_IP 216.58.211.142

int main()
{
    struct socket_t socket;
    char http_request[] =
        "GET / HTTP/1.1\r\n"
        "Host: " STRINGIFY(REMOTE_HOST_IP) "\r\n"
        "\r\n";
    char http_response[64];
    char remote_host_ip[] = STRINGIFY(REMOTE_HOST_IP);
    struct inet_addr_t remote_host_address;
    
    /* Start the system. Brings up the configured network interfaces
       and starts the TCP/IP-stack. */
    sys_start();

    /* Open the tcp socket. */
    socket_open_tcp(&socket);

    std_printf(FSTR("Connecting to '%s'.\r\n"), remote_host_ip);

    if (inet_aton(remote_host_ip, &remote_host_address.ip) != 0) {
        std_printf(FSTR("Bad ip address ''.\r\n"), remote_host_ip);
        return (-1);
    }
    
    remote_host_address.port = 80;
    
    if (socket_connect(&socket, &remote_host_address) != 0) {
        std_printf(FSTR("Failed to connect to '%s'.\r\n"), remote_host_ip);
        return (-1);
    }
    
    /* Send the HTTP request... */
    if (socket_write(&socket,
                     http_request,
                     strlen(http_request)) != strlen(http_request)) {
        std_printf(FSTR("Failed to send the HTTP request.\r\n"));
        return (-1);
    }

    /* ...and receive the first 64 bytes of the response. */
    if (socket_read(&socket,
                    http_response,
                    sizeof(http_response)) != sizeof(http_response)) {
        std_printf(FSTR("Failed to receive the response.\r\n"));
    }
    
    std_printf(FSTR("First 64 bytes of the response:\r\n"
                    "%s"),
               http_response);

    /* Close the socket. */
    socket_close(&socket);

    return (0);
}
