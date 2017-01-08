:mod:`ssl` --- Secure socket layer
==================================

.. module:: ssl
   :synopsis: Secure socket layer.

SSL/TLS based on mbedTLS. Server side sockets works, but not client
side.

.. warning:: This module may lead to a false sense of security, as it
             is implemented by a TLS/SSL novice, me. Use with care!

Simplified server and client side examples to illustrate how to use
the module. All error checking is left out to make the example easier
to understand. There are links to the full examples further down in
this document.

Server side:

.. code-block:: c

   /* Create the SSL context. */
   ssl_context_init(&context, ssl_protocol_tls_v1_0);
   ssl_context_load_cert_chain(&context, &certificate[0], &key[0]);

   /* Create the TCP listener socket. */
   socket_open_tcp(&listener_sock);
   socket_bind(&listener_sock, &addr);
   socket_listen(&listener_sock, 5);

   /* Accept a client.*/
   socket_accept(&listener_sock, &sock, &addr);
   ssl_socket_open(&ssl_sock,
                   &context,
                   &sock,
                   SSL_SOCKET_SERVER_SIDE,
                   NULL);

   /* Communicate with the client. */
   ssl_socket_read(&ssl_sock, &buf[0], 6);
   ssl_socket_write(&ssl_sock, "Goodbye!", 8);
   ssl_socket_close(&ssl_sock);
   socket_close(&sock);

Client side:

.. code-block:: c

   /* Create the SSL context. */
   ssl_context_init(&context, ssl_protocol_tls_v1_0);
   ssl_context_load_verify_location(&context, &certificate[0]);

   /* Create the TCP socket and connect to the server. */
   socket_open_tcp(&sock);
   socket_connect(&sock, &addr);
   ssl_socket_open(&ssl_sock,
                   &context,
                   &sock,
                   0,
                   "foobar.org");

   /* Communicate with the client. */
   ssl_socket_write(&ssl_sock, "Hello!", 6);
   ssl_socket_read(&ssl_sock, &buf[0], 8);
   ssl_socket_close(&ssl_sock);
   socket_close(&ssl_sock);

----------------------------------------------

Source code: :github-blob:`src/inet/ssl.h`, :github-blob:`src/inet/ssl.c`

Test code: :github-blob:`tst/inet/ssl/main.c`,

Test coverage: :codecov:`src/inet/ssl.c`

Example code: :github-blob:`examples/ssl_client/main.c`, :github-blob:`examples/ssl_server/main.c`

----------------------------------------------

.. doxygenfile:: inet/ssl.h
   :project: simba
