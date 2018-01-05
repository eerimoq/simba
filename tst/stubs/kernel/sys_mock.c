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
#include "sys_mock.h"

int mock_write_sys_module_init(int res)
{
    harness_mock_write("sys_module_init()",
                       NULL,
                       0);

    harness_mock_write("sys_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sys_module_init)()
{
    int res;

    harness_mock_assert("sys_module_init()",
                        NULL,
                        0);

    harness_mock_read("sys_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_start(int res)
{
    harness_mock_write("sys_start()",
                       NULL,
                       0);

    harness_mock_write("sys_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sys_start)()
{
    int res;

    harness_mock_assert("sys_start()",
                        NULL,
                        0);

    harness_mock_read("sys_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_stop(int error)
{
    harness_mock_write("sys_stop(error)",
                       &error,
                       sizeof(error));

    return (0);
}

void __attribute__ ((weak)) STUB(sys_stop)(int error)
{
    harness_mock_assert("sys_stop(error)",
                        &error,
                        sizeof(error));
}

int mock_write_sys_panic(far_string_t fmt_p)
{
    harness_mock_write("sys_panic(fmt_p, ...)",
                       fmt_p,
                       std_strlen(fmt_p) + 1);

    return (0);
}

void __attribute__ ((weak)) STUB(sys_panic)(far_string_t fmt_p, ...)
{
    harness_mock_assert("sys_panic(fmt_p, ...)",
                        fmt_p,
                        std_strlen(fmt_p) + 1);
}

int mock_write_sys_reboot()
{
    harness_mock_write("sys_reboot()",
                       NULL,
                       0);

    return (0);
}

void __attribute__ ((weak)) STUB(sys_reboot)()
{
    harness_mock_assert("sys_reboot()",
                        NULL,
                        0);
}

int mock_write_sys_backtrace(void **buf_p,
                             size_t size,
                             int res)
{
    harness_mock_write("sys_backtrace(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("sys_backtrace(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("sys_backtrace(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sys_backtrace)(void **buf_p,
                                               size_t size)
{
    int res;

    harness_mock_read("sys_backtrace(): return (buf_p)",
                      buf_p,
                      size);

    harness_mock_assert("sys_backtrace(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("sys_backtrace(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_reset_cause(enum sys_reset_cause_t res)
{
    harness_mock_write("sys_reset_cause()",
                       NULL,
                       0);

    harness_mock_write("sys_reset_cause(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

enum sys_reset_cause_t __attribute__ ((weak)) STUB(sys_reset_cause)()
{
    enum sys_reset_cause_t res;

    harness_mock_assert("sys_reset_cause()",
                        NULL,
                        0);

    harness_mock_read("sys_reset_cause(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_uptime(struct time_t *uptime_p,
                          int res)
{
    harness_mock_write("sys_uptime(): return (uptime_p)",
                       uptime_p,
                       sizeof(*uptime_p));

    harness_mock_write("sys_uptime(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sys_uptime)(struct time_t *uptime_p)
{
    int res;

    harness_mock_read("sys_uptime(): return (uptime_p)",
                      uptime_p,
                      sizeof(*uptime_p));

    harness_mock_read("sys_uptime(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_uptime_isr(struct time_t *uptime_p,
                              int res)
{
    harness_mock_write("sys_uptime_isr(): return (uptime_p)",
                       uptime_p,
                       sizeof(*uptime_p));

    harness_mock_write("sys_uptime_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(sys_uptime_isr)(struct time_t *uptime_p)
{
    int res;

    harness_mock_read("sys_uptime_isr(): return (uptime_p)",
                      uptime_p,
                      sizeof(*uptime_p));

    harness_mock_read("sys_uptime_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_set_on_fatal_callback(sys_on_fatal_fn_t callback)
{
    harness_mock_write("sys_set_on_fatal_callback(callback)",
                       &callback,
                       sizeof(callback));

    return (0);
}

void __attribute__ ((weak)) STUB(sys_set_on_fatal_callback)(sys_on_fatal_fn_t callback)
{
    harness_mock_assert("sys_set_on_fatal_callback(callback)",
                        &callback,
                        sizeof(callback));
}

int mock_write_sys_set_stdin(void *chan_p)
{
    harness_mock_write("sys_set_stdin(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    return (0);
}

void __attribute__ ((weak)) STUB(sys_set_stdin)(void *chan_p)
{
    harness_mock_assert("sys_set_stdin(chan_p)",
                        chan_p,
                        sizeof(*chan_p));
}

int mock_write_sys_get_stdin(void *res)
{
    harness_mock_write("sys_get_stdin()",
                       NULL,
                       0);

    harness_mock_write("sys_get_stdin(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(sys_get_stdin)()
{
    void *res;

    harness_mock_assert("sys_get_stdin()",
                        NULL,
                        0);

    harness_mock_read("sys_get_stdin(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_set_stdout(void *chan_p)
{
    harness_mock_write("sys_set_stdout(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    return (0);
}

void __attribute__ ((weak)) STUB(sys_set_stdout)(void *chan_p)
{
    harness_mock_assert("sys_set_stdout(chan_p)",
                        chan_p,
                        sizeof(*chan_p));
}

int mock_write_sys_get_stdout(void *res)
{
    harness_mock_write("sys_get_stdout()",
                       NULL,
                       0);

    harness_mock_write("sys_get_stdout(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(sys_get_stdout)()
{
    void *res;

    harness_mock_assert("sys_get_stdout()",
                        NULL,
                        0);

    harness_mock_read("sys_get_stdout(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_lock()
{
    harness_mock_write("sys_lock()",
                       NULL,
                       0);

    return (0);
}

void __attribute__ ((weak)) STUB(sys_lock)()
{
    harness_mock_assert("sys_lock()",
                        NULL,
                        0);
}

int mock_write_sys_unlock()
{
    harness_mock_write("sys_unlock()",
                       NULL,
                       0);

    return (0);
}

void __attribute__ ((weak)) STUB(sys_unlock)()
{
    harness_mock_assert("sys_unlock()",
                        NULL,
                        0);
}

int mock_write_sys_lock_isr()
{
    harness_mock_write("sys_lock_isr()",
                       NULL,
                       0);

    return (0);
}

void __attribute__ ((weak)) STUB(sys_lock_isr)()
{
    harness_mock_assert("sys_lock_isr()",
                        NULL,
                        0);
}

int mock_write_sys_unlock_isr()
{
    harness_mock_write("sys_unlock_isr()",
                       NULL,
                       0);

    return (0);
}

void __attribute__ ((weak)) STUB(sys_unlock_isr)()
{
    harness_mock_assert("sys_unlock_isr()",
                        NULL,
                        0);
}

int mock_write_sys_get_info(far_string_t res)
{
    harness_mock_write("sys_get_info()",
                       NULL,
                       0);

    harness_mock_write("sys_get_info(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

far_string_t __attribute__ ((weak)) STUB(sys_get_info)()
{
    far_string_t res;

    harness_mock_assert("sys_get_info()",
                        NULL,
                        0);

    harness_mock_read("sys_get_info(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_get_config(far_string_t res)
{
    harness_mock_write("sys_get_config()",
                       NULL,
                       0);

    harness_mock_write("sys_get_config(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

far_string_t __attribute__ ((weak)) STUB(sys_get_config)()
{
    far_string_t res;

    harness_mock_assert("sys_get_config()",
                        NULL,
                        0);

    harness_mock_read("sys_get_config(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_interrupt_cpu_usage_get(cpu_usage_t res)
{
    harness_mock_write("sys_interrupt_cpu_usage_get()",
                       NULL,
                       0);

    harness_mock_write("sys_interrupt_cpu_usage_get(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

cpu_usage_t __attribute__ ((weak)) STUB(sys_interrupt_cpu_usage_get)()
{
    cpu_usage_t res;

    harness_mock_assert("sys_interrupt_cpu_usage_get()",
                        NULL,
                        0);

    harness_mock_read("sys_interrupt_cpu_usage_get(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_sys_interrupt_cpu_usage_reset()
{
    harness_mock_write("sys_interrupt_cpu_usage_reset()",
                       NULL,
                       0);

    return (0);
}

void __attribute__ ((weak)) STUB(sys_interrupt_cpu_usage_reset)()
{
    harness_mock_assert("sys_interrupt_cpu_usage_reset()",
                        NULL,
                        0);
}

int mock_write_sys_reset_cause_as_string(enum sys_reset_cause_t reset_cause,
                                         far_string_t res)
{
    harness_mock_write("sys_reset_cause_as_string(reset_cause)",
                       &reset_cause,
                       sizeof(reset_cause));

    harness_mock_write("sys_reset_cause_as_string(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

far_string_t __attribute__ ((weak)) STUB(sys_reset_cause_as_string)(enum sys_reset_cause_t reset_cause)
{
    far_string_t res;

    harness_mock_assert("sys_reset_cause_as_string(reset_cause)",
                        &reset_cause,
                        sizeof(reset_cause));

    harness_mock_read("sys_reset_cause_as_string(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
