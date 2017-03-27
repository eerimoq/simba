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

static int test_init(struct harness_t *harness)
{
    /* This function may be called multiple times. */
    BTASSERT(ssl_module_init() == 0);
    BTASSERT(ssl_module_init() == 0);
    
    return (0);
}

static int test_client(struct harness_t *harness_p)
{
    struct ssl_context_t context;
    struct ssl_socket_t ssl_socket;
    struct socket_t socket;
    struct inet_addr_t addr;
    char buf[8];
    const char *server_hostname_p;
    const char *cipher_p;
    const char *protocol_p;
    int number_of_secret_bits;

    /* Create a context with default settings. */
    BTASSERT(ssl_context_init(&context, ssl_protocol_tls_v1_0) == 0);
    BTASSERT(ssl_context_set_verify_mode(&context,
                                         ssl_verify_mode_cert_none_t) == 0);
    BTASSERT(ssl_context_set_verify_mode(&context,
                                         ssl_verify_mode_cert_required_t) == 0);
    BTASSERT(ssl_context_load_verify_location(&context, "foo") == 0);

    /* Create a socket and connect to the server. */
    BTASSERT(socket_open_tcp(&socket) == 0);

    inet_aton("1.2.3.4", &addr.ip);
    addr.port = 1234;
    BTASSERT(socket_connect(&socket, &addr) == 0);

    /* Wrap the socket in SSL. */
    BTASSERT(ssl_socket_open(&ssl_socket,
                             &context,
                             &socket,
                             0,
                             "client_foo") == 0);

    /* Test a few functions. */
    BTASSERT(ssl_socket_size(&ssl_socket) == 0);

    server_hostname_p = ssl_socket_get_server_hostname(&ssl_socket);
    BTASSERT(strcmp(server_hostname_p, "client_foo") == 0);

    BTASSERT(ssl_socket_get_cipher(&ssl_socket,
                                   &cipher_p,
                                   &protocol_p,
                                   &number_of_secret_bits) == 0);
    BTASSERT(strcmp(cipher_p, "TLS-RSA-WITH-AES-256-GCM-SHA384") == 0);
    BTASSERT(strcmp(protocol_p, "TLSv1.1") == 0);
    BTASSERT(number_of_secret_bits == -1);

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
    const char *server_hostname_p;
    const char *cipher_p;
    const char *protocol_p;
    int number_of_secret_bits;

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
    BTASSERT(ssl_socket_open(&ssl_socket,
                             &context,
                             &socket,
                             SSL_SOCKET_SERVER_SIDE,
                             NULL) == 0);

    /* Test a few functions. */
    BTASSERT(chan_size(&ssl_socket) == 0);

    server_hostname_p = ssl_socket_get_server_hostname(&ssl_socket);
    BTASSERT(server_hostname_p == NULL);

    BTASSERT(ssl_socket_get_cipher(&ssl_socket,
                                   &cipher_p,
                                   &protocol_p,
                                   &number_of_secret_bits) == 0);
    BTASSERT(strcmp(cipher_p, "TLS-RSA-WITH-AES-256-GCM-SHA384") == 0);
    BTASSERT(strcmp(protocol_p, "TLSv1.1") == 0);
    BTASSERT(number_of_secret_bits == -1);

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
    BTASSERT(ssl_socket_open(&ssl_socket, &context, &socket, 0, NULL) == 0);

    /* Wrap the socket in SSL, as a server. Should fail. */
    BTASSERT(ssl_socket_open(&ssl_socket,
                             &context,
                             &socket,
                             SSL_SOCKET_SERVER_SIDE,
                             NULL) == -1);

    /* Close the SSL connection. */
    BTASSERT(ssl_socket_close(&ssl_socket) == 0);

    BTASSERT(ssl_context_destroy(&context) == 0);

    return (0);
}

static int test_errors(struct harness_t *harness_p)
{
    struct ssl_context_t context;
    struct ssl_context_t context2;
    struct ssl_socket_t ssl_socket;
    struct ssl_socket_t ssl_socket2;
    struct socket_t socket;

    /* Out of resources. */
    BTASSERT(ssl_context_init(&context, ssl_protocol_tls_v1_0) == 0);
    BTASSERT(ssl_context_init(&context2, ssl_protocol_tls_v1_0) == -1);

    /* Out of resources. */
    BTASSERT(ssl_socket_open(&ssl_socket,
                             &context,
                             &socket,
                             0,
                             NULL) == 0);
    BTASSERT(ssl_socket_open(&ssl_socket2,
                             &context,
                             &socket,
                             0,
                             NULL) == -1);
    BTASSERT(ssl_socket_close(&ssl_socket) == 0);

    /* Setup failure in setup. */
    BTASSERT(ssl_socket_open(&ssl_socket,
                             &context,
                             &socket,
                             0,
                             NULL) == -1);

    /* Setup failure in handover. */
    BTASSERT(ssl_socket_open(&ssl_socket,
                             &context,
                             &socket,
                             0,
                             NULL) == -1);
    
    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_client, "test_client" },
        { test_server, "test_server" },
        { test_client_server_context, "test_client_server_context" },
        { test_errors, "test_errors" },
        { NULL, NULL }
    };

    sys_start();
    ssl_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
