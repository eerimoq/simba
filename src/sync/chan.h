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

#ifndef __SYNC_CHAN_H__
#define __SYNC_CHAN_H__

#include "simba.h"

/**
 * Beginning of a log entry.
 */
#define CHAN_CONTROL_LOG_BEGIN                              1

/**
 * End of a log entry.
 */
#define CHAN_CONTROL_LOG_END                                2

/**
 * Beginning of printf output.
 */
#define CHAN_CONTROL_PRINTF_BEGIN                           3

/**
 * End of printf output.
 */
#define CHAN_CONTROL_PRINTF_END                             4

/**
 * Channel read function callback type.
 *
 * @param[in] self_p Channel to read from.
 * @param[out] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of read bytes or negative error code.
 */
typedef ssize_t (*chan_read_fn_t)(void *self_p,
                                  void *buf_p,
                                  size_t size);

/**
 * Channel write function callback type.
 *
 * @param[in] self_p Channel to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
typedef ssize_t (*chan_write_fn_t)(void *self_p,
                                   const void *buf_p,
                                   size_t size);

/**
 * Channel control function callback type.
 *
 * @param[in] self_p Channel to read from.
 * @param[in] operation Control operation.
 *
 * @return Operation specific.
 */
typedef int (*chan_control_fn_t)(void *self_p,
                                 int operation);

/**
 * Channel write filter function callback type.
 *
 * @param[in] self_p Channel to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes in buffer.
 *
 * @return true(1) if the buffer shall be written to the channel,
 *         otherwise false(0).
 */
typedef int (*chan_write_filter_fn_t)(void *self_p,
                                      const void *buf_p,
                                      size_t size);

/**
 * Channel size function callback type.
 *
 * @param[in] self_p Channel to get the size of.
 *
 * @return Number of bytes available.
 */
typedef size_t (*chan_size_fn_t)(void *self_p);

struct chan_list_t {
    struct chan_t **chans_pp;
    size_t max;
    size_t len;
    int flags;
};

/**
 * Channel datastructure.
 */
struct chan_t {
    chan_read_fn_t read;
    chan_write_fn_t write;
    chan_size_fn_t size;
    chan_control_fn_t control;
    chan_write_filter_fn_t write_filter_cb;
    chan_write_fn_t write_isr;
    chan_write_filter_fn_t write_filter_isr_cb;
    /* Reader thread waiting for data. */
    struct thrd_t *reader_p;
    /* Used by the reader when polling channels. */
    struct chan_list_t *list_p;
};

/**
 * Initialize the channel module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int chan_module_init(void);

/**
 * Initialize given channel with given callbacks. A channel must be
 * initialized before it can be used.
 *
 * @param[out] self_p Channel to initialize.
 * @param[in] read Read function callback. This function must
 *                 implement the channel read functionality, and will
 *                 be called when the user reads data from the
 *                 channel.
 * @param[in] write Write function callback. This function must
 *                  implement the channel write functionality, and
 *                  will be called when the user writes data to the
 *                  channel.
 * @param[in] size Size function callback. This function must return
 *                 the size of the channel. It should return zero(0)
 *                 if there is no data available in the channel, and
 *                 otherwise a positive integer.
 *
 * @return zero(0) or negative error code.
 */
int chan_init(struct chan_t *self_p,
              chan_read_fn_t read,
              chan_write_fn_t write,
              chan_size_fn_t size);

/**
 * Set the write isr function callback.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] filter Write isr function to set.
 *
 * @return zero(0) or negative error code.
 */
int chan_set_write_isr_cb(struct chan_t *self_p,
                          chan_write_fn_t write_isr_cb);

/**
 * Set the write filter callback function. The write filter function
 * is called when data is written to the channel, and its return value
 * determines if the data shall be written to the underlying channel
 * implementation, or discarded.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] write_filter_cb filter Write filter function to set.
 *
 * @return zero(0) or negative error code.
 */
int chan_set_write_filter_cb(struct chan_t *self_p,
                             chan_write_filter_fn_t write_filter_cb);

/**
 * Set the write isr filter callback function. The write filter
 * function is called when data is written to the channel, and its
 * return value determines is the data shall be written to the
 * underlying channel implementation, or discarded.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] write_filter_isr_cb filter Write filter function to set.
 *
 * @return zero(0) or negative error code.
 */
int chan_set_write_filter_isr_cb(struct chan_t *self_p,
                                 chan_write_filter_fn_t write_filter_isr_cb);

/**
 * Set control function callback.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] control Control function to set.
 *
 * @return zero(0) or negative error code.
 */
int chan_set_control_cb(struct chan_t *self_p,
                        chan_control_fn_t control_cb);

/**
 * Read data from given channel. The behaviour of this function
 * depends on the channel implementation. Often, the calling thread
 * will be blocked until all data has been read or an error occurs.
 *
 * @param[in] self_p Channel to read from.
 * @param[out] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t chan_read(void *self_p,
                  void *buf_p,
                  size_t size);

/**
 * Write data to given channel. The behaviour of this function depends
 * on the channel implementation. Some channel implementations blocks
 * until the receiver has read the data, and some returns immediately.
 *
 * @param[in] self_p Channel to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t chan_write(void *self_p,
                   const void *buf_p,
                   size_t size);

/**
 * Get the number of bytes available to read from given channel.
 *
 * @param[in] self_p Channel to get the size of.
 *
 * @return Number of bytes available.
 */
size_t chan_size(void *self_p);

/**
 * Control given channel.
 *
 * @return Operation specific.
 */
int chan_control(void *self_p, int operation);

/**
 * Write data to given channel from interrupt context or with the
 * system lock taken. The behaviour of this function depends on the
 * channel implementation. Some channel implementations blocks until
 * the receiver has read the data, and some returns immediately.
 *
 * @param[in] self_p Channel to write to.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t chan_write_isr(void *self_p,
                       const void *buf_p,
                       size_t size);

/**
 * Check if a channel is polled. May only be called from isr or with
 * the system lock taken (see `sys_lock()`).
 *
 * @param[in] self_p Channel to check.
 *
 * @return true(1) or false(0).
 */
int chan_is_polled_isr(struct chan_t *self_p);

/**
 * Initialize an empty list of channels. A list is used to wait for
 * data on multiple channel at the same time. When there is data on at
 * least one channel, the poll function returns and the application
 * can read from the channel with data.
 *
 * @param[in] list_p List to initialize.
 * @param[in] workspace_p Workspace for internal use.
 * @param[in] size Size of the workspace in bytes.
 *
 * @return zero(0) or negative error code.
 */
int chan_list_init(struct chan_list_t *list_p,
                   void *workspace_p,
                   size_t size);

/**
 * Destroy an initialized list of channels.
 *
 * @param[in] list_p List to destroy.
 *
 * @return zero(0) or negative error code.
 */
int chan_list_destroy(struct chan_list_t *list_p);

/**
 * Add given channel to list of channels.
 *
 * @param[in] list_p List of channels.
 * @param[in] chan_p Channel to add.
 *
 * @return zero(0) or negative error code.
 */
int chan_list_add(struct chan_list_t *list_p, void *chan_p);

/**
 * Remove given channel from list of channels.
 *
 * @param[in] list_p List of channels.
 * @param[in] chan_p Channel to remove.
 *
 * @return zero(0) or negative error code.
 */
int chan_list_remove(struct chan_list_t *list_p, void *chan_p);

/**
 * Poll given list of channels for events. Blocks until at least one
 * of the channels in the list has data ready to be read or an timeout
 * occurs.
 *
 * @param[in] list_p List of channels to poll.
 * @param[in] timeout_p Time to wait for data on any channel before a
 *                      timeout occurs. Set to NULL to wait forever.
 *
 * @return Channel with data or NULL on timeout.
 */
void *chan_list_poll(struct chan_list_t *list_p,
                     const struct time_t *timeout_p);

/**
 * Poll given channel for events. Blocks until the channel has data
 * ready to be read or an timeout occurs.
 *
 * @param[in] chan_p Channel to poll.
 * @param[in] timeout_p Time to wait for data on the channel before a
 *                      timeout occurs. Set to NULL to wait forever.
 *
 * @return The channel or NULL on timeout.
 */
void *chan_poll(void *chan_p, const struct time_t *timeout_p);

/**
 * Get a reference to the null channel. This channel will ignore all
 * written data but return that it was successfully written.
 *
 * @return The null channel.
 */
void *chan_null(void);

/**
 * Null channel read function callback. Pass to ``chan_init()`` if no
 * read function callback is needed for the channel.
 *
 * @return Always returns -1.
 */
ssize_t chan_read_null(void *self_p,
                       void *buf_p,
                       size_t size);

/**
 * Null channel write function callback. Pass to ``chan_init()`` if no
 * write function callback is needed for the channel.
 *
 * @return Always returns ``size``.
 */
ssize_t chan_write_null(void *self_p,
                        const void *buf_p,
                        size_t size);

/**
 * Null channel size function callback. Pass to ``chan_init()`` if no
 * size function callback is needed for the channel.
 *
 * @return Always returns zero(0).
 */
size_t chan_size_null(void *self_p);

/**
 * Null channel control function callback. Will silently ignore the
 * control request.
 *
 * @return Always returns zero(0).
 */
int chan_control_null(void *self_p, int operation);

#endif
