/**
 * @file espressif_wifi.c
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

#include "espressif/esp_wifi.h"

int espressif_wifi_set_op_mode(enum espressif_wifi_op_mode_t mode)
{
    return (!wifi_set_opmode_current(mode));
}

enum espressif_wifi_op_mode_t espressif_wifi_get_op_mode()
{
    return (wifi_get_opmode());
}

int espressif_wifi_set_phy_mode(enum espressif_wifi_phy_mode_t mode)
{
    return (!wifi_set_phy_mode(mode));
}

enum espressif_wifi_phy_mode_t espressif_wifi_get_phy_mode()
{
    return (wifi_get_phy_mode());
}

void espressif_wifi_print(void)
{
    int i;
    int number_of_infos;
    struct espressif_wifi_softap_station_info_t info[4];
    struct espressif_wifi_ip_info_t ip_info;
    char buf[16];
    char *op_mode_p;
    char *phy_mode_p;
    char *connection_status_p;
    char *dhcp_status_p;

    /* Get the operating mode. */
    switch (espressif_wifi_get_op_mode()) {

    case espressif_wifi_op_mode_null_t:
        op_mode_p = "NULL";
        break;

    case espressif_wifi_op_mode_station_t:
        op_mode_p = "STA";
        break;

    case espressif_wifi_op_mode_softap_t:
        op_mode_p = "AP";
        break;

    case espressif_wifi_op_mode_station_softap_t:
        op_mode_p = "STA-AP";
        break;

    default:
        op_mode_p = "INVALID";
        break;
    }

    /* Get the physical mode. */
    switch (espressif_wifi_get_phy_mode()) {

    case espressif_wifi_phy_mode_11b_t:
        phy_mode_p = "b";
        break;

    case espressif_wifi_phy_mode_11g_t:
        phy_mode_p = "g";
        break;

    case espressif_wifi_phy_mode_11n_t:
        phy_mode_p = "n";
        break;

    default:
        phy_mode_p = "invalid";
        break;
    }

    /* Get the DHCP server status. */
    switch (espressif_wifi_softap_dhcp_server_status()) {

    case espressif_wifi_dhcp_status_running_t:
        dhcp_status_p = "running";
        break;

    case espressif_wifi_dhcp_status_stopped_t:
        dhcp_status_p = "stopped";
        break;

    default:
        dhcp_status_p = "invalid";
        break;
    }

    std_printf(FSTR("General information:\r\n"
                    "  Operating mode: %s\r\n"
                    "  Physical mode: 11%s\r\n"
                    "\r\n"),
               op_mode_p,
               phy_mode_p);

    espressif_wifi_softap_get_ip_info(&ip_info);
    number_of_infos = espressif_wifi_softap_get_station_info(info,
                                                             membersof(info));

    std_printf(FSTR("SoftAP:\r\n"
                    "  DHCP server status: %s\r\n"
                    "  Address: %s\r\n"),
               dhcp_status_p,
               inet_ntoa(&ip_info.address, &buf[0]));

    std_printf(FSTR("  Netmask: %s\r\n"),
               inet_ntoa(&ip_info.netmask, &buf[0]));

    std_printf(FSTR("  Gateway: %s\r\n"
                    "  Number of connections: %d\r\n"),
               inet_ntoa(&ip_info.gateway, &buf[0]),
               number_of_infos);

    for (i = 0; i < number_of_infos; i++) {
        std_printf(FSTR("    [%d]: BSSID: %02x:%02x:%02x:%02x:%02x:%02x,"
                        " IP: %s\r\n"),
                   i,
                   (int)info[i].bssid[0],
                   (int)info[i].bssid[1],
                   (int)info[i].bssid[2],
                   (int)info[i].bssid[3],
                   (int)info[i].bssid[4],
                   (int)info[i].bssid[5],
                   inet_ntoa(&info[i].ip_address, &buf[0]));
    }

    /* Get the station connection status. */
    switch (espressif_wifi_station_get_connect_status()) {

    case espressif_wifi_station_status_idle_t:
        connection_status_p = "idle";
        break;

    case espressif_wifi_station_status_connecting_t:
        connection_status_p = "connecting";
        break;

    case espressif_wifi_station_status_wrong_password_t:
        connection_status_p = "wrong-password";
        break;

    case espressif_wifi_station_status_no_ap_found_t:
        connection_status_p = "no-ap";
        break;

    case espressif_wifi_station_status_connect_fail_t:
        connection_status_p = "connect-fail";
        break;

    case espressif_wifi_station_status_got_ip_t:
        connection_status_p = "got-ip";
        break;

    default:
        connection_status_p = "invalid";
        break;
    }

    /* Get the DHCP client status. */
    switch (espressif_wifi_station_dhcp_client_status()) {

    case espressif_wifi_dhcp_status_running_t:
        dhcp_status_p = "running";
        break;

    case espressif_wifi_dhcp_status_stopped_t:
        dhcp_status_p = "stopped";
        break;

    default:
        dhcp_status_p = "invalid";
        break;
    }

    espressif_wifi_station_get_ip_info(&ip_info);

    std_printf(FSTR("\r\n"
                    "Station:\r\n"
                    "  DHCP client status: %s\r\n"
                    "  Conenction status: %s\r\n"
                    "  Address: %s\r\n"),
               dhcp_status_p,
               connection_status_p,
               inet_ntoa(&ip_info.address, &buf[0]));

    std_printf(FSTR("  Netmask: %s\r\n"),
               inet_ntoa(&ip_info.netmask, &buf[0]));

    std_printf(FSTR("  Gateway: %s\r\n"
                    "\r\n"),
               inet_ntoa(&ip_info.gateway, &buf[0]));
}
