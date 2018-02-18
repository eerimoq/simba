/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

static ssize_t uart_port_device_write(struct uart_device_t *dev_p,
                                      const void *buf_p,
                                      size_t size);

static int is_rx_fifo_empty(void)
{
    return ((VIRT_UART_PL011->FR & VIRT_UART_PL011_FR_RXFE));
}

static int is_tx_fifo_full(void)
{
    return (VIRT_UART_PL011->FR & VIRT_UART_PL011_FR_TXFF);
}

static int is_data_error(uint32_t data)
{
    return (data & (VIRT_UART_PL011_DR_OE
                    | VIRT_UART_PL011_DR_BE
                    | VIRT_UART_PL011_DR_PE
                    | VIRT_UART_PL011_DR_FE));
}

static void clear_errors()
{
    VIRT_UART_PL011->RSR_ECR = (VIRT_UART_PL011_RSR_ECR_OE
                                | VIRT_UART_PL011_RSR_ECR_BE
                                | VIRT_UART_PL011_RSR_ECR_PE
                                | VIRT_UART_PL011_RSR_ECR_FE);
}

ISR(pl011)
{
    struct uart_driver_t *drv_p;
    uint32_t mis;
    uint32_t data;
    uint8_t u8_data;

    drv_p = uart_device[0].drv_p;

    if (drv_p == NULL) {
        return;
    }

    mis = VIRT_UART_PL011->MIS;

    /* Transmisstion completion interrupt handling. */
    if (mis & VIRT_UART_PL011_MIS_TX) {
        VIRT_UART_PL011->ICR = VIRT_UART_PL011_ICR_TX;

        if (drv_p->tx.size > 0) {
            VIRT_UART_PL011->DR = *drv_p->tx.buf_p++;
            drv_p->tx.size--;
        } else if (drv_p->thrd_p != NULL) {
            thrd_resume_isr(drv_p->thrd_p, 0);
            drv_p->thrd_p = NULL;
        }
    }

    /* Reception interrupt handling. */
    if (mis & VIRT_UART_PL011_MIS_RX) {
        while (!is_rx_fifo_empty()) {
            data = VIRT_UART_PL011->DR;

            if (is_data_error(data)) {
                clear_errors();
            } else {
                u8_data = (uint8_t)data;
                queue_write_isr(&drv_p->base, &u8_data, sizeof(u8_data));
            }
        }

        VIRT_UART_PL011->ICR = VIRT_UART_PL011_ICR_RX;
    }
}

static int uart_port_module_init()
{
    return (0);
}

static int uart_port_start(struct uart_driver_t *self_p)
{
    self_p->dev_p->drv_p = self_p;

    VIRT_UART_PL011->LCR_H = (VIRT_UART_PL011_LCR_H_WLEN_8
                              | VIRT_UART_PL011_LCR_H_FEN);
    VIRT_UART_PL011->CR = (VIRT_UART_PL011_CR_RXE
                           | VIRT_UART_PL011_CR_TXE
                           | VIRT_UART_PL011_CR_UARTEN);
    VIRT_UART_PL011->IMSC = (VIRT_UART_PL011_IMSC_TX
                             | VIRT_UART_PL011_IMSC_RX);
    mcu_configure_interrupt(33, 0);

    return (0);
}

static int uart_port_stop(struct uart_driver_t *self_p)
{
    VIRT_UART_PL011->CR = 0;

    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *buf_p,
                                  size_t size)
{
    struct uart_driver_t *self_p;

    self_p = container_of(arg_p, struct uart_driver_t, base);

    mutex_lock(&self_p->mutex);

    /* Initiate transfer by writing the first byte. */
    self_p->tx.buf_p = buf_p;
    self_p->tx.size = size;
    self_p->thrd_p = thrd_self();

    sys_lock();

    /* Write the first byte. The rest are written from the interrupt
       routine. */
    VIRT_UART_PL011->DR = *self_p->tx.buf_p++;
    self_p->tx.size--;

    thrd_suspend_isr(NULL);

    sys_unlock();

    mutex_unlock(&self_p->mutex);

    return (size);
}

static ssize_t uart_port_write_cb_isr(void *arg_p,
                                      const void *buf_p,
                                      size_t size)
{
    return (uart_port_device_write(NULL, buf_p, size));
}

static int uart_port_device_start(const struct uart_device_t *dev_p,
                                  long baudrate)
{
    VIRT_UART_PL011->LCR_H = (VIRT_UART_PL011_LCR_H_WLEN_8
                              | VIRT_UART_PL011_LCR_H_FEN);
    VIRT_UART_PL011->CR = (VIRT_UART_PL011_CR_RXE
                           | VIRT_UART_PL011_CR_TXE
                           | VIRT_UART_PL011_CR_UARTEN);

    return (0);
}

static int uart_port_device_stop(const struct uart_device_t *dev_p)
{
    VIRT_UART_PL011->CR = 0;

    return (0);
}

static ssize_t uart_port_device_read(const struct uart_device_t *dev_p,
                                     void *buf_p,
                                     size_t size)
{
    size_t i;
    char *c_buf_p;
    uint32_t data;

    c_buf_p = buf_p;

    for (i = 0; i < size; i++) {
        while (is_rx_fifo_empty());

        data = VIRT_UART_PL011->DR;

        if (is_data_error(data)) {
            clear_errors();

            return (-EIO);
        }

        c_buf_p[i] = (char)data;
    }

    return (size);
}

static ssize_t uart_port_device_write(struct uart_device_t *dev_p,
                                      const void *buf_p,
                                      size_t size)
{
    size_t i;
    const char *c_buf_p;

    c_buf_p = buf_p;

    for (i = 0; i < size; i++) {
        while (is_tx_fifo_full());

        VIRT_UART_PL011->DR = c_buf_p[i];
    }

    return (size);
}
