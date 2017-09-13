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

#ifndef __SYS_MOCK_H__
#define __SYS_MOCK_H__

#include "simba.h"

int mock_write_sys_module_init(int res);

int mock_write_sys_start(int res);

int mock_write_sys_stop(int error);

int mock_write_sys_panic(const char *message_p);

int mock_write_sys_reboot();

int mock_write_sys_backtrace(void **buf_p,
                             size_t size,
                             int res);

int mock_write_sys_reset_cause(enum sys_reset_cause_t res);

int mock_write_sys_uptime(struct time_t *uptime_p,
                          int res);

int mock_write_sys_uptime_isr(struct time_t *uptime_p,
                              int res);

int mock_write_sys_set_on_fatal_callback(sys_on_fatal_fn_t callback);

int mock_write_sys_set_stdin(void *chan_p);

int mock_write_sys_get_stdin(void *res);

int mock_write_sys_set_stdout(void *chan_p);

int mock_write_sys_get_stdout(void *res);

int mock_write_sys_lock();

int mock_write_sys_unlock();

int mock_write_sys_lock_isr();

int mock_write_sys_unlock_isr();

int mock_write_sys_get_info(far_string_t res);

int mock_write_sys_get_config(far_string_t res);

int mock_write_sys_interrupt_cpu_usage_get(cpu_usage_t res);

int mock_write_sys_interrupt_cpu_usage_reset();

int mock_write_sys_reset_cause_as_string(enum sys_reset_cause_t reset_cause,
                                         far_string_t res);

#endif
