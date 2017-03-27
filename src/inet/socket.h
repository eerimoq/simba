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

#ifndef __INET_SOCKET_H__
#define __INET_SOCKET_H__

#include "simba.h"

#define SOCKET_DOMAIN_INET     0

/** TCP socket type. */
#define SOCKET_TYPE_STREAM     1

/** UDP socket type. */
#define SOCKET_TYPE_DGRAM      2

/** RAW socket type. */
#define SOCKET_TYPE_RAW        3

#define SOCKET_PROTO_ICMP      0

struct socket_t {
    struct chan_t base;
    int type;
    struct {
        union {
            struct {
                ssize_t left;
            } common;
            struct {
                ssize_t left; /* Number of bytes left to read or -1 if the
                                 connection is closed. */
                struct pbuf *pbuf_p;
                struct inet_addr_t remote_addr;
                int closed;
            } recvfrom;
            struct {
                ssize_t left;
                struct tcp_pcb *pcb_p;
            } accept;
        } u;
        struct {
            int state;
            void *args_p;
            struct thrd_t *thrd_p;
        } cb;
    } input;
    struct {
        struct {
            int state;
            void *args_p;
            struct thrd_t *thrd_p;
        } cb;
    } output;
    void *pcb_p;
};

/**
 * Initialize the socket module. This function will start the lwIP
 * TCP/IP stack. This function must be called before calling any other
 * function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
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
 * Initialize given RAW socket.
 *
 * @param[out] self_p Socket to initialize.
 *
 * @return zero(0) or negative error code.
 */
int socket_open_raw(struct socket_t *self_p);

/**
 * Initialize given socket.
 *
 * @param[out] self_p Socket to initialize.
 * @param[in] domain Socket domain.
 * @param[in] type Socket type.
 * @param[in] protocol Socket protocol.
 *
 * @return zero(0) or negative error code.
 */
int socket_open(struct socket_t *self_p,
                int domain,
                int type,
                int protocol);

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

/**
 * Get the number of input bytes currently stored in the socket. May
 * return less bytes than number of bytes stored in the channel.
 *
 * @param[in] self_p Socket.
 *
 * @return Number of input bytes in the socket.
 */
ssize_t socket_size(struct socket_t *self_p);

#endif
