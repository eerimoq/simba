/**
 * @file etharp.i
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

#define HWTYPE_ETHERNET 1

#define ARP_OP_REQUEST 0x1
#define ARP_OP_REPLY   0x2

#define PROTO_IP 0x0800

struct ipmac_t {
    uint32_t ip;
    uint8_t mac[ETH_MAX];
};

static struct ipmac_t cache[INET_ARP_CACHE_MAX];

/**
 * Find MAC address for given IP address.
 * @param[in] daddr destination IP address
 * @param[out] dmac destination MAC address
 * @return zero(0) if MAC address found, or negative error code
 */
int inet_etharp_lookup(uint32_t daddr, uint8_t *dmac)
{
    /* Search in cache.*/
    struct ipmac_t *im = cache;

    for (im = cache; im < &cache[YIP_ARP_CACHE_MAX]; im++) {
        if (im->ip == daddr) {
            memcpy(dmac, im->mac, sizeof(im->mac));
            return (0);
        }
    }

    return (-1);
}

/**
 * Build ARP request header.
 * @param[out] request built request header
 * @param[in] daddr destination IP address
 * @param[in] saddr source IP address
 * @param[in] smac source MAC address
 * @return zero(0) or negative error code
 */
int inet_etharp_request(struct etharp_header_t *header,
                        uint32_t daddr,
                        uint32_t saddr,
                        uint8_t *smac)
{
    header->hwtype = htons(HWTYPE_ETHERNET);
    header->proto = htons(PROTO_IP);
    header->hwlen = sizeof(header->smacaddr);
    header->protolen = sizeof(header->sipaddr);
    header->opcode = htons(ARP_OP_REQUEST);
    memcpy(header->smacaddr, smac, sizeof(header->smacaddr));
    header->sipaddr = saddr;
    memset(header->dmacaddr, 0, sizeof(header->dmacaddr));
    header->dipaddr = daddr;

    return (0);
}

/**
 * Handle ARP request/reply.
 * @return true(1) if reply was built, false(0) or negative error code
 */
int inet_etharp_input(struct etharp_header_t *header,
                      uint32_t lipaddr,
                      uint8_t *lmac)
{
    int res = 0;
    int i;
    uint8_t *mac;
    uint32_t ip;

    if ((htons(header->opcode) == ARP_OP_REQUEST)
        && (header->dipaddr == lipaddr)) {
        /* Build ARP reply.*/
        header->opcode = htons(ARP_OP_REPLY);
        memcpy(header->dmacaddr, header->smacaddr, sizeof(header->dmacaddr));
        memcpy(header->smacaddr, lmac, sizeof(header->smacaddr));
        header->dipaddr = header->sipaddr;
        header->sipaddr = lipaddr;
        res = 1;
    } else if (htons(header->opcode) == ARP_OP_REPLY) {
        mac = header->smacaddr;
        ip = header->sipaddr;
        /* chprintf((BaseSequentialStream *)&SD1, */
        /*          PSTR("ARP: Adding %u.%u.%u.%u %x:%x:%x:%x:%x:%x!\r\n"), */
        /*          (int)(ip & 0xff), (int)((ip >> 8) & 0xff), */
        /*          (int)((ip >> 16) & 0xff), (int)((ip >> 24) & 0xff), */
        /*          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]); */
        /* Add MAC as first entry in cache. Remove oldest entry.*/

        for (i = 0; i < YIP_ARP_CACHE_MAX - 1; i++) {
            cache[i+1] = cache[i];
        }

        memcpy(cache[0].mac, mac, sizeof(cache[0].mac));
        cache[0].ip = ip;
    }

    return (res);
}
