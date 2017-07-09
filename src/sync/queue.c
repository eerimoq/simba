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

#define BUFFER_SIZE(buffer_p)                           \
    ((buffer_p)->size > 0 ? (buffer_p)->size - 1 : 0)

#define BUFFER_USED_UNTIL_END(buffer_p)         \
    ((buffer_p)->end_p - (buffer_p)->read_p)

#define WRITER_SIZE(queue_p)                                            \
    ((queue_p)->writer_p == NULL ? 0 : (queue_p)->writer_p->left)

#define READER_SIZE(queue_p)                                            \
    (((queue_p)->base.reader_p != NULL) * (queue_p)->reader.left)

#define BUFFER_UNUSED_UNTIL_END(buffer_p)       \
    ((buffer_p)->end_p - (buffer_p)->write_p)

#define BUFFER_UNUSED(buffer_p)                         \
    (BUFFER_SIZE(buffer_p) - get_buffer_used(buffer_p))

RAM_CODE static inline size_t get_buffer_used(struct queue_buffer_t *buffer_p)
{
    if (buffer_p->begin_p == NULL) {
        return (0);
    }

    if (buffer_p->write_p == buffer_p->read_p) {
        return (0);
    } else if (buffer_p->write_p > buffer_p->read_p) {
        return (buffer_p->write_p - buffer_p->read_p);
    } else {
        return ((buffer_p->end_p - buffer_p->begin_p)
                - (buffer_p->read_p - buffer_p->write_p));
    }
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

    thrd_prio_list_init(&self_p->writers);

    self_p->writer_p = NULL;
    
    self_p->reader.buf_p = NULL;
    self_p->reader.size = 0;
    self_p->reader.left = 0;

    self_p->buffer.begin_p = buf_p;
    self_p->buffer.read_p = buf_p;
    self_p->buffer.write_p = buf_p;
    self_p->buffer.end_p = &((char*)buf_p)[size];
    self_p->buffer.size = size;

    self_p->state = QUEUE_STATE_INITIALIZED;

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
    ASSERTN(size > 0, EINVAL);

    size_t left, n, buffer_used_until_end, buffer_used;
    char *cbuf_p;

    left = size;
    cbuf_p = buf_p;

    sys_lock();

    /* Copy data from queue buffer. */
    if (self_p->buffer.begin_p != NULL) {
        buffer_used = get_buffer_used(&self_p->buffer);

        /* Number of bytes to read from the buffer. */
        if (left < buffer_used) {
            n = left;
        } else {
            n = buffer_used;
        }

        buffer_used_until_end = BUFFER_USED_UNTIL_END(&self_p->buffer);

        if (n <= buffer_used_until_end) {
            /* Read one chunk. */
            memcpy(cbuf_p, self_p->buffer.read_p, n);
            self_p->buffer.read_p += n;
        } else {
            /* Read two chunks, to end and then from beginning. */
            memcpy(cbuf_p, self_p->buffer.read_p, buffer_used_until_end);
            memcpy(cbuf_p + buffer_used_until_end,
                   self_p->buffer.begin_p,
                   (n - buffer_used_until_end));
            self_p->buffer.read_p = self_p->buffer.begin_p;
            self_p->buffer.read_p += (n - buffer_used_until_end);
        }

        cbuf_p += n;
        left -= n;
    }

    /* Copy data from the writer, if one is present. */
    while ((self_p->writer_p != NULL) && (left > 0)) {
        if (left < self_p->writer_p->left) {
            n = left;
        } else {
            n = self_p->writer_p->left;
        }

        memcpy(cbuf_p, self_p->writer_p->buf_p, n);
        self_p->writer_p->buf_p += n;
        self_p->writer_p->left -= n;
        cbuf_p += n;
        left -= n;

        /* Writer buffer empty. */
        if (self_p->writer_p->left == 0) {
            /* Wake the writer. */
            thrd_resume_isr(self_p->writer_p->base.thrd_p,
                            self_p->writer_p->size);
            self_p->writer_p = NULL;

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
        } else {
            /* The writer writes the remaining data to the reader buffer. */
            self_p->base.reader_p = thrd_self();
            self_p->reader.buf_p = cbuf_p;
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
    ASSERTN(size > 0, EINVAL);

    ssize_t res;
    size_t left;
    const char *cbuf_p;
    struct queue_writer_elem_t elem;

    left = size;
    cbuf_p = buf_p;

    sys_lock();

    res = queue_write_isr(self_p, cbuf_p, size);

    if (res >= 0) {
        left -= res;

        /* The reader reads the remaining data. */
        if (left > 0) {
            cbuf_p += (size - left);
            elem.base.thrd_p = thrd_self();
            elem.buf_p = (void *)cbuf_p;
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
    size_t buffer_unused_until_end, buffer_unused;
    const char *cbuf_p;

    left = size;
    cbuf_p = buf_p;

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
        if (left < self_p->reader.left) {
            n = left;
        } else {
            n = self_p->reader.left;
        }

        memcpy(self_p->reader.buf_p, cbuf_p, n);

        self_p->reader.buf_p += n;
        self_p->reader.left -= n;
        cbuf_p += n;
        left -= n;

        /* Read buffer full. */
        if (self_p->reader.left == 0) {
            /* Wake the reader. */
            thrd_resume_isr(self_p->base.reader_p, self_p->reader.size);
            self_p->base.reader_p = NULL;
        }
    }

    if ((left > 0) && (self_p->buffer.begin_p != NULL)) {
        buffer_unused = BUFFER_UNUSED(&self_p->buffer);

        if (left < buffer_unused) {
            n = left;
        } else {
            n = buffer_unused;
        }

        buffer_unused_until_end = BUFFER_UNUSED_UNTIL_END(&self_p->buffer);

        if (n <= buffer_unused_until_end) {
            /* Data fits before the end of the buffer. */
            memcpy(self_p->buffer.write_p, cbuf_p, n);
            self_p->buffer.write_p += n;
        } else {
            /* The data does not fit begore the end of the
             * buffer. Write until end and then start from the
             * beginning. */
            memcpy(self_p->buffer.write_p, cbuf_p, buffer_unused_until_end);
            memcpy(self_p->buffer.begin_p,
                   cbuf_p + buffer_unused_until_end,
                   (n - buffer_unused_until_end));
            self_p->buffer.write_p = self_p->buffer.begin_p;
            self_p->buffer.write_p += (n - buffer_unused_until_end);
        }

        left -= n;
    }

    return (size - left);
}

RAM_CODE ssize_t queue_size(struct queue_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (get_buffer_used(&self_p->buffer) + WRITER_SIZE(self_p));
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
    return (BUFFER_UNUSED(&self_p->buffer) + READER_SIZE(self_p));
}
