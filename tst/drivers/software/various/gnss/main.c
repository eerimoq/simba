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
#include "sync/chan_mock.h"

static struct gnss_driver_t gnss;
static struct chan_t transport;
static struct queue_t queue;
static uint8_t buf[768];

static int test_init(void)
{
    BTASSERT(queue_init(&queue, &buf[0], sizeof(buf)) == 0);

    BTASSERT(gnss_module_init() == 0);
    BTASSERT(gnss_module_init() == 0);

    BTASSERT(chan_init(&transport,
                       chan_read_null,
                       chan_write_null,
                       chan_size_null) == 0);
    BTASSERT(gnss_init(&gnss, &transport, &transport) == 0);

    return (0);
}

static int test_get_no_data(void)
{
    struct date_t date;
    float latitude;
    float longitude;
    float speed;
    int number_of_satellites;
    float altitude;

    /* Date should be unmodified. */
    date.year = 1200;
    BTASSERTI(gnss_get_date(&gnss, &date), ==, -ENODATA);
    BTASSERTI(date.year, ==, 1200);

    /* Position should be unmodified. */
    latitude = 23;
    longitude = 24;
    BTASSERTI(gnss_get_position(&gnss, &latitude, &longitude), ==, -ENODATA);
    BTASSERTI(latitude, ==, 23);
    BTASSERTI(longitude, ==, 24);

    /* Speed should be unmodified. */
    speed = 654;
    BTASSERTI(gnss_get_speed(&gnss, &speed), ==, -ENODATA);
    BTASSERTI(speed, ==, 654);

    /* Number of satellites. */
    number_of_satellites = 654;
    BTASSERTI(gnss_get_number_of_satellites(
                  &gnss,
                  &number_of_satellites), ==, -ENODATA);
    BTASSERTI(number_of_satellites, ==, 654);

    /* Altitude. */
    altitude = 654;
    BTASSERTI(gnss_get_altitude(&gnss, &altitude), ==, -ENODATA);
    BTASSERTI(altitude, ==, 654);

    return (0);
}

static int test_print_no_data(void)
{
    BTASSERTI(gnss_print(&gnss, &queue), ==, 0);
    BTASSERTI(harness_expect(&queue,
                             "Date:                 unavailable\r\n"
                             "Position:             unavailable\r\n"
                             "Speed:                unavailable\r\n"
                             "Number of satellites: unavailable\r\n"
                             "Altitude:             unavailable\r\n",
                             NULL), ==, 175);

    return (0);
}

static int test_read_rmc(void)
{
    int i;
    char sentence[] =
        "$GPRMC,123519,A,4807.038,N,01131.000,W,022.4,084.4,230394,003.1,W*78\r\n";
    struct date_t date;
    float latitude;
    float longitude;
    float speed;

    for (i = 0; i < strlen(sentence); i++) {
        mock_write_chan_read(&sentence[i], 1, 1);
    }

    BTASSERTI(gnss_read(&gnss), ==, 0);

    /* Date. */
    date.year = 1200;
    BTASSERTI(gnss_get_date(&gnss, &date), >=, 0);
    BTASSERTI(date.year, ==, 94);
    BTASSERTI(date.month, ==, 3);
    BTASSERTI(date.date, ==, 23);
    BTASSERTI(date.hour, ==, 12);
    BTASSERTI(date.minute, ==, 35);
    BTASSERTI(date.second, ==, 19);

    /* Position. */
    latitude = 23;
    longitude = 24;
    BTASSERTI(gnss_get_position(&gnss, &latitude, &longitude), >=, 0);
    BTASSERTI(latitude, ==, 48 + (7.038f / 60));
    BTASSERTI(longitude, ==, -(11 + (31.0f / 60)));

    /* Speed. */
    speed = 654;
    BTASSERTI(gnss_get_speed(&gnss, &speed), >=, 0);
    BTASSERTI(speed, ==, 11.523f);

    BTASSERTI(gnss_print(&gnss, &queue), ==, 0);
    BTASSERTI(harness_expect(
                  &queue,
                  "Date:                 12:35:19 94-03-23                   (age: 0 seconds)\r\n"
                  "Position:             48.117298, -11.516666 degrees       (age: 0 seconds)\r\n"
                  "Speed:                11.522999 m/s                       (age: 0 seconds)\r\n"
                  "Number of satellites: unavailable\r\n"
                  "Altitude:             unavailable\r\n",
                  NULL), ==, 298);

    return (0);
}

static int test_read_gga_glonass(void)
{
    int i;
    char sentence[] =
        "$GNGGA,123520,4907.038,N,01031.000,E,1,08,0.9,545.4,M,46.9,M,,*53\r\n";
    struct date_t date;
    float latitude;
    float longitude;
    float speed;
    int number_of_satellites;
    float altitude;

    for (i = 0; i < strlen(sentence); i++) {
        mock_write_chan_read(&sentence[i], 1, 1);
    }

    BTASSERTI(gnss_read(&gnss), ==, 0);

    /* Date. */
    date.year = 1200;
    BTASSERTI(gnss_get_date(&gnss, &date), >=, 0);
    BTASSERTI(date.year, ==, 94);
    BTASSERTI(date.month, ==, 3);
    BTASSERTI(date.date, ==, 23);
    BTASSERTI(date.hour, ==, 12);
    BTASSERTI(date.minute, ==, 35);
    BTASSERTI(date.second, ==, 19);

    /* Position. */
    latitude = 23;
    longitude = 24;
    BTASSERTI(gnss_get_position(&gnss, &latitude, &longitude), >=, 0);
    BTASSERTI(latitude, ==, 49 + (7.038f / 60));
    BTASSERTI(longitude, ==, 10 + (31.0f / 60));

    /* Speed. */
    speed = 654;
    BTASSERTI(gnss_get_speed(&gnss, &speed), >=, 0);
    BTASSERTI(speed, ==, 11.523f);

    /* Number of satellites. */
    number_of_satellites = 654;
    BTASSERTI(gnss_get_number_of_satellites(&gnss,
                                            &number_of_satellites), >=, 0);
    BTASSERTI(number_of_satellites, ==, 8);

    /* Altitude. */
    altitude = 654;
    BTASSERTI(gnss_get_altitude(&gnss, &altitude), >=, 0);
    BTASSERTI(altitude, ==, 545.4f);

    BTASSERTI(gnss_print(&gnss, &queue), ==, 0);
    BTASSERTI(harness_expect(
                  &queue,
                  "Date:                 12:35:19 94-03-23                   (age: 0 seconds)\r\n"
                  "Position:             49.117298, 10.516666 degrees        (age: 0 seconds)\r\n"
                  "Speed:                11.522999 m/s                       (age: 0 seconds)\r\n"
                  "Number of satellites: 8                                   (age: 0 seconds)\r\n"
                  "Altitude:             545.400024 m                        (age: 0 seconds)\r\n",
                  NULL), ==, 380);

    return (0);
}

static int test_read_read_failed(void)
{
    mock_write_chan_read(NULL, 1, -EIO);
    BTASSERTI(gnss_read(&gnss), ==, -EIO);

    mock_write_chan_read(NULL, 1, -EIO);
    BTASSERTI(gnss_read(&gnss), ==, -EIO);

    return (0);
}

static int test_read_sentence_too_long(void)
{
    int i;
    char byte;

    byte = '$';

    for (i = 0; i < NMEA_SENTENCE_SIZE_MAX; i++) {
        mock_write_chan_read(&byte, 1, 1);
    }

    BTASSERTI(gnss_read(&gnss), ==, -ENOMEM);

    return (0);
}

static int test_read_start_not_first(void)
{
    int i;
    char sentence[] =
        "\r\n$GPRMC,123519,A,4807.038,N,01131.000,W,022.4,084.4,230394,"
        "003.1,W*78\r\n";

    for (i = 0; i < strlen(sentence); i++) {
        mock_write_chan_read(&sentence[i], 1, 1);
    }

    BTASSERTI(gnss_read(&gnss), ==, 0);

    return (0);
}

static int test_read_unsupported_sentence(void)
{
    int i;
    char sentence[] = "$GPFOO,BAR*2C\r\n";

    for (i = 0; i < strlen(sentence); i++) {
        mock_write_chan_read(&sentence[i], 1, 1);
    }

    BTASSERTI(gnss_read(&gnss), ==, 0);

    return (0);
}

static int test_read_wrong_crc(void)
{
    int i;
    char sentence[] = "$GPFOO,BAR*2D\r\n";

    for (i = 0; i < strlen(sentence); i++) {
        mock_write_chan_read(&sentence[i], 1, 1);
    }

    BTASSERTI(gnss_read(&gnss), ==, -EPROTO);

    return (0);
}

static int test_write(void)
{
    mock_write_chan_write("$GPFOO,BAR*2C\r\n", 15, 15);
    BTASSERTI(gnss_write(&gnss, "GPFOO,BAR"), ==, 0);

    return (0);
}

int main()
{
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_get_no_data, "test_get_no_data" },
        { test_print_no_data, "test_print_no_data" },
        { test_read_rmc, "test_read_rmc" },
        { test_read_gga_glonass, "test_read_gga_glonass" },
        { test_read_read_failed, "test_read_read_failed" },
        { test_read_sentence_too_long, "test_read_sentence_too_long" },
        { test_read_start_not_first, "test_read_start_not_first" },
        { test_read_unsupported_sentence, "test_read_unsupported_sentence" },
        { test_read_wrong_crc, "test_read_wrong_crc" },
        { test_write, "test_write" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(harness_testcases);

    return (0);
}
