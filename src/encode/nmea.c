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

static uint8_t calculate_crc(char *buf_p, size_t size)
{
    size_t i;
    uint8_t crc;

    crc = 0;

    for (i = 0; i < size; i++) {
        crc ^= buf_p[i];
    }

    return (crc);
}

static int decode_triple(char *src_p,
                         int *v0_p,
                         int *v1_p,
                         int *v2_p)
{
    long v0;
    long v1;
    long v2;

    /* v2. */
    if (std_strtolb(&src_p[4], &v2, 10) != &src_p[6]) {
        return (-EPROTO);
    }

    src_p[4] = '\0';

    /* v1. */
    if (std_strtolb(&src_p[2], &v1, 10) != &src_p[4]) {
        return (-EPROTO);
    }

    src_p[2] = '\0';

    /* v0. */
    if (std_strtolb(&src_p[0], &v0, 10) != &src_p[2]) {
        return (-EPROTO);
    }

    *v0_p = v0;
    *v1_p = v1;
    *v2_p = v2;

    return (0);
}

/**
 * Decode given GGA sentence by validating and splitting the source
 * string into values.
 */
static ssize_t decode_gga(struct nmea_sentence_t *dst_p,
                          char *src_p)
{
    src_p += 6;

    /* Set the type. */
    dst_p->type = nmea_sentence_type_gga_t;

    /* Find all values in the sentence. */
    dst_p->gga.time_of_fix_p = strsep(&src_p, ",");
    dst_p->gga.latitude.angle_p = strsep(&src_p, ",");
    dst_p->gga.latitude.direction_p = strsep(&src_p, ",");
    dst_p->gga.longitude.angle_p = strsep(&src_p, ",");
    dst_p->gga.longitude.direction_p = strsep(&src_p, ",");
    dst_p->gga.fix_quality_p = strsep(&src_p, ",");
    dst_p->gga.number_of_tracked_satellites_p = strsep(&src_p, ",");
    dst_p->gga.horizontal_dilution_of_position_p = strsep(&src_p, ",");
    dst_p->gga.altitude.value_p = strsep(&src_p, ",");
    dst_p->gga.altitude.unit_p = strsep(&src_p, ",");
    dst_p->gga.height_of_geoid.value_p = strsep(&src_p, ",");
    dst_p->gga.height_of_geoid.unit_p = strsep(&src_p, ",");
    (void)strsep(&src_p, ",");
    (void)strsep(&src_p, "*");

    /* All values parsed successfully? */
    if (src_p == NULL) {
        return (-EPROTO);
    }

    return (0);
}

/**
 * Decode given GLL sentence by validating and splitting the source
 * string into values.
 */
static ssize_t decode_gll(struct nmea_sentence_t *dst_p,
                          char *src_p)
{
    src_p += 6;

    /* Set the type. */
    dst_p->type = nmea_sentence_type_gll_t;

    /* Find all values in the sentence. */
    dst_p->gll.latitude.angle_p = strsep(&src_p, ",");
    dst_p->gll.latitude.direction_p = strsep(&src_p, ",");
    dst_p->gll.longitude.angle_p = strsep(&src_p, ",");
    dst_p->gll.longitude.direction_p = strsep(&src_p, ",");
    dst_p->gll.time_of_fix_p = strsep(&src_p, ",");
    dst_p->gll.data_active_p = strsep(&src_p, ",");
    (void)strsep(&src_p, "*");

    /* All values parsed successfully? */
    if (src_p == NULL) {
        return (-EPROTO);
    }

    return (0);
}

/**
 * Decode given GSA sentence by validating and splitting the source
 * string into values.
 */
static ssize_t decode_gsa(struct nmea_sentence_t *dst_p,
                          char *src_p)
{
    int i;

    src_p += 6;

    /* Set the type. */
    dst_p->type = nmea_sentence_type_gsa_t;

    /* Find all values in the sentence. */
    dst_p->gsa.selection_p = strsep(&src_p, ",");
    dst_p->gsa.fix_p = strsep(&src_p, ",");

    for (i = 0; i < membersof(dst_p->gsa.prns); i++) {
        dst_p->gsa.prns[i] = strsep(&src_p, ",");
    }

    dst_p->gsa.pdop_p = strsep(&src_p, ",");
    dst_p->gsa.hdop_p = strsep(&src_p, ",");
    dst_p->gsa.vdop_p = strsep(&src_p, "*");

    /* All values parsed successfully? */
    if (src_p == NULL) {
        return (-EPROTO);
    }

    return (0);
}

/**
 * Decode given GSV sentence by validating and splitting the source
 * string into values.
 */
static ssize_t decode_gsv(struct nmea_sentence_t *dst_p,
                          char *src_p)
{
    int i;

    src_p += 6;

    /* Set the type. */
    dst_p->type = nmea_sentence_type_gsv_t;

    /* Find all values in the sentence. */
    dst_p->gsv.number_of_sentences_p = strsep(&src_p, ",");
    dst_p->gsv.sentence_p = strsep(&src_p, ",");
    dst_p->gsv.number_of_satellites_p = strsep(&src_p, ",");

    for (i = 0; i < membersof(dst_p->gsv.satellites); i++) {
        dst_p->gsv.satellites[i].prn_p = strsep(&src_p, ",");
        dst_p->gsv.satellites[i].elevation_p = strsep(&src_p, ",");
        dst_p->gsv.satellites[i].azimuth_p = strsep(&src_p, ",");

        if (i < membersof(dst_p->gsv.satellites) - 1) {
            dst_p->gsv.satellites[i].snr_p = strsep(&src_p, ",");
        } else {
            dst_p->gsv.satellites[i].snr_p = strsep(&src_p, "*");
        }
    }

    /* All values parsed successfully? */
    if (src_p == NULL) {
        return (-EPROTO);
    }

    return (0);
}

/**
 * Decode given VTG sentence by validating and splitting the source
 * string into values.
 */
static ssize_t decode_vtg(struct nmea_sentence_t *dst_p,
                          char *src_p)
{
    src_p += 6;

    /* Set the type. */
    dst_p->type = nmea_sentence_type_vtg_t;

    /* Find all values in the sentence. */
    dst_p->vtg.track_made_good_true.value_p = strsep(&src_p, ",");
    dst_p->vtg.track_made_good_true.relative_to_p = strsep(&src_p, ",");
    dst_p->vtg.track_made_good_magnetic.value_p = strsep(&src_p, ",");
    dst_p->vtg.track_made_good_magnetic.relative_to_p = strsep(&src_p, ",");
    dst_p->vtg.ground_speed_knots.value_p = strsep(&src_p, ",");
    dst_p->vtg.ground_speed_knots.unit_p = strsep(&src_p, ",");
    dst_p->vtg.ground_speed_kmph.value_p = strsep(&src_p, ",");
    dst_p->vtg.ground_speed_kmph.unit_p = strsep(&src_p, "*");

    /* All values parsed successfully? */
    if (src_p == NULL) {
        return (-EPROTO);
    }

    return (0);
}


/**
 * Decode given RMC sentence by validating and splitting the source
 * string into values.
 */
static ssize_t decode_rmc(struct nmea_sentence_t *dst_p,
                          char *src_p)
{
    src_p += 6;

    /* Set the type. */
    dst_p->type = nmea_sentence_type_rmc_t;

    /* Find all values in the sentence. */
    dst_p->rmc.time_of_fix_p = strsep(&src_p, ",");
    dst_p->rmc.status_p = strsep(&src_p, ",");
    dst_p->rmc.latitude.angle_p = strsep(&src_p, ",");
    dst_p->rmc.latitude.direction_p = strsep(&src_p, ",");
    dst_p->rmc.longitude.angle_p = strsep(&src_p, ",");
    dst_p->rmc.longitude.direction_p = strsep(&src_p, ",");
    dst_p->rmc.speed_knots_p = strsep(&src_p, ",");
    dst_p->rmc.track_angle_p = strsep(&src_p, ",");
    dst_p->rmc.date_p = strsep(&src_p, ",");
    dst_p->rmc.magnetic_variation.angle_p = strsep(&src_p, ",");
    dst_p->rmc.magnetic_variation.direction_p = strsep(&src_p, "*");

    /* All values parsed successfully? */
    if (src_p == NULL) {
        return (-EPROTO);
    }

    return (0);
}

ssize_t nmea_encode(char *dst_p,
                    struct nmea_sentence_t *src_p)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    ssize_t res;
    uint8_t crc;

    /* Header. */
    dst_p[0] = '$';

    /* Type specific contents. */
    switch (src_p->type) {

    default:
        return (-ENOSYS);
    }

    /* CRC. */
    dst_p[res + 1] = '*';
    crc = calculate_crc(&dst_p[1], res);
    std_sprintf(&dst_p[res + 2], FSTR("%02x"), crc);
    dst_p[res + 2] = toupper((int)dst_p[res + 2]);
    dst_p[res + 3] = toupper((int)dst_p[res + 3]);

    /* Line termination. */
    dst_p[res + 4] = '\r';
    dst_p[res + 5] = '\n';
    dst_p[res + 6] = '\0';

    return (res + 6);
}

ssize_t nmea_decode(struct nmea_sentence_t *dst_p,
                    char *src_p,
                    size_t size)
{
    ASSERTN(dst_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    ssize_t res;
    uint8_t actual_crc;
    long expected_crc;
    const char *end_p;

    /* Basic validation of the sentence. */
    if ((src_p[0] != '$')
        || (src_p[size - 5] != '*')
        || (src_p[size - 2] != '\r')
        || (src_p[size - 1] != '\n')
        || (src_p[size] != '\0')) {
        return (-EPROTO);
    }

    /* Check the CRC. */
    actual_crc = calculate_crc(&src_p[1], size - 6);
    end_p = std_strtolb(&src_p[size - 4], &expected_crc, 16);

    if (end_p != &src_p[size - 2]) {
        return (-EPROTO);
    }

    /* std_printf(FSTR("actual_crc: 0x%02x, expected_crc: 0x%02x\r\n"), */
    /*            actual_crc, */
    /*            expected_crc); */

    if (actual_crc != expected_crc) {
        return (-EPROTO);
    }

    /* Subtract $, CRC and line termination. Keep * to ease
       parsing. */
    size -= 5;

    /* Parse the sentence. */
    if (memcmp(&src_p[1], "GPGGA,", 6) == 0) {
        res = decode_gga(dst_p, &src_p[1]);
    } else if (memcmp(&src_p[1], "GPGLL,", 6) == 0) {
        res = decode_gll(dst_p, &src_p[1]);
    } else if (memcmp(&src_p[1], "GPGSA,", 6) == 0) {
        res = decode_gsa(dst_p, &src_p[1]);
    } else if (memcmp(&src_p[1], "GPGSV,", 6) == 0) {
        res = decode_gsv(dst_p, &src_p[1]);
    } else if (memcmp(&src_p[1], "GPRMC,", 6) == 0) {
        res = decode_rmc(dst_p, &src_p[1]);
    } else if (memcmp(&src_p[1], "GPVTG,", 6) == 0) {
        res = decode_vtg(dst_p, &src_p[1]);
    } else {
        res = -ENOSYS;
    }

    if (res != 0) {
        return (res);
    }

    return (0);
}

int nmea_decode_fix_time(char *src_p,
                         int *hour_p,
                         int *minute_p,
                         int *second_p)
{
    ASSERTN(src_p != NULL, EINVAL);
    ASSERTN(hour_p != NULL, EINVAL);
    ASSERTN(minute_p != NULL, EINVAL);
    ASSERTN(second_p != NULL, EINVAL);

    return (decode_triple(src_p, hour_p, minute_p, second_p));
}

int nmea_decode_date(char *src_p,
                     int *year_p,
                     int *month_p,
                     int *date_p)
{
    ASSERTN(src_p != NULL, EINVAL);
    ASSERTN(year_p != NULL, EINVAL);
    ASSERTN(month_p != NULL, EINVAL);
    ASSERTN(date_p != NULL, EINVAL);

    return (decode_triple(src_p, date_p, month_p, year_p));
}

int nmea_decode_position(struct nmea_position_t *src_p,
                         long *degrees_p)
{
    ASSERTN(src_p != NULL, EINVAL);
    ASSERTN(degrees_p != NULL, EINVAL);

    size_t pos;
    size_t minutes_pos;
    long degrees;
    long minutes;
    char *angle_p;
    char *direction_p;

    angle_p = src_p->angle_p;
    direction_p = src_p->direction_p;

    pos = strcspn(angle_p, ".");

    /* The string must contain a dot. */
    if (angle_p[pos] == '\0') {
        return (-EPROTO);
    }

    minutes_pos = (pos - 2);

    if (std_strtodfp(&angle_p[minutes_pos], &minutes, 6) == NULL) {
        return (-EPROTO);
    }

    /* Degrees. */
    angle_p[minutes_pos] = '\0';

    if (std_strtolb(&angle_p[0], &degrees, 10) != &angle_p[minutes_pos]) {
        return (-EPROTO);
    }

    *degrees_p = ((degrees * 1000000) + (minutes / 60));

    /* Positive sign for north and east, negtive for south and
       west. */
    if ((*direction_p == 'S') || (*direction_p == 'W')) {
        *degrees_p *= -1;
    }

    return (0);
}
