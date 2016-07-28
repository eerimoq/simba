/**
 * @file boards/esp12e/board.h
 * @version 3.1.1
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

#define pin_d0_dev pin_device[0]
#define pin_d2_dev pin_device[2]
#define pin_d4_dev pin_device[4]
#define pin_d5_dev pin_device[5]
#define pin_d12_dev pin_device[12]
#define pin_d13_dev pin_device[13]
#define pin_d14_dev pin_device[14]
#define pin_d15_dev pin_device[15]

#define pin_led_dev pin_d2_dev

#define pin_a0_dev pin_device[0]

#define adc_0_dev adc_device[0]

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number of negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
