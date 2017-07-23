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

#ifndef __DRIVERS_LED_7SEG_I2C_H__
#define __DRIVERS_LED_7SEG_I2C_H__

struct led_7seg_ht16k33_driver_t {
    struct i2c_soft_driver_t *i2c_p;
    int i2c_addr;
    uint8_t buf[5];
};

//! Minimum brightness.
#define LED_7SEG_HT16K33_BRIGHTNESS_MIN 0
//! Maximum brightness.
#define LED_7SEG_HT16K33_BRIGHTNESS_MAX 0xf
//! Default I2C address for HT16K33.
#define LED_7SEG_HT16K33_DEFAULT_I2C_ADDR 0x70

/**
 * Initialize the driver module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_module_init(void);

/**
 * Initialize driver object. The driver object will be used for a
 * single display.
 *
 * @param[out] self_p Driver object to be initialize.
 * @param[in] i2c_p The I2C driver pointer.
 * @param[in] i2c_addr The address of the HT16K33 controller.
 *                     Probably LED_7SEG_HT16K33_DEFAULT_I2C_ADDR.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_init(struct led_7seg_ht16k33_driver_t *self_p,
                          struct i2c_soft_driver_t *i2c_p,
                          int i2c_addr);
/**
 * Start driver.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_start(struct led_7seg_ht16k33_driver_t *self_p);

/**
 * Send content of display buffer to the display.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_display(struct led_7seg_ht16k33_driver_t *self_p);

/**
 * Clear content of display buffer.
 *
 * @param[in] self_p Driver object.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_clear(struct led_7seg_ht16k33_driver_t *self_p);

/**
 * Set display brightness.
 *
 * @param[in] self_p Driver object.
 * @param[in] brightness Brightness from LED_7SEG_HT16K33_BRIGHTNESS_MIN
 *                       to LED_7SEG_HT16K33_BRIGHTNESS_MAX.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_brightness(struct led_7seg_ht16k33_driver_t *self_p,
                                int brightness);

/**
 * Set a number in the display buffer.
 *
 * Number cannot be more than 4 digits AKA base^4 - 1.
 *
 * @param[in] self_p Driver object.
 * @param[in] num Number to set.
 * @param[in] base Base of \a num.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_set_num(struct led_7seg_ht16k33_driver_t *self_p,
                             int num,
                             int base);

/**
 * Set show/hide of colon in the display buffer.
 *
 * @param[in] self_p Driver object.
 * @param[in] show_colon If true light colon, otherwise turn off.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_show_colon(struct led_7seg_ht16k33_driver_t *self_p,
                                int show_colon);

/**
 * Set show/hide of dot in the display buffer.
 *
 * @param[in] self_p Driver object.
 * @param[in] position The position to light colon or not. Range: 0 to 3.
 * @param[in] show_dot If true light dot, otherwise turn off.
 *
 * @return zero(0) or negative error code.
 */
int led_7seg_ht16k33_show_dot(struct led_7seg_ht16k33_driver_t *self_p,
                              int position,
                              int show_colon);

#endif
