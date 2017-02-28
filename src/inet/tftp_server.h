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

#ifndef __INET_TFTP_SERVER_H__
#define __INET_TFTP_SERVER_H__

#include "simba.h"

struct tftp_server_t {
    struct inet_addr_t addr;
    struct socket_t listener;
    int timeout_ms;
    const char *name_p;
    const char *root_p;
    void *stack_p;
    size_t stack_size;
    struct thrd_t *thrd_p;
};

/**
 * Initialize given TFTP server.
 *
 * @param[in, out] self_p TFTP server to initialize.
 * @param[in] addr_p Ip address and port of the server.
 * @param[in] timeout_ms Packet reception timeout.
 * @param[in] name_p Name of the server thread.
 * @param[in] root_p File system root path.
 * @param[in] stack_p Server thread stack.
 * @param[in] stack_size Server thread stack size.
 *
 * @return zero(0) or negative error code.
 */
int tftp_server_init(struct tftp_server_t *self_p,
                     struct inet_addr_t *addr_p,
                     int timeout_ms,
                     const char *name_p,
                     const char *root_p,
                     void *stack_p,
                     size_t stack_size);

/**
 * Start given TFTP server.
 *
 * @param[in] self_p TFTP server to start.
 *
 * @return zero(0) or negative error code.
 */
int tftp_server_start(struct tftp_server_t *self_p);

#endif
