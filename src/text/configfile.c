/**
 * @file configfile.c
 * @version 7.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

int configfile_init(struct configfile_t *self_p,
                    char *buf_p,
                    size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    self_p->buf_p = buf_p;
    self_p->size = size;

    return (0);
}

int configfile_set(struct configfile_t *self_p,
                   const char *section_p,
                   const char *property_p,
                   const char *value_p)
{
    return (-1);
}

char *configfile_get(struct configfile_t *self_p,
                     const char *section_p,
                     const char *property_p,
                     char *value_p,
                     int length)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(section_p != NULL, EINVAL);
    ASSERTN(property_p != NULL, EINVAL);
    ASSERTN(value_p != NULL, EINVAL);
    ASSERTN(length > 0, EINVAL);

    int in_correct_section;
    int section_length;
    int property_length;
    int value_length;
    int first_length;
    int section_end_found;
    char *buf_p, *first_p;

    in_correct_section = 0;
    buf_p = self_p->buf_p;
    section_length = strlen(section_p);
    property_length = strlen(property_p);

    while (*buf_p != '\0') {
        if (*buf_p == '\r') {
            /* A line my start with "\r". */
        } else if ((*buf_p == '#') || (*buf_p == ';')) {
            /* Commented line. Ignore it. */
            while (*buf_p != '\n') {
                if (*buf_p == '\0') {
                    return (NULL);
                }

                buf_p++;
            }
        } else if (*buf_p == '[') {
            /* Section found. */
            buf_p++;
            first_p = buf_p;
            first_length = 0;
            section_end_found = 0;

            while (*buf_p != '\n') {
                if (*buf_p == '\0') {
                    return (NULL);
                }

                /* Ignore any carriage return and ']'. */
                if (*buf_p == ']') {
                    section_end_found = 1;
                }

                if (*buf_p != '\r') {
                    if (section_end_found == 0) {
                        first_length++;
                    }
                }

                buf_p++;
            }

            /* Is it the section we are looking for? */
            if ((section_length == first_length)
                && (strncmp(section_p, first_p, first_length) == 0)) {
                in_correct_section = 1;
            } else {
                in_correct_section = 0;
            }
        } else if (in_correct_section == 0) {
            /* Ignore any property in wrong section. */
            while (*buf_p != '\n') {
                if (*buf_p == '\0') {
                    return (NULL);
                }

                buf_p++;
            }
        } else {
            /* Property or empty line.*/
            if (strncmp(property_p, buf_p, property_length) == 0) {
                value_length = 0;
                buf_p += property_length;

                /* Ignore whitespaces before ':'. */
                while ((*buf_p == ' ') || (*buf_p == '\t')) {
                    if (*buf_p == '\0') {
                        return (NULL);
                    }

                    buf_p++;
                }

                if ((*buf_p != ':') && (*buf_p != '=')) {
                    /* Malformed property entry. */
                    return (NULL);
                }

                /* Skip the ':'. */
                buf_p++;

                /* Ignore whitespaces after ':'. */
                while ((*buf_p == ' ') || (*buf_p == '\t')) {
                    if (*buf_p == '\0') {
                        return (NULL);
                    }

                    buf_p++;
                }

                while (*buf_p != '\n') {
                    if (*buf_p == '\0') {
                        return (NULL);
                    }

                    /* Ignore any carriage return. */
                    if (*buf_p != '\r') {
                        if (value_length == (length - 1)) {
                            return (NULL);
                        }
                        
                        value_p[value_length] = *buf_p;
                        value_length++;
                    }

                    buf_p++;
                }

                value_p[value_length] = '\0';

                return (std_strip(value_p, NULL));
            } else {
                /* Ignore wrong property. */
                while (*buf_p != '\n') {
                    if (*buf_p == '\0') {
                        return (NULL);
                    }

                    buf_p++;
                }
            }
        }

        buf_p++;
    }

    return (NULL);
}

int configfile_get_long(struct configfile_t *self_p,
                        const char *section_p,
                        const char *property_p,
                        long *value_p)
{
    char buf[16];
    const char *next_p;

    /* Get the property value as a string. */
    if (configfile_get(self_p, section_p, property_p, buf, sizeof(buf)) == NULL) {
        return (-1);
    }

    /* Convert the property value string to a long. */
    next_p = std_strtol(buf, value_p);

    if ((next_p == NULL) || (*next_p != '\0')) {
        return (-1);
    }

    return (0);
}

int configfile_get_float(struct configfile_t *self_p,
                         const char *section_p,
                         const char *property_p,
                         float *value_p)
{
    char buf[32];
    char *next_p;

    /* Get the property value as a string. */
    if (configfile_get(self_p, section_p, property_p, buf, sizeof(buf)) == NULL) {
        return (-1);
    }

    /* Convert the property value string to a float. */
    *value_p = strtod(buf, &next_p);

    if ((buf == next_p) || (*next_p != '\0')) {
        return (-1);
    }

    return (0);
}
