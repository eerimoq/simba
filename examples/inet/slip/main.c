/**
 * @file main.c
 * @version 1.0.0
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

static struct uart_driver_t ipuart;
static char iprxbuf[512];
static struct uart_driver_t uart;
static uint8_t rxbuf[256];
static struct network_interface_slip_t slip;

int main()
{
    uint8_t data;
    struct inet_ip_addr_t ipaddr;
    struct inet_ip_addr_t netmask;
    struct inet_ip_addr_t gw;

    sys_start();

    uart_module_init();
    uart_init(&uart, &uart_device[0], 38400, rxbuf, sizeof(rxbuf));
    uart_start(&uart);
    sys_set_stdout(&uart.chout);

    log_set_default_handler_output_channel(sys_get_stdout());

    std_printf(sys_get_info());

    uart_init(&ipuart, &uart_device[1], 115200, iprxbuf, sizeof(iprxbuf));
    uart_start(&ipuart);

    inet_module_init();
    socket_module_init();
    network_interface_slip_module_init();

    inet_aton("169.254.1.2", &ipaddr);
    inet_aton("255.255.255.0", &netmask);
    inet_aton("0.0.0.0", &gw);

    network_interface_slip_init(&slip,
                                &ipaddr,
                                &netmask,
                                &gw,
                                &ipuart.chout);
    network_interface_add(&slip.network_interface);
    network_interface_enable(&slip.network_interface);

    while (1) {
        uart_read(&ipuart, &data, 1);
        network_interface_slip_input(&slip, data);
    }

    return (0);
}
