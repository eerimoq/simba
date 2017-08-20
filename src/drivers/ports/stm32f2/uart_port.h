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

#ifndef __DRIVERS_UART_PORT_H__
#define __DRIVERS_UART_PORT_H__

/*
 * STM32F2xx product line supports 8/9 data bits, N/E/O parity, 1/2 stop bits
 * NOTE format field codification:
 *      bits 0-7 are 8-15 of CR1 (coded 8 bits to the right)
 *      bits 8-15 are 8-15 of CR2
 */
#define STM32_UART_DATA_BITS_8  0x00
#define STM32_UART_DATA_BITS_9  ((0x01 << STM32_USART_CR1_M_POS) >> 8)
#define STM32_UART_PARITY_N     0x00
#define STM32_UART_PARITY_E     ((0x02 << STM32_USART_CR1_PS_POS) >> 8)
#define STM32_UART_PARITY_O     ((0x03 << STM32_USART_CR1_PS_POS) >> 8)
#define STM32_UART_STOP_BITS_1  0x00
#define STM32_UART_STOP_BITS_2  (0x02 << STM32_USART_CR2_STOP_POS)
#define UART_PORT_FRAME_FORMAT_8N1 (STM32_UART_DATA_BITS_8 \
        | STM32_UART_PARITY_N | STM32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_8N2 (STM32_UART_DATA_BITS_8 \
        | STM32_UART_PARITY_N | STM32_UART_STOP_BITS_2)
#define UART_PORT_FRAME_FORMAT_8O1 (STM32_UART_DATA_BITS_8 \
        | STM32_UART_PARITY_O | STM32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_8O2 (STM32_UART_DATA_BITS_8 \
        | STM32_UART_PARITY_O | STM32_UART_STOP_BITS_2)
#define UART_PORT_FRAME_FORMAT_8E1 (STM32_UART_DATA_BITS_8 \
        | STM32_UART_PARITY_E | STM32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_8E2 (STM32_UART_DATA_BITS_8 \
        | STM32_UART_PARITY_E | STM32_UART_STOP_BITS_2)
#define UART_PORT_FRAME_FORMAT_9N1 (STM32_UART_DATA_BITS_9 \
        | STM32_UART_PARITY_N | STM32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_9N2 (STM32_UART_DATA_BITS_9 \
        | STM32_UART_PARITY_N | STM32_UART_STOP_BITS_2)
#define UART_PORT_FRAME_FORMAT_9O1 (STM32_UART_DATA_BITS_9 \
        | STM32_UART_PARITY_O | STM32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_9O2 (STM32_UART_DATA_BITS_9 \
        | STM32_UART_PARITY_O | STM32_UART_STOP_BITS_2)
#define UART_PORT_FRAME_FORMAT_9E1 (STM32_UART_DATA_BITS_9 \
        | STM32_UART_PARITY_E | STM32_UART_STOP_BITS_1)
#define UART_PORT_FRAME_FORMAT_9E2 (STM32_UART_DATA_BITS_9 \
        | STM32_UART_PARITY_E | STM32_UART_STOP_BITS_2)
#define UART_PORT_FRAME_FORMAT_DEFAULT UART_PORT_FRAME_FORMAT_8N1

struct uart_device_t {
    struct uart_driver_t *drv_p;
    volatile struct stm32_usart_t *regs_p;
    struct pin_device_t *tx_pin_dev_p;
    struct pin_device_t *rx_pin_dev_p;
};

struct uart_driver_t {
    struct queue_t base;
    struct uart_device_t *dev_p;
    struct mutex_t mutex;
    const uint8_t *txbuf_p;
    size_t txsize;
    struct thrd_t *thrd_p;
    long baudrate;
    int format;
};

#endif
