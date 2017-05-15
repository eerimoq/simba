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

#undef BIT
#undef O_RDONLY
#undef O_WRONLY
#undef O_RDWR
#undef O_APPEND
#undef O_CREAT
#undef O_TRUNC
#undef O_EXCL
#undef O_SYNC

#include "esp_wifi.h"

static wifi_sta_config_t config = {{0}};
static enum esp_wifi_station_status_t connection_status = esp_wifi_station_status_idle_t;

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

    return (esp_esp_wifi_set_config(WIFI_IF_STA, (wifi_config_t *)&config));
}

static int esp_wifi_station_port_connect(void)
{
    return (esp_esp_wifi_connect());
}

static int esp_wifi_station_port_disconnect(void)
{
    return (esp_wifi_station_disconnect());
}

static int esp_wifi_station_port_set_ip_info(const struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, EINVAL);

    tcpip_adapter_ip_info_t info;

    info.ip.addr = info_p->address.number;
    info.netmask.addr = info_p->netmask.number;
    info.gw.addr = info_p->gateway.number;

    esp_wifi_station_dhcp_client_stop();

    if (esp_tcpip_adapter_set_ip_info(WIFI_IF_STA, &info) != ESP_OK) {
        return (-1);
    }

    return (0);
}

static int esp_wifi_station_port_get_ip_info(struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, EINVAL);

    tcpip_adapter_ip_info_t info;

    if (esp_tcpip_adapter_get_ip_info(WIFI_IF_STA, &info) != ESP_OK) {
        return (-1);
    }

    info_p->address.number = info.ip.addr;
    info_p->netmask.number = info.netmask.addr;
    info_p->gateway.number = info.gw.addr;

    return (0);
}

static int esp_wifi_station_port_set_reconnect_policy(int policy)
{
    return (esp_esp_wifi_set_auto_connect(policy));
}

static int esp_wifi_station_port_get_reconnect_policy(void)
{
    bool auto_reconnect;

    esp_esp_wifi_get_auto_connect(&auto_reconnect);

    return (auto_reconnect);
}

int esp_wifi_station_port_set_connect_status(enum esp_wifi_station_status_t status)
{
    connection_status = status;

    return (0);
}

static enum esp_wifi_station_status_t esp_wifi_station_port_get_status(void)
{
    return (connection_status);
}

static int esp_wifi_station_port_dhcp_client_start(void)
{
    return (esp_tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA));
}

static int esp_wifi_station_port_dhcp_client_stop(void)
{
    return (esp_tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA));
}

enum esp_wifi_dhcp_status_t esp_wifi_station_port_dhcp_client_status(void)
{
    tcpip_adapter_dhcp_status_t status;

    if (esp_tcpip_adapter_dhcpc_get_status(TCPIP_ADAPTER_IF_STA,
                                           &status) != ESP_OK) {
        return (-1);
    }

    return (status == TCPIP_ADAPTER_DHCP_STARTED
            ? esp_wifi_dhcp_status_running_t
            : esp_wifi_dhcp_status_stopped_t);
}
