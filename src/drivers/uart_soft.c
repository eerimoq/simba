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

#if CONFIG_UART_SOFT == 1

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
    struct uart_soft_driver_t *self_p = arg_p;

    /* Wait half the sample time so following samples are taken in the
       middle of the sample period.*/
    time_busy_wait_us(self_p->sample_time / 3);

    /* Get 8 bits. */
    for (i = 0; i < 8; i++) {
        time_busy_wait_us(self_p->sample_time);

        /* Sample the pin. */
        data >>= 1;
        sample = pin_read(&self_p->rx_pin);
        data |= (0x80 * sample);
    }

    /* Write data to input channel. */
    queue_write_isr(&self_p->chin, &data, sizeof(data));

    /* During the execution of this routinue multiple falling edges
       are detected by the hardware. The interrupt flag will be set
       and should be cleared to inhibit that the routine is executed
       twise per 8 bits.*/
    exti_clear(&self_p->rx_exti);
}

static ssize_t uart_soft_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    int i, j;
    uint8_t data;
    struct uart_soft_driver_t *self_p;
    const uint8_t *tx_p = txbuf_p;

    self_p = container_of(arg_p, struct uart_soft_driver_t, chout);

    for (i = 0; i < size; i++) {
        sys_lock();
        pin_write(&self_p->tx_pin, 0);

        /* Put 8 bits on the transmission wire. */
        data = tx_p[i];

        for (j = 0; j < 8; j++) {
            time_busy_wait_us(self_p->sample_time);
            pin_write(&self_p->tx_pin, data & 1);
            data >>= 1;
        }

        time_busy_wait_us(self_p->sample_time);
        pin_write(&self_p->tx_pin, 1);
        time_busy_wait_us(self_p->sample_time);
        sys_unlock();
    }

    return (size);
}

int uart_soft_init(struct uart_soft_driver_t *self_p,
                   struct pin_device_t *tx_dev_p,
                   struct pin_device_t *rx_dev_p,
                   struct exti_device_t *rx_exti_dev_p,
                   int baudrate,
                   void *rxbuf_p,
                   size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(tx_dev_p != NULL, EINVAL);
    ASSERTN(rx_dev_p != NULL, EINVAL);
    ASSERTN(rx_exti_dev_p != NULL, EINVAL);
    ASSERTN(rxbuf_p != NULL, EINVAL);

    self_p->sample_time = BAUDRATE2US(baudrate);

    chan_init(&self_p->chout,
              chan_read_null,
              (ssize_t (*)(void *, const void *, size_t))uart_soft_write_cb,
              chan_size_null);

    pin_init(&self_p->tx_pin, tx_dev_p, PIN_OUTPUT);
    pin_init(&self_p->rx_pin, rx_dev_p, PIN_INPUT);

    /* Keep TX line high when no transmission is ongoing. */
    pin_write(&self_p->tx_pin, 1);

    exti_init(&self_p->rx_exti,
              rx_exti_dev_p,
              EXTI_TRIGGER_FALLING_EDGE,
              rx_isr,
              self_p);

    exti_start(&self_p->rx_exti);

    return (queue_init(&self_p->chin, rxbuf_p, size));
}

#endif
