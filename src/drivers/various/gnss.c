/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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

#if CONFIG_GNSS_DEBUG_LOG_MASK > -1
#    define DLOG(level, msg, ...)                                       \
    log_object_print(&(self_p)->log, LOG_ ## level, OSTR(msg), ##__VA_ARGS__)
#else
#    define DLOG(level, msg, ...)
#endif

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

    res = chan_read(self_p->chin_p,
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
            DLOG(DEBUG, "NMEA sentence start found.\r\n");
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
 * Process given NMEA GGA sentence.
 */
static int process_gga(struct gnss_driver_t *self_p)
{
    int res;
    struct nmea_sentence_gga_t *gga_p;
    long number_of_satellites;
    long latitude_degrees;
    long longitude_degrees;
    long altitude;

    gga_p = &self_p->nmea.decoded.gga;

    /* Latitude. */
    res = nmea_decode_position(&gga_p->latitude,
                               &latitude_degrees);

    if (res != 0) {
        return (res);
    }

    /* Longitude. */
    res = nmea_decode_position(&gga_p->longitude,
                               &longitude_degrees);

    if (res != 0) {
        return (res);
    }

    /* Number of satellites. */
    if (std_strtolb(gga_p->number_of_tracked_satellites_p,
                    &number_of_satellites,
                    10) == NULL) {
        return (-EPROTO);
    }

    /* Altitude. */
    if (std_strtodfp(gga_p->altitude.value_p, &altitude, 3) == NULL) {
        return (-EPROTO);
    }

    /* Save parsed data. */
    self_p->position.latitude_degrees = latitude_degrees;
    self_p->position.longitude_degrees = longitude_degrees;
    self_p->position.timestamp_p = &self_p->gga_timestamp;
    self_p->number_of_satellites = number_of_satellites;
    self_p->altitude = altitude;

    sys_uptime(&self_p->gga_timestamp);

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
    self_p->position.timestamp_p = &self_p->rmc_timestamp;
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
        DLOG(WARNING, "NMEA sentence decoding failed with %d.\r\n", res);
        return (res);
    }

    /* Process the decoded sentence. */
    switch (self_p->nmea.decoded.type) {

    case nmea_sentence_type_gga_t:
        res = process_gga(self_p);
        break;

    case nmea_sentence_type_rmc_t:
        res = process_rmc(self_p);
        break;

    default:
        DLOG(INFO,
             "Discarding NMEA sentence of type %d.\r\n",
             self_p->nmea.decoded.type);
        break;
    }

    return (0);
}

int gnss_module_init()
{
#if CONFIG_GNSS_DEBUG_LOG_MASK > -1
    log_module_init();
#endif

    return (0);
}

int gnss_init(struct gnss_driver_t *self_p,
              void *chin_p,
              void *chout_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(chin_p != NULL, EINVAL);
    ASSERTN(chout_p != NULL, EINVAL);

    self_p->chin_p = chin_p;
    self_p->chout_p = chout_p;
    self_p->rmc_timestamp.seconds = -1;
    self_p->gga_timestamp.seconds = -1;
    self_p->position.timestamp_p = &self_p->rmc_timestamp;
    self_p->nmea.input.size = 0;

#if CONFIG_GNSS_DEBUG_LOG_MASK > -1
    log_object_init(&self_p->log, "gnss", CONFIG_GNSS_DEBUG_LOG_MASK);
#endif

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

int gnss_write(struct gnss_driver_t *self_p,
               char *str_p)
{
    int res;
    char encoded[NMEA_SENTENCE_SIZE_MAX];
    struct nmea_sentence_t decoded;

    decoded.type = nmea_sentence_type_raw_t;
    decoded.raw.str_p = str_p;

    res = nmea_encode(&encoded[0], &decoded, sizeof(encoded));

    if (res < 0) {
        return (res);
    }

    if (chan_write(self_p->chout_p, &encoded[0], res) == res) {
        res = 0;
    } else {
        res = -EIO;
    }

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

#if CONFIG_FLOAT == 1

int gnss_get_position(struct gnss_driver_t *self_p,
                      float *latitude_p,
                      float *longitude_p)
{
    int res;

    res = get_data_age(self_p->position.timestamp_p);

    if (res >= 0) {
        *latitude_p = (self_p->position.latitude_degrees / 1000000.0f);
        *longitude_p = (self_p->position.longitude_degrees / 1000000.0f);
    }

    return (res);
}

int gnss_get_speed(struct gnss_driver_t *self_p,
                   float *speed_p)
{
    int res;

    res = get_data_age(&self_p->rmc_timestamp);

    if (res >= 0) {
        *speed_p = (self_p->speed / 1000.0f);
    }

    return (res);
}

#endif

int gnss_get_number_of_satellites(struct gnss_driver_t *self_p,
                                  int *number_of_satellites_p)
{
    int res;

    res = get_data_age(&self_p->gga_timestamp);

    if (res >= 0) {
        *number_of_satellites_p = self_p->number_of_satellites;
    }

    return (res);
}

#if CONFIG_FLOAT == 1

int gnss_get_altitude(struct gnss_driver_t *self_p,
                      float *altitude_p)
{
    int res;

    res = get_data_age(&self_p->gga_timestamp);

    if (res >= 0) {
        *altitude_p = (self_p->altitude / 1000.0f);
    }

    return (res);
}

int gnss_print(struct gnss_driver_t *self_p,
               void *chan_p)
{
    struct date_t date;
    float latitude;
    float longitude;
    float speed;
    int number_of_satellites;
    float altitude;
    int age;
    char buf[48];

    /* Date. */
    age = gnss_get_date(self_p, &date);

    if (age >= 0) {
        std_fprintf(chan_p,
                    OSTR("Date:                 "
                         "%02u:%02u:%02u %02u-%02u-%02u                   "
                         "(age: %d seconds)\r\n"),
                    date.hour,
                    date.minute,
                    date.second,
                    date.year,
                    date.month,
                    date.date,
                    age);
    } else {
        std_fprintf(chan_p, OSTR("Date:                 unavailable\r\n"));
    }

    /* Position. */
    age = gnss_get_position(self_p, &latitude, &longitude);

    if (age >= 0) {
        std_snprintf(&buf[0],
                     sizeof(buf),
                     OSTR("%f, %f degrees"),
                     latitude,
                     longitude);
        std_fprintf(chan_p,
                    OSTR("Position:             %-36s(age: %d seconds)\r\n"),
                    &buf[0],
                    age);
    } else {
        std_fprintf(chan_p, OSTR("Position:             unavailable\r\n"));
    }

    /* Speed. */
    age = gnss_get_speed(self_p, &speed);

    if (age >= 0) {
        std_snprintf(&buf[0], sizeof(buf), OSTR("%f m/s"), speed);
        std_fprintf(chan_p,
                    OSTR("Speed:                %-36s(age: %d seconds)\r\n"),
                    &buf[0],
                    age);
    } else {
        std_fprintf(chan_p, OSTR("Speed:                unavailable\r\n"));
    }

    /* Number of tracked satellites. */
    age = gnss_get_number_of_satellites(self_p,
                                        &number_of_satellites);

    if (age >= 0) {
        std_fprintf(chan_p,
                    OSTR("Number of satellites: %-2u                                  "
                         "(age: %d seconds)\r\n"),
                    number_of_satellites,
                    age);
    } else {
        std_fprintf(chan_p, OSTR("Number of satellites: unavailable\r\n"));
    }

    /* Altitude. */
    age = gnss_get_altitude(self_p, &altitude);

    if (age >= 0) {
        std_snprintf(&buf[0], sizeof(buf), OSTR("%3f m"), altitude);
        std_fprintf(chan_p,
                    OSTR("Altitude:             %-36s(age: %d seconds)\r\n"),
                    &buf[0],
                    age);
    } else {
        std_fprintf(chan_p, OSTR("Altitude:             unavailable\r\n"));
    }

    return (0);
}

#endif
