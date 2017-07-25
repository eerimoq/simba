/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

ISR(uart0)
{
    struct uart_device_t *dev_p = &uart_device[0];
    struct uart_driver_t *drv_p = dev_p->drv_p;

    if (drv_p == NULL) {
        return;
    }

    /* TX and/or RX complete. */
}

static int uart_port_module_init()
{
    return (0);
}

static int uart_port_start(struct uart_driver_t *self_p)
{
    const struct uart_device_t *dev_p;
    volatile struct nrf5_uart_t *regs_p;
    int port;
    
    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;

    /* Configure pin functions. */
    port = (dev_p->tx_pin_p->regs_p == NRF5_GPIO_P1);
    regs_p->PSEL.TXD = ((port << 5) | dev_p->tx_pin_p->pin);
    port = (dev_p->rx_pin_p->regs_p == NRF5_GPIO_P1);
    regs_p->PSEL.RXD = ((port << 5) | dev_p->rx_pin_p->pin);
    
    regs_p->BAUDRATE = 0x01d7e000;
    regs_p->ENABLE = 4;
    regs_p->TASKS.STARTTX = 1;
    regs_p->TASKS.STARTRX = 1;
    regs_p->EVENTS.TXDRDY = 0;
    regs_p->EVENTS.RXDRDY = 0;

    return (0);
}

static int uart_port_stop(struct uart_driver_t *self_p)
{
    self_p->dev_p->regs_p->TASKS.SUSPEND = 1;

    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *buf_p,
                                  size_t size)
{
    size_t i;
    struct uart_driver_t *self_p;
    volatile struct nrf5_uart_t *regs_p;
    const uint8_t *u8_buf_p;

    self_p = container_of(arg_p, struct uart_driver_t, base);
    regs_p = self_p->dev_p->regs_p;
    u8_buf_p = buf_p;

    mutex_lock(&self_p->mutex);

    for (i = 0; i < size; i++) {
        regs_p->TXD = u8_buf_p[i];

        while (regs_p->EVENTS.TXDRDY == 0);

        regs_p->EVENTS.TXDRDY = 0;
    }

    mutex_unlock(&self_p->mutex);

    return (size);
}

static ssize_t uart_port_write_cb_isr(void *arg_p,
                                      const void *txbuf_p,
                                      size_t size)
{
    return (-ENOSYS);
}

static int uart_port_device_start(struct uart_device_t *dev_p,
                                  long baudrate)
{
    return (-ENOSYS);
}

static int uart_port_device_stop(struct uart_device_t *dev_p)
{
    return (-ENOSYS);
}

static ssize_t uart_port_device_read(struct uart_device_t *dev_p,
                                     void *buf_p,
                                     size_t size)
{
    return (-ENOSYS);
}

static ssize_t uart_port_device_write(struct uart_device_t *dev_p,
                                      const void *buf_p,
                                      size_t size)
{
    return (-ENOSYS);
}
