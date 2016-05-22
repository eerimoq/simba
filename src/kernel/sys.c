/**
 * @file sys.c
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

static struct fs_command_t cmd_info;

static const FAR char config[] = 
    "config: assert=" STRINGIFY(CONFIG_ASSERT) "\r\n"
    "        profile-stack=" STRINGIFY(CONFIG_PROFILE_STACK) "\r\n"
    "        preemptive-scheduler=" STRINGIFY(CONFIG_PREEMPTIVE_SCHEDULER) "\r\n"
    "        monitor-thread=" STRINGIFY(CONFIG_MONITOR_THREAD) "\r\n";

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

int sys_module_init(void)
{
    fs_command_init(&cmd_info,
                    FSTR("/kernel/sys/info"),
                    cmd_info_cb,
                    NULL);
    fs_command_register(&cmd_info);

    return (sys_port_module_init());
}

int sys_start(void)
{
    setting_module_init();
    fs_module_init();
    std_module_init();
    sem_module_init();
    log_module_init();
    chan_module_init();
    thrd_module_init();
    shell_module_init();
    sys_module_init();

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
