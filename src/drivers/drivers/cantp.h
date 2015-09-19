/**
 * @file drivers/cantp.h
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

#ifndef __DRIVERS_CANTP_H__
#define __DRIVERS_CANTP_H__

#include "simba.h"

enum cantp_type_id_t {
    cantp_type_id_normal = 0,
    cantp_type_id_extended
};

struct cantp_chan_t {
    struct chan_t base;
    struct queue_t chin;
    struct {
        struct queue_t chan;
    } tx;
    struct cantp_t *cantp_p;
};

struct cantp_subscribe_t {
    struct cantp_subscribe_t *next_p;
    struct cantp_chan_t *chan_p;
};

struct cantp_message_header_t {
    int id;
};

struct cantp_config_t {
    uint32_t id;
    enum cantp_type_id_t type;
    int rx_mailbox;
    int tx_mailbox;
    struct cantp_subscribe_t *sub_p;
};

struct cantp_t {
    struct canif_t *canif_p;
    struct cantp_config_t *config_p;
    struct cantp_config_t *(*get_id_config)(void *arg, uint32_t id);
    void *get_id_config_arg_p;
    struct {
        struct thrd_t *thrd_p;
        struct queue_t chan;
        char stack[256];
    } tx;
    struct {
        char stack[256];
    } rx;
};

/**
 * Initialize CANTP module.
 */
int cantp_module_init(void);

/**
 * Initialize CANTP object.
 *
 * @param[in] cantp_p CANTP object to initialize.
 * @param[in] canif_p CAN interface to use.
 *
 * @return zero(0) or negative error code.
 */
int cantp_init(struct cantp_t *cantp_p,
               struct canif_t *canif_p,
               struct cantp_config_t *config_p,
               struct cantp_config_t *(*get_id_config)(void *arg, uint32_t id),
               void *get_id_config_arg);

/**
 * Subscribe for CANTP messages with given ID.  When a cantp
 * frame/message is received from the hardware it is written to the
 * receive channel. Zero or more subscribers per frame ID is allowed.
 *
 * @param[in] cantp_p Initialized driver object.
 * @param[in] id Frame ID to subscribe for.
 * @param[in] sub_p Subscribe structure with receive channel.
 *
 * @return zero(0) or negative error code.
 */
int cantp_subscribe(struct cantp_t *cantp_p,
                    int id,
                    struct cantp_subscribe_t *sub_p);

/**
 * Unsubscribe for CANTP messages with given ID.
 *
 * @param[in] cantp_p Initialized driver object.
 * @param[in] id Frame ID to unsubscribe from.
 * @param[in] sub_p Subscribe structure used when subscribing.
 *
 * @return zero(0) or negative error code.
 */
int cantp_unsubscribe(struct cantp_t *cantp_p,
                      int id,
                      struct cantp_subscribe_t *sub_p);

/**
 * Open a CANTP channel for reading and writing of messages.
 *
 * @param[in] cantp_p Initialized driver object.
 * @param[in] chan_p Channel to initialize.
 * @param[in] buf_p Receive buffer.
 * @param[in] size Receive buffer size.
 *
 * @return zero(0) or negative error code.
 */
int cantp_chan_init(struct cantp_t *cantp_p,
                    struct cantp_chan_t *chan_p,
                    void *buf_p,
                    size_t size);

/**
 * Read message from channel.
 *
 * @param[in] chan_p Receive channel.
 * @param[out] msg_p Read message with header and data. It musy fit
 *                   the expected message size.
 * @param[in] size Message size.
 *
 * @return Number of bytes read or negative error code.
 */
static inline ssize_t cantp_chan_read(struct cantp_chan_t *chan_p,
                                      struct cantp_message_header_t *msg_p,
                                      size_t size)
{
    return (chan_p->chin.base.read(&chan_p->chin, msg_p, size));
}

/**
 * Write message to channel.
 *
 * @param[in] chan_p Channel.
 * @param[in] msg_p Message to write.
 * @param[in] size Message size.
 *
 * @return Number of bytes written or negative error code.
 */
static inline ssize_t cantp_chan_write(struct cantp_chan_t *chan_p,
                                       const struct cantp_message_header_t *msg_p,
                                       size_t size)
{
    return (chan_p->base.write(chan_p, msg_p, size));
}

#endif
