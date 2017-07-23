/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Google Inc.
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

/*
 * This is a driver for Adafruit 0.56" 4-Digit 7-Segment Display w/I2C
 * Backpack which uses the Holtek HT16K33 chip.
 *
 * The driver has been tested with: https://www.adafruit.com/products/878
 *
 * Datasheet for Holtek HT16K33:
 * https://cdn-shop.adafruit.com/datasheets/ht16K33v110.pdf
 *
 * Protocol:
 *
 * The protocol on top of I2C for the chip / display is fairly
 * simple. One command byte possibly followed by databytes.
 *
 * The display has a RAM buffer which we write the digits to.  There
 * are a number of unused locations, which I presume is because the 7
 * digit display just doesn't hook those up to LEDs.
 *
 * Note that the colon is lid or not based on buffer position 2, so we
 * need to skip that when dealing with setting numbers in the drivers
 * internal buffer etc.
 *
 * For each digit we use the table from led_7seg_ht16k33_numbertable.i
 * to light the right LEDs.  We can bitwise-or a digit with
 * SEVEN_SEG_DOT to turn on the coresponding dot.
 *
 * I2C data packet we send is:
 * - Adddress (also tell display our command is writing to display buffer).
 * - Digit 0
 * - Unused
 * - Digit 1
 * - Unused
 * - Colon
 * - Unused
 * - Digit 2
 * - Unused
 * - Digit 3
 * - Unused
 */

#include "simba.h"

#if CONFIG_LED_7SEG_HT16K33 == 1

#include "led_7seg_ht16k33_numbertable.i"

#define HT16K33_CMD_SIZE              0x1
#define HT16K33_CMD_OSCILLATOR_OFF   0x20
#define HT16K33_CMD_OSCILLATOR_ON    0x21
#define HT16K33_CMD_DISPLAY_OFF      0x80
#define HT16K33_CMD_DISPLAY_ON       0x81
#define HT16K33_DISPLAY_BLINK_OFF     0x0
#define HT16K33_DISPLAY_BLINK_2HZ     0x2
#define HT16K33_CMD_BRIGHTNESS       0xe0
#define SEVEN_SEG_DIGITS              0x4
#define SEVEN_SEG_POSITIONS           0x5
#define SEVEN_SEG_DOT                0x80
#define SEVEN_SEG_COLON               0x2
#define SEVEN_SEG_COLON_POS           0x2

/**
 * Send a one byte command to the display controller.
 */
static int i2c_sendcmd(struct led_7seg_ht16k33_driver_t *self_p, uint8_t cmd)
{
    int8_t buf[HT16K33_CMD_SIZE];
    int r;

    buf[0] = cmd;
    r = i2c_soft_write(self_p->i2c_p, self_p->i2c_addr, buf, HT16K33_CMD_SIZE);

    return (r);
}

int led_7seg_ht16k33_module_init()
{

    i2c_soft_module_init();

    return (0);
}


int led_7seg_ht16k33_init(struct led_7seg_ht16k33_driver_t *self_p,
                          struct i2c_soft_driver_t *i2c_p,
                          int i2c_addr)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(i2c_p != NULL, EINVAL);

    self_p->i2c_p = i2c_p;
    self_p->i2c_addr = i2c_addr;
    led_7seg_ht16k33_clear(self_p);

    return (0);
}

int led_7seg_ht16k33_start(struct led_7seg_ht16k33_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    i2c_soft_start(self_p->i2c_p);
    if (i2c_soft_scan(self_p->i2c_p, self_p->i2c_addr) != 1) {
        return (-ENOENT);
    }

    i2c_sendcmd(self_p, HT16K33_CMD_OSCILLATOR_ON);
    i2c_sendcmd(self_p, HT16K33_CMD_DISPLAY_ON | HT16K33_DISPLAY_BLINK_OFF);
    led_7seg_ht16k33_brightness(self_p, LED_7SEG_HT16K33_BRIGHTNESS_MAX);

    return (0);
}

int led_7seg_ht16k33_clear(struct led_7seg_ht16k33_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    memset(&self_p->buf, 0, sizeof(self_p->buf));

    return (0);
}

int led_7seg_ht16k33_set_num(struct led_7seg_ht16k33_driver_t *self_p,
                             int num, int base)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(base > 0, EINVAL);
    ASSERTN(base <= 16, EINVAL);

    for (int i = 0, j = 4; i < 4; i++, j--) {
        if (i == SEVEN_SEG_COLON_POS) {
            j--;
	}
        // Avoid leading 0s, but allow a single one.
        if (num || i == 0) {
            int dot = self_p->buf[j] & SEVEN_SEG_DOT;
            self_p->buf[j] = numbertable[num % base] | dot;
        } else {
            self_p->buf[j] = 0;
        }
        num = num / base;
    }
    if (num > 0) {
        // We could not write complete number
        return (-ERANGE);
    }

    return (0);
}

int led_7seg_ht16k33_show_colon(struct led_7seg_ht16k33_driver_t *self_p,
                                int show_colon)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->buf[SEVEN_SEG_COLON_POS] = show_colon ? SEVEN_SEG_COLON : 0;

    return (0);
}

int led_7seg_ht16k33_show_dot(struct led_7seg_ht16k33_driver_t *self_p,
                              int position, int show_colon)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(position >= 0, EINVAL);
    ASSERTN(position <= 3, EINVAL);

    if (position >= SEVEN_SEG_COLON_POS)
        position++;

    if (show_colon) {
        self_p->buf[position] |= SEVEN_SEG_DOT;
    } else {
        self_p->buf[position] &= ~SEVEN_SEG_DOT;
    }

    return (0);
}


int led_7seg_ht16k33_brightness(struct led_7seg_ht16k33_driver_t *self_p,
                                int brightness)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(brightness >= LED_7SEG_HT16K33_BRIGHTNESS_MIN, EINVAL);
    ASSERTN(brightness <= LED_7SEG_HT16K33_BRIGHTNESS_MAX, EINVAL);

    int r;

    r = i2c_sendcmd(self_p, HT16K33_CMD_BRIGHTNESS | brightness);

    if (r == 1)
        return (0);
    if (r < 0)
        return (r);
    return (-EIO);
}

int led_7seg_ht16k33_display(struct led_7seg_ht16k33_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    uint8_t sendbuf[HT16K33_CMD_SIZE + 2 * SEVEN_SEG_POSITIONS];
    int i, r;

    // sendbuf[0] will be 0 which is our write command and start address.
    memset(sendbuf, 0, sizeof(sendbuf));
    for (i = 0; i < sizeof(self_p->buf); i++)
        sendbuf[(i * 2) + 1] = self_p->buf[i];

    r = i2c_soft_write(self_p->i2c_p, self_p->i2c_addr, sendbuf,
                       sizeof(sendbuf));

    if (r == sizeof(sendbuf)) {
        return (0);
    }
    if (r < 0) {
        return (r);
    }
    return (-EIO);
}

#endif
