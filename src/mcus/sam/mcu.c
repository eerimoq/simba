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

struct pin_device_t pin_device[PIN_DEVICE_MAX] = {
    /* Conroller A, 0-29. */
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P0 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P1 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P2 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P3 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P4 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P5 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P6 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P7 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P8 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P9 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P10 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P11 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P12 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P13 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P14 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P15 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P16 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P17 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P18 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P19 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P20 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P21 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P22 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P23 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P24 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P25 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P26 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P27 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P28 },
    { .pio_p = SAM_PIOA, .mask = SAM_PIO_P29 },

    /* Conroller B, 30-61. */
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P0 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P1 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P2 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P3 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P4 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P5 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P6 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P7 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P8 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P9 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P10 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P11 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P12 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P13 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P14 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P15 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P16 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P17 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P18 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P19 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P20 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P21 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P22 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P23 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P24 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P25 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P26 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P27 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P28 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P29 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P30 },
    { .pio_p = SAM_PIOB, .mask = SAM_PIO_P31 },

    /* Conroller C, 62-92. */
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P0 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P1 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P2 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P3 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P4 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P5 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P6 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P7 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P8 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P9 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P10 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P11 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P12 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P13 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P14 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P15 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P16 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P17 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P18 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P19 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P20 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P21 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P22 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P23 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P24 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P25 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P26 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P27 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P28 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P29 },
    { .pio_p = SAM_PIOC, .mask = SAM_PIO_P30 },

    /* Conroller D, 93-102. */
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P0 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P1 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P2 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P3 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P4 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P5 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P6 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P7 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P8 },
    { .pio_p = SAM_PIOD, .mask = SAM_PIO_P9 }
};

struct spi_device_t spi_device[SPI_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = SAM_SPI0,
        .mosi_p = &pin_device[26],
        .miso_p = &pin_device[25],
        .sck_p = &pin_device[27],
        .id = PERIPHERAL_ID_SPI0,
        .sem = {
            .count = 0,
            .count_max = 1,
            .waiters = {
                .head_p = NULL }
        }
    }
};

struct uart_device_t uart_device[UART_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = SAM_UART,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P8
            },
            .tx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P9
            }
        },
        .id = PERIPHERAL_ID_UART
    },

    {
        .drv_p = NULL,
        .regs_p = (struct sam_uart_t *)SAM_USART0,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P10
            },
            .tx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P11
            }
        },
        .id = PERIPHERAL_ID_USART0
    },

    {
        .drv_p = NULL,
        .regs_p = (struct sam_uart_t *)SAM_USART1,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P12
            },
            .tx = {
                .regs_p = SAM_PIOA,
                .mask = SAM_PIO_P13
            }
        },
        .id = PERIPHERAL_ID_USART1
    },

    {
        .drv_p = NULL,
        .regs_p = (struct sam_uart_t *)SAM_USART2,
        .pio = {
            .rx = {
                .regs_p = SAM_PIOB,
                .mask = SAM_PIO_P21
            },
            .tx = {
                .regs_p = SAM_PIOB,
                .mask = SAM_PIO_P22
            }
        },
        .id = PERIPHERAL_ID_USART2
    }
};

struct exti_device_t exti_device[EXTI_DEVICE_MAX] = {
    /* Conroller A, 0-29. */
    { .drv_p = NULL, .pin_dev_p = &pin_device[0] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[1] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[2] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[3] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[4] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[5] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[6] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[7] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[8] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[9] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[10] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[11] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[12] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[13] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[14] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[15] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[16] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[17] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[18] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[19] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[20] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[21] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[22] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[23] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[24] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[25] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[26] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[27] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[28] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[29] },

    /* Conroller B, 30-61. */
    { .drv_p = NULL, .pin_dev_p = &pin_device[30] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[31] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[32] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[33] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[34] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[35] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[36] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[37] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[38] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[39] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[40] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[41] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[42] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[43] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[44] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[45] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[46] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[47] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[48] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[49] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[50] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[51] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[52] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[53] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[54] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[55] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[56] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[57] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[58] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[59] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[60] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[61] },

    /* Conroller C, 62-92. */
    { .drv_p = NULL, .pin_dev_p = &pin_device[62] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[63] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[64] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[65] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[66] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[67] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[68] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[69] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[70] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[71] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[72] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[73] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[74] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[75] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[76] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[77] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[78] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[79] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[80] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[81] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[82] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[83] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[84] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[85] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[86] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[87] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[88] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[89] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[90] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[91] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[92] },

    /* Conroller D, 93-102. */
    { .drv_p = NULL, .pin_dev_p = &pin_device[93] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[94] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[95] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[96] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[97] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[98] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[99] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[100] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[101] },
    { .drv_p = NULL, .pin_dev_p = &pin_device[102] },
};

struct flash_device_t flash_device[FLASH_DEVICE_MAX] = {
    {
        .banks = {
            {
                .index = 0,
                .regs_p = SAM_EEFC0,
                .begin = FLASH0_BEGIN,
                .end = FLASH0_END,
                .page_size = FLASH0_PAGE_SIZE
            },
            {
                .index = 1,
                .regs_p = SAM_EEFC1,
                .begin = FLASH1_BEGIN,
                .end = FLASH1_END,
                .page_size = FLASH1_PAGE_SIZE
            }
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

struct can_device_t can_device[CAN_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = SAM_CAN0,
        .id = PERIPHERAL_ID_CAN0,
        .rx = {
            .pio_p = SAM_PIOA,
            .mask = SAM_PIO_P1
        },
        .tx = {
            .pio_p = SAM_PIOA,
            .mask = SAM_PIO_P0
        }
    },

    {
        .drv_p = NULL,
        .regs_p = SAM_CAN1,
        .id = PERIPHERAL_ID_CAN1,
        .rx = {
            .pio_p = SAM_PIOB,
            .mask = SAM_PIO_P15
        },
        .tx = {
            .pio_p = SAM_PIOB,
            .mask = SAM_PIO_P14
        }
    }
};

struct adc_device_t adc_device[ADC_DEVICE_MAX] = {
    {
        .regs_p = SAM_ADC,
        .id = PERIPHERAL_ID_ADC,
        .jobs = {
            .head_p = NULL,
            .tail_p = NULL
        },
        .tc = {
            .regs_p = SAM_TC0,
            .channel = 2,
            .id = PERIPHERAL_ID_TC2
        }
    }
};

struct dac_device_t dac_device[DAC_DEVICE_MAX] = {
    {
        .regs_p = SAM_DACC,
        .id = PERIPHERAL_ID_DACC,
        .jobs = {
            .head_p = NULL,
            .tail_p = NULL
        },
        .tc = {
            .regs_p = SAM_TC0,
            .channel = 1,
            .id = PERIPHERAL_ID_TC1
        }
    }
};

struct usb_device_t usb_device[USB_DEVICE_MAX] = {
    {
        .drv_p = NULL,
        .regs_p = SAM_UOTGHS,
        .id = PERIPHERAL_ID_UOTGHS
    }
};

/* Use software i2c. */
struct i2c_device_t i2c_device[I2C_DEVICE_MAX] = {
    {
        .id = PERIPHERAL_ID_TWI0,
        .regs_p = SAM_TWI0,
        .scl = {
            .pio_p = SAM_PIOA,
            .mask = SAM_PIO_P18
        },
        .sda = {
            .pio_p = SAM_PIOA,
            .mask = SAM_PIO_P17
        },
        .drv_p = NULL
    },
    {
        .id = PERIPHERAL_ID_TWI1,
        .regs_p = SAM_TWI1,
        .scl = {
            .pio_p = SAM_PIOB,
            .mask = SAM_PIO_P13
        },
        .sda = {
            .pio_p = SAM_PIOB,
            .mask = SAM_PIO_P12
        },
        .drv_p = NULL
    }
};
