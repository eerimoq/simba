/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

int ssl_open_counter = 0;
int ssl_close_counter = 0;
int ssl_write_counter = 0;
int ssl_read_counter = 0;
int ssl_size_counter = 0;

int ssl_module_init()
{
    return (0);
}

int ssl_socket_open(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    void *socket_p,
                    int flags,
                    const char *server_hostname_p)
{
    BTASSERT(self_p != NULL);
    BTASSERT(context_p != NULL);
    BTASSERT(socket_p != NULL);
    BTASSERT(flags & SSL_SOCKET_SERVER_SIDE);

    ssl_open_counter++;

    return (chan_init(&self_p->base,
                      (chan_read_fn_t)ssl_socket_read,
                      (chan_write_fn_t)ssl_socket_write,
                      (chan_size_fn_t)ssl_socket_size));
}

int ssl_socket_close(struct ssl_socket_t *self_p)
{
    ssl_close_counter++;

    return (0);
}

ssize_t ssl_socket_write(struct ssl_socket_t *self_p,
                         const void *buf_p,
                         size_t size)
{
    BTASSERT(self_p != NULL);

    ssl_write_counter++;

    return (socket_write(NULL, buf_p, size));
}

ssize_t ssl_socket_read(struct ssl_socket_t *self_p,
                        void *buf_p,
                        size_t size)
{
    BTASSERT(self_p != NULL);

    ssl_read_counter++;

    return (socket_read(NULL, buf_p, size));
}

ssize_t ssl_socket_size(struct ssl_socket_t *self_p)
{
    PRINT_FILE_LINE();
    BTASSERT(self_p != NULL);

    ssl_size_counter++;

    return (0);
}
