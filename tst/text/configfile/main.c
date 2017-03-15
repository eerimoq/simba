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

static int test_get_colon(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]\r\n"
        "milk: 3\r\n"
        "cheese: 1 cheddar\r\n";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Get the value of property 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "3") == 0);

    /* Get the value of property 'cheese' in section 'shopping
       list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "cheese",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "1 cheddar") == 0);

    /* Get the value of a non-existing property in section 'shopping
       list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "ham",
                            &value[0],
                            sizeof(value)) == NULL);

    /* Get the value of a non-existing property in a non-existing
       section. */
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

    /* Get the value of property 'milk' in section 'shopping list'. */
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

    /* Get the value of property 'positive' in section 'numbers'. */
    BTASSERT(configfile_get_long(&configfile,
                                 "numbers", 
                                 "positive", 
                                 &value) == 0);
    BTASSERT(value == 3);

    /* Get the value of property 'negative' in section 'numbers'. */
    BTASSERT(configfile_get_long(&configfile,
                                 "numbers", 
                                 "negative", 
                                 &value) == 0);
    BTASSERT(value == -54);

    /* Get the value of property 'NaN' in section 'numbers'. Fails
       since "NaN" is not an integer. */
    BTASSERT(configfile_get_long(&configfile,
                                 "numbers", 
                                 "NaN", 
                                 &value) == -1);

    /* Get the value of property 'missing' in section 'numbers'. Fails
       since the property is missing. */
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

    /* Get the value of property 'positive' in section 'numbers'. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "positive", 
                                  &value) == 0);
    BTASSERT(value == 3.0f);

    /* Get the value of property 'negative' in section 'numbers'. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "negative", 
                                  &value) == 0);
    BTASSERT(value == -54.5f);

    /* Get the value of property 'NaN' in section 'numbers'. Fails
       since "NaN" is not an integer. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "NaN", 
                                  &value) == -1);

    /* Get the value of property 'NaN2' in section 'numbers'. Fails
       since "NaN2" is not an integer. */
    BTASSERT(configfile_get_float(&configfile,
                                  "numbers", 
                                  "NaN2", 
                                  &value) == -1);

    /* Get the value of property 'missing' in section 'numbers'. Fails
       since the property is missing. */
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

    /* Get the value of property 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "3") == 0);

    /* Get the value of property 'cheese' in section 'shopping
       list'. */
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

    /* Get the value of property 'milk' in section 'shopping list'. */
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

    /* Get the value of property 'milk' in section 'shopping list'. */
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

    /* Get the value of property 'milk' in section 'shopping
       list'. Finds nothing since the milk line is a coment.*/
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == NULL);

    /* Get the value of property 'cheese' in section 'shopping
       list'. */
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

    /* Get the value of property 'milk' in section 'shopping list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list", 
                            "milk", 
                            value,
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "3") == 0);

    /* Get the value of property 'cheese' in section 'shopping
       list'. */
    BTASSERT(configfile_get(&configfile,
                            "shopping list",
                            "cheese",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "1 cheddar") == 0);

    return (0);
}

static int test_get_bad_property_format(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] =
        "[shopping list]\r\n"
        "milk ; 3\r\n";
    char value[16];

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* Bad property format. */
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

    /* Set the value of property 'milk' in section 'shopping list'. */
    BTASSERT(configfile_set(&configfile, "shopping list", "milk", "2") == 0);

    /* Set the value of property 'cheese' in section 'shopping
       list'. */
    BTASSERT(configfile_set(&configfile, "shopping list", "cheese", "brie") == 0);

    /* Set the value of property 'skirt' in section 'clothes'. */
    BTASSERT(configfile_set(&configfile, "clothes", "skirt", "1") == 0);

    /* No room left in the buffer for another property. */
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


static int test_get_complex(struct harness_t *harness_p)
{
    struct configfile_t configfile;
    char buf[] = 
        "; last modified 1 April 2001 by John Doe\n"
        "[owner]\n"
        "name = John Doe\n"
        "organization = Acme Widgets Inc.\n"
        "\n"
        "[database]\n"
        "; use IP address in case network name resolution is not working\n"
        "server = 192.0.2.62\n"
        "port = 143\n"
        "file = \"payroll.dat\"\n";
    char value[32];
    long long_value;

    BTASSERT(configfile_init(&configfile, buf, sizeof(buf)) == 0);

    /* owner -> name. */
    BTASSERT(configfile_get(&configfile,
                            "owner", 
                            "name", 
                            value,
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "John Doe") == 0);

    /* owner -> organization. */
    BTASSERT(configfile_get(&configfile,
                            "owner",
                            "organization",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "Acme Widgets Inc.") == 0);

    /* database -> server. */
    BTASSERT(configfile_get(&configfile,
                            "database",
                            "server",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "192.0.2.62") == 0);

    /* database -> port. */
    BTASSERT(configfile_get_long(&configfile,
                                 "database",
                                 "port",
                                 &long_value) == 0);
    BTASSERT(long_value == 143);

    /* database -> file. */
    BTASSERT(configfile_get(&configfile,
                            "database",
                            "file",
                            &value[0],
                            sizeof(value)) == &value[0]);
    BTASSERT(strcmp(&value[0], "\"payroll.dat\"") == 0);

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
        { test_get_bad_property_format, "test_get_bad_property_format" },
        { test_get_value_too_long, "test_get_value_too_long" },
        { test_get_complex, "test_get_complex" },
        { test_set, "test_set" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
