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
#include "softap_mock.h"

int mock_write_esp_wifi_softap_init(const char *ssid_p,
                                    const char *password_p,
                                    int res)
{
    harness_mock_write("esp_wifi_softap_init(ssid_p)",
                       ssid_p,
                       strlen(ssid_p) + 1);

    harness_mock_write("esp_wifi_softap_init(password_p)",
                       password_p,
                       strlen(password_p) + 1);

    harness_mock_write("esp_wifi_softap_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(esp_wifi_softap_init)(const char *ssid_p,
                                                      const char *password_p)
{
    int res;

    harness_mock_assert("esp_wifi_softap_init(ssid_p)",
                        ssid_p,
                        sizeof(*ssid_p));

    harness_mock_assert("esp_wifi_softap_init(password_p)",
                        password_p,
                        sizeof(*password_p));

    harness_mock_read("esp_wifi_softap_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_esp_wifi_softap_set_ip_info(const struct inet_if_ip_info_t *info_p,
                                           int res)
{
    harness_mock_write("esp_wifi_softap_set_ip_info(): return (info_p)",
                       info_p,
                       sizeof(*info_p));

    harness_mock_write("esp_wifi_softap_set_ip_info(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(esp_wifi_softap_set_ip_info)(const struct inet_if_ip_info_t *info_p)
{
    int res;

    harness_mock_read("esp_wifi_softap_set_ip_info(): return (info_p)",
                      info_p,
                      sizeof(*info_p));

    harness_mock_read("esp_wifi_softap_set_ip_info(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_esp_wifi_softap_get_ip_info(struct inet_if_ip_info_t *info_p,
                                           int res)
{
    harness_mock_write("esp_wifi_softap_get_ip_info(): return (info_p)",
                       info_p,
                       sizeof(*info_p));

    harness_mock_write("esp_wifi_softap_get_ip_info(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(esp_wifi_softap_get_ip_info)(struct inet_if_ip_info_t *info_p)
{
    int res;

    harness_mock_read("esp_wifi_softap_get_ip_info(): return (info_p)",
                      info_p,
                      sizeof(*info_p));

    harness_mock_read("esp_wifi_softap_get_ip_info(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_esp_wifi_softap_get_number_of_connected_stations(int res)
{
    harness_mock_write("esp_wifi_softap_get_number_of_connected_stations()",
                       NULL,
                       0);

    harness_mock_write("esp_wifi_softap_get_number_of_connected_stations(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(esp_wifi_softap_get_number_of_connected_stations)()
{
    int res;

    harness_mock_assert("esp_wifi_softap_get_number_of_connected_stations()",
                        NULL,
                        0);

    harness_mock_read("esp_wifi_softap_get_number_of_connected_stations(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_esp_wifi_softap_get_station_info(struct esp_wifi_softap_station_info_t *info_p,
                                                int length,
                                                int res)
{
    harness_mock_write("esp_wifi_softap_get_station_info(): return (info_p)",
                       info_p,
                       sizeof(*info_p));

    harness_mock_write("esp_wifi_softap_get_station_info(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("esp_wifi_softap_get_station_info(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(esp_wifi_softap_get_station_info)(struct esp_wifi_softap_station_info_t *info_p,
                                                                  int length)
{
    int res;

    harness_mock_read("esp_wifi_softap_get_station_info(): return (info_p)",
                      info_p,
                      sizeof(*info_p));

    harness_mock_assert("esp_wifi_softap_get_station_info(length)",
                        &length,
                        sizeof(length));

    harness_mock_read("esp_wifi_softap_get_station_info(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_esp_wifi_softap_dhcp_server_start(int res)
{
    harness_mock_write("esp_wifi_softap_dhcp_server_start()",
                       NULL,
                       0);

    harness_mock_write("esp_wifi_softap_dhcp_server_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(esp_wifi_softap_dhcp_server_start)()
{
    int res;

    harness_mock_assert("esp_wifi_softap_dhcp_server_start()",
                        NULL,
                        0);

    harness_mock_read("esp_wifi_softap_dhcp_server_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_esp_wifi_softap_dhcp_server_stop(int res)
{
    harness_mock_write("esp_wifi_softap_dhcp_server_stop()",
                       NULL,
                       0);

    harness_mock_write("esp_wifi_softap_dhcp_server_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(esp_wifi_softap_dhcp_server_stop)()
{
    int res;

    harness_mock_assert("esp_wifi_softap_dhcp_server_stop()",
                        NULL,
                        0);

    harness_mock_read("esp_wifi_softap_dhcp_server_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_esp_wifi_softap_dhcp_server_status(enum esp_wifi_dhcp_status_t res)
{
    harness_mock_write("esp_wifi_softap_dhcp_server_status()",
                       NULL,
                       0);

    harness_mock_write("esp_wifi_softap_dhcp_server_status(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

enum esp_wifi_dhcp_status_t __attribute__ ((weak)) STUB(esp_wifi_softap_dhcp_server_status)()
{
    enum esp_wifi_dhcp_status_t res;

    harness_mock_assert("esp_wifi_softap_dhcp_server_status()",
                        NULL,
                        0);

    harness_mock_read("esp_wifi_softap_dhcp_server_status(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
