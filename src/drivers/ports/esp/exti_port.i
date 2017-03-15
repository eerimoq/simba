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

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

void gpio_pin_intr_state_set(uint32 i, uint32 intr_state)
{
    uint32 pin_reg;

    portENTER_CRITICAL();

    pin_reg = ESP8266_GPIO->CONF[i];
    pin_reg &= (~ESP8266_GPIO_CONF_INT_TYPE_MASK);
    pin_reg |= (intr_state << ESP8266_GPIO_CONF_INT_TYPE_POS);
    ESP8266_GPIO->CONF[i] = pin_reg;

    portEXIT_CRITICAL();
}

static void isr(void *arg_p)
{
    portENTER_CRITICAL();
    for (int i = 0; i < EXTI_DEVICE_MAX; i++) {
        if ((ESP8266_GPIO->STATUS & BIT(i)) != 0) {
            //gpio_pin_intr_state_set(i, 0);
            ESP8266_GPIO->STATUS_W1TC = BIT(i);
            exti_device[i].drv_p->on_interrupt(exti_device[i].drv_p->arg_p);
            //gpio_pin_intr_state_set(i, exti_device[i].drv_p->trigger);
        }
    }
    portEXIT_CRITICAL();
}

static int exti_port_module_init()
{
    _xt_isr_attach(ESP8266_IRQ_NUM_GPIO, isr, NULL);
    _xt_isr_unmask(1<<ESP8266_IRQ_NUM_GPIO);
    return (0);
}

int exti_port_start(struct exti_driver_t *self_p)
{
    self_p->dev_p->drv_p = self_p;
    gpio_pin_intr_state_set(self_p->dev_p->pin_p->id, self_p->trigger);
    return (0);
}

int exti_port_stop(struct exti_driver_t *self_p)
{
    gpio_pin_intr_state_set(self_p->dev_p->pin_p->id, 0);
    return (0);
}

static int exti_port_clear(struct exti_driver_t *self_p)
{
    ESP8266_GPIO->STATUS_W1TC = BIT(self_p->dev_p->pin_p->id);
    return (0);
}
