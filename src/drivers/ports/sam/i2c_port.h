/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#ifndef __DRIVERS_I2C_PORT_H__
#define __DRIVERS_I2C_PORT_H__

/* Predefined baudrates. */
#define I2C_PORT_BAUDRATE_1MBPS    0x00
#define I2C_PORT_BAUDRATE_400KBPS  0x0c
#define I2C_PORT_BAUDRATE_100KBPS  0x48

struct i2c_device_t {
    struct i2c_driver_t *drv_p;
    volatile struct sam_twi_t *regs_p;
    struct pin_device_t *twd_p;
    struct pin_device_t *twck_p;
    int id;
};

struct i2c_driver_t {
    struct i2c_device_t *dev_p; /* dev_p contains info about the TWI0 or TWI1 peripheral. Example Usage: dev_p->regs_p->CR */
    int twbr;                   /* Baud Rate */
    volatile ssize_t size; /* Remember remaining bytes to send */
    uint8_t *buf_p; /* Remember pointer to what we are reading/writing so we have it available in interrupts */
    struct thrd_t *thrd_p; 
    
    /* Almost temporary variables */
    int address; /* What most people think about when they think of an i2c address for the chip */
    uint32_t internalAddress;        /* SAM Has concept of internal address */
    uint8_t  internalAddressSize;    /* SAM Has concept of internal address size (0,1,2,3 bytes) */
};

#endif
