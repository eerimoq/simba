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
#include "espressif/esp_softap.h"

static struct softap_config config;

static int esp_wifi_softap_port_init(const char *ssid_p,
                         const char *password_p)
{
    ASSERTN(ssid_p != NULL, EINVAL);

    if ((strlen(ssid_p) == 0)
        || (strlen(ssid_p) >= sizeof(config.ssid))) {
        return (-1);
    }

    strcpy((char *)config.ssid, ssid_p);
    config.ssid_len = strlen(ssid_p);
    config.channel = 1;
    config.ssid_hidden = 0;
    config.max_connection = 4;
    config.beacon_interval = 100;

    if (password_p == NULL) {
        config.authmode = AUTH_OPEN;
    } else {
        strcpy((char *)config.password, password_p);
        config.authmode = AUTH_WPA2_PSK;
    }

    return (!wifi_softap_set_config_current(&config));
}

static int esp_wifi_softap_port_set_ip_info(const struct inet_if_ip_info_t *info_p)
{
    return (-1);
}

static int esp_wifi_softap_port_get_ip_info(struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, EINVAL);

    struct ip_info info;

    if (wifi_get_ip_info(SOFTAP_IF, &info) != 1) {
        return (-1);
    }

    info_p->address.number = info.ip.addr;
    info_p->netmask.number = info.netmask.addr;
    info_p->gateway.number = info.gw.addr;

    return (0);
}

static int esp_wifi_softap_port_get_number_of_connected_stations()
{
    return (wifi_softap_get_station_num());
}

static int esp_wifi_softap_port_get_station_info(struct esp_wifi_softap_station_info_t *info_p,
                                     int length)
{
    int i;
    struct station_info *elem_p;

    elem_p = wifi_softap_get_station_info();
    i = 0;

    while (elem_p != NULL) {
        memcpy(&info_p[i].bssid[0], &elem_p->bssid[0], sizeof(info_p->bssid));
        info_p[i].ip_address.number = elem_p->ip.addr;
        i++;

        if (i == length) {
            break;
        }

        elem_p = STAILQ_NEXT(elem_p, next);
    }

    wifi_softap_free_station_info();

    return (i);
}

static int esp_wifi_softap_port_dhcp_server_start()
{
    return (!wifi_softap_dhcps_start());
}

static int esp_wifi_softap_port_dhcp_server_stop()
{
    return (!wifi_softap_dhcps_stop());
}

static enum esp_wifi_dhcp_status_t esp_wifi_softap_port_dhcp_server_status()
{
    return (wifi_softap_dhcps_status());
}
