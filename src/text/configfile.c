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

/**
 * Ignore one line.
 */
static char *ignore_line(char *buf_p)
{
    while (*buf_p != '\n') {
        if (*buf_p == '\0') {
            return (NULL);
        }

        buf_p++;
    }

    return (buf_p);
}

/**
 * Parse and ignore a line comment.
 */
static char *ignore_comment(char *buf_p)
{
    return (ignore_line(buf_p));
}

/**
 * Ignore wrong property.
 */
static char *ignore_wrong_property(char *buf_p)
{
    return (ignore_line(buf_p));
}

/**
 * Parse given section header.
 */
static char *parse_section_header(char *buf_p,
                                  const char *section_p,
                                  int section_length,
                                  int *in_correct_section_p)
{
    char *name_p;
    int name_length;
    int section_end_found;

    buf_p++;
    name_p = buf_p;
    name_length = 0;
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
                name_length++;
            }
        }

        buf_p++;
    }

    /* Is it the section we are looking for? */
    *in_correct_section_p = ((section_length == name_length)
                             && (strncmp(section_p, name_p, name_length) == 0));

    return (buf_p);
}

static char *parse_property(char *buf_p,
                            char *value_p,
                            int length,
                            int property_length)
{
    int value_length;

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
}

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
    return (-ENOSYS);
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
    char *buf_p;

    in_correct_section = 0;
    buf_p = self_p->buf_p;
    section_length = strlen(section_p);
    property_length = strlen(property_p);

    while (*buf_p != '\0') {
        if (*buf_p == '\r') {
            /* A line my start with "\r". */
        } else if ((*buf_p == '#') || (*buf_p == ';')) {
            buf_p = ignore_comment(buf_p);
        } else if (*buf_p == '[') {
            buf_p = parse_section_header(buf_p,
                                         section_p,
                                         section_length,
                                         &in_correct_section);
        } else if (in_correct_section == 0) {
            buf_p = ignore_line(buf_p);
        } else {
            /* Property or empty line.*/
            if (strncmp(property_p, buf_p, property_length) == 0) {
                return (parse_property(buf_p,
                                       value_p,
                                       length,
                                       property_length));
            } else {
                buf_p = ignore_wrong_property(buf_p);
            }
        }

        if (buf_p == NULL) {
            return (buf_p);
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
    char *buf_p;
    char buf[16];
    const char *next_p;

    /* Get the property value as a string. */
    buf_p = configfile_get(self_p, section_p, property_p, buf, sizeof(buf));

    if (buf_p == NULL) {
        return (-EKEYNOTFOUND);
    }

    /* Convert the property value string to a long. */
    next_p = std_strtol(buf_p, value_p);

    if ((next_p == NULL) || (*next_p != '\0')) {
        return (-EBADVALUE);
    }

    return (0);
}

int configfile_get_float(struct configfile_t *self_p,
                         const char *section_p,
                         const char *property_p,
                         float *value_p)
{
    char *buf_p;
    char buf[32];
    const char *next_p;
    double value;

    /* Get the property value as a string. */
    buf_p = configfile_get(self_p, section_p, property_p, buf, sizeof(buf));

    if (buf_p == NULL) {
        return (-EKEYNOTFOUND);
    }

    /* Convert the property value string to a float. */
    next_p = std_strtod(buf_p, &value);
    *value_p = value;

    if ((next_p == NULL) || (*next_p != '\0')) {
        return (-EBADVALUE);
    }

    return (0);
}
