/**
 * @file uart_port.i
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

#include <avr/interrupt.h>

#if defined(__AVR_ATmega2560__)

static int uart_port_start(struct uart_driver_t *drv_p)
{
    uint16_t baudrate = (F_CPU / 16 / drv_p->baudrate - 1);

    UBRR0H = (baudrate >> 8);
    UBRR0L = baudrate;
    UCSR0B = (_BV(RXCIE0) | _BV(TXCIE0) | _BV(RXEN0) | _BV(TXEN0));
    UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01));

    drv_p->dev_p->drv_p = drv_p;

    return (0);
}

static int uart_port_stop(struct uart_driver_t *drv_p)
{
    UCSR0B = 0;
    UCSR0C = 0;

    drv_p->dev_p->drv_p = NULL;

    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    struct uart_driver_t *drv_p;

    /* Initiate transfer by writing the first byte. */
    drv_p = container_of(arg_p, struct uart_driver_t, chout);
    drv_p->txbuf_p = txbuf_p;
    drv_p->txsize = (size - 1);
    drv_p->thrd_p = thrd_self();
    UDR0 = *drv_p->txbuf_p++;

    thrd_suspend(NULL);

    return (size);
}

ISR(USART0_TX_vect)
{
    struct uart_driver_t *drv_p = uart_device[0].drv_p;

    /* Write next byte or resume suspended thread. */
    if (drv_p->txsize > 0) {
        UDR0 = *drv_p->txbuf_p++;
        drv_p->txsize--;
    } else {
        thrd_resume_irq(drv_p->thrd_p, 0);
    }
}

ISR(USART0_RX_vect)
{
    struct uart_driver_t *drv_p = uart_device[0].drv_p;
    char c;

    /* Write data to input queue. */
    c = UDR0;
    queue_write_irq(&drv_p->chin, &c, 1);
}

#else

static int uart_port_start(struct uart_driver_t *drv_p)
{
    uint16_t baudrate = (F_CPU / 16 / drv_p->baudrate - 1);

    UBRR0H = (baudrate >> 8);
    UBRR0L = baudrate;
    UCSR0B = (_BV(RXCIE0) | _BV(TXCIE0) | _BV(RXEN0) | _BV(TXEN0));
    UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01));

    drv_p->dev_p->drv_p = drv_p;

    return (0);
}

static int uart_port_stop(struct uart_driver_t *drv_p)
{
    UCSR0B = 0;
    UCSR0C = 0;

    drv_p->dev_p->drv_p = NULL;

    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    struct uart_driver_t *drv_p;

    /* Initiate transfer by writing the first byte. */
    drv_p = container_of(arg_p, struct uart_driver_t, chout);
    drv_p->txbuf_p = txbuf_p;
    drv_p->txsize = (size - 1);
    drv_p->thrd_p = thrd_self();
    UDR0 = *drv_p->txbuf_p++;

    thrd_suspend(NULL);

    return (size);
}

ISR(USART_TX_vect)
{
    struct uart_driver_t *drv_p = uart_device[0].drv_p;

    /* Write next byte or resume suspended thread. */
    if (drv_p->txsize > 0) {
        UDR0 = *drv_p->txbuf_p++;
        drv_p->txsize--;
    } else {
        thrd_resume_irq(drv_p->thrd_p, 0);
    }
}

ISR(USART_RX_vect)
{
    struct uart_driver_t *drv_p = uart_device[0].drv_p;
    char c;

    /* Write data to input queue. */
    c = UDR0;
    queue_write_irq(&drv_p->chin, &c, 1);
}

#endif
