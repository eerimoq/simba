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

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#include "stdlib.h"

/* These defines should not be changed by the user. The Simba port of
 * the lwIP stack requires them to have given values. */
#define NO_SYS                      0
#define SYS_LIGHTWEIGHT_PROT        1
#define LWIP_SOCKET                 0
#define LWIP_COMPAT_SOCKETS         0
#define LWIP_POSIX_SOCKETS_IO_NAMES 0
#define LWIP_PREFIX_BYTEORDER_FUNCS

/* Architecture defiens that the user hopefully does not have to
 * change, but may redefine. */

#ifndef TCPIP_THREAD_NAME
#    define TCPIP_THREAD_NAME           "tcpip"
#endif

#ifndef TCPIP_THREAD_STACKSIZE
#    define TCPIP_THREAD_STACKSIZE      1024
#endif

#ifndef TCPIP_MBOX_SIZE
#    define TCPIP_MBOX_SIZE             8
#endif

#ifndef MEM_ALIGNMENT
#    define MEM_ALIGNMENT               4
#endif

#ifndef LWIP_RAND
#    define LWIP_RAND()                 rand()
#endif

#ifndef LWIP_CHECKSUM_ON_COPY
#    define LWIP_CHECKSUM_ON_COPY       0
#endif

/* TCP/IP stack configuration. */

#ifndef LWIP_RAW
#    define LWIP_RAW                    1
#endif

#ifndef DEFAULT_TCP_RECVMBOX_SIZE
#    define DEFAULT_TCP_RECVMBOX_SIZE   8
#endif

#ifndef DEFAULT_UDP_RECVMBOX_SIZE
#    define DEFAULT_UDP_RECVMBOX_SIZE   8
#endif

#ifndef DEFAULT_RAW_RECVMBOX_SIZE
#    define DEFAULT_RAW_RECVMBOX_SIZE   8
#endif

#ifndef DEFAULT_ACCEPTMBOX_SIZE
#    define DEFAULT_ACCEPTMBOX_SIZE     8
#endif

#ifndef MEM_SIZE
#    define MEM_SIZE                    8000
#endif

#ifndef MEMP_NUM_SYS_TIMEOUT
#    define MEMP_NUM_SYS_TIMEOUT        16
#endif

#ifndef PBUF_POOL_SIZE
#    define PBUF_POOL_SIZE              8
#endif

#ifndef MEMP_NUM_TCP_PCB_LISTEN
#    define MEMP_NUM_TCP_PCB_LISTEN     4
#endif

#ifndef MEMP_NUM_TCP_PCB
#    define MEMP_NUM_TCP_PCB            4
#endif

#ifndef MEMP_NUM_UDP_PCB
#    define MEMP_NUM_UDP_PCB            4
#endif

#ifndef MEMP_NUM_PBUF
#    define MEMP_NUM_PBUF               8
#endif

#ifndef MEMP_NUM_NETBUF
#    define MEMP_NUM_NETBUF             8
#endif

#ifndef TCP_QUEUE_OOSEQ
#    define TCP_QUEUE_OOSEQ             0
#endif

#ifndef TCP_OVERSIZE
#    define TCP_OVERSIZE                0
#endif

#ifndef LWIP_DHCP
#    define LWIP_DHCP                   1
#endif

#ifndef LWIP_DNS
#    define LWIP_DNS                    1
#endif

#ifndef LWIP_IGMP
#    define LWIP_IGMP                   1
#endif

#ifndef LWIP_SO_RCVTIMEO
#    define LWIP_SO_RCVTIMEO            1
#endif

#ifndef LWIP_TCP_KEEPALIVE
#    define LWIP_TCP_KEEPALIVE          1
#endif

#ifndef TCP_MSS
#    define TCP_MSS                     1460
#endif

#ifndef TCP_SND_BUF
#    define TCP_SND_BUF                 (2 * TCP_MSS)
#endif

#ifndef TCP_WND
#    define TCP_WND                     (2 * TCP_MSS)
#endif

#ifndef IP_SOF_BROADCAST
#    define IP_SOF_BROADCAST            1
#endif

#ifndef IP_SOF_BROADCAST_RECV
#    define IP_SOF_BROADCAST_RECV       1
#endif

#ifndef LWIP_BROADCAST_PING
#    define LWIP_BROADCAST_PING         1
#endif

#ifndef LWIP_NETIF_HOSTNAME
#    define LWIP_NETIF_HOSTNAME         1
#endif

#ifndef LWIP_NETIF_STATUS_CALLBACK
#    define LWIP_NETIF_STATUS_CALLBACK  1
#endif

#ifndef LWIP_NETIF_LINK_CALLBACK
#    define LWIP_NETIF_LINK_CALLBACK    1
#endif


/* Debugging. */

#ifndef LWIP_NOASSERT
#    define LWIP_NOASSERT               1
#endif

#ifndef LWIP_STATS
#    define LWIP_STATS                  0
#endif

#endif
