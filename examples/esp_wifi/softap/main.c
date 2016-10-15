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
    sys_start();

    /* Configure and start the WiFi module as a SoftAP. */
    esp_wifi_set_op_mode(esp_wifi_op_mode_softap_t);

    if (esp_wifi_softap_init("Simba", NULL) != 0) {
        std_printf(FSTR("Failed to configure the Soft AP.\r\n"));
    }

    while (1) {
        esp_wifi_print();
        thrd_sleep(2);
    }

    return (0);
}
