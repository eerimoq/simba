/**
 * @file kernel/sock.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __KERNEL_SOCK_H__
#define __KERNEL_SOCK_H__

#include "simba.h"

/* address family.*/
#define AF_INET  2

/* socket types (TCP/UDP).*/
#define SOCK_TYPE_STREAM    1
#define SOCK_TYPE_DGRAM     2

struct sockaddr_t {
};

struct sock_t {
    struct chan_t base;
    const struct sock_proto_t *proto;
    union {
        struct inet_tcp_t tcp;
        struct inet_udp_t udp;
    } pcb;
};

/**
 * Open socket.
 * @param[out] sock Opened socket.
 * @param[in] domain AF_INET.
 * @param[in] type Socket type one of SOCK_TYPE_DGRAM (UDP)
 *                 and SOCK_TYPE_STREAM (TCP).
 * @param[in] protocol Unused.
 * @return zero(0) or negative error code.
 */
int sock_open(struct sock_t *sock,
              int domain,
              int type,
              int protocol);

/**
 * Close given socket.
 * @param[in] sock Socket.
 * @return zero(0) or negative error code.
 */
int sock_close(struct sock_t *sock);

/**
 * Bind given local address to socket.
 * @param[in] sock Socket.
 * @param[in] addr Local address.
 * @param[in] addrlen Local address length.
 * @return zero(0) or negative error code.
 */
int sock_bind(struct sock_t *sock,
              const struct sockaddr_t *addr,
              size_t addrlen);

/**
 * Listen for conntctions. Only used by TCP
 * sockets.
 * @param[in] sock Socket.
 * @param[in] backlog Unused.
 * @return zero(0) or negative error code.
 */
int sock_listen(struct sock_t *sock, int backlog);

/**
 * Connect to given remote address. Connecting a UDP
 * socket sets the default remote address for outgoing
 * datagrams. For TCP a three-way handshake with the remote
 * is initiated.
 * @param[in] sock Socket.
 * @param[in] addr Remote address.
 * @param[in] addrlen Remote address length.
 * @return zero(0) or negative error code.
 */
int sock_connect(struct sock_t *sock,
                 const struct sockaddr_t *addr,
                 size_t addrlen);

/**
 * Accept client connect attempt.
 * @param[in] sock Socket.
 * @param[out] accepted New client socket.
 * @param[out] addr Remote address.
 * @param[out] addrlen Remote address length.
 * @return zero(0) or negative error code.
 */
int sock_accept(struct sock_t *sock,
                struct sock_t *accepted,
                struct sockaddr_t *addr,
                size_t *addrlen);

/**
 * Write data to socket.
 * @param[in] sock Socket.
 * @param[in] buf Buffer to send.
 * @param[in] size Size of buffer to send.
 * @param[in] flags Unused.
 * @param[in] remote_addr Remote address to send the data to.
 * @param[in] addrlen Length of remote_addr.
 * @return Number of sent bytes or negative error code.
 */
ssize_t sock_sendto(struct sock_t *sock,
                    const void *buf_p,
                    size_t size,
                    int flags,
                    const struct sockaddr_t *remote_addr,
                    size_t addrlen);

/**
 * Read data from socket.
 * @param[in] sock Socket.
 * @param[in] buf Buffer to read into.
 * @param[in] size Size of buffer to read.
 * @param[in] flags Unused.
 * @param[in] remote_addr Remote address to receive data from.
 * @param[in] addrlen Length of remote_addr.
 * @return Number of received bytes or negative error code.
 */
ssize_t sock_recvfrom(struct sock_t *sock,
                      void *buf_p,
                      size_t size,
                      int flags,
                      struct sockaddr_t *remote_addr,
                      size_t addrlen);

#endif
