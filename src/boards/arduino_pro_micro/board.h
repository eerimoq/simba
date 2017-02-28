/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#define pin_d2_dev pin_device[11]
#define pin_d3_dev pin_device[10]
#define pin_d4_dev pin_device[14]
#define pin_d5_dev pin_device[8]
#define pin_d6_dev pin_device[17]
#define pin_d7_dev pin_device[18]
#define pin_d8_dev pin_device[4]
#define pin_d9_dev pin_device[5]
#define pin_d10_dev pin_device[6]
#define pin_d14_dev pin_device[3]
#define pin_d15_dev pin_device[1]
#define pin_d16_dev pin_device[2]

#define pin_a0_dev pin_device[24]
#define pin_a1_dev pin_device[23]
#define pin_a2_dev pin_device[22]
#define pin_a3_dev pin_device[21]

#define pin_led_dev pin_device[0]

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
