/**
 * @file drivers/cantp.c
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

FS_COUNTER_DEFINE("/drivers/can/no_subscriber", cantp_id_no_subscriber);

enum cantp_type_t {
    cantp_tp_type_single_t = 0,
    cantp_tp_type_first_t,
    cantp_tp_type_consecutive_t,
    cantp_tp_type_flow_t
};

struct cantp_write_t {
    struct cantp_write_t *next_p;
    size_t size;
    size_t pos;
    const void *buf_p;
    struct cantp_config_t *config_p;
    struct queue_t *chan_p;
};

struct cantp_read_t {
    int size;
    struct cantp_message_header_t header;
};

static int tp_normal_rx(struct cantp_chan_t *chan_p,
                        struct canif_frame_t *frame_p)
{
    struct cantp_read_t read;

    read.header.id = frame_p->id;

    switch (frame_p->data[0] >> 4) {

    case cantp_tp_type_single_t:
        read.size = (frame_p->size - 1);
        chan_write(&chan_p->chin, &read, sizeof(read));
        chan_write(&chan_p->chin, &frame_p->data[1], read.size);
        break;

    case cantp_tp_type_first_t:
        read.size = frame_p->size;
        chan_write(&chan_p->chin, &read, sizeof(read));
        chan_write(&chan_p->chin, &frame_p->data[2], 6);
        //flow_control_write();
        //channel_p->tp.index = 1;
        break;

    case cantp_tp_type_consecutive_t:
        /* if ((chan_p->tp.index == -1) || */
        /*     (channel_p->tp.index != frame_p->tp.first.index)) { */
        /*     /\* Bad consecutive index. *\/ */
        /*     chan_write(channel_p->chin, NULL, -1); */
        /*     break; */
        /* } */

        chan_write(&chan_p->chin, &frame_p->data[1], (frame_p->size - 1));

        /* Update tp index. */
        /* if (channel_p->tp.left > 0) { */
        /*     channel_p->tp.index++; */
        /* } else { */
        /*     channel_p->tp.index = -1; */
        /* } */

        /* /\* Flow control. *\/ */
        /* if () { */
        /*     flow_control_write(); */
        /* } */

        break;

    case cantp_tp_type_flow_t:
        /* channel_p->tp.flow; */
        break;

    default:
        break;
    }

    return (0);
}

static int tp_extended_rx(struct cantp_chan_t *chan_p,
                          struct canif_frame_t *frame_p)
{
    return (0);
}

static void *rx_main(void *arg_p)
{
    struct canif_frame_t frame;
    struct cantp_config_t *config_p;
    struct cantp_t *cantp_p = arg_p;

    while (1) {
        /* Wait for next CANTP frame. */
        cantp_p->canif_p->read(cantp_p->canif_p, &frame);
        config_p = cantp_p->get_id_config(cantp_p->get_id_config_arg_p, frame.id);

        if (config_p == NULL) {
            /* No channel set for CANTP frame ID. */
            FS_COUNTER_INC(cantp_id_no_subscriber, 1);
            continue;
        }

        switch (config_p->type) {

        case cantp_type_id_normal:
            tp_normal_rx(config_p->sub_p->chan_p, &frame);
            break;

        case cantp_type_id_extended:
            tp_extended_rx(config_p->sub_p->chan_p, &frame);
            break;

        default:
            break;
        }
    }

    return (0);
}

static void *tx_main(void *arg_p)
{
    struct cantp_t *cantp_p = arg_p;
    struct cantp_write_t *write_p;
    struct canif_frame_t frame;
    char c;

    while (1) {
        /* Wait for timeout or write request. */
        queue_read(&cantp_p->tx.chan, &write_p, sizeof(write_p));

        switch (write_p->config_p->type) {

        case cantp_type_id_normal:
            frame.id = write_p->config_p->id;
            frame.size = (write_p->size + 1);
            frame.data[0] = ((cantp_tp_type_single_t << 4) | write_p->size);
            memcpy(&frame.data[1], write_p->buf_p, write_p->size);
            cantp_p->canif_p->write(cantp_p->canif_p, &frame);
            queue_write(write_p->chan_p, &c, sizeof(c));
            break;

        case cantp_type_id_extended:
            ASSERT(0);
            break;

        default:
            break;
        }
    }

    return (0);
}

static ssize_t read_cb(struct cantp_chan_t *chan_p, void *buf_p, size_t size)
{
    int msg_size;

    if (queue_read(&chan_p->chin, &msg_size, sizeof(msg_size)) != sizeof(msg_size)) {
        return (-1);
    }

    msg_size += sizeof(int);

    ASSERT(msg_size <= size);

    return (queue_read(&chan_p->chin, buf_p, msg_size));
}

static ssize_t write_cb(struct cantp_chan_t *chan_p,
                        const struct cantp_message_header_t *msg_p,
                        size_t size)
{
    struct cantp_write_t write, *write_p = &write;
    struct cantp_t *cantp_p = chan_p->cantp_p;
    char c;

    write.size = (size - sizeof(*msg_p));
    write.pos = 0;
    write.buf_p = &msg_p[1];
    write.config_p = cantp_p->get_id_config(cantp_p->get_id_config_arg_p, msg_p->id);
    write.chan_p = &chan_p->tx.chan;

    if (write.config_p == NULL) {
        return (-EINVAL);
    }

    /* Send write request and wait for completion. */
    queue_write(&cantp_p->tx.chan, &write_p, sizeof(write_p));
    queue_read(&chan_p->tx.chan, &c, sizeof(c));

    return (size);
}

static ssize_t size_cb(struct cantp_chan_t *chan_p)
{
    return (queue_size(&chan_p->chin));
}

int cantp_module_init(void)
{
    return (0);
}

int cantp_init(struct cantp_t *cantp_p,
               struct canif_t *canif_p,
               struct cantp_config_t *config_p,
               struct cantp_config_t *(*get_id_config)(void *arg_p,
                                                       uint32_t id),
               void *get_id_config_arg_p)
{
    ASSERT(get_id_config != NULL);
    ASSERT(cantp_p != NULL);
    ASSERT(canif_p != NULL);
    ASSERT(config_p != NULL);

    cantp_p->canif_p = canif_p;
    cantp_p->config_p = config_p;
    cantp_p->get_id_config = get_id_config;
    cantp_p->get_id_config_arg_p = get_id_config_arg_p;
    queue_init(&cantp_p->tx.chan, NULL, 0);
    thrd_spawn(rx_main,
               cantp_p,
               0,
               cantp_p->rx.stack,
               sizeof(cantp_p->rx.stack));
    cantp_p->tx.thrd_p = thrd_spawn(tx_main,
                                    cantp_p,
                                    0,
                                    cantp_p->tx.stack,
                                    sizeof(cantp_p->tx.stack));

    return (0);
}

int cantp_subscribe(struct cantp_t *cantp_p,
                    int id,
                    struct cantp_subscribe_t *sub_p)
{
    struct cantp_config_t *config_p = cantp_p->config_p;

    while (config_p->id != CANIF_ID_INVALID) {
        if (config_p->id == id) {
            sub_p->next_p = config_p->sub_p;
            config_p->sub_p = sub_p;
            return (0);
        }

        config_p++;
    }

    return (-EINVAL);
}

int cantp_unsubscribe(struct cantp_t *cantp_p,
                      int id,
                      struct cantp_subscribe_t *sub_p)
{
    struct cantp_config_t *config_p = cantp_p->config_p;
    struct cantp_subscribe_t *curr_p, **prev_pp;

    while (config_p->id != CANIF_ID_INVALID) {
        if (config_p->id == id) {
            prev_pp = &config_p->sub_p;
            curr_p = config_p->sub_p;

            while (curr_p != NULL) {
                if (curr_p == sub_p) {
                    *prev_pp = curr_p->next_p;
                    return (0);
                }

                prev_pp = &curr_p->next_p;
                curr_p = curr_p->next_p;
            }

            break;
        }

        config_p++;
    }

    return (-EINVAL);
}

int cantp_chan_init(struct cantp_t *cantp_p,
                    struct cantp_chan_t *chan_p,
                    void *buf_p,
                    size_t size)
{
    chan_init(&chan_p->base,
              (ssize_t (*)(chan_t *, void *, size_t))read_cb,
              (ssize_t (*)(chan_t *, const void *, size_t))write_cb,
              (size_t (*)(chan_t *))size_cb);
    chan_p->cantp_p = cantp_p;
    queue_init(&chan_p->tx.chan, NULL, 0);

    return (queue_init(&chan_p->chin, buf_p, size));
}
