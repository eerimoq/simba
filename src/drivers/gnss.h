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

#ifndef __DRIVERS_GNSS_H__
#define __DRIVERS_GNSS_H__

#include "simba.h"

struct gnss_driver_t {
    void *transport_p;
    struct time_t rmc_timestamp;
    struct time_t gga_timestamp;
    struct date_t date;
    struct {
        long latitude_degrees;
        long longitude_degrees;
        struct time_t *timestamp_p;
    } position;
    long speed;
    int number_of_satellites;
    long altitude;
    struct {
        struct {
            char buf[NMEA_SENTENCE_SIZE_MAX];
            size_t size;
        } input;
        struct nmea_sentence_t decoded;
    } nmea;
#if CONFIG_GNSS_DEBUG_LOG_MASK > -1
    struct log_object_t log;
#endif
};

/**
 * Initialize the GNSS module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int gnss_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[out] self_p Driver object to be initialized.
 * @param[in] transport_p Transport channel.
 *
 * @return zero(0) or negative error code.
 */
int gnss_init(struct gnss_driver_t *self_p,
              void *transport_p);

/**
 * Update the GNSS driver state by reading and parsing a NMEA sentence
 * from the transport channel.
 *
 * NOTE: NMEA sentences will be lost if this function is called too
 *       seldom (due to transport channel input overrun).
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) if an NEMA sentence was read, otherwise negative
 *         error code.
 */
int gnss_read(struct gnss_driver_t *self_p);

/**
 * Encode an NMEA sentence of given string by prepending a ``$`` and
 * appending a CRC and line termination, and write the sentence to the
 * transport channel.
 *
 * For example, the string ``GPGLL,4916.45,N,12311.12,W,225444,A,`` is
 * encoded to ``$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n`` and
 * then written to the transport channel.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] str_p String to write.
 *
 * @return zero(0) if the NEMA sentence was written to the transport
 *         channel, otherwise negative error code.
 */
int gnss_write(struct gnss_driver_t *self_p,
               char *str_p);

/**
 * Get most recently received date.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] date_p Current date.
 *
 * @return Date age in seconds or negative error code.
 */
int gnss_get_date(struct gnss_driver_t *self_p,
                  struct date_t *date_p);

/**
 * Get most recently received position.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] latitude_p Current latitude in microdegrees. Positive
 *                        number for north, negative south.
 * @param[out] longitude_p Current longitude in microdegrees. Positive
 *                         number for east, negative for west.
 *
 * @return Position age in seconds or negative error code.
 */
int gnss_get_position(struct gnss_driver_t *self_p,
                      long *latitude_p,
                      long *longitude_p);

/**
 * Get most recently received speed.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] speed_p Current speed in millimeters per second.
 *
 * @return Speed age in seconds or negative error code.
 */
int gnss_get_speed(struct gnss_driver_t *self_p,
                   long *speed_p);

/**
 * Get most recently received number of tracked satellites.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] number_of_satellites_p Current number of tracked
 *                                    satellites.
 *
 * @return Number of tracked satellites age in seconds or negative
 *         error code.
 */
int gnss_get_number_of_satellites(struct gnss_driver_t *self_p,
                                  int *number_of_satellites_p);

/**
 * Get most recently received altitude.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] altitude_p Current altitude in millimeters.
 *
 * @return Altitude age in seconds or negative error code.
 */
int gnss_get_altitude(struct gnss_driver_t *self_p,
                      long *altitude_p);

/**
 * Print the driver state as a human readable string to given channel.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] chan_p Channel to print to.
 *
 * @return zero(0) or negative error code.
 */
int gnss_print(struct gnss_driver_t *self_p,
               void *chan_p);

#endif
