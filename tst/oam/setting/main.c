/**
 * @file main.c
 * @version 5.0.0
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

static char qbuf[512];
static struct queue_t queue;

static int test_reset(struct harness_t *harness_p)
{
    BTASSERT(setting_reset() == 0);

    return (0);
}

static int test_cmd_list(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTING_LIST == 1

    char buf[512];
    size_t size;
    FAR const char *response_p;

    /* Call the list command and validate the output. */
    strcpy(buf, "oam/setting/list");
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
    response_p = FSTR("NAME                  TYPE     SIZE  VALUE\r\n"
                      "int8                  int8_t      1  -4\r\n"
                      "int16                 int16_t     2  -3\r\n"
                      "int32                 int32_t     4  -2\r\n"
                      "string                string      4  y\r\n");
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
#if CONFIG_FS_CMD_SETTING_READ == 1

    char buf[128];
    char response[64];
    size_t size;

    /* Bad number of arguments. */
    std_sprintf(buf, FSTR("oam/setting/read"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    std_sprintf(response, FSTR("Usage: oam/setting/read <name>\r\n"));
    size = strlen(response);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Bad setting name. */
    std_sprintf(buf, FSTR("oam/setting/read missing"));
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
#if CONFIG_FS_CMD_SETTING_READ == 1

    int i;
    char buf[128];
    char response[64];
    size_t size;
    char *names_p[] = {
        "int8", "int8", "int16", "int16"
    };
    char *out_of_range_values_p[] = {
        "128", "-129", "32768", "-32769"
    };

    /* Write a value that is outside the allowed range. */
    for (i = 0; i < membersof(names_p); i++) {
        std_sprintf(buf,
                    FSTR("oam/setting/write %s %s"),
                    names_p[i],
                    out_of_range_values_p[i]);
        BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
        std_sprintf(response, FSTR("%s: value out of range\r\n"), out_of_range_values_p[i]);
        size = strlen(response);
        BTASSERT(chan_read(&queue, buf, size) == size);
        buf[size] = '\0';
        BTASSERT(strcmp(buf, response) == 0);
    }

    /* Write a string that is too long. */
    std_sprintf(buf,
                FSTR("oam/setting/write string ThisStringIsTooLong"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    std_sprintf(response, FSTR("ThisStringIsTooLong: string too long\r\n"));
    size = strlen(response);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Bad number of arguments. */
    std_sprintf(buf, FSTR("oam/setting/write"));
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == -1);
    std_sprintf(response, FSTR("Usage: oam/setting/write <name> <value>\r\n"));
    size = strlen(response);
    BTASSERT(chan_read(&queue, buf, size) == size);
    buf[size] = '\0';
    BTASSERT(strcmp(buf, response) == 0);

    /* Bad setting name. */
    std_sprintf(buf, FSTR("oam/setting/write missing 1"));
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
#if CONFIG_FS_CMD_SETTING_READ == 1

    int i;
    char buf[128];
    char response[64];
    size_t size;
    char *names_p[] = {
        "int8", "int16", "int32", "string"
    };
    char *values_before_p[] = {
        "-4", "-3", "-2", "y"
    };
    char *values_after_p[] = {
        "78", "543", "12345678", "hi"
    };

    for (i = 0; i < membersof(names_p); i++) {
        /* Read the value. */
        std_sprintf(buf, FSTR("oam/setting/read %s"), names_p[i]);
        BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
        std_sprintf(response, FSTR("%s\r\n"), values_before_p[i]);
        size = strlen(response);
        BTASSERT(chan_read(&queue, buf, size) == size);
        buf[size] = '\0';
        BTASSERT(strcmp(buf, response) == 0);

        /* Write a new value. */
        std_sprintf(buf,
                    FSTR("oam/setting/write %s %s"),
                    names_p[i],
                    values_after_p[i]);
        BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);

        /* Read the value again to verify that the write command
           works. */
        std_sprintf(buf, FSTR("oam/setting/read %s"), names_p[i]);
        BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
        std_sprintf(response, FSTR("%s\r\n"), values_after_p[i]);
        size = strlen(response);
        BTASSERT(chan_read(&queue, buf, size) == size);
        buf[size] = '\0';
        BTASSERT(strcmp(buf, response) == 0);
    }

    return (0);

#else

    return (1);

#endif
}

static int test_cmd_reset(struct harness_t *harness_p)
{
#if CONFIG_FS_CMD_SETTING_RESET == 1

    char buf[512];
    size_t size;
    FAR const char *response_p;

    /* Call the list command and validate the output. */
    strcpy(buf, "oam/setting/reset");
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);

    /* Check the list output. */
    strcpy(buf, "oam/setting/list");
    BTASSERT(fs_call(buf, NULL, &queue, NULL) == 0);
    response_p = FSTR("NAME                  TYPE     SIZE  VALUE\r\n"
                      "int8                  int8_t      1  -4\r\n"
                      "int16                 int16_t     2  -3\r\n"
                      "int32                 int32_t     4  -2\r\n"
                      "string                string      4  y\r\n");
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
    int8_t int8;
    int16_t int16;
    int32_t int32;

    BTASSERT(setting_read(&int8,
                          SETTING_INT8_ADDR,
                          SETTING_INT8_SIZE) == SETTING_INT8_SIZE);
    BTASSERT(int8 == SETTING_INT8_VALUE);

    BTASSERT(setting_read(&int16,
                          SETTING_INT16_ADDR,
                          SETTING_INT16_SIZE) == SETTING_INT16_SIZE);
    BTASSERT(int16 == SETTING_INT16_VALUE);

    BTASSERT(setting_read(&int32,
                          SETTING_INT32_ADDR,
                          SETTING_INT32_SIZE) == SETTING_INT32_SIZE);
    BTASSERT(int32 == SETTING_INT32_VALUE);

    int32 = 46;
    BTASSERT(setting_write(SETTING_INT32_ADDR,
                           &int32,
                           SETTING_INT32_SIZE) == SETTING_INT32_SIZE);

    int32 = 0;
    BTASSERT(setting_read(&int32,
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
    BTASSERT(setting_read(string,
                          SETTING_STRING_ADDR,
                          SETTING_STRING_SIZE) == SETTING_STRING_SIZE);
    BTASSERT(strcmp(string, "y") == 0);

    /* OVerwrite the default value with "x". */
    string[0] = 'x';
    string[1] = '\0';
    BTASSERT(setting_write(SETTING_STRING_ADDR,
                           string,
                           SETTING_STRING_SIZE) == SETTING_STRING_SIZE);

    /* Read the overwritten value. */
    memset(string, 0, sizeof(string));
    BTASSERT(setting_read(string,
                          SETTING_STRING_ADDR,
                          SETTING_STRING_SIZE) == SETTING_STRING_SIZE);
    BTASSERT(strcmp(string, "x") == 0);

    return (0);
}

static int test_read_write_by_name(struct harness_t *harness_p)
{
    int32_t int32;

    int32 = 10;
    BTASSERT(setting_write_by_name("int32",
                                   &int32,
                                   sizeof(int32)) == sizeof(int32));

    int32 = 0;
    BTASSERT(setting_read_by_name("int32",
                                  &int32,
                                  sizeof(int32)) == sizeof(int32));
    BTASSERT(int32 == 10);

    return (0);
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
        { NULL, NULL }
    };

    sys_start();

    queue_init(&queue, qbuf, sizeof(qbuf));

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
