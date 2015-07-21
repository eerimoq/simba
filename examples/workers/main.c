/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

struct message_t {
    struct message_t *next_p;
    int x;
};

struct context_t {
    struct list_singly_linked_t messages;
    int i;
    int a;
    int flags;
};

static char jobsbuf[16];
static QUEUE_INIT_DECL(qjobs, jobsbuf, sizeof(jobsbuf));

/* Three worker threads.*/
static char worker_stack[3][512];

/* Eight contexts.*/
static struct context_t contexts[8];
static struct spin_lock_t contexts_lock;

/* Messages.*/
static struct message_t messages[30];

/**
 * Get a job from context's job list or release context.
 */
static struct message_t *get_next(struct context_t *context_p)
{
    spin_irq_t irq;
    struct message_t *message_p;

    spin_lock(&contexts_lock, &irq);
    LIST_SL_REMOVE_HEAD(&context_p->messages, &message_p);
    if (message_p == NULL) {
        context_p->flags = 0;
    }
    spin_unlock(&contexts_lock, &irq);

    return (message_p);
}

/**
 * The worker thread gets jobs from the job channel.
 */
void *worker_entry(void *arg_p)
{
    struct message_t *message_p;
    struct context_t *context_p;

    fprintf(stderr, "worker %p started\n", arg_p);

    while (1) {
        /* Wait for a context with message(s). */
        chan_read(&qjobs, &context_p, sizeof(context_p));

        /* Get job from context jobs list.*/
        while ((message_p = get_next(context_p)) != NULL) {
            context_p->a++;
            fprintf(stderr, "worker = %p, context = %d, a = %d, x = %d\n",
                    arg_p, context_p->i, context_p->a, message_p->x);
            thrd_usleep(10000);
        }
    }
}

int main()
{
    int i, write;
    spin_irq_t irq;
    struct context_t *context_p;
    int dst[membersof(messages)] = {
        0, 0, 4, 5, 6, 3, 3, 5,
        4, 2, 3, 4, 5, 6, 6, 6,
        6, 6, 4, 3, 4, 4, 4, 7,
        7, 7, 3, 2, 2, 2
    };

    for (i = 0; i < membersof(contexts); i++) {
        LIST_SL_INIT(&contexts[i].messages);
        contexts[i].i = i;
        contexts[i].a = 0;
        contexts[i].flags = 0;
    }

    sys_start();
    spin_init(&contexts_lock);

    /* Start the three worker threads.*/
    for (i = 0; i < membersof(worker_stack); i++) {
        thrd_spawn(worker_entry,
                   worker_stack[i],
                   0,
                   worker_stack[i],
                   sizeof(worker_stack[0]));
    }

    /* Write a bunch of jobs to job channel.*/
    for (i = 0; i < membersof(messages); i++) {
        messages[i].x = i;
        context_p = &contexts[dst[i]];

        write = 0;

        /* Add message to context.*/
        spin_lock(&contexts_lock, &irq);
        if (context_p->flags == 0) {
            context_p->flags = 1;
            write = 1;
        }
        LIST_SL_ADD_TAIL(&context_p->messages, &messages[i]);
        spin_unlock(&contexts_lock, &irq);

        /* Write previously empty contexts to channel.*/
        if (write == 1) {
            chan_write(&qjobs, &context_p, sizeof(context_p));
        }
    }

    thrd_suspend(NULL);

    return (0);
}
