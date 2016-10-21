/**
 * @file drivers/ports/esp/pin_port.h
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
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

    ESP32_IO_MUX->PIN[dev_p->iomux] = (ESP32_IO_MUX_PIN_FUNC_GPIO
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
