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

#define IP 192.168.0.7
#define PORT 10023

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

/**
 * Copied from 'server.key'.
 */
static const char key[] =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEpQIBAAKCAQEAtg1j8ZnZA1h1UW/f28zEWRensCiBNh9VKoWgnA+d5pcC3bhq\n"
    "IoPAc7c47dbydh4DAFn0mXXNAoDnBp9zm5pFQfg26rUg2JBlDqBgvvgpoGM2hfxO\n"
    "AsE6NyLYp5rwcBkCj8pjzxQTUIbuwSsmO5nlLIpgbN0W496T6OfLw7HOqk+o/s5G\n"
    "sOpgpsU46lRSEdmQ95UyFtZHY5KfTnZG07reu0Mnj0uW0XL7wpdXpJoS7tm7KThZ\n"
    "oE5tJcuUwUNkqn1mVdg4pJnbEZwq2Ccw5Oi6zWP9fEZN3ReVuvZNmL9x+rvEWp9Z\n"
    "GN9aJNf3Dl38pZ2HWO/2Ws2xF+orzoUoVbZI4QIDAQABAoIBAQCSDSIONK/eODw+\n"
    "bjM4ogkvtIOKFqS6p95qc7PLH9g8ow/gks9LC0n9S5O3c4cm7aLYyFIsP06OYJ13\n"
    "ObrErbo8V4avO0qzkvwQBOg4rNywWIfoa+al3SD4FNOdevbD57yfYO47tk5cfzPR\n"
    "8u0ESc2KsyTykViIzQr6geN+BwrWpB+vP2DLdoDlhVwtMnlbViRFjsFOhxpfrgA6\n"
    "dSWRrfD7qv6hA62BEttk+L30vN/uLLLdLPLHtqiNwp3ri7AkLf/WEpACvHwEvCdg\n"
    "2dGNrQHLUpF0WPuIEV6/R0ps3wXcfLJB9ZWX68U2lGb+w1ue7o8VEiTKfGVTK6nD\n"
    "xSyU0FEBAoGBAO8GQoBlTZj771YIZ0PxWn62Gk+F1E5paUKLRsoaeWzLyxEawnwS\n"
    "crShxWUsK24N/b6JsSwRxJe7uSplKP9VBvnjxecUc+9wXpIjd6pRSjBcSqliCMDO\n"
    "i72SziCu7SOkUU/wfwzmlRgWveRAixOyRUTI/rn+9X69t6uojBxsoQsxAoGBAML7\n"
    "T7H3xVPgWFgbZuaDZiELZ3uGt6ggsEPB9OhUmuQhklwSpY6CSsHlsc/QqQLOk+33\n"
    "yksLn0VRUly8Qk3Yn9W1jzvRAt1N1TzU6bV30bUUFW8y/NjHr/46y6P+sqOWVsA9\n"
    "73kWLKYQS3eXruWH9P6gUNrWoF09tCJsATIS+0yxAoGBAIemLM2DMFOW/s/7eudz\n"
    "LKFmpW3mDIPnjMYoFqeR+MFGstrlLeSN760VddoiBA+0PwIIUJUrDD/0WHR37plh\n"
    "XtEoc+Ldl6IWSoVzPXFaKtlhNzoDmeYaEfInQ3YG8CvfEApm+SOQJKMEHQZRntwx\n"
    "ut/7lZxm1ln0Py1bPRDQkWCxAoGBAK0FrdbTc7vWnQ6yYkGz4HD6Wb338gmz2DaI\n"
    "avev/DLsx6AEu/0WCZi+Z2duVkwzXHoTJOQrUA6G93trY7sIu+08y/nco5oWc18j\n"
    "4LAzv1fclpiyukT0DmyfaA4C+irIQfsd+t+MmdpTI2TgqUbtuq4pPRa01mFnW+4f\n"
    "wX/Z8D5BAoGALEvKgnYKO74qcrgG58FylvaXwyv9oaDEioRZ2fLHbcTU6M2yTdrA\n"
    "YlbSZl9moMeV18cHr55PzL/qxFQuoSQ26kA7Fag1MYLvakyL3Y0QnhTI+/JvqFLZ\n"
    "CTsgcsS/vH3Kvg+o/FsQmz/T0Pk+Q0Zb94qnlK30gdY6AQFVb2A71p0=\n"
    "-----END RSA PRIVATE KEY-----\n";

int main()
{
    struct ssl_context_t context;
    struct ssl_socket_t ssl_client_sock;
    struct socket_t listener_sock, client_sock;
    struct inet_addr_t addr;
    char buf[8];

    sys_start();

    ssl_module_init();

    /* Create a SSL context. */
    if (ssl_context_init(&context, ssl_protocol_tls_v1_0) != 0) {
        std_printf(FSTR("ssl_context_init() failed\r\n"));
        return (-1);
    }

    if (ssl_context_load_cert_chain(&context,
                                    &certificate[0],
                                    &key[0]) != 0) {
        std_printf(FSTR("ssl_context_load_cert_chain() failed\r\n"));
        return (-1);
    }

    /* Create a listener socket. */
    if (socket_open_tcp(&listener_sock) != 0) {
        std_printf(FSTR("socket_open_tcp() failed\r\n"));
        return (-1);
    }

    inet_aton(STRINGIFY(IP), &addr.ip);
    addr.port = PORT;

    if (socket_bind(&listener_sock, &addr) != 0) {
        std_printf(FSTR("socket_bind() failed\r\n"));
        return (-1);
    }

    if (socket_listen(&listener_sock, 5) != 0) {
        std_printf(FSTR("socket_listen() failed\r\n"));
        return (-1);
    }

    while (1) {
        std_printf(FSTR("Waiting for a client to connect to "
                        STRINGIFY(IP) ":" STRINGIFY(PORT) "...\r\n"));

        if (socket_accept(&listener_sock, &client_sock, &addr) != 0) {
            std_printf(FSTR("socket_accept() failed\r\n"));
            continue;
        }

        std_printf(FSTR("Client accepted.\r\n"));

        /* Wrap the socket in SSL. */
        if (ssl_socket_open(&ssl_client_sock,
                            &context,
                            &client_sock,
                            SSL_SOCKET_SERVER_SIDE,
                            NULL) == 0) {
            /* Read data from the client socket. */
            if (ssl_socket_read(&ssl_client_sock, &buf[0], 6) == 6) {
                buf[6] = '\0';
                std_printf(FSTR("read: '%s'\r\n"), buf);

                /* Write data to the client socket. */
                if (ssl_socket_write(&ssl_client_sock, "Goodbye!", 8) != 8) {
                    std_printf(FSTR("ssl_socket_write() failed\r\n"));
                }
            } else {
                std_printf(FSTR("ssl_socket_read() failed\r\n"));
            }

            /* Close the connection. */
            if (ssl_socket_close(&ssl_client_sock) != 0) {
                std_printf(FSTR("ssl_socket_close() failed\r\n"));
            }
        } else {
            std_printf(FSTR("ssl_socket_open() failed\r\n"));
        }

        socket_close(&client_sock);
    }

    return (0);
}
