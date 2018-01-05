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

#ifndef __CHAN_MOCK_H__
#define __CHAN_MOCK_H__

#include "simba.h"

int mock_write_chan_module_init(int res);

int mock_write_chan_init(chan_read_fn_t read,
                         chan_write_fn_t write,
                         chan_size_fn_t size,
                         int res);

int mock_write_chan_set_write_cb(chan_write_fn_t write_cb,
                                 int res);

int mock_write_chan_set_write_isr_cb(chan_write_fn_t write_isr_cb,
                                     int res);

int mock_write_chan_set_write_filter_cb(chan_write_filter_fn_t write_filter_cb,
                                        int res);

int mock_write_chan_set_write_filter_isr_cb(chan_write_filter_fn_t write_filter_isr_cb,
                                            int res);

int mock_write_chan_set_control_cb(chan_control_fn_t control_cb,
                                   int res);

int mock_write_chan_read(void *buf_p,
                         size_t size,
                         ssize_t res);

int mock_write_chan_write(const void *buf_p,
                          size_t size,
                          ssize_t res);

int mock_write_chan_getc(int res);

int mock_write_chan_putc(int character,
                         int res);

int mock_write_chan_size(size_t res);

int mock_write_chan_control(int operation,
                            int res);

int mock_write_chan_write_isr(const void *buf_p,
                              size_t size,
                              ssize_t res);

int mock_write_chan_is_polled_isr(int res);

int mock_write_chan_list_init(struct chan_list_t *list_p,
                              struct chan_list_elem_t *elements_p,
                              size_t number_of_elements,
                              int res);

int mock_write_chan_list_destroy(struct chan_list_t *list_p,
                                 int res);

int mock_write_chan_list_add(struct chan_list_t *list_p,
                             void *chan_p,
                             int res);

int mock_write_chan_list_remove(struct chan_list_t *list_p,
                                void *chan_p,
                                int res);

int mock_write_chan_list_poll(struct chan_list_t *list_p,
                              const struct time_t *timeout_p,
                              void *res);

int mock_write_chan_poll(void *chan_p,
                         const struct time_t *timeout_p,
                         void *res);

int mock_write_chan_null(void *res);

int mock_write_chan_read_null(void *buf_p,
                              size_t size,
                              ssize_t res);

int mock_write_chan_write_null(const void *buf_p,
                               size_t size,
                               ssize_t res);

int mock_write_chan_size_null(size_t res);

int mock_write_chan_control_null(int operation,
                                 int res);

#endif
