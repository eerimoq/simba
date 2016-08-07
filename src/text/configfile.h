/**
 * @file text/configfile.h
 * @version 5.0.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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
