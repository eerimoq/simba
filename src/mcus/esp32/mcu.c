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

#undef BIT
#include "soc/dport_reg.h"

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    /* GPIO 0-9. */
    { .id =  0, .iomux = 16 },
    { .id =  1, .iomux = 33 },
    { .id =  2, .iomux = 15 },
    { .id =  3, .iomux = 32 },
    { .id =  4, .iomux = 17 },
    { .id =  5, .iomux = 26 },
    { .id =  6, .iomux = 23 },
    { .id =  7, .iomux = 24 },
    { .id =  8, .iomux = 25 },
    { .id =  9, .iomux = 20 },

    /* GPIO 10-19. */
    { .id = 10, .iomux = 21 },
    { .id = 11, .iomux = 22 },
    { .id = 12, .iomux = 12 },
    { .id = 13, .iomux = 13 },
    { .id = 14, .iomux = 11 },
    { .id = 15, .iomux = 14 },
    { .id = 16, .iomux = 18 },
    { .id = 17, .iomux = 19 },
    { .id = 18, .iomux = 27 },
    { .id = 19, .iomux = 28 },

    /* GPIO 20-29. */
    { .id = 20, .iomux = 29 },
    { .id = 21, .iomux = 30 },
    { .id = 22, .iomux = 31 },
    { .id = 23, .iomux = 34 },
    { .id = 24, .iomux = 35 },
    { .id = 25, .iomux =  8 },
    { .id = 26, .iomux =  9 },
    { .id = 27, .iomux = 10 },
    /* { .id = 28, .iomux = -1 }, */
    /* { .id = 29, .iomux = -1 }, */

    /* GPIO 30-39. */
    /* { .id = 30, .iomux = -1 }, */
    /* { .id = 31, .iomux = -1 }, */
    { .id = 32, .iomux =  6 },
    { .id = 33, .iomux =  7 },
    { .id = 34, .iomux =  4 },
    { .id = 35, .iomux =  5 },
    { .id = 36, .iomux =  0 },
    { .id = 37, .iomux =  1 },
    { .id = 38, .iomux =  2 },
    { .id = 39, .iomux =  3 }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART0,
        .tx_signal = ESP32_PERIPHERAL_SIGNAL_U0TXD_OUT,
        .rx_signal = ESP32_PERIPHERAL_SIGNAL_U0RXD_IN,
        .tx_pin_p = &pin_gpio01_dev,
        .rx_pin_p = &pin_gpio03_dev,
        .interrupt = {
            .source = ESP32_INTR_SOURCE_UART0,
            .cpu = ESP32_CPU_INTR_UART_NUM
        }
    },
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART1,
        .tx_signal = ESP32_PERIPHERAL_SIGNAL_U1TXD_OUT,
        .rx_signal = ESP32_PERIPHERAL_SIGNAL_U1RXD_IN,
        .tx_pin_p = &pin_gpio19_dev,
        .rx_pin_p = &pin_gpio23_dev,
        .interrupt = {
            .source = ESP32_INTR_SOURCE_UART1,
            .cpu = ESP32_CPU_INTR_UART_NUM
        }
    },
    {
        .drv_p = NULL,
        .regs_p = ESP32_UART2,
        .tx_signal = ESP32_PERIPHERAL_SIGNAL_U2TXD_OUT,
        .rx_signal = ESP32_PERIPHERAL_SIGNAL_U2RXD_IN,
        .tx_pin_p = &pin_gpio18_dev,
        .rx_pin_p = &pin_gpio05_dev,
        .interrupt = {
            .source = ESP32_INTR_SOURCE_UART2,
            .cpu = ESP32_CPU_INTR_UART_NUM
        }
    }
};

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = ESP32_SPI1,
        .dport_mask = DPORT_SPI_CLK_EN,
        .gpio = {
            .miso_func_in_sel = ESP32_PERIPHERAL_SIGNAL_SPIQ_IN,
            .mosi_func_in_sel = ESP32_PERIPHERAL_SIGNAL_SPID_IN,
            .sck_func_in_sel = ESP32_PERIPHERAL_SIGNAL_SPICLK_IN
        },
        .ss_p = NULL,
        .mosi_p = &pin_device[8],
        .miso_p = &pin_device[7],
        .sck_p = &pin_device[6],
        .interrupt = {
            .source = ESP32_INTR_SOURCE_SPI1,
            .cpu = ESP32_CPU_INTR_SPI_NUM
        },
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    },
    {
        .drv_p = NULL,
        .regs_p = ESP32_SPI2,
        .dport_mask = DPORT_SPI_CLK_EN_1,
        .gpio = {
            .miso_func_in_sel = ESP32_PERIPHERAL_SIGNAL_HSPIQ_IN,
            .mosi_func_in_sel = ESP32_PERIPHERAL_SIGNAL_HSPID_IN,
            .sck_func_in_sel = ESP32_PERIPHERAL_SIGNAL_HSPICLK_IN
        },
        .mosi_p = &pin_device[13],
        .miso_p = &pin_device[12],
        .sck_p = &pin_device[14],
        .interrupt = {
            .source = ESP32_INTR_SOURCE_SPI2,
            .cpu = ESP32_CPU_INTR_SPI_NUM
        },
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    },
    {
        .drv_p = NULL,
        .regs_p = ESP32_SPI3,
        .dport_mask = DPORT_SPI_CLK_EN_2,
        .gpio = {
            .miso_func_in_sel = ESP32_PERIPHERAL_SIGNAL_VSPIQ_IN,
            .mosi_func_in_sel = ESP32_PERIPHERAL_SIGNAL_VSPID_IN,
            .sck_func_in_sel = ESP32_PERIPHERAL_SIGNAL_VSPICLK_IN
        },
        .ss_p = NULL,
        .mosi_p = &pin_device[23],
        .miso_p = &pin_device[19],
        .sck_p = &pin_device[18],
        .interrupt = {
            .source = ESP32_INTR_SOURCE_SPI3,
            .cpu = ESP32_CPU_INTR_SPI_NUM
        },
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    }
};

/* struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = { */
/*     { */
/*         .drv_p = NULL, */
/*         .regs_p = ESP32_I2C0 */
/*     }, */
/*     { */
/*         .drv_p = NULL, */
/*         .regs_p = ESP32_I2C1 */
/*     } */
/* }; */

struct flash_device_t flash_device[FLASH_DEVICE_MAX] = {
    {
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL
            }
        }
    }
};

struct can_device_t can_device[CAN_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = ESP32_CAN,
        .tx_pin_p = &pin_gpio16_dev,
        .rx_pin_p = &pin_gpio17_dev
    }
};

struct adc_device_t adc_device[ADC_DEVICE_MAX];

struct dac_device_t dac_device[DAC_DEVICE_MAX];

/* Use software i2c. */
struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = {
    {
        .scl_p = &pin_device[22],
        .sda_p = &pin_device[21],
        .max_clock_stretching_us = 100000,
        .clock_stretching_sleep_us = 1000
    }
};
