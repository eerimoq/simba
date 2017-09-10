/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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
#include "hd44780_mock.h"

int mock_write_hd44780_module_init(int res)
{
    harness_mock_write("hd44780_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_module_init)()
{
    int res;

    harness_mock_read("hd44780_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_init(struct pin_device_t *rs_p,
                            struct pin_device_t *enable_p,
                            struct pin_device_t *data_4_p,
                            struct pin_device_t *data_5_p,
                            struct pin_device_t *data_6_p,
                            struct pin_device_t *data_7_p,
                            unsigned int number_of_rows,
                            unsigned int number_of_columns,
                            int res)
{
    harness_mock_write("hd44780_init(): return (rs_p)",
                       rs_p,
                       sizeof(*rs_p));

    harness_mock_write("hd44780_init(): return (enable_p)",
                       enable_p,
                       sizeof(*enable_p));

    harness_mock_write("hd44780_init(): return (data_4_p)",
                       data_4_p,
                       sizeof(*data_4_p));

    harness_mock_write("hd44780_init(): return (data_5_p)",
                       data_5_p,
                       sizeof(*data_5_p));

    harness_mock_write("hd44780_init(): return (data_6_p)",
                       data_6_p,
                       sizeof(*data_6_p));

    harness_mock_write("hd44780_init(): return (data_7_p)",
                       data_7_p,
                       sizeof(*data_7_p));

    harness_mock_write("hd44780_init(number_of_rows)",
                       &number_of_rows,
                       sizeof(number_of_rows));

    harness_mock_write("hd44780_init(number_of_columns)",
                       &number_of_columns,
                       sizeof(number_of_columns));

    harness_mock_write("hd44780_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_init)(struct hd44780_driver_t *self_p,
                                              struct pin_device_t *rs_p,
                                              struct pin_device_t *enable_p,
                                              struct pin_device_t *data_4_p,
                                              struct pin_device_t *data_5_p,
                                              struct pin_device_t *data_6_p,
                                              struct pin_device_t *data_7_p,
                                              unsigned int number_of_rows,
                                              unsigned int number_of_columns)
{
    int res;

    harness_mock_read("hd44780_init(): return (rs_p)",
                      rs_p,
                      -1);

    harness_mock_read("hd44780_init(): return (enable_p)",
                      enable_p,
                      -1);

    harness_mock_read("hd44780_init(): return (data_4_p)",
                      data_4_p,
                      -1);

    harness_mock_read("hd44780_init(): return (data_5_p)",
                      data_5_p,
                      -1);

    harness_mock_read("hd44780_init(): return (data_6_p)",
                      data_6_p,
                      -1);

    harness_mock_read("hd44780_init(): return (data_7_p)",
                      data_7_p,
                      -1);

    harness_mock_assert("hd44780_init(number_of_rows)",
                        &number_of_rows);

    harness_mock_assert("hd44780_init(number_of_columns)",
                        &number_of_columns);

    harness_mock_read("hd44780_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_start(int res)
{
    harness_mock_write("hd44780_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_start)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_stop(int res)
{
    harness_mock_write("hd44780_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_stop)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_display(const char *text_p,
                               int res)
{
    harness_mock_write("hd44780_display(text_p)",
                       text_p,
                       strlen(text_p) + 1);

    harness_mock_write("hd44780_display(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_display)(struct hd44780_driver_t *self_p,
                                                 const char *text_p)
{
    int res;

    harness_mock_assert("hd44780_display(text_p)",
                        text_p);

    harness_mock_read("hd44780_display(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_write(const char *text_p,
                             int res)
{
    harness_mock_write("hd44780_write(text_p)",
                       text_p,
                       strlen(text_p) + 1);

    harness_mock_write("hd44780_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_write)(struct hd44780_driver_t *self_p,
                                               const char *text_p)
{
    int res;

    harness_mock_assert("hd44780_write(text_p)",
                        text_p);

    harness_mock_read("hd44780_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_put(char character,
                           int res)
{
    harness_mock_write("hd44780_put(character)",
                       &character,
                       sizeof(character));

    harness_mock_write("hd44780_put(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_put)(struct hd44780_driver_t *self_p,
                                             char character)
{
    int res;

    harness_mock_assert("hd44780_put(character)",
                        &character);

    harness_mock_read("hd44780_put(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_clear(int res)
{
    harness_mock_write("hd44780_clear(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_clear)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_clear(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_cursor_move(unsigned int row,
                                   unsigned int column,
                                   int res)
{
    harness_mock_write("hd44780_cursor_move(row)",
                       &row,
                       sizeof(row));

    harness_mock_write("hd44780_cursor_move(column)",
                       &column,
                       sizeof(column));

    harness_mock_write("hd44780_cursor_move(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_cursor_move)(struct hd44780_driver_t *self_p,
                                                     unsigned int row,
                                                     unsigned int column)
{
    int res;

    harness_mock_assert("hd44780_cursor_move(row)",
                        &row);

    harness_mock_assert("hd44780_cursor_move(column)",
                        &column);

    harness_mock_read("hd44780_cursor_move(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_cursor_show(int res)
{
    harness_mock_write("hd44780_cursor_show(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_cursor_show)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_cursor_show(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_cursor_hide(int res)
{
    harness_mock_write("hd44780_cursor_hide(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_cursor_hide)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_cursor_hide(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_text_show(int res)
{
    harness_mock_write("hd44780_text_show(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_text_show)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_text_show(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_text_hide(int res)
{
    harness_mock_write("hd44780_text_hide(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_text_hide)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_text_hide(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_scroll_left(int res)
{
    harness_mock_write("hd44780_scroll_left(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_scroll_left)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_scroll_left(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_hd44780_scroll_right(int res)
{
    harness_mock_write("hd44780_scroll_right(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(hd44780_scroll_right)(struct hd44780_driver_t *self_p)
{
    int res;

    harness_mock_read("hd44780_scroll_right(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
