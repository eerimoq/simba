/**
 * @file log.c
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

FS_COMMAND_DEFINE("/kernel/log/set_mode", log_cmd_set_mode);
FS_COMMAND_DEFINE("/kernel/log/get_mode", log_cmd_get_mode);
FS_COMMAND_DEFINE("/kernel/log/format", log_cmd_format);

COUNTER_DEFINE("/kernel/log/discarded", log_discarded);

extern int (*log_id_to_format_fn[])(chan_t *, void *);

struct log_t {
    char mode;
    char *write_p;
    char *end_p;
    unsigned long next_number;
    char buffer[LOG_BUFFER_SIZE];
};

static struct log_t log = {
    .mode = LOG_MODE_CIRCULAR,
    .write_p = &log.buffer[0],
    .end_p = &log.buffer[0],
    .next_number = 0
};

static FAR const char level_emergency[] = "emergency";
static FAR const char level_alert[] = "alert";
static FAR const char level_critical[] = "critical";
static FAR const char level_error[] = "error";
static FAR const char level_warning[] = "warning";
static FAR const char level_notice[] = "notice";
static FAR const char level_info[] = "info";
static FAR const char level_debug[] = "debug";

/* Level strings array. */
static const char FAR *level_as_string[] = {
    level_emergency,
    level_alert,
    level_critical,
    level_error,
    level_warning,
    level_notice,
    level_info,
    level_debug
};

int log_cmd_set_mode(int argc,
                     const char *argv[],
                     chan_t *chout_p,
                     chan_t *chin_p)
{
    if (argc != 2) {
        std_fprintf(chout_p, FSTR("Usage: set_mode <off|circular|capture>\r\n"));
        return (1);
    }

    if (std_strcmp(argv[1], FSTR("off")) == 0) {
        log_set_mode(LOG_MODE_OFF);
    } else if (std_strcmp(argv[1], FSTR("circular")) == 0) {
        log_set_mode(LOG_MODE_CIRCULAR);
    } else if (std_strcmp(argv[1], FSTR("capture")) == 0) {
        log_set_mode(LOG_MODE_CAPTURE);
    } else {
        std_fprintf(chout_p, FSTR("%s: bad mode\r\n"), argv[1]);
        return (1);
    }

    return (0);
}

int log_cmd_get_mode(int argc,
                     const char *argv[],
                     chan_t *chout_p,
                     chan_t *chin_p)
{
    int mode;

    mode = log_get_mode();

    switch (mode) {
    case LOG_MODE_OFF:
        std_fprintf(chout_p, FSTR("off\r\n"));
        break;
    case LOG_MODE_CIRCULAR:
        std_fprintf(chout_p, FSTR("circular\r\n"));
        break;
    case LOG_MODE_CAPTURE:
        std_fprintf(chout_p, FSTR("capture\r\n"));
        break;
    }

    return (0);
}

int log_cmd_format(int argc,
                   const char *argv[],
                   chan_t *chout_p,
                   chan_t *chin_p)
{
    return (log_format(chout_p));
}

int log_module_init(void)
{
    return (0);
}

int log_reset(void)
{
    sys_lock();

    log.mode = LOG_MODE_CIRCULAR;
    log.write_p = &log.buffer[0];
    log.end_p = &log.buffer[0];

    sys_unlock();

    return (0);
}

int log_set_mode(int mode)
{
    int old;

    sys_lock();

    old = log.mode;
    log.mode = mode;

    sys_unlock();

    return (old);
}

int log_get_mode(void)
{
    return (log.mode);
}

int log_write(char level, int id, void *buf_p, size_t size)
{
    struct time_t now;
    struct log_entry_header_t header;
    struct log_entry_footer_t footer;
    int written = 0;
    size_t entry_size = (size + sizeof(header) + sizeof(footer));

    /* Check if severity level is set. */
    if ((thrd_get_log_mask() & (1 << level)) == 0) {
        return (0);
    }

    /* The entry must fit in the buffer. */
    if (entry_size > LOG_BUFFER_SIZE) {
        return (-1);
    }

    /* Create the entry header and footer. */
    time_get(&now);
    header.size = size;
    header.time = now.seconds;
    header.level = level;
    header.id = id;

    footer.size = size;

    sys_lock();

    header.number = log.next_number++;

    if (log.mode != LOG_MODE_OFF) {
        /* Write the entry to the beginning of the buffer if it does
           not fit at the end of the buffer.*/
        if (entry_size > ((char *)&log.buffer[LOG_BUFFER_SIZE] - log.write_p)) {
            log.end_p = log.write_p;
            log.write_p = &log.buffer[0];
        }

        /* Write the entry to the buffer. */
        memcpy(log.write_p, &header, sizeof(header));
        log.write_p += sizeof(header);
        memcpy(log.write_p, buf_p, size);
        log.write_p += size;
        memcpy(log.write_p, &footer, sizeof(footer));
        log.write_p += sizeof(footer);

        if (log.write_p > log.end_p) {
            log.end_p = log.write_p;
        }
    } else {
        COUNTER_INC(log_discarded, 1);
    }

    sys_unlock();

    return (written);
}

int log_format(chan_t *chout_p)
{
    int i, number_of_entries;
    struct log_entry_header_t *header_p;
    struct log_entry_footer_t *footer_p;
    char *begin_p = NULL, *buf_p, *entry_end_p;
    int (*format_fn)(chan_t *, void *);
    size_t entry_size;
    int old_mode;

    /* Empty log? */
    if (log.end_p == &log.buffer[0]) {
        return (0);
    }

    old_mode = log_set_mode(LOG_MODE_OFF);

    /* Find the first entry. */
    number_of_entries = 0;

    /* From last written entry to the beginning of the buffer. */
    entry_end_p = log.write_p;

    while (entry_end_p != &log.buffer[0]) {
        number_of_entries++;
        footer_p = ((struct log_entry_footer_t *)entry_end_p - 1);
        entry_size = (footer_p->size + sizeof(*header_p) + sizeof(*footer_p));
        entry_end_p -= entry_size;
        begin_p = entry_end_p;
    }


    /* From the end pointer to the write pointer. */
    entry_end_p = log.end_p;

    while (((char *)entry_end_p - sizeof(*footer_p)) > log.write_p) {
        footer_p = ((struct log_entry_footer_t *)entry_end_p - 1);
        entry_size = (footer_p->size + sizeof(*header_p) + sizeof(*footer_p));

        if (((char *)entry_end_p - entry_size) > log.write_p) {
            number_of_entries++;
            begin_p = ((char *)entry_end_p - entry_size);
        }

        entry_end_p -= entry_size;
    }

    std_fprintf(chout_p, FSTR("number:time:level: message\r\n"));

    /* Write entries to the channel. */
    for (i = 0; i < number_of_entries; i++) {
        header_p = (struct log_entry_header_t *)begin_p;
        buf_p = (char *)(header_p + 1);
        footer_p = (struct log_entry_footer_t *)(buf_p + header_p->size);

        std_fprintf(chout_p, FSTR("%lu:%lu:"), header_p->number, header_p->time);
        std_fprintf(chout_p, level_as_string[(int)header_p->level]);
        std_fprintf(chout_p, FSTR(": "));

        format_fn = log_id_to_format_fn[header_p->id];
        format_fn(chout_p, buf_p);

        std_fprintf(chout_p, FSTR("\r\n"));

        begin_p = (char *)(footer_p + 1);

        if (begin_p >= log.end_p) {
            begin_p = &log.buffer[0];
        }
    }

    log_set_mode(old_mode);

    return (number_of_entries);
}
