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
