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

#if CONFIG_UART == 1

struct module_t {
    int8_t initialized;
};

#include "uart_port.h"
#include "uart_port.i"

static struct module_t module;

int uart_module_init(void)
{
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    return (uart_port_module_init());
}

int uart_init(struct uart_driver_t *self_p,
              struct uart_device_t *dev_p,
              long baudrate,
              void *rxbuf_p,
              size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);

    self_p->dev_p = dev_p;
    self_p->baudrate = baudrate;
    self_p->format = UART_FRAME_FORMAT_DEFAULT;

    mutex_init(&self_p->mutex);

    /* The base channel is used for both TX and RX. */
    queue_init(&self_p->base, rxbuf_p, size);
    chan_set_write_cb(&self_p->base.base, uart_port_write_cb);
    chan_set_write_isr_cb(&self_p->base.base, uart_port_write_cb_isr);

    return (0);
}

int uart_start(struct uart_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (uart_port_start(self_p));
}

int uart_stop(struct uart_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (uart_port_stop(self_p));
}

int uart_set_frame_format(struct uart_driver_t *self_p, int format)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->format = format;

    return (0);
}

int uart_device_start(struct uart_device_t *dev_p,
                      long baudrate)
{
    return (uart_port_device_start(dev_p, baudrate));
}

int uart_device_stop(struct uart_device_t *dev_p)
{
    return (uart_port_device_stop(dev_p));
}

ssize_t uart_device_read(struct uart_device_t *dev_p,
                         void *buf_p,
                         size_t size)
{
    return (uart_port_device_read(dev_p, buf_p, size));
}

ssize_t uart_device_write(struct uart_device_t *dev_p,
                          const void *buf_p,
                          size_t size)
{
    return (uart_port_device_write(dev_p, buf_p, size));
}

#endif
