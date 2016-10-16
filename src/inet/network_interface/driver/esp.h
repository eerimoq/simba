/**
 * @file inet/network_interface/drivers/esp.h
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
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __INET_NETWORK_INTERFACE_DRIVER_ESP_H__
#define __INET_NETWORK_INTERFACE_DRIVER_ESP_H__

#include "simba.h"

/**
 * Esressif WiFi Station driver callbacks. To be used as driver in the
 * wifi network interface.
 */
extern struct network_interface_wifi_driver_t network_interface_wifi_driver_esp_station;

/**
 * Esressif WiFi SoftAP driver callbacks. To be used as driver in the
 * wifi network interface.
 */
extern struct network_interface_wifi_driver_t network_interface_wifi_driver_esp_softap;

#endif
