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

static struct network_interface_wifi_softap_espressif_t wifi;
static uint8_t buffer[5000];

static int test_init(struct harness_t *harness_p)
{
    struct inet_ip_addr_t addr;

    network_interface_module_init();
    network_interface_wifi_softap_espressif_module_init();
    network_interface_wifi_softap_espressif_init(&wifi,
                                                  (uint8_t *)STRINGIFY(SSID),
                                                  (uint8_t *)STRINGIFY(PASSWORD));
    network_interface_add(&wifi.network_interface);
    network_interface_start(&wifi.network_interface);

    network_interface_get_ip_address(&wifi.network_interface,
                                     &addr);

    BTASSERT(addr.number > 0);
    BTASSERT(network_interface_get_by_name("esp-softap") == &wifi.network_interface);
    BTASSERT(network_interface_get_by_name("none") == NULL);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { NULL, NULL }
    };

    sys_start();
    inet_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
