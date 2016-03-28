/**
 * @file drivers/linux/pin_port.i
 * @version 0.3.0
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

static int pin_port_module_init(void)
{
    return (0);
}

static int pin_port_init(struct pin_driver_t *drv_p,
                         const struct pin_device_t *dev_p,
                         int mode)
{
    ESP8266_GPIO->CONF[dev_p->id] = 0;

    return (pin_set_mode(drv_p, mode));
}

static int pin_port_read(struct pin_driver_t *drv_p)
{
    const struct pin_device_t *dev_p = drv_p->dev_p;

    return ((ESP8266_GPIO->IN & dev_p->mask) >> dev_p->id);
}

static int pin_port_write(struct pin_driver_t *drv_p, int value)
{
    const struct pin_device_t *dev_p = drv_p->dev_p;

    if (value == 1) {
        ESP8266_GPIO->ENABLE_DATA_W1TS = dev_p->mask;
    } else {
        ESP8266_GPIO->ENABLE_DATA_W1TC = dev_p->mask;
    }

    return (0);
}

static int pin_port_toggle(struct pin_driver_t *drv_p)
{
    int value;
    const struct pin_device_t *dev_p = drv_p->dev_p;

    value = ((ESP8266_GPIO->ENABLE & dev_p->mask) == 0);

    return (pin_port_write(drv_p, value));
}

static int pin_port_set_mode(struct pin_driver_t *drv_p, int mode)
{
    const struct pin_device_t *dev_p = drv_p->dev_p;
    uint32_t iomux_pin_flags;

    if (mode == PIN_OUTPUT) {
        ESP8266_GPIO->ENABLE_DATA_W1TS = (dev_p->mask);
        iomux_pin_flags = ESP8266_IOMUX_PIN_OUTPUT_ENABLE;
    } else {
        ESP8266_GPIO->ENABLE_DATA_W1TC = (dev_p->mask);
        iomux_pin_flags = 0;
    }

    ESP8266_IOMUX->PIN[dev_p->iomux] =
        (ESP8266_IOMUX_PIN_FUNC_GPIO(dev_p->iomux)
         | iomux_pin_flags);

    return (0);
}
