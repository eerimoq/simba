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
#include "log_mock.h"

int mock_write_log_module_init(int res)
{
    harness_mock_write("log_module_init()",
                       NULL,
                       0);

    harness_mock_write("log_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_module_init)()
{
    int res;

    harness_mock_assert("log_module_init()",
                        NULL);

    harness_mock_read("log_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_object_init(const char *name_p,
                               char mask,
                               int res)
{
    harness_mock_write("log_object_init(name_p)",
                       name_p,
                       strlen(name_p) + 1);

    harness_mock_write("log_object_init(mask)",
                       &mask,
                       sizeof(mask));

    harness_mock_write("log_object_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_object_init)(struct log_object_t *self_p,
                                                 const char *name_p,
                                                 char mask)
{
    int res;

    harness_mock_assert("log_object_init(name_p)",
                        name_p);

    harness_mock_assert("log_object_init(mask)",
                        &mask);

    harness_mock_read("log_object_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_object_set_log_mask(char mask,
                                       int res)
{
    harness_mock_write("log_object_set_log_mask(mask)",
                       &mask,
                       sizeof(mask));

    harness_mock_write("log_object_set_log_mask(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_object_set_log_mask)(struct log_object_t *self_p,
                                                         char mask)
{
    int res;

    harness_mock_assert("log_object_set_log_mask(mask)",
                        &mask);

    harness_mock_read("log_object_set_log_mask(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_object_get_log_mask(char res)
{
    harness_mock_write("log_object_get_log_mask(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

char __attribute__ ((weak)) STUB(log_object_get_log_mask)(struct log_object_t *self_p)
{
    char res;

    harness_mock_read("log_object_get_log_mask(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_object_is_enabled_for(int level,
                                         int res)
{
    harness_mock_write("log_object_is_enabled_for(level)",
                       &level,
                       sizeof(level));

    harness_mock_write("log_object_is_enabled_for(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_object_is_enabled_for)(struct log_object_t *self_p,
                                                           int level)
{
    int res;

    harness_mock_assert("log_object_is_enabled_for(level)",
                        &level);

    harness_mock_read("log_object_is_enabled_for(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_object_print(int level,
                                const char *fmt_p,
                                int res)
{
    harness_mock_write("log_object_print(level)",
                       &level,
                       sizeof(level));

    harness_mock_write("log_object_print(fmt_p)",
                       fmt_p,
                       strlen(fmt_p) + 1);

    harness_mock_write("log_object_print(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_object_print)(struct log_object_t *self_p,
                                                  int level,
                                                  const char *fmt_p)
{
    int res;

    harness_mock_assert("log_object_print(level)",
                        &level);

    harness_mock_assert("log_object_print(fmt_p)",
                        fmt_p);

    harness_mock_read("log_object_print(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_handler_init(void *chout_p,
                                int res)
{
    harness_mock_write("log_handler_init(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("log_handler_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_handler_init)(struct log_handler_t *self_p,
                                                  void *chout_p)
{
    int res;

    harness_mock_assert("log_handler_init(chout_p)",
                        chout_p);

    harness_mock_read("log_handler_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_add_handler(struct log_handler_t *handler_p,
                               int res)
{
    harness_mock_write("log_add_handler(handler_p)",
                       handler_p,
                       sizeof(*handler_p));

    harness_mock_write("log_add_handler(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_add_handler)(struct log_handler_t *handler_p)
{
    int res;

    harness_mock_assert("log_add_handler(handler_p)",
                        handler_p);

    harness_mock_read("log_add_handler(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_remove_handler(struct log_handler_t *handler_p,
                                  int res)
{
    harness_mock_write("log_remove_handler(handler_p)",
                       handler_p,
                       sizeof(*handler_p));

    harness_mock_write("log_remove_handler(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_remove_handler)(struct log_handler_t *handler_p)
{
    int res;

    harness_mock_assert("log_remove_handler(handler_p)",
                        handler_p);

    harness_mock_read("log_remove_handler(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_add_object(struct log_object_t *object_p,
                              int res)
{
    harness_mock_write("log_add_object(object_p)",
                       object_p,
                       sizeof(*object_p));

    harness_mock_write("log_add_object(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_add_object)(struct log_object_t *object_p)
{
    int res;

    harness_mock_assert("log_add_object(object_p)",
                        object_p);

    harness_mock_read("log_add_object(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_remove_object(struct log_object_t *object_p,
                                 int res)
{
    harness_mock_write("log_remove_object(object_p)",
                       object_p,
                       sizeof(*object_p));

    harness_mock_write("log_remove_object(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_remove_object)(struct log_object_t *object_p)
{
    int res;

    harness_mock_assert("log_remove_object(object_p)",
                        object_p);

    harness_mock_read("log_remove_object(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_log_set_default_handler_output_channel(void *chout_p,
                                                      int res)
{
    harness_mock_write("log_set_default_handler_output_channel(chout_p)",
                       chout_p,
                       sizeof(chout_p));

    harness_mock_write("log_set_default_handler_output_channel(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(log_set_default_handler_output_channel)(void *chout_p)
{
    int res;

    harness_mock_assert("log_set_default_handler_output_channel(chout_p)",
                        chout_p);

    harness_mock_read("log_set_default_handler_output_channel(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
