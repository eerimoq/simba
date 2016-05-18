/**
 * @file avr/gnu/sys_port.i
 * @version 0.5.0
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

#include "esp_common.h"
#include "eagle_soc.h"

#define SYS_TICK_TIMER_PRESCALE ESP8266_TIMER_CTRL_PRESCALE_256
#define SYS_TICK_TIMER_LOAD 3125

#define SYS_SIMBA_MAIN_STACK_MAX 2048

/* The main function is defined by the user in main.c. */
extern int main();

/* Defined in the linker script. */
extern char __simba_stack_begin;
extern char __simba_stack_size;

/* The main stack is placed before all other stacks in the memory. */
static uint32_t always_free_stack[8]
__attribute__ ((section (".simba_stack_free")));

/* The main stack is placed before all other stacks in the memory. */
uint32_t main_stack[DIV_CEIL(sizeof(struct thrd_t)
                             + (SYS_SIMBA_MAIN_STACK_MAX),
                             sizeof(uint32_t))]
__attribute__ ((section (".simba_main_stack")));

static int sys_port_module_init(void)
{
    /* Configure and start the system tick timer. */
    ESP8266_TIMER0->CTRL = (ESP8266_TIMER_CTRL_ENABLE
                            | ESP8266_TIMER_CTRL_AUTO_LOAD
                            | SYS_TICK_TIMER_PRESCALE
                            | ESP8266_TIMER_CTRL_INT_TRIGGER_EDGE);
    
    _xt_isr_attach(ESP8266_IRQ_NUM_TIMER1,
                   (_xt_isr)sys_tick,
                   NULL);
    TM1_EDGE_INT_ENABLE();
    _xt_isr_unmask(1 << ESP8266_IRQ_NUM_TIMER1);
    ESP8266_TIMER0->LOAD = SYS_TICK_TIMER_LOAD;

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

/**
 * This function is called by Espressif FreeRTOS when the system
 * initialized.
 */
void user_init()
{
    /* The linker garbage collects there if they are unused. */
    main_stack[0] = 0;
    always_free_stack[0] = 0;

    xTaskGenericCreate(main_task,
                       (const signed char *)"simba",
                       ((size_t)&__simba_stack_size) / sizeof(int),
                       NULL,
                       4,
                       NULL,
                       (long unsigned int *)(&__simba_stack_begin),
                       NULL);
}
