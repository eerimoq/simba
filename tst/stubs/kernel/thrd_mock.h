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

#ifndef __THRD_MOCK_H__
#define __THRD_MOCK_H__

#include "simba.h"

int mock_write_thrd_module_init(int res);

int mock_write_thrd_spawn(void *(*main)(void,
                          void *arg_p,
                          int prio,
                          void *stack_p,
                          size_t stack_size,
                          struct thrd_t *res);

int mock_write_thrd_suspend(const struct time_t *timeout_p,
                            int res);

int mock_write_thrd_resume(struct thrd_t *thrd_p,
                           int err,
                           int res);

int mock_write_thrd_yield(int res);

int mock_write_thrd_join(struct thrd_t *thrd_p,
                         int res);

int mock_write_thrd_sleep(float seconds,
                          int res);

int mock_write_thrd_sleep_ms(int milliseconds,
                             int res);

int mock_write_thrd_sleep_us(long microseconds,
                             int res);

int mock_write_thrd_self(struct thrd_t *res);

int mock_write_thrd_set_name(const char *name_p,
                             int res);

int mock_write_thrd_get_name(const char *res);

int mock_write_thrd_get_by_name(const char *name_p,
                                struct thrd_t *res);

int mock_write_thrd_set_log_mask(struct thrd_t *thrd_p,
                                 int mask,
                                 int res);

int mock_write_thrd_get_log_mask(int res);

int mock_write_thrd_set_prio(struct thrd_t *thrd_p,
                             int prio,
                             int res);

int mock_write_thrd_get_prio(int res);

int mock_write_thrd_init_global_env(struct thrd_environment_variable_t *variables_p,
                                    int length,
                                    int res);

int mock_write_thrd_set_global_env(const char *name_p,
                                   const char *value_p,
                                   int res);

int mock_write_thrd_get_global_env(const char *name_p,
                                   const char *res);

int mock_write_thrd_init_env(struct thrd_environment_variable_t *variables_p,
                             int length,
                             int res);

int mock_write_thrd_set_env(const char *name_p,
                            const char *value_p,
                            int res);

int mock_write_thrd_get_env(const char *name_p,
                            const char *res);

int mock_write_thrd_suspend_isr(const struct time_t *timeout_p,
                                int res);

int mock_write_thrd_resume_isr(struct thrd_t *thrd_p,
                               int err,
                               int res);

int mock_write_thrd_yield_isr(int res);

int mock_write_thrd_stack_alloc(size_t size,
                                void *res);

int mock_write_thrd_stack_free(void *stack_p,
                               int res);

int mock_write_thrd_get_bottom_of_stack(struct thrd_t *thrd_p,
                                        const void *res);

int mock_write_thrd_get_top_of_stack(struct thrd_t *thrd_p,
                                     const void *res);

int mock_write_thrd_prio_list_init(int res);

int mock_write_thrd_prio_list_push_isr(struct thrd_prio_list_elem_t *elem_p);

int mock_write_thrd_prio_list_pop_isr(struct thrd_prio_list_elem_t *res);

int mock_write_thrd_prio_list_remove_isr(struct thrd_prio_list_elem_t *elem_p,
                                         int res);

#endif
