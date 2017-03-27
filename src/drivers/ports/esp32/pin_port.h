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

#ifndef __DRIVERS_PIN_PORT_H__
#define __DRIVERS_PIN_PORT_H__

struct pin_device_t {
    int id;    /* GPIO number. */
    int iomux; /* IO_MUX index. */
};

struct pin_driver_t {
    const struct pin_device_t *dev_p;
};

static inline int pin_port_device_set_mode(const struct pin_device_t *dev_p,
                                           int mode)
{
    if (mode == PIN_OUTPUT) {
        /* Do not use GPIO matrix for this pin. */
        ESP32_GPIO->FUNC_OUT_SEL_CFG[dev_p->id] = 0x100;

        if (dev_p->id < 32) {
            ESP32_GPIO->ENABLE[0].W1TS = (1 << dev_p->id);
        } else {
            ESP32_GPIO->ENABLE[1].W1TS = (1 << (dev_p->id - 32));
        }
    } else {
        if (dev_p->id < 32) {
            ESP32_GPIO->ENABLE[0].W1TC = (1 << dev_p->id);
        } else {
            ESP32_GPIO->ENABLE[1].W1TC = (1 << (dev_p->id - 32));
        }
    }

    ESP32_IO_MUX->PIN[dev_p->iomux] = (ESP32_IO_MUX_PIN_MCU_SEL_GPIO
                                       | ESP32_IO_MUX_PIN_FUNC_IE);
    ESP32_GPIO->PIN[dev_p->id] = 0;

    return (0);
}

static inline int pin_port_device_read(const struct pin_device_t *dev_p)
{
    if (dev_p->id < 32) {
        return ((ESP32_GPIO->IN_VALUE[0] >> dev_p->id) & 0x1);
    } else {
        return ((ESP32_GPIO->IN_VALUE[1] >> (dev_p->id - 32)) & 0x1);
    }
}

static inline int pin_port_device_write_high(const struct pin_device_t *dev_p)
{
    if (dev_p->id < 32) {
        ESP32_GPIO->OUT[0].W1TS = (1 << dev_p->id);
    } else {
        ESP32_GPIO->OUT[1].W1TS = (1 << (dev_p->id - 32));
    }

    return (0);
}

static inline int pin_port_device_write_low(const struct pin_device_t *dev_p)
{
    if (dev_p->id < 32) {
        ESP32_GPIO->OUT[0].W1TC = (1 << dev_p->id);
    } else {
        ESP32_GPIO->OUT[1].W1TC = (1 << (dev_p->id - 32));
    }

    return (0);
}

#endif
