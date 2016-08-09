/**
 * @file drivers/ports/esp/pin_port.h
 * @version 6.0.0
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
    int id;
    uint32_t mask;
    int iomux;
};

struct pin_driver_t {
    const struct pin_device_t *dev_p;
};

static inline int pin_port_device_set_mode(const struct pin_device_t *dev_p,
                                           int mode)           
{
    if (mode == PIN_OUTPUT) {
        ESP8266_IOMUX->PIN[dev_p->iomux] =
            (ESP8266_IOMUX_PIN_FUNC_GPIO(dev_p->iomux)
             | ESP8266_IOMUX_PIN_OUTPUT_ENABLE);
        ESP8266_GPIO->CONF[dev_p->id] = 0;
        ESP8266_GPIO->ENABLE_DATA_W1TS = dev_p->mask;
    } else {
        ESP8266_IOMUX->PIN[dev_p->iomux] =
            (ESP8266_IOMUX_PIN_FUNC_GPIO(dev_p->iomux));
        ESP8266_GPIO->CONF[dev_p->id] = ESP8266_GPIO_CONF_DRIVER;
        ESP8266_GPIO->ENABLE_DATA_W1TC = dev_p->mask;
    }
    
    return (0);
}

static inline int pin_port_device_read(const struct pin_device_t *dev_p)
{
    return ((ESP8266_GPIO->IN & dev_p->mask) >> dev_p->id);
}

static inline int pin_port_device_write_high(const struct pin_device_t *dev_p)
{
    ESP8266_GPIO->OUT_W1TS = dev_p->mask;
    
    return (0);
}

static inline int pin_port_device_write_low(const struct pin_device_t *dev_p) 
{
    ESP8266_GPIO->OUT_W1TC = dev_p->mask;
    
    return (0);
}

#endif
