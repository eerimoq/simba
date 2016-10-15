/**
 * @file drivers/espressif_wifi.h
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

#ifndef __DRIVERS_ESPRESSIF_WIFI_H__
#define __DRIVERS_ESPRESSIF_WIFI_H__

#include "simba.h"

/**
 * WiFi operational mode.
 */
enum espressif_wifi_op_mode_t {
    espressif_wifi_op_mode_null_t = 0,
    espressif_wifi_op_mode_station_t,
    espressif_wifi_op_mode_softap_t,
    espressif_wifi_op_mode_station_softap_t,
    espressif_wifi_op_mode_max_t
};

/**
 * Physical WiFi mode.
 */
enum espressif_wifi_phy_mode_t {
    espressif_wifi_phy_mode_11b_t = 1,
    espressif_wifi_phy_mode_11g_t,
    espressif_wifi_phy_mode_11n_t
};

/**
 * Interface IP information.
 */
struct espressif_wifi_ip_info_t {
    struct inet_ip_addr_t address;
    struct inet_ip_addr_t netmask;
    struct inet_ip_addr_t gateway;
};

/**
 * DHCP status.
 */
enum espressif_wifi_dhcp_status_t {
    espressif_wifi_dhcp_status_stopped_t = 0,
    espressif_wifi_dhcp_status_running_t
};

/**
  * Set the WiFi operating mode to Station, SoftAP or Station +
  * SoftAP. The default mode is SoftAP.
  *
  * @param[in] mode Operating mode to set.
  *
  * @return zero(0) or negative error code.
  */
int espressif_wifi_set_op_mode(enum espressif_wifi_op_mode_t mode);

/**
  * Get the current WiFi operating mode.
  *
  * @return Operating mode.
  */
enum espressif_wifi_op_mode_t espressif_wifi_get_op_mode(void);

/**
  * Set the WiFi physical mode (802.11b/g/n).
  *
  * The SoftAP only supports b/g.
  *
  * @param[in] mode Physical mode.
  *
  * @return zero(0) or negative error code.
  */
int espressif_wifi_set_phy_mode(enum espressif_wifi_phy_mode_t mode);

/**
  * Get the physical mode (802.11b/g/n).
  *
  * @return WiFi physical mode.
  */
enum espressif_wifi_phy_mode_t espressif_wifi_get_phy_mode(void);

/**
  * Print information about the WiFi.
  */
void espressif_wifi_print(void);

#endif
