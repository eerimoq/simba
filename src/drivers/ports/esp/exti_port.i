/**
 * @file exti_port.i
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
