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

/* Instructions. */
#define CLEAR_DISPLAY             BIT(0)
#define CURSOR_HOME               BIT(1)
#define ENTRY_MODE_SET            BIT(2)
#define DISPLAY_ON_OFF_CONTROL    BIT(3)
#define CURSOR_DISPLAY_SHIFT      BIT(4)
#define FUNCTION_SET              BIT(5)
#define SET_CGRAM_ADDRESS         BIT(6)
#define SET_DDRAM_ADDRESS         BIT(7)

#define ENTRY_MODE_SET_I_D        BIT(1)

#define DISPLAY_ON_OFF_CONTROL_D  BIT(2)
#define DISPLAY_ON_OFF_CONTROL_C  BIT(1)
#define DISPLAY_ON_OFF_CONTROL_B  BIT(0)

#define CURSOR_DISPLAY_SHIFT_S_C  BIT(3)
#define CURSOR_DISPLAY_SHIFT_L_R  BIT(2)

#define FUNCTION_SET_DL           BIT(4)
#define FUNCTION_SET_N            BIT(3)
#define FUNCTION_SET_F            BIT(2)

/* Instruction delays. */
#define DELAY_1520_US       1520
#define DELAY_37_US           37

static void write_nibble(struct hd44780_driver_t *self_p,
                         uint8_t data)
{
    pin_device_write(self_p->data_4_p, (data & 0x8));
    pin_device_write(self_p->data_5_p, (data & 0x4));
    pin_device_write(self_p->data_6_p, (data & 0x2));
    pin_device_write(self_p->data_7_p, (data & 0x1));

    pin_device_write_low(self_p->enable_p);
    time_busy_wait_us(1);
    pin_device_write_high(self_p->enable_p);
}

static void write_byte(struct hd44780_driver_t *self_p,
                       uint8_t data,
                       long delay_us)
{
    write_nibble(self_p, data >> 4);
    write_nibble(self_p, data);
    time_busy_wait_us(delay_us);
}

static int write_command_sleep(struct hd44780_driver_t *self_p,
                               uint8_t data,
                               long delay_us)
{
    pin_device_write_low(self_p->rs_p);
    write_byte(self_p, data, 0);

    return (thrd_sleep_us(delay_us));
}

static int write_command_busy_wait(struct hd44780_driver_t *self_p,
                                   uint8_t data,
                                   long delay_us)
{
    pin_device_write_low(self_p->rs_p);
    write_byte(self_p, data, delay_us);
    
    return (0);
}

static void write_data(struct hd44780_driver_t *self_p,
                       uint8_t data)
{
    pin_device_write_high(self_p->rs_p);
    write_byte(self_p, data, DELAY_37_US);
}

int hd44780_module_init()
{
    return (0);
}

int hd44780_init(struct hd44780_driver_t *self_p,
                 struct pin_device_t *rs_p,
                 struct pin_device_t *enable_p,
                 struct pin_device_t *data_4_p,
                 struct pin_device_t *data_5_p,
                 struct pin_device_t *data_6_p,
                 struct pin_device_t *data_7_p,
                 unsigned int number_of_rows,
                 unsigned int number_of_columns)
{
    self_p->rs_p = rs_p;
    self_p->enable_p = enable_p;
    self_p->data_4_p = data_4_p;
    self_p->data_5_p = data_5_p;
    self_p->data_6_p = data_6_p;
    self_p->data_7_p = data_7_p;
    self_p->number_of_rows = number_of_rows;
    self_p->number_of_columns = number_of_columns;
    self_p->display_on_off_control = DISPLAY_ON_OFF_CONTROL_D;

    return (0);
}

int hd44780_start(struct hd44780_driver_t *self_p)
{
    /* Give the device time to start. */
    thrd_sleep_ms(50);
    
    /* Get to a known state, 8 bit mode. */
    write_command_busy_wait(self_p,
                            (FUNCTION_SET | FUNCTION_SET_DL),
                            0);
    thrd_sleep_ms(5);
    write_command_busy_wait(self_p,
                            (FUNCTION_SET | FUNCTION_SET_DL),
                            0);
    thrd_sleep_us(100);
    write_command_busy_wait(self_p,
                            (FUNCTION_SET | FUNCTION_SET_DL),
                            DELAY_37_US);
   
    /* Set 4 bit mode. */
    write_command_busy_wait(self_p, FUNCTION_SET, DELAY_37_US);

    /* Default configuration. */
    write_command_busy_wait(self_p,
                            (ENTRY_MODE_SET | ENTRY_MODE_SET_I_D),
                            DELAY_37_US);
    hd44780_text_show(self_p);

    return (hd44780_clear(self_p));
}

int hd44780_stop(struct hd44780_driver_t *self_p)
{
    return (hd44780_clear(self_p));
}

int hd44780_display(struct hd44780_driver_t *self_p,
                    const char *text_p)
{
    hd44780_clear(self_p);
    
    return (hd44780_write(self_p, text_p));
}

int hd44780_write(struct hd44780_driver_t *self_p,
                  const char *text_p)
{
    while (*text_p != '\0') {
        hd44780_put(self_p, *text_p++);
    }

    return (0);
}

int hd44780_put(struct hd44780_driver_t *self_p, char character)
{
    if (character == '\r') {
        hd44780_cursor_move(self_p, self_p->cursor.row, 0);
    } else if (character == '\n') {
        hd44780_cursor_move(self_p,
                            self_p->cursor.row + 1,
                            self_p->cursor.column);
    } else if (self_p->cursor.column < self_p->number_of_columns) {
        write_data(self_p, (uint8_t)character);
        self_p->cursor.column++;
    }

    return (0);
}

int hd44780_clear(struct hd44780_driver_t *self_p)
{
    self_p->cursor.row = 0;
    self_p->cursor.column = 0;

    write_command_sleep(self_p, CLEAR_DISPLAY, DELAY_1520_US);

    return (write_command_sleep(self_p, CURSOR_HOME, DELAY_1520_US));
}

int hd44780_cursor_move(struct hd44780_driver_t *self_p,
                        unsigned int row,
                        unsigned int column)
{
    uint8_t command;

    if (row >= self_p->number_of_rows) {
        row = 0;
    }

    if (column >= self_p->number_of_columns) {
        column = 0;
    }

    self_p->cursor.row = row;
    self_p->cursor.column = column;

    command = (SET_DDRAM_ADDRESS | ((0x40 * row) + column));

    return (write_command_busy_wait(self_p,
                                    command,
                                    DELAY_37_US));
}

int hd44780_cursor_show(struct hd44780_driver_t *self_p)
{
    self_p->display_on_off_control |= DISPLAY_ON_OFF_CONTROL_C;
        
    return (write_command_busy_wait(self_p,
                                    self_p->display_on_off_control,
                                    DELAY_37_US));
}

int hd44780_cursor_hide(struct hd44780_driver_t *self_p)
{
    self_p->display_on_off_control &= ~DISPLAY_ON_OFF_CONTROL_C;
        
    return (write_command_busy_wait(self_p,
                                    self_p->display_on_off_control,
                                    DELAY_37_US));
}

int hd44780_text_show(struct hd44780_driver_t *self_p)
{
    self_p->display_on_off_control |= DISPLAY_ON_OFF_CONTROL_D;
        
    return (write_command_busy_wait(self_p,
                                    self_p->display_on_off_control,
                                    DELAY_37_US));
}

int hd44780_text_hide(struct hd44780_driver_t *self_p)
{
    self_p->display_on_off_control &= ~DISPLAY_ON_OFF_CONTROL_D;
        
    return (write_command_busy_wait(self_p,
                                    self_p->display_on_off_control,
                                    DELAY_37_US));
}

int hd44780_scroll_left(struct hd44780_driver_t *self_p)
{
    return (write_command_busy_wait(self_p,
                                    (CURSOR_DISPLAY_SHIFT
                                     | CURSOR_DISPLAY_SHIFT_S_C),
                                    DELAY_37_US));
}

int hd44780_scroll_right(struct hd44780_driver_t *self_p)
{
    return (write_command_busy_wait(self_p,
                                    (CURSOR_DISPLAY_SHIFT
                                     | CURSOR_DISPLAY_SHIFT_S_C
                                     | CURSOR_DISPLAY_SHIFT_L_R),
                                    DELAY_37_US));
}
