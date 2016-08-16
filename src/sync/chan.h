/**
 * @file sync/chan.h
 * @version 6.0.0
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

#ifndef __SYNC_CHAN_H__
#define __SYNC_CHAN_H__

#include "simba.h"

/**
 * Abstract channel type.
 */
typedef void chan_t;

/**
 * Channel read function callback type.
 *
 * @param[in] self_p Channel to read from.
 * @param[out] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of read bytes or negative error code.
 */
typedef ssize_t (*thrd_read_fn_t)(chan_t *self_p,
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
typedef ssize_t (*thrd_write_fn_t)(chan_t *self_p,
                                   const void *buf_p,
                                   size_t size);

/**
 * Channel size function callback type.
 *
 * @param[in] self_p Channel to get the size of.
 *
 * @return Number of bytes available.
 */
typedef size_t (*thrd_size_fn_t)(chan_t *self_p);

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
    thrd_read_fn_t read;
    thrd_write_fn_t write;
    thrd_size_fn_t size;
    /* Reader thread waiting for data or writer thread waiting for a
       reader. */
    struct thrd_t *writer_p;
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
              thrd_read_fn_t read,
              thrd_write_fn_t write,
              thrd_size_fn_t size);

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
ssize_t chan_read(chan_t *self_p,
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
ssize_t chan_write(chan_t *self_p,
                   const void *buf_p,
                   size_t size);

/**
 * Get the number of bytes available to read from given channel.
 *
 * @param[in] self_p Channel to get the size of.
 *
 * @return Number of bytes available.
 */
size_t chan_size(chan_t *self_p);

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
int chan_list_add(struct chan_list_t *list_p, chan_t *chan_p);

/**
 * Remove given channel from list of channels.
 *
 * @param[in] list_p List of channels.
 * @param[in] chan_p Channel to remove.
 *
 * @return zero(0) or negative error code.
 */
int chan_list_remove(struct chan_list_t *list_p, chan_t *chan_p);

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
chan_t *chan_list_poll(struct chan_list_t *list_p,
                       struct time_t *timeout_p);

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
chan_t *chan_poll(chan_t *chan_p, struct time_t *timeout_p);

/**
 * Get a reference to the null channel. This channel will ignore all
 * written data but return that it was successfully written.
 *
 * @return The null channel.
 */
chan_t *chan_null(void);

/**
 * Null channel read function callback. Pass to ``chan_init()`` if no
 * read function callback is needed for the channel.
 *
 * @return Always returns -1.
 */
ssize_t chan_read_null(chan_t *self_p,
                       void *buf_p,
                       size_t size);

/**
 * Null channel write function callback. Pass to ``chan_init()`` if no
 * write function callback is needed for the channel.
 *
 * @return Always returns ``size``.
 */
ssize_t chan_write_null(chan_t *self_p,
                        const void *buf_p,
                        size_t size);

/**
 * Null channel size function callback. Pass to ``chan_init()`` if no
 * size function callback is needed for the channel.
 *
 * @return Always returns zero(0).
 */
size_t chan_size_null(chan_t *self_p);

#endif
