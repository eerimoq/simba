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

int http_websocket_server_init(struct http_websocket_server_t *self_p,
                               struct socket_t *socket_p)
{
    ASSERTN(self_p != NULL, EINVAL)
    ASSERTN(socket_p != NULL, EINVAL)

    self_p->socket_p = socket_p;

    return (0);
}

int http_websocket_server_handshake(struct http_websocket_server_t *self_p,
                                    struct http_server_request_t *request_p)
{
    ASSERTN(self_p != NULL, EINVAL)
    ASSERTN(request_p != NULL, EINVAL)

    char buf[160];
    char accept_key[29];
    const char *key_p;
    struct sha1_t sha;
    uint8_t hash[20];
    size_t size;
    static const char secret[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    /* Only the GET action is supported. */
    if (request_p->action != http_server_request_action_get_t) {
        return (-1);
    }

    /* Sec-Websocket-Key is required. */
    if (request_p->headers.sec_websocket_key.present == 0) {
        log_object_print(NULL,
                         LOG_DEBUG,
                         OSTR("Missing HTTP header field: Sec-Websocket-Key\r\n"));

        return (-1);
    }

    /* Calculate the accept key. */
    key_p = request_p->headers.sec_websocket_key.value;
    size = strlen(key_p);
    memcpy(buf, key_p, size);
    memcpy(&buf[size], secret, sizeof(secret));

    sha1_init(&sha);
    sha1_update(&sha, buf, size + sizeof(secret) - 1);
    sha1_digest(&sha, hash);

    (void)base64_encode(accept_key, hash, sizeof(hash));
    accept_key[sizeof(accept_key) - 1] = '\0';

    /* Format and write the websocket handshake response to the
       client. */
    size = std_sprintf(buf,
                       FSTR("HTTP/1.1 101 Switching Protocols\r\n"
                            "Upgrade: websocket\r\n"
                            "Connection: Upgrade\r\n"
                            "Sec-WebSocket-Accept: %s\r\n"
                            "\r\n"),
                       accept_key);

    if (socket_write(self_p->socket_p, buf, size) != size) {
        return (-EIO);
    }

    return (0);
}

ssize_t http_websocket_server_read(struct http_websocket_server_t *self_p,
                                   int *type_p,
                                   void *buf_p,
                                   size_t size)
{
    ASSERTN(self_p != NULL, EINVAL)
    ASSERTN(buf_p != NULL, EINVAL)
    ASSERTN(size > 0, EINVAL)

    uint8_t buf[16], *b_p = buf_p;
    size_t payload_left, left = size, n;
    int fin = 0;
    uint8_t masking_key[4];
    int pos;
    int i;

    while (fin == 0) {
        /* Read the next frame. */
        if (socket_read(self_p->socket_p, buf, 2) != 2) {
            return (-EIO);
        }

        fin = (buf[0] & INET_HTTP_WEBSOCKET_FIN);
        payload_left = (buf[1] & ~INET_HTTP_WEBSOCKET_MASK);

        if (payload_left == 126) {
            if (socket_read(self_p->socket_p, &buf[2], 2) != 2) {
                return (-EIO);
            }

            payload_left = ((uint32_t)(buf[2]) << 8 | buf[3]);
        } else if (payload_left == 127) {
            if (socket_read(self_p->socket_p, &buf[2], 8) != 8) {
                return (-EIO);
            }

            payload_left = ((uint32_t)(buf[6]) << 24
                    | (uint32_t)(buf[7]) << 16
                    | (uint32_t)(buf[8]) << 8
                    | buf[9]);
        }

        /* Read the mask. */
        if (buf[1] & INET_HTTP_WEBSOCKET_MASK) {
            if (socket_read(self_p->socket_p,
                            &masking_key[0],
                            sizeof(masking_key)) != sizeof(masking_key)) {
                return (-EIO);
            }
        } else {
            memset(&masking_key[0], 0, sizeof(masking_key));
        }

        /* Read the payload. */
        pos = 0;

        while ((payload_left > 0) && (left > 0)) {
            if (payload_left < left) {
                n = payload_left;
            } else {
                n = left;
            }

            if (socket_read(self_p->socket_p, b_p, n) != n) {
                return (-1);
            }

            for (i = 0; i < n; i++) {
                *b_p++ ^= masking_key[pos % 4];
                pos++;
            }

            left -= n;
            payload_left -= n;
        }

        /* Discard leftover data. */
        while (payload_left > 0) {
            if (socket_read(self_p->socket_p, buf, 1) != 1) {
                return (-1);
            }

            payload_left--;
        }
    }

    return (size - left);
}

ssize_t http_websocket_server_write(struct http_websocket_server_t *self_p,
                                    int type,
                                    const void *buf_p,
                                    uint32_t size)
{
    ASSERTN(self_p != NULL, EINVAL)
    ASSERTN(buf_p != NULL, EINVAL)
    ASSERTN(size > 0, EINVAL)

    uint8_t header[16];
    size_t header_size = 2;

    header[0] = (INET_HTTP_WEBSOCKET_FIN | type);

    if (size < 126) {
        header[1] = size;
    } else if (size < 65536) {
        header[1] = 126;
        header[2] = ((size >> 8) & 0xff);
        header[3] = ((size >> 0) & 0xff);
        header_size += 2;
    } else {
        header[1] = 127;
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

    if (socket_write(self_p->socket_p,
                     header,
                     header_size) != header_size) {
        return (-EIO);
    }

    if (socket_write(self_p->socket_p, buf_p, size) != size) {
        return (-EIO);
    }

    return (size);
}
