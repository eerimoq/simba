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

int main()
{
    struct inet_if_ip_info_t info;

    sys_start();

    /* Configure and start the WiFi module as a Station.*/
    esp_wifi_set_op_mode(esp_wifi_op_mode_station_t);

    inet_aton("192.168.0.200", &info.address);
    inet_aton("255.255.255.0", &info.netmask);
    inet_aton("192.168.0.1", &info.gateway);

    if (esp_wifi_station_init("Qvist2", "maxierik", &info) != 0) {
        std_printf(FSTR("Failed to configure the Station.\r\n"));
    }

    while (1) {
        esp_wifi_print(sys_get_stdout());
        thrd_sleep(2);
    }

    return (0);
}
