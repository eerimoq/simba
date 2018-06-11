/*
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
#include "slip_mock.h"

int mock_write_network_interface_slip_module_init(int res)
{
    harness_mock_write("network_interface_slip_module_init()",
                       NULL,
                       0);

    harness_mock_write("network_interface_slip_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_slip_module_init)()
{
    int res;

    harness_mock_assert("network_interface_slip_module_init()",
                        NULL,
                        0);

    harness_mock_read("network_interface_slip_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_network_interface_slip_init(struct inet_ip_addr_t *ipaddr_p,
                                           struct inet_ip_addr_t *netmask_p,
                                           struct inet_ip_addr_t *gateway_p,
                                           void *chout_p,
                                           int res)
{
    harness_mock_write("network_interface_slip_init(ipaddr_p)",
                       ipaddr_p,
                       sizeof(*ipaddr_p));

    harness_mock_write("network_interface_slip_init(netmask_p)",
                       netmask_p,
                       sizeof(*netmask_p));

    harness_mock_write("network_interface_slip_init(gateway_p)",
                       gateway_p,
                       sizeof(*gateway_p));

    harness_mock_write("network_interface_slip_init(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("network_interface_slip_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_slip_init)(struct network_interface_slip_t *self_p,
                                                             struct inet_ip_addr_t *ipaddr_p,
                                                             struct inet_ip_addr_t *netmask_p,
                                                             struct inet_ip_addr_t *gateway_p,
                                                             void *chout_p)
{
    int res;

    harness_mock_assert("network_interface_slip_init(ipaddr_p)",
                        ipaddr_p,
                        sizeof(*ipaddr_p));

    harness_mock_assert("network_interface_slip_init(netmask_p)",
                        netmask_p,
                        sizeof(*netmask_p));

    harness_mock_assert("network_interface_slip_init(gateway_p)",
                        gateway_p,
                        sizeof(*gateway_p));

    harness_mock_assert("network_interface_slip_init(chout_p)",
                        chout_p,
                        sizeof(*chout_p));

    harness_mock_read("network_interface_slip_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_network_interface_slip_input(uint8_t data,
                                            int res)
{
    harness_mock_write("network_interface_slip_input(data)",
                       &data,
                       sizeof(data));

    harness_mock_write("network_interface_slip_input(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(network_interface_slip_input)(struct network_interface_slip_t *self_p,
                                                              uint8_t data)
{
    int res;

    harness_mock_assert("network_interface_slip_input(data)",
                        &data,
                        sizeof(data));

    harness_mock_read("network_interface_slip_input(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
