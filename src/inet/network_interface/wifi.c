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

int network_interface_wifi_module_init(void)
{
    return (0);
}

int network_interface_wifi_init(struct network_interface_wifi_t *self_p,
                                const char *name_p,
                                struct network_interface_wifi_driver_t *driver_p,
                                void *arg_p,
                                const char *ssid_p,
                                const char *password_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(name_p != NULL, EINVAL);
    ASSERTN(driver_p != NULL, EINVAL);
    ASSERTN(ssid_p != NULL, EINVAL);

    self_p->network_interface.name_p = name_p;
    self_p->network_interface.start =
        (network_interface_start_t)network_interface_wifi_start;
    self_p->network_interface.stop =
        (network_interface_stop_t)network_interface_wifi_stop;
    self_p->network_interface.is_up =
        (network_interface_is_up_t)network_interface_wifi_is_up;
    self_p->network_interface.set_ip_info =
        (network_interface_set_ip_info_t)network_interface_wifi_set_ip_info;
    self_p->network_interface.get_ip_info =
        (network_interface_get_ip_info_t)network_interface_wifi_get_ip_info;

    self_p->driver_p = driver_p;
    self_p->arg_p = arg_p;
    self_p->ssid_p = ssid_p;
    self_p->password_p = password_p;
    self_p->info_p = NULL;

    /* Initialize the driver. */
    driver_p->init(arg_p);
    
    return (0);
}

int network_interface_wifi_start(struct network_interface_wifi_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (self_p->driver_p->start(self_p->arg_p,
                                    self_p->ssid_p,
                                    self_p->password_p,
                                    self_p->info_p));
}

int network_interface_wifi_stop(struct network_interface_wifi_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (self_p->driver_p->stop(self_p->arg_p));
}

int network_interface_wifi_is_up(struct network_interface_wifi_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (self_p->driver_p->is_up(self_p->arg_p));
}

int network_interface_wifi_set_ip_info(struct network_interface_wifi_t *self_p,
                                       const struct inet_if_ip_info_t *info_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(info_p != NULL, EINVAL);

    self_p->info_p = info_p;
    
    return (self_p->driver_p->set_ip_info(self_p->arg_p, info_p));
}

int network_interface_wifi_get_ip_info(struct network_interface_wifi_t *self_p,
                                       struct inet_if_ip_info_t *info_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(info_p != NULL, EINVAL);

    return (self_p->driver_p->get_ip_info(self_p->arg_p, info_p));
}
