/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#include "gen/settings.h"

static char qbuf[512];
static struct queue_t queue;

static int test_reset(struct harness_t *harness_p)
{
    BTASSERT(settings_reset() == 0);

    return (0);
}

static int test_cmd_list(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_LIST == 1

    char buf[512];
    size_t size;
    FAR const char *response_p;

    /* Call the list command and validate the output. */
    strcpy(buf, "oam/settings/list");
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
    response_p = FSTR("NAME                  TYPE      SIZE  VALUE\r\n"
                      "int32                 int32_t      4  -2\r\n"
                      "string                string_t     4  y\r\n"
                      "blob                  blob_t       2  \\x11\\x12\r\n");
    size = std_strlen(response_p);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    std_printf(FSTR("%s\r\n"), buf);
    BTASSERT(std_strcmp(buf, response_p) == 0);
    
    return (0);

#else

    return (1);

#endif
}

static int test_cmd_read(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_READ == 1

    char buf[128];
    char response[64];
    size_t size;

    /* Bad number of arguments. */
    std_sprintf(buf, FSTR("oam/settings/read"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    std_sprintf(response, FSTR("Usage: read <name>\r\n"));
    size = strlen(response);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0, "'%s'", buf);

    /* Bad setting name. */
    std_sprintf(buf, FSTR("oam/settings/read missing"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    std_sprintf(response, FSTR("missing: setting not found\r\n"));
    size = strlen(response);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    return (0);

#else

    return (1);

#endif
}

static int test_cmd_write(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_READ == 1

    char buf[128];
    char response[64];
    size_t size;

    /* Write a string that is too long. */
    std_sprintf(buf,
                FSTR("oam/settings/write string ThisStringIsTooLong"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    size = std_sprintf(response, FSTR("ThisStringIsTooLong: string too long\r\n"));
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Write a blob that is too long. */
    std_sprintf(buf,
                FSTR("oam/settings/write blob \\x01\\x02\\x03"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    size = std_sprintf(response, FSTR("3: bad blob data length\r\n"));
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Write a blob that is too short. */
    std_sprintf(buf,
                FSTR("oam/settings/write blob \\x01"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    size = std_sprintf(response, FSTR("1: bad blob data length\r\n"));
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Write a blob that contains invalid data. */
    std_sprintf(buf,
                FSTR("oam/settings/write blob \\x01\\xXX"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    size = std_sprintf(response, FSTR("bad blob data\r\n"));
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Write a blob that contains invalid data. */
    std_sprintf(buf,
                FSTR("oam/settings/write blob 12345678"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    size = std_sprintf(response, FSTR("bad blob data\r\n"));
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Bad number of arguments. */
    std_sprintf(buf, FSTR("oam/settings/write"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    size = std_sprintf(response,
                       FSTR("Usage: write <name> <value>\r\n"));
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Bad setting name. */
    std_sprintf(buf, FSTR("oam/settings/write missing 1"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    std_sprintf(response, FSTR("missing: setting not found\r\n"));
    size = strlen(response);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

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
    size_t size;
    char *names_p[] = {
        "int32", "string", "blob"
    };
    char *values_before_p[] = {
        "-2", "y", "\\x11\\x12"
    };
    char *values_after_p[] = {
        "12345678", "hi", "\\x01\\x02"
    };

    for (i = 0; i < membersof(names_p); i++) {
        /* Read the value. */
        std_sprintf(buf, FSTR("oam/settings/read %s"), names_p[i]);
        std_printf(FSTR("%s\r\n"), buf);
        BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
        std_sprintf(response, FSTR("%s\r\n"), values_before_p[i]);
        size = strlen(response);
        BTASSERT(chan_read(&queue, buf, size) == size);
        buf[size] = '\0';
        std_printf(FSTR("%s"), &buf[0]);
        BTASSERT(strcmp(buf, response) == 0);

        /* Write a new value. */
        std_sprintf(buf,
                    FSTR("oam/settings/write %s %s"),
                    names_p[i],
                    values_after_p[i]);
        std_printf(FSTR("%s\r\n"), buf);
        BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);

        /* Read the value again to verify that the write command
           works. */
        std_sprintf(buf, FSTR("oam/settings/read %s"), names_p[i]);
        std_printf(FSTR("%s\r\n"), buf);
        BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
        std_sprintf(response, FSTR("%s\r\n"), values_after_p[i]);
        size = strlen(response);
        BTASSERT(chan_read(&queue, buf, size) == size);
        buf[size] = '\0';
        std_printf(FSTR("%s"), &buf[0]);
        BTASSERT(strcmp(buf, response) == 0);
    }

    return (0);

#else

    return (1);

#endif
}

static int test_cmd_reset(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_RESET == 1

    char buf[512];
    size_t size;
    FAR const char *response_p;

    /* Call the list command and validate the output. */
    strcpy(buf, "oam/settings/reset");
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);

    /* Check the list output. */
    strcpy(buf, "oam/settings/list");
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
    response_p = FSTR("NAME                  TYPE      SIZE  VALUE\r\n"
                      "int32                 int32_t      4  -2\r\n"
                      "string                string_t     4  y\r\n"
                      "blob                  blob_t       2  \\x11\\x12\r\n");
    size = std_strlen(response_p);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(std_strcmp(buf, response_p) == 0);

    return (0);

#else

    return (1);

#endif
}

static int test_integer(struct harness_t *harness_p)
{
    int32_t int32;

    BTASSERT(settings_read(&int32,
                           SETTING_INT32_ADDR,
                           SETTING_INT32_SIZE) == SETTING_INT32_SIZE);
    BTASSERT(int32 == SETTING_INT32_VALUE);

    int32 = 46;
    BTASSERT(settings_write(SETTING_INT32_ADDR,
                            &int32,
                            SETTING_INT32_SIZE) == SETTING_INT32_SIZE);

    int32 = 0;
    BTASSERT(settings_read(&int32,
                           SETTING_INT32_ADDR,
                           SETTING_INT32_SIZE) == SETTING_INT32_SIZE);
    BTASSERT(int32 == 46);

    return (0);
}

static int test_string(struct harness_t *harness_p)
{
    char string[SETTING_STRING_SIZE];

    /* Read the default value. */
    memset(string, 0, sizeof(string));
    BTASSERT(settings_read(string,
                           SETTING_STRING_ADDR,
                           SETTING_STRING_SIZE) == SETTING_STRING_SIZE);
    BTASSERT(strcmp(string, "y") == 0);

    /* OVerwrite the default value with "x". */
    string[0] = 'x';
    string[1] = '\0';
    BTASSERT(settings_write(SETTING_STRING_ADDR,
                            string,
                            SETTING_STRING_SIZE) == SETTING_STRING_SIZE);

    /* Read the overwritten value. */
    memset(string, 0, sizeof(string));
    BTASSERT(settings_read(string,
                           SETTING_STRING_ADDR,
                           SETTING_STRING_SIZE) == SETTING_STRING_SIZE);
    BTASSERT(strcmp(string, "x") == 0);

    return (0);
}

static int test_read_write_by_name(struct harness_t *harness_p)
{
    int32_t int32;

    int32 = 10;
    BTASSERT(settings_write_by_name("int32",
                                    &int32,
                                    sizeof(int32)) == sizeof(int32));

    int32 = 0;
    BTASSERT(settings_read_by_name("int32",
                                   &int32,
                                   sizeof(int32)) == sizeof(int32));
    BTASSERT(int32 == 10);

    return (0);
}

static int test_cmd_list_after_updates(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTINGS_LIST == 1

    char buf[512];
    size_t size;
    FAR const char *response_p;

    /* Call the list command and validate the output. */
    strcpy(buf, "oam/settings/list");
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
    response_p = FSTR("NAME                  TYPE      SIZE  VALUE\r\n"
                      "int32                 int32_t      4  10\r\n"
                      "string                string_t     4  x\r\n"
                      "blob                  blob_t       2  \\x11\\x12\r\n");
    size = std_strlen(response_p);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    std_printf(FSTR("%s\r\n"), buf);
    BTASSERT(std_strcmp(buf, response_p) == 0);

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
