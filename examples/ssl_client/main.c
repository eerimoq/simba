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
    "MIIDCjCCAfICCQCoXfSQydXTczANBgkqhkiG9w0BAQsFADBHMQswCQYDVQQGEwJT\n"
    "RTETMBEGA1UECAwKU29tZS1TdGF0ZTEOMAwGA1UECgwFU2ltYmExEzARBgNVBAMM\n"
    "CmZvb2Jhci5vcmcwHhcNMTcwMTA3MTgxOTQ3WhcNMTgwMTA3MTgxOTQ3WjBHMQsw\n"
    "CQYDVQQGEwJTRTETMBEGA1UECAwKU29tZS1TdGF0ZTEOMAwGA1UECgwFU2ltYmEx\n"
    "EzARBgNVBAMMCmZvb2Jhci5vcmcwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n"
    "AoIBAQDugoc9amynDB/ODJzh2aqB6cnubSJEPlgB87jNG0akcbmUFt6BhPhMPSoP\n"
    "esHWl5OWscoga0cKrPURmMVVhfaeZLQGmrv5N4/liVlwae1n0gUEruX4d6MqSSDW\n"
    "3C/WKjCn647udZwyzCvyrvPOq0qAzaxR4EFRdwjSEPO5sDw2zxeTjGW2WxaH9PEu\n"
    "C8vaNqTsLYl3YBkR3zVCbuQXTQhStsv3gT4Yhz2wJpY0yyWyDiaBkGKpdxJQiNAd\n"
    "x5JKSqtRshlYZM3+cdKLywNoYUnezp6Wm4mzz09TCFv+esJ0h7/6pMdVjhxLsAg5\n"
    "ZbZyrtNIapN07AjIJS4qjkJ/HUC3AgMBAAEwDQYJKoZIhvcNAQELBQADggEBAGHS\n"
    "U5AvDESzTNoak5HHx166bp5+bvECvJg45GMDGhqKeFoOlR34u2b+wyTm/piz3nYJ\n"
    "12kn+BbG/PwGfndL0kZYRz46fY8Rf+MxCFfcglweDJhA6ULNpera11OC35Q/lKn5\n"
    "M6w6eQkZMB4VqwigvDGHGpXRTIhJHHoR2VFBFGoPTLrXilChUpiXi9DmuYXJ/19x\n"
    "sxOVwvvO/m/6g68G+uZYUoCsQsKllM2fgdNLTzbYvnFtsq5QnZS8m7CoZgEy2c3m\n"
    "VrrPsfwmyhwejDawjz2epSLNXaaDeSz4g1cQes+oehaA1IwFfKhb9tdiWUm48VuU\n"
    "oaFZ8e1HofYUy+65pws=\n"
    "-----END CERTIFICATE-----\n";

int main()
{
    struct ssl_context_t context;
    struct ssl_socket_t ssl_sock;
    struct socket_t sock;
    struct inet_addr_t addr;
    const char *cipher_p;
    const char *protocol_p;
    int number_of_secret_bits;
    char buf[16];
    int i;
    
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

    /* Create a socket. */
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
    if (ssl_socket_open(&ssl_sock,
                        &context,
                        &sock,
                        0,
                        "foobar.org") == 0) {
        /* Print connection information. */
        std_printf(FSTR("Server hostname: %s\r\n"), ssl_socket_get_server_hostname(&ssl_sock));
        ssl_socket_get_cipher(&ssl_sock,
                              &cipher_p,
                              &protocol_p,
                              &number_of_secret_bits);
        std_printf(FSTR("Cipher: %s\r\n"), cipher_p);
        std_printf(FSTR("Protocol: %s\r\n"), protocol_p);
        std_printf(FSTR("Number of secret bits: %d\r\n"), number_of_secret_bits);
        
        /* Write data to the socket. */
        for (i = 0; i < 50; i++) {
            std_printf(FSTR("write: Hello!\r\n"));
            
            if (ssl_socket_write(&ssl_sock, "Hello!", 6) == 6) {
                if (ssl_socket_read(&ssl_sock, &buf[0], 8) == 8) {
                    buf[8] = '\0';
                    std_printf(FSTR("read: %s\r\n"), &buf[0]);
                } else {
                    std_printf(FSTR("ssl_socket_read() failed\r\n"));
                    break;
                }
            } else {
                std_printf(FSTR("ssl_socket_write() failed\r\n"));
                break;
            }
        }
        
        /* Close the connection. */
        if (ssl_socket_close(&ssl_sock) != 0) {
            std_printf(FSTR("ssl_socket_close() failed\r\n"));
        }
    } else {
        std_printf(FSTR("ssl_socket_open() failed\r\n"));
    }

    socket_close(&sock);

    std_printf(FSTR("Connection closed.\r\n"));
    
    return (0);
}
