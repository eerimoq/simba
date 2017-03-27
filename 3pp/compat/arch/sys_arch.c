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

#include "simba.h"

#include "lwip/init.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/raw.h"
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
    time_subtract(&diff, &stop, &start);

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
    thrd_sleep_us(1000 * ms);
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
