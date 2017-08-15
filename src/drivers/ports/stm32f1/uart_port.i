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

static struct fs_counter_t rx_channel_overflow;
static struct fs_counter_t rx_errors;
static struct fs_counter_t tx_resume_thrd_null;

static void isr(int index)
{
    struct uart_device_t *dev_p = &uart_device[index];
    struct uart_driver_t *drv_p = dev_p->drv_p;
    volatile struct stm32_usart_t *regs_p = dev_p->regs_p;
    uint32_t sr;
    uint32_t error;
    uint8_t byte;

    if (drv_p == NULL) {
        return;
    }

    sr = regs_p->SR;

    /* TX complete. */
    if (sr & STM32_USART_SR_TC) {
        if (drv_p->txsize > 0) {
            regs_p->DR = *drv_p->txbuf_p++;
            drv_p->txsize--;
        } else {
            regs_p->SR = ~STM32_USART_SR_TC;

            if (drv_p->thrd_p != NULL) {
                thrd_resume_isr(drv_p->thrd_p, 0);
            } else {
                fs_counter_increment(&tx_resume_thrd_null, 1);
            }
        }
    }

    /* RX complete. */
    if (sr & STM32_USART_SR_RXNE) {
        error = 0;

        if (error == 0) {
            byte = regs_p->DR;

            /* Write data to input queue. */
            if (queue_write_isr(&drv_p->base, &byte, 1) != 1) {
                fs_counter_increment(&rx_channel_overflow, 1);
            }
        } else {
            fs_counter_increment(&rx_errors, 1);
        }
    }
}

ISR(usart1)
{
    isr(0);
}

ISR(usart2)
{
    isr(1);
}

ISR(usart3)
{
    isr(2);
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

    fs_counter_init(&tx_resume_thrd_null,
                    FSTR("/drivers/uart/tx_resume_thrd_null"),
                    0);
    fs_counter_register(&tx_resume_thrd_null);

    return (0);
}

static int uart_port_start(struct uart_driver_t *self_p)
{
    volatile struct stm32_usart_t *regs_p;
    struct uart_device_t *dev_p = self_p->dev_p;

    regs_p = dev_p->regs_p;

    /* Configure the TX and RX pins. */
    STM32_GPIOA->CRH = bits_insert_32(STM32_GPIOA->CRH, 4, 8, 0x8b);

    regs_p->CR3 = 0;
    regs_p->BRR = ((F_CPU / 16 / self_p->baudrate) << 4);
    regs_p->SR = 0;
    regs_p->CR1 = (((self_p->format << 8) & 0x0000ff00)
                   | STM32_USART_CR1_UE
                   | STM32_USART_CR1_TCIE
                   | STM32_USART_CR1_RXNEIE
                   | STM32_USART_CR1_TE
                   | STM32_USART_CR1_RE);
    regs_p->CR2 = (self_p->format & 0x0000ff00);

    /* nvic */
    nvic_enable_interrupt(37);

    dev_p->drv_p = self_p;

    return (0);
}

static int uart_port_stop(struct uart_driver_t *self_p)
{
    self_p->dev_p->regs_p->CR1 = 0;

    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    struct uart_driver_t *self_p;

    self_p = container_of(arg_p, struct uart_driver_t, base);

    mutex_lock(&self_p->mutex);

    /* Initiate transfer by writing the first byte. */
    self_p->txbuf_p = (txbuf_p + 1);
    self_p->txsize = (size - 1);
    self_p->thrd_p = thrd_self();

    sys_lock();
    self_p->dev_p->regs_p->DR = self_p->txbuf_p[-1];
    thrd_suspend_isr(NULL);
    sys_unlock();

    mutex_unlock(&self_p->mutex);

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
