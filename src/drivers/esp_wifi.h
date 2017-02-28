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

#ifndef __DRIVERS_ESP_WIFI_H__
#define __DRIVERS_ESP_WIFI_H__

#include "simba.h"

/**
 * WiFi operational mode.
 */
enum esp_wifi_op_mode_t {
    esp_wifi_op_mode_null_t = 0,
    esp_wifi_op_mode_station_t,
    esp_wifi_op_mode_softap_t,
    esp_wifi_op_mode_station_softap_t,
    esp_wifi_op_mode_max_t
};

/**
 * Physical WiFi mode.
 */
enum esp_wifi_phy_mode_t {
    esp_wifi_phy_mode_11b_t = 1,
    esp_wifi_phy_mode_11g_t,
    esp_wifi_phy_mode_11n_t
};

/**
 * DHCP status.
 */
enum esp_wifi_dhcp_status_t {
    esp_wifi_dhcp_status_stopped_t = 0,
    esp_wifi_dhcp_status_running_t
};

/**
 * Initialize the Espressif WiFi module. This function must be called
 * before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int esp_wifi_module_init(void);

/**
  * Set the WiFi operating mode to None, Station, SoftAP or Station +
  * SoftAP. The default mode is SoftAP.
  *
  * @param[in] mode Operating mode to set.
  *
  * @return zero(0) or negative error code.
  */
int esp_wifi_set_op_mode(enum esp_wifi_op_mode_t mode);

/**
  * Get the current WiFi operating mode. The operating mode can be
  * None, Station, SoftAP, or Station + SoftAP.
  *
  * @return Current operating mode.
  */
enum esp_wifi_op_mode_t esp_wifi_get_op_mode(void);

/**
  * Set the WiFi physical mode (802.11b/g/n).
  *
  * The SoftAP only supports b/g.
  *
  * @param[in] mode Physical mode.
  *
  * @return zero(0) or negative error code.
  */
int esp_wifi_set_phy_mode(enum esp_wifi_phy_mode_t mode);

/**
  * Get the physical mode (802.11b/g/n).
  *
  * @return WiFi physical mode.
  */
enum esp_wifi_phy_mode_t esp_wifi_get_phy_mode(void);

/**
  * Print information about the WiFi.
  */
void esp_wifi_print(void *chout_p);

#endif
