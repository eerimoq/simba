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
#include "re_mock.h"

int mock_write_re_module_init(int res)
{
    harness_mock_write("re_module_init()",
                       NULL,
                       0);

    harness_mock_write("re_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(re_module_init)()
{
    int res;

    harness_mock_assert("re_module_init()",
                        NULL,
                        0);

    harness_mock_read("re_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_re_compile(char *compiled_p,
                          const char *pattern_p,
                          char flags,
                          size_t size,
                          char *res)
{
    harness_mock_write("re_compile(): return (compiled_p)",
                       compiled_p,
                       strlen(compiled_p) + 1);

    harness_mock_write("re_compile(pattern_p)",
                       pattern_p,
                       strlen(pattern_p) + 1);

    harness_mock_write("re_compile(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("re_compile(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("re_compile(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

char *__attribute__ ((weak)) STUB(re_compile)(char *compiled_p,
                                              const char *pattern_p,
                                              char flags,
                                              size_t size)
{
    char *res;

    harness_mock_read("re_compile(): return (compiled_p)",
                      compiled_p,
                      sizeof(*compiled_p));

    harness_mock_assert("re_compile(pattern_p)",
                        pattern_p,
                        sizeof(*pattern_p));

    harness_mock_assert("re_compile(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_assert("re_compile(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("re_compile(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_re_match(const char *compiled_p,
                        const char *buf_p,
                        size_t size,
                        struct re_group_t *groups_p,
                        size_t *number_of_groups_p,
                        ssize_t res)
{
    harness_mock_write("re_match(compiled_p)",
                       compiled_p,
                       strlen(compiled_p) + 1);

    harness_mock_write("re_match(buf_p)",
                       buf_p,
                       strlen(buf_p) + 1);

    harness_mock_write("re_match(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("re_match(): return (groups_p)",
                       groups_p,
                       sizeof(*groups_p));

    harness_mock_write("re_match(): return (number_of_groups_p)",
                       number_of_groups_p,
                       sizeof(*number_of_groups_p));

    harness_mock_write("re_match(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(re_match)(const char *compiled_p,
                                              const char *buf_p,
                                              size_t size,
                                              struct re_group_t *groups_p,
                                              size_t *number_of_groups_p)
{
    ssize_t res;

    harness_mock_assert("re_match(compiled_p)",
                        compiled_p,
                        sizeof(*compiled_p));

    harness_mock_assert("re_match(buf_p)",
                        buf_p,
                        sizeof(*buf_p));

    harness_mock_assert("re_match(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("re_match(): return (groups_p)",
                      groups_p,
                      sizeof(*groups_p));

    harness_mock_read("re_match(): return (number_of_groups_p)",
                      number_of_groups_p,
                      sizeof(*number_of_groups_p));

    harness_mock_read("re_match(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
