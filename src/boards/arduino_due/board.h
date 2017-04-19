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

#define pin_d0_dev pin_device[SAM_PA+8]
#define pin_d1_dev pin_device[SAM_PA+9]
#define pin_d2_dev pin_device[SAM_PB+25]
#define pin_d3_dev pin_device[SAM_PC+28]
#define pin_d4_dev pin_device[SAM_PC+26]
#define pin_d5_dev pin_device[SAM_PC+25]
#define pin_d6_dev pin_device[SAM_PC+24]
#define pin_d7_dev pin_device[SAM_PC+23]
#define pin_d8_dev pin_device[SAM_PC+22]
#define pin_d9_dev pin_device[SAM_PC+21]
#define pin_d10_dev pin_device[SAM_PA+28]
#define pin_d11_dev pin_device[SAM_PD+7]
#define pin_d12_dev pin_device[SAM_PD+8]
#define pin_d13_dev pin_device[SAM_PB+27]

#define pin_d14_dev pin_device[SAM_PD+4]
#define pin_d15_dev pin_device[SAM_PD+5]
#define pin_d16_dev pin_device[SAM_PA+13]
#define pin_d17_dev pin_device[SAM_PA+12]
#define pin_d18_dev pin_device[SAM_PA+11]
#define pin_d19_dev pin_device[SAM_PA+10]
#define pin_d20_dev pin_device[SAM_PB+12]
#define pin_d21_dev pin_device[SAM_PB+13]
#define pin_d22_dev pin_device[SAM_PB+26]
#define pin_d23_dev pin_device[SAM_PA+14]
#define pin_d24_dev pin_device[SAM_PA+15]
#define pin_d25_dev pin_device[SAM_PD+0]
#define pin_d26_dev pin_device[SAM_PD+1]
#define pin_d27_dev pin_device[SAM_PD+2]
#define pin_d28_dev pin_device[SAM_PD+3]
#define pin_d29_dev pin_device[SAM_PD+6]
#define pin_d30_dev pin_device[SAM_PD+9]
#define pin_d31_dev pin_device[SAM_PA+7]
#define pin_d32_dev pin_device[SAM_PD+10]
#define pin_d33_dev pin_device[SAM_PC+1]
#define pin_d34_dev pin_device[SAM_PC+2]
#define pin_d35_dev pin_device[SAM_PC+3]
#define pin_d36_dev pin_device[SAM_PC+4]
#define pin_d37_dev pin_device[SAM_PC+5]
#define pin_d38_dev pin_device[SAM_PC+6]
#define pin_d39_dev pin_device[SAM_PC+7]
#define pin_d40_dev pin_device[SAM_PC+8]
#define pin_d41_dev pin_device[SAM_PC+9]
#define pin_d42_dev pin_device[SAM_PA+19]
#define pin_d43_dev pin_device[SAM_PA+20]
#define pin_d44_dev pin_device[SAM_PC+19]
#define pin_d45_dev pin_device[SAM_PC+18]
#define pin_d46_dev pin_device[SAM_PC+17]
#define pin_d47_dev pin_device[SAM_PC+16]
#define pin_d48_dev pin_device[SAM_PC+15]
#define pin_d49_dev pin_device[SAM_PC+14]
#define pin_d50_dev pin_device[SAM_PC+13]
#define pin_d51_dev pin_device[SAM_PC+12]
#define pin_d52_dev pin_device[SAM_PB+21]
#define pin_d53_dev pin_device[SAM_PB+14]

#define pin_a0_dev pin_device[SAM_PA+16]
#define pin_a1_dev pin_device[SAM_PA+24]
#define pin_a2_dev pin_device[SAM_PA+23]
#define pin_a3_dev pin_device[SAM_PA+22]
#define pin_a4_dev pin_device[SAM_PA+6]
#define pin_a5_dev pin_device[SAM_PA+4]
#define pin_a6_dev pin_device[SAM_PA+3]
#define pin_a7_dev pin_device[SAM_PA+2]
#define pin_a8_dev pin_device[SAM_PB+17]
#define pin_a9_dev pin_device[SAM_PB+18]
#define pin_a10_dev pin_device[SAM_PB+19]
#define pin_a11_dev pin_device[SAM_PB+20]

#define pin_led_dev pin_d13_dev

#define pin_dac0_dev pin_device[SAM_PB+15]
#define pin_dac1_dev pin_device[SAM_PB+16]

/* Interrupts. */
#define exti_d0_dev exti_device[SAM_PA+8]
#define exti_d1_dev exti_device[SAM_PA+9]
#define exti_d2_dev exti_device[SAM_PB+25]
#define exti_d3_dev exti_device[SAM_PC+28]
#define exti_d4_dev exti_device[SAM_PC+26]
#define exti_d5_dev exti_device[SAM_PC+25]
#define exti_d6_dev exti_device[SAM_PC+24]
#define exti_d7_dev exti_device[SAM_PC+23]
#define exti_d8_dev exti_device[SAM_PC+22]
#define exti_d9_dev exti_device[SAM_PC+21]
#define exti_d10_dev exti_device[SAM_PA+28]
#define exti_d11_dev exti_device[SAM_PD+7]
#define exti_d12_dev exti_device[SAM_PD+8]
#define exti_d13_dev exti_device[SAM_PB+27]

#define exti_d14_dev exti_device[SAM_PD+4]
#define exti_d15_dev exti_device[SAM_PD+5]
#define exti_d16_dev exti_device[SAM_PA+13]
#define exti_d17_dev exti_device[SAM_PA+12]
#define exti_d18_dev exti_device[SAM_PA+11]
#define exti_d19_dev exti_device[SAM_PA+10]
#define exti_d20_dev exti_device[SAM_PB+12]
#define exti_d21_dev exti_device[SAM_PB+13]
#define exti_d22_dev exti_device[SAM_PB+26]
#define exti_d23_dev exti_device[SAM_PA+14]
#define exti_d24_dev exti_device[SAM_PA+15]
#define exti_d25_dev exti_device[SAM_PD+0]
#define exti_d26_dev exti_device[SAM_PD+1]
#define exti_d27_dev exti_device[SAM_PD+2]
#define exti_d28_dev exti_device[SAM_PD+3]
#define exti_d29_dev exti_device[SAM_PD+6]
#define exti_d30_dev exti_device[SAM_PD+9]
#define exti_d31_dev exti_device[SAM_PA+7]
#define exti_d32_dev exti_device[SAM_PD+10]
#define exti_d33_dev exti_device[SAM_PC+1]
#define exti_d34_dev exti_device[SAM_PC+2]
#define exti_d35_dev exti_device[SAM_PC+3]
#define exti_d36_dev exti_device[SAM_PC+4]
#define exti_d37_dev exti_device[SAM_PC+5]
#define exti_d38_dev exti_device[SAM_PC+6]
#define exti_d39_dev exti_device[SAM_PC+7]
#define exti_d40_dev exti_device[SAM_PC+8]
#define exti_d41_dev exti_device[SAM_PC+9]
#define exti_d42_dev exti_device[SAM_PA+19]
#define exti_d43_dev exti_device[SAM_PA+20]
#define exti_d44_dev exti_device[SAM_PC+19]
#define exti_d45_dev exti_device[SAM_PC+18]
#define exti_d46_dev exti_device[SAM_PC+17]
#define exti_d47_dev exti_device[SAM_PC+16]
#define exti_d48_dev exti_device[SAM_PC+15]
#define exti_d49_dev exti_device[SAM_PC+14]
#define exti_d50_dev exti_device[SAM_PC+13]
#define exti_d51_dev exti_device[SAM_PC+12]
#define exti_d52_dev exti_device[SAM_PB+21]
#define exti_d53_dev exti_device[SAM_PB+14]

#define exti_a0_dev exti_device[SAM_PA+16]
#define exti_a1_dev exti_device[SAM_PA+24]
#define exti_a2_dev exti_device[SAM_PA+23]
#define exti_a3_dev exti_device[SAM_PA+22]
#define exti_a4_dev exti_device[SAM_PA+6]
#define exti_a5_dev exti_device[SAM_PA+4]
#define exti_a6_dev exti_device[SAM_PA+3]
#define exti_a7_dev exti_device[SAM_PA+2]
#define exti_a8_dev exti_device[SAM_PB+17]
#define exti_a9_dev exti_device[SAM_PB+18]
#define exti_a10_dev exti_device[SAM_PB+19]
#define exti_a11_dev exti_device[SAM_PB+20]

#define exti_led_dev exti_d13_dev

#define exti_dac0_dev exti_device[SAM_PB+15]
#define exti_dac1_dev exti_device[SAM_PB+16]

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
#define dac_0_dev dac_device[0]

#define flash_0_dev flash_device[0]

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number or negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
