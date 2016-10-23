/**
 * @file avr/gnu/sys_port.i
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

#undef BIT

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_intr.h"

/* The main function is defined by the user in main.c. */
extern int main();

static THRD_STACK(main_stack, CONFIG_SYS_SIMBA_MAIN_STACK_MAX)
     __attribute__ ((section (".main_stack")));

static int sys_port_module_init(void)
{
    /* Setup interrupt handler. */
    xt_set_interrupt_handler(ESP32_CPU_INTR_SYS_TICK_NUM,
                             (xt_handler)sys_tick,
                             NULL);
    xt_ints_on(BIT(ESP32_CPU_INTR_SYS_TICK_NUM));
    intr_matrix_set(xPortGetCoreID(),
                    ESP32_INTR_SOURCE_TG0_T0_EDGE,
                    ESP32_CPU_INTR_SYS_TICK_NUM);

    /* Configure and start the system tick timer. */
    ESP32_TIMG0->TIMER[0].ALARMLO = 10000;
    ESP32_TIMG0->TIMER[0].ALARMHI = 0;
    ESP32_TIMG0->TIMER[0].UPDATE = 1;
    ESP32_TIMG0->TIMER[0].CONFIG = (ESP32_TIMG_TIMER_CONFIG_ALARM_EN
                                    | ESP32_TIMG_TIMER_CONFIG_EDGE_INT_EN
                                    | ESP32_TIMG_TIMER_CONFIG_AUTORELOAD
                                    | ESP32_TIMG_TIMER_CONFIG_INCREASE
                                    | ESP32_TIMG_TIMER_CONFIG_EN);

    return (0);
}

static void sys_port_lock(void)
{
    portDISABLE_INTERRUPTS();
}

static void sys_port_unlock(void)
{
    portENABLE_INTERRUPTS();
}

static void sys_port_lock_isr(void)
{
}

static void sys_port_unlock_isr(void)
{
}

void sys_stop(int error)
{
    while (1);
}

static float sys_port_interrupt_cpu_usage_get(void)
{
    return (0.0f);
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
}

/**
 * Simba runs in this FreeRTOS task.
 */
static void main_task(void *events)
{
    /* Call the Simba application main function. */
    main();

    thrd_suspend(NULL);
}

int app_main()
{
    system_init();
    xTaskGenericCreate(&main_task,
                       "simba",
                       sizeof(main_stack) / sizeof(int),
                       NULL,
                       5,
                       NULL,
                       (StackType_t *const)main_stack,
                       NULL,
                       0);

    return (0);
}
