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

#include "simba.h"

struct pin_device_t pin_device[PIN_DEVICE_MAX];
struct exti_device_t exti_device[EXTI_DEVICE_MAX];
struct uart_device_t uart_device[UART_DEVICE_MAX];
struct can_device_t can_device[CAN_DEVICE_MAX];
struct adc_device_t adc_device[ADC_DEVICE_MAX];
struct dac_device_t dac_device[DAC_DEVICE_MAX];

struct flash_device_t flash_device[FLASH_DEVICE_MAX] = {
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } },
    { .sem = { .count = 0, .count_max = 1, .waiters = { .head_p = NULL } } }
};

struct i2c_device_t i2c_device[I2C_DEVICE_MAX];

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    }
};

struct pwm_device_t pwm_device[PWM_DEVICE_MAX] = {
    { .pin_dev_p = &pin_device[0] },
    { .pin_dev_p = &pin_device[1] },
    { .pin_dev_p = &pin_device[2] },
    { .pin_dev_p = &pin_device[3] },
    { .pin_dev_p = &pin_device[4] },
    { .pin_dev_p = &pin_device[5] },
    { .pin_dev_p = &pin_device[6] },
    { .pin_dev_p = &pin_device[7] },
    { .pin_dev_p = &pin_device[8] },
    { .pin_dev_p = &pin_device[9] },
    { .pin_dev_p = &pin_device[10] },
    { .pin_dev_p = &pin_device[11] },
    { .pin_dev_p = &pin_device[12] },
    { .pin_dev_p = &pin_device[13] },
    { .pin_dev_p = &pin_device[14] },
    { .pin_dev_p = &pin_device[15] },
    { .pin_dev_p = &pin_device[16] },
    { .pin_dev_p = &pin_device[17] },
    { .pin_dev_p = &pin_device[18] },
    { .pin_dev_p = &pin_device[19] },
    { .pin_dev_p = &pin_device[20] },
    { .pin_dev_p = &pin_device[21] },
    { .pin_dev_p = &pin_device[22] },
    { .pin_dev_p = &pin_device[23] },
    { .pin_dev_p = &pin_device[24] },
    { .pin_dev_p = &pin_device[25] },
    { .pin_dev_p = &pin_device[26] },
    { .pin_dev_p = &pin_device[27] },
    { .pin_dev_p = &pin_device[28] },
    { .pin_dev_p = &pin_device[29] },
    { .pin_dev_p = &pin_device[30] },
    { .pin_dev_p = &pin_device[31] },
    { .pin_dev_p = &pin_device[32] },
    { .pin_dev_p = &pin_device[33] },
    { .pin_dev_p = &pin_device[34] },
    { .pin_dev_p = &pin_device[35] },
    { .pin_dev_p = &pin_device[36] },
    { .pin_dev_p = &pin_device[37] },
    { .pin_dev_p = &pin_device[38] },
    { .pin_dev_p = &pin_device[39] },
    { .pin_dev_p = &pin_device[40] },
    { .pin_dev_p = &pin_device[41] },
    { .pin_dev_p = &pin_device[42] },
    { .pin_dev_p = &pin_device[43] },
    { .pin_dev_p = &pin_device[44] },
    { .pin_dev_p = &pin_device[45] },
    { .pin_dev_p = &pin_device[46] },
    { .pin_dev_p = &pin_device[47] },
    { .pin_dev_p = &pin_device[48] },
    { .pin_dev_p = &pin_device[49] },
    { .pin_dev_p = &pin_device[50] },
    { .pin_dev_p = &pin_device[51] },
    { .pin_dev_p = &pin_device[52] },
    { .pin_dev_p = &pin_device[53] },
    { .pin_dev_p = &pin_device[54] },
    { .pin_dev_p = &pin_device[55] },
    { .pin_dev_p = &pin_device[56] },
    { .pin_dev_p = &pin_device[57] },
    { .pin_dev_p = &pin_device[58] },
    { .pin_dev_p = &pin_device[59] },
    { .pin_dev_p = &pin_device[60] },
    { .pin_dev_p = &pin_device[61] },
    { .pin_dev_p = &pin_device[62] },
    { .pin_dev_p = &pin_device[63] },
    { .pin_dev_p = &pin_device[64] },
    { .pin_dev_p = &pin_device[65] },
    { .pin_dev_p = &pin_device[66] },
    { .pin_dev_p = &pin_device[67] },
    { .pin_dev_p = &pin_device[68] },
    { .pin_dev_p = &pin_device[69] },
    { .pin_dev_p = &pin_device[70] },
    { .pin_dev_p = &pin_device[71] },
    { .pin_dev_p = &pin_device[72] },
    { .pin_dev_p = &pin_device[73] },
    { .pin_dev_p = &pin_device[74] },
    { .pin_dev_p = &pin_device[75] },
    { .pin_dev_p = &pin_device[76] },
    { .pin_dev_p = &pin_device[77] },
    { .pin_dev_p = &pin_device[78] },
    { .pin_dev_p = &pin_device[79] },
    { .pin_dev_p = &pin_device[80] },
    { .pin_dev_p = &pin_device[81] },
    { .pin_dev_p = &pin_device[82] },
    { .pin_dev_p = &pin_device[83] },
    { .pin_dev_p = &pin_device[84] },
    { .pin_dev_p = &pin_device[85] },
    { .pin_dev_p = &pin_device[86] },
    { .pin_dev_p = &pin_device[87] },
    { .pin_dev_p = &pin_device[88] },
    { .pin_dev_p = &pin_device[89] },
    { .pin_dev_p = &pin_device[90] },
    { .pin_dev_p = &pin_device[91] },
    { .pin_dev_p = &pin_device[92] },
    { .pin_dev_p = &pin_device[93] },
    { .pin_dev_p = &pin_device[94] },
    { .pin_dev_p = &pin_device[95] },
    { .pin_dev_p = &pin_device[96] },
    { .pin_dev_p = &pin_device[97] },
    { .pin_dev_p = &pin_device[98] },
    { .pin_dev_p = &pin_device[99] },
    { .pin_dev_p = &pin_device[100] },
    { .pin_dev_p = &pin_device[101] },
    { .pin_dev_p = &pin_device[102] },
    { .pin_dev_p = &pin_device[103] },
    { .pin_dev_p = &pin_device[104] },
    { .pin_dev_p = &pin_device[105] },
    { .pin_dev_p = &pin_device[106] },
    { .pin_dev_p = &pin_device[107] },
    { .pin_dev_p = &pin_device[108] },
    { .pin_dev_p = &pin_device[109] },
    { .pin_dev_p = &pin_device[110] },
    { .pin_dev_p = &pin_device[111] },
    { .pin_dev_p = &pin_device[112] },
    { .pin_dev_p = &pin_device[113] },
    { .pin_dev_p = &pin_device[114] },
    { .pin_dev_p = &pin_device[115] },
    { .pin_dev_p = &pin_device[116] },
    { .pin_dev_p = &pin_device[117] },
    { .pin_dev_p = &pin_device[118] },
    { .pin_dev_p = &pin_device[119] },
    { .pin_dev_p = &pin_device[120] },
    { .pin_dev_p = &pin_device[121] },
    { .pin_dev_p = &pin_device[122] },
    { .pin_dev_p = &pin_device[123] },
    { .pin_dev_p = &pin_device[124] },
    { .pin_dev_p = &pin_device[125] },
    { .pin_dev_p = &pin_device[126] },
    { .pin_dev_p = &pin_device[127] }
};
