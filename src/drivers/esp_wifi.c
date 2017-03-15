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

#if CONFIG_ESP_WIFI == 1

#include "esp_wifi_port.i"

struct module_t {
    int initialized;
#if CONFIG_FS_CMD_ESP_WIFI_STATUS == 1
    struct fs_command_t cmd_status;
#endif
};

static struct module_t module;

#if CONFIG_FS_CMD_ESP_WIFI_STATUS == 1

static int cmd_status_cb(int argc,
                         const char *argv[],
                         void *out_p,
                         void *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    esp_wifi_print(out_p);

    return (0);
}

#endif

int esp_wifi_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_ESP_WIFI_STATUS == 1

    fs_command_init(&module.cmd_status,
                    CSTR("/drivers/esp_wifi/status"),
                    cmd_status_cb,
                    NULL);
    fs_command_register(&module.cmd_status);

#endif

    return (esp_wifi_port_module_init());
}

int esp_wifi_set_op_mode(enum esp_wifi_op_mode_t mode)
{
    return (esp_wifi_port_set_op_mode(mode));
}

enum esp_wifi_op_mode_t esp_wifi_get_op_mode()
{
    return (esp_wifi_port_get_op_mode());
}

int esp_wifi_set_phy_mode(enum esp_wifi_phy_mode_t mode)
{
    return (esp_wifi_port_set_phy_mode(mode));
}

enum esp_wifi_phy_mode_t esp_wifi_get_phy_mode()
{
    return (esp_wifi_port_get_phy_mode());
}

void esp_wifi_print(void *chout_p)
{
    ASSERTNRV(chout_p != NULL, EINVAL);

    int i;
    int number_of_infos;
    int number_of_connections;
    struct esp_wifi_softap_station_info_t info[4];
    struct inet_if_ip_info_t ip_info;
    char buf[16];
    char *op_mode_p;
    char *phy_mode_p;
    const char *connection_status_p;
    char *dhcp_status_p;

    /* Get the operating mode. */
    switch (esp_wifi_get_op_mode()) {

    case esp_wifi_op_mode_null_t:
        op_mode_p = "NULL";
        break;

    case esp_wifi_op_mode_station_t:
        op_mode_p = "STA";
        break;

    case esp_wifi_op_mode_softap_t:
        op_mode_p = "AP";
        break;

    case esp_wifi_op_mode_station_softap_t:
        op_mode_p = "STA-AP";
        break;

    default:
        op_mode_p = "INVALID";
        break;
    }

    /* Get the physical mode. */
    switch (esp_wifi_get_phy_mode()) {

    case esp_wifi_phy_mode_11b_t:
        phy_mode_p = "b";
        break;

    case esp_wifi_phy_mode_11g_t:
        phy_mode_p = "g";
        break;

    case esp_wifi_phy_mode_11n_t:
        phy_mode_p = "n";
        break;

    default:
        phy_mode_p = "invalid";
        break;
    }

    /* Get the DHCP server status. */
    switch (esp_wifi_softap_dhcp_server_status()) {

    case esp_wifi_dhcp_status_running_t:
        dhcp_status_p = "running";
        break;

    case esp_wifi_dhcp_status_stopped_t:
        dhcp_status_p = "stopped";
        break;

    default:
        dhcp_status_p = "invalid";
        break;
    }

    std_fprintf(chout_p,
                FSTR("General information:\r\n"
                     "  Operating mode: %s\r\n"
                     "  Physical mode: 11%s\r\n"
                     "\r\n"),
                op_mode_p,
                phy_mode_p);

    memset(&ip_info, 0, sizeof(ip_info));
    esp_wifi_softap_get_ip_info(&ip_info);
    number_of_connections = esp_wifi_softap_get_number_of_connected_stations();
    number_of_infos = esp_wifi_softap_get_station_info(info,
                                                       membersof(info));

    std_fprintf(chout_p,
                FSTR("SoftAP:\r\n"
                     "  DHCP server status: %s\r\n"
                     "  Address: %s\r\n"),
                dhcp_status_p,
                inet_ntoa(&ip_info.address, &buf[0]));

    std_fprintf(chout_p,
                FSTR("  Netmask: %s\r\n"),
                inet_ntoa(&ip_info.netmask, &buf[0]));

    std_fprintf(chout_p,
                FSTR("  Gateway: %s\r\n"
                     "  Number of connections: %d\r\n"),
                inet_ntoa(&ip_info.gateway, &buf[0]),
                number_of_connections);

    for (i = 0; i < number_of_infos; i++) {
        std_fprintf(chout_p,
                    FSTR("    [%d]: BSSID: %02x:%02x:%02x:%02x:%02x:%02x,"
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
    connection_status_p =
        esp_wifi_station_status_as_string(esp_wifi_station_get_status());

    /* Get the DHCP client status. */
    switch (esp_wifi_station_dhcp_client_status()) {

    case esp_wifi_dhcp_status_running_t:
        dhcp_status_p = "running";
        break;

    case esp_wifi_dhcp_status_stopped_t:
        dhcp_status_p = "stopped";
        break;

    default:
        dhcp_status_p = "invalid";
        break;
    }

    memset(&ip_info, 0, sizeof(ip_info));
    esp_wifi_station_get_ip_info(&ip_info);

    std_fprintf(chout_p,
                FSTR("\r\n"
                     "Station:\r\n"
                     "  DHCP client status: %s\r\n"
                     "  Conenction status: %s\r\n"
                     "  Address: %s\r\n"),
                dhcp_status_p,
                connection_status_p,
                inet_ntoa(&ip_info.address, &buf[0]));

    std_fprintf(chout_p,
                FSTR("  Netmask: %s\r\n"),
                inet_ntoa(&ip_info.netmask, &buf[0]));

    std_fprintf(chout_p,
                FSTR("  Gateway: %s\r\n"
                     "\r\n"),
                inet_ntoa(&ip_info.gateway, &buf[0]));
}

#endif
