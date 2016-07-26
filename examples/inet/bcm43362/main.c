/**
 * @file main.c
 * @version 3.1.0
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

static struct uart_driver_t uart;
static uint8_t rxbuf[256];
static struct network_interface_bcm43362_t bcm43362;

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

    inet_module_init();
    socket_module_init();
    network_interface_bcm43362_module_init();

    inet_aton("169.254.1.2", &ipaddr);
    inet_aton("255.255.255.0", &netmask);
    inet_aton("0.0.0.0", &gw);

    bcm43362_init(&bcm43362,
                  &sdio_device[0]);

    network_interface_bcm43362_init(&netif,
                                    &bcm43362,
                                    &ipaddr,
                                    &netmask,
                                    &gw);
    network_interface_add(&bcm43362.network_interface);
    network_interface_enable(&bcm43362.network_interface);

    thrd_suspend(NULL);

    return (0);
}
