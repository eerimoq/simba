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

#define SERVER_IP 192.168.0.4
#define SERVER_PORT 10023

/**
 * Copied from 'server.crt'.
 */
static const char certificate[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDBjCCAe4CCQDxnqw02a38fDANBgkqhkiG9w0BAQsFADBFMQswCQYDVQQGEwJB\n"
    "VTETMBEGA1UECAwKU29tZS1TdGF0ZTEhMB8GA1UECgwYSW50ZXJuZXQgV2lkZ2l0\n"
    "cyBQdHkgTHRkMB4XDTE2MTIxODA4Mzg1MloXDTE3MTIxODA4Mzg1MlowRTELMAkG\n"
    "A1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3RhdGUxITAfBgNVBAoMGEludGVybmV0\n"
    "IFdpZGdpdHMgUHR5IEx0ZDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n"
    "ALYNY/GZ2QNYdVFv39vMxFkXp7AogTYfVSqFoJwPneaXAt24aiKDwHO3OO3W8nYe\n"
    "AwBZ9Jl1zQKA5wafc5uaRUH4Nuq1INiQZQ6gYL74KaBjNoX8TgLBOjci2Kea8HAZ\n"
    "Ao/KY88UE1CG7sErJjuZ5SyKYGzdFuPek+jny8OxzqpPqP7ORrDqYKbFOOpUUhHZ\n"
    "kPeVMhbWR2OSn052RtO63rtDJ49LltFy+8KXV6SaEu7Zuyk4WaBObSXLlMFDZKp9\n"
    "ZlXYOKSZ2xGcKtgnMOTous1j/XxGTd0Xlbr2TZi/cfq7xFqfWRjfWiTX9w5d/KWd\n"
    "h1jv9lrNsRfqK86FKFW2SOECAwEAATANBgkqhkiG9w0BAQsFAAOCAQEAD33b4qXN\n"
    "BA13KsOSba7zCla8Z5dKk8JOwTp0wM7xqTWzR/eN7xkaQ6NFZavsbGACtHbGs2vU\n"
    "DVRsTzTzn/0RVoCaEqYsELwXoaz7sUBZoc39+kwbC5BCvZlHLPRBkEy9nAehqIg6\n"
    "bbzbVObZ1gqI1GWXtVONv65n1W8hjhTa2L841EMCVKKeR5sg1Pts0W7F3e9K2r7j\n"
    "NumMWFa8RxDPvHwfE0zXzJYbXLAbDClvdjNboAq/jZu6f1O71yAJkPNCZKHJKFKO\n"
    "2p4hsmEigByb2jr0BB6Bsmdcf1RyRwIemJ5fXauUsvJydYcvklo0xcpqQrTkhIzb\n"
    "UoNMyyG9ikCJjA==\n"
    "-----END CERTIFICATE-----\n";

int main()
{
    struct ssl_context_t context;
    struct ssl_socket_t ssl_sock;
    struct socket_t sock;
    struct inet_addr_t addr;
    char buf[16];

    sys_start();

    ssl_module_init();

    /* Create a SSL context. */
    if (ssl_context_init(&context, ssl_protocol_tls_v1_0) != 0) {
        std_printf(FSTR("ssl_context_init() failed\r\n"));
        return (-1);
    }

    /* Require a certificate from the server. We used a self-signed
       certificate so here ca_certs_p must be the server certificate
       itself. */
    if (ssl_context_load_verify_location(&context,
                                         &certificate[0]) != 0) {
        std_printf(FSTR("ssl_context_load_verify_location() failed\r\n"));
        return (-1);
    }

    /* Create a listener socket. */
    if (socket_open_tcp(&sock) != 0) {
        std_printf(FSTR("socket_open_tcp() failed\r\n"));
        return (-1);
    }

    std_printf(FSTR("Connecting to server at "
                    STRINGIFY(SERVER_IP) ":" STRINGIFY(SERVER_PORT)
                    "...\r\n"));

    inet_aton(STRINGIFY(SERVER_IP), &addr.ip);
    addr.port = SERVER_PORT;

    if (socket_connect(&sock, &addr) != 0) {
        std_printf(FSTR("socket_connect() failed\r\n"));
        return (-1);
    }

    std_printf(FSTR("Connected.\r\n"));

    /* Wrap the socket in SSL. */
    if (ssl_socket_open(&ssl_sock, &context, &sock, 0) == 0) {
        /* Write data to the socket. */
        std_printf(FSTR("write: Hello!\r\n"));

        if (ssl_socket_write(&ssl_sock, "Hello!", 6) == 6) {
            if (ssl_socket_read(&ssl_sock, &buf[0], 8) == 8) {
                buf[8] = '\0';
                std_printf(FSTR("read: %s\r\n"), &buf[0]);
            } else {
                std_printf(FSTR("ssl_socket_read() failed\r\n"));
            }
        } else {
            std_printf(FSTR("ssl_socket_write() failed\r\n"));
        }

        /* Close the connection. */
        if (ssl_socket_close(&ssl_sock) != 0) {
            std_printf(FSTR("ssl_socket_close() failed\r\n"));
        }
    } else {
        std_printf(FSTR("ssl_socket_open() failed\r\n"));
    }

    socket_close(&sock);

    return (0);
}
