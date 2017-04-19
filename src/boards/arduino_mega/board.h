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

#define pin_d0_dev pin_device[0]
#define pin_d1_dev pin_device[1]
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
#define pin_d14_dev pin_device[14]
#define pin_d15_dev pin_device[15]
#define pin_d16_dev pin_device[16]
#define pin_d17_dev pin_device[17]
#define pin_d18_dev pin_device[18]
#define pin_d19_dev pin_device[19]
#define pin_d20_dev pin_device[20]
#define pin_d21_dev pin_device[21]
#define pin_d22_dev pin_device[22]
#define pin_d23_dev pin_device[23]
#define pin_d24_dev pin_device[24]
#define pin_d25_dev pin_device[25]
#define pin_d26_dev pin_device[26]
#define pin_d27_dev pin_device[27]
#define pin_d28_dev pin_device[28]
#define pin_d29_dev pin_device[29]
#define pin_d30_dev pin_device[30]
#define pin_d31_dev pin_device[31]
#define pin_d32_dev pin_device[32]
#define pin_d33_dev pin_device[33]
#define pin_d34_dev pin_device[34]
#define pin_d35_dev pin_device[35]
#define pin_d36_dev pin_device[36]
#define pin_d37_dev pin_device[37]
#define pin_d38_dev pin_device[38]
#define pin_d39_dev pin_device[39]
#define pin_d40_dev pin_device[40]
#define pin_d41_dev pin_device[41]
#define pin_d42_dev pin_device[42]
#define pin_d43_dev pin_device[43]
#define pin_d44_dev pin_device[44]
#define pin_d45_dev pin_device[45]
#define pin_d46_dev pin_device[46]
#define pin_d47_dev pin_device[47]
#define pin_d48_dev pin_device[48]
#define pin_d49_dev pin_device[49]
#define pin_d50_dev pin_device[50]
#define pin_d51_dev pin_device[51]
#define pin_d52_dev pin_device[52]
#define pin_d53_dev pin_device[53]

#define pin_a0_dev pin_device[54]
#define pin_a1_dev pin_device[55]
#define pin_a2_dev pin_device[56]
#define pin_a3_dev pin_device[57]
#define pin_a4_dev pin_device[58]
#define pin_a5_dev pin_device[59]
#define pin_a6_dev pin_device[60]
#define pin_a7_dev pin_device[61]
#define pin_a8_dev pin_device[62]
#define pin_a9_dev pin_device[63]
#define pin_a10_dev pin_device[64]
#define pin_a11_dev pin_device[65]
#define pin_a12_dev pin_device[66]
#define pin_a13_dev pin_device[67]
#define pin_a14_dev pin_device[68]
#define pin_a15_dev pin_device[69]

#define pin_led_dev pin_d13_dev

#define exti_d2_dev exti_device[4]
#define exti_d3_dev exti_device[5]
#define exti_d18_dev exti_device[3]
#define exti_d19_dev exti_device[2]
#define exti_d20_dev exti_device[1]
#define exti_d21_dev exti_device[0]

/* PWM d4 and d13 cannot be used since the timer is used for the system tick. */
#define pwm_d2_dev pwm_device[0]
#define pwm_d3_dev pwm_device[1]
#define pwm_d5_dev pwm_device[2]
#define pwm_d6_dev pwm_device[3]
#define pwm_d7_dev pwm_device[4]
#define pwm_d8_dev pwm_device[5]
#define pwm_d9_dev pwm_device[6]
#define pwm_d10_dev pwm_device[7]
#define pwm_d11_dev pwm_device[8]
#define pwm_d12_dev pwm_device[9]

#define adc_0_dev adc_device[0]

#define i2c_0_dev i2c_device[0]

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number or negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
