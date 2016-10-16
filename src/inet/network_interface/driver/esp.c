/**
 * @file network_interface/driver/esp.c
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

static int esp_station_start(void *arg_p)
{
    int mode;

    mode = esp_wifi_get_op_mode();
    mode |= esp_wifi_op_mode_station_t;
    
    return (esp_wifi_set_op_mode(mode));
}

static int esp_station_stop(void *arg_p)
{
    int mode;

    mode = esp_wifi_get_op_mode();
    mode &= ~esp_wifi_op_mode_station_t;
    
    return (esp_wifi_set_op_mode(mode));
}

static int esp_station_init(void *arg_p,
                            const char *ssid_p,
                            const char *password_p,
                            struct inet_if_ip_info_t *info_p)
{
    esp_station_start(arg_p);
    
    return (esp_wifi_station_init(ssid_p, password_p, info_p));
}

static int esp_station_is_up(void *arg_p)
{
    return (esp_wifi_station_get_connect_status() == esp_wifi_station_status_got_ip_t);
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

static int esp_softap_start(void *arg_p)
{
    int mode;

    mode = esp_wifi_get_op_mode();
    mode |= esp_wifi_op_mode_softap_t;
    
    return (esp_wifi_set_op_mode(mode));
}

static int esp_softap_stop(void *arg_p)
{
    int mode;

    mode = esp_wifi_get_op_mode();
    mode &= ~esp_wifi_op_mode_softap_t;
    
    return (esp_wifi_set_op_mode(mode));
}

static int esp_softap_init(void *arg_p,
                           const char *ssid_p,
                           const char *password_p,
                           struct inet_if_ip_info_t *info_p)
{
    esp_softap_start(arg_p);

    return (esp_wifi_softap_init(ssid_p, password_p));
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
