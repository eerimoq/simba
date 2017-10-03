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
#include "configfile_mock.h"

int mock_write_configfile_init(char *buf_p,
                               size_t size,
                               int res)
{
    harness_mock_write("configfile_init(buf_p)",
                       buf_p,
                       strlen(buf_p) + 1);

    harness_mock_write("configfile_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("configfile_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(configfile_init)(struct configfile_t *self_p,
                                                 char *buf_p,
                                                 size_t size)
{
    int res;

    harness_mock_assert("configfile_init(buf_p)",
                        buf_p,
                        sizeof(*buf_p));

    harness_mock_assert("configfile_init(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("configfile_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_configfile_set(const char *section_p,
                              const char *property_p,
                              const char *value_p,
                              int res)
{
    harness_mock_write("configfile_set(section_p)",
                       section_p,
                       strlen(section_p) + 1);

    harness_mock_write("configfile_set(property_p)",
                       property_p,
                       strlen(property_p) + 1);

    harness_mock_write("configfile_set(value_p)",
                       value_p,
                       strlen(value_p) + 1);

    harness_mock_write("configfile_set(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(configfile_set)(struct configfile_t *self_p,
                                                const char *section_p,
                                                const char *property_p,
                                                const char *value_p)
{
    int res;

    harness_mock_assert("configfile_set(section_p)",
                        section_p,
                        sizeof(*section_p));

    harness_mock_assert("configfile_set(property_p)",
                        property_p,
                        sizeof(*property_p));

    harness_mock_assert("configfile_set(value_p)",
                        value_p,
                        sizeof(*value_p));

    harness_mock_read("configfile_set(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_configfile_get(const char *section_p,
                              const char *property_p,
                              char *value_p,
                              int length,
                              char *res)
{
    harness_mock_write("configfile_get(section_p)",
                       section_p,
                       strlen(section_p) + 1);

    harness_mock_write("configfile_get(property_p)",
                       property_p,
                       strlen(property_p) + 1);

    harness_mock_write("configfile_get(): return (value_p)",
                       value_p,
                       strlen(value_p) + 1);

    harness_mock_write("configfile_get(length)",
                       &length,
                       sizeof(length));

    harness_mock_write("configfile_get(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

char *__attribute__ ((weak)) STUB(configfile_get)(struct configfile_t *self_p,
                                                  const char *section_p,
                                                  const char *property_p,
                                                  char *value_p,
                                                  int length)
{
    char *res;

    harness_mock_assert("configfile_get(section_p)",
                        section_p,
                        sizeof(*section_p));

    harness_mock_assert("configfile_get(property_p)",
                        property_p,
                        sizeof(*property_p));

    harness_mock_read("configfile_get(): return (value_p)",
                      value_p,
                      sizeof(*value_p));

    harness_mock_assert("configfile_get(length)",
                        &length,
                        sizeof(length));

    harness_mock_read("configfile_get(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_configfile_get_long(const char *section_p,
                                   const char *property_p,
                                   long *value_p,
                                   int res)
{
    harness_mock_write("configfile_get_long(section_p)",
                       section_p,
                       strlen(section_p) + 1);

    harness_mock_write("configfile_get_long(property_p)",
                       property_p,
                       strlen(property_p) + 1);

    harness_mock_write("configfile_get_long(): return (value_p)",
                       value_p,
                       sizeof(*value_p));

    harness_mock_write("configfile_get_long(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(configfile_get_long)(struct configfile_t *self_p,
                                                     const char *section_p,
                                                     const char *property_p,
                                                     long *value_p)
{
    int res;

    harness_mock_assert("configfile_get_long(section_p)",
                        section_p,
                        sizeof(*section_p));

    harness_mock_assert("configfile_get_long(property_p)",
                        property_p,
                        sizeof(*property_p));

    harness_mock_read("configfile_get_long(): return (value_p)",
                      value_p,
                      sizeof(*value_p));

    harness_mock_read("configfile_get_long(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_configfile_get_float(const char *section_p,
                                    const char *property_p,
                                    float *value_p,
                                    int res)
{
    harness_mock_write("configfile_get_float(section_p)",
                       section_p,
                       strlen(section_p) + 1);

    harness_mock_write("configfile_get_float(property_p)",
                       property_p,
                       strlen(property_p) + 1);

    harness_mock_write("configfile_get_float(): return (value_p)",
                       value_p,
                       sizeof(*value_p));

    harness_mock_write("configfile_get_float(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(configfile_get_float)(struct configfile_t *self_p,
                                                      const char *section_p,
                                                      const char *property_p,
                                                      float *value_p)
{
    int res;

    harness_mock_assert("configfile_get_float(section_p)",
                        section_p,
                        sizeof(*section_p));

    harness_mock_assert("configfile_get_float(property_p)",
                        property_p,
                        sizeof(*property_p));

    harness_mock_read("configfile_get_float(): return (value_p)",
                      value_p,
                      sizeof(*value_p));

    harness_mock_read("configfile_get_float(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
