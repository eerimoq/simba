/**
 * @file qlog.c
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
#include <stdarg.h>
#include <limits.h>

#define QLOG_ID_LEVEL(id) ((id >> 16) & 0xff)
#define QLOG_ID_ID(id) (id & 0xffff)

FS_COMMAND_DEFINE("/kernel/qlog/format", qlog_cmd_format);
FS_COMMAND_DEFINE("/kernel/qlog/trigger", qlog_cmd_trigger);

extern const char FAR *qlog_id_to_string[];

struct entry_t {
    unsigned long time;
    qlog_id_t qlog_id;
    long args[4];
};

struct trigger_t {
    qlog_id_t qlog_id;
    uint8_t mask;
    long args[4];
};

struct qlog_t {
    struct spin_lock_t spin;
    int mode;
    int pos;
    struct entry_t entries[QLOG_ENTRIES_MAX];
    struct trigger_t trigger;
};

static struct qlog_t qlog;

static int is_trigger_condition_met(struct entry_t *entry_p)
{
    int i;

    if (entry_p->qlog_id != qlog.trigger.qlog_id) {
        return (0);
    }

    for (i = 0; i < membersof(entry_p->args); i++) {
        if (qlog.trigger.mask & (1 << i)) {
            if (entry_p->args[i] != qlog.trigger.args[i]) {
                return (0);
            }
        }
    }

    return (1);
}

int qlog_cmd_format(int argc,
                    const char *argv[],
                    chan_t *chout_p,
                    chan_t *chin_p)
{
    return (qlog_format(chout_p));
}

int qlog_cmd_trigger(int argc,
                     const char *argv[],
                     chan_t *chout_p,
                     chan_t *chin_p)
{
    return (0);
}

int qlog_module_init(void)
{
    int i;

    /* Initialize qlog object. */
    spin_init(&qlog.spin);
    qlog.mode = QLOG_MODE_CIRCULAR;
    qlog.pos = 0;

    for (i = 0; i < QLOG_ENTRIES_MAX; i++) {
        qlog.entries[i].qlog_id = -1;
    }

    return (0);
}

int qlog_set_mode(int mode)
{
    int old;
    spin_irq_t irq;

    spin_lock(&qlog.spin, &irq);

    old = qlog.mode;
    qlog.mode = mode;

    spin_unlock(&qlog.spin, &irq);

    return (old);
}

int qlog_get_mode()
{
    return (qlog.mode);
}

int qlog_set_trigger(qlog_id_t qlog_id,
                     uint8_t mask,
                     long v0,
                     long v1,
                     long v2,
                     long v3)
{
    spin_irq_t irq;

    spin_lock(&qlog.spin, &irq);

    qlog.trigger.qlog_id = qlog_id;
    qlog.trigger.mask = mask;
    qlog.trigger.args[0] = v0;
    qlog.trigger.args[1] = v1;
    qlog.trigger.args[2] = v2;
    qlog.trigger.args[3] = v3;

    spin_unlock(&qlog.spin, &irq);

    return (0);
}

int qlog_write(qlog_id_t qlog_id,
               long v0,
               long v1,
               long v2,
               long v3)
{
    struct time_t now;
    struct entry_t entry;
    uint8_t level = QLOG_ID_LEVEL(qlog_id);
    int written = 0;
    spin_irq_t irq;

    /* Check if severity level is set. */
    if ((thrd_get_log_mask() & (1 << level)) == 0) {
        return (0);
    }

    /* Create entry. */
    time_get(&now);
    entry.time = now.seconds;
    entry.qlog_id = qlog_id;
    entry.args[0] = v0;
    entry.args[1] = v1;
    entry.args[2] = v2;
    entry.args[3] = v3;

    spin_lock(&qlog.spin, &irq);

    if (qlog.mode != QLOG_MODE_OFF) {
        if (qlog.mode == QLOG_MODE_TRIGGER) {
            if (is_trigger_condition_met(&entry) == 0) {
                spin_unlock(&qlog.spin, &irq);
                return (0);
            }

            qlog.pos = 0;
            qlog.mode = QLOG_MODE_CAPTURE;
        }

        qlog.entries[qlog.pos++] = entry;
        qlog.pos %= QLOG_ENTRIES_MAX;

        if (qlog.mode == QLOG_MODE_CAPTURE) {
            if (qlog.pos == 0) {
                qlog.mode = QLOG_MODE_OFF;
            }
        }
    }

    spin_unlock(&qlog.spin, &irq);

    return (written);
}

int qlog_format(chan_t *chout_p)
{
    int i;
    int pos;
    int old;
    struct entry_t *entry_p;
    FAR const char *fmt_p;
    uint16_t id;

    old = qlog_set_mode(QLOG_MODE_OFF);

    pos = qlog.pos;

    for (i = 0; i < QLOG_ENTRIES_MAX; i++) {
        entry_p = &qlog.entries[pos];

        id = QLOG_ID_ID(entry_p->qlog_id);

        if (id != 0xffff) {
            fmt_p = qlog_id_to_string[id];

            std_fprintf(chout_p, FSTR("%lu: "), entry_p->time);
            std_fprintf(chout_p,
                        fmt_p,
                        entry_p->args[0],
                        entry_p->args[1],
                        entry_p->args[2],
                        entry_p->args[3]);
        }

        pos++;
        pos %= QLOG_ENTRIES_MAX;
    }

    qlog_set_mode(old);

    return (0);
}
