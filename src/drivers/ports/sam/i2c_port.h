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

#ifndef __DRIVERS_I2C_PORT_H__
#define __DRIVERS_I2C_PORT_H__

/* Predefined baudrates. */
#define I2C_PORT_BAUDRATE_1MBPS                 \
    (SAM_TWI_CWGR_CLDIV(36)                     \
     | SAM_TWI_CWGR_CHDIV(36)                   \
     | SAM_TWI_CWGR_CKDIV(0))

#define I2C_PORT_BAUDRATE_400KBPS               \
    (SAM_TWI_CWGR_CLDIV(50)                     \
     | SAM_TWI_CWGR_CHDIV(50)                   \
     | SAM_TWI_CWGR_CKDIV(1))

#define I2C_PORT_BAUDRATE_100KBPS               \
    (SAM_TWI_CWGR_CLDIV(206)                    \
     | SAM_TWI_CWGR_CHDIV(206)                  \
     | SAM_TWI_CWGR_CKDIV(1))

struct i2c_device_t {
    int id;
    volatile struct sam_twi_t *regs_p;
    struct {
        volatile struct sam_pio_t *pio_p;
        uint32_t mask;
    } scl;
    struct {
        volatile struct sam_pio_t *pio_p;
        uint32_t mask;
    } sda;
    struct i2c_driver_t *drv_p;
};

struct i2c_driver_t {
    struct i2c_device_t *dev_p;
    int address;
    int cwgr;
};

#endif
