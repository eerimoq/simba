/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "wifi_mock.h"

int mock_write_network_interface_wifi_module_init(int res)
{
    harness_mock_write("network_interface_wifi_module_init()",
                       NULL,
                       0);

    harness_mock_write("network_interface_wifi_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_wifi_module_init)()
{
    int res;

    harness_mock_assert("network_interface_wifi_module_init()",
                        NULL,
                        0);

    harness_mock_read("network_interface_wifi_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_network_interface_wifi_init(const char *name_p,
                                           struct network_interface_wifi_driver_t *driver_p,
                                           void *arg_p,
                                           const char *ssid_p,
                                           const char *password_p,
                                           int res)
{
    harness_mock_write("network_interface_wifi_init(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("network_interface_wifi_init(driver_p)",
                       driver_p,
                       sizeof(*driver_p));

    harness_mock_write("network_interface_wifi_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("network_interface_wifi_init(ssid_p)",
                       ssid_p,
                       strlen(ssid_p) + 1);

    harness_mock_write("network_interface_wifi_init(password_p)",
                       password_p,
                       strlen(password_p) + 1);

    harness_mock_write("network_interface_wifi_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_wifi_init)(struct network_interface_wifi_t *self_p,
                                                             const char *name_p,
                                                             struct network_interface_wifi_driver_t *driver_p,
                                                             void *arg_p,
                                                             const char *ssid_p,
                                                             const char *password_p)
{
    int res;

    harness_mock_assert("network_interface_wifi_init(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("network_interface_wifi_init(driver_p)",
                        driver_p,
                        sizeof(*driver_p));

    harness_mock_assert("network_interface_wifi_init(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_assert("network_interface_wifi_init(ssid_p)",
                        ssid_p,
                        sizeof(*ssid_p));

    harness_mock_assert("network_interface_wifi_init(password_p)",
                        password_p,
                        sizeof(*password_p));

    harness_mock_read("network_interface_wifi_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_network_interface_wifi_start(int res)
{
    harness_mock_write("network_interface_wifi_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_wifi_start)(struct network_interface_wifi_t *self_p)
{
    int res;

    harness_mock_read("network_interface_wifi_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_network_interface_wifi_stop(int res)
{
    harness_mock_write("network_interface_wifi_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_wifi_stop)(struct network_interface_wifi_t *self_p)
{
    int res;

    harness_mock_read("network_interface_wifi_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_network_interface_wifi_is_up(int res)
{
    harness_mock_write("network_interface_wifi_is_up(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_wifi_is_up)(struct network_interface_wifi_t *self_p)
{
    int res;

    harness_mock_read("network_interface_wifi_is_up(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_network_interface_wifi_set_ip_info(const struct inet_if_ip_info_t *info_p,
                                                  int res)
{
    harness_mock_write("network_interface_wifi_set_ip_info(info_p)",
                       info_p,
                       sizeof(*info_p));

    harness_mock_write("network_interface_wifi_set_ip_info(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_wifi_set_ip_info)(struct network_interface_wifi_t *self_p,
                                                                    const struct inet_if_ip_info_t *info_p)
{
    int res;

    harness_mock_assert("network_interface_wifi_set_ip_info(info_p)",
                        info_p,
                        sizeof(*info_p));

    harness_mock_read("network_interface_wifi_set_ip_info(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_network_interface_wifi_get_ip_info(struct inet_if_ip_info_t *info_p,
                                                  int res)
{
    harness_mock_write("network_interface_wifi_get_ip_info(): return (info_p)",
                       info_p,
                       sizeof(*info_p));

    harness_mock_write("network_interface_wifi_get_ip_info(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_wifi_get_ip_info)(struct network_interface_wifi_t *self_p,
                                                                    struct inet_if_ip_info_t *info_p)
{
    int res;

    harness_mock_read("network_interface_wifi_get_ip_info(): return (info_p)",
                      info_p,
                      sizeof(*info_p));

    harness_mock_read("network_interface_wifi_get_ip_info(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
