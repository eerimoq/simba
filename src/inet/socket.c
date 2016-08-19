/**
 * @file socket.c
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

#include "simba.h"

/** TCP socket type. */
#define SOCKET_TYPE_STREAM     1

/** UDP socket type. */
#define SOCKET_TYPE_DGRAM      2

/** RAW socket type. */
#define SOCKET_TYPE_RAW        3

#if !defined(ARCH_LINUX)

#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/raw.h"

#if defined(ARCH_ESP)

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

extern xSemaphoreHandle thrd_idle_sem;

#endif

struct module_t {
    int initialized;
    struct fs_counter_t udp_rx_bytes;
    struct fs_counter_t udp_tx_bytes;
    struct fs_counter_t tcp_accepts;
    struct fs_counter_t tcp_rx_bytes;
    struct fs_counter_t tcp_tx_bytes;
    struct fs_counter_t raw_tx_bytes;
};

static struct module_t module;

static void init(struct socket_t *self_p,
                 int type,
                 void *pcb_p)
{
    /* Channel functions. */
    self_p->base.read = (thrd_read_fn_t)socket_read;
    self_p->base.write = (thrd_write_fn_t)socket_write;
    self_p->base.size = (thrd_size_fn_t)socket_size;

    self_p->type = type;
    self_p->pcb_p = pcb_p;
    self_p->io.recv.reading = 0;
    self_p->io.recv.pbuf.buf_p = NULL;
    self_p->io.recv.pbuf.size = -1;
    self_p->io.recv.pbuf.offset = 0;
}

static void resume_thrd(struct socket_t *socket_p)
{
    /* Resume the reading thread. */
    sys_lock();
    thrd_resume_isr((struct thrd_t *)socket_p->io.thrd_p, 0);
    socket_p->io.thrd_p = NULL;
    sys_unlock();

#if defined(ARCH_ESP)
    xSemaphoreGive(thrd_idle_sem);
#endif
}

/*
 * All callback functions below are called from the LwIP-thread. For
 * ESP, this is the FreeRTOS LwIP-thread.
 */

/**
 * TCP data is available in the lwip stack.
 */
static err_t on_tcp_recv(void *arg_p,
                         struct tcp_pcb *pcb_p,
                         struct pbuf *pbuf_p,
                         err_t err)
{
    struct socket_t *socket_p = arg_p;

    /* Ready for the next buffer? */
    if (socket_p->io.recv.pbuf.size != -1) {
        return (ERR_MEM);
    }

    /* Save the new buffer. */
    socket_p->io.recv.pbuf.buf_p = pbuf_p;

    if (pbuf_p == NULL) {
        socket_p->io.recv.pbuf.size = 0;
    } else {
        socket_p->io.recv.pbuf.size = pbuf_p->tot_len;
        tcp_recved(pcb_p, pbuf_p->tot_len);
    }

    /* Resume any waiting thread. */
    if ((socket_p->io.thrd_p != NULL)
        && (socket_p->io.recv.reading == 1)) {
        socket_p->io.recv.reading = 0;
        resume_thrd(socket_p);
    }

    return (ERR_OK);
}

/**
 * Open a TCP socket from the lwip thread.
 */
static void tcp_open_cb(void *ctx_p)
{
    struct socket_t *self_p = ctx_p;
    void *pcb_p;

    /* Create and initiate the TCP pcb. */
    pcb_p = tcp_new();
    tcp_arg(pcb_p, self_p);
    tcp_recv(pcb_p, on_tcp_recv);
    init(self_p, SOCKET_TYPE_STREAM, pcb_p);
    resume_thrd(self_p);
}

/**
 * Close a TCP socket from the lwip thread.
 */
static void tcp_close_cb(void *ctx_p)
{
    struct socket_t *self_p = ctx_p;

    tcp_close(self_p->pcb_p);
    resume_thrd(self_p);
}

/**
 * A TCP connection has been established using `connect()`.
 */
static err_t on_tcp_connected(void *arg_p,
                              struct tcp_pcb *pcb_p,
                              err_t err)
{
    struct socket_t *self_p = arg_p;

    self_p->io.size = err;
    resume_thrd(self_p);

    return (ERR_OK);
}

/**
 * A TCP client is trying to connect.
 */
static err_t on_tcp_accept(void *arg_p,
                           struct tcp_pcb *new_pcb_p,
                           err_t err)
{
    struct socket_t *socket_p = arg_p;
    struct socket_t *accepted_p = socket_p->io.buf_p;

    if (socket_p->io.thrd_p != NULL) {
        /* Initialize the new socket and accpet the client. */
        tcp_arg(new_pcb_p, accepted_p);
        tcp_recv(new_pcb_p, on_tcp_recv);
        init(accepted_p, SOCKET_TYPE_STREAM, new_pcb_p);
        tcp_accepted(((struct tcp_pcb *)socket_p->pcb_p));
        resume_thrd(socket_p);
    }

    return (ERR_OK);
}

/**
 * Write TCP data from the lwip thread.
 */
static void tcp_write_cb(void *ctx_p)
{
    struct socket_t *self_p = ctx_p;
    err_t res;

    res = tcp_write(self_p->pcb_p,
                    self_p->io.buf_p,
                    self_p->io.size,
                    TCP_WRITE_FLAG_COPY);

    if (res != ERR_OK) {
        self_p->io.size = -1;
    }

    resume_thrd(self_p);
}

/**
 * An UDP packet has been received.
 */
static void on_udp_recv(void *arg_p,
                        struct udp_pcb *pcb_p,
                        struct pbuf *pbuf_p,
                        ip_addr_t *addr_p,
                        uint16_t port)
{
    struct socket_t *socket_p = arg_p;

    if (socket_p->io.recv.pbuf.size == -1) {
        /* Save the new buffer. */
        socket_p->io.recv.pbuf.buf_p = pbuf_p;

        if (pbuf_p == NULL) {
            socket_p->io.recv.pbuf.size = 0;
        } else {
            socket_p->io.recv.pbuf.size = pbuf_p->tot_len;
        }

        /* Copy the remote address and port to the receiver. */
        socket_p->io.remote_addr.ip.number = addr_p->addr;
        socket_p->io.remote_addr.port = port;

        /* Resume any waiting thread. */
        if (socket_p->io.thrd_p != NULL) {
            resume_thrd(socket_p);
        }
    }
}

static ssize_t udp_send_to(struct socket_t *self_p,
                           const void *buf_p,
                           size_t size,
                           int flags,
                           const struct inet_addr_t *remote_addr_p)
{
    ssize_t res = size;
    struct pbuf *pbuf_p;
    ip_addr_t ip;

    /* Copy the data to a pbuf.*/
    ip.addr = remote_addr_p->ip.number;
    pbuf_p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
    memcpy(pbuf_p->payload, buf_p, size);

    fs_counter_increment(&module.udp_tx_bytes, size);

    if (udp_sendto(self_p->pcb_p,
                   pbuf_p,
                   &ip,
                   remote_addr_p->port) != 0) {
        res = -1;
    }

    pbuf_free(pbuf_p);

    return (res);
}

static ssize_t tcp_send_to(struct socket_t *self_p,
                           const void *buf_p,
                           size_t size,
                           int flags,
                           const struct inet_addr_t *remote_addr_p)
{
    self_p->io.buf_p = (void *)buf_p;
    self_p->io.size = size;
    self_p->io.thrd_p = thrd_self();

    tcpip_callback_with_block(tcp_write_cb, self_p, 0);
    thrd_suspend(NULL);

    if (self_p->io.size >= 0) {
        fs_counter_increment(&module.tcp_tx_bytes, self_p->io.size);
    }

    return (self_p->io.size);
}

static ssize_t udp_recv_from(struct socket_t *self_p,
                             void *buf_p,
                             size_t size,
                             int flags,
                             struct inet_addr_t *remote_addr_p)
{
    struct pbuf *pbuf_p;

    /* Wait for data if none is available. */
    if (self_p->io.recv.pbuf.size == -1) {
        self_p->io.thrd_p = thrd_self();
        thrd_suspend(NULL);
    }

    /* Socket closed. */
    if (self_p->io.recv.pbuf.size == 0) {
        return (0);
    }

    pbuf_p = (struct pbuf *)self_p->io.recv.pbuf.buf_p;

    /* Copy data from the pbuf to the local buffer. */
    if (size > pbuf_p->tot_len) {
        size = pbuf_p->tot_len;
    }

    *remote_addr_p = self_p->io.remote_addr;

    pbuf_copy_partial(pbuf_p, buf_p, size, 0);
    self_p->io.recv.pbuf.size = -1;
    pbuf_free(pbuf_p);

    fs_counter_increment(&module.udp_rx_bytes, size);

    return (size);
}

static ssize_t tcp_recv_from(struct socket_t *self_p,
                             void *buf_p,
                             size_t size,
                             int flags,
                             struct inet_addr_t *remote_addr_p)
{
    size_t left = size, pbuf_len, chunk_size;
    volatile struct pbuf *pbuf_p;

    while (left > 0) {
        /* Wait for data if none is available. */
        sys_lock();

        if (self_p->io.recv.pbuf.size == -1) {
            self_p->io.recv.reading = 1;
            self_p->io.thrd_p = thrd_self();
            thrd_suspend_isr(NULL);
        }

        sys_unlock();

        /* Socket closed. */
        if (self_p->io.recv.pbuf.size == 0) {
            break;
        }

        /* Copy data from the pbuf to the local buffer. */
        pbuf_p = self_p->io.recv.pbuf.buf_p;
        pbuf_len = (pbuf_p->tot_len - self_p->io.recv.pbuf.offset);

        if (pbuf_len > left) {
            chunk_size = left;
        } else {
            chunk_size = pbuf_len;
        }

        pbuf_copy_partial((struct pbuf *)pbuf_p,
                          buf_p,
                          chunk_size,
                          (size_t)self_p->io.recv.pbuf.offset);
        left -= chunk_size;
        buf_p += chunk_size;
        self_p->io.recv.pbuf.offset += chunk_size;

        /* Free the pbuf when all data has been read. */
        if (pbuf_len == chunk_size) {
            self_p->io.recv.pbuf.buf_p = NULL;
            self_p->io.recv.pbuf.size = -1;
            self_p->io.recv.pbuf.offset = 0;
            pbuf_free((struct pbuf *)pbuf_p);
        }
    }

    fs_counter_increment(&module.tcp_rx_bytes, size - left);

    return (size - left);
}

static ssize_t raw_recv_from(struct socket_t *self_p,
                             void *buf_p,
                             size_t size,
                             int flags,
                             struct inet_addr_t *remote_addr_p)
{
    struct pbuf *pbuf_p;
    
    /* Wait for data if none is available. */
    sys_lock();
    
    if (self_p->io.recv.pbuf.size == -1) {
        self_p->io.thrd_p = thrd_self();
        thrd_suspend_isr(NULL);
    }
    
    sys_unlock();

    /* Socket closed. */
    if (self_p->io.recv.pbuf.size <= 0) {
        return (self_p->io.recv.pbuf.size);
    }
    
    pbuf_p = (struct pbuf *)self_p->io.recv.pbuf.buf_p;

    /* Copy data from the pbuf to the local buffer. */
    if (size > pbuf_p->tot_len) {
        size = pbuf_p->tot_len;
    }

    *remote_addr_p = self_p->io.remote_addr;
    
    pbuf_copy_partial(pbuf_p, buf_p, size, 0);
    self_p->io.recv.pbuf.size = -1;
    pbuf_free(pbuf_p);

    return (size);
}

/**
 * An RAW packet has been received.
 */
static uint8_t on_raw_recv(void *arg_p,
                           struct raw_pcb *pcb_p,
                           struct pbuf *pbuf_p,
                           ip_addr_t *addr_p)
{
    struct socket_t *socket_p = arg_p;

    if (socket_p->io.recv.pbuf.size == -1) {
        /* Resume any polling thread. */
        sys_lock();
        
        if (chan_is_polled_isr(&socket_p->base)) {
            thrd_resume_isr(socket_p->base.reader_p, 0);
            socket_p->base.reader_p = NULL;
        }
        
        /* Save the new buffer. */
        socket_p->io.recv.pbuf.buf_p = pbuf_p;

        if (pbuf_p == NULL) {
            socket_p->io.recv.pbuf.size = 0;
        } else {
            socket_p->io.recv.pbuf.size = pbuf_p->tot_len;
        }

        /* Copy the remote address to the receiver. */
        socket_p->io.remote_addr.ip.number = addr_p->addr;

        sys_unlock();

        /* Resume any waiting thread. */
        if (socket_p->io.thrd_p != NULL) {
            resume_thrd(socket_p);
        }
    }
    
    return (1);
}

static void raw_write_cb(void *ctx_p)
{
    struct socket_t *self_p = ctx_p;
    err_t res = ERR_MEM;
    struct pbuf *pbuf_p;
    ip_addr_t ip;

    ip.addr = self_p->io.remote_addr.ip.number;
    pbuf_p = pbuf_alloc(PBUF_IP, self_p->io.size, PBUF_RAM);

    if (pbuf_p != NULL) {
        memcpy(pbuf_p->payload, self_p->io.buf_p, self_p->io.size);
        res = raw_sendto(self_p->pcb_p, pbuf_p, &ip);
        pbuf_free(pbuf_p);
    }
    
    if (res != ERR_OK) {
        self_p->io.size = -1;
    }

    resume_thrd(self_p);
}

static ssize_t raw_send_to(struct socket_t *self_p,
                           const void *buf_p,
                           size_t size,
                           int flags,
                           const struct inet_addr_t *remote_addr_p)
{
    self_p->io.buf_p = (void *)buf_p;
    self_p->io.size = size;
    self_p->io.remote_addr = *remote_addr_p;
    self_p->io.thrd_p = thrd_self();

    tcpip_callback_with_block(raw_write_cb, self_p, 0);
    thrd_suspend(NULL);

    if (self_p->io.size >= 0) {
        fs_counter_increment(&module.raw_tx_bytes, self_p->io.size);
    }

    return (self_p->io.size);
}

int socket_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    /* UDP counters. */
    fs_counter_init(&module.udp_rx_bytes,
                    FSTR("/inet/socket/udp/rx_bytes"),
                    0);
    fs_counter_register(&module.udp_rx_bytes);

    fs_counter_init(&module.udp_tx_bytes,
                    FSTR("/inet/socket/udp/tx_bytes"),
                    0);
    fs_counter_register(&module.udp_tx_bytes);

    /* TCP counters. */
    fs_counter_init(&module.tcp_accepts,
                    FSTR("/inet/socket/tcp/accepts"),
                    0);
    fs_counter_register(&module.tcp_accepts);

    fs_counter_init(&module.tcp_rx_bytes,
                    FSTR("/inet/socket/tcp/rx_bytes"),
                    0);
    fs_counter_register(&module.tcp_rx_bytes);

    fs_counter_init(&module.tcp_tx_bytes,
                    FSTR("/inet/socket/tcp/tx_bytes"),
                    0);
    fs_counter_register(&module.tcp_tx_bytes);

    fs_counter_init(&module.raw_tx_bytes,
                    FSTR("/inet/socket/raw/tx_bytes"),
                    0);
    fs_counter_register(&module.raw_tx_bytes);

#if !defined(ARCH_ESP)
    /* Initialize the LwIP stack. */
    tcpip_init(NULL, NULL);
#endif

    return (0);
}

int socket_open_tcp(struct socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->io.thrd_p = thrd_self();
    tcpip_callback_with_block(tcp_open_cb, self_p, 0);
    thrd_suspend(NULL);

    return (0);
}

int socket_open_udp(struct socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    void *pcb_p = NULL;

    /* Create and initiate the UDP pcb. */
    pcb_p = udp_new();

    if (pcb_p == NULL) {
        return (-1);
    }

    udp_recv(pcb_p, on_udp_recv, self_p);
    init(self_p, SOCKET_TYPE_DGRAM, pcb_p);

    return (0);
}

int socket_open_raw(struct socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    void *pcb_p = NULL;

    /* Create and initiate the UDP pcb. */
    pcb_p = raw_new(IP_PROTO_ICMP);

    if (pcb_p == NULL) {
        return (-1);
    }

    raw_recv(pcb_p, on_raw_recv, self_p);
    init(self_p, SOCKET_TYPE_RAW, pcb_p);
    
    return (0);
}

int socket_close(struct socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        self_p->io.thrd_p = thrd_self();
        tcpip_callback_with_block(tcp_close_cb, self_p, 0);
        thrd_suspend(NULL);
        break;

    case SOCKET_TYPE_DGRAM:
        udp_recv(self_p->pcb_p, NULL, NULL);
        udp_remove(self_p->pcb_p);
        break;

    case SOCKET_TYPE_RAW:
        raw_recv(self_p->pcb_p, NULL, NULL);
        raw_remove(self_p->pcb_p);
        break;

    default:
        return (-1);
    }

    return (0);
}

int socket_bind(struct socket_t *self_p,
                const struct inet_addr_t *local_addr_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(local_addr_p != NULL, EINVAL);

    ip_addr_t ip;

    ip.addr = local_addr_p->ip.number;

    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        return (tcp_bind(self_p->pcb_p, &ip, local_addr_p->port));

    case SOCKET_TYPE_DGRAM:
        return (udp_bind(self_p->pcb_p, &ip, local_addr_p->port));

    default:
        return (-1);
    }
}

int socket_listen(struct socket_t *self_p, int backlog)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(backlog >= 0, EINVAL);

    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        self_p->pcb_p = tcp_listen_with_backlog(self_p->pcb_p, backlog);
        tcp_accept(self_p->pcb_p, on_tcp_accept);
        break;

    default:
        return (-1);
    }

    return (0);
}

int socket_connect(struct socket_t *self_p,
                   const struct inet_addr_t *remote_addr_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(remote_addr_p != NULL, EINVAL);

    ip_addr_t ip;

    ip.addr = remote_addr_p->ip.number;

    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        if (tcp_connect(self_p->pcb_p,
                        &ip,
                        remote_addr_p->port,
                        on_tcp_connected) != ERR_OK) {
            return (-1);
        }
        
        self_p->io.thrd_p = thrd_self();
        thrd_suspend(NULL);
        
        return (self_p->io.size == ERR_OK ? 0 : -1);
        
    case SOCKET_TYPE_DGRAM:
        return (udp_connect(self_p->pcb_p, &ip, remote_addr_p->port));

    default:
        return (-1);
    }

}

int socket_connect_by_hostname(struct socket_t *self_p,
                               const char *hostname_p,
                               uint16_t port)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(hostname_p != NULL, EINVAL);
    ASSERTN(self_p->type == SOCKET_TYPE_STREAM, EINVAL);

    struct inet_ip_addr_t ip2;
    ip_addr_t ip;

    if (inet_aton("216.58.211.142", &ip2) != 0) {
        return (-1);
    }

    ip.addr = ip2.number;

    if (tcp_connect(self_p->pcb_p, &ip, port, on_tcp_connected) != ERR_OK) {
        return (-1);
    }
    
    self_p->io.thrd_p = thrd_self();
    thrd_suspend(NULL);
        
    return (self_p->io.size == ERR_OK ? 0 : -1);
}

int socket_accept(struct socket_t *self_p,
                  struct socket_t *accepted_p,
                  struct inet_addr_t *addr_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(accepted_p != NULL, EINVAL);

    self_p->io.buf_p = accepted_p;
    self_p->io.thrd_p = thrd_self();
    thrd_suspend(NULL);
    fs_counter_increment(&module.tcp_accepts, 1);

    return (0);
}

ssize_t socket_sendto(struct socket_t *self_p,
                      const void *buf_p,
                      size_t size,
                      int flags,
                      const struct inet_addr_t *remote_addr_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        return (tcp_send_to(self_p,
                            buf_p,
                            size,
                            flags,
                            remote_addr_p));

    case SOCKET_TYPE_DGRAM:
        return (udp_send_to(self_p,
                            buf_p,
                            size,
                            flags,
                            remote_addr_p));

    case SOCKET_TYPE_RAW:
        return (raw_send_to(self_p,
                            buf_p,
                            size,
                            flags,
                            remote_addr_p));

    default:
        return (-1);
    }
}

ssize_t socket_recvfrom(struct socket_t *self_p,
                        void *buf_p,
                        size_t size,
                        int flags,
                        struct inet_addr_t *remote_addr_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);
    
    switch (self_p->type) {

    case SOCKET_TYPE_STREAM:
        return (tcp_recv_from(self_p,
                              buf_p,
                              size,
                              flags,
                              remote_addr_p));

    case SOCKET_TYPE_DGRAM:
        return (udp_recv_from(self_p,
                              buf_p,
                              size,
                              flags,
                              remote_addr_p));

    case SOCKET_TYPE_RAW:
        return (raw_recv_from(self_p,
                              buf_p,
                              size,
                              flags,
                              remote_addr_p));

    default:
        return (-1);
    }
}

ssize_t socket_write(struct socket_t *self_p,
                     const void *buf_p,
                     size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (socket_sendto(self_p, buf_p, size, 0, NULL));
}

ssize_t socket_read(struct socket_t *self_p,
                    void *buf_p,
                    size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (socket_recvfrom(self_p, buf_p, size, 0, NULL));
}

ssize_t socket_size(struct socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    
    return (self_p->io.recv.pbuf.size > -1);
}

#else

int socket_module_init(void)
{
    return (0);
}

int socket_open_tcp(struct socket_t *self_p)
{
    return (-1);
}

int socket_open_udp(struct socket_t *self_p)
{
    return (-1);
}

int socket_open_raw(struct socket_t *self_p)
{
    return (-1);
}

int socket_close(struct socket_t *self_p)
{
    return (-1);
}

int socket_bind(struct socket_t *self_p,
                const struct inet_addr_t *local_addr_p)
{
    return (-1);
}

int socket_listen(struct socket_t *self_p, int backlog)
{
    return (-1);
}

int socket_connect(struct socket_t *self_p,
                   const struct inet_addr_t *addr_p)
{
    return (-1);
}

int socket_accept(struct socket_t *self_p,
                  struct socket_t *accepted_p,
                  struct inet_addr_t *addr_p)
{
    return (-1);
}

ssize_t socket_sendto(struct socket_t *self_p,
                      const void *buf_p,
                      size_t size,
                      int flags,
                      const struct inet_addr_t *remote_addr_p)
{
    return (-1);
}

ssize_t socket_recvfrom(struct socket_t *self_p,
                        void *buf_p,
                        size_t size,
                        int flags,
                        struct inet_addr_t *remote_addr_p)
{
    return (-1);
}

ssize_t socket_write(struct socket_t *self_p,
                     const void *buf_p,
                     size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (socket_sendto(self_p, buf_p, size, 0, NULL));
}

ssize_t socket_read(struct socket_t *self_p,
                    void *buf_p,
                    size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (socket_recvfrom(self_p, buf_p, size, 0, NULL));
}

ssize_t socket_size(struct socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (-1);
}

#endif
