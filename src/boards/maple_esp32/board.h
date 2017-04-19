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

#ifndef __BOARD_H__
#define __BOARD_H__

#define pin_gpio00_dev                          pin_device[0]
#define pin_gpio01_dev                          pin_device[1]
#define pin_gpio02_dev                          pin_device[2]
#define pin_gpio03_dev                          pin_device[3]
#define pin_gpio04_dev                          pin_device[4]
#define pin_gpio05_dev                          pin_device[5]
#define pin_gpio06_dev                          pin_device[6]
#define pin_gpio07_dev                          pin_device[7]
#define pin_gpio08_dev                          pin_device[8]
#define pin_gpio09_dev                          pin_device[9]
#define pin_gpio10_dev                          pin_device[10]
#define pin_gpio11_dev                          pin_device[11]
#define pin_gpio12_dev                          pin_device[12]
#define pin_gpio13_dev                          pin_device[13]
#define pin_gpio14_dev                          pin_device[14]
#define pin_gpio15_dev                          pin_device[15]
#define pin_gpio16_dev                          pin_device[16]
#define pin_gpio17_dev                          pin_device[17]
#define pin_gpio18_dev                          pin_device[18]
#define pin_gpio19_dev                          pin_device[19]
#define pin_gpio21_dev                          pin_device[21]
#define pin_gpio22_dev                          pin_device[22]
#define pin_gpio23_dev                          pin_device[23]
#define pin_gpio25_dev                          pin_device[25]
#define pin_gpio26_dev                          pin_device[26]
#define pin_gpio27_dev                          pin_device[27]
#define pin_gpio32_dev                          pin_device[28]
#define pin_gpio33_dev                          pin_device[29]
#define pin_gpio34_dev                          pin_device[30]
#define pin_gpio35_dev                          pin_device[31]
#define pin_gpio36_dev                          pin_device[32]
#define pin_gpio39_dev                          pin_device[35]

#define pin_led_dev                             pin_gpio16_dev

#define pin_dac1_dev                            pin_gpio25_dev
#define pin_dac2_dev                            pin_gpio26_dev

#define pin_a0_dev                              pin_gpio36_dev
#define pin_a3_dev                              pin_gpio39_dev
#define pin_a4_dev                              pin_gpio32_dev
#define pin_a5_dev                              pin_gpio33_dev
#define pin_a6_dev                              pin_gpio34_dev
#define pin_a7_dev                              pin_gpio35_dev

#define i2c_dev                                  i2c_device[0]

#define spi_h_dev                                spi_device[1]
#define spi_v_dev                                spi_device[2]

#define adc_0_dev                                adc_device[0]
#define adc_1_dev                                adc_device[1]

#define flash_0_dev                            flash_device[0]

#define dac_0_dev                                dac_device[0]

#define ADC_PINS_MAX                                        16

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number or negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
