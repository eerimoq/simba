/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

/**
 * Fill the tx fifo with data from given uart driver.
 */
static void fill_tx_fifo(struct uart_driver_t *drv_p)
{
    volatile struct pic32mm_uart_t *regs_p;

    regs_p = drv_p->dev_p->regs_p;

    while ((drv_p->txsize > 0)
           && (!(pic32mm_reg_read(&regs_p->STA) & PIC32MM_UART_STA_UTXBF))) {
        pic32mm_reg_write(&regs_p->TXREG, *drv_p->txbuf_p++);
        drv_p->txsize--;
    }

    pic32mm_reg_write(&PIC32MM_INT->IFS[1].CLR, 0x02000000);
}

ISR(uart2_transmission)
{
    struct uart_driver_t *drv_p;

    drv_p = uart_device[1].drv_p;

    if (drv_p->txsize > 0) {
        fill_tx_fifo(drv_p);
    } else {
        pic32mm_reg_write(&PIC32MM_INT->IEC[1].CLR, 0x02000000);
        thrd_resume_isr(drv_p->thrd_p, 0);
    }
}

static int uart_port_module_init()
{
    return (0);
}

static int uart_port_start(struct uart_driver_t *self_p)
{
    volatile struct pic32mm_uart_t *regs_p;
    uint16_t brg;

    regs_p = self_p->dev_p->regs_p;
    brg = (DIV_ROUND(F_CPU, 4 * self_p->baudrate) - 1);

    /* 4 = UART 2 TX (RP14) */
    pic32mm_reg_write(&PIC32MM_PINSEL->RPOR[3].VALUE, (4 << 8));
    pic32mm_reg_write(&regs_p->BRG, brg);
    pic32mm_reg_write(&regs_p->STA, 0);
    pic32mm_reg_write(&regs_p->MODE, (PIC32MM_UART_MODE_ON
                                      | PIC32MM_UART_MODE_BRGH));
    pic32mm_reg_write(&regs_p->STA, (PIC32MM_UART_STA_UTXISEL(1)
                                     | PIC32MM_UART_STA_UTXEN));

    pic32mm_reg_write(&PIC32MM_INT->IPC[14].VALUE, 7 << 10);

    self_p->dev_p->drv_p = self_p;

    // RP18 RX

    return (0);
}

static int uart_port_stop(struct uart_driver_t *self_p)
{
    /* self_p->dev_p->regs_p->TASKS.SUSPEND = 1; */

    return (-ENOSYS);
}

static ssize_t uart_port_write_cb(void *arg_p,
                                  const void *buf_p,
                                  size_t size)
{
    struct uart_driver_t *self_p;

    self_p = container_of(arg_p, struct uart_driver_t, base);

    mutex_lock(&self_p->mutex);

    /* Initiate transfer by writing to the fifo. */
    self_p->txbuf_p = buf_p;
    self_p->txsize = size;
    self_p->thrd_p = thrd_self();

    sys_lock();
    fill_tx_fifo(self_p);
    pic32mm_reg_write(&PIC32MM_INT->IEC[1].SET, 0x02000000);
    thrd_suspend_isr(NULL);

    sys_unlock();

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
    volatile struct pic32mm_uart_t *regs_p;

    regs_p = dev_p->regs_p;

    pic32mm_reg_write(&PIC32MM_PINSEL->RPOR[3].VALUE, (4 << 8)); // 4 = UART 2 TX (RP14)
    pic32mm_reg_write(&regs_p->BRG, 51); // baudrate 9600
    pic32mm_reg_write(&regs_p->STA, 0);
    pic32mm_reg_write(&regs_p->MODE, PIC32MM_UART_MODE_ON);
    pic32mm_reg_write(&regs_p->STA, (PIC32MM_UART_STA_UTXISEL(2)
                                     | PIC32MM_UART_STA_UTXEN));

    // RP18 RX

    return (0);
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
    size_t i;
    const uint8_t *u8_buf_p;
    volatile struct pic32mm_uart_t *regs_p;

    u8_buf_p = buf_p;
    regs_p = dev_p->regs_p;

    for (i = 0; i < size; i++) {
        while ((pic32mm_reg_read(&regs_p->STA) & BIT(8)) == 0);

        pic32mm_reg_write(&regs_p->TXREG, *u8_buf_p++);
    }

    return (size);
}
