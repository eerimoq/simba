/**
 * @file bcm43362.c
 * @version 4.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

#include "wwd_management.h"
#include "network/wwd_network_interface.h"
#include "wwd_wifi.h"

int bcm43362_module_init(void)
{
    return (0);
}

int bcm43362_init(struct bcm43362_driver_t *self_p,
                  struct sdio_device_t *sdio_dev_p)
{
    return (0);
}

int bcm43362_start(struct bcm43362_driver_t *self_p)
{
    wwd_result_t wwd_res;

    wwd_res = wwd_management_init(WICED_COUNTRY_SWEDEN, NULL);

    return (wwd_res == WWD_SUCCESS ? 0 : -1);
}

int bcm43362_stop(struct bcm43362_driver_t *self_p)
{
    wwd_result_t wwd_res;

    wwd_res = wwd_management_wifi_off();

    return (wwd_res == WWD_SUCCESS ? 0 : -1);
}

int bcm43362_connect(struct bcm43362_driver_t *self_p,
                     const char *ssid_p,
                     const char *password_p)
{
    wwd_result_t wwd_res;
    wiced_ssid_t ssid;

    ssid.length = strlen(ssid_p);
    strcpy((char *)ssid.value, ssid_p);

    wwd_res = wwd_wifi_join(&ssid,
                            WICED_SECURITY_WEP_PSK,
                            (uint8_t *)password_p,
                            strlen(password_p),
                            NULL);

    return (wwd_res == WWD_SUCCESS ? 0 : -1);
}

int bcm43362_disconnect(struct bcm43362_driver_t *self_p)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

ssize_t bcm43362_read(struct bcm43362_driver_t *self_p,
                      struct pbuf *pbuf_p,
                      size_t size)
{
    ASSERTN(0, ENOSYS);

    return (-1);
}

ssize_t bcm43362_write(struct bcm43362_driver_t *self_p,
                       struct pbuf *pbuf_p,
                       size_t size)
{
    wwd_network_send_ethernet_data(pbuf_p, WWD_STA_INTERFACE);

    return (size);
}
