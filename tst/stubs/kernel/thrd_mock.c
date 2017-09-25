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
#include "thrd_mock.h"

int mock_write_thrd_module_init(int res)
{
    harness_mock_write("thrd_module_init()",
                       NULL,
                       0);

    harness_mock_write("thrd_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_module_init)()
{
    int res;

    harness_mock_assert("thrd_module_init()",
                        NULL,
                        0);

    harness_mock_read("thrd_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_spawn(void *(*main)(void,
                          void *arg_p,
                          int prio,
                          void *stack_p,
                          size_t stack_size,
                          struct thrd_t *res)
{
    harness_mock_write("thrd_spawn(): return (void)",
                       void,
                       sizeof(void));

    harness_mock_write("thrd_spawn(arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("thrd_spawn(prio)",
                       &prio,
                       sizeof(prio));

    harness_mock_write("thrd_spawn(stack_p)",
                       stack_p,
                       sizeof(stack_p));

    harness_mock_write("thrd_spawn(stack_size)",
                       &stack_size,
                       sizeof(stack_size));

    harness_mock_write("thrd_spawn(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct thrd_t *__attribute__ ((weak)) STUB(thrd_spawn)(void *(*main)(void,
                                                       void *arg_p,
                                                       int prio,
                                                       void *stack_p,
                                                       size_t stack_size)
{
    struct thrd_t *res;

    harness_mock_read("thrd_spawn(): return (void)",
                      void,
                      sizeof(*void));

    harness_mock_assert("thrd_spawn(arg_p)",
                        arg_p,
                        sizeof(*arg_p));

    harness_mock_assert("thrd_spawn(prio)",
                        &prio,
                        sizeof(prio));

    harness_mock_assert("thrd_spawn(stack_p)",
                        stack_p,
                        sizeof(*stack_p));

    harness_mock_assert("thrd_spawn(stack_size)",
                        &stack_size,
                        sizeof(stack_size));

    harness_mock_read("thrd_spawn(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_suspend(const struct time_t *timeout_p,
                            int res)
{
    harness_mock_write("thrd_suspend(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("thrd_suspend(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_suspend)(const struct time_t *timeout_p)
{
    int res;

    harness_mock_assert("thrd_suspend(timeout_p)",
                        timeout_p,
                        sizeof(*timeout_p));

    harness_mock_read("thrd_suspend(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_resume(struct thrd_t *thrd_p,
                           int err,
                           int res)
{
    harness_mock_write("thrd_resume(thrd_p)",
                       thrd_p,
                       sizeof(*thrd_p));

    harness_mock_write("thrd_resume(err)",
                       &err,
                       sizeof(err));

    harness_mock_write("thrd_resume(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_resume)(struct thrd_t *thrd_p,
                                             int err)
{
    int res;

    harness_mock_assert("thrd_resume(thrd_p)",
                        thrd_p,
                        sizeof(*thrd_p));

    harness_mock_assert("thrd_resume(err)",
                        &err,
                        sizeof(err));

    harness_mock_read("thrd_resume(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_yield(int res)
{
    harness_mock_write("thrd_yield()",
                       NULL,
                       0);

    harness_mock_write("thrd_yield(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_yield)()
{
    int res;

    harness_mock_assert("thrd_yield()",
                        NULL,
                        0);

    harness_mock_read("thrd_yield(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_join(struct thrd_t *thrd_p,
                         int res)
{
    harness_mock_write("thrd_join(thrd_p)",
                       thrd_p,
                       sizeof(*thrd_p));

    harness_mock_write("thrd_join(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_join)(struct thrd_t *thrd_p)
{
    int res;

    harness_mock_assert("thrd_join(thrd_p)",
                        thrd_p,
                        sizeof(*thrd_p));

    harness_mock_read("thrd_join(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_sleep(float seconds,
                          int res)
{
    harness_mock_write("thrd_sleep(seconds)",
                       &seconds,
                       sizeof(seconds));

    harness_mock_write("thrd_sleep(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_sleep)(float seconds)
{
    int res;

    harness_mock_assert("thrd_sleep(seconds)",
                        &seconds,
                        sizeof(seconds));

    harness_mock_read("thrd_sleep(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_sleep_ms(int milliseconds,
                             int res)
{
    harness_mock_write("thrd_sleep_ms(milliseconds)",
                       &milliseconds,
                       sizeof(milliseconds));

    harness_mock_write("thrd_sleep_ms(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_sleep_ms)(int milliseconds)
{
    int res;

    harness_mock_assert("thrd_sleep_ms(milliseconds)",
                        &milliseconds,
                        sizeof(milliseconds));

    harness_mock_read("thrd_sleep_ms(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_sleep_us(long microseconds,
                             int res)
{
    harness_mock_write("thrd_sleep_us(microseconds)",
                       &microseconds,
                       sizeof(microseconds));

    harness_mock_write("thrd_sleep_us(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_sleep_us)(long microseconds)
{
    int res;

    harness_mock_assert("thrd_sleep_us(microseconds)",
                        &microseconds,
                        sizeof(microseconds));

    harness_mock_read("thrd_sleep_us(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_self(struct thrd_t *res)
{
    harness_mock_write("thrd_self()",
                       NULL,
                       0);

    harness_mock_write("thrd_self(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct thrd_t *__attribute__ ((weak)) STUB(thrd_self)()
{
    struct thrd_t *res;

    harness_mock_assert("thrd_self()",
                        NULL,
                        0);

    harness_mock_read("thrd_self(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_set_name(const char *name_p,
                             int res)
{
    harness_mock_write("thrd_set_name(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("thrd_set_name(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_set_name)(const char *name_p)
{
    int res;

    harness_mock_assert("thrd_set_name(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_read("thrd_set_name(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_get_name(const char *res)
{
    harness_mock_write("thrd_get_name()",
                       NULL,
                       0);

    harness_mock_write("thrd_get_name(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(thrd_get_name)()
{
    const char *res;

    harness_mock_assert("thrd_get_name()",
                        NULL,
                        0);

    harness_mock_read("thrd_get_name(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_get_by_name(const char *name_p,
                                struct thrd_t *res)
{
    harness_mock_write("thrd_get_by_name(): return (name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("thrd_get_by_name(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct thrd_t *__attribute__ ((weak)) STUB(thrd_get_by_name)(const char *name_p)
{
    struct thrd_t *res;

    harness_mock_read("thrd_get_by_name(): return (name_p)",
                      name_p,
                      sizeof(*name_p));

    harness_mock_read("thrd_get_by_name(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_set_log_mask(struct thrd_t *thrd_p,
                                 int mask,
                                 int res)
{
    harness_mock_write("thrd_set_log_mask(thrd_p)",
                       thrd_p,
                       sizeof(*thrd_p));

    harness_mock_write("thrd_set_log_mask(mask)",
                       &mask,
                       sizeof(mask));

    harness_mock_write("thrd_set_log_mask(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_set_log_mask)(struct thrd_t *thrd_p,
                                                   int mask)
{
    int res;

    harness_mock_assert("thrd_set_log_mask(thrd_p)",
                        thrd_p,
                        sizeof(*thrd_p));

    harness_mock_assert("thrd_set_log_mask(mask)",
                        &mask,
                        sizeof(mask));

    harness_mock_read("thrd_set_log_mask(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_get_log_mask(int res)
{
    harness_mock_write("thrd_get_log_mask()",
                       NULL,
                       0);

    harness_mock_write("thrd_get_log_mask(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_get_log_mask)()
{
    int res;

    harness_mock_assert("thrd_get_log_mask()",
                        NULL,
                        0);

    harness_mock_read("thrd_get_log_mask(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_set_prio(struct thrd_t *thrd_p,
                             int prio,
                             int res)
{
    harness_mock_write("thrd_set_prio(thrd_p)",
                       thrd_p,
                       sizeof(*thrd_p));

    harness_mock_write("thrd_set_prio(prio)",
                       &prio,
                       sizeof(prio));

    harness_mock_write("thrd_set_prio(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_set_prio)(struct thrd_t *thrd_p,
                                               int prio)
{
    int res;

    harness_mock_assert("thrd_set_prio(thrd_p)",
                        thrd_p,
                        sizeof(*thrd_p));

    harness_mock_assert("thrd_set_prio(prio)",
                        &prio,
                        sizeof(prio));

    harness_mock_read("thrd_set_prio(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_get_prio(int res)
{
    harness_mock_write("thrd_get_prio()",
                       NULL,
                       0);

    harness_mock_write("thrd_get_prio(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_get_prio)()
{
    int res;

    harness_mock_assert("thrd_get_prio()",
                        NULL,
                        0);

    harness_mock_read("thrd_get_prio(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_init_global_env(struct thrd_environment_variable_t *variables_p,
                                    int length,
                                    int res)
{
    harness_mock_write("thrd_init_global_env(variables_p)",
                       variables_p,
                       sizeof(*variables_p));

    harness_mock_write("thrd_init_global_env(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("thrd_init_global_env(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_init_global_env)(struct thrd_environment_variable_t *variables_p,
                                                      int length)
{
    int res;

    harness_mock_assert("thrd_init_global_env(variables_p)",
                        variables_p,
                        sizeof(*variables_p));

    harness_mock_assert("thrd_init_global_env(length)",
                        &length,
                        sizeof(length));

    harness_mock_read("thrd_init_global_env(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_set_global_env(const char *name_p,
                                   const char *value_p,
                                   int res)
{
    harness_mock_write("thrd_set_global_env(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("thrd_set_global_env(value_p)",
                       value_p,
                       strlen(value_p) + 1);

    harness_mock_write("thrd_set_global_env(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_set_global_env)(const char *name_p,
                                                     const char *value_p)
{
    int res;

    harness_mock_assert("thrd_set_global_env(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("thrd_set_global_env(value_p)",
                        value_p,
                        sizeof(*value_p));

    harness_mock_read("thrd_set_global_env(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_get_global_env(const char *name_p,
                                   const char *res)
{
    harness_mock_write("thrd_get_global_env(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("thrd_get_global_env(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(thrd_get_global_env)(const char *name_p)
{
    const char *res;

    harness_mock_assert("thrd_get_global_env(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_read("thrd_get_global_env(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_init_env(struct thrd_environment_variable_t *variables_p,
                             int length,
                             int res)
{
    harness_mock_write("thrd_init_env(variables_p)",
                       variables_p,
                       sizeof(*variables_p));

    harness_mock_write("thrd_init_env(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("thrd_init_env(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_init_env)(struct thrd_environment_variable_t *variables_p,
                                               int length)
{
    int res;

    harness_mock_assert("thrd_init_env(variables_p)",
                        variables_p,
                        sizeof(*variables_p));

    harness_mock_assert("thrd_init_env(length)",
                        &length,
                        sizeof(length));

    harness_mock_read("thrd_init_env(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_set_env(const char *name_p,
                            const char *value_p,
                            int res)
{
    harness_mock_write("thrd_set_env(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("thrd_set_env(value_p)",
                       value_p,
                       strlen(value_p) + 1);

    harness_mock_write("thrd_set_env(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_set_env)(const char *name_p,
                                              const char *value_p)
{
    int res;

    harness_mock_assert("thrd_set_env(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_assert("thrd_set_env(value_p)",
                        value_p,
                        sizeof(*value_p));

    harness_mock_read("thrd_set_env(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_get_env(const char *name_p,
                            const char *res)
{
    harness_mock_write("thrd_get_env(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("thrd_get_env(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const char *__attribute__ ((weak)) STUB(thrd_get_env)(const char *name_p)
{
    const char *res;

    harness_mock_assert("thrd_get_env(name_p)",
                        name_p,
                        sizeof(*name_p));

    harness_mock_read("thrd_get_env(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_suspend_isr(const struct time_t *timeout_p,
                                int res)
{
    harness_mock_write("thrd_suspend_isr(timeout_p)",
                       timeout_p,
                       sizeof(*timeout_p));

    harness_mock_write("thrd_suspend_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_suspend_isr)(const struct time_t *timeout_p)
{
    int res;

    harness_mock_assert("thrd_suspend_isr(timeout_p)",
                        timeout_p,
                        sizeof(*timeout_p));

    harness_mock_read("thrd_suspend_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_resume_isr(struct thrd_t *thrd_p,
                               int err,
                               int res)
{
    harness_mock_write("thrd_resume_isr(thrd_p)",
                       thrd_p,
                       sizeof(*thrd_p));

    harness_mock_write("thrd_resume_isr(err)",
                       &err,
                       sizeof(err));

    harness_mock_write("thrd_resume_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_resume_isr)(struct thrd_t *thrd_p,
                                                 int err)
{
    int res;

    harness_mock_assert("thrd_resume_isr(thrd_p)",
                        thrd_p,
                        sizeof(*thrd_p));

    harness_mock_assert("thrd_resume_isr(err)",
                        &err,
                        sizeof(err));

    harness_mock_read("thrd_resume_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_yield_isr(int res)
{
    harness_mock_write("thrd_yield_isr()",
                       NULL,
                       0);

    harness_mock_write("thrd_yield_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_yield_isr)()
{
    int res;

    harness_mock_assert("thrd_yield_isr()",
                        NULL,
                        0);

    harness_mock_read("thrd_yield_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_stack_alloc(size_t size,
                                void *res)
{
    harness_mock_write("thrd_stack_alloc(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("thrd_stack_alloc(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(thrd_stack_alloc)(size_t size)
{
    void *res;

    harness_mock_assert("thrd_stack_alloc(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("thrd_stack_alloc(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_stack_free(void *stack_p,
                               int res)
{
    harness_mock_write("thrd_stack_free(): return (stack_p)",
                       stack_p,
                       sizeof(stack_p));

    harness_mock_write("thrd_stack_free(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_stack_free)(void *stack_p)
{
    int res;

    harness_mock_read("thrd_stack_free(): return (stack_p)",
                      stack_p,
                      sizeof(*stack_p));

    harness_mock_read("thrd_stack_free(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_get_bottom_of_stack(struct thrd_t *thrd_p,
                                        const void *res)
{
    harness_mock_write("thrd_get_bottom_of_stack(): return (thrd_p)",
                       thrd_p,
                       sizeof(*thrd_p));

    harness_mock_write("thrd_get_bottom_of_stack(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const void *__attribute__ ((weak)) STUB(thrd_get_bottom_of_stack)(struct thrd_t *thrd_p)
{
    const void *res;

    harness_mock_read("thrd_get_bottom_of_stack(): return (thrd_p)",
                      thrd_p,
                      sizeof(*thrd_p));

    harness_mock_read("thrd_get_bottom_of_stack(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_get_top_of_stack(struct thrd_t *thrd_p,
                                     const void *res)
{
    harness_mock_write("thrd_get_top_of_stack(): return (thrd_p)",
                       thrd_p,
                       sizeof(*thrd_p));

    harness_mock_write("thrd_get_top_of_stack(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

const void *__attribute__ ((weak)) STUB(thrd_get_top_of_stack)(struct thrd_t *thrd_p)
{
    const void *res;

    harness_mock_read("thrd_get_top_of_stack(): return (thrd_p)",
                      thrd_p,
                      sizeof(*thrd_p));

    harness_mock_read("thrd_get_top_of_stack(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_prio_list_init(int res)
{
    harness_mock_write("thrd_prio_list_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_prio_list_init)(struct thrd_prio_list_t *self_p)
{
    int res;

    harness_mock_read("thrd_prio_list_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_prio_list_push_isr(struct thrd_prio_list_elem_t *elem_p)
{
    harness_mock_write("thrd_prio_list_push_isr(elem_p)",
                       elem_p,
                       sizeof(*elem_p));

    return (0);
}

void __attribute__ ((weak)) STUB(thrd_prio_list_push_isr)(struct thrd_prio_list_t *self_p,
                                                          struct thrd_prio_list_elem_t *elem_p)
{
    harness_mock_assert("thrd_prio_list_push_isr(elem_p)",
                        elem_p,
                        sizeof(*elem_p));
}

int mock_write_thrd_prio_list_pop_isr(struct thrd_prio_list_elem_t *res)
{
    harness_mock_write("thrd_prio_list_pop_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct thrd_prio_list_elem_t *__attribute__ ((weak)) STUB(thrd_prio_list_pop_isr)(struct thrd_prio_list_t *self_p)
{
    struct thrd_prio_list_elem_t *res;

    harness_mock_read("thrd_prio_list_pop_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_thrd_prio_list_remove_isr(struct thrd_prio_list_elem_t *elem_p,
                                         int res)
{
    harness_mock_write("thrd_prio_list_remove_isr(elem_p)",
                       elem_p,
                       sizeof(*elem_p));

    harness_mock_write("thrd_prio_list_remove_isr(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(thrd_prio_list_remove_isr)(struct thrd_prio_list_t *self_p,
                                                           struct thrd_prio_list_elem_t *elem_p)
{
    int res;

    harness_mock_assert("thrd_prio_list_remove_isr(elem_p)",
                        elem_p,
                        sizeof(*elem_p));

    harness_mock_read("thrd_prio_list_remove_isr(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
