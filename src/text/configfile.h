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

#ifndef __TEXT_CONFIGFILE_H__
#define __TEXT_CONFIGFILE_H__

#include "simba.h"

struct configfile_t {
    char *buf_p;
    size_t size;
};

/**
 * Initialize given configuration file object.
 *
 * @param[in,out] self_p Object to initialize.
 * @param[in] buf_p Configuration file contents as a NULL terminated
 *                  string.
 * @param[in] size Size of the configuration file contents.
 *
 * @return zero(0) or negative error code.
 */
int configfile_init(struct configfile_t *self_p,
                    char *buf_p,
                    size_t size);

/**
 * Set the value of given property in given section.
 *
 * @param[in] self_p Initialized parser.
 * @param[in] section_p Section to set the property from.
 * @param[in] property_p Property to set the value for.
 * @param[in] value_p NULL terminated value to set.
 *
 * @return zero(0) or negative error code.
 */
int configfile_set(struct configfile_t *self_p,
                   const char *section_p,
                   const char *property_p,
                   const char *value_p);

/**
 * Get the value of given property in given section.
 *
 * @param[in] self_p Initialized parser.
 * @param[in] section_p Section to get the property from.
 * @param[in] property_p Property to get the value for.
 * @param[out] value_p Value of given property in given section.
 * @param[in] size Size of the value buffer.
 *
 * @return Value pointer or NULL on failure.
 */
char *configfile_get(struct configfile_t *self_p,
                     const char *section_p,
                     const char *property_p,
                     char *value_p,
                     int length);

/**
 * Get the value of given property in given section, converted to an
 * integer.
 *
 * @param[in] self_p Initialized parser.
 * @param[in] section_p Section to get the property from.
 * @param[in] property_p Property to get the value for.
 * @param[out] value_p Value of given property in given section.
 *
 * @return zero(0) or negative error code.
 */
int configfile_get_long(struct configfile_t *self_p,
                        const char *section_p,
                        const char *property_p,
                        long *value_p);

/**
 * Get the value of given property in given section, converted to a
 * float.
 *
 * @param[in] self_p Initialized parser.
 * @param[in] section_p Section to get the property from.
 * @param[in] property_p Property to get the value for.
 * @param[out] value_p Value of given property in given section.
 *
 * @return zero(0) or negative error code.
 */
int configfile_get_float(struct configfile_t *self_p,
                        const char *section_p,
                        const char *property_p,
                        float *value_p);

#endif
