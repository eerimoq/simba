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
#include "tftp_mock.h"

int mock_write_upgrade_tftp_init(int port,
                                 int timeout_ms,
                                 int res)
{
    harness_mock_write("upgrade_tftp_init(port)",
                       &port,
                       sizeof(port));

    harness_mock_write("upgrade_tftp_init(timeout_ms)",
                       &timeout_ms,
                       sizeof(timeout_ms));

    harness_mock_write("upgrade_tftp_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_tftp_init)(int port,
                                                   int timeout_ms)
{
    int res;

    harness_mock_assert("upgrade_tftp_init(port)",
                        &port);

    harness_mock_assert("upgrade_tftp_init(timeout_ms)",
                        &timeout_ms);

    harness_mock_read("upgrade_tftp_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_tftp_start(int res)
{
    harness_mock_write("upgrade_tftp_start()",
                       NULL,
                       0);

    harness_mock_write("upgrade_tftp_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_tftp_start)()
{
    int res;

    harness_mock_assert("upgrade_tftp_start()",
                        NULL);

    harness_mock_read("upgrade_tftp_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
