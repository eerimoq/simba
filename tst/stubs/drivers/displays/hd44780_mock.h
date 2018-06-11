/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

#ifndef __HD44780_MOCK_H__
#define __HD44780_MOCK_H__

#include "simba.h"

int mock_write_hd44780_module_init(int res);

int mock_write_hd44780_init(struct pin_device_t *rs_p,
                            struct pin_device_t *enable_p,
                            struct pin_device_t *data_4_p,
                            struct pin_device_t *data_5_p,
                            struct pin_device_t *data_6_p,
                            struct pin_device_t *data_7_p,
                            unsigned int number_of_rows,
                            unsigned int number_of_columns,
                            int res);

int mock_write_hd44780_start(int res);

int mock_write_hd44780_stop(int res);

int mock_write_hd44780_display(const char *text_p,
                               int res);

int mock_write_hd44780_write(const char *text_p,
                             int res);

int mock_write_hd44780_put(char character,
                           int res);

int mock_write_hd44780_clear(int res);

int mock_write_hd44780_cursor_move(unsigned int row,
                                   unsigned int column,
                                   int res);

int mock_write_hd44780_cursor_show(int res);

int mock_write_hd44780_cursor_hide(int res);

int mock_write_hd44780_text_show(int res);

int mock_write_hd44780_text_hide(int res);

int mock_write_hd44780_scroll_left(int res);

int mock_write_hd44780_scroll_right(int res);

#endif
