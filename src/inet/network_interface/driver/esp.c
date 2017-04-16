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

static int esp_station_init(void *arg_p)
{
    return (0);
}

static int esp_station_start(void *arg_p,
                             const char *ssid_p,
                             const char *password_p,
                             const struct inet_if_ip_info_t *info_p)
{
    int mode;

    mode = esp_wifi_get_op_mode();
    mode |= esp_wifi_op_mode_station_t;

    if (esp_wifi_set_op_mode(mode) != 0) {
        return (-1);
    }

    esp_wifi_station_init(ssid_p, password_p, arg_p, info_p);
    esp_wifi_station_connect();

    return (0);
}

static int esp_station_stop(void *arg_p)
{
    int mode;
    
    mode = esp_wifi_get_op_mode();
    mode &= ~esp_wifi_op_mode_station_t;
    
    return (esp_wifi_set_op_mode(mode));
}

static int esp_station_is_up(void *arg_p)
{
    return (esp_wifi_station_get_status() == esp_wifi_station_status_got_ip_t);
}

static int esp_station_set_ip_info(void *arg_p,
                                   const struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_station_set_ip_info(info_p));
}

static int esp_station_get_ip_info(void *arg_p,
                                   struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_station_get_ip_info(info_p));
}

struct network_interface_wifi_driver_t network_interface_wifi_driver_esp_station = {
    .init = esp_station_init,
    .start = esp_station_start,
    .stop = esp_station_stop,
    .is_up = esp_station_is_up,
    .set_ip_info = esp_station_set_ip_info,
    .get_ip_info = esp_station_get_ip_info
};

static int esp_softap_init(void *arg_p)
{
    return (0);
}

static int esp_softap_start(void *arg_p,
                            const char *ssid_p,
                            const char *password_p,
                            const struct inet_if_ip_info_t *info_p)
{
    int mode;

    mode = esp_wifi_get_op_mode();
    mode |= esp_wifi_op_mode_softap_t;
    
    if (esp_wifi_set_op_mode(mode) != 0) {
        return (-1);
    }

    return (esp_wifi_softap_init(ssid_p, password_p));
}

static int esp_softap_stop(void *arg_p)
{
    int mode;

    mode = esp_wifi_get_op_mode();
    mode &= ~esp_wifi_op_mode_softap_t;
    
    return (esp_wifi_set_op_mode(mode));
}

static int esp_softap_is_up(void *arg_p)
{
    return (1);
}

static int esp_softap_set_ip_info(void *arg_p,
                                  const struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_softap_set_ip_info(info_p));
}

static int esp_softap_get_ip_info(void *arg_p,
                                  struct inet_if_ip_info_t *info_p)
{
    return (esp_wifi_softap_get_ip_info(info_p));
}

struct network_interface_wifi_driver_t network_interface_wifi_driver_esp_softap = {
    .init = esp_softap_init,
    .start = esp_softap_start,
    .stop = esp_softap_stop,
    .is_up = esp_softap_is_up,
    .set_ip_info = esp_softap_set_ip_info,
    .get_ip_info = esp_softap_get_ip_info
};

#endif
