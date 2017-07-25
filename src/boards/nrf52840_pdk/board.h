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

#ifndef __BOARD_H__
#define __BOARD_H__

/* Digital pins. */
#define pin_p00_dev  pin_device[0]
#define pin_p01_dev  pin_device[1]
#define pin_p02_dev  pin_device[2]
#define pin_p03_dev  pin_device[3]
#define pin_p04_dev  pin_device[4]
#define pin_p05_dev  pin_device[5]
#define pin_p06_dev  pin_device[6]
#define pin_p07_dev  pin_device[7]
#define pin_p08_dev  pin_device[8]
#define pin_p09_dev  pin_device[9]
#define pin_p10_dev  pin_device[10]
#define pin_p11_dev  pin_device[11]
#define pin_p12_dev  pin_device[12]
#define pin_p13_dev  pin_device[13]
#define pin_p14_dev  pin_device[14]
#define pin_p15_dev  pin_device[15]
#define pin_p16_dev  pin_device[16]
#define pin_p17_dev  pin_device[17]
#define pin_p18_dev  pin_device[18]
#define pin_p19_dev  pin_device[19]
#define pin_p20_dev  pin_device[20]
#define pin_p21_dev  pin_device[21]
#define pin_p22_dev  pin_device[22]
#define pin_p23_dev  pin_device[23]
#define pin_p24_dev  pin_device[24]
#define pin_p25_dev  pin_device[25]
#define pin_p26_dev  pin_device[26]
#define pin_p27_dev  pin_device[27]
#define pin_p28_dev  pin_device[28]
#define pin_p29_dev  pin_device[29]
#define pin_p30_dev  pin_device[30]
#define pin_p31_dev  pin_device[31]

/* Analog pins. */
#define pin_ain0_dev  pin_p02_dev
#define pin_ain1_dev  pin_p03_dev
#define pin_ain2_dev  pin_p04_dev
#define pin_ain3_dev  pin_p05_dev
#define pin_ain4_dev  pin_p28_dev
#define pin_ain5_dev  pin_p29_dev
#define pin_ain6_dev  pin_p30_dev
#define pin_ain7_dev  pin_p31_dev

/* Button pins. */
#define pin_btn1_dev  pin_p11_dev
#define pin_btn2_dev  pin_p12_dev
#define pin_btn3_dev  pin_p24_dev
#define pin_btn4_dev  pin_p25_dev

/* LED pins. */
#define pin_led1_dev  pin_p13_dev
#define pin_led2_dev  pin_p14_dev
#define pin_led3_dev  pin_p15_dev
#define pin_led4_dev  pin_p16_dev

/* On board LED.*/
#define pin_led_dev  pin_led1_dev

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number or negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
