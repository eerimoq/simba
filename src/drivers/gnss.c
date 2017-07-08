/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

static int get_data_age(struct time_t *timestamp_p)
{
    struct time_t uptime;

    if (timestamp_p->seconds == -1) {
        return (-ENODATA);
    }

    sys_uptime(&uptime);

    return (uptime.seconds - timestamp_p->seconds);
}

/**
 * Read a single byte from the transport channel.
 *
 * @return zero(0) or negative error code.
 */
static int read_byte(struct gnss_driver_t *self_p,
                     char *byte_p)
{
    int res;

    res = chan_read(self_p->transport_p,
                    byte_p,
                    sizeof(*byte_p));

    if (res == 0) {
        res = -EIO;
    }

    return (res);
}

/**
 * Find the start of an NMEA sentence (dollar sign).
 */
static int find_sentence_start(struct gnss_driver_t *self_p)
{
    int res;
    char byte;

    while (1) {
        res = read_byte(self_p, &byte);

        if (res != sizeof(byte)) {
            return (res);
        }

        if (byte == '$') {
            self_p->nmea.input.buf[0] = '$';
            self_p->nmea.input.size = 1;
            break;
        }
    }

    return (0);
}

/**
 * Read the rest of the sentence (until linefeed).
 */
static int read_until_sentence_end(struct gnss_driver_t *self_p)
{
    int res;
    char byte;

    while (1) {
        res = read_byte(self_p, &byte);

        if (res != sizeof(byte)) {
            return (res);
        }

        /* Space for the read character and a null-termination. */
        if ((self_p->nmea.input.size + 1) == sizeof(self_p->nmea.input.buf)) {
            self_p->nmea.input.size = 0;
            return (-ENOMEM);
        }

        self_p->nmea.input.buf[self_p->nmea.input.size] = byte;
        self_p->nmea.input.size++;

        if (byte == '\n') {
            self_p->nmea.input.buf[self_p->nmea.input.size] = '\0';
            break;
        }
    }

    return (0);
}

/**
 * Get the date from given NMEA RMC sentence.
 */
static int process_rmc_date(struct nmea_sentence_rmc_t *rmc_p,
                            struct date_t *date_p)
{
    int res;

    /* Time. */
    res = nmea_decode_fix_time(rmc_p->time_of_fix_p,
                               &date_p->hour,
                               &date_p->minute,
                               &date_p->second);

    if (res != 0) {
        return (res);
    }

    /* Day is not available. */
    date_p->day = -1;

    /* Date. */
    res = nmea_decode_date(rmc_p->date_p,
                           &date_p->year,
                           &date_p->month,
                           &date_p->date);

    if (res != 0) {
        return (res);
    }

    return (0);
}

/**
 * Process given NMEA RMC sentence.
 */
static int process_rmc(struct gnss_driver_t *self_p)
{
    int res;
    struct nmea_sentence_rmc_t *rmc_p;
    struct date_t date;
    long latitude_degrees;
    long longitude_degrees;
    long speed;

    rmc_p = &self_p->nmea.decoded.rmc;

    /* Date. */
    res = process_rmc_date(rmc_p, &date);

    if (res != 0) {
        return (res);
    }

    /* Latitude. */
    res = nmea_decode_position(&rmc_p->latitude,
                               &latitude_degrees);

    if (res != 0) {
        return (res);
    }

    /* Longitude. */
    res = nmea_decode_position(&rmc_p->longitude,
                               &longitude_degrees);

    if (res != 0) {
        return (res);
    }

    /* Speed. */
    if (std_strtodfp(rmc_p->speed_knots_p, &speed, 3) == NULL) {
        return (-EPROTO);
    }

    /* Save parsed data. */
    self_p->date = date;
    self_p->position.latitude_degrees = latitude_degrees;
    self_p->position.longitude_degrees = longitude_degrees;
    self_p->speed = NMEA_KNOTS_TO_METERS_PER_SECOND(speed);

    sys_uptime(&self_p->rmc_timestamp);

    return (0);
}

/**
 * Process the read sentence.
 */
static int process_sentence(struct gnss_driver_t *self_p)
{
    ssize_t res;

    /* Decode the sentence. */
    res = nmea_decode(&self_p->nmea.decoded,
                      &self_p->nmea.input.buf[0],
                      self_p->nmea.input.size);

    if (res != 0) {
        return (res);
    }

    /* Process the decoded sentence. */
    switch (self_p->nmea.decoded.type) {

    case nmea_sentence_type_rmc_t:
        res = process_rmc(self_p);
        break;

    default:
        break;
    }

    return (0);
}

int gnss_module_init()
{
    return (0);
}

int gnss_init(struct gnss_driver_t *self_p,
              void *transport_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(transport_p != NULL, EINVAL);

    self_p->transport_p = transport_p;
    self_p->rmc_timestamp.seconds = -1;
    self_p->rmc_timestamp.nanoseconds = 0;
    self_p->nmea.input.size = 0;

    return (0);
}

int gnss_read(struct gnss_driver_t *self_p)
{
    int res;

    /* Find the beginning of an NMEA sentence if the sentence buffer
       is empty. */
    if (self_p->nmea.input.size == 0) {
        res = find_sentence_start(self_p);

        if (res != 0) {
            return (res);
        }
    }

    /* Read the rest of the sentence. */
    res = read_until_sentence_end(self_p);

    if (res != 0) {
        return (res);
    }

    /* Process the read sentence. */
    res = process_sentence(self_p);

    /* Expect a new sentence. */
    self_p->nmea.input.size = 0;

    return (res);
}

int gnss_get_date(struct gnss_driver_t *self_p,
                  struct date_t *date_p)
{
    int res;

    res = get_data_age(&self_p->rmc_timestamp);

    if (res >= 0) {
        *date_p = self_p->date;
    }

    return (res);
}

int gnss_get_position(struct gnss_driver_t *self_p,
                      long *latitude_p,
                      long *longitude_p)
{
    int res;

    res = get_data_age(&self_p->rmc_timestamp);

    if (res >= 0) {
        *latitude_p = self_p->position.latitude_degrees;
        *longitude_p = self_p->position.longitude_degrees;
    }

    return (res);
}

int gnss_get_speed(struct gnss_driver_t *self_p,
                   long *speed_p)
{
    int res;

    res = get_data_age(&self_p->rmc_timestamp);

    if (res >= 0) {
        *speed_p = self_p->speed;
    }

    return (res);
}
