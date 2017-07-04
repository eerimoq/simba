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

int i2c_port_module_init()
{
    return (0);
}

int i2c_port_init(struct i2c_driver_t *self_p,
                  struct i2c_device_t *dev_p,
                  int baudrate,
                  int address)
{
    self_p->dev_p = dev_p;
    self_p->cwgr = baudrate;
    self_p->address = address;

    return (0);
}

int i2c_port_start(struct i2c_driver_t *self_p)
{
    struct i2c_device_t *dev_p;
    volatile struct sam_twi_t *regs_p;
    uint32_t mask;

    dev_p = self_p->dev_p;
    dev_p->drv_p = self_p;
    regs_p = dev_p->regs_p;

    /* PMC. */
    pmc_peripheral_clock_enable(dev_p->id);

    /* Configure the SCL pin. */
    mask = dev_p->scl.mask;
    dev_p->scl.pio_p->PDR = mask;
    dev_p->scl.pio_p->ODR = mask;
    dev_p->scl.pio_p->IDR = mask;
    dev_p->scl.pio_p->PUER = mask;
    dev_p->scl.pio_p->ABSR &= ~mask;

    /* Configure the SDA pin. */
    mask = dev_p->sda.mask;
    dev_p->sda.pio_p->PDR = mask;
    dev_p->sda.pio_p->ODR = mask;
    dev_p->sda.pio_p->IDR = mask;
    dev_p->scl.pio_p->PUER = mask;
    dev_p->sda.pio_p->ABSR &= ~mask;

    /* Clock configuration. */
    regs_p->CWGR = self_p->cwgr;

    /* Master mode. */
    regs_p->CR = SAM_TWI_CR_MSEN;

    return (0);
}

int i2c_port_stop(struct i2c_driver_t *self_p)
{
    self_p->dev_p->regs_p->CR = SAM_TWI_CR_SWRST;

    return (0);
}

ssize_t i2c_port_read(struct i2c_driver_t *self_p,
                      int address,
                      void *buf_p,
                      size_t size)
{
    volatile struct sam_twi_t *regs_p;
    size_t i;
    uint8_t *u8_buf_p;

    u8_buf_p = buf_p;
    regs_p = self_p->dev_p->regs_p;

    /* Slave address and read command. */
    regs_p->MMR = (SAM_TWI_MMR_DADR(address)
                   | SAM_TWI_MMR_MREAD);

    /* Read from the slave into given buffer. */
    regs_p->CR = SAM_TWI_CR_START;

    for (i = 0; i < size - 1; i++) {
        while ((regs_p->SR & SAM_TWI_SR_RXRDY) == 0);

        u8_buf_p[i] = regs_p->RHR;
    }

    /* Send the stop condition after next read. */
    regs_p->CR = SAM_TWI_CR_STOP;

    /* Read the last byte. */
    while ((regs_p->SR & SAM_TWI_SR_RXRDY) == 0);

    u8_buf_p[i] = regs_p->RHR;

    /* Wait for completion. */
    while ((regs_p->SR & SAM_TWI_SR_TXCOMP) == 0);

    return (size);
}

ssize_t i2c_port_write(struct i2c_driver_t *self_p,
                       int address,
                       const void *buf_p,
                       size_t size)
{
    ssize_t res;
    volatile struct sam_twi_t *regs_p;
    size_t i;
    const uint8_t *u8_buf_p;
    uint32_t sr;

    res = size;
    u8_buf_p = buf_p;
    regs_p = self_p->dev_p->regs_p;

    /* Slave address. */
    regs_p->MMR = SAM_TWI_MMR_DADR(address);

    /* Write given buffer to the slave. */
    regs_p->CR = SAM_TWI_CR_START;

    /* Write the first byte. */
    for (i = 0; i < size; i++) {
        regs_p->THR = u8_buf_p[i];

        do {
            sr = regs_p->SR;
        } while ((sr & SAM_TWI_SR_TXRDY) == 0);

        if (sr & SAM_TWI_SR_NACK) {
            res = -1;
            break;
        }
    }

    /* Send the stop condition. */
    regs_p->CR = SAM_TWI_CR_STOP;

    /* Wait for completion. */
    while ((regs_p->SR & SAM_TWI_SR_TXCOMP) == 0);

    return (res);
}

int i2c_port_scan(struct i2c_driver_t *self_p,
                  int address)
{
    volatile struct sam_twi_t *regs_p;
    uint32_t sr;

    regs_p = self_p->dev_p->regs_p;

    /* Slave address and read command. */
    regs_p->MMR = (SAM_TWI_MMR_DADR(address)
                   | SAM_TWI_MMR_MREAD);

    /* Set both start and stop conditions to read a single byte. */
    regs_p->CR = (SAM_TWI_CR_START | SAM_TWI_CR_STOP);

    /* Wait for completion. */
    do {
        sr = regs_p->SR;
    } while ((sr & SAM_TWI_SR_TXCOMP) == 0);

    return ((sr & SAM_TWI_SR_NACK) == 0);
}

int i2c_port_slave_start(struct i2c_driver_t *self_p)
{
    return (-ENOSYS);
}

int i2c_port_slave_stop(struct i2c_driver_t *self_p)
{
    return (-ENOSYS);
}

ssize_t i2c_port_slave_read(struct i2c_driver_t *self_p,
                            void *buf_p,
                            size_t size)
{
    return (-ENOSYS);
}

ssize_t i2c_port_slave_write(struct i2c_driver_t *self_p,
                             const void *buf_p,
                             size_t size)
{
    return (-ENOSYS);
}
