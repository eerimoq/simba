/**
 * @file kernel/chan.h
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

#ifndef __KERNEL_CHAN_H__
#define __KERNEL_CHAN_H__

#include "simba.h"

typedef void chan_t;

struct chan_list_t {
    struct chan_t **chans_pp;
    size_t max;
    size_t len;
    int flags;
};

/* Channel. */
struct chan_t {
    ssize_t (*read)(chan_t *chan_p,
                    void *buf_p,
                    size_t size);
    ssize_t (*write)(chan_t *chan_p,
                     const void *buf_p,
                     size_t size);
    size_t (*size)(chan_t *chan_p);
    /* Reader thread waiting for data or writer thread waiting for a
       reader. */
    struct thrd_t *writer_p;
    struct thrd_t *reader_p;
    /* Used by the reader when polling channels. */
    struct chan_list_t *list_p;
};

/**
 * Initialize module.
 * @return void
 */
int chan_module_init();

/**
 * Initialize channel with given callbacks.
 *
 * @param[in] chan_p Channel to initialize.
 * @param[in] read_p Read function callback.
 * @param[in] write_p Write function callback.
 * @param[in] size Size function callback.
 *
 * @return zero(0) or negative error code.
 */
int chan_init(struct chan_t *chan_p,
              ssize_t (*read)(chan_t *chan_p, void *buf_p, size_t size),
              ssize_t (*write)(chan_t *chan_p, const void *buf_p, size_t size),
              size_t (*size)(chan_t *chan_p));

/**
 * Read from given channel.
 *
 * @param[in] chan_p Channel to read from.
 * @param[in] buf_p Buffer to read to.
 * @param[in] size Size to read.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t chan_read(chan_t *chan_p,
                  void *buf_p,
                  size_t size);

/**
 * Write bytes to given channel.
 *
 * @param[in] chan_p Channel to write to.
 * @param[in] buf_p Buffer to write from.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t chan_write(chan_t *chan_p,
                   const void *buf_p,
                   size_t size);

/**
 * Get number of bytes available to read from the channel.
 *
 * @param[in] chan_p Channel to write to.
 *
 * @return Number of bytes available.
 */
size_t chan_size(chan_t *chan_p);

/**
 * Initialize an empty list of channels.
 *
 * @param[in] list_p List to initialize.
 * @param[in] workspace_p Workspace for internal use.
 * @param[in] size Size of workspace.
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
 * Poll given list of channels for events. Blocks until one of the
 * channels has data ready to be read.
 *
 * @param[in] list_p List of channels to poll.
 *
 * @return Channel with data or NULL.
 */
chan_t *chan_list_poll(struct chan_list_t *list_p);

int chan_is_polled_irq(struct chan_t *chan_p);

#endif
