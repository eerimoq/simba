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

#ifndef __HARNESS_MOCK_H__
#define __HARNESS_MOCK_H__

#include "simba.h"

int mock_write_harness_init(int res);

int mock_write_harness_run(struct harness_testcase_t *testcases_p,
                           int res);

int mock_write_harness_expect(void *chan_p,
                              const char *pattern_p,
                              const struct time_t *timeout_p,
                              int res);

int mock_write_harness_mock_write(const char *id_p,
                                  const void *buf_p,
                                  size_t size,
                                  ssize_t res);

int mock_write_harness_mock_read(const char *id_p,
                                 void *buf_p,
                                 ssize_t size,
                                 ssize_t res);

int mock_write_harness_mock_assert(const char *id_p,
                                   const void *buf_p,
                                   int res);

int mock_write_harness_mock_try_read(const char *id_p,
                                     void *buf_p,
                                     ssize_t size,
                                     ssize_t res);

int mock_write_harness_mock_write_notify(const char *id_p,
                                         const void *buf_p,
                                         size_t size,
                                         ssize_t res);

int mock_write_harness_mock_read_wait(const char *id_p,
                                      void *buf_p,
                                      size_t size,
                                      struct time_t *timeout_p,
                                      ssize_t res);

#endif
