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

#include <avr/interrupt.h>

/* READ/WRITE transfer. */
#define I2C_WRITE                          0x00
#define I2C_READ                           0x01

/* Master start. */
#define I2C_M_START                        0x08
#define I2C_M_REPEATED_START               0x10

/* Master transmit status codes. */
#define I2C_M_TX_START                     0x08
#define I2C_M_TX_REPEATED_START            0x10
#define I2C_M_TX_SLA_W_ACK                 0x18
#define I2C_M_TX_SLA_W_NACK                0x20
#define I2C_M_TX_DATA_ACK                  0x28
#define I2C_M_TX_DATA_NACK                 0x30
#define I2C_M_TX_ARB_LOST                  0x38

#define I2C_M_TX_MASK                      0x20

/* Master receive status codes. */
#define I2C_M_RX_START                     0x08
#define I2C_M_RX_REPEATED_START            0x10
#define I2C_M_RX_ARB_LOST                  0x38
#define I2C_M_RX_SLA_R_ACK                 0x40
#define I2C_M_RX_SLA_R_NACK                0x48
#define I2C_M_RX_DATA_ACK                  0x50
#define I2C_M_RX_DATA_NACK                 0x58

#define I2C_M_RX_MASK                      0x40

/* Slave receive status codes. */
#define I2C_S_RX_SLA_W_ACK                 0x60
#define I2C_S_RX_ARB_LOST_SLA_W_ACK        0x68
#define I2C_S_RX_GENERAL_ADDR_ACK          0x70
#define I2C_S_RX_ARB_LOST_GENERAL_ADDR_ACK 0x78
#define I2C_S_RX_DATA_ACK                  0x80
#define I2C_S_RX_DATA_NACK                 0x88
#define I2C_S_RX_GENERAL_DATA_ACK          0x90
#define I2C_S_RX_GENERAL_DATA_NACK         0x98
#define I2C_S_RX_STOP_OR_REPEATED_START    0xa0

/* Slave transmit status codes. */
#define I2C_S_TX_SLA_R_ACK                 0xa8
#define I2C_S_TX_ARB_LOST_SLA_R_ACK        0xb0
#define I2C_S_TX_DATA_ACK                  0xb8
#define I2C_S_TX_DATA_NACK                 0xc0
#define I2C_S_TX_LAST_DATA_ACK             0xc8

/* Miscellanous status codes. */
#define I2C_MISC_NO_INFO                   0xf8
#define I2C_MISC_ERROR                     0x00

/**
 * Transfer data to and from a slave.
 *
 * @return Number of bytes transferred or negative error code.
 */
static ssize_t transfer(struct i2c_driver_t *self_p,
                        int address,
                        void *buf_p,
                        size_t size,
                        int direction)
{
    self_p->address = ((address << 1) | direction);
    self_p->buf_p = (void *)buf_p;
    self_p->size = size;
    self_p->thrd_p = thrd_self();

    /* Start the transfer by sending the START condition, and then
       wait for the transfer to complete. */
    sys_lock();
    TWCR = (_BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE));
    thrd_suspend_isr(NULL);
    sys_unlock();

    return (size - self_p->size);
}

ISR(TWI_vect)
{
    struct i2c_device_t *dev_p = &i2c_device[0];
    struct i2c_driver_t *drv_p = dev_p->drv_p;
    uint8_t status;

    if (drv_p == NULL) {
        return;
    }

    status = (TWSR & 0xf8);

    switch (status) {

        /* Start. */
    case I2C_M_START:
    case I2C_M_REPEATED_START:
        TWDR = drv_p->address;
        TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWIE));
        break;

        /* Acknowledgement. */
    case I2C_M_TX_SLA_W_ACK:
    case I2C_M_TX_DATA_ACK:
        if (drv_p->size > 0) {
            TWDR = *drv_p->buf_p++;
            drv_p->size--;
            TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWIE));
        } else {
            TWCR = (_BV(TWINT) | _BV(TWSTO) | _BV(TWEN));
            thrd_resume_isr(drv_p->thrd_p, 0);
        }

        break;

    case I2C_M_RX_SLA_R_ACK:
        if (drv_p->size > 1) {
            TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
        } else {
            /* Last data read. */
            TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWIE));
        }

        break;

    case I2C_M_RX_DATA_ACK:
        *drv_p->buf_p++ = TWDR;
        drv_p->size--;

        if (drv_p->size > 1) {
            TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
        } else {
            /* Send NACK on last data read. */
            TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWIE));
        }

        break;

        /* Negative acknowledgement. */
    case I2C_M_RX_DATA_NACK:
        *drv_p->buf_p++ = TWDR;
        drv_p->size--;
    case I2C_M_TX_SLA_W_NACK:
    case I2C_M_TX_DATA_NACK:
    case I2C_M_RX_SLA_R_NACK:
        TWCR = (_BV(TWINT) | _BV(TWSTO) | _BV(TWEN));
        thrd_resume_isr(drv_p->thrd_p, -1);
        break;

        /* Slave transmit. */
    case I2C_S_TX_DATA_ACK:
        drv_p->size--;
    case I2C_S_TX_SLA_R_ACK:
    case I2C_S_TX_ARB_LOST_SLA_R_ACK:
        if (drv_p->thrd_p == NULL) {
            drv_p->size = -1;
            break;
        }

        TWDR = *drv_p->buf_p++;

        if (drv_p->size > 1) {
            TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
        } else {
            /* Last data transmission. */
            TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWIE));
        }

        break;

    case I2C_S_TX_LAST_DATA_ACK:
    case I2C_S_TX_DATA_NACK:
        drv_p->size--;
        TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
        thrd_resume_isr(drv_p->thrd_p, 0);
        drv_p->thrd_p = NULL;
        break;

        /* Slave receive. */
    case I2C_S_RX_SLA_W_ACK:
    case I2C_S_RX_ARB_LOST_SLA_W_ACK:
        if (drv_p->thrd_p == NULL) {
            drv_p->size = -1;
            break;
        }

        TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
        break;

    case I2C_S_RX_DATA_ACK:
    case I2C_S_RX_DATA_NACK:
        *drv_p->buf_p++ = TWDR;
        drv_p->size--;

        if (drv_p->size > 0) {
            TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
        } else {
            /* Last data transmission. */
            TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWIE));
        }

        break;

    case I2C_S_RX_STOP_OR_REPEATED_START:
        TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
        thrd_resume_isr(drv_p->thrd_p, 0);
        drv_p->thrd_p = NULL;
        break;

    default:
        /* ToDo: Handle error cases. */
        break;
    }
}

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
    self_p->twbr = baudrate;
    self_p->address = address;

    return (0);
}

int i2c_port_start(struct i2c_driver_t *self_p)
{
    self_p->dev_p->drv_p = self_p;
    self_p->thrd_p = NULL;

    TWSR = 0;
    TWBR = self_p->twbr;

    return (0);
}

int i2c_port_stop(struct i2c_driver_t *self_p)
{
    return (0);
}

ssize_t i2c_port_read(struct i2c_driver_t *self_p,
                      int address,
                      void *buf_p,
                      size_t size)
{
    return (transfer(self_p, address, buf_p, size, I2C_READ));
}

ssize_t i2c_port_write(struct i2c_driver_t *self_p,
                       int address,
                       const void *buf_p,
                       size_t size)
{
    return (transfer(self_p, address, (void *)buf_p, size, I2C_WRITE));
}

int i2c_port_scan(struct i2c_driver_t *self_p,
                  int address)
{
    int res;

    self_p->address = ((address << 1) | I2C_WRITE);
    self_p->size = 0;
    self_p->thrd_p = thrd_self();

    /* Start the transfer by sending the START condition, and then
       wait for the transfer to complete. */
    sys_lock();
    TWCR = (_BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE));
    res = thrd_suspend_isr(NULL);
    sys_unlock();

    return (res == 0);
}

int i2c_port_slave_start(struct i2c_driver_t *self_p)
{
    self_p->dev_p->drv_p = self_p;
    self_p->thrd_p = NULL;

    TWSR = 0;
    TWAR = (self_p->address << 1);
    TWCR = (_BV(TWEA) | _BV(TWEN) | _BV(TWIE));

    return (0);
}

int i2c_port_slave_stop(struct i2c_driver_t *self_p)
{
    return (0);
}

ssize_t i2c_port_slave_read(struct i2c_driver_t *self_p,
                            void *buf_p,
                            size_t size)
{
    sys_lock();

    /* Read immediately if already addressed by the master. */
    if (self_p->size == -1) {
        TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
    }

    self_p->buf_p = (void *)buf_p;
    self_p->size = size;
    self_p->thrd_p = thrd_self();
    thrd_suspend_isr(NULL);
    sys_unlock();

    return (size - self_p->size);
}

ssize_t i2c_port_slave_write(struct i2c_driver_t *self_p,
                             const void *buf_p,
                             size_t size)
{
    sys_lock();
    self_p->buf_p = (void *)buf_p;

    /* Write immediately if already addressed by the master. */
    if (self_p->size == -1) {
        TWDR = *self_p->buf_p++;

        if (size > 1) {
            TWCR = (_BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE));
        } else {
            /* Last data transmission. */
            TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWIE));
        }
    }

    self_p->size = size;
    self_p->thrd_p = thrd_self();
    thrd_suspend_isr(NULL);
    sys_unlock();

    return (size - self_p->size);
}
