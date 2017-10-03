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
#include "uds_mock.h"

int mock_write_upgrade_uds_init(void *chin_p,
                                void *chout_p,
                                int res)
{
    harness_mock_write("upgrade_uds_init(chin_p)",
                       chin_p,
                       sizeof(chin_p));

    harness_mock_write("upgrade_uds_init(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("upgrade_uds_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_uds_init)(struct upgrade_uds_t *self_p,
                                                  void *chin_p,
                                                  void *chout_p)
{
    int res;

    harness_mock_assert("upgrade_uds_init(chin_p)",
                        chin_p,
                        sizeof(*chin_p));

    harness_mock_assert("upgrade_uds_init(chout_p)",
                        chout_p,
                        sizeof(*chout_p));

    harness_mock_read("upgrade_uds_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_uds_handle_service(int res)
{
    harness_mock_write("upgrade_uds_handle_service(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_uds_handle_service)(struct upgrade_uds_t *self_p)
{
    int res;

    harness_mock_read("upgrade_uds_handle_service(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_uds_main()
{
    return (0);
}

void __attribute__ ((weak)) STUB(upgrade_uds_main)(struct upgrade_uds_t *self_p)
{
}
