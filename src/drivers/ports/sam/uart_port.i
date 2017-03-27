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

static int uart_port_start(struct uart_driver_t *self_p)
{
    uint16_t cd = (F_CPU / 16 / self_p->baudrate - 1);
    struct uart_device_t *dev_p = self_p->dev_p;
    uint32_t mask;

    /* PMC */
    pmc_peripheral_clock_enable(dev_p->id);

    /* Configure the RX pin. */
    mask = dev_p->pio.rx.mask;
    dev_p->pio.rx.regs_p->PDR = mask;
    dev_p->pio.rx.regs_p->ODR = mask;
    dev_p->pio.rx.regs_p->IDR = mask;
    dev_p->pio.rx.regs_p->PUER = mask;
    dev_p->pio.rx.regs_p->ABSR &= ~mask;

    /* Configure the TX pin. */
    mask = dev_p->pio.tx.mask;
    dev_p->pio.tx.regs_p->PDR = mask;
    dev_p->pio.tx.regs_p->ODR = mask;
    dev_p->pio.tx.regs_p->IDR = mask;
    dev_p->pio.tx.regs_p->ABSR &= ~mask;

    /* Set baudrate. */
    dev_p->regs_p->BRGR = cd;

    /* Set mode and parity. */
    dev_p->regs_p->MR = (US_MR_PAR_NO
                         | US_MR_CHRL_8_BIT
                         | US_MR_USART_MODE_NORMAL);

    /* Disable all interrupts. */
    dev_p->regs_p->IDR = (0xfffffffful);

    /* Setup RX buffer of one byte in PDC. */
    dev_p->regs_p->PDC.RPR = (uint32_t)dev_p->rxbuf;
    dev_p->regs_p->PDC.RCR = 1;

    /* Enable rx signals and interrupt. */
    dev_p->regs_p->IER = (US_IER_ENDRX
                          | US_IER_RXRDY
                          | US_IER_OVRE
                          | US_IER_FRAME
                          | US_IER_PARE);
    dev_p->regs_p->PDC.PTCR = (PERIPH_PTCR_RXTEN);

    /* Disable the tx interrupt. */
    dev_p->regs_p->PDC.PTCR = (PERIPH_PTCR_TXTDIS);

    /* Enable tx and rx. */
    dev_p->regs_p->CR = (US_CR_TXEN | US_CR_RXEN);

    /* nvic */
    nvic_enable_interrupt(dev_p->id);

    dev_p->drv_p = self_p;

    return (0);
}

static int uart_port_stop(struct uart_driver_t *self_p)
{
    struct uart_device_t *dev_p = self_p->dev_p;

    dev_p->regs_p->CR = 0;
    dev_p->regs_p->IDR = 0xfffffffful;
    dev_p->regs_p->PDC.PTCR = 0;

    dev_p->drv_p = NULL;

    return (0);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    struct uart_driver_t *self_p;
    struct uart_device_t *dev_p;

    self_p = container_of(arg_p, struct uart_driver_t, chout);
    dev_p = self_p->dev_p;

    sem_take(&self_p->sem, NULL);

    sys_lock();

    /* Initiate transfer by writing to the PDC registers. */
    dev_p->regs_p->PDC.TPR = (uint32_t)txbuf_p;
    dev_p->regs_p->PDC.TCR = size;

    /* Enalbe the PDC. */
    dev_p->regs_p->PDC.PTCR = (PERIPH_PTCR_TXTEN);

    dev_p->regs_p->IER = (US_IER_ENDTX);

    self_p->thrd_p = thrd_self();

    thrd_suspend_isr(NULL);

    /* Disable the PDC. */
    dev_p->regs_p->PDC.PTCR = (PERIPH_PTCR_TXTDIS);

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

static void isr(int index)
{
    struct uart_device_t *dev_p = &uart_device[index];
    struct uart_driver_t *drv_p = dev_p->drv_p;
    uint32_t csr, error;

    if (drv_p == NULL) {
        return;
    }

    /* Mask the expected signals. */
    csr = dev_p->regs_p->CSR;
    csr &= dev_p->regs_p->IMR;

    /* Handle tx complete signal. */
    if (csr & US_CSR_ENDTX) {
        dev_p->regs_p->IDR = (US_IDR_ENDTX);
        thrd_resume_isr(drv_p->thrd_p, 0);
    }

    /* Handle rx complete signal. */
    if (csr & US_CSR_ENDRX) {
        error = (csr & (US_CSR_OVRE | US_CSR_FRAME | US_CSR_PARE));

        if (error == 0) {
            /* Write data to input queue. */
            if (chan_write_isr(&drv_p->chin, dev_p->rxbuf, 1) != 1) {
                fs_counter_increment(&rx_channel_overflow, 1);
            }
        } else {
            fs_counter_increment(&rx_errors, 1);
        }

        /* Reset counter to receive next byte. */
        dev_p->regs_p->PDC.RPR = (uint32_t)dev_p->rxbuf;
        dev_p->regs_p->PDC.RCR = 1;
    }
}

#define UART_ISR(vector, index)                 \
    ISR(vector) {                               \
        isr(index);                             \
    }                                           \

#if (UART_DEVICE_MAX >= 1)
UART_ISR(uart, 0)
#endif

#if (UART_DEVICE_MAX >= 2)
UART_ISR(usart0, 1)
#endif

#if (UART_DEVICE_MAX >= 3)
UART_ISR(usart1, 2)
#endif

#if (UART_DEVICE_MAX >= 4)
UART_ISR(usart2, 3)
#endif

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
