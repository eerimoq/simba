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

static char qbuf[512];
static struct queue_t queue;

static int test_reset(struct harness_t *harness_p)
{
    BTASSERTI(settings_reset(), ==, 0);

    return (0);
}

static int test_cmd_list(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_LIST == 1

    char buf[64];

    /* Call the list command and validate the output. */
    strcpy(buf, "oam/settings/list");
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, 0);
    BTASSERTI(harness_expect(&queue,
                             "NAME                                      TYPE      SIZE  VALUE\r\n"
                             "int32                                     int32_t      4  -2\r\n"
                             "string                                    string_t     4  y\r\n"
                             "blob                                      blob_t       2  1112\r\n"
                             "blob_with_empty_default_data              blob_t       4  ffffffff\r\n"
                             "max_name_length_40_123456789012345678901  string_t     4  \r\n",
                             NULL), ==, 380);

    return (0);

#else

    return (1);

#endif
}

static int test_cmd_read(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_READ == 1

    char buf[64];

    /* Bad number of arguments. */
    std_sprintf(buf, FSTR("oam/settings/read"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "Usage: read <name>\r\n",
                             NULL), ==, 20);

    /* Bad setting name. */
    std_sprintf(buf, FSTR("oam/settings/read missing"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "missing: setting not found\r\n",
                             NULL), ==, 28);

    return (0);

#else

    return (1);

#endif
}

static int test_cmd_write(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_READ == 1

    char buf[128];

    /* Write a string that is too long. */
    std_sprintf(buf,
                FSTR("oam/settings/write string ThisStringIsTooLong"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "ThisStringIsTooLong: string too long\r\n",
                             NULL), ==, 38);

    /* Write a blob that is too long. */
    std_sprintf(buf,
                FSTR("oam/settings/write blob 010203"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "3: bad blob data length\r\n",
                             NULL), ==, 25);

    /* Write a blob that is too short. */
    std_sprintf(buf,
                FSTR("oam/settings/write blob 01"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "1: bad blob data length\r\n",
                             NULL), ==, 25);

    /* Write a blob that contains invalid data. */
    std_sprintf(buf,
                FSTR("oam/settings/write blob 01XX"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "01XX: bad blob data\r\n",
                             NULL), ==, 21);

    /* Write a blob that contains invalid data. */
    std_sprintf(buf,
                FSTR("oam/settings/write blob foo1"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "foo1: bad blob data\r\n",
                             NULL), ==, 21);

    /* Bad number of arguments. */
    std_sprintf(buf, FSTR("oam/settings/write"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "Usage: write <name> <value>\r\n",
                             NULL), ==, 29);

    /* Bad setting name. */
    std_sprintf(buf, FSTR("oam/settings/write missing 1"));
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, -EINVAL);
    BTASSERTI(harness_expect(&queue,
                             "missing: setting not found\r\n",
                             NULL), ==, 28);

    return (0);

#else

    return (1);

#endif
}

static int test_cmd_read_write_read(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_READ == 1

    int i;
    char buf[128];
    char response[64];
    char *names_p[] = {
        "int32", "string", "blob"
    };
    char *values_before_p[] = {
        "-2", "y", "1112"
    };
    char *values_after_p[] = {
        "12345678", "hi", "ffff"
    };

    for (i = 0; i < membersof(names_p); i++) {
        /* Read the value. */
        std_sprintf(buf, FSTR("oam/settings/read %s"), names_p[i]);
        std_printf(FSTR("%s\r\n"), buf);
        BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, 0);
        std_sprintf(&response[0], FSTR("%s\r\n"), values_before_p[i]);
        BTASSERTI(harness_expect(&queue, &response[0], NULL),
                  ==,
                  strlen(values_before_p[i]) + 2);

        /* Write a new value. */
        std_sprintf(buf,
                    FSTR("oam/settings/write %s %s"),
                    names_p[i],
                    values_after_p[i]);
        std_printf(FSTR("%s\r\n"), buf);
        BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, 0);

        /* Read the value again to verify that the write command
           works. */
        std_sprintf(buf, FSTR("oam/settings/read %s"), names_p[i]);
        std_printf(FSTR("%s\r\n"), buf);
        BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, 0);
        std_sprintf(&response[0], FSTR("%s\r\n"), values_after_p[i]);
        BTASSERTI(harness_expect(&queue, &response[0], NULL),
                  ==,
                  strlen(values_after_p[i]) + 2);
    }

    return (0);

#else

    return (1);

#endif
}

static int test_cmd_reset(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_RESET == 1

    char buf[64];

    /* Call the list command and validate the output. */
    strcpy(buf, "oam/settings/reset");
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, 0);

    /* Check the list output. */
    strcpy(buf, "oam/settings/list");
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, 0);
    BTASSERTI(harness_expect(&queue,
                             "NAME                                      TYPE      SIZE  VALUE\r\n"
                             "int32                                     int32_t      4  -2\r\n"
                             "string                                    string_t     4  y\r\n"
                             "blob                                      blob_t       2  1112\r\n"
                             "blob_with_empty_default_data              blob_t       4  ffffffff\r\n"
                             "max_name_length_40_123456789012345678901  string_t     4  \r\n",
                             NULL), ==, 380);

    return (0);

#else

    return (1);

#endif
}

static int test_integer(struct harness_t *harness_p)
{
    int32_t int32;

    BTASSERTI(settings_read(&int32,
                            SETTING_INT32_ADDR,
                            SETTING_INT32_SIZE), ==, SETTING_INT32_SIZE);
    BTASSERTI(int32, ==, SETTING_INT32_VALUE);

    int32 = 46;
    BTASSERTI(settings_write(SETTING_INT32_ADDR,
                             &int32,
                             SETTING_INT32_SIZE), ==, SETTING_INT32_SIZE);

    int32 = 0;
    BTASSERTI(settings_read(&int32,
                            SETTING_INT32_ADDR,
                            SETTING_INT32_SIZE), ==, SETTING_INT32_SIZE);
    BTASSERTI(int32, ==, 46);

    return (0);
}

static int test_string(struct harness_t *harness_p)
{
    char string[SETTING_STRING_SIZE];

    /* Read the default value. */
    memset(string, 0, sizeof(string));
    BTASSERTI(settings_read(string,
                            SETTING_STRING_ADDR,
                            SETTING_STRING_SIZE), ==, SETTING_STRING_SIZE);
    BTASSERTM(string, "y", 1);

    /* Overwrite the default value with "x". */
    string[0] = 'x';
    string[1] = '\0';
    BTASSERTI(settings_write(SETTING_STRING_ADDR,
                             string,
                             SETTING_STRING_SIZE), ==, SETTING_STRING_SIZE);

    /* Read the overwritten value. */
    memset(string, 0, sizeof(string));
    BTASSERTI(settings_read(string,
                            SETTING_STRING_ADDR,
                            SETTING_STRING_SIZE), ==, SETTING_STRING_SIZE);
    BTASSERTM(string, "x", 1);

    return (0);
}

static int test_read_write_by_name(struct harness_t *harness_p)
{
    int32_t int32;

    int32 = 10;
    BTASSERTI(settings_write_by_name("int32",
                                     &int32,
                                     sizeof(int32)), ==, sizeof(int32));

    int32 = 0;
    BTASSERTI(settings_read_by_name("int32",
                                    &int32,
                                    sizeof(int32)), ==, sizeof(int32));
    BTASSERTI(int32, ==, 10);

    return (0);
}

static int test_cmd_list_after_updates(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_LIST == 1

    char buf[64];

    /* Call the list command and validate the output. */
    strcpy(buf, "oam/settings/list");
    BTASSERTI(fs_call(buf, NULL, &queue, NULL), ==, 0);
    BTASSERTI(harness_expect(&queue,
                             "NAME                                      TYPE      SIZE  VALUE\r\n"
                             "int32                                     int32_t      4  10\r\n"
                             "string                                    string_t     4  x\r\n"
                             "blob                                      blob_t       2  1112\r\n"
                             "blob_with_empty_default_data              blob_t       4  ffffffff\r\n"
                             "max_name_length_40_123456789012345678901  string_t     4  \r\n",
                             NULL), ==, 380);

    return (0);

#else

    return (1);

#endif
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_reset, "test_reset" },
        { test_cmd_list, "test_cmd_list" },
        { test_cmd_read, "test_cmd_read" },
        { test_cmd_write, "test_cmd_write" },
        { test_cmd_read_write_read, "test_cmd_read_write_read" },
        { test_cmd_reset, "test_cmd_reset" },
        { test_integer, "test_integer" },
        { test_string, "test_string" },
        { test_read_write_by_name, "test_read_write_by_name" },
        { test_cmd_list_after_updates, "test_cmd_list_after_updates" },
        { NULL, NULL }
    };

    sys_start();

    queue_init(&queue, qbuf, sizeof(qbuf));

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
