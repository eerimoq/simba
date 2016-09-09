/**
 * @file inet/network_interface/wifi_station_espressif.h
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

#ifndef __INET_NETWORK_INTERFACE_WIFI_ESPRESSIF_H__
#define __INET_NETWORK_INTERFACE_WIFI_ESPRESSIF_H__

#include "simba.h"

#include "esp_wifi.h"
#include "esp_sta.h"

struct network_interface_wifi_station_espressif_t {
    struct network_interface_t network_interface;
    struct station_config station_config;
    struct ip_info ip_config;
};

/**
 * Initialize the WiFi espressif network interface module.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_wifi_station_espressif_module_init(void);

/**
 * Initialize given WiFi network interface with given configuration.
 *
 * @param[in] ssid_p Access Point SSID.
 * @param[in] password_p Access Point password.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_wifi_station_espressif_init(struct network_interface_wifi_station_espressif_t *self_p,
                                                  const uint8_t *ssid_p,
                                                  const uint8_t *password_p);

/**
 * Start given WiFi network interface.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_wifi_station_espressif_start(struct network_interface_wifi_station_espressif_t *self_p);

/**
 * Stop given WiFi network interface.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_wifi_station_espressif_stop(struct network_interface_wifi_station_espressif_t *self_p);

/**
 * Get the connection status of given network interface.
 *
 * @param[in] self_p Network interface to get the connection status
 *                   of.
 *
 * @return true(1) if the network interface is up, false(0) is it is
 *         down, and otherwise negative error code.
 */
int network_interface_wifi_station_espressif_is_up(struct network_interface_wifi_station_espressif_t *self_p);

/**
 * Get the ip address of given network interface.
 *
 * @param[in] self_p Network interface to get the connection status
 *                   of.
 * @param[out] addr_p IP address of the interface. Only valid if this
 *                    function returns zero(0).
 *
 * @return zero(0) if the interface has a valid IP address, otherwise
 *         negative error code.
 */
int network_interface_wifi_station_espressif_get_ip_address(struct network_interface_wifi_station_espressif_t *self_p,
                                                            struct inet_ip_addr_t *addr_p);

#endif
