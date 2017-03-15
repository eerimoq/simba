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

extern int http_server_stub_request(const char *path_p,
                                    struct http_server_connection_t *connection_p,
                                    struct http_server_request_t *request_p);

static int test_init(struct harness_t *self_p)
{
    BTASSERT(upgrade_http_init(80) == 0);
    BTASSERT(upgrade_http_start() == 0);

    return (0);
}

static int test_request_application_enter(struct harness_t *self_p)
{
    struct http_server_connection_t connection;
    struct http_server_request_t request;

    request.action = http_server_request_action_get_t;

    /* It actually returns -1 on success. */
    BTASSERT(http_server_stub_request("/oam/upgrade/application/enter",
                                      &connection,
                                      &request) == -1)

    /* Failed to enter the application. */
    BTASSERT(http_server_stub_request("/oam/upgrade/application/enter",
                                      &connection,
                                      &request) == 0)

    /* Only GET action is supported, so this should fail. */
    request.action = http_server_request_action_post_t;

    BTASSERT(http_server_stub_request("/oam/upgrade/application/enter",
                                      &connection,
                                      &request) == -1)

    return (0);
}

/*     { .path_p = "/oam/upgrade/application/erase", */
/*       .callback = http_request_application_erase }, */
/*     { .path_p = "/oam/upgrade/application/is_valid", */
/*       .callback = http_request_application_is_valid }, */
/*     { .path_p = "/oam/upgrade/upload", */
/*       .callback = http_request_upload }, */

static int test_request_bootloader_enter(struct harness_t *self_p)
{
    struct http_server_connection_t connection;
    struct http_server_request_t request;

    request.action = http_server_request_action_get_t;

    /* It actually returns -1 on success. */
    BTASSERT(http_server_stub_request("/oam/upgrade/bootloader/enter",
                                      &connection,
                                      &request) == -1)

    /* Failed to enter the bootloader. */
    BTASSERT(http_server_stub_request("/oam/upgrade/bootloader/enter",
                                      &connection,
                                      &request) == 0)

    /* Only GET action is supported, so this should fail. */
    request.action = http_server_request_action_post_t;

    BTASSERT(http_server_stub_request("/oam/upgrade/bootloader/enter",
                                      &connection,
                                      &request) == -1)

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_request_application_enter, "test_request_application_enter" },
        { test_request_bootloader_enter, "test_request_bootloader_enter" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
