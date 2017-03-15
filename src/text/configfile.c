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
    ASSERTNRN(self_p != NULL, EINVAL);
    ASSERTNRN(section_p != NULL, EINVAL);
    ASSERTNRN(property_p != NULL, EINVAL);
    ASSERTNRN(value_p != NULL, EINVAL);
    ASSERTNRN(length > 0, EINVAL);

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
    const char *next_p;
    double value;

    /* Get the property value as a string. */
    if (configfile_get(self_p, section_p, property_p, buf, sizeof(buf)) == NULL) {
        return (-1);
    }

    /* Convert the property value string to a float. */
    next_p = std_strtod(buf, &value);
    *value_p = value;

    if ((next_p == NULL) || (*next_p != '\0')) {
        return (-1);
    }

    return (0);
}
