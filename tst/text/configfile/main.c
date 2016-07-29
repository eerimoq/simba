/**
 * @file main.c
 * @version 3.1.1
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

static int test_get_colon(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]\r\n"
        "milk: 3\r\n"
        "cheese: 1 cheddar\r\n";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "3") == 0);

    /* Get the value of option 'cheese' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "cheese",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "1 cheddar") == 0);

    /* Get the value of a non-existing option in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "ham",
                            &value[0],
                            sizeof(value)) == NULL);

    /* Get the value of a non-existing option in a non-existing section. */
    BTASSERT(configfile_get(&configfile,
                            "clothes",
                            "skirt",
                            &value[0],
                            sizeof(value)) == NULL);

    return (0);
}

static int test_get_equal_sign(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]\r\n"
        "milk = 3\r\n"
        "cheese = 1 cheddar\r\n";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "3") == 0);

    return (0);
}

static int test_get_long(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[numbers]\r\n"
        "positive: 3\r\n"
        "negative: -54\r\n"
        "NaN: 1 cheddar\r\n";
    long value;

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'positive' in section 'numbers'. */
    BTASSERT(configfile_get_long(&configfile,
                                 "numbers", 
                                 "positive", 
                                 &value) == 0);
    BTASSERT(value == 3);

    /* Get the value of option 'negative' in section 'numbers'. */
    BTASSERT(configfile_get_long(&configfile,
                                 "numbers", 
                                 "negative", 
                                 &value) == 0);
    BTASSERT(value == -54);

    /* Get the value of option 'NaN' in section 'numbers'. Fails since
       "NaN" is not an integer. */
    BTASSERT(configfile_get_long(&configfile,
                                 "numbers", 
                                 "NaN", 
                                 &value) == -1);

    /* Get the value of option 'missing' in section 'numbers'. Fails
       since the option is missing. */
    BTASSERT(configfile_get_long(&configfile,
                                 "numbers", 
                                 "missing", 
                                 &value) == -1);

    return (0);
}

static int test_get_float(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[numbers]\r\n"
        "positive: 3.0\r\n"
        "negative: -54.5\r\n"
        "NaN: 1.0 cheddar\r\n"
        "NaN2: cheddar\r\n";
    float value;

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'positive' in section 'numbers'. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "positive", 
                                  &value) == 0);
    BTASSERT(value == 3.0f);

    /* Get the value of option 'negative' in section 'numbers'. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "negative", 
                                  &value) == 0);
    BTASSERT(value == -54.5f);

    /* Get the value of option 'NaN' in section 'numbers'. Fails since
       "NaN" is not an integer. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "NaN", 
                                  &value) == -1);

    /* Get the value of option 'NaN2' in section 'numbers'. Fails
       since "NaN2" is not an integer. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "NaN2", 
                                  &value) == -1);

    /* Get the value of option 'missing' in section 'numbers'. Fails
       since the option is missing. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "missing", 
                                  &value) == -1);

    return (0);
}

static int test_get_line_termination(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]\n"
        "\r\n"
        "milk = 3\n\r"
        "cheese = 1 cheddar\r\n"
        "ham = 1";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "3") == 0);

    /* Get the value of option 'cheese' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "cheese",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "1 cheddar") == 0);

    /* Bad line termination NULL of the ham line. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "ham",
                            &value[0],
                            sizeof(value)) == NULL);

    return (0);
}

static int test_get_null_after_section(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] = "[shopping list]";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == NULL);

    return (0);
}

static int test_get_missing_section_termination(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] = "[shopping list\r\n";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == NULL);

    return (0);
}

static int test_get_comments(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]\n"
        "#milk = 3\n\r"
        "cheese = 1 cheddar\r\n"
        ";ham = 1";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'milk' in section 'shopping
       list'. Finds nothing since the milk line is a coment.*/
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == NULL);

    /* Get the value of option 'cheese' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "cheese",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "1 cheddar") == 0);

    return (0);
}

static int test_get_whitespace(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]  \n"
        "milk  :   3   \t \r\n"
        "cheese\t:  1 cheddar  \r\n";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of option 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "3") == 0);

    /* Get the value of option 'cheese' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "cheese",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "1 cheddar") == 0);

    return (0);
}

static int test_get_bad_option_format(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]\r\n"
        "milk ; 3\r\n";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Bad option format. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "milk",
                            &value[0],
                            sizeof(value)) == NULL);

    return (0);
}

static int test_get_value_too_long(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]\r\n"
        "milk: 12\r\n";
    char value[2];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Value too long. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "milk",
                            &value[0],
                            sizeof(value)) == NULL);

    return (0);
}

static int test_set(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[68];

    memset(buf, '\0', sizeof(buf));
    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

#if 0

    /* Set the value of option 'milk' in section 'shopping list'. */
    BTASSERT(configfile_set(&configfile, "shopping list", "milk", "2") == 0);

    /* Set the value of option 'cheese' in section 'shopping list'. */
    BTASSERT(configfile_set(&configfile, "shopping list", "cheese", "brie") == 0);

    /* Set the value of option 'skirt' in section 'clothes'. */
    BTASSERT(configfile_set(&configfile, "clothes", "skirt", "1") == 0);

    /* No room left in the buffer for another option. */
    BTASSERT(configfile_set(&configfile, "clothes", "pants", "2") == -1);

    BTASSERT(memcmp(buf,
                    "[shopping list]\r\n"
                    "milk: 2\r\n"
                    "cheese: brie\r\n"
                    "[clothes]\r\n"
                    "skirt: 1\r\n",
                    66) == 0);

#endif

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_get_colon, "test_get_colon" },
        { test_get_equal_sign, "test_get_equal_sign" },
        { test_get_long, "test_get_long" },
        { test_get_float, "test_get_float" },
        { test_get_line_termination, "test_get_line_termination" },
        { test_get_null_after_section, "test_get_null_after_section" },
        { test_get_missing_section_termination, "test_get_missing_section_termination" },
        { test_get_comments, "test_get_comments" },
        { test_get_whitespace, "test_get_whitespace" },
        { test_get_bad_option_format, "test_get_bad_option_format" },
        { test_get_value_too_long, "test_get_value_too_long" },
        { test_set, "test_set" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
