/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#define WRITER_SIZE(queue_p)                                            \
    ((queue_p)->writer_p == NULL ? 0 : (queue_p)->writer_p->left)

#define READER_SIZE(queue_p)                                            \
    (((queue_p)->base.reader_p != NULL) * (queue_p)->reader.left)

struct queue_writer_elem_t {
    struct thrd_prio_list_elem_t base;
    void *buf_p;
    size_t size;
    size_t left;
};

static int control(struct queue_t *self_p, int operation)
{
    int res;

    res = 0;

    switch (operation) {

    case CHAN_CONTROL_NON_BLOCKING_READ:
        self_p->flags |= QUEUE_FLAGS_NON_BLOCKING_READ;
        break;

    case CHAN_CONTROL_BLOCKING_READ:
        self_p->flags &= ~QUEUE_FLAGS_NON_BLOCKING_READ;
        break;

    default:
        res = -EINVAL;
        break;
    }

    return (res);
}

int queue_init(struct queue_t *self_p,
               void *buf_p,
               size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN((buf_p == NULL)
            || ((buf_p != NULL) && (size > 0)), EINVAL);

    chan_init(&self_p->base,
              (chan_read_fn_t)queue_read,
              (chan_write_fn_t)queue_write,
              (chan_size_fn_t)queue_size);
    chan_set_write_isr_cb(&self_p->base, (chan_write_fn_t)queue_write_isr);
    chan_set_control_cb(&self_p->base, (chan_control_fn_t)control);

    thrd_prio_list_init(&self_p->writers);

    self_p->writer_p = NULL;

    self_p->reader.buf_p = NULL;
    self_p->reader.size = 0;
    self_p->reader.left = 0;

    self_p->buf_p = buf_p;

    if (buf_p != NULL) {
        circular_buffer_init(&self_p->buffer, buf_p, size);
    }

    self_p->state = QUEUE_STATE_INITIALIZED;
    self_p->flags = 0;

    return (0);
}

int queue_start(struct queue_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    sys_lock();
    self_p->state = QUEUE_STATE_RUNNING;
    sys_unlock();

    return (0);
}

int queue_stop(struct queue_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int res = 0;

    sys_lock();
    res = queue_stop_isr(self_p);
    sys_unlock();

    return (res);
}

RAM_CODE int queue_stop_isr(struct queue_t *self_p)
{
    int res = 0;

    /* If the reader is from a poll call, the resume value is
       ignored. */
    if (self_p->base.reader_p != NULL) {
        thrd_resume_isr(self_p->base.reader_p,
                        self_p->reader.size - self_p->reader.left);
        self_p->base.reader_p = NULL;
        res = 1;
    }

    if (self_p->writer_p != NULL) {
        thrd_resume_isr(self_p->writer_p->base.thrd_p,
                        self_p->reader.size - self_p->reader.left);
        self_p->writer_p = NULL;
        res = 1;
    }

    self_p->state = QUEUE_STATE_STOPPED;

    return (res);
}

ssize_t queue_read(struct queue_t *self_p, void *buf_p, size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    size_t left, n;
    char *c_buf_p;

    left = size;
    c_buf_p = buf_p;

    sys_lock();

    /* Copy data from queue buffer. */
    if (self_p->buf_p != NULL) {
        n = circular_buffer_read(&self_p->buffer, buf_p, left);
        left -= n;
        c_buf_p += n;
    }

    /* Copy data from the writer, if one is present. */
    while ((self_p->writer_p != NULL) && (left > 0)) {
        n = MIN(left, self_p->writer_p->left);
        memcpy(c_buf_p, self_p->writer_p->buf_p, n);
        self_p->writer_p->buf_p += n;
        self_p->writer_p->left -= n;
        c_buf_p += n;
        left -= n;

        /* Writer buffer empty. */
        if (self_p->writer_p->left == 0) {
            /* Wake the writer. */
            thrd_resume_isr(self_p->writer_p->base.thrd_p,
                            self_p->writer_p->size);

            /* More writers waiting? */
            self_p->writer_p =
                (struct queue_writer_elem_t *)thrd_prio_list_pop_isr(
                    &self_p->writers);
        }
    }

    /* Suspend this thread if more data should be read. */
    if (left > 0) {
        /* No more data will be written to a stopped queue. */
        if (self_p->state == QUEUE_STATE_STOPPED) {
            size = (size - left);
        } else if (self_p->flags & QUEUE_FLAGS_NON_BLOCKING_READ) {
            size = (size - left);

            if (size == 0) {
                size = -EAGAIN;
            }
        } else {
            /* The writer writes the remaining data to the reader buffer. */
            self_p->base.reader_p = thrd_self();
            self_p->reader.buf_p = c_buf_p;
            self_p->reader.size = size;
            self_p->reader.left = left;

            size = thrd_suspend_isr(NULL);
        }
    }

    sys_unlock();

    return (size);
}

ssize_t queue_write(struct queue_t *self_p,
                    const void *buf_p,
                    size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    ssize_t res;
    size_t left;
    const char *c_buf_p;
    struct queue_writer_elem_t elem;

    left = size;
    c_buf_p = buf_p;
    
    sys_lock();

    res = queue_write_isr(self_p, c_buf_p, size);

    if (res >= 0) {
        left -= res;

        /* The reader reads the remaining data. */
        if (left > 0) {
            c_buf_p += (size - left);
            elem.base.thrd_p = thrd_self();
            elem.buf_p = (void *)c_buf_p;
            elem.size = size;
            elem.left = left;

            if (self_p->writer_p == NULL) {
                self_p->writer_p = &elem;
            } else {
                thrd_prio_list_push_isr(&self_p->writers,
                                        (struct thrd_prio_list_elem_t *)&elem);
            }

            res = thrd_suspend_isr(NULL);
        }
    }

    sys_unlock();

    return (res);
}

RAM_CODE ssize_t queue_write_isr(struct queue_t *self_p,
                                 const void *buf_p,
                                 size_t size)
{
    size_t n, left;
    const char *c_buf_p;

    left = size;
    c_buf_p = buf_p;

    /* Resume any polling thread. */
    if (chan_is_polled_isr(&self_p->base)) {
        thrd_resume_isr(self_p->base.reader_p, 0);
        self_p->base.reader_p = NULL;
    }

    /* Write is not possible to a stopped queue. */
    if (self_p->state == QUEUE_STATE_STOPPED) {
        return (-1);
    }

    /* Copy data to the reader, if one is present. */
    if (self_p->base.reader_p != NULL) {
        n = MIN(left, self_p->reader.left);
        memcpy(self_p->reader.buf_p, c_buf_p, n);
        self_p->reader.buf_p += n;
        self_p->reader.left -= n;
        c_buf_p += n;
        left -= n;

        /* Read buffer full. */
        if (self_p->reader.left == 0) {
            /* Wake the reader. */
            thrd_resume_isr(self_p->base.reader_p, self_p->reader.size);
            self_p->base.reader_p = NULL;
        }
    }

    if ((left > 0) && (self_p->buf_p != NULL)) {
        left -= circular_buffer_write(&self_p->buffer, c_buf_p, left);
    }

    return (size - left);
}

RAM_CODE ssize_t queue_size(struct queue_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return ((self_p->buf_p != NULL
             ? circular_buffer_used_size(&self_p->buffer)
             : 0) + WRITER_SIZE(self_p));
}

ssize_t queue_unused_size(struct queue_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    ssize_t res;

    sys_lock();
    res = queue_unused_size_isr(self_p);
    sys_unlock();

    return (res);
}

RAM_CODE ssize_t queue_unused_size_isr(struct queue_t *self_p)
{
    return ((self_p->buf_p != NULL
             ? circular_buffer_unused_size(&self_p->buffer)
             : 0) + READER_SIZE(self_p));
}

ssize_t queue_ignore(struct queue_t *self_p,
                     size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(size >= 0, EINVAL);

    size_t left, n;

    left = size;

    sys_lock();

    /* Ignore data in queue buffer. */
    if (self_p->buf_p != NULL) {
        left -= circular_buffer_skip_front(&self_p->buffer, size);
    }

    /* Ignore data in writers. */
    while ((self_p->writer_p != NULL) && (left > 0)) {
        n = MIN(left, self_p->writer_p->left);
        self_p->writer_p->buf_p += n;
        self_p->writer_p->left -= n;
        left -= n;

        /* Writer buffer empty. */
        if (self_p->writer_p->left == 0) {
            /* Wake the writer. */
            thrd_resume_isr(self_p->writer_p->base.thrd_p,
                            self_p->writer_p->size);

            /* More writers waiting? */
            self_p->writer_p =
                (struct queue_writer_elem_t *)thrd_prio_list_pop_isr(
                    &self_p->writers);
        }
    }

    sys_unlock();

    return (size - left);
}
