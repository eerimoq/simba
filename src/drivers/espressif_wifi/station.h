/**
 * @file drivers/espressif_wifi/station.h
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

#ifndef __DRIVERS_ESPRESSIF_WIFI_STATION_H__
#define __DRIVERS_ESPRESSIF_WIFI_STATION_H__

#include "simba.h"

/**
 * WiFi station connection status.
 */
enum espressif_wifi_station_status_t {
    espressif_wifi_station_status_idle_t = 0,
    espressif_wifi_station_status_connecting_t,
    espressif_wifi_station_status_wrong_password_t,
    espressif_wifi_station_status_no_ap_found_t,
    espressif_wifi_station_status_connect_fail_t,
    espressif_wifi_station_status_got_ip_t
};

/**
 * Initialize the WiFi station.
 *
 * @param[in] ssid_p WiFi SSID to connect to.
 * @param[in] password_p WiFi password.
 * @param[in] info_p Static ip configration or NULL to use DHCP.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_station_init(const char *ssid_p,
                                const char *password_p,
                                struct espressif_wifi_ip_info_t *info_p);

/**
 * Connect the WiFi station to the Access Point (AP).
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_station_connect(void);

/**
 * Disconnect the WiFi station from the AP.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_station_disconnect(void);

/**
 * Set the ip address, netmask and gateway of the WiFi station.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_station_set_ip_info(struct espressif_wifi_ip_info_t *info_p);

/**
 * Get the station ip address, netmask and gateway.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_station_get_ip_info(struct espressif_wifi_ip_info_t *info_p);

/**
 * Set whether the station will reconnect to the AP after
 * disconnection. It will do so by default.
 *
 * @param policy If it's true, it will enable reconnection; if it's
 *               false, it will disable reconnection.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_station_set_reconnect_policy(int policy);

/**
 * Check whether the station will reconnect to the AP after
 * disconnection.
 *
 * @return true(1) or false(0).
 */
int espressif_wifi_station_get_reconnect_policy(void);

/**
 * Get the connection status of the WiFi station.
 *
 * @return The connection status.
 */
enum espressif_wifi_station_status_t espressif_wifi_station_get_connect_status(void);

/**
 * Enable the station DHCP client.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_station_dhcp_client_start(void);

/**
 * Disable the station DHCP client.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_station_dhcp_client_stop(void);

/**
 * Get the station DHCP client status.
 *
 * @return Station DHCP client status.
 */
enum espressif_wifi_dhcp_status_t espressif_wifi_station_dhcp_client_status(void);

#endif
