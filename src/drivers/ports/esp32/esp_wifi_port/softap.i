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

static wifi_ap_config_t config;
static int connected_stations = 0;

static int esp_wifi_softap_port_init(const char *ssid_p,
                                     const char *password_p)
{
    ASSERTN(ssid_p != NULL, EINVAL);

    esp_err_t res;
 
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
        config.authmode = WIFI_AUTH_OPEN;
    } else {
        strcpy((char *)config.password, password_p);
        config.authmode = WIFI_AUTH_WPA2_PSK;
    }

    res = esp_esp_wifi_set_config(WIFI_IF_AP, (wifi_config_t *)&config);
    
    return (res == ESP_OK ? 0 : -1);
}

static int esp_wifi_softap_port_set_ip_info(const struct inet_if_ip_info_t *info_p)
{
    return (-1);
}

static int esp_wifi_softap_port_get_ip_info(struct inet_if_ip_info_t *info_p)
{
    ASSERTN(info_p != NULL, EINVAL);

    tcpip_adapter_ip_info_t info;

    if (esp_tcpip_adapter_get_ip_info(WIFI_IF_AP, &info) != 1) {
        return (-1);
    }

    info_p->address.number = info.ip.addr;
    info_p->netmask.number = info.netmask.addr;
    info_p->gateway.number = info.gw.addr;

    return (0);
}

static int esp_wifi_softap_port_get_number_of_connected_stations()
{
    return (connected_stations);
}

static int esp_wifi_softap_port_get_station_info(struct esp_wifi_softap_station_info_t *info_p,
                                                 int length)
{
    return (-1);
}

static int esp_wifi_softap_port_dhcp_server_start()
{
    return (!esp_tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));
}

static int esp_wifi_softap_port_dhcp_server_stop()
{
    return (!esp_tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
}

static enum esp_wifi_dhcp_status_t esp_wifi_softap_port_dhcp_server_status()
{
    tcpip_adapter_dhcp_status_t status;
    
    if (esp_tcpip_adapter_dhcps_get_status(TCPIP_ADAPTER_IF_AP,
                                           &status) != ESP_OK) {
        return (-1);
    }
    
    return (status == TCPIP_ADAPTER_DHCP_STARTED
            ? esp_wifi_dhcp_status_running_t
            : esp_wifi_dhcp_status_stopped_t);
}

int esp_wifi_softap_port_station_connected()
{
    connected_stations++;
    
    return (0);
}

int esp_wifi_softap_port_station_disconnected()
{
    connected_stations--;

    return (0);
}
