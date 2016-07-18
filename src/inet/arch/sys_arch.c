/**
 * @file arch/sys_arch.c
 * @version 2.0.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"
#include "arch/sys_arch.h"

static THRD_STACK(tcpip_stack, TCPIP_THREAD_STACKSIZE);
static uint8_t mboxbuf[32];
static struct chan_list_t poll;
static uint8_t workspace[32];

err_t sys_mbox_new(sys_mbox_t *self_p, int queue_sz)
{
    queue_init(&self_p->queue, mboxbuf, sizeof(mboxbuf));
    chan_list_init(&poll, workspace, sizeof(workspace));
    chan_list_add(&poll, &self_p->queue);
    self_p->valid = 1;

    return (ERR_OK);
}

void sys_mbox_free(sys_mbox_t *self_p)
{
}

void sys_mbox_post(sys_mbox_t *self_p, void *msg)
{
    queue_write(&self_p->queue, &msg, sizeof(msg));
}

err_t sys_mbox_trypost(sys_mbox_t *self_p, void *msg)
{
    queue_write(&self_p->queue, &msg, sizeof(msg));

    return (ERR_OK);
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *self_p,
                          void **msg_pp,
                          u32_t timeout_ms)
{
    struct time_t timeout;
    struct time_t start;
    struct time_t stop;
    struct time_t diff;

    *msg_pp = NULL;
    time_get(&start);

    if (timeout_ms == 0) {
        queue_read(&self_p->queue, msg_pp, sizeof(msg_pp));
    } else {
        timeout.seconds = (timeout_ms / 1000);
        timeout.nanoseconds = (1000000ul * (timeout_ms % 1000));
        
        if (chan_list_poll(&poll, &timeout) != NULL) {
            queue_read(&self_p->queue, msg_pp, sizeof(msg_pp));
        }
    }

    time_get(&stop);
    time_diff(&diff, &stop, &start);

    return (1000 * diff.seconds + diff.nanoseconds / 1000000ul);
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *self_p, void **msg_pp)
{
    queue_read(&self_p->queue, *msg_pp, sizeof(*msg_pp));

    return ERR_OK;
}

err_t sys_sem_new(sys_sem_t *self_p, u8_t count)
{
    sem_init(self_p, 0, count);

    return (ERR_OK);
}

u32_t sys_arch_sem_wait(sys_sem_t *self_p, u32_t timeout)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(timeout == 0, EINVAL);

    sem_take(self_p, NULL);
    
    return (0);
}

void sys_sem_signal(sys_sem_t *self_p)
{
    sem_give(self_p, 1);
}

void sys_sem_free(sys_sem_t *self_p)
{
}

err_t sys_mutex_new(sys_mutex_t *self_p)
{
    sem_init(self_p, 0, 1);

    return (ERR_OK);
}

void sys_mutex_lock(sys_mutex_t *self_p)
{
    sem_take(self_p, NULL);
}

void sys_mutex_unlock(sys_mutex_t *self_p)
{
    sem_give(self_p, 1);
}

void sys_mutex_free(sys_mutex_t *self_p)
{
}

void sys_init(void)
{
}

u32_t sys_jiffies(void)
{
    return (sys_now());
}

sys_prot_t sys_arch_protect(void)
{
    sys_lock();

    return (0);
}

void sys_arch_unprotect(sys_prot_t p)
{
    sys_unlock();
}

u32_t sys_now(void)
{
    struct time_t now;

    time_get(&now);

    return (1000 * now.seconds + now.nanoseconds / 1000000ul);
}

void sys_msleep(u32_t ms)
{
    thrd_usleep(1000 * ms);
}

sys_thread_t sys_thread_new(const char *pcName,
                            void (*thread)(void *arg),
                            void *arg_p,
                            int stacksize,
                            int priority)
{
    /* Only the TCPIP thread will be created. */
    
    return (thrd_spawn((void *(*)(void *))thread,
                       arg_p,
                       priority,
                       tcpip_stack,
                       sizeof(tcpip_stack)));
}
