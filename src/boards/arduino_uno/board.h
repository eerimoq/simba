/**
 * @file board.h
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

#define pin_d2_dev pin_device[2]
#define pin_d3_dev pin_device[3]
#define pin_d4_dev pin_device[4]
#define pin_d5_dev pin_device[5]
#define pin_d6_dev pin_device[6]
#define pin_d7_dev pin_device[7]
#define pin_d8_dev pin_device[8]
#define pin_d9_dev pin_device[9]
#define pin_d10_dev pin_device[10]
#define pin_d11_dev pin_device[11]
#define pin_d12_dev pin_device[12]
#define pin_d13_dev pin_device[13]

#define pin_a0_dev pin_device[14]
#define pin_a1_dev pin_device[15]
#define pin_a2_dev pin_device[16]
#define pin_a3_dev pin_device[17]
#define pin_a4_dev pin_device[18]
#define pin_a5_dev pin_device[19]

#define pin_led_dev pin_d13_dev

#define exti_d2_dev exti_device[0]
#define exti_d3_dev exti_device[1]

#define pwm_d3_dev pwm_device[3]
#define pwm_d9_dev pwm_device[0]
#define pwm_d10_dev pwm_device[1]
#define pwm_d11_dev pwm_device[2]

#define adc_0_dev adc_device[0]

#define i2c_0_dev i2c_device[0]

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number of negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
