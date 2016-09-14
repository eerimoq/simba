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
#include "esp8266/ets_sys.h"
#include "gpio_register.h"
#include "eagle_soc.h"

#define GPIO_PIN_ADDR(i)        (GPIO_PIN0_ADDRESS + i*4)

void gpio_pin_intr_state_set(uint32 i, uint32 intr_state)
{
    uint32 pin_reg;

    portENTER_CRITICAL();
    
    pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(i));
    pin_reg &= (~GPIO_PIN_INT_TYPE_MASK);
    pin_reg |= (intr_state << GPIO_PIN_INT_TYPE_LSB);
    GPIO_REG_WRITE(GPIO_PIN_ADDR(i), pin_reg);

    portEXIT_CRITICAL();
}

extern int count;

void on_d4_click(void *arg)
{
    portENTER_CRITICAL();
    gpio_pin_intr_state_set(GPIO_ID_PIN(2), 0);
    count++;
    gpio_pin_intr_state_set(GPIO_ID_PIN(2), 1);
    portEXIT_CRITICAL();
}

static int exti_port_module_init()
{
    count++;
    _xt_isr_attach(ETS_GPIO_INUM, on_d4_click, &count);
    _xt_isr_unmask(1<<ETS_GPIO_INUM);
    return (0);
}

int exti_port_init(struct exti_driver_t *drv,
                   struct exti_device_t *dev,
                   void (*on_interrupt)(struct exti_driver_t *drv))
{
    return (0);
}

int exti_port_start(struct exti_driver_t *drv)
{
    count++;
    gpio_pin_intr_state_set(drv->dev_p, 1);
    return (0);
}

int exti_port_stop(struct exti_driver_t *drv)
{
    gpio_pin_intr_state_set(GPIO_ID_PIN(2), 0);
    return (0);
}

static int exti_port_clear(struct exti_driver_t *self_p)
{
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 0x0000FFFF);
    return (0);
}
