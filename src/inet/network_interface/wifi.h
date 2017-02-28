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

#ifndef __INET_NETWORK_INTERFACE_WIFI_H__
#define __INET_NETWORK_INTERFACE_WIFI_H__

#include "simba.h"

/**
 * A WiFi network interface.
 */
struct network_interface_wifi_t {
    struct network_interface_t network_interface;
    struct network_interface_wifi_driver_t *driver_p;
    void *arg_p;
    const char *ssid_p;
    const char *password_p;
    const struct inet_if_ip_info_t *info_p;
};

/**
 * Driver virtualization callbacks. See the `driver/` subfolder for
 * available drivers.
 */
struct network_interface_wifi_driver_t {
    int (*init)(void *arg_p);
    int (*start)(void *arg_p,
                 const char *ssid_p,
                 const char *password_p,
                 const struct inet_if_ip_info_t *info_p);
    int (*stop)(void *arg_p);
    int (*is_up)(void *arg_p);
    int (*set_ip_info)(void *arg_p, const struct inet_if_ip_info_t *info_p);
    int (*get_ip_info)(void *arg_p, struct inet_if_ip_info_t *info_p);
};

/**
 * Initialize the WiFi network interface module.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_wifi_module_init(void);

/**
 * Initialize given WiFi network interface with given configuration.
 *
 * @param[in] self_p The WiFi network interface to initialize.
 * @param[in] name_p Name to assign the to interface.
 * @param[in] driver_p Driver virtualization callbacks to use.
 * @param[in] arg_p Argument passed to the driver callbacks.
 * @param[in] ssid_p Access Point SSID.
 * @param[in] password_p Access Point password.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_wifi_init(struct network_interface_wifi_t *self_p,
                                const char *name_p,
                                struct network_interface_wifi_driver_t *driver_p,
                                void *arg_p,
                                const char *ssid_p,
                                const char *password_p);

/**
 * Start given WiFi network interface.
 *
 * @param[in] self_p WiFi network interface to start.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_wifi_start(struct network_interface_wifi_t *self_p);

/**
 * Stop given WiFi network interface.
 *
 * @param[in] self_p WiFi network interface to stop.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_wifi_stop(struct network_interface_wifi_t *self_p);

/**
 * Get the connection status of given network interface.
 *
 * @param[in] self_p Network interface to get the connection status
 *                   of.
 *
 * @return true(1) if the network interface is up, false(0) is it is
 *         down, and otherwise negative error code.
 */
int network_interface_wifi_is_up(struct network_interface_wifi_t *self_p);

/**
 * Set the ip address, netmask and gateway of given network interface.
 *
 * @param[in] self_p Network interface.
 * @param[in] info_p Interface IP information to set.
 *
 * @return zero(0) if the interface has valid IP information,
 *         otherwise negative error code.
 */
int network_interface_wifi_set_ip_info(struct network_interface_wifi_t *self_p,
                                       const struct inet_if_ip_info_t *info_p);

/**
 * Get the ip address, netmask and gateway of given network interface.
 *
 * @param[in] self_p Network interface.
 * @param[out] info_p Interface IP information. Only valid if this
 *                    function returns zero(0).
 *
 * @return zero(0) if the interface has valid IP information,
 *         otherwise negative error code.
 */
int network_interface_wifi_get_ip_info(struct network_interface_wifi_t *self_p,
                                       struct inet_if_ip_info_t *info_p);

#endif
