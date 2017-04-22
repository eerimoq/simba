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

#include "c_types.h"
#include "esp_libc.h"
#include "esp_misc.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_ssc.h"

#include "esp8266/esp8266.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "eagle_soc.h"

#define SYS_TICK_TIMER_PRESCALE ESP8266_TIMER_CTRL_PRESCALE_256
#define SYS_TICK_TIMER_LOAD 3125

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
                             + (CONFIG_SYS_SIMBA_MAIN_STACK_MAX),
                             sizeof(uint32_t))]
__attribute__ ((section (".simba_main_stack")));

#if CONFIG_SYSTEM_TICK_SOFTWARE == 0

static int sys_port_module_init(void)
{
    /* Configure and start the system tick timer. */
    ESP8266_TIMER0->CTRL = (ESP8266_TIMER_CTRL_ENABLE
                            | ESP8266_TIMER_CTRL_AUTO_LOAD
                            | SYS_TICK_TIMER_PRESCALE
                            | ESP8266_TIMER_CTRL_INT_TRIGGER_EDGE);

    _xt_isr_attach(ESP8266_IRQ_NUM_TIMER1, (_xt_isr)sys_tick_isr, NULL);
    TM1_EDGE_INT_ENABLE();
    _xt_isr_unmask(1 << ESP8266_IRQ_NUM_TIMER1);
    ESP8266_TIMER0->LOAD = SYS_TICK_TIMER_LOAD;

    return (0);
}

#else

static os_timer_t sys_tick_timer;

/**
 * System tick software timer callback.
 */
static void sys_tick_sw(void *arg_p)
{
    sys_lock();
    sys_tick_isr();
    sys_unlock();
}

static int sys_port_module_init(void)
{
    os_timer_setfn(&sys_tick_timer, sys_tick_sw, NULL);
    os_timer_arm(&sys_tick_timer, 10, 1);

    return (0);
}

#endif

__attribute__ ((noreturn))
static void sys_port_stop(int error)
{
    while (1);
}

static void sys_port_panic_putc(char c)
{
}

__attribute__ ((noreturn))
static void sys_port_reboot()
{
    system_restart();

    while (1);
}

static int sys_port_backtrace(void **buf_pp, size_t size)
{
    return (0);
}

static int sys_port_get_time_into_tick()
{
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

static cpu_usage_t sys_port_interrupt_cpu_usage_get(void)
{
    return (0);
}

static void sys_port_interrupt_cpu_usage_reset(void)
{
}

static void putc_null(char c)
{
}

/**
 * Simba runs in this FreeRTOS task.
 */
static void main_task(void *events)
{
    /* Ignore all output from the SDK. */
    os_install_putc1(putc_null);

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
