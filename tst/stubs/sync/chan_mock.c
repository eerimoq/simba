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
#include "chan_mock.h"

int mock_write_chan_module_init(int res)
{
    harness_mock_write("chan_module_init()",
                       NULL,
                       0);

    harness_mock_write("chan_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_module_init)()
{
    int res;

    harness_mock_assert("chan_module_init()",
                        NULL);

    harness_mock_read("chan_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_init(chan_read_fn_t read,
                         chan_write_fn_t write,
                         chan_size_fn_t size,
                         int res)
{
    harness_mock_write("chan_init(read)",
                       &read,
                       sizeof(read));

    harness_mock_write("chan_init(write)",
                       &write,
                       sizeof(write));

    harness_mock_write("chan_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("chan_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_init)(struct chan_t *self_p,
                                           chan_read_fn_t read,
                                           chan_write_fn_t write,
                                           chan_size_fn_t size)
{
    int res;

    harness_mock_assert("chan_init(read)",
                        &read);

    harness_mock_assert("chan_init(write)",
                        &write);

    harness_mock_assert("chan_init(size)",
                        &size);

    harness_mock_read("chan_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_set_write_cb(chan_write_fn_t write_cb,
                                 int res)
{
    harness_mock_write("chan_set_write_cb(write_cb)",
                       &write_cb,
                       sizeof(write_cb));

    harness_mock_write("chan_set_write_cb(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_set_write_cb)(struct chan_t *self_p,
                                                   chan_write_fn_t write_cb)
{
    int res;

    harness_mock_assert("chan_set_write_cb(write_cb)",
                        &write_cb);

    harness_mock_read("chan_set_write_cb(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_set_write_isr_cb(chan_write_fn_t write_isr_cb,
                                     int res)
{
    harness_mock_write("chan_set_write_isr_cb(write_isr_cb)",
                       &write_isr_cb,
                       sizeof(write_isr_cb));

    harness_mock_write("chan_set_write_isr_cb(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_set_write_isr_cb)(struct chan_t *self_p,
                                                       chan_write_fn_t write_isr_cb)
{
    int res;

    harness_mock_assert("chan_set_write_isr_cb(write_isr_cb)",
                        &write_isr_cb);

    harness_mock_read("chan_set_write_isr_cb(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_set_write_filter_cb(chan_write_filter_fn_t write_filter_cb,
                                        int res)
{
    harness_mock_write("chan_set_write_filter_cb(write_filter_cb)",
                       &write_filter_cb,
                       sizeof(write_filter_cb));

    harness_mock_write("chan_set_write_filter_cb(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_set_write_filter_cb)(struct chan_t *self_p,
                                                          chan_write_filter_fn_t write_filter_cb)
{
    int res;

    harness_mock_assert("chan_set_write_filter_cb(write_filter_cb)",
                        &write_filter_cb);

    harness_mock_read("chan_set_write_filter_cb(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_set_write_filter_isr_cb(chan_write_filter_fn_t write_filter_isr_cb,
                                            int res)
{
    harness_mock_write("chan_set_write_filter_isr_cb(write_filter_isr_cb)",
                       &write_filter_isr_cb,
                       sizeof(write_filter_isr_cb));

    harness_mock_write("chan_set_write_filter_isr_cb(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_set_write_filter_isr_cb)(struct chan_t *self_p,
                                                              chan_write_filter_fn_t write_filter_isr_cb)
{
    int res;

    harness_mock_assert("chan_set_write_filter_isr_cb(write_filter_isr_cb)",
                        &write_filter_isr_cb);

    harness_mock_read("chan_set_write_filter_isr_cb(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_set_control_cb(chan_control_fn_t control_cb,
                                   int res)
{
    harness_mock_write("chan_set_control_cb(control_cb)",
                       &control_cb,
                       sizeof(control_cb));

    harness_mock_write("chan_set_control_cb(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_set_control_cb)(struct chan_t *self_p,
                                                     chan_control_fn_t control_cb)
{
    int res;

    harness_mock_assert("chan_set_control_cb(control_cb)",
                        &control_cb);

    harness_mock_read("chan_set_control_cb(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_read(void *buf_p,
                         size_t size,
                         ssize_t res)
{
    harness_mock_write("chan_read(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("chan_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("chan_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(chan_read)(void *self_p,
                                               void *buf_p,
                                               size_t size)
{
    ssize_t res;

    harness_mock_read("chan_read(): return (buf_p)",
                      buf_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("chan_read(size)",
                        &size);

    harness_mock_read("chan_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_write(const void *buf_p,
                          size_t size,
                          ssize_t res)
{
    harness_mock_write("chan_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("chan_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("chan_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(chan_write)(void *self_p,
                                                const void *buf_p,
                                                size_t size)
{
    ssize_t res;

    harness_mock_assert("chan_write(buf_p)",
                        buf_p);

    harness_mock_assert("chan_write(size)",
                        &size);

    harness_mock_read("chan_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_getc(int res)
{
    harness_mock_write("chan_getc(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_getc)(void *self_p)
{
    int res;

    harness_mock_read("chan_getc(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_putc(int character,
                         int res)
{
    harness_mock_write("chan_putc(character)",
                       &character,
                       sizeof(character));

    harness_mock_write("chan_putc(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_putc)(void *self_p,
                                           int character)
{
    int res;

    harness_mock_assert("chan_putc(character)",
                        &character);

    harness_mock_read("chan_putc(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_size(size_t res)
{
    harness_mock_write("chan_size(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

size_t __attribute__ ((weak)) STUB(chan_size)(void *self_p)
{
    size_t res;

    harness_mock_read("chan_size(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_control(int operation,
                            int res)
{
    harness_mock_write("chan_control(operation)",
                       &operation,
                       sizeof(operation));

    harness_mock_write("chan_control(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_control)(void *self_p,
                                              int operation)
{
    int res;

    harness_mock_assert("chan_control(operation)",
                        &operation);

    harness_mock_read("chan_control(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_write_isr(const void *buf_p,
                              size_t size,
                              ssize_t res)
{
    harness_mock_write("chan_write_isr(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("chan_write_isr(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("chan_write_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(chan_write_isr)(void *self_p,
                                                    const void *buf_p,
                                                    size_t size)
{
    ssize_t res;

    harness_mock_assert("chan_write_isr(buf_p)",
                        buf_p);

    harness_mock_assert("chan_write_isr(size)",
                        &size);

    harness_mock_read("chan_write_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_is_polled_isr(int res)
{
    harness_mock_write("chan_is_polled_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_is_polled_isr)(struct chan_t *self_p)
{
    int res;

    harness_mock_read("chan_is_polled_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_list_init(struct chan_list_t *list_p,
                              void *workspace_p,
                              size_t size,
                              int res)
{
    harness_mock_write("chan_list_init(list_p)",
                       list_p,
                       sizeof(*list_p));

    harness_mock_write("chan_list_init(workspace_p)",
                       workspace_p,
                       size);

    harness_mock_write("chan_list_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("chan_list_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_list_init)(struct chan_list_t *list_p,
                                                void *workspace_p,
                                                size_t size)
{
    int res;

    harness_mock_assert("chan_list_init(list_p)",
                        list_p);

    harness_mock_assert("chan_list_init(workspace_p)",
                        workspace_p);

    harness_mock_assert("chan_list_init(size)",
                        &size);

    harness_mock_read("chan_list_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_list_destroy(struct chan_list_t *list_p,
                                 int res)
{
    harness_mock_write("chan_list_destroy(list_p)",
                       list_p,
                       sizeof(*list_p));

    harness_mock_write("chan_list_destroy(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_list_destroy)(struct chan_list_t *list_p)
{
    int res;

    harness_mock_assert("chan_list_destroy(list_p)",
                        list_p);

    harness_mock_read("chan_list_destroy(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_list_add(struct chan_list_t *list_p,
                             void *chan_p,
                             int res)
{
    harness_mock_write("chan_list_add(list_p)",
                       list_p,
                       sizeof(*list_p));

    harness_mock_write("chan_list_add(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("chan_list_add(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_list_add)(struct chan_list_t *list_p,
                                               void *chan_p)
{
    int res;

    harness_mock_assert("chan_list_add(list_p)",
                        list_p);

    harness_mock_assert("chan_list_add(chan_p)",
                        chan_p);

    harness_mock_read("chan_list_add(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_list_remove(struct chan_list_t *list_p,
                                void *chan_p,
                                int res)
{
    harness_mock_write("chan_list_remove(list_p)",
                       list_p,
                       sizeof(*list_p));

    harness_mock_write("chan_list_remove(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("chan_list_remove(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_list_remove)(struct chan_list_t *list_p,
                                                  void *chan_p)
{
    int res;

    harness_mock_assert("chan_list_remove(list_p)",
                        list_p);

    harness_mock_assert("chan_list_remove(chan_p)",
                        chan_p);

    harness_mock_read("chan_list_remove(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_list_poll(struct chan_list_t *list_p,
                              const struct time_t *timeout_p,
                              void *res)
{
    harness_mock_write("chan_list_poll(list_p)",
                       list_p,
                       sizeof(*list_p));

    harness_mock_write("chan_list_poll(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("chan_list_poll(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(chan_list_poll)(struct chan_list_t *list_p,
                                                  const struct time_t *timeout_p)
{
    void *res;

    harness_mock_assert("chan_list_poll(list_p)",
                        list_p);

    harness_mock_assert("chan_list_poll(timeout_p)",
                        timeout_p);

    harness_mock_read("chan_list_poll(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_poll(void *chan_p,
                         const struct time_t *timeout_p,
                         void *res)
{
    harness_mock_write("chan_poll(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("chan_poll(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("chan_poll(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(chan_poll)(void *chan_p,
                                             const struct time_t *timeout_p)
{
    void *res;

    harness_mock_assert("chan_poll(chan_p)",
                        chan_p);

    harness_mock_assert("chan_poll(timeout_p)",
                        timeout_p);

    harness_mock_read("chan_poll(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_null(void *res)
{
    harness_mock_write("chan_null()",
                       NULL,
                       0);

    harness_mock_write("chan_null(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(chan_null)()
{
    void *res;

    harness_mock_assert("chan_null()",
                        NULL);

    harness_mock_read("chan_null(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_read_null(void *buf_p,
                              size_t size,
                              ssize_t res)
{
    harness_mock_write("chan_read_null(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("chan_read_null(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("chan_read_null(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(chan_read_null)(void *self_p,
                                                    void *buf_p,
                                                    size_t size)
{
    ssize_t res;

    harness_mock_read("chan_read_null(): return (buf_p)",
                      buf_p,
                      HARNESS_MOCK_READ_ALL);

    harness_mock_assert("chan_read_null(size)",
                        &size);

    harness_mock_read("chan_read_null(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_write_null(const void *buf_p,
                               size_t size,
                               ssize_t res)
{
    harness_mock_write("chan_write_null(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("chan_write_null(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("chan_write_null(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(chan_write_null)(void *self_p,
                                                     const void *buf_p,
                                                     size_t size)
{
    ssize_t res;

    harness_mock_assert("chan_write_null(buf_p)",
                        buf_p);

    harness_mock_assert("chan_write_null(size)",
                        &size);

    harness_mock_read("chan_write_null(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_size_null(size_t res)
{
    harness_mock_write("chan_size_null(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

size_t __attribute__ ((weak)) STUB(chan_size_null)(void *self_p)
{
    size_t res;

    harness_mock_read("chan_size_null(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_chan_control_null(int operation,
                                 int res)
{
    harness_mock_write("chan_control_null(operation)",
                       &operation,
                       sizeof(operation));

    harness_mock_write("chan_control_null(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(chan_control_null)(void *self_p,
                                                   int operation)
{
    int res;

    harness_mock_assert("chan_control_null(operation)",
                        &operation);

    harness_mock_read("chan_control_null(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
