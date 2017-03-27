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

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static struct fs_counter_t rx_channel_overflow;
static struct fs_counter_t rx_errors;

extern xSemaphoreHandle thrd_idle_sem;

/**
 * Fill the tx fifo with data from given uart driver.
 */
static void fill_tx_fifo(struct uart_driver_t *drv_p)
{
    while ((drv_p->txsize > 0)
           && ((drv_p->dev_p->regs_p->STATUS
                & ESP8266_UART_STATUS_TXFIFO_CNT_MASK)
               != ESP8266_UART_STATUS_TXFIFO_CNT_MASK)) {
        drv_p->dev_p->regs_p->FIFO = *drv_p->txbuf_p++;
        drv_p->txsize--;
    }
}

static inline void tx_isr(struct uart_driver_t *drv_p,
                          struct uart_device_t *dev_p)
{
    /* Clear the interrupt. */
    dev_p->regs_p->INT_CLR = ESP8266_UART_INT_CLR_TXFIFO_EMPTY;

    /* Fill the fifo (if there is more data to send). */
    fill_tx_fifo(drv_p);

    /* Resume the writing thread when all data has been written to the
     * fifo. */
    if (drv_p->txsize == 0) {
        dev_p->regs_p->INT_ENA &= ~ESP8266_UART_INT_ENA_TXFIFO_EMPTY;
        thrd_resume_isr(drv_p->thrd_p, 0);

        xSemaphoreGiveFromISR(thrd_idle_sem, NULL);
    }
}

static inline void rx_isr(struct uart_driver_t *drv_p,
                          struct uart_device_t *dev_p)
{
    char c;
    uint32_t error;

    /* ToDo: hande errors.*/
    error = 0;
    c = dev_p->regs_p->FIFO;

    /* Clear the interrupt. */
    dev_p->regs_p->INT_CLR = ESP8266_UART_INT_CLR_RXFIFO_FULL;

    /* Error frames are discarded. */
    if (error == 0) {
        /* Write data to input queue. */
        if (chan_write_isr(&drv_p->chin, &c, 1) != 1) {
            fs_counter_increment(&rx_channel_overflow, 1);
        }

        xSemaphoreGiveFromISR(thrd_idle_sem, NULL);
    } else {
        fs_counter_increment(&rx_errors, 1);
    }
}

/**
 * Both UART:s share the same IRQ.
 */
static void isr(void *arg_p)
{
    struct uart_device_t *dev_p = &uart_device[0];
    struct uart_driver_t *drv_p = dev_p->drv_p;

    if (dev_p->regs_p->INT_ST & ESP8266_UART_INT_ST_TXFIFO_EMPTY) {
        tx_isr(drv_p, dev_p);
    }

    if (dev_p->regs_p->INT_ST & ESP8266_UART_INT_ST_RXFIFO_FULL) {
        rx_isr(drv_p, dev_p);
    }
}

static int uart_port_module_init()
{
    fs_counter_init(&rx_channel_overflow,
                    FSTR("/drivers/uart/rx_channel_overflow"),
                    0);
    fs_counter_register(&rx_channel_overflow);

    fs_counter_init(&rx_errors,
                    FSTR("/drivers/uart/rx_errors"),
                    0);
    fs_counter_register(&rx_errors);

    return (0);
}

static int uart_port_start(struct uart_driver_t *drv_p)
{
    volatile struct esp8266_uart_t *regs_p;

    regs_p = drv_p->dev_p->regs_p;

   /* Wait until the transmit fifo is empty. */
    while ((regs_p->STATUS & ESP8266_UART_STATUS_TXFIFO_CNT_MASK) != 0);

    thrd_sleep_us(100000);

    /* Configure the hardware and reset the fifos. */
    regs_p->CLKDIV = (F_CPU / drv_p->baudrate);
    regs_p->CONF0 |= (ESP8266_UART_CONF0_TXFIFO_RST
                      | ESP8266_UART_CONF0_RXFIFO_RST);
    regs_p->CONF0 &= (~(ESP8266_UART_CONF0_TXFIFO_RST
                        | ESP8266_UART_CONF0_RXFIFO_RST));

    regs_p->INT_ENA = (ESP8266_UART_INT_ST_RXFIFO_FULL);

    drv_p->dev_p->drv_p = drv_p;

    /* Setup the interrut handler. */
    _xt_isr_attach(ESP8266_IRQ_NUM_UART, (_xt_isr)isr, NULL);
    _xt_isr_unmask(1 << ESP8266_IRQ_NUM_UART);

    return (0);
}

static int uart_port_stop(struct uart_driver_t *drv)
{
    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    struct uart_driver_t *self_p;

    self_p = container_of(arg_p, struct uart_driver_t, chout);

    sem_take(&self_p->sem, NULL);

    /* Initiate transfer by writing to the fifo. */
    self_p->txbuf_p = txbuf_p;
    self_p->txsize = size;
    self_p->thrd_p = thrd_self();

    sys_lock();

    fill_tx_fifo(self_p);
    self_p->dev_p->regs_p->INT_ENA |= ESP8266_UART_INT_ENA_TXFIFO_EMPTY;

    thrd_suspend_isr(NULL);

    sys_unlock();

    sem_give(&self_p->sem, 1);

    return (size);
}

static ssize_t uart_port_write_cb_isr(void *arg_p,
                                      const void *txbuf_p,
                                      size_t size)
{
    return (-1);
}

static int uart_port_device_start(struct uart_device_t *dev_p,
                                  long baudrate)
{
    return (-1);
}

static int uart_port_device_stop(struct uart_device_t *dev_p)
{
    return (-1);
}

static ssize_t uart_port_device_read(struct uart_device_t *dev_p,
                                     void *buf_p,
                                     size_t size)
{
    return (-1);
}

static ssize_t uart_port_device_write(struct uart_device_t *dev_p,
                                      const void *buf_p,
                                      size_t size)
{
    return (-1);
}
