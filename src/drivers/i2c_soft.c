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

#include "simba.h"

#if CONFIG_I2C_SOFT == 1

/**
 * Wait for clock streching to end. That is, wait for SCL to be 1.
 *
 * @return zero(0) if the end was found (SCL is 1), otherwise negative
 *         error code.
 */
static int wait_for_clock_stretching_end(struct i2c_soft_driver_t *self_p)
{
    long clock_stretching_us;

    clock_stretching_us = 0;

    do {
        if (pin_device_read(self_p->scl_p) == 1) {
            return (0);
        }

        thrd_sleep_us(self_p->clock_stretching_sleep_us);
        clock_stretching_us += self_p->clock_stretching_sleep_us;
    } while (clock_stretching_us < self_p->max_clock_stretching_us);

    return (-1);
}

/**
 * Send the start condition by setting SDA low while keeping SCL
 * high.
 *
 * Both SDA and SCL are set to 0 if this function returns 0.
 */
static int start_cond(struct i2c_soft_driver_t *self_p)
{
    /* The line is busy if SDA is low. */
    pin_device_set_mode(self_p->sda_p, PIN_INPUT);

    if (pin_device_read(self_p->sda_p) == 0) {
        return (-1);
    }

    /* SCL is high, set SDA from 1 to 0. */
    pin_device_set_mode(self_p->sda_p, PIN_OUTPUT);
    time_busy_wait_us(self_p->baudrate_us);

    /* Set SCL low as preparation for the first transfer. */
    pin_device_set_mode(self_p->scl_p, PIN_OUTPUT);

    return (0);
}

/**
 * Send the stop condition by pulling SDA high while SCL is high.
 *
 * Both SDA and SCL are floating (set to 1 by pullup) if this function
 * returns 0.
 */
static int stop_cond(struct i2c_soft_driver_t *self_p)
{
    /* Set SDA to 0. */
    pin_device_set_mode(self_p->sda_p, PIN_OUTPUT);
    time_busy_wait_us(self_p->baudrate_us);

    /* SDA to 1. */
    pin_device_set_mode(self_p->scl_p, PIN_INPUT);

    /* Clock stretching. */
    if (wait_for_clock_stretching_end(self_p) != 0) {
        return (-1);
    }

    /* Stop bit setup time, minimum 4us. */
    time_busy_wait_us(self_p->baudrate_us);

    /* SCL is high, set SDA from 0 to 1. */
    pin_device_set_mode(self_p->sda_p, PIN_INPUT);
    time_busy_wait_us(self_p->baudrate_us);

    /* Make sure no device is pulling SDA low. */
    if (pin_device_read(self_p->sda_p) == 0) {
        return (-1);
    }

    time_busy_wait_us(self_p->baudrate_us);

    return (0);
}

/**
 * Write a bit to I2C bus.
 *
 * SDA must be 0 (conf. output) entering and returning from this
 * function.
 */
static int write_bit(struct i2c_soft_driver_t *self_p,
                     int value)
{
    if (value == 1) {
        pin_device_set_mode(self_p->sda_p, PIN_INPUT);
    } else {
        pin_device_set_mode(self_p->sda_p, PIN_OUTPUT);
    }

    /* SDA change propagation delay. */
    time_busy_wait_us(self_p->baudrate_us);

    /* Set SCL high to indicate a new valid SDA value is available */
    pin_device_set_mode(self_p->scl_p, PIN_INPUT);

    /* Wait for SDA value to be read by slave, minimum of 4us for
       standard mode. */
    time_busy_wait_us(self_p->baudrate_us);

    /* Clock stretching */
    if (wait_for_clock_stretching_end(self_p) != 0) {
        return (-1);
    }

    /* Clear the SCL to low in preparation for next change. */
    pin_device_set_mode(self_p->scl_p, PIN_OUTPUT);

    return (0);
}

/**
 * Read a bit from the I2C bus.
 *
 * SDA must be 0 (conf. output) entering and returning from this
 * function.
 */
static int read_bit(struct i2c_soft_driver_t *self_p,
                    uint8_t *value_p)
{
    /* Let the slave drive data. */
    pin_device_set_mode(self_p->sda_p, PIN_INPUT);

    /* Wait for SDA value to be written by slave, minimum of 4us for
       standard mode. */
    time_busy_wait_us(self_p->baudrate_us);

    /* Set SCL high to indicate a new valid SDA value is available. */
    pin_device_set_mode(self_p->scl_p, PIN_INPUT);

    /* Clock stretching. */
    if (wait_for_clock_stretching_end(self_p) != 0) {
        return (-1);
    }

    /* Wait for SDA value to be written by slave, minimum of 4us for
       standard mode. */
    time_busy_wait_us(self_p->baudrate_us);

    /* SCL is high, read out bit. */
    *value_p = pin_device_read(self_p->sda_p);

    /* Set SCL low in preparation for next operation. */
    pin_device_set_mode(self_p->scl_p, PIN_OUTPUT);

    return (0);
}

/**
 * Write a byte to I2C bus. Return 0 if ack by the slave.
 */
static int write_byte(struct i2c_soft_driver_t *self_p,
                      uint8_t byte)
{
    int i;
    uint8_t nack;

    for (i = 0; i < 8; i++) {
        if (write_bit(self_p, (byte & 0x80) != 0) != 0) {
            return (-1);
        }

        byte <<= 1;
    }

    if (read_bit(self_p, &nack) != 0) {
        return (-1);
    }

    return (nack);

}

/**
 * Read a byte from I2C bus.
 */
static int read_byte(struct i2c_soft_driver_t *self_p,
                     uint8_t *byte_p,
                     int ack)
{
    uint8_t bit;
    int i;

    bit = 0;
    *byte_p = 0;

    for (i = 0; i < 8; i++) {
        if (read_bit(self_p, &bit) != 0) {
            return (-1);
        }

        *byte_p = ((*byte_p << 1 ) | bit);
    }

    /* Acknowledge that the byte was successfully received. */
    if (write_bit(self_p, !ack) != 0) {
        return (-1);
    }

    return (0);
}

int i2c_soft_module_init()
{
    return (0);
}

int i2c_soft_init(struct i2c_soft_driver_t *self_p,
                  struct pin_device_t *scl_dev_p,
                  struct pin_device_t *sda_dev_p,
                  long baudrate,
                  long max_clock_stretching_us,
                  long clock_stretching_sleep_us)
{
    self_p->scl_p = scl_dev_p;
    self_p->sda_p = sda_dev_p;
    self_p->baudrate = baudrate;
    self_p->baudrate_us = (1000000L / 2L / baudrate);
    self_p->max_clock_stretching_us = max_clock_stretching_us;
    self_p->clock_stretching_sleep_us = clock_stretching_sleep_us;

    pin_device_set_mode(self_p->scl_p, PIN_INPUT);
    pin_device_set_mode(self_p->sda_p, PIN_INPUT);

    /* The pin output values are always set to 0. The bus state is
       high by configuring the pin as an input with a pullup
       resistor. */
    pin_device_write_low(self_p->scl_p);
    pin_device_write_low(self_p->sda_p);

    return (0);
}

int i2c_soft_start(struct i2c_soft_driver_t *self_p)
{
    return (0);
}

int i2c_soft_stop(struct i2c_soft_driver_t *self_p)
{
    return (0);
}

ssize_t i2c_soft_read(struct i2c_soft_driver_t *self_p,
                      int address,
                      void *buf_p,
                      size_t size)
{
    size_t i;
    uint8_t *b_p;
    int ack;

    b_p = buf_p;

    /* Send the start condition. */
    if (start_cond(self_p) != 0) {
        return (-1);
    }

    /* Write the address with the direction bit set to 1. */
    if (write_byte(self_p, ((address << 1) | 0x1)) != 0) {
        stop_cond(self_p);
        return (-1);
    }

    /* Read the data. */
    for (i = 0; i < size; i++) {
        /* ACK all but last read byte. */
        ack = (i + 1 != size);

        if (read_byte(self_p, &b_p[i], ack) != 0) {
            stop_cond(self_p);
            return (-1);
        }
    }

    /* Send the stop condition. */
    if (stop_cond(self_p) != 0) {
        return (-1);
    }

    return (size);
}

ssize_t i2c_soft_write(struct i2c_soft_driver_t *self_p,
                       int address,
                       const void *buf_p,
                       size_t size)
{
    size_t i;
    const uint8_t *b_p;

    b_p = buf_p;

    /* Send the start condition. */
    if (start_cond(self_p) != 0) {
        return (-1);
    }

    /* Write the address with the direction bit set to 0. */
    if (write_byte(self_p, ((address << 1) | 0x0)) != 0) {
        stop_cond(self_p);
        return (-1);
    }

    /* Write the data. */
    for (i = 0; i < size; i++) {
        if (write_byte(self_p, b_p[i]) != 0) {
            stop_cond(self_p);
            return (-1);
        }
    }

    /* Send the stop condition. */
    if (stop_cond(self_p) != 0) {
        return (-1);
    }

    return (size);
}

int i2c_soft_scan(struct i2c_soft_driver_t *self_p,
                  int address)
{
    int res;

    /* Send the start condition. */
    if (start_cond(self_p) != 0) {
        return (-1);
    }

    /* Write the address with the direction bit set to 0. */
    res = (write_byte(self_p, ((address << 1) | 0x0)) == 0);

    /* Send the stop condition. */
    if (stop_cond(self_p) != 0) {
        return (-1);
    }

    return (res);
}

#endif
