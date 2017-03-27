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

#undef BIT

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_intr.h"

extern xSemaphoreHandle thrd_idle_sem;

static struct fs_counter_t rx_channel_overflow;
static struct fs_counter_t rx_errors;

/**
 * Fill the tx fifo with data from given uart driver.
 *
 * @return Number of bytes not yet added to the transmit fifo.
 */
static size_t fill_tx_fifo(struct uart_driver_t *drv_p)
{
    while ((drv_p->txsize > 0)
           && ((drv_p->dev_p->regs_p->STATUS
                & ESP32_UART_STATUS_TXFIFO_CNT_MASK)
               < ESP32_UART_STATUS_TXFIFO_CNT(128))) {
        drv_p->dev_p->regs_p->FIFO = *drv_p->txbuf_p++;
        drv_p->txsize--;
    }

    return (drv_p->txsize);
}

static inline void tx_isr(struct uart_driver_t *drv_p,
                          struct uart_device_t *dev_p)
{
    portBASE_TYPE higher_prio_task_woken;

    /* Clear the interrupt. */
    dev_p->regs_p->INT_CLR = ESP32_UART_INT_CLR_TXFIFO_EMPTY;

    /* Fill the fifo (if there is more data to send). Resume the
       writing thread when all data has been written to the fifo. */
    if (fill_tx_fifo(drv_p) == 0) {
        dev_p->regs_p->INT_ENA &= ~ESP32_UART_INT_ENA_TXFIFO_EMPTY;
        thrd_resume_isr(drv_p->thrd_p, 0);

        higher_prio_task_woken = pdFALSE;
        xSemaphoreGiveFromISR(thrd_idle_sem, &higher_prio_task_woken);

        if (higher_prio_task_woken == pdTRUE) {
            portYIELD_FROM_ISR() ;
        }
    }
}

static inline void rx_isr(struct uart_driver_t *drv_p,
                          struct uart_device_t *dev_p)
{
    char c;
    portBASE_TYPE higher_prio_task_woken;

    /* ToDo: hande errors.*/

    /* Drain the RX FIFO. Write data to the input channel. */
    while (dev_p->regs_p->STATUS & ESP32_UART_STATUS_RXFIFO_CNT_MASK) {
        c = dev_p->regs_p->FIFO;

        if (chan_write_isr(&drv_p->chin, &c, 1) != 1) {
            fs_counter_increment(&rx_channel_overflow, 1);
        }
    }

    /* Clear the interrupt(s). */
    dev_p->regs_p->INT_CLR = (ESP32_UART_INT_CLR_RXFIFO_TOUT
                              | ESP32_UART_INT_CLR_RXFIFO_FULL);

    higher_prio_task_woken = pdFALSE;
    xSemaphoreGiveFromISR(thrd_idle_sem, &higher_prio_task_woken);

    if (higher_prio_task_woken == pdTRUE) {
        portYIELD_FROM_ISR() ;
    }
}

static RAM_CODE void isr(void *arg_p)
{
    struct uart_device_t *dev_p;
    struct uart_driver_t *drv_p;
    int i;

    for (i = 0; i < membersof(uart_device); i++) {
        dev_p = &uart_device[i];
        drv_p = dev_p->drv_p;

        /* Skip devices without active transfers. */
        if (drv_p == NULL) {
            continue;
        }

        drv_p = dev_p->drv_p;

        if (dev_p->regs_p->INT_ST & ESP32_UART_INT_ST_TXFIFO_EMPTY) {
            tx_isr(drv_p, dev_p);
        }

        if (dev_p->regs_p->INT_ST & (ESP32_UART_INT_ST_RXFIFO_TOUT
                                     | ESP32_UART_INT_ST_RXFIFO_FULL)) {
            rx_isr(drv_p, dev_p);
        }
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

#include "soc/dport_reg.h"

static int uart_port_start(struct uart_driver_t *self_p)
{
    volatile struct esp32_uart_t *regs_p;
    struct uart_device_t *dev_p;
    struct pin_device_t *tx_pin_p;
    struct pin_device_t *rx_pin_p;

    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;
    tx_pin_p = dev_p->tx_pin_p;
    rx_pin_p = dev_p->rx_pin_p;

    /* Wait until the transmit fifo is empty. */
    while ((regs_p->STATUS & ESP32_UART_STATUS_TXFIFO_CNT_MASK) != 0);

    /* Configure TX pin in the GPIO matrix and IO MUX. */
    ESP32_GPIO->FUNC_OUT_SEL_CFG[tx_pin_p->id] = dev_p->tx_signal;
    ESP32_GPIO->FUNC_IN_SEL_CFG[dev_p->tx_signal] = 0x30;
    ESP32_GPIO->PIN[tx_pin_p->id] = 0;
    ESP32_IO_MUX->PIN[tx_pin_p->iomux] = (ESP32_IO_MUX_PIN_MCU_SEL_GPIO);

    /* Configure RX pin in the GPIO matrix and IO MUX. */
    ESP32_GPIO->FUNC_OUT_SEL_CFG[rx_pin_p->id] = 0x100;
    ESP32_GPIO->FUNC_IN_SEL_CFG[dev_p->rx_signal] =
        (ESP32_GPIO_FUNC_IN_SEL_CFG_SIG_IN_SEL
         | rx_pin_p->id);
    ESP32_GPIO->PIN[rx_pin_p->id] = 0;
    ESP32_IO_MUX->PIN[rx_pin_p->iomux] = (ESP32_IO_MUX_PIN_MCU_SEL_GPIO
                                          | ESP32_IO_MUX_PIN_FUNC_WPU
                                          | ESP32_IO_MUX_PIN_FUNC_IE);

    /* Configure the hardware and reset the fifos. */
    regs_p->CLKDIV = (80000000 / self_p->baudrate);
    regs_p->CONF0 = (ESP32_UART_CONF0_BIT_NUM(3)
                     | ESP32_UART_CONF0_STOP_BIT_NUM(1)
                     | ESP32_UART_CONF0_UART_ERR_WR_MASK
                     | ESP32_UART_CONF0_UART_TICK_REF_ALWAYS_ON);
    regs_p->CONF1 = (ESP32_UART_CONF1_RX_TOUT_EN
                     | ESP32_UART_CONF1_RX_TOUT_THRHD(8)
                     | ESP32_UART_CONF1_RXFIFO_FULL_THRHD(16));

    regs_p->INT_ENA = (ESP32_UART_INT_ENA_RXFIFO_TOUT
                       | ESP32_UART_INT_ST_RXFIFO_FULL);

    self_p->dev_p->drv_p = self_p;

    /* Install interrupt handler. */
    esp_xt_set_interrupt_handler(self_p->dev_p->interrupt.cpu, isr, NULL);
    esp_xt_ints_on(BIT(self_p->dev_p->interrupt.cpu));

    /* Map given peripheral interrupts to given CPU interrupt. */
    intr_matrix_set(xPortGetCoreID(),
                    self_p->dev_p->interrupt.source,
                    self_p->dev_p->interrupt.cpu);

    return (0);
}

static int uart_port_stop(struct uart_driver_t *self_p)
{
    intr_matrix_set(xPortGetCoreID(),
                    self_p->dev_p->interrupt.source,
                    PERIPHERAL_DISABLED);

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

    if (fill_tx_fifo(self_p) > 0) {
        self_p->dev_p->regs_p->INT_ENA |= ESP32_UART_INT_ENA_TXFIFO_EMPTY;
        thrd_suspend_isr(NULL);
    }

    sys_unlock();

    sem_give(&self_p->sem, 1);

    return (size);
}

static ssize_t uart_port_write_cb_isr(void *arg_p,
                                      const void *txbuf_p,
                                      size_t size)
{
    struct uart_driver_t *self_p;

    self_p = container_of(arg_p, struct uart_driver_t, chout);

    /* Initiate transfer by writing to the fifo. */
    self_p->txbuf_p = txbuf_p;
    self_p->txsize = size;

    while (fill_tx_fifo(self_p) > 0);

    return (size);
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
