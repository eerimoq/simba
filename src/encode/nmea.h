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

#ifndef __ENCODE_NMEA_H__
#define __ENCODE_NMEA_H__

#include "simba.h"

#define NMEA_SENTENCE_SIZE_MAX                       (80 + 3)

#define NMEA_KNOTS_TO_METERS_PER_SECOND(knots) \
    DIV_ROUND((51444L * knots), 100000L)

struct nmea_position_t {
    char *angle_p;
    char *direction_p;
};

struct nmea_value_t {
    char *value_p;
    char *unit_p;
};

struct nmea_track_made_good_t {
    char *value_p;
    char *relative_to_p;
};

/**
 * Fix information.
 */
struct nmea_sentence_gga_t {
    char *time_of_fix_p;
    struct nmea_position_t latitude;
    struct nmea_position_t longitude;
    char *fix_quality_p;
    char *number_of_tracked_satellites_p;
    char *horizontal_dilution_of_position_p;
    struct nmea_value_t altitude;
    struct nmea_value_t height_of_geoid;
};

/**
 * Latitude and longitude data.
 */
struct nmea_sentence_gll_t {
    char *time_of_fix_p;
    struct nmea_position_t latitude;
    struct nmea_position_t longitude;
    char *data_active_p;
};

/**
 * Overall satellite data.
 */
struct nmea_sentence_gsa_t {
    char *selection_p;
    char *fix_p;
    char *prns[12];
    char *pdop_p;
    char *hdop_p;
    char *vdop_p;
};

/**
 * Detailed satellite data.
 */
struct nmea_sentence_gsv_t {
    char *number_of_sentences_p;
    char *sentence_p;
    char *number_of_satellites_p;
    struct {
        char *prn_p;
        char *elevation_p;
        char *azimuth_p;
        char *snr_p;
    } satellites[4];
};

/**
 * Recommended minimum data for GPS.
 */
struct nmea_sentence_rmc_t {
    char *time_of_fix_p;
    char *status_p;
    struct nmea_position_t latitude;
    struct nmea_position_t longitude;
    char *speed_knots_p;
    char *track_angle_p;
    char *date_p;
    struct nmea_position_t magnetic_variation;
};

/**
 * Track made good and speed over ground.
 */
struct nmea_sentence_vtg_t {
    struct nmea_track_made_good_t track_made_good_true;
    struct nmea_track_made_good_t track_made_good_magnetic;
    struct nmea_value_t ground_speed_knots;
    struct nmea_value_t ground_speed_kmph;
};

/**
 * Sentence types.
 */
enum nmea_sentence_type_t {
    nmea_sentence_type_gga_t = 0,
    nmea_sentence_type_gll_t,
    nmea_sentence_type_gsa_t,
    nmea_sentence_type_gsv_t,
    nmea_sentence_type_rmc_t,
    nmea_sentence_type_vtg_t
};

/**
 * A union of all sentences.
 */
struct nmea_sentence_t {
    enum nmea_sentence_type_t type;
    union {
        struct nmea_sentence_gga_t gga;
        struct nmea_sentence_gll_t gll;
        struct nmea_sentence_gsa_t gsa;
        struct nmea_sentence_gsv_t gsv;
        struct nmea_sentence_rmc_t rmc;
        struct nmea_sentence_vtg_t vtg;
    };
};

/**
 * Encode given NMEA sentence into given buffer.
 *
 * @param[out] dst_p Null-terminated encoded sentence.
 * @param[in] src_p Sentence to encode.
 *
 * @return Number of bytes in the encoded string, not including the
 *         null-termination, or negative error code.
 */
ssize_t nmea_encode(char *dst_p, struct nmea_sentence_t *src_p);

/**
 * Decode given NMEA sentence into given struct.
 *
 * @param[out] dst_p Decoded NMEA sentence.
 * @param[in,out] src_p Sentence to decode, starting with a dollar
 *                      sign and ending with <CR><LF>. This string is
 *                      modified by this function and the decoded
 *                      sentence has references to it.
 * @param[in] size Number of bytes in the sentence to decode, not
 *                 including the null-termination.
 *
 * @return zero(0) or negative error code.
 */
ssize_t nmea_decode(struct nmea_sentence_t *dst_p,
                    char *src_p,
                    size_t size);

/**
 * Decode given NMEA fix time ``hhmmss``. The output variables have
 * not been modified if the decoding failed.
 *
 * @param[in] src_p Fix time to decode.
 * @param[out] hour_p Decoded hour.
 * @param[out] minute_p Decoded minute.
 * @param[out] second_p Decoded second.
 *
 * @return zero(0) or negative error code.
 */
int nmea_decode_fix_time(char *src_p,
                         int *hour_p,
                         int *minute_p,
                         int *second_p);

/**
 * Decode given NMEA date ``ddmmyy``. The output variables have not
 * been modified if the decoding failed.
 *
 * @param[in] src_p Date to decode.
 * @param[out] year_p Decoded year.
 * @param[out] month_p Decoded month.
 * @param[out] date_p Decoded date.
 *
 * @return zero(0) or negative error code.
 */
int nmea_decode_date(char *src_p,
                     int *year_p,
                     int *month_p,
                     int *date_p);

/**
 * Decode given NMEA position angle ``d{2,3}mm.m+`` and direction
 * ``[NSEW]``, for example ``4703.324`` ``N``, which is decoded as 47
 * degrees, 3.324 minutes, or 47055400 microdegrees.
 *
 * The output variable has not been modified if the decoding failed.
 *
 * @param[in] src_p Position to decode.
 * @param[out] degrees_p Decoded position in microdegrees (millions
 *                       of degrees).
 *
 * @return zero(0) or negative error code.
 */
int nmea_decode_position(struct nmea_position_t *src_p,
                         long *degrees_p);

#endif
