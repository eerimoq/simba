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

extern int ssl_open_counter;
extern int ssl_close_counter;
extern int ssl_write_counter;
extern int ssl_read_counter;
extern int ssl_size_counter;

extern void socket_stub_init(void);
extern void socket_stub_accept();
extern void socket_stub_input(void *buf_p, size_t size);
extern void socket_stub_input_flush(void);
extern void socket_stub_output(void *buf_p, size_t size);
extern void socket_stub_wait_closed(void);
extern void socket_stub_close_connection(void);

static int request_index(struct http_server_connection_t *connection_p,
                         struct http_server_request_t *request_p);
static int request_auth(struct http_server_connection_t *connection_p,
                        struct http_server_request_t *request_p);
static int request_form(struct http_server_connection_t *connection_p,
                        struct http_server_request_t *request_p);
static int request_websocket_echo(struct http_server_connection_t *connection_p,
                                  struct http_server_request_t *request_p);
static int request_404_not_found(struct http_server_connection_t *connection_p,
                                 struct http_server_request_t *request_p);

static struct http_server_t foo;

static struct http_server_route_t routes[] = {
    { .path_p = "/index.html", .callback = request_index },
    { .path_p = "/auth.html", .callback = request_auth },
    { .path_p = "/form.html", .callback = request_form },
    { .path_p = "/websocket/echo", .callback = request_websocket_echo },
    { .path_p = NULL, .callback = NULL }
};

THRD_STACK(listener_stack, 2048);
THRD_STACK(connection_stack, 2048);

THRD_STACK(https_listener_stack, 2048);
THRD_STACK(https_connection_stack, 2048);

/**
 * Handler for the index request.
 */
static int request_index(struct http_server_connection_t *connection_p,
                         struct http_server_request_t *request_p)
{
    struct http_server_response_t response;

    /* Only the GET action is supported. */
    if (request_p->action != http_server_request_action_get_t) {
        return (-1);
    }

    /* Create the response. */
    response.code = http_server_response_code_200_ok_t;
    response.content.type = http_server_content_type_text_html_t;
    response.content.buf_p = "Welcome!";
    response.content.size = strlen(response.content.buf_p);

    return (http_server_response_write(connection_p, request_p, &response));
}

/**
 * Handler for the auth request.
 */
static int request_auth(struct http_server_connection_t *connection_p,
                         struct http_server_request_t *request_p)
{
    struct http_server_response_t response;

    BTASSERT(strcmp(request_p->headers.authorization.value,
                    "Basic YWRtaW46YWRtaW4=") == 0);

    /* Create the response. */
    response.code = http_server_response_code_401_unauthorized_t;
    response.content.type = http_server_content_type_text_html_t;
    response.content.buf_p = NULL;
    response.content.size = 0;

    return (http_server_response_write(connection_p, request_p, &response));
}

/**
 * Handler for the form request.
 */
static int request_form(struct http_server_connection_t *connection_p,
                        struct http_server_request_t *request_p)
{
    struct http_server_response_t response;
    char buf[16];

    /* Verify the request. */
    BTASSERT(request_p->action == http_server_request_action_post_t);
    BTASSERT(request_p->headers.content_length.present == 1);
    BTASSERT(request_p->headers.content_length.value == 9);
    BTASSERT(request_p->headers.content_type.present == 1);
    BTASSERT(strcmp(request_p->headers.content_type.value,
                    "application/x-www-form-urlencoded") == 0);
    BTASSERT(chan_read(connection_p->chan_p,
                       buf,
                       request_p->headers.content_length.value) == 9);
    BTASSERT(strncmp("key=value", buf, 9) == 0);

    /* Create the response. */
    response.code = http_server_response_code_200_ok_t;
    response.content.type = http_server_content_type_text_html_t;
    response.content.buf_p = "Form!";
    response.content.size = strlen(response.content.buf_p);

    return (http_server_response_write(connection_p, request_p, &response));
}

/**
 * Handler for the websocket echo request. Echo all websocket messages
 * the client sends on the socket.
 */
static int request_websocket_echo(struct http_server_connection_t *connection_p,
                                  struct http_server_request_t *request_p)
{
    ssize_t res;
    struct http_websocket_server_t server;
    uint8_t buf[32];
    int type;

    if (http_websocket_server_init(&server, connection_p->chan_p) != 0) {
        return (-1);
    }

    /* Handshake sequence. */
    if (http_websocket_server_handshake(&server, request_p) != 0) {
        return (-1);
    }

    /* Echo all read data. */
    while (1) {
        res = http_websocket_server_read(&server, &type, buf, sizeof(buf));

        /* Stop when the socket is closed. */
        if (res <= 0) {
            return (res);
        }

        if (http_websocket_server_write(&server, type, buf, res) != res) {
            PRINT_FILE_LINE();
            return (-1);
        }
    }

    return (0);
}

/**
 * Handler for all requests except those in the route array.
 */
static int request_404_not_found(struct http_server_connection_t *connection_p,
                                 struct http_server_request_t *request_p)
{
    int res;
    char content[64];
    size_t size;
    struct http_server_response_t response;

    /* Create the response. */
    size = std_sprintf(content,
                       FSTR("The requested page '%s' could not be found."),
                       request_p->path);
    response.code = http_server_response_code_404_not_found_t;
    response.content.type = http_server_content_type_text_plain_t;
    response.content.buf_p = NULL;
    response.content.size = size;

    res = http_server_response_write(connection_p, request_p, &response);

    if (res != 0) {
        return (res);
    }

    if (chan_write(connection_p->chan_p, content, size) != size) {
        return (-EIO);
    }

    return (0);
}

static int test_start(struct harness_t *harness_p)
{
    static struct http_server_listener_t listener = {
        .address_p = "127.0.0.1",
        .port = 80,
        .thrd = {
            .name_p = "http_listener",
            .stack = {
                .buf_p = listener_stack,
                .size = sizeof(listener_stack)
            }
        }
    };
    static struct http_server_connection_t connections[] = {
        {
            .thrd = {
                .name_p = "http_conn_0",
                .stack = {
                    .buf_p = connection_stack,
                    .size = sizeof(connection_stack)
                }
            }
        },
        {
            .thrd = {
                .name_p = NULL
            }
        }
    };

    BTASSERT(http_server_init(&foo,
                              &listener,
                              connections,
                              NULL,
                              routes,
                              request_404_not_found) == 0);

    BTASSERT(http_server_start(&foo) == 0);

    thrd_set_log_mask(listener.thrd.id_p, LOG_UPTO(DEBUG));
    thrd_set_log_mask(connections[0].thrd.id_p, LOG_UPTO(DEBUG));

    /* Less log clobbering. */
    thrd_sleep_us(100000);

    return (0);
}

static int test_request_index(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input GET /index.html on the connection socket. */
    str_p =
        "GET /index.html HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the GET response and verify it. */
    str_p =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 8\r\n"
        "\r\n"
        "Welcome!";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    socket_stub_wait_closed();

    return (0);
}

static int test_request_index_with_query_string(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input GET /index.html?hello=world on the connection socket. */
    str_p =
        "GET /index.html?hello=world HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the GET response and verify it. */
    str_p =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 8\r\n"
        "\r\n"
        "Welcome!";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    socket_stub_wait_closed();

    return (0);
}

static int test_request_auth(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input GET /missing.html on the connection socket. */
    str_p =
        "GET /auth.html HTTP/1.1\r\n"
        "Authorization: Basic YWRtaW46YWRtaW4=\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the GET response and verify it. */
    str_p =
        "HTTP/1.1 401 Unauthorized\r\n"
        "WWW-Authenticate: Basic realm=\"\"\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 0\r\n"
        "\r\n";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    socket_stub_wait_closed();

    return (0);
}

static int test_request_form(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input POST /form.html on the connection socket. */
    str_p =
        "POST /form.html HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 9\r\n"
        "\r\n"
        "key=value";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the POST response and verify it. */
    str_p =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "Form!";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    socket_stub_wait_closed();

    return (0);
}

static int test_request_websocket(struct harness_t *harness_p)
{
    int i;
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input the handshake GET /websocket/echo. */
    str_p =
        "GET /websocket/echo HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Upgrade: WebSocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the handshake response and verify it. */
    str_p =
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=\r\n"
        "\r\n";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    for (i = 0; i < 3; i++) {
        /* Input a websocket frame on the socket. */
        buf[0] = 0x81; /* FIN & TEXT. */
        buf[1] = 0x84; /* MASK and 1 byte payload length. */
        buf[2] = 0x00; /* Masking key 0. */
        buf[3] = 0x00; /* Masking key 1. */
        buf[4] = 0x00; /* Masking key 2. */
        buf[5] = 0x00; /* Masking key 3. */
        buf[6] = '1';  /* Payload 0. */
        buf[7] = '2';  /* Payload 1. */
        buf[8] = '3';  /* Payload 2. */
        buf[9] = '\0'; /* Payload 3. */
        socket_stub_input(buf, 10);

        /* Read the echo frame from the socket. */
        socket_stub_output(buf, 6);
        BTASSERT(strcmp(&buf[2], "123") == 0);
    }

    socket_stub_close_connection();
    socket_stub_wait_closed();

    return (0);
}

static int test_request_no_route(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input GET /missing.html on the connection socket. */
    str_p =
        "GET /missing.html HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    socket_stub_input(str_p, strlen(str_p));

    /* Read the GET response and verify it. */
    str_p =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 54\r\n"
        "\r\n"
        "The requested page '/missing.html' could not be found.";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    socket_stub_wait_closed();

    return (0);
}

static int test_request_url_too_long(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input a very long URL on the connection socket. */
    str_p =
        "GET /this/url/is/far/too/long/to/fit/in/the/statically/allocated/"
        "buffer/that/the/received/url/is/read/into//the/request/will/be/"
        "dropped.html HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";
    socket_stub_input(str_p, strlen(str_p));

    /* Read the GET response and verify it. */
    str_p =
        "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 32\r\n"
        "\r\n"
        "Failed to parse the HTTP header.";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    socket_stub_wait_closed();
    socket_stub_input_flush();

    return (0);
}

static int test_request_header_field_too_long(struct harness_t *harness_p)
{
    char *str_p;
    char buf[256];

    /* Input the accept answer. */
    socket_stub_accept();

    /* Input a very long header field on the connection socket. */
    str_p =
        "GET /foo.html HTTP/1.1\r\n"
        "User-Agent: TestcaseRequestIndexTestcaseRequestIndex"
        "TestcaseRequestIndexTestcaseRequestIndex"
        "TestcaseRequestIndexTestcaseRequestIndex"
        "TestcaseRequestIndexTestcaseRequestIndex\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";
    socket_stub_input(str_p, strlen(str_p));

    /* Read the GET response and verify it. */
    str_p =
        "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 32\r\n"
        "\r\n"
        "Failed to parse the HTTP header.";

    socket_stub_output(buf, strlen(str_p));
    buf[strlen(str_p)] = '\0';
    BTASSERT(strcmp(buf, str_p) == 0);

    socket_stub_wait_closed();
    socket_stub_input_flush();

    return (0);
}

static int test_stop(struct harness_t *harness_p)
{
    BTASSERT(http_server_stop(&foo) == 0);

    return (0);
}

static int test_https_start(struct harness_t *harness_p)
{
#if CONFIG_HTTP_SERVER_SSL == 1
    struct ssl_context_t context;
    static struct http_server_listener_t listener = {
        .address_p = "127.0.0.1",
        .port = 443,
        .thrd = {
            .name_p = "https_listener",
            .stack = {
                .buf_p = https_listener_stack,
                .size = sizeof(https_listener_stack)
            }
        }
    };
    static struct http_server_connection_t connections[] = {
        {
            .thrd = {
                .name_p = "https_conn_0",
                .stack = {
                    .buf_p = https_connection_stack,
                    .size = sizeof(https_connection_stack)
                }
            }
        },
        {
            .thrd = {
                .name_p = NULL
            }
        }
    };

    BTASSERT(http_server_init(&foo,
                              &listener,
                              connections,
                              NULL,
                              routes,
                              request_404_not_found) == 0);
    BTASSERT(http_server_wrap_ssl(&foo, &context) == 0);

    BTASSERT(http_server_start(&foo) == 0);

    thrd_set_log_mask(listener.thrd.id_p, LOG_UPTO(DEBUG));
    thrd_set_log_mask(connections[0].thrd.id_p, LOG_UPTO(DEBUG));

    /* Less log clobbering. */
    thrd_sleep_us(100000);

    return (0);
#else
    return (1);
#endif
}

static int test_https_stop(struct harness_t *harness_p)
{
#if CONFIG_HTTP_SERVER_SSL == 1
    BTASSERT(http_server_stop(&foo) == 0);

    BTASSERT(ssl_open_counter == 6);
    BTASSERT(ssl_close_counter == 6);
    BTASSERT(ssl_write_counter == 9);
    BTASSERT(ssl_read_counter == 730);
    BTASSERT(ssl_size_counter == 0);

    return (0);
#else
    return (1);
#endif
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_start, "test_start" },
        { test_request_index, "test_request_index" },
        { test_request_index_with_query_string, "test_request_index_with_query_string" },
        { test_request_auth, "test_request_auth" },
        { test_request_form, "test_request_form" },
        { test_request_websocket, "test_request_websocket" },
        { test_request_no_route, "test_request_no_route" },
        { test_request_url_too_long, "test_request_url_too_long" },
        { test_request_header_field_too_long, "test_request_header_field_too_long" },
        { test_stop, "test_stop" },
        { test_https_start, "test_https_start" },
#if CONFIG_HTTP_SERVER_SSL == 1
        { test_request_index, "test_https_request_index" },
        { test_request_index_with_query_string, "test_https_request_index_with_query_string" },
        { test_request_auth, "test_https_request_auth" },
        { test_request_form, "test_https_request_form" },
        { test_request_websocket, "test_https_request_websocket" },
        { test_request_no_route, "test_https_request_no_route" },
#endif
        { test_https_stop, "test_https_stop" },
        { NULL, NULL }
    };

    sys_start();

    socket_stub_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
