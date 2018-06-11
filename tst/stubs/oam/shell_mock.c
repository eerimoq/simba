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
#include "shell_mock.h"

int mock_write_shell_module_init(int res)
{
    harness_mock_write("shell_module_init()",
                       NULL,
                       0);

    harness_mock_write("shell_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(shell_module_init)()
{
    int res;

    harness_mock_assert("shell_module_init()",
                        NULL,
                        0);

    harness_mock_read("shell_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_shell_init(void *chin_p,
                          void *chout_p,
                          void *arg_p,
                          const char *name_p,
                          const char *username_p,
                          const char *password_p,
                          int res)
{
    harness_mock_write("shell_init(chin_p)",
                       chin_p,
                       sizeof(chin_p));

    harness_mock_write("shell_init(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("shell_init(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("shell_init(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("shell_init(username_p)",
                       username_p,
                       strlen(username_p) + 1);

    harness_mock_write("shell_init(password_p)",
                       password_p,
                       strlen(password_p) + 1);

    harness_mock_write("shell_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(shell_init)(struct shell_t *self_p,
                                            void *chin_p,
                                            void *chout_p,
                                            void *arg_p,
                                            const char *name_p,
                                            const char *username_p,
                                            const char *password_p)
{
    int res;

    harness_mock_assert("shell_init(chin_p)",
                        chin_p,
                        sizeof(*chin_p));

    harness_mock_assert("shell_init(chout_p)",
                        chout_p,
                        sizeof(*chout_p));

    harness_mock_assert("shell_init(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_assert("shell_init(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("shell_init(username_p)",
                        username_p,
                        sizeof(*username_p));

    harness_mock_assert("shell_init(password_p)",
                        password_p,
                        sizeof(*password_p));

    harness_mock_read("shell_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_shell_main(void *arg_p,
                          void *res)
{
    harness_mock_write("shell_main(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("shell_main(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(shell_main)(void *arg_p)
{
    void *res;

    harness_mock_assert("shell_main(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_read("shell_main(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
