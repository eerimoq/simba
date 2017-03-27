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

static int readline(struct http_websocket_client_t *self_p,
                    const FAR char *expected_p,
                    size_t size)
{
    int pos;
    int match = 1;
    char curr, prev;

    pos = 0;
    prev = '\0';

    while (1) {
        /* Get a byte from the stream. */
        if (socket_read(&self_p->server.socket,
                        &curr,
                        sizeof(curr)) != sizeof(curr)) {
            return (-EIO);
        }

        /* Line ending. */
        if ((prev == '\r') && (curr == '\n')) {
            break;
        }

        /* Compare the read byte to the expected byte. */
        if (expected_p != NULL) {
            if (pos < size) {
                if (curr != expected_p[pos]) {
                    match = 0;
                }
            }
        }

        prev = curr;
        pos++;
    }

    if (match == 0) {
        return (pos);
    } else {
        if ((pos - 1) == size) {
            return (0);
        }
    }

    return (-1);
}

int http_websocket_client_init(struct http_websocket_client_t *self_p,
                               const char *host_p,
                               int port,
                               const char *path_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(host_p != NULL, EINVAL);
    ASSERTN(path_p != NULL, EINVAL);

    self_p->server.host_p = host_p;
    self_p->server.port = port;
    self_p->path_p = path_p;

    return (0);
}

int http_websocket_client_connect(struct http_websocket_client_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int res;
    struct inet_addr_t server_addr;

    /* Open a TCP socket and connect to the server. */
    if (socket_open_tcp(&self_p->server.socket) != 0) {
        return (-EIO);
    }

    if (socket_connect(&self_p->server.socket, &server_addr) != 0) {
        (void)socket_close(&self_p->server.socket);

        return (-EIO);
    }

    /* Perform the handshake with the server. */
    std_fprintf(&self_p->server.socket,
                FSTR("GET %s HTTP/1.1\r\n"
                     "Host: %s\r\n"
                     "Upgrade: WebSocket\r\n"
                     "Connection: Upgrade\r\n"
                     "Origin: SimbaWebSocketClient\r\n"
                     "\r\n"),
                self_p->path_p,
                self_p->server.host_p);

    /* Expect a positive response. */
    if (readline(self_p, FSTR("HTTP/1.1 101"), 12) != 0) {
        return (-1);
    }

    /* Find the empty line at the end of the message. */
    while (1) {
        res = readline(self_p, FSTR(""), 0);

        if (res == 0) {
            break;
        } else if (res > 0) {
            continue;
        } else {
            return (res);
        }
    }

    return (0);
}

int http_websocket_client_disconnect(struct http_websocket_client_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (socket_close(&self_p->server.socket));
}

ssize_t http_websocket_client_read(struct http_websocket_client_t *self_p,
                                   void *buf_p,
                                   size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    uint8_t buf[16];
    size_t left = size, n;

    while (left > 0) {
        /* Read buffered frame data. */
        if (self_p->frame.left > 0) {
            if (left > self_p->frame.left) {
                n = self_p->frame.left;
            } else {
                n = left;
            }

            if (socket_read(&self_p->server.socket, buf_p, n) != n) {
                return (-EIO);
            }

            self_p->frame.left -= n;
            left -= n;
        }

        if (left > 0) {
            /* Read the next frame. */
            if (socket_read(&self_p->server.socket, buf, 2) != 2) {
                return (-EIO);
            }

            self_p->frame.left = (buf[1] & ~INET_HTTP_WEBSOCKET_MASK);

            if (self_p->frame.left == 126) {
                if (socket_read(&self_p->server.socket, &buf[2], 2) != 2) {
                    return (-EIO);
                }

                self_p->frame.left = ((uint32_t)(buf[2]) << 8 | buf[3]);
            } else if (self_p->frame.left == 127) {
                if (socket_read(&self_p->server.socket, &buf[2], 8) != 8) {
                    return (-EIO);
                }

                self_p->frame.left = ((uint32_t)(buf[6]) << 24
                                      | (uint32_t)(buf[7]) << 16
                                      | (uint32_t)(buf[8]) << 8
                                      | buf[9]);
            }

            if (buf[1] & INET_HTTP_WEBSOCKET_MASK) {
                if (socket_read(&self_p->server.socket, buf, 4) != 4) {
                    return (-EIO);
                }
            }
        }
    }

    return (size);
}

ssize_t http_websocket_client_write(struct http_websocket_client_t *self_p,
                                    int type,
                                    const void *buf_p,
                                    uint32_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    const uint8_t masking_key[4] = { 0x00, 0x00, 0x00, 0x00 };
    uint8_t header[16];
    size_t header_size = 2;

    header[0] = (INET_HTTP_WEBSOCKET_FIN | type);

    if (size < 126) {
        header[1] = (INET_HTTP_WEBSOCKET_MASK | size);
    } else if (size < 65536) {
        header[1] = (INET_HTTP_WEBSOCKET_MASK | 126);
        header[2] = ((size >> 8) & 0xff);
        header[3] = ((size >> 0) & 0xff);
        header_size += 2;
    } else {
        header[1] = (INET_HTTP_WEBSOCKET_MASK | 127);
        header[2] = 0;
        header[3] = 0;
        header[4] = 0;
        header[5] = 0;
        header[6] = ((size >> 24) & 0xff);
        header[7] = ((size >> 16) & 0xff);
        header[8] = ((size >>  8) & 0xff);
        header[9] = ((size >>  0) & 0xff);
        header_size += 8;
    }

    header[header_size + 0] = masking_key[0];
    header[header_size + 1] = masking_key[1];
    header[header_size + 2] = masking_key[2];
    header[header_size + 3] = masking_key[3];
    header_size += 4;

    if (socket_write(&self_p->server.socket,
                     header,
                     header_size) != header_size) {
        return (-EIO);
    }

    if (socket_write(&self_p->server.socket, buf_p, size) != size) {
        return (-EIO);
    }

    return (size);
}
