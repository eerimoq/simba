/**
 * @file queue.c
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
 * MERQUEUETABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

#define QUEUE_GET_SIZE(queue)                                   \
    (((queue)->wrpos - (queue)->rdpos) & ((queue)->size - 1))

#define QUEUE_GET_FREE(queue)                                           \
    ((queue)->size - QUEUE_GET_SIZE(queue) - 1)

struct queue_element_t {
    /* Channel element base structure. */
    struct chan_element_t base;
    /* Buffer to read data into. NOTE: not the internal queue input
       buffer. */
    void *buf_p;
    /* Number of bytes to read into buf_p.*/
    size_t size;
};

int queue_init(struct queue_t *queue_p,
               void *buf_p,
               size_t size)
{
    chan_init(&queue_p->base,
              (ssize_t (*)(void *, void *, size_t))queue_read,
              (ssize_t (*)(void *, const void *, size_t))queue_write,
              (size_t (*)(void *))queue_size);
    queue_p->buf_p = buf_p;
    queue_p->size = size;
    queue_p->wrpos = 0;
    queue_p->rdpos = 0;
    LIST_SL_INIT(&queue_p->writers);

    return (0);
}

ssize_t queue_read(struct queue_t *queue_p, void *buf_p, size_t size)
{
    struct queue_element_t element, *reader_p, *writer_p;
    struct chan_element_t *r_p, *w_p;
    size_t size0, n;
    spin_irq_t irq;

    spin_lock(&chan_lock, &irq);

    /* Add ourself to readers list. */
    element.base.thrd_p = thrd_self();
    element.base.list_p = NULL;
    element.buf_p = buf_p;
    element.size = size;
    LIST_SL_ADD_TAIL(&queue_p->base.readers, &element.base);

    /* Just suspend if there are other readers in the list. Write function
       will copy data from writer to reader. Therefore return when
       resumed. */
    LIST_SL_PEEK_HEAD(&queue_p->base.readers, &reader_p);

    if (reader_p != &element) {
        spin_unlock(&chan_lock, &irq);
        thrd_suspend(NULL);

        return (size);
    }

    /* Read from buffer if there is data to read. */
    if (queue_p->buf_p != NULL) {
        n = (element.size > QUEUE_GET_SIZE(queue_p) ?
             QUEUE_GET_SIZE(queue_p) :
             element.size);
        if ((queue_p->rdpos + n) <= queue_p->size) {
            memcpy(buf_p, &((char *)queue_p->buf_p)[queue_p->rdpos], n);
        } else {
            size0 = (queue_p->size - queue_p->rdpos);
            memcpy(buf_p, &((char *)queue_p->buf_p)[queue_p->rdpos], size0);
            memcpy(buf_p + size0, &((char *)queue_p->buf_p)[0], n - size0);
        }
        queue_p->rdpos += n;
        queue_p->rdpos &= (queue_p->size - 1);
        element.buf_p += n;
        element.size -= n;
    }

    /* There is no readers present, is there any writers? */
    while (element.size > 0) {
        LIST_SL_REMOVE_HEAD(&queue_p->writers, &w_p);
        writer_p = (struct queue_element_t *)w_p;

        /* No writer in list. */
        if (writer_p == NULL) {
            break;
        }

        /* Copy data to ourself. */
        n = (element.size > writer_p->size ? writer_p->size : element.size);
        memcpy(element.buf_p, writer_p->buf_p, n);
        element.buf_p += n;
        element.size -= n;
        writer_p->buf_p += n;
        writer_p->size -= n;

        if (element.size == 0) {
            LIST_SL_REMOVE_HEAD(&queue_p->base.readers, &r_p);
        }

        /* Resume writer with all data written, otherwise re-add to list. */
        if (writer_p->size == 0) {
            spin_unlock(&chan_lock, &irq);
            thrd_resume(writer_p->base.thrd_p, 0);
            spin_lock(&chan_lock, &irq);
        } else {
            LIST_SL_ADD_HEAD(&queue_p->writers, &writer_p->base);
        }
    }

    if (element.size == 0) {
        LIST_SL_REMOVE_HEAD(&queue_p->base.readers, &r_p);
    }

    spin_unlock(&chan_lock, &irq);

    /* If there is more data left to read, just suspend as we
       are already in readers list.*/
    if (element.size > 0) {
        thrd_suspend(NULL);
    }

    return (size);
}

ssize_t queue_write(struct queue_t *queue_p,
                    const void *buf_p,
                    size_t size)
{
    struct queue_element_t element, *reader_p, *writer_p;
    struct chan_element_t *r_p, *w_p;
    struct thrd_t *thrd_p;
    size_t size0, n;
    spin_irq_t irq;

    spin_lock(&chan_lock, &irq);

    /* Add ourself to writers list. */
    element.base.thrd_p = thrd_self();
    element.base.list_p = NULL;
    element.buf_p = (void *)buf_p;
    element.size = size;
    LIST_SL_ADD_TAIL(&queue_p->writers, &element.base);

    /* Just suspend if there are other writers in the list. Read function
       will copy data from writer to reader. Therefore return when
       resumed. */
    LIST_SL_PEEK_HEAD(&queue_p->writers, &writer_p);

    if (writer_p != &element) {
        spin_unlock(&chan_lock, &irq);
        thrd_suspend(NULL);

        return (size);
    }

    /* There is no writers present, is there any readers? */
    while (element.size > 0) {
        LIST_SL_REMOVE_HEAD(&queue_p->base.readers, &r_p);
        reader_p = (struct queue_element_t *)r_p;

        /* No reader in list. */
        if (reader_p == NULL) {
            break;
        }

        /* If the reader is in an active poll list, resume it. */
        if (reader_p->base.list_p != NULL) {
            if ((reader_p->base.list_p->flags & CHAN_LIST_POLLING) != 0) {
                thrd_p = reader_p->base.thrd_p;
                reader_p->base.thrd_p = NULL;
                reader_p->base.list_p->flags = 0;
                /* TODO: BUG: cannot let go of the lock. a thrd resume
                   without rescheduling is needed. */
                spin_unlock(&chan_lock, &irq);
                thrd_resume(thrd_p, 0);
                spin_lock(&chan_lock, &irq);
            }
        } else {
            /* Copy data to reader. */
            n = (element.size > reader_p->size ? reader_p->size : element.size);
            memcpy(reader_p->buf_p, buf_p, n);
            element.buf_p += n;
            element.size -= n;
            reader_p->buf_p += n;
            reader_p->size -= n;

            if (element.size == 0) {
                LIST_SL_REMOVE_HEAD(&queue_p->writers, &w_p);
            }

            /* Resume reader with all data received, otherwise re-add to list. */
            if (reader_p->size == 0) {
                spin_unlock(&chan_lock, &irq);
                thrd_resume(reader_p->base.thrd_p, 0);
                spin_lock(&chan_lock, &irq);
            } else {
                LIST_SL_ADD_HEAD(&queue_p->base.readers, &reader_p->base);
            }
        }
    }

    /* Write to buffer if there is data left to write. */
    if (queue_p->buf_p != NULL) {
        n = (element.size > QUEUE_GET_FREE(queue_p) ?
             QUEUE_GET_FREE(queue_p) :
             element.size);

        if ((queue_p->wrpos + n) <= queue_p->size) {
            memcpy(&((char *)queue_p->buf_p)[queue_p->wrpos], buf_p, n);
        } else {
            size0 = (queue_p->size - queue_p->wrpos);
            memcpy(&((char *)queue_p->buf_p)[queue_p->wrpos], buf_p, size0);
            memcpy(&((char *)queue_p->buf_p)[0], buf_p + size0, n - size0);
        }

        queue_p->wrpos += n;
        queue_p->wrpos &= (queue_p->size - 1);
        element.buf_p += n;
        element.size -= n;
    }

    if (element.size == 0) {
        LIST_SL_REMOVE_HEAD(&queue_p->writers, &w_p);
    }

    spin_unlock(&chan_lock, &irq);

    /* If there is more data left to write, just suspend as we
       are already in writers list.*/
    if (element.size > 0) {
        thrd_suspend(NULL);
    }

    return (size);
}

ssize_t queue_write_irq(struct queue_t *queue_p,
                        const void *buf_p,
                        size_t size)
{
    struct queue_element_t *reader_p;
    struct chan_element_t *r_p;
    size_t size0, n, left;
    struct thrd_t *thrd_p;

    left = size;

    spin_lock_irq(&chan_lock);

    /* Any reader? */
    LIST_SL_REMOVE_HEAD(&queue_p->base.readers, &r_p);
    reader_p = (struct queue_element_t *)r_p;

    /* A reader is added in two places; when calling read() and when
       polling a list. In the second case there is no buffer to read
       into, so add the data to the queue input buffer. */
    if (reader_p != NULL) {
        /* If the reader is in an active poll list, resume it. */
        if (reader_p->base.list_p != NULL) {
            if ((reader_p->base.list_p->flags & CHAN_LIST_POLLING) != 0) {
                thrd_p = reader_p->base.thrd_p;
                reader_p->base.thrd_p = NULL;
                reader_p->base.list_p->flags = 0;
                spin_unlock_irq(&chan_lock);
                thrd_resume_irq(thrd_p, 0);
                spin_lock_irq(&chan_lock);
            }
        } else {
            /* It's a normal read(), copy data to reader. */
            n = (left > reader_p->size ? reader_p->size : left);
            memcpy(reader_p->buf_p, buf_p, n);
            reader_p->buf_p += n;
            reader_p->size -= n;
            buf_p += n;
            left -= n;
            
            /* Resume reader with all data received, otherwise re-add to list. */
            if (reader_p->size == 0) {
                thrd_resume_irq(reader_p->base.thrd_p, 0);
            } else {
                LIST_SL_ADD_HEAD(&queue_p->base.readers, &reader_p->base);
            }
        }
    }

    /* Write to buffer if there is data left to write. */
    if ((left > 0) && (queue_p->buf_p != NULL)) {
        n = (left > QUEUE_GET_FREE(queue_p) ? QUEUE_GET_FREE(queue_p) : left);

        if (n > 0) {
            if ((queue_p->wrpos + n) <= queue_p->size) {
                memcpy(&((char *)queue_p->buf_p)[queue_p->wrpos], buf_p, n);
            } else {
                size0 = (queue_p->size - queue_p->wrpos);
                memcpy(&((char *)queue_p->buf_p)[queue_p->wrpos], buf_p, size0);
                memcpy(&((char *)queue_p->buf_p)[0], buf_p + size0, n - size0);
            }

            left -= n;
            queue_p->wrpos += n;
            queue_p->wrpos &= (queue_p->size - 1);
        }
    }

    spin_unlock_irq(&chan_lock);

    return (size - left);
}

ssize_t queue_size(struct queue_t *queue_p)
{
    return (((queue_p->wrpos - queue_p->rdpos) & (queue_p->size - 1)) +
            (queue_p->writers.head_p != NULL));
}
