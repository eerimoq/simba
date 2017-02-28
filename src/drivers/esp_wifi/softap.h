/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_ESP_WIFI_SOFTAP_H__
#define __DRIVERS_ESP_WIFI_SOFTAP_H__

#include "simba.h"

/**
 * Information about a connected station.
 */
struct esp_wifi_softap_station_info_t {
    uint8_t bssid[6];
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
int esp_wifi_softap_init(const char *ssid_p,
                         const char *password_p);

/**
 * Set the ip address, netmask and gateway of the WiFi SoftAP.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_softap_set_ip_info(const struct inet_if_ip_info_t *info_p);

/**
 * Get the SoftAP ip address, netmask and gateway.
 *
 * @param[out] info_p Read ip information.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_softap_get_ip_info(struct inet_if_ip_info_t *info_p);

/**
 * Get the number of stations connected to the SoftAP.
 *
 * @return Number of conencted stations.
 */
int esp_wifi_softap_get_number_of_connected_stations(void);

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
int esp_wifi_softap_get_station_info(struct esp_wifi_softap_station_info_t *info_p,
                                     int length);

/**
 * Enable the SoftAP DHCP server.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_softap_dhcp_server_start(void);

/**
 * Disable the SoftAP DHCP server. The DHCP server is enabled by
 * default.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_softap_dhcp_server_stop(void);

/**
 * Get the SoftAP DHCP server status.
 *
 * @return DHCP server status.
 */
enum esp_wifi_dhcp_status_t esp_wifi_softap_dhcp_server_status(void);

#endif
