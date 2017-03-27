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
    if (dev_p->id < 16) {
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
    } else {
        ESP8266_RTC->GPIO.PIN = 1;
        ESP8266_RTC->GPIO.CONTROL = 0;

        if (mode == PIN_OUTPUT) {
            ESP8266_RTC->GPIO.EN |= 1;
        } else {
            ESP8266_RTC->GPIO.EN &= ~1;
        }
    }

    return (0);
}

static inline int pin_port_device_read(const struct pin_device_t *dev_p)
{
    if (dev_p->id < 16){
        return ((ESP8266_GPIO->IN & dev_p->mask) >> dev_p->id);
    } else {
        return (ESP8266_RTC->GPIO.IN & 0x01);
    }
}

static inline int pin_port_device_write_high(const struct pin_device_t *dev_p)
{
    if (dev_p->id < 16){
        ESP8266_GPIO->OUT_W1TS = dev_p->mask;
    } else {
        ESP8266_RTC->GPIO.OUT |= 1;
    }

    return (0);
}

static inline int pin_port_device_write_low(const struct pin_device_t *dev_p)
{
    if (dev_p->id < 16){
        ESP8266_GPIO->OUT_W1TC = dev_p->mask;
    } else {
        ESP8266_RTC->GPIO.OUT &= ~1;
    }

    return (0);
}

#endif
