/**
 * @file board.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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
#define pin_d10_dev pin_device[SAM_PC+29]
#define pin_d11_dev pin_device[SAM_PD+7]
#define pin_d12_dev pin_device[SAM_PD+8]
#define pin_d13_dev pin_device[SAM_PB+27]

#define pin_d14_dev pin_device[SAM_PD+4]
#define pin_d15_dev pin_device[SAM_PD+5]
#define pin_d16_dev pin_device[SAM_PA+1]
#define pin_d17_dev pin_device[SAM_PA+1]
#define pin_d18_dev pin_device[SAM_PA+1]
#define pin_d19_dev pin_device[SAM_PA+1]
#define pin_d20_dev pin_device[SAM_PB+1]
#define pin_d21_dev pin_device[SAM_PB+1]
#define pin_d22_dev pin_device[SAM_PB+2]
#define pin_d23_dev pin_device[SAM_PA+1]
#define pin_d24_dev pin_device[SAM_PA+1]
#define pin_d25_dev pin_device[SAM_PD+0]
#define pin_d26_dev pin_device[SAM_PD+1]
#define pin_d27_dev pin_device[SAM_PD+2]
#define pin_d28_dev pin_device[SAM_PD+3]
#define pin_d29_dev pin_device[SAM_PD+6]
#define pin_d30_dev pin_device[SAM_PD+9]
#define pin_d31_dev pin_device[SAM_PA+7]
#define pin_d32_dev pin_device[SAM_PD+1]
#define pin_d33_dev pin_device[SAM_PC+1]
#define pin_d34_dev pin_device[SAM_PC+2]
#define pin_d35_dev pin_device[SAM_PC+3]
#define pin_d36_dev pin_device[SAM_PC+4]
#define pin_d37_dev pin_device[SAM_PC+5]
#define pin_d38_dev pin_device[SAM_PC+6]
#define pin_d39_dev pin_device[SAM_PC+7]
#define pin_d40_dev pin_device[SAM_PC+8]
#define pin_d41_dev pin_device[SAM_PC+9]
#define pin_d42_dev pin_device[SAM_PA+1]
#define pin_d43_dev pin_device[SAM_PA+2]
#define pin_d44_dev pin_device[SAM_PC+1]
#define pin_d45_dev pin_device[SAM_PC+1]
#define pin_d46_dev pin_device[SAM_PC+1]
#define pin_d47_dev pin_device[SAM_PC+1]
#define pin_d48_dev pin_device[SAM_PC+1]
#define pin_d49_dev pin_device[SAM_PC+1]
#define pin_d50_dev pin_device[SAM_PC+1]
#define pin_d51_dev pin_device[SAM_PC+1]
#define pin_d52_dev pin_device[SAM_PB+2]
#define pin_d53_dev pin_device[SAM_PB+1]

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

#endif
