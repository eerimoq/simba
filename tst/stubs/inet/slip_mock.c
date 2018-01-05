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
#include "slip_mock.h"

int mock_write_slip_init(void *buf_p,
                         size_t size,
                         void *chout_p,
                         int res)
{
    harness_mock_write("slip_init(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("slip_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("slip_init(): return (chout_p)",
                       chout_p,
                       size);

    harness_mock_write("slip_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(slip_init)(struct slip_t *self_p,
                                           void *buf_p,
                                           size_t size,
                                           void *chout_p)
{
    int res;

    harness_mock_read("slip_init(): return (buf_p)",
                      buf_p,
                      size);

    harness_mock_assert("slip_init(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("slip_init(): return (chout_p)",
                      chout_p,
                      size);

    harness_mock_read("slip_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_slip_input(uint8_t data,
                          ssize_t res)
{
    harness_mock_write("slip_input(data)",
                       &data,
                       sizeof(data));

    harness_mock_write("slip_input(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(slip_input)(struct slip_t *self_p,
                                                uint8_t data)
{
    ssize_t res;

    harness_mock_assert("slip_input(data)",
                        &data,
                        sizeof(data));

    harness_mock_read("slip_input(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_slip_get_output_channel(void *res)
{
    harness_mock_write("slip_get_output_channel(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(slip_get_output_channel)(struct slip_t *self_p)
{
    void *res;

    harness_mock_read("slip_get_output_channel(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
