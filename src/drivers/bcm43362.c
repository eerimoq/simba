/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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
