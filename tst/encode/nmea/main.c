/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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

static int test_encode(struct harness_t *harness_p)
{
    struct nmea_sentence_t decoded;
    char buf[NMEA_SENTENCE_SIZE_MAX];

    decoded.type = nmea_sentence_type_gll_t;

    BTASSERTI(nmea_encode(&buf[0], &decoded), ==, -ENOSYS);

    return (0);
}

static int test_decode_bad_dollar(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "|GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERT(nmea_decode(&decoded, &encoded[0], size) == -EPROTO);

    return (0);
}

static int test_decode_bad_line_termination(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERT(nmea_decode(&decoded, &encoded[0], size) == -EPROTO);

    return (0);
}

static int test_decode_bad_asterix(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W|6A\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERT(nmea_decode(&decoded, &encoded[0], size) == -EPROTO);

    return (0);
}

static int test_decode_wrong_crc(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPRMC,,,,,,,,,,,*66\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -EPROTO);

    return (0);
}

static int test_decode_corrupt_crc(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPRMC,,,,,,,,,,,*r6\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -EPROTO);

    return (0);
}

static int test_decode_unsupported_type(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$FOO,,,,,,*46\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -ENOSYS);

    return (0);
}

static int test_decode_gga(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_gga_t);
    BTASSERTM(decoded.gga.time_of_fix_p, "123519", strlen("123519") + 1);
    BTASSERTM(decoded.gga.latitude.angle_p, "4807.038", strlen("4807.038") + 1);
    BTASSERTM(decoded.gga.latitude.direction_p, "N", strlen("N") + 1);
    BTASSERTM(decoded.gga.longitude.angle_p, "01131.000", strlen("01131.000") + 1);
    BTASSERTM(decoded.gga.longitude.direction_p, "E", strlen("E") + 1);
    BTASSERTM(decoded.gga.fix_quality_p, "1", strlen("1") + 1);
    BTASSERTM(decoded.gga.number_of_tracked_satellites_p, "08", strlen("08") + 1);
    BTASSERTM(decoded.gga.horizontal_dilution_of_position_p, "0.9", strlen("0.9") + 1);
    BTASSERTM(decoded.gga.altitude.value_p, "545.4", strlen("545.4") + 1);
    BTASSERTM(decoded.gga.altitude.unit_p, "M", strlen("M") + 1);
    BTASSERTM(decoded.gga.height_of_geoid.value_p, "46.9", strlen("46.9") + 1);
    BTASSERTM(decoded.gga.height_of_geoid.unit_p, "M", strlen("M") + 1);

    return (0);
}

static int test_decode_gga_empty(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGGA,,,,,,,,,,,,,,*56\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_gga_t);
    BTASSERTM(decoded.gga.time_of_fix_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.latitude.angle_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.latitude.direction_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.longitude.angle_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.longitude.direction_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.fix_quality_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.number_of_tracked_satellites_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.horizontal_dilution_of_position_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.altitude.value_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.altitude.unit_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.height_of_geoid.value_p, "", strlen("") + 1);
    BTASSERTM(decoded.gga.height_of_geoid.unit_p, "", strlen("") + 1);

    return (0);
}

static int test_decode_gga_short(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGGA,,,,,,,,,,,,,*7a\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -EPROTO);

    return (0);
}

static int test_decode_gll(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_gll_t);
    BTASSERTM(decoded.gll.time_of_fix_p, "225444", strlen("225444") + 1);
    BTASSERTM(decoded.gll.latitude.angle_p, "4916.45", strlen("4916.45") + 1);
    BTASSERTM(decoded.gll.latitude.direction_p, "N", strlen("N") + 1);
    BTASSERTM(decoded.gll.longitude.angle_p, "12311.12", strlen("12311.12") + 1);
    BTASSERTM(decoded.gll.longitude.direction_p, "W", strlen("W") + 1);
    BTASSERTM(decoded.gll.data_active_p, "A", strlen("A") + 1);

    return (0);
}

static int test_decode_gll_empty(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGLL,,,,,,,*7c\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_gll_t);
    BTASSERTM(decoded.gll.time_of_fix_p, "", strlen("") + 1);
    BTASSERTM(decoded.gll.latitude.angle_p, "", strlen("") + 1);
    BTASSERTM(decoded.gll.latitude.direction_p, "", strlen("") + 1);
    BTASSERTM(decoded.gll.longitude.angle_p, "", strlen("") + 1);
    BTASSERTM(decoded.gll.longitude.direction_p, "", strlen("") + 1);
    BTASSERTM(decoded.gll.data_active_p, "", strlen("") + 1);

    return (0);
}

static int test_decode_gll_short(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGLL,,,,,,*50\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -EPROTO);

    return (0);
}

static int test_decode_gsa(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_gsa_t);
    BTASSERTM(decoded.gsa.selection_p, "A", strlen("A") + 1);
    BTASSERTM(decoded.gsa.fix_p, "3", strlen("3") + 1);
    BTASSERTM(decoded.gsa.prns[0], "04", strlen("04") + 1);
    BTASSERTM(decoded.gsa.prns[1], "05", strlen("05") + 1);
    BTASSERTM(decoded.gsa.prns[2], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[3], "09", strlen("09") + 1);
    BTASSERTM(decoded.gsa.prns[4], "12", strlen("12") + 1);
    BTASSERTM(decoded.gsa.prns[5], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[6], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[7], "24", strlen("24") + 1);
    BTASSERTM(decoded.gsa.prns[8], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[9], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[10], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[11], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.pdop_p, "2.5", strlen("2.5") + 1);
    BTASSERTM(decoded.gsa.hdop_p, "1.3", strlen("1.3") + 1);
    BTASSERTM(decoded.gsa.vdop_p, "2.1", strlen("2.1") + 1);

    return (0);
}

static int test_decode_gsa_empty(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGSA,,,,,,,,,,,,,,,,,*6E\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_gsa_t);
    BTASSERTM(decoded.gsa.selection_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsa.fix_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[0], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[1], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[2], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[3], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[4], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[5], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[6], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[7], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[8], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[9], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[10], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.prns[11], "", strlen("") + 1);
    BTASSERTM(decoded.gsa.pdop_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsa.hdop_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsa.vdop_p, "", strlen("") + 1);

    return (0);
}

static int test_decode_gsa_short(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGSA,,,,,,,,,,,,,,,,*42\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -EPROTO);

    return (0);
}

static int test_decode_gsv(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_gsv_t);
    BTASSERTM(decoded.gsv.number_of_sentences_p, "2", strlen("2") + 1);
    BTASSERTM(decoded.gsv.sentence_p, "1", strlen("1") + 1);
    BTASSERTM(decoded.gsv.number_of_satellites_p, "08", strlen("08") + 1);
    BTASSERTM(decoded.gsv.satellites[0].prn_p, "01", strlen("01") + 1);
    BTASSERTM(decoded.gsv.satellites[0].elevation_p, "40", strlen("40") + 1);
    BTASSERTM(decoded.gsv.satellites[0].azimuth_p, "083", strlen("083") + 1);
    BTASSERTM(decoded.gsv.satellites[0].snr_p, "46", strlen("46") + 1);
    BTASSERTM(decoded.gsv.satellites[1].prn_p, "02", strlen("02") + 1);
    BTASSERTM(decoded.gsv.satellites[1].elevation_p, "17", strlen("17") + 1);
    BTASSERTM(decoded.gsv.satellites[1].azimuth_p, "308", strlen("308") + 1);
    BTASSERTM(decoded.gsv.satellites[1].snr_p, "41", strlen("41") + 1);
    BTASSERTM(decoded.gsv.satellites[2].prn_p, "12", strlen("12") + 1);
    BTASSERTM(decoded.gsv.satellites[2].elevation_p, "07", strlen("07") + 1);
    BTASSERTM(decoded.gsv.satellites[2].azimuth_p, "344", strlen("344") + 1);
    BTASSERTM(decoded.gsv.satellites[2].snr_p, "39", strlen("39") + 1);
    BTASSERTM(decoded.gsv.satellites[3].prn_p, "14", strlen("14") + 1);
    BTASSERTM(decoded.gsv.satellites[3].elevation_p, "22", strlen("22") + 1);
    BTASSERTM(decoded.gsv.satellites[3].azimuth_p, "228", strlen("228") + 1);
    BTASSERTM(decoded.gsv.satellites[3].snr_p, "45", strlen("45") + 1);


    return (0);
}

static int test_decode_gsv_empty(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGSV,,,,,,,,,,,,,,,,,,,*79\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_gsv_t);
    BTASSERTM(decoded.gsv.number_of_sentences_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.sentence_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.number_of_satellites_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[0].prn_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[0].elevation_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[0].azimuth_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[0].snr_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[1].prn_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[1].elevation_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[1].azimuth_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[1].snr_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[2].prn_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[2].elevation_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[2].azimuth_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[2].snr_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[3].prn_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[3].elevation_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[3].azimuth_p, "", strlen("") + 1);
    BTASSERTM(decoded.gsv.satellites[3].snr_p, "", strlen("") + 1);

    return (0);
}

static int test_decode_gsv_short(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPGSV,,,,,,,,,,,,,,,,,,*55\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -EPROTO);

    return (0);
}

static int test_decode_rmc(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_rmc_t);
    BTASSERTM(decoded.rmc.time_of_fix_p, "123519", strlen("123519") + 1);
    BTASSERTM(decoded.rmc.status_p, "A", strlen("A") + 1);
    BTASSERTM(decoded.rmc.latitude.angle_p, "4807.038", strlen("4807.038") + 1);
    BTASSERTM(decoded.rmc.latitude.direction_p, "N", strlen("N") + 1);
    BTASSERTM(decoded.rmc.longitude.angle_p, "01131.000", strlen("01131.000") + 1);
    BTASSERTM(decoded.rmc.longitude.direction_p, "E", strlen("E") + 1);
    BTASSERTM(decoded.rmc.speed_knots_p, "022.4", strlen("022.4") + 1);
    BTASSERTM(decoded.rmc.track_angle_p, "084.4", strlen("084.4") + 1);
    BTASSERTM(decoded.rmc.date_p, "230394", strlen("230394") + 1);
    BTASSERTM(decoded.rmc.magnetic_variation.angle_p, "003.1", strlen("003.1") + 1);
    BTASSERTM(decoded.rmc.magnetic_variation.direction_p, "W", strlen("W") + 1);

    return (0);
}

static int test_decode_rmc_empty(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPRMC,,,,,,,,,,,*67\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_rmc_t);
    BTASSERTM(decoded.rmc.time_of_fix_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.status_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.latitude.angle_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.latitude.direction_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.longitude.angle_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.longitude.direction_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.speed_knots_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.track_angle_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.date_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.magnetic_variation.angle_p, "", strlen("") + 1);
    BTASSERTM(decoded.rmc.magnetic_variation.direction_p, "", strlen("") + 1);

    return (0);
}

static int test_decode_rmc_short(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPRMC,,,,,,,,,,*4B\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -EPROTO);

    return (0);
}

static int test_decode_vtg(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_vtg_t);
    BTASSERTM(decoded.vtg.track_made_good_true.value_p, "054.7", strlen("054.7") + 1);
    BTASSERTM(decoded.vtg.track_made_good_true.relative_to_p, "T", strlen("T") + 1);
    BTASSERTM(decoded.vtg.track_made_good_magnetic.value_p, "034.4", strlen("034.4") + 1);
    BTASSERTM(decoded.vtg.track_made_good_magnetic.relative_to_p, "M", strlen("M") + 1);
    BTASSERTM(decoded.vtg.ground_speed_knots.value_p, "005.5", strlen("005.5") + 1);
    BTASSERTM(decoded.vtg.ground_speed_knots.unit_p, "N", strlen("N") + 1);
    BTASSERTM(decoded.vtg.ground_speed_kmph.value_p, "010.2", strlen("010.2") + 1);
    BTASSERTM(decoded.vtg.ground_speed_kmph.unit_p, "K", strlen("K") + 1);

    return (0);
}

static int test_decode_vtg_empty(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPVTG,,,,,,,,*52\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, 0);

    BTASSERTI(decoded.type, ==, nmea_sentence_type_vtg_t);
    BTASSERTM(decoded.vtg.track_made_good_true.value_p, "", strlen("") + 1);
    BTASSERTM(decoded.vtg.track_made_good_true.relative_to_p, "", strlen("") + 1);
    BTASSERTM(decoded.vtg.track_made_good_magnetic.value_p, "", strlen("") + 1);
    BTASSERTM(decoded.vtg.track_made_good_magnetic.relative_to_p, "", strlen("") + 1);
    BTASSERTM(decoded.vtg.ground_speed_knots.value_p, "", strlen("") + 1);
    BTASSERTM(decoded.vtg.ground_speed_knots.unit_p, "", strlen("") + 1);
    BTASSERTM(decoded.vtg.ground_speed_kmph.value_p, "", strlen("") + 1);
    BTASSERTM(decoded.vtg.ground_speed_kmph.unit_p, "", strlen("") + 1);

    return (0);
}

static int test_decode_vtg_short(struct harness_t *harness_p)
{
    size_t size;
    char encoded[] =
        "$GPVTG,,,,,,,*7e\r\n";
    struct nmea_sentence_t decoded;

    size = strlen(encoded);
    BTASSERTI(nmea_decode(&decoded, &encoded[0], size), ==, -EPROTO);

    return (0);
}

static int test_decode_fix_time(struct harness_t *harness_p)
{
    char early_morning[] = "052859";
    int hour;
    int minute;
    int second;

    BTASSERTI(nmea_decode_fix_time(&early_morning[0],
                                   &hour,
                                   &minute,
                                   &second), ==, 0);
    BTASSERTI(hour, ==, 5);
    BTASSERTI(minute, ==, 28);
    BTASSERTI(second, ==, 59);

    return (0);
}

static int test_decode_date(struct harness_t *harness_p)
{
    char late_spring[] = "140517";
    int year;
    int month;
    int date;

    BTASSERTI(nmea_decode_date(&late_spring[0],
                               &year,
                               &month,
                               &date), ==, 0);
    BTASSERTI(year, ==, 17);
    BTASSERTI(month, ==, 5);
    BTASSERTI(date, ==, 14);

    return (0);
}

static int test_decode_position(struct harness_t *harness_p)
{
    long degrees;

    /* Position 1. */
    char angle_1[] = "4703.324";
    struct nmea_position_t position_1 = { &angle_1[0], "N" };
    BTASSERTI(nmea_decode_position(&position_1, &degrees), ==, 0);
    BTASSERTI(degrees, ==, 47000000 + (3324000 / 60));

    /* Position 2. */
    char angle_2[] = "10012.0245";
    struct nmea_position_t position_2 = { &angle_2[0], "S" };
    BTASSERTI(nmea_decode_position(&position_2, &degrees), ==, 0);
    BTASSERTI(degrees, ==, -(100000000 + (12024500 / 60)));

    /* Position 3. */
    char angle_3[] = "10012.0000019";
    struct nmea_position_t position_3 = { &angle_3[0], "E" };
    BTASSERTI(nmea_decode_position(&position_3, &degrees), ==, 0);
    BTASSERTI(degrees, ==, 100000000 + (12000001 / 60));

    /* Position 4. */
    char angle_4[] = "0000.00019";
    struct nmea_position_t position_4 = { &angle_4[0], "W" };
    BTASSERTI(nmea_decode_position(&position_4, &degrees), ==, 0);
    BTASSERTI(degrees, ==, -(190 / 60));

    /* Position 5. */
    char angle_5[] = "35959.999999";
    struct nmea_position_t position_5 = { &angle_5[0], "N" };
    BTASSERTI(nmea_decode_position(&position_5, &degrees), ==, 0);
    BTASSERTI(degrees, ==, 359000000 + (59999999 / 60));

    /* Position 6. */
    char angle_6[] = "36000.0";
    struct nmea_position_t position_6 = { &angle_6[0], "N" };
    BTASSERTI(nmea_decode_position(&position_6, &degrees), ==, 0);
    BTASSERTI(degrees, ==, 360000000);

    /* Position 7. */
    char angle_7[] = "10012";
    struct nmea_position_t position_7 = { &angle_7[0], "N" };
    BTASSERTI(nmea_decode_position(&position_7, &degrees), ==, -EPROTO);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_encode, "test_encode" },
        { test_decode_bad_dollar, "test_decode_bad_dollar" },
        { test_decode_bad_line_termination, "test_decode_bad_line_termination" },
        { test_decode_bad_asterix, "test_decode_bad_asterix" },
        { test_decode_wrong_crc, "test_decode_wrong_crc" },
        { test_decode_corrupt_crc, "test_decode_corrupt_crc" },
        { test_decode_unsupported_type, "test_decode_unsupported_type" },
        { test_decode_gga, "test_decode_gga" },
        { test_decode_gga_empty, "test_decode_gga_empty" },
        { test_decode_gga_short, "test_decode_gga_short" },
        { test_decode_gll, "test_decode_gll" },
        { test_decode_gll_empty, "test_decode_gll_empty" },
        { test_decode_gll_short, "test_decode_gll_short" },
        { test_decode_gsa, "test_decode_gsa" },
        { test_decode_gsa_empty, "test_decode_gsa_empty" },
        { test_decode_gsa_short, "test_decode_gsa_short" },
        { test_decode_gsv, "test_decode_gsv" },
        { test_decode_gsv_empty, "test_decode_gsv_empty" },
        { test_decode_gsv_short, "test_decode_gsv_short" },
        { test_decode_rmc, "test_decode_rmc" },
        { test_decode_rmc_empty, "test_decode_rmc_empty" },
        { test_decode_rmc_short, "test_decode_rmc_short" },
        { test_decode_vtg, "test_decode_vtg" },
        { test_decode_vtg_empty, "test_decode_vtg_empty" },
        { test_decode_vtg_short, "test_decode_vtg_short" },
        { test_decode_fix_time, "test_decode_fix_time" },
        { test_decode_date, "test_decode_date" },
        { test_decode_position, "test_decode_position" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
