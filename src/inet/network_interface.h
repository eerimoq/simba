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

#ifndef __INET_NETWORK_INTERFACE_H__
#define __INET_NETWORK_INTERFACE_H__

#include "simba.h"

struct network_interface_t;

typedef int (*network_interface_start_t)(struct network_interface_t *netif_p);
typedef int (*network_interface_stop_t)(struct network_interface_t *netif_p);
typedef int (*network_interface_is_up_t)(struct network_interface_t *netif_p);
typedef int (*network_interface_set_ip_info_t)(struct network_interface_t *netif_p,
                                               const struct inet_if_ip_info_t *info_p);
typedef int (*network_interface_get_ip_info_t)(struct network_interface_t *netif_p,
                                               struct inet_if_ip_info_t *info_p);

struct network_interface_t {
    const char *name_p;
    struct inet_if_ip_info_t info;
    network_interface_start_t start;
    network_interface_stop_t stop;
    network_interface_is_up_t is_up;
    network_interface_set_ip_info_t set_ip_info;
    network_interface_get_ip_info_t get_ip_info;
    void *netif_p;
    struct network_interface_t *next_p;
};

/**
 * Initialize the network interface module. This function must be
 * called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_module_init(void);

/**
 * Add given network interface to the global list of network
 * interfaces. Call `network_interface_start()` to enable the
 * interface.
 *
 * @param[in] netif_p Network interface to register.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_add(struct network_interface_t *netif_p);

/**
 * Start given network interface. Enables the interface in the IP
 * stack to allow packets to be sent and received. If the interface is
 * a WiFi station interface it will try initiate the connection to its
 * configured access point. Use `network_interface_is_up()` to check
 * if the interface is connected to its access point.
 *
 * @param[in] netif_p Network interface to start.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_start(struct network_interface_t *netif_p);

/**
 * Stop given network interface. Disconnects from any WiFi access
 * points and disables the interface in the IP stack. No packets can
 * be sent or received on this interface after this function is
 * called.
 *
 * @param[in] netif_p Network interface to stop.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_stop(struct network_interface_t *netif_p);

/**
 * Get the connection status of given network interface. PAckets can
 * only be sent and received when the interface in up.
 *
 * @param[in] netif_p Network interface to get the connection status
 *                    of.
 *
 * @return true(1) if the network interface is up, false(0) is it is
 *         down, and otherwise negative error code.
 */
int network_interface_is_up(struct network_interface_t *netif_p);

/**
 * Search the global list of network interfaces for an interface with
 * given name and return it.
 *
 * @param[in] name_p Name of the network interface to find.
 *
 * @return Found network interface or NULL if it was not found.
 */
struct network_interface_t *network_interface_get_by_name(const char *name_p);

/**
 * Set the IP information of given network interface.
 *
 * @param[in] netif_p Network interface to get the IP information of.
 * @param[in] info_p IP information to set.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_set_ip_info(struct network_interface_t *netif_p,
                                  const struct inet_if_ip_info_t *info_p);

/**
 * Get the IP information of given network interface.
 *
 * @param[in] netif_p Network interface to get the IP information of.
 * @param[out] info_p Read IP information.
 *
 * @return zero(0) or negative error code.
 */
int network_interface_get_ip_info(struct network_interface_t *netif_p,
                                  struct inet_if_ip_info_t *info_p);

#endif
