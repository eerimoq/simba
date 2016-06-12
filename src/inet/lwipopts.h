/**
 * @file lwipopts.h
 * @version 0.6.0
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

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define NO_SYS                      0

#define SYS_LIGHTWEIGHT_PROT        1

#define LWIP_RAW                    0

#define TCPIP_MBOX_SIZE             8
#define DEFAULT_TCP_RECVMBOX_SIZE   8
#define DEFAULT_UDP_RECVMBOX_SIZE   8
#define DEFAULT_RAW_RECVMBOX_SIZE   8
#define DEFAULT_ACCEPTMBOX_SIZE     8

#define TCPIP_THREAD_STACKSIZE      1024
#define TCPIP_THREAD_PRIO           0

#define MEMP_NUM_SYS_TIMEOUT        16

#define MEM_ALIGNMENT               4

#define PBUF_POOL_SIZE              5
#define MEMP_NUM_TCP_PCB_LISTEN     4
#define MEMP_NUM_TCP_PCB            4
#define MEMP_NUM_UDP_PCB            4
#define MEMP_NUM_PBUF               8
#define MEMP_NUM_NETBUF             8

#define TCP_QUEUE_OOSEQ             0
#define TCP_OVERSIZE                0

#define LWIP_DHCP                   1
#define LWIP_DNS                    1

#include "stdlib.h"
#define LWIP_IGMP                   1
#define LWIP_RAND()                 rand()

#define LWIP_SOCKET                 0

#define LWIP_COMPAT_SOCKETS         0
#define LWIP_POSIX_SOCKETS_IO_NAMES 0
#define LWIP_SO_RCVTIMEO            1
#define LWIP_TCP_KEEPALIVE          1

#define LWIP_NOASSERT               1
#define LWIP_STATS                  0

#define LWIP_PLATFORM_BYTESWAP      1

/* MSS should match the hardware packet size */
#define TCP_MSS                     1460
#define TCP_SND_BUF                 (2 * TCP_MSS)
#define TCP_WND                     (2 * TCP_MSS)
//#define TCP_SND_QUEUELEN            (2 * TCP_SND_BUF/TCP_MSS)

// Broadcast
#define IP_SOF_BROADCAST            1
#define IP_SOF_BROADCAST_RECV       1

#define LWIP_BROADCAST_PING         1

#define LWIP_CHECKSUM_ON_COPY       1

#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

#define LWIP_PREFIX_BYTEORDER_FUNCS

#endif /* LWIPOPTS_H_ */
