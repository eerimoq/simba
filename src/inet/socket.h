/**
 * @file inet/socket.h
 * @version 4.1.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
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

#ifndef __INET_SOCKET_H__
#define __INET_SOCKET_H__

#include "simba.h"

struct socket_t {
    struct chan_t base;
    int type;
    struct {
        struct {
            volatile int reading;
            struct {
                void * volatile buf_p;
                volatile size_t size;
                volatile size_t offset;
            } pbuf;
        } recv;
        void *buf_p;
        size_t size;
        struct inet_addr_t remote_addr;
        struct thrd_t * volatile thrd_p;
    } io;
    void *pcb_p;
};

/**
 * Initialize the socket module. This function will start the lwIP
 * TCP/IP stack.
 *
 * @return zero(0) or negative error code.
 */
int socket_module_init(void);

/**
 * Initialize given TCP socket.
 *
 * @param[out] self_p Socket to initialize.
 *
 * @return zero(0) or negative error code.
 */
int socket_open_tcp(struct socket_t *self_p);

/**
 * Initialize given UDP socket.
 *
 * @param[out] self_p Socket to initialize.
 *
 * @return zero(0) or negative error code.
 */
int socket_open_udp(struct socket_t *self_p);

/**
 * Close given socket. No data transfers are allowed on after the
 * socket has been closed.
 *
 * @param[in] self_p Socket to close.
 *
 * @return zero(0) or negative error code.
 */
int socket_close(struct socket_t *self_p);

/**
 * Bind given local address to given socket.
 *
 * @param[in] self_p Socket.
 * @param[in] local_addr_p Local address.
 *
 * @return zero(0) or negative error code.
 */
int socket_bind(struct socket_t *self_p,
                const struct inet_addr_t *local_addr_p);

/**
 * Listen for connections from remote clients. Only applicable for TCP
 * sockets.
 *
 * @param[in] self_p Socket to listen on.
 * @param[in] backlog Unused.
 *
 * @return zero(0) or negative error code.
 */
int socket_listen(struct socket_t *self_p, int backlog);

/**
 * Connect to given remote address. Connecting a UDP socket sets the
 * default remote address for outgoing datagrams. For TCP a three-way
 * handshake with the remote peer is initiated.
 *
 * @param[in] self_p Socket.
 * @param[in] remote_addr_p Remote address.
 *
 * @return zero(0) or negative error code.
 */
int socket_connect(struct socket_t *self_p,
                   const struct inet_addr_t *remote_addr_p);

/**
 * Connect to the remote device with given hostname.
 *
 * In computer networking, a hostname (archaically nodename) is a
 * label that is assigned to a device connected to a computer network
 * and that is used to identify the device in various forms of
 * electronic communication, such as the World Wide Web.
 *
 * @param[in] self_p Socket.
 * @param[in] hostname_p The hostname of the remote device to connect
 *                       to.
 * @param[in] port Remote device port to connect to.
 *
 * @return zero(0) or negative error code.
 */
int socket_connect_by_hostname(struct socket_t *self_p,
                               const char *hostname_p,
                               uint16_t port);

/**
 * Accept a client connect attempt. Only applicable for TCP sockets
 * that are listening for connections.
 *
 * @param[in] self_p TCP socket.
 * @param[out] accepted_p New client socket of the accepted client.
 * @param[out] remote_addr_p Address of the client.
 *
 * @return zero(0) or negative error code.
 */
int socket_accept(struct socket_t *self_p,
                  struct socket_t *accepted_p,
                  struct inet_addr_t *remote_addr_p);

/**
 * Write data to given socket. Only used by UDP sockets.
 *
 * @param[in] self_p Socket to send data on.
 * @param[in] buf_p Buffer to send.
 * @param[in] size Size of buffer to send.
 * @param[in] flags Unused.
 * @param[in] remote_addr_p Remote address to send the data to.
 *
 * @return Number of sent bytes or negative error code.
 */
ssize_t socket_sendto(struct socket_t *self_p,
                      const void *buf_p,
                      size_t size,
                      int flags,
                      const struct inet_addr_t *remote_addr_p);

/**
 * Read data from given socket. Only used by UDP sockets.
 *
 * @param[in] self_p Socket to receive data on.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Size of buffer to read.
 * @param[in] flags Unused.
 * @param[in] remote_addr_p Remote address to receive data from.
 *
 * @return Number of received bytes or negative error code.
 */
ssize_t socket_recvfrom(struct socket_t *self_p,
                        void *buf_p,
                        size_t size,
                        int flags,
                        struct inet_addr_t *remote_addr_p);

/**
 * Write data to given TCP or UDP socket. For UDP sockets,
 * ``socket_connect()`` must have been called prior to calling this
 * function.
 *
 * @param[in] self_p Socket.
 * @param[in] buf_p Buffer to send.
 * @param[in] size Numer of bytes to send.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t socket_write(struct socket_t *self_p,
                     const void *buf_p,
                     size_t size);

/**
 * Read data from given socket.
 *
 * @param[in] self_p Socket.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t socket_read(struct socket_t *self_p,
                    void *buf_p,
                    size_t size);

#endif
