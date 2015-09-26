/**
 * @file uart_soft.c
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

#include "simba.h"

/* Convert baud rate to microseconds. */
#define BAUDRATE2US(baudrate) (1000000L / baudrate)

/**
 * The interrupt service routine is called on falling edges. Read a
 * byte and write it on the receive channel.
 */
static void rx_isr(void *arg_p)
{
    int i;
    uint8_t data = 0, sample;
    struct uart_soft_driver_t *drv_p = arg_p;

    /* Wait half the sample time so following samples are taken in the
       middle of the sample period.*/
    time_sleep(drv_p->sample_time / 3);

    /* Get 8 bits. */
    for (i = 0; i < 8; i++) {
        time_sleep(drv_p->sample_time);

        /* Sample the pin. */
        data >>= 1;
        sample = pin_read(&drv_p->rx_pin);
        data |= (0x80 * sample);
    }

    /* Write data to input channel. */
    queue_write_irq(&drv_p->chin, &data, sizeof(data));

    /* During the execution of this routinue multiple falling edges
       are detected by the hardware. The interrupt flag will be set
       and should be cleared to inhibit that the routine is executed
       twise per 8 bits.*/
    exti_clear(&drv_p->rx_exti);
}

static ssize_t uart_soft_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    int i, j;
    uint8_t data;
    struct uart_soft_driver_t *drv_p;
    const uint8_t *tx_p = txbuf_p;

    drv_p = container_of(arg_p, struct uart_soft_driver_t, chout);

    for (i = 0; i < size; i++) {
        sys_lock();
        pin_write(&drv_p->tx_pin, 0);

        /* Put 8 bits on the transmission wire. */
        data = tx_p[i];

        for (j = 0; j < 8; j++) {
            time_sleep(drv_p->sample_time);
            pin_write(&drv_p->tx_pin, data & 1);
            data >>= 1;
        }

        time_sleep(drv_p->sample_time);
        pin_write(&drv_p->tx_pin, 1);
        time_sleep(drv_p->sample_time);
        sys_unlock();
    }

    return (size);
}

int uart_soft_init(struct uart_soft_driver_t *drv_p,
                   struct pin_device_t *tx_dev_p,
                   struct pin_device_t *rx_dev_p,
                   struct exti_device_t *rx_exti_dev_p,
                   int baudrate,
                   void *rxbuf_p,
                   size_t size)
{
    drv_p->sample_time = BAUDRATE2US(baudrate);

    chan_init(&drv_p->chout,
              NULL,
              (ssize_t (*)(chan_t *, const void *, size_t))uart_soft_write_cb,
              NULL);

    pin_init(&drv_p->tx_pin, tx_dev_p, PIN_OUTPUT);
    pin_init(&drv_p->rx_pin, rx_dev_p, PIN_INPUT);

    /* Keep TX line high when no transmission is ongoing. */
    pin_write(&drv_p->tx_pin, 1);

    exti_init(&drv_p->rx_exti,
              rx_exti_dev_p,
              EXTI_TRIGGER_FALLING_EDGE,
              rx_isr,
              drv_p);

    exti_start(&drv_p->rx_exti);

    return (queue_init(&drv_p->chin, rxbuf_p, size));
}
