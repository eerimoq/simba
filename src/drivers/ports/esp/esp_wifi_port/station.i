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

#include "simba.h"

#include "lwip/init.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/raw.h"
#include "espressif/esp_wifi.h"
#include "espressif/esp_misc.h"
#include "espressif/esp_sta.h"

static struct station_config config = {{0}};

static int esp_wifi_station_port_init(const char *ssid_p,
                                      const char *password_p,
                                      const uint8_t *bssid_p,
                                      const struct inet_if_ip_info_t *info_p)
{
    ASSERTN(ssid_p != NULL, EINVAL);

    if ((strlen(ssid_p) == 0)
        || (strlen(ssid_p) >= sizeof(config.ssid))) {
        return (-1);
    }

    strcpy((char *)config.ssid, ssid_p);

    if (password_p != NULL) {
        strcpy((char *)config.password, password_p);
    } else {
        config.password[0] = 0;
    }

    if (bssid_p != NULL) {
        config.bssid_set = 1;
        memcpy(config.bssid, bssid_p, sizeof(config.bssid));
    } else {
        config.bssid_set = 0;
    }

    /* Static or dynamic ip. */
    if (info_p != NULL) {
        esp_wifi_station_set_ip_info(info_p);
    } else {
        esp_wifi_station_dhcp_client_start();
    }

    return (!wifi_station_set_config_current(&config));
}

static int esp_wifi_station_port_connect(void)
{
    return (!wifi_station_connect());
}

static int esp_wifi_station_port_disconnect(void)
{
    return (!wifi_station_disconnect());
}

static int esp_wifi_station_port_set_ip_info(const struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, EINVAL);

    struct ip_info info;

    info.ip.addr = info_p->address.number;
    info.netmask.addr = info_p->netmask.number;
    info.gw.addr = info_p->gateway.number;

    esp_wifi_station_dhcp_client_stop();

    if (wifi_set_ip_info(STATION_IF, &info) != 1) {
        return (-1);
    }

    return (0);
}

static int esp_wifi_station_port_get_ip_info(struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, EINVAL);

    struct ip_info info;

    if (wifi_get_ip_info(STATION_IF, &info) != 1) {
        return (-1);
    }

    info_p->address.number = info.ip.addr;
    info_p->netmask.number = info.netmask.addr;
    info_p->gateway.number = info.gw.addr;

    return (0);
}

static int esp_wifi_station_port_set_reconnect_policy(int policy)
{
    return (!wifi_station_set_reconnect_policy(policy));
}

static int esp_wifi_station_port_get_reconnect_policy(void)
{
    return (wifi_station_get_reconnect_policy());
}

static enum esp_wifi_station_status_t esp_wifi_station_port_get_status(void)
{
    return (wifi_station_get_connect_status());
}

static int esp_wifi_station_port_dhcp_client_start(void)
{
    return (!wifi_station_dhcpc_start());
}

static int esp_wifi_station_port_dhcp_client_stop(void)
{
    return (!wifi_station_dhcpc_stop());
}

enum esp_wifi_dhcp_status_t esp_wifi_station_port_dhcp_client_status(void)
{
    return (wifi_station_dhcpc_status());
}
