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

#ifndef __DRIVERS_ESP_WIFI_STATION_H__
#define __DRIVERS_ESP_WIFI_STATION_H__

#include "simba.h"

/**
 * WiFi station connection status.
 */
enum esp_wifi_station_status_t {
    esp_wifi_station_status_idle_t = 0,
    esp_wifi_station_status_connecting_t,
    esp_wifi_station_status_auth_failure_t,
    esp_wifi_station_status_no_ap_found_t,
    esp_wifi_station_status_connect_fail_t,
    esp_wifi_station_status_got_ip_t,
    esp_wifi_station_status_connected_t
};

/**
 * Initialize the WiFi station.
 *
 * @param[in] ssid_p WiFi SSID to connect to.
 * @param[in] password_p WiFi password.
 * @param[in] bssid_p WiFi station MAC (BSSID) or NULL to ignore.
 * @param[in] info_p Static ip configration or NULL to use DHCP.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_station_init(const char *ssid_p,
                          const char *password_p,
                          const uint8_t *bssid_p,
                          const struct inet_if_ip_info_t *info_p);

/**
 * Connect the WiFi station to the Access Point (AP).
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_station_connect(void);

/**
 * Disconnect the WiFi station from the AP.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_station_disconnect(void);

/**
 * Set the ip address, netmask and gateway of the WiFi station.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_station_set_ip_info(const struct inet_if_ip_info_t *info_p);

/**
 * Get the station ip address, netmask and gateway.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_station_get_ip_info(struct inet_if_ip_info_t *info_p);

/**
 * Set whether the station will reconnect to the AP after
 * disconnection. It will do so by default.
 *
 * @param policy If it's true, it will enable reconnection; if it's
 *               false, it will disable reconnection.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_station_set_reconnect_policy(int policy);

/**
 * Check whether the station will reconnect to the AP after
 * disconnection.
 *
 * @return true(1) or false(0).
 */
int esp_wifi_station_get_reconnect_policy(void);

/**
 * Get the connection status of the WiFi station.
 *
 * @return The connection status.
 */
enum esp_wifi_station_status_t esp_wifi_station_get_status(void);

/**
 * Enable the station DHCP client.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_station_dhcp_client_start(void);

/**
 * Disable the station DHCP client.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_station_dhcp_client_stop(void);

/**
 * Get the station DHCP client status.
 *
 * @return Station DHCP client status.
 */
enum esp_wifi_dhcp_status_t esp_wifi_station_dhcp_client_status(void);

/**
 * Convert given status code to a string.
 *
 * @return Status code as a string.
 */
const char *esp_wifi_station_status_as_string(enum esp_wifi_station_status_t status);

#endif
