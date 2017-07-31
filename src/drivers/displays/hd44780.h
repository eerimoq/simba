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

#ifndef __DRIVERS_HD44780_H__
#define __DRIVERS_HD44780_H__

struct hd44780_driver_t {
    struct pin_device_t *rs_p;
    struct pin_device_t *enable_p;
    struct pin_device_t *data_4_p;
    struct pin_device_t *data_5_p;
    struct pin_device_t *data_6_p;
    struct pin_device_t *data_7_p;
    unsigned int number_of_rows;
    unsigned int number_of_columns;
    struct {
        unsigned int row;
        unsigned int column;
    } cursor;
    uint8_t display_on_off_control;
};

/**
 * Initialize the driver module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_module_init(void);

/**
 * Initialize driver object. The driver object will be used for a
 * single display.
 *
 * @param[out] self_p Driver object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_init(struct hd44780_driver_t *self_p,
                 struct pin_device_t *rs_p,
                 struct pin_device_t *enable_p,
                 struct pin_device_t *data_4_p,
                 struct pin_device_t *data_5_p,
                 struct pin_device_t *data_6_p,
                 struct pin_device_t *data_7_p,
                 unsigned int number_of_rows,
                 unsigned int number_of_columns);

/**
 * Start the driver.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_start(struct hd44780_driver_t *self_p);

/**
 * Stop the driver.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_stop(struct hd44780_driver_t *self_p);

/**
 * Display given text on the display.
 *
 * @param[in] self_p Driver object.
 * @param[in] text_p Text to display.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_display(struct hd44780_driver_t *self_p,
                    const char *text_p);

/**
 * Write given text to the display at current cursor position.
 *
 * @param[in] self_p Driver object.
 * @param[in] text_p Text to write to the display.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_write(struct hd44780_driver_t *self_p,
                  const char *text_p);

/**
 * Write given character to the display at current cursor position.
 *
 * @param[in] self_p Driver object.
 * @param[in] character Character to write to the display.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_put(struct hd44780_driver_t *self_p, char character);

/**
 * Clear the display. 
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_clear(struct hd44780_driver_t *self_p);

/**
 * Move the cursor to given row and column, relative to the upper
 * left-hand corner of the screen, which is (0, 0).
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_cursor_move(struct hd44780_driver_t *self_p,
                        unsigned int row,
                        unsigned int column);

/**
 * Show the cursor on the display.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_cursor_show(struct hd44780_driver_t *self_p);

/**
 * Do not show the cursor on the display.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_cursor_hide(struct hd44780_driver_t *self_p);

/**
 * Show the text on the display.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_text_show(struct hd44780_driver_t *self_p);

/**
 * Do not show the text on the display.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_text_hide(struct hd44780_driver_t *self_p);

/**
 * Scroll the text on the display one position to the left.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_scroll_left(struct hd44780_driver_t *self_p);

/**
 * Scroll the text on the display one position to the right.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int hd44780_scroll_right(struct hd44780_driver_t *self_p);

#endif
