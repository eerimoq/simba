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
#include "ping_mock.h"

int mock_write_ping_module_init(int res)
{
    harness_mock_write("ping_module_init()",
                       NULL,
                       0);

    harness_mock_write("ping_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ping_module_init)()
{
    int res;

    harness_mock_assert("ping_module_init()",
                        NULL,
                        0);

    harness_mock_read("ping_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ping_host_by_ip_address(struct inet_ip_addr_t *address_p,
                                       struct time_t *timeout_p,
                                       struct time_t *round_trip_time_p,
                                       int res)
{
    harness_mock_write("ping_host_by_ip_address(address_p)",
                       address_p,
                       sizeof(*address_p));

    harness_mock_write("ping_host_by_ip_address(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("ping_host_by_ip_address(): return (round_trip_time_p)",
                       round_trip_time_p,
                       sizeof(*round_trip_time_p));

    harness_mock_write("ping_host_by_ip_address(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ping_host_by_ip_address)(struct inet_ip_addr_t *address_p,
                                                         struct time_t *timeout_p,
                                                         struct time_t *round_trip_time_p)
{
    int res;

    harness_mock_assert("ping_host_by_ip_address(address_p)",
                        address_p,
                        sizeof(*address_p));

    harness_mock_assert("ping_host_by_ip_address(timeout_p)",
                        timeout_p,
                        sizeof(*timeout_p));

    harness_mock_read("ping_host_by_ip_address(): return (round_trip_time_p)",
                      round_trip_time_p,
                      sizeof(*round_trip_time_p));

    harness_mock_read("ping_host_by_ip_address(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
