/**
 * @file drivers/espressif_wifi/softap.h
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

#ifndef __DRIVERS_ESPRESSIF_WIFI_SOFTAP_H__
#define __DRIVERS_ESPRESSIF_WIFI_SOFTAP_H__

#include "simba.h"

/**
 * Information about a connected station.
 */
struct espressif_wifi_softap_station_info_t {
    uint8 bssid[6];
    struct inet_ip_addr_t ip_address;
};

/**
 * Initialize the WiFi SoftAP interface.
 *
 * @param[in] ssid_p SSID of the SoftAP.
 * @param[in] password_p Password of SoftAP.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_softap_init(const char *ssid_p,
                               const char *password_p);

/**
 * Set the ip address, netmask and gateway of the WiFi SoftAP.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_softap_set_ip_info(struct espressif_wifi_ip_info_t *info_p);

/**
 * Get the SoftAP ip address, netmask and gateway.
 *
 * @param[out] info_p Read ip information.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_softap_get_ip_info(struct espressif_wifi_ip_info_t *info_p);

/**
 * Get the number of stations connected to the SoftAP.
 *
 * @return Number of conencted stations.
 */
int espressif_wifi_softap_get_number_of_connected_stations(void);

/**
 * Get the information of stations connected to the SoftAP, including
 * MAC and IP addresses.
 *
 * @param[out] info_p An array to write the station information to.
 * @param[in] length Length of the info array.
 *
 * @return Number of valid station information entries or negative
 *         error code.
 */
int espressif_wifi_softap_get_station_info(struct espressif_wifi_softap_station_info_t *info_p,
                                           int length);

/**
 * Enable the SoftAP DHCP server.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_softap_dhcp_server_start(void);

/**
 * Disable the SoftAP DHCP server. The DHCP server is enabled by
 * default.
 *
 * @return zero(0) or negative error code.
 */
int espressif_wifi_softap_dhcp_server_stop(void);

/**
 * Get the SoftAP DHCP server status.
 *
 * @return DHCP server status.
 */
enum espressif_wifi_dhcp_status_t espressif_wifi_softap_dhcp_server_status(void);

#endif
