/**
 * @file main.c
 * @version 0.6.0
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

int main()
{
    struct uart_driver_t uart;
    uint8_t rxbuf[256];
    struct network_interface_slip_t slip;
    uint8_t data;
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    sys_start();
    uart_module_init();
    pin_module_init();
    socket_module_init();
    network_interface_slip_module_init();

    sys_set_stdout(&uart.chout);

    /* Initialize and start the SLIP network interface. */
    uart_init(&uart, &uart_device[0], 115200, rxbuf, sizeof(rxbuf));
    uart_start(&uart);

    ipaddr.addr = htonl(0xa9fe0102);
    netmask.addr = htonl(0xffffff00);
    gw.addr = 0x0;

    network_interface_slip_init(&slip, &ipaddr, &netmask, &gw, &uart.chout);
    network_interface_add(&slip.network_interface);
    network_interface_enable(&slip.network_interface);

    while (1) {
        uart_read(&uart, &data, 1);
        network_interface_slip_input(&slip, data);
    }

    return (0);
}
