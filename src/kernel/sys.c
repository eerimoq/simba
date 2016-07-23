/**
 * @file sys.c
 * @version 2.0.0
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

#include "simba.h"

struct sys_t sys = {
    .tick = 0,
    .on_fatal_callback = sys_stop,
    .std_out_p = NULL,
    .interrupt = {
        .start = 0,
        .time = 0
    }
};

static const FAR char config[] = 
    "config: assert=" STRINGIFY(CONFIG_ASSERT) "\r\n"
    "        debug=" STRINGIFY(CONFIG_DEBUG) "\r\n"
    "        profile-stack=" STRINGIFY(CONFIG_PROFILE_STACK) "\r\n"
    "        preemptive-scheduler=" STRINGIFY(CONFIG_PREEMPTIVE_SCHEDULER) "\r\n"
    "        system-tick-frequency=" STRINGIFY(CONFIG_SYSTEM_TICK_FREQUENCY) "\r\n"
    "        console="
#if CONFIG_CONSOLE == CONFIG_CONSOLE_UART
    "uart"
#elif CONFIG_CONSOLE == CONFIG_CONSOLE_USB_CDC
    "usb_cdc"
#else
    "none"
#endif
    "\r\n"
    "        console-device=" STRINGIFY(CONFIG_CONSOLE_DEVICE) "\r\n"
    "        console-baudrate=" STRINGIFY(CONFIG_CONSOLE_BAUDRATE) "\r\n"
    "        monitor-thread=" STRINGIFY(CONFIG_MONITOR_THREAD) "\r\n"
    "        shell-minimal=" STRINGIFY(CONFIG_SHELL_MINIMAL) "\r\n";

extern void time_tick_isr(void);
extern void timer_tick_isr(void);
extern void thrd_tick_isr(void);
extern const FAR char sysinfo[];

static void sys_tick(void) {
    sys.tick++;
    time_tick_isr();
    timer_tick_isr();
    thrd_tick_isr();
}

#include "sys_port.i"

#if CONFIG_FS_CMD_SYS_INFO == 1

static struct fs_command_t cmd_info;

static int cmd_info_cb(int argc,
                       const char *argv[],
                       chan_t *out_p,
                       chan_t *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    std_fprintf(out_p, sysinfo);
    std_fprintf(out_p, config);

    return (0);
}

#endif

#if CONFIG_FS_CMD_SYS_UPTIME == 1

static struct fs_command_t cmd_uptime;

static int cmd_uptime_cb(int argc,
                         const char *argv[],
                         chan_t *out_p,
                         chan_t *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    struct time_t now;

    time_get(&now);

    std_fprintf(out_p,
                FSTR("%lu.%lu seconds\r\n"),
                now.seconds,
                now.nanoseconds / 1000000ul);

    return (0);
}

#endif

int sys_module_init(void)
{
#if CONFIG_FS_CMD_SYS_INFO == 1

    fs_command_init(&cmd_info,
                    FSTR("/kernel/sys/info"),
                    cmd_info_cb,
                    NULL);
    fs_command_register(&cmd_info);

#endif

#if CONFIG_FS_CMD_SYS_UPTIME == 1

    fs_command_init(&cmd_uptime,
                    FSTR("/kernel/sys/uptime"),
                    cmd_uptime_cb,
                    NULL);
    fs_command_register(&cmd_uptime);

#endif

    return (sys_port_module_init());
}

int sys_start(void)
{
    setting_module_init();
    rwlock_module_init();
    fs_module_init();
    std_module_init();
    sem_module_init();
    timer_module_init();
    log_module_init();
    chan_module_init();
    thrd_module_init();
    shell_module_init();
    sys_module_init();

#if CONFIG_CONSOLE != CONFIG_CONSOLE_NONE

    console_module_init();
    console_init();
    console_start();

    sys_set_stdout(console_get_output_channel());
    log_set_default_handler_output_channel(console_get_output_channel());

#endif

    return (0);
}

void sys_set_on_fatal_callback(void (*callback)(int error))
{
    sys.on_fatal_callback = callback;
}

void sys_set_stdout(chan_t *chan_p)
{
    sys.std_out_p = chan_p;
}

chan_t *sys_get_stdout(void)
{
    return (sys.std_out_p);
}

void sys_lock(void)
{
    sys_port_lock();
}

void sys_unlock(void)
{
    sys_port_unlock();
}

void sys_lock_isr(void)
{
    sys_port_lock_isr();
}

void sys_unlock_isr(void)
{
    sys_port_unlock_isr();
}

const FAR char *sys_get_info(void)
{
    return (sysinfo);
}

const FAR char *sys_get_config(void)
{
    return (config);
}

float sys_interrupt_cpu_usage_get(void)
{
    return (sys_port_interrupt_cpu_usage_get());
}

void sys_interrupt_cpu_usage_reset(void)
{
    sys_port_interrupt_cpu_usage_reset();
}
