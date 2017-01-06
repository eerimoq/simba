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

static int test_client(struct harness_t *harness_p)
{
    struct ssl_context_t context;
    struct ssl_socket_t ssl_socket;
    struct socket_t socket;
    struct inet_addr_t addr;
    char buf[8];

    /* Create a context with default settings. */
    BTASSERT(ssl_context_init(&context, ssl_protocol_tls_v1_0) == 0);
    BTASSERT(ssl_context_set_verify_mode(&context,
                                         ssl_verify_mode_cert_none_t) == 0);
    BTASSERT(ssl_context_set_verify_mode(&context,
                                         ssl_verify_mode_cert_required_t) == 0);

    /* Create a socket and connect to the server. */
    BTASSERT(socket_open_tcp(&socket) == 0);

    inet_aton("1.2.3.4", &addr.ip);
    addr.port = 1234;
    BTASSERT(socket_connect(&socket, &addr) == 0);

    /* Wrap the socket in SSL. */
    BTASSERT(ssl_socket_open(&ssl_socket, &context, &socket, 0) == 0);

    BTASSERT(ssl_socket_size(&ssl_socket) == 0);
    
    /* Transfer data to and from the server. */
    BTASSERT(ssl_socket_write(&ssl_socket, "hello", 6) == 6);
    BTASSERT(ssl_socket_read(&ssl_socket, &buf[0], 8) == 8);
    BTASSERT(strcmp("goodbye", buf) == 0);

    /* Close the SSL connection. */
    BTASSERT(ssl_socket_close(&ssl_socket) == 0);

    /* Close the connection. */
    BTASSERT(socket_close(&socket) == 0);

    BTASSERT(ssl_context_destroy(&context) == 0);

    return (0);
}

static int test_server(struct harness_t *harness_p)
{
    struct ssl_context_t context;
    struct ssl_socket_t ssl_socket;
    struct socket_t listener, socket;
    struct inet_addr_t addr;
    char buf[8];
    static char certificate[] = "";
    static char key[] = "";
    
    /* Create a context with default settings. */
    BTASSERT(ssl_context_init(&context, ssl_protocol_tls_v1_0) == 0);
    BTASSERT(ssl_context_load_cert_chain(&context,
                                         &certificate[0],
                                         &key[0]) == 0);

    /* Create a socket and connect to the server. */
    BTASSERT(socket_open_tcp(&listener) == 0);
    BTASSERT(socket_listen(&listener, 5) == 0);
    BTASSERT(socket_accept(&listener, &socket, &addr) == 0);

    /* Wrap the socket in SSL. */
    BTASSERT(ssl_socket_open(&ssl_socket, &context, &socket, 1) == 0);

    BTASSERT(chan_size(&ssl_socket) == 0);

    /* Transfer data to and from the server. */
    BTASSERT(chan_read(&ssl_socket, &buf[0], 6) == 6);
    BTASSERT(strcmp("hello", buf) == 0);
    BTASSERT(chan_write(&ssl_socket, "goodbye", 8) == 8);

    /* Close the SSL connection. */
    BTASSERT(ssl_socket_close(&ssl_socket) == 0);

    /* Close the connection. */
    BTASSERT(socket_close(&socket) == 0);

    BTASSERT(ssl_context_destroy(&context) == 0);

    return (0);
}

static int test_client_server_context(struct harness_t *harness_p)
{
    struct ssl_context_t context;
    struct ssl_socket_t ssl_socket;
    struct socket_t socket;

    /* Create a context with default settings. */
    BTASSERT(ssl_context_init(&context, ssl_protocol_tls_v1_0) == 0);

    /* Create a socket and connect to the server. */
    BTASSERT(socket_open_tcp(&socket) == 0);

    /* Wrap the socket in SSL, as a client. */
    BTASSERT(ssl_socket_open(&ssl_socket, &context, &socket, 0) == 0);

    /* Wrap the socket in SSL, as a server. Should fail. */
    BTASSERT(ssl_socket_open(&ssl_socket, &context, &socket, 1) == -1);

    /* Close the SSL connection. */
    BTASSERT(ssl_socket_close(&ssl_socket) == 0);

    BTASSERT(ssl_context_destroy(&context) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_client, "test_client" },
        { test_server, "test_server" },
        { test_client_server_context, "test_client_server_context" },
        { NULL, NULL }
    };

    sys_start();
    ssl_module_init();
    
    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
