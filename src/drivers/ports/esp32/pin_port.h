/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
