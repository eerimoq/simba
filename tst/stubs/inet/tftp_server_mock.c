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
#include "tftp_server_mock.h"

int mock_write_tftp_server_init(struct inet_addr_t *addr_p,
                                int timeout_ms,
                                const char *name_p,
                                const char *root_p,
                                void *stack_p,
                                size_t stack_size,
                                int res)
{
    harness_mock_write("tftp_server_init(addr_p)",
                       addr_p,
                       sizeof(*addr_p));

    harness_mock_write("tftp_server_init(timeout_ms)",
                       &timeout_ms,
                       sizeof(timeout_ms));

    harness_mock_write("tftp_server_init(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("tftp_server_init(root_p)",
                       root_p,
                       strlen(root_p) + 1);

    harness_mock_write("tftp_server_init(stack_p)",
                       stack_p,
                       sizeof(stack_p));

    harness_mock_write("tftp_server_init(stack_size)",
                       &stack_size,
                       sizeof(stack_size));

    harness_mock_write("tftp_server_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(tftp_server_init)(struct tftp_server_t *self_p,
                                                  struct inet_addr_t *addr_p,
                                                  int timeout_ms,
                                                  const char *name_p,
                                                  const char *root_p,
                                                  void *stack_p,
                                                  size_t stack_size)
{
    int res;

    harness_mock_assert("tftp_server_init(addr_p)",
                        addr_p,
                        sizeof(*addr_p));

    harness_mock_assert("tftp_server_init(timeout_ms)",
                        &timeout_ms,
                        sizeof(timeout_ms));

    harness_mock_assert("tftp_server_init(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("tftp_server_init(root_p)",
                        root_p,
                        sizeof(*root_p));

    harness_mock_assert("tftp_server_init(stack_p)",
                        stack_p,
                        sizeof(*stack_p));

    harness_mock_assert("tftp_server_init(stack_size)",
                        &stack_size,
                        sizeof(stack_size));

    harness_mock_read("tftp_server_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_tftp_server_start(int res)
{
    harness_mock_write("tftp_server_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(tftp_server_start)(struct tftp_server_t *self_p)
{
    int res;

    harness_mock_read("tftp_server_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
