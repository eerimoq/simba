/**
 * @file drivers/sh/uart_port.c
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

static void isr(int index)
{
    struct uart_driver_t *drv_p = uart_device[index].drv_p;
    (void)drv_p;
}

static int uart_port_start(struct uart_driver_t *drv)
{
    return (0);
}

static int uart_port_stop(struct uart_driver_t *drv)
{
    return (0);
}

static ssize_t uart_port_write_cb(void *arg,
                                  const void *txbuf,
                                  size_t size)
{
    return (size);
}

#define UART_ISR(vector, index)                 \
    ISR(vector) {                               \
        isr(index);                             \
    }                                           \

#if (UART_DEVICE_MAX >= 1)
UART_ISR(usart0, 0)
#endif

#if (UART_DEVICE_MAX >= 2)
UART_ISR(usart1, 1)
#endif

#if (UART_DEVICE_MAX >= 3)
UART_ISR(usart2, 2)
#endif

#if (UART_DEVICE_MAX >= 4)
UART_ISR(usart3, 3)
#endif
