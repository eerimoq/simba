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

static struct uart_driver_t ipuart;
static char iprxbuf[512];
static struct network_interface_slip_t slip;

int main()
{
    uint8_t data;
    struct inet_ip_addr_t ipaddr;
    struct inet_ip_addr_t netmask;
    struct inet_ip_addr_t gateway;

    sys_start();

    std_printf(sys_get_info());

    uart_init(&ipuart, &uart_device[1], 115200, iprxbuf, sizeof(iprxbuf));
    uart_start(&ipuart);

    inet_module_init();
    socket_module_init();
    network_interface_slip_module_init();

    inet_aton("169.254.1.2", &ipaddr);
    inet_aton("255.255.255.0", &netmask);
    inet_aton("0.0.0.0", &gateway);

    network_interface_slip_init(&slip,
                                &ipaddr,
                                &netmask,
                                &gateway,
                                &ipuart.base);
    network_interface_add(&slip.network_interface);
    network_interface_start(&slip.network_interface);

    while (1) {
        uart_read(&ipuart, &data, 1);
        network_interface_slip_input(&slip, data);
    }

    return (0);
}
