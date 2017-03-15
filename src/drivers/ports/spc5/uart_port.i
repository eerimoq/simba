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

#if CONFIG_FS_COUNTERS_UART == 1
static struct fs_counter_t rx_channel_overflow;
static struct fs_counter_t errors;
#endif

static void tx_isr(int index)
{
    struct uart_driver_t *drv_p;
    volatile struct spc5_linflex_t *regs_p;

    drv_p = uart_device[index].drv_p;

    if (drv_p == NULL) {
        return;
    }

    regs_p = drv_p->dev_p->regs_p;

    /* Clear interrupt flags. */
    regs_p->UARTSR = SPC5_LINFLEX_UARTSR_DTF;
    regs_p->LINSR = SPC5_LINFLEX_LINSR_DTF;

    /* Write the next byte or resume suspended thread if all data have
       been written. */
    if (drv_p->txsize > 0) {
        drv_p->dev_p->regs_p->BDRL[3] = *drv_p->txbuf_p++;
        drv_p->txsize--;
    } else {
        thrd_resume_isr(drv_p->thrd_p, 0);
    }
}

static void rx_isr(int index)
{
    struct uart_driver_t *drv_p = uart_device[index].drv_p;
    volatile struct spc5_linflex_t *regs_p;
    char c;
    int error;

    if (drv_p == NULL) {
        return;
    }

    regs_p = drv_p->dev_p->regs_p;

    error = (regs_p->UARTSR & (SPC5_LINFLEX_UARTSR_PE0
                               | SPC5_LINFLEX_UARTSR_PE1
                               | SPC5_LINFLEX_UARTSR_PE2
                               | SPC5_LINFLEX_UARTSR_PE3));
    c = regs_p->BDRM;

    /* Clear status flags. */
    regs_p->UARTSR = (SPC5_LINFLEX_UARTSR_DRF
                      | SPC5_LINFLEX_UARTSR_PE0
                      | SPC5_LINFLEX_UARTSR_PE1
                      | SPC5_LINFLEX_UARTSR_PE2
                      | SPC5_LINFLEX_UARTSR_PE3);
    regs_p->LINSR = SPC5_LINFLEX_LINSR_DRF;

    /* Error frames are discarded. */
    if (error == 0) {
        /* Write data to input queue. */
        if (queue_write_isr(&drv_p->chin, &c, 1) != 1) {
#if CONFIG_FS_COUNTERS_UART == 1
            fs_counter_increment(&rx_channel_overflow, 1);
#endif
        }
    } else {
#if CONFIG_FS_COUNTERS_UART == 1
        fs_counter_increment(&errors, 1);
#endif
    }
}

static void err_isr(int index)
{
    struct uart_device_t *dev_p = &uart_device[index];

    dev_p->regs_p->UARTSR = (SPC5_LINFLEX_UARTSR_SZF
                             | SPC5_LINFLEX_UARTSR_OCF
                             | SPC5_LINFLEX_UARTSR_FEF
                             | SPC5_LINFLEX_UARTSR_BOF);

#if CONFIG_FS_COUNTERS_UART == 1
    fs_counter_increment(&errors, 1);
#endif
}

#define LINFLEX_ISR(index)                      \
    ISR(linflex_ ## index ## _txi) {            \
        tx_isr(index);                          \
    }                                           \
                                                \
    ISR(linflex_ ## index ## _rxi) {            \
        rx_isr(index);                          \
    }                                           \
                                                \
    ISR(linflex_ ## index ## _err) {            \
        err_isr(index);                         \
    }

LINFLEX_ISR(0);
LINFLEX_ISR(1);
LINFLEX_ISR(2);

static int uart_port_module_init()
{
#if CONFIG_FS_COUNTERS_UART == 1
    fs_counter_init(&rx_channel_overflow,
                    FSTR("/drivers/uart/rx_channel_overflow"),
                    0);
    fs_counter_register(&rx_channel_overflow);

    fs_counter_init(&errors,
                    FSTR("/drivers/uart/errors"),
                    0);
    fs_counter_register(&errors);
#endif

    return (0);
}

static int uart_port_start(struct uart_driver_t *self_p)
{
    struct uart_device_t *dev_p;
    volatile struct spc5_linflex_t *regs_p;
    uint32_t linibrr;

    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;

    /* Pin configration. */
    SPC5_SIUL->PCR[18] = (1 << 10) | (1 << 9);
    SPC5_SIUL->PCR[19] = 0x0103;

    SPC5_INTC->PSR[79/4] = 0xffffffff;
    SPC5_INTC->PSR[80/4] = 0xffffffff;
    SPC5_INTC->PSR[81/4] = 0xffffffff;

    /* Configure the UART. */
    regs_p->LINCR1 = 1;
    regs_p->UARTCR = SPC5_LINFLEX_UARTCR_UART;
    regs_p->UARTCR = (SPC5_LINFLEX_UARTCR_UART
                      | SPC5_LINFLEX_UARTCR_WL
                      | SPC5_LINFLEX_UARTCR_TXEN
                      | SPC5_LINFLEX_UARTCR_RXEN
                      | SPC5_LINFLEX_UARTCR_RDFL(0)
                      | SPC5_LINFLEX_UARTCR_TDFL(0));
    linibrr = (F_CPU / 16 / self_p->baudrate);
    regs_p->LINIBRR = linibrr;
    regs_p->LINFBRR = ((((300000000 / self_p->baudrate) - 100 * linibrr)
                        * 16 + 99) / 100);
    regs_p->UARTSR = 0xffffffff;
    regs_p->LINIER = (SPC5_LINFLEX_LINIER_DRIE
                      | SPC5_LINFLEX_LINIER_DTIE);
    regs_p->LINCR1 = 0;

    dev_p->drv_p = self_p;

    return (0);
}

static int uart_port_stop(struct uart_driver_t *self_p)
{
    self_p->dev_p->regs_p->UARTCR = 0;

    return (-1);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *txbuf_p,
                                  size_t size)
{
    struct uart_driver_t *self_p;
    volatile struct spc5_linflex_t *regs_p;

    self_p = container_of(arg_p, struct uart_driver_t, chout);
    regs_p = self_p->dev_p->regs_p;

    sem_take(&self_p->sem, NULL);

    self_p->txbuf_p = (txbuf_p + 1);
    self_p->txsize = (size - 1);
    self_p->thrd_p = thrd_self();

    /* Initiate transfer by writing the first byte, and wait for
       completion. */
    sys_lock();
    regs_p->BDRL[3] = self_p->txbuf_p[-1];
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
    volatile struct spc5_linflex_t *regs_p;
    uint32_t linibrr;

    regs_p = dev_p->regs_p;

    SPC5_SIUL->PCR[18] = 0x0603;
    SPC5_SIUL->PCR[19] = 0x0103;

    /* Configure the UART. */
    regs_p->LINCR1 = SPC5_LINFLEX_LINCR1_INIT;
    regs_p->UARTCR = SPC5_LINFLEX_UARTCR_UART;
    regs_p->UARTCR = (SPC5_LINFLEX_UARTCR_UART
                      | SPC5_LINFLEX_UARTCR_WL
                      | SPC5_LINFLEX_UARTCR_TXEN
                      | SPC5_LINFLEX_UARTCR_RXEN
                      | SPC5_LINFLEX_UARTCR_RDFL(0)
                      | SPC5_LINFLEX_UARTCR_TDFL(0));
    linibrr = (F_CPU / 16 / baudrate);
    regs_p->LINIBRR = linibrr;
    regs_p->LINFBRR = ((((300000000 / baudrate) - 100 * linibrr)
                        * 16 + 99) / 100);
    regs_p->UARTSR = 0xffffffff;
    regs_p->LINCR1 = 0;

    return (0);
}

static int uart_port_device_stop(struct uart_device_t *dev_p)
{
    volatile struct spc5_linflex_t *regs_p;

    regs_p = dev_p->regs_p;

    regs_p->LINCR1 = SPC5_LINFLEX_LINCR1_INIT;
    regs_p->UARTCR = 0;
    regs_p->LINCR1 = 0;

    return (0);
}

static ssize_t uart_port_device_read(struct uart_device_t *dev_p,
                                     void *rxbuf_p,
                                     size_t size)
{
    int i;
    uint8_t *rxb_p;
    volatile struct spc5_linflex_t *regs_p;

    rxb_p = rxbuf_p;
    regs_p = dev_p->regs_p;

    for (i = 0; i < size; i++) {
        while ((regs_p->UARTSR & SPC5_LINFLEX_UARTSR_DRF) == 0);

        *rxb_p++ = regs_p->BDRM;
        regs_p->UARTSR = SPC5_LINFLEX_UARTSR_DRF;
    }

    return (size);
}

static ssize_t uart_port_device_write(struct uart_device_t *dev_p,
                                      const void *txbuf_p,
                                      size_t size)
{
    volatile struct spc5_linflex_t *regs_p;
    int i;
    const uint8_t *buf_p;

    regs_p = dev_p->regs_p;
    buf_p = txbuf_p;

    for (i = 0; i < size; i++) {
        /* Write the next byte. */
        regs_p->BDRL[3] = buf_p[i];

        /* Wait for the byte to be transmitted. */
        while (((regs_p->UARTSR & SPC5_LINFLEX_UARTSR_DTF) == 0));

        regs_p->UARTSR = SPC5_LINFLEX_UARTSR_DTF;
    }

    return (size);
}
