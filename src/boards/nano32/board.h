/**
 * @file boards/nano32/board.h
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#define pin_gpio00_dev pin_device[0]
#define pin_gpio01_dev pin_device[1]
#define pin_gpio02_dev pin_device[2]
#define pin_gpio03_dev pin_device[3]
#define pin_gpio04_dev pin_device[4]
#define pin_gpio05_dev pin_device[5]
#define pin_gpio06_dev pin_device[6]
#define pin_gpio07_dev pin_device[7]
#define pin_gpio08_dev pin_device[8]
#define pin_gpio09_dev pin_device[9]
#define pin_gpio10_dev pin_device[10]
#define pin_gpio11_dev pin_device[11]
#define pin_gpio12_dev pin_device[12]
#define pin_gpio13_dev pin_device[13]
#define pin_gpio14_dev pin_device[14]
#define pin_gpio15_dev pin_device[15]
#define pin_gpio16_dev pin_device[16]
#define pin_gpio17_dev pin_device[17]
#define pin_gpio18_dev pin_device[18]
#define pin_gpio19_dev pin_device[19]
#define pin_gpio21_dev pin_device[21]
#define pin_gpio22_dev pin_device[22]
#define pin_gpio23_dev pin_device[23]
#define pin_gpio25_dev pin_device[25]
#define pin_gpio26_dev pin_device[26]
#define pin_gpio27_dev pin_device[27]
#define pin_gpio32_dev pin_device[28]
#define pin_gpio33_dev pin_device[29]
#define pin_gpio34_dev pin_device[30]
#define pin_gpio35_dev pin_device[31]
#define pin_gpio36_dev pin_device[32]
#define pin_gpio39_dev pin_device[35]

#define pin_led_dev pin_gpio16_dev

#define pin_adc0_dev pin_gpio36_dev
#define pin_adc3_dev pin_gpio39_dev
#define pin_adc4_dev pin_gpio32_dev
#define pin_adc5_dev pin_gpio33_dev
#define pin_adc6_dev pin_gpio34_dev
#define pin_adc7_dev pin_gpio35_dev
#define pin_adc10_dev pin_gpio04_dev
#define pin_adc11_dev pin_gpio00_dev
#define pin_adc12_dev pin_gpio02_dev
#define pin_adc13_dev pin_gpio15_dev
#define pin_adc14_dev pin_gpio13_dev
#define pin_adc15_dev pin_gpio12_dev
#define pin_adc16_dev pin_gpio14_dev
#define pin_adc17_dev pin_gpio27_dev
#define pin_adc18_dev pin_gpio25_dev
#define pin_adc19_dev pin_gpio26_dev

#define pin_dac1_dev pin_gpio25_dev
#define pin_dac2_dev pin_gpio26_dev

#define i2c_dev i2c_device[0]

#define spi_h_dev spi_device[1]
#define spi_v_dev spi_device[2]

#define adc_1_dev adc_device[0]
#define adc_2_dev adc_device[1]

#define flash_0_dev flash_device[0]

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number of negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
