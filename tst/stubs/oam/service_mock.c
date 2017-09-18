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
#include "service_mock.h"

int mock_write_service_module_init(int res)
{
    harness_mock_write("service_module_init()",
                       NULL,
                       0);

    harness_mock_write("service_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(service_module_init)()
{
    int res;

    harness_mock_assert("service_module_init()",
                        NULL);

    harness_mock_read("service_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_service_init(const char *name_p,
                            service_get_status_cb_t status_cb,
                            int res)
{
    harness_mock_write("service_init(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("service_init(status_cb)",
                       &status_cb,
                       sizeof(status_cb));

    harness_mock_write("service_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(service_init)(struct service_t *self_p,
                                              const char *name_p,
                                              service_get_status_cb_t status_cb)
{
    int res;

    harness_mock_assert("service_init(name_p)",
                        name_p);

    harness_mock_assert("service_init(status_cb)",
                        &status_cb);

    harness_mock_read("service_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_service_start(int res)
{
    harness_mock_write("service_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(service_start)(struct service_t *self_p)
{
    int res;

    harness_mock_read("service_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_service_stop(int res)
{
    harness_mock_write("service_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(service_stop)(struct service_t *self_p)
{
    int res;

    harness_mock_read("service_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_service_register(struct service_t *service_p,
                                int res)
{
    harness_mock_write("service_register(service_p)",
                       service_p,
                       sizeof(*service_p));

    harness_mock_write("service_register(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(service_register)(struct service_t *service_p)
{
    int res;

    harness_mock_assert("service_register(service_p)",
                        service_p);

    harness_mock_read("service_register(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_service_deregister(struct service_t *service_p,
                                  int res)
{
    harness_mock_write("service_deregister(service_p)",
                       service_p,
                       sizeof(*service_p));

    harness_mock_write("service_deregister(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(service_deregister)(struct service_t *service_p)
{
    int res;

    harness_mock_assert("service_deregister(service_p)",
                        service_p);

    harness_mock_read("service_deregister(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
